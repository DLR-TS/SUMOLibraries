/*

 shapefiles.c -- implements shapefile support [import - export]

 version 5.0, 2020 August 1

 Author: Sandro Furieri a.furieri@lqt.it

 -----------------------------------------------------------------------------
 
 Version: MPL 1.1/GPL 2.0/LGPL 2.1
 
 The contents of this file are subject to the Mozilla Public License Version
 1.1 (the "License"); you may not use this file except in compliance with
 the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/
 
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

The Original Code is the SpatiaLite library

The Initial Developer of the Original Code is Alessandro Furieri
 
Portions created by the Initial Developer are Copyright (C) 2008-2021
the Initial Developer. All Rights Reserved.

Contributor(s): Brad Hards <bradh@frogmouth.net>

Alternatively, the contents of this file may be used under the terms of
either the GNU General Public License Version 2 or later (the "GPL"), or
the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
in which case the provisions of the GPL or the LGPL are applicable instead
of those above. If you wish to allow use of your version of this file only
under the terms of either the GPL or the LGPL, and not to allow others to
use your version of this file under the terms of the MPL, indicate your
decision by deleting the provisions above and replace them with the notice
and other provisions required by the GPL or the LGPL. If you do not delete
the provisions above, a recipient may use your version of this file under
the terms of any one of the MPL, the GPL or the LGPL.
 
*/

#if defined(_WIN32) && !defined(__MINGW32__)
/* MSVC strictly requires this include [off_t] */
#include <sys/types.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#if defined(_WIN32) && !defined(__MINGW32__)
#include "config-msvc.h"
#else
#include "config.h"
#endif

#include <spatialite/sqlite.h>
#include <spatialite/debug.h>

#include <spatialite/gaiaaux.h>
#include <spatialite/gaiageo.h>
#include <spatialite.h>
#include <spatialite_private.h>
#include <spatialite/geojson.h>

#ifndef OMIT_FREEXL
#include <freexl.h>
#endif

#ifndef OMIT_PROJ		/* including PROJ */
#ifdef PROJ_NEW			/* supporting PROJ.6 */
#include <proj.h>
#endif
#endif

#ifdef ENABLE_MINIZIP		/* MINIZIP is enabled */
#include <minizip/unzip.h>
#endif

#if defined(_WIN32) && !defined(__MINGW32__)
#define strcasecmp	_stricmp
#define strncasecmp	_strnicmp
#endif

/* 64 bit integer: portable format for printf() */
#if defined(_WIN32) && !defined(__MINGW32__)
#define FRMT64 "%I64d"
#else
#define FRMT64 "%lld"
#endif

#define GAIA_ZIPFILE_SHP	1
#define GAIA_ZIPFILE_SHX	2
#define GAIA_ZIPFILE_DBF	3
#define GAIA_ZIPFILE_PRJ	4

struct auxdbf_fld
{
/* auxiliary DBF field struct */
    char already_used;
    gaiaDbfFieldPtr dbf_field;
    struct auxdbf_fld *next;
};

struct auxdbf_list
{
/* auxiliary DBF struct */
    struct auxdbf_fld *first;
    struct auxdbf_fld *last;
};

struct resultset_values
{
/* a struct wrapping values from a resultset */
    int type;
    sqlite3_int64 int_value;
    double dbl_value;
    unsigned char *txt_blob_value;
    int txt_blob_size;
};

struct resultset_comparator
{
/* object for comparing two rows of the same resultset */
    struct resultset_values *previous;
    struct resultset_values *current;
    int num_columns;
    sqlite3_int64 previous_rowid;
    sqlite3_int64 current_rowid;
};

struct aux_elemgeom_ignore
{
/* an Elementary Geometries Ignore option */
    char *column;
    struct aux_elemgeom_ignore *next;
};

struct aux_elemgeom_options
{
/* Elementary Geometries Options */
    struct aux_elemgeom_ignore *first;
    struct aux_elemgeom_ignore *last;
    int cast2multi;
};

struct zip_mem_shapefile
{
/* a struct wrapping a Memory Shapefile from Zipfile */
    gaiaMemFile shp;
    gaiaMemFile shx;
    gaiaMemFile dbf;
    gaiaMemFile prj;
};

struct zip_mem_shp_item
{
/* a Shapefile within a Zipfile */
    char *basename;
    int shp;
    int shx;
    int dbf;
    int prj;
    struct zip_mem_shp_item *next;
};

struct zip_mem_shp_list
{
/* a list of Shapefiles within a Zipfile */
    struct zip_mem_shp_item *first;
    struct zip_mem_shp_item *last;
};

SPATIALITE_PRIVATE const void *
gaiaElemGeomOptionsCreate ()
{
/* creating an Elementary Geometries Options object */
    struct aux_elemgeom_options *options =
	malloc (sizeof (struct aux_elemgeom_options));
    options->first = NULL;
    options->last = NULL;
    options->cast2multi = 0;
    return options;
}

SPATIALITE_PRIVATE void
gaiaElemGeomOptionsDestroy (const void *opts)
{
/* destroying an Elementary Geometries Options object */
    struct aux_elemgeom_ignore *ign;
    struct aux_elemgeom_ignore *ign2;
    struct aux_elemgeom_options *options = (struct aux_elemgeom_options *) opts;
    if (options == NULL)
	return;

    ign = options->first;
    while (ign != NULL)
      {
	  ign2 = ign->next;
	  if (ign->column != NULL)
	      free (ign->column);
	  free (ign);
	  ign = ign2;
      }
    free (options);
}

static void
ignore_column (struct aux_elemgeom_options *options, const char *column)
{
/* marking a Column to be ignored */
    int len;
    struct aux_elemgeom_ignore *ign;
    if (options == NULL)
	return;

    ign = malloc (sizeof (struct aux_elemgeom_ignore));
    len = strlen (column);
    ign->column = malloc (len + 1);
    strcpy (ign->column, column);
    ign->next = NULL;

    if (options->first == NULL)
	options->first = ign;
    if (options->last != NULL)
	options->last->next = ign;
    options->last = ign;
}

SPATIALITE_PRIVATE void
gaiaElemGeomOptionsAdd (const void *opts, const char *option)
{
/* adding an Elementary Geometries Option */
    struct aux_elemgeom_options *options = (struct aux_elemgeom_options *) opts;
    if (options == NULL)
	return;

    if (strncasecmp (option, "::cast2multi::", 14) == 0)
	options->cast2multi = 1;
    if (strncasecmp (option, "::ignore::", 10) == 0)
	ignore_column (options, option + 10);
}

static struct resultset_comparator *
create_resultset_comparator (int columns)
{
/* creating an empty resultset comparator object */
    int i;
    struct resultset_comparator *p =
	malloc (sizeof (struct resultset_comparator));
    p->num_columns = columns;
    p->previous_rowid = -1;
    p->current_rowid = -1;
    p->previous = malloc (sizeof (struct resultset_values) * columns);
    p->current = malloc (sizeof (struct resultset_values) * columns);
    for (i = 0; i < columns; i++)
      {
	  struct resultset_values *value = p->previous + i;
	  value->type = SQLITE_NULL;
	  value->txt_blob_value = NULL;
	  value = p->current + i;
	  value->type = SQLITE_NULL;
	  value->txt_blob_value = NULL;
      }
    return p;
}

static void
destroy_resultset_comparator (struct resultset_comparator *ptr)
{
/* memory cleanup - destroying a resultset comparator object */
    int i;
    if (ptr == NULL)
	return;
    for (i = 0; i < ptr->num_columns; i++)
      {
	  struct resultset_values *value = ptr->previous + i;
	  if (value->txt_blob_value != NULL)
	      free (value->txt_blob_value);
	  value = ptr->current + i;
	  if (value->txt_blob_value != NULL)
	      free (value->txt_blob_value);
      }
    if (ptr->previous != NULL)
	free (ptr->previous);
    if (ptr->current != NULL)
	free (ptr->current);
    free (ptr);
}

static void
save_row_from_resultset (struct resultset_comparator *ptr, sqlite3_stmt * stmt)
{
/* saving the current row values */
    int i;
    int size;
    const unsigned char *p;
    if (ptr == NULL)
	return;
    ptr->current_rowid = sqlite3_column_int64 (stmt, 0);
    for (i = 0; i < ptr->num_columns; i++)
      {
	  struct resultset_values *value = ptr->current + i;
	  value->type = sqlite3_column_type (stmt, i + 1);
	  switch (value->type)
	    {
	    case SQLITE_INTEGER:
		value->int_value = sqlite3_column_int64 (stmt, i + 1);
		break;
	    case SQLITE_FLOAT:
		value->dbl_value = sqlite3_column_double (stmt, i + 1);
		break;
	    case SQLITE_TEXT:
		p = sqlite3_column_text (stmt, i + 1);
		size = strlen ((const char *) p);
		value->txt_blob_value = malloc (size + 1);
		strcpy ((char *) (value->txt_blob_value), (const char *) p);
		break;
	    case SQLITE_BLOB:
		p = sqlite3_column_blob (stmt, i + 1);
		size = sqlite3_column_bytes (stmt, i + 1);
		value->txt_blob_value = malloc (size);
		memcpy (value->txt_blob_value, p, size);
		value->txt_blob_size = size;
		break;
	    };
      }
}

static int
resultset_rows_equals (struct resultset_comparator *ptr)
{
/* comparing the current and previous row from the resultset */
    int i;
    if (ptr == NULL)
	return 0;
    for (i = 0; i < ptr->num_columns; i++)
      {
	  struct resultset_values *val_prev = ptr->previous + i;
	  struct resultset_values *val_curr = ptr->current + i;
	  if (val_prev->type != val_curr->type)
	      return 0;
	  switch (val_prev->type)
	    {
	    case SQLITE_INTEGER:
		if (val_prev->int_value != val_curr->int_value)
		    return 0;
		break;
	    case SQLITE_FLOAT:
		if (val_prev->dbl_value != val_curr->dbl_value)
		    return 0;
		break;
	    case SQLITE_TEXT:
		if (strcmp
		    ((const char *) (val_prev->txt_blob_value),
		     (const char *) (val_curr->txt_blob_value)) != 0)
		    return 0;
		break;
	    case SQLITE_BLOB:
		if (val_prev->txt_blob_size != val_curr->txt_blob_size)
		    return 0;
		if (memcmp
		    (val_prev->txt_blob_value, val_curr->txt_blob_value,
		     val_curr->txt_blob_size) != 0)
		    return 0;
		break;
	    };
      }
    return 1;
}

static sqlite3_int64
get_current_resultset_rowid (struct resultset_comparator *ptr)
{
/* returns the current ROWID */
    if (ptr == NULL)
	return -1;
    return ptr->current_rowid;
}

static void
reset_resultset_current_row (struct resultset_comparator *ptr)
{
/* resetting the resultset current row values */
    int i;
    if (ptr == NULL)
	return;
    ptr->current_rowid = -1;
    for (i = 0; i < ptr->num_columns; i++)
      {
	  struct resultset_values *value = ptr->current + i;
	  value->type = SQLITE_NULL;
	  if (value->txt_blob_value != NULL)
	      free (value->txt_blob_value);
	  value->txt_blob_value = NULL;
      }
}

static void
swap_resultset_rows (struct resultset_comparator *ptr)
{
/* resetting the resultset comparator */
    int i;
    if (ptr == NULL)
	return;
    ptr->previous_rowid = ptr->current_rowid;
    ptr->current_rowid = -1;
    for (i = 0; i < ptr->num_columns; i++)
      {
	  struct resultset_values *val_prev = ptr->previous + i;
	  struct resultset_values *val_curr = ptr->current + i;
	  if (val_prev->txt_blob_value != NULL)
	      free (val_prev->txt_blob_value);
	  val_prev->type = val_curr->type;
	  val_prev->int_value = val_curr->int_value;
	  val_prev->dbl_value = val_curr->dbl_value;
	  val_prev->txt_blob_value = val_curr->txt_blob_value;
	  val_prev->txt_blob_size = val_curr->txt_blob_size;
	  val_curr->type = SQLITE_NULL;
	  val_curr->txt_blob_value = NULL;
      }
}

#ifndef OMIT_ICONV		/* ICONV enabled: supporting SHP */

static struct auxdbf_list *
alloc_auxdbf (gaiaDbfListPtr dbf_list)
{
/* allocating the auxiliary DBF struct */
    gaiaDbfFieldPtr fld;
    struct auxdbf_fld *fld_ex;
    struct auxdbf_list *auxdbf = malloc (sizeof (struct auxdbf_list));
    auxdbf->first = NULL;
    auxdbf->last = NULL;
    fld = dbf_list->First;
    while (fld)
      {
	  fld_ex = malloc (sizeof (struct auxdbf_fld));
	  fld_ex->already_used = 0;
	  fld_ex->dbf_field = fld;
	  fld_ex->next = NULL;
	  if (auxdbf->first == NULL)
	      auxdbf->first = fld_ex;
	  if (auxdbf->last != NULL)
	      auxdbf->last->next = fld_ex;
	  auxdbf->last = fld_ex;
	  fld = fld->Next;
      }
    return auxdbf;
}

static void
free_auxdbf (struct auxdbf_list *auxdbf)
{
/* freeing an auxiliary DBF struct */
    struct auxdbf_fld *n_fld;
    struct auxdbf_fld *fld = auxdbf->first;
    while (fld != NULL)
      {
	  n_fld = fld->next;
	  free (fld);
	  fld = n_fld;
      }
    free (auxdbf);
}

static gaiaDbfFieldPtr
getDbfField (struct auxdbf_list *aux, char *name)
{
/* find a DBF attribute by name */
    struct auxdbf_fld *fld = aux->first;
    while (fld)
      {
	  if (strcasecmp (fld->dbf_field->Name, name) == 0)
	    {
		fld->already_used = 1;
		return fld->dbf_field;
	    }
	  fld = fld->next;
      }
    fld = aux->first;
    while (fld)
      {
	  if (fld->already_used)
	    {
		fld = fld->next;
		continue;
	    }
	  if (strncasecmp (fld->dbf_field->Name, name, 9) == 0)
	    {
		fld->already_used = 1;
		return fld->dbf_field;
	    }
	  fld = fld->next;
      }
    return NULL;
}

static int
do_check_shp_unique_pk_values (sqlite3 * sqlite, gaiaShapefilePtr shp, int srid,
			       int text_dates, const char *pk_name, int pk_type)
{
/* checking for duplicate PK values */
    char *sql;
    sqlite3_stmt *stmt = NULL;
    gaiaDbfFieldPtr dbf_field;
    int ret;
    int duplicates = 0;
    int current_row = 0;

    sql = "CREATE TABLE TEMP.check_unique_pk (pkey ANYVALUE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
	return 0;

    sql = "INSERT INTO TEMP.check_unique_pk (pkey) VALUES (?)";
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    if (ret != SQLITE_OK)
	goto error;

    sqlite3_exec (sqlite, "BEGIN", NULL, NULL, NULL);

    while (1)
      {
	  /* reading rows from shapefile */
	  int ok_insert = 0;
	  ret = gaiaReadShpEntity_ex (shp, current_row, srid, text_dates);
	  if (ret < 0)
	    {
		/* found a DBF deleted record */
		current_row++;
		continue;
	    }
	  if (!ret)
	      break;
	  current_row++;
	  /* binding query params */
	  sqlite3_reset (stmt);
	  sqlite3_clear_bindings (stmt);
	  dbf_field = shp->Dbf->First;
	  while (dbf_field)
	    {
		/* Primary Key value */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      if (pk_type == SQLITE_TEXT)
			{
			    ok_insert = 1;
			    sqlite3_bind_text (stmt, 1,
					       dbf_field->Value->TxtValue,
					       strlen (dbf_field->
						       Value->TxtValue),
					       SQLITE_STATIC);
			}
		      else if (pk_type == SQLITE_FLOAT)
			{
			    ok_insert = 1;
			    sqlite3_bind_double (stmt, 1,
						 dbf_field->Value->DblValue);
			}
		      else
			{
			    ok_insert = 1;
			    sqlite3_bind_int64 (stmt, 1,
						dbf_field->Value->IntValue);
			}
		  }
		dbf_field = dbf_field->Next;
	    }
	  if (ok_insert)
	    {
		ret = sqlite3_step (stmt);
		if (ret == SQLITE_DONE || ret == SQLITE_ROW)
		    ;
		else
		    goto error;
	    }
      }
    sqlite3_finalize (stmt);
    stmt = NULL;
    sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);

    sql = "SELECT Count(*) FROM TEMP.check_unique_pk GROUP BY pkey";
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    if (ret != SQLITE_OK)
	goto error;

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		if (sqlite3_column_int (stmt, 0) > 1)
		    duplicates = 1;
	    }
      }
    sqlite3_finalize (stmt);

    sqlite3_exec (sqlite, "DROP TABLE TEMP.check_unique_pk", NULL, NULL, NULL);
    if (duplicates)
	return 0;
    else
	return 1;

  error:
    if (stmt != NULL)
	sqlite3_finalize (stmt);
    sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);
    sqlite3_exec (sqlite, "DROP TABLE TEMP.check_unique_pk", NULL, NULL, NULL);
    return 0;
}

static char *
convert_dbf_colname_case (const char *buf, int colname_case)
{
/* converts a DBF column-name to Lower- or Upper-case */
    int len = strlen (buf);
    char *clean = malloc (len + 1);
    char *p = clean;
    strcpy (clean, buf);
    while (*p != '\0')
      {
	  if (colname_case == GAIA_DBF_COLNAME_LOWERCASE)
	    {
		if (*p >= 'A' && *p <= 'Z')
		    *p = *p - 'A' + 'a';
	    }
	  if (colname_case == GAIA_DBF_COLNAME_UPPERCASE)
	    {
		if (*p >= 'a' && *p <= 'z')
		    *p = *p - 'a' + 'A';
	    }
	  p++;
      }
    return clean;
}

SPATIALITE_DECLARE int
load_shapefile (sqlite3 * sqlite, char *shp_path, char *table, char *charset,
		int srid, char *column, int coerce2d, int compressed,
		int verbose, int spatial_index, int *rows, char *err_msg)
{
    return load_shapefile_ex (sqlite, shp_path, table, charset, srid, column,
			      NULL, NULL, coerce2d, compressed, verbose,
			      spatial_index, rows, err_msg);
}

SPATIALITE_DECLARE int
load_shapefile_ex (sqlite3 * sqlite, char *shp_path, char *table,
		   char *charset, int srid, char *g_column, char *gtype,
		   char *pk_column, int coerce2d, int compressed, int verbose,
		   int spatial_index, int *rows, char *err_msg)
{
    return load_shapefile_ex2 (sqlite, shp_path, table, charset, srid, g_column,
			       gtype, pk_column, coerce2d, compressed, verbose,
			       spatial_index, 0, rows, err_msg);
}

SPATIALITE_DECLARE int
load_shapefile_ex2 (sqlite3 * sqlite, char *shp_path, char *table,
		    char *charset, int srid, char *g_column, char *gtype,
		    char *pk_column, int coerce2d, int compressed,
		    int verbose, int spatial_index, int text_dates, int *rows,
		    char *err_msg)
{
    return load_shapefile_ex3 (sqlite, shp_path, table, charset, srid, g_column,
			       gtype, pk_column, coerce2d, compressed, verbose,
			       spatial_index, text_dates, rows,
			       GAIA_DBF_COLNAME_LOWERCASE, err_msg);
}

static int
load_shapefile_common (struct zip_mem_shapefile *mem_shape, sqlite3 * sqlite,
		       const char *shp_path, const char *table,
		       const char *charset, int srid, const char *g_column,
		       const char *gtype, const char *pk_column, int coerce2d,
		       int compressed, int verbose, int spatial_index,
		       int text_dates, int *rows, int colname_case,
		       char *err_msg)
{
    sqlite3_stmt *stmt = NULL;
    int ret;
    char *errMsg = NULL;
    char *sql;
    char *dummy;
    int already_exists = 0;
    int metadata = 0;
    int sqlError = 0;
    gaiaShapefilePtr shp = NULL;
    gaiaDbfFieldPtr dbf_field;
    int cnt;
    int col_cnt = 0;
    int seed;
    int len;
    int dup;
    int idup;
    int current_row = 0;
    int deleted = 0;
    char **col_name = NULL;
    unsigned char *blob;
    int blob_size;
    char *geom_type = NULL;
    char *txt_dims;
    const char *geo_column = g_column;
    const char *xgtype = gtype;
    char *qtable = NULL;
    char *qpk_name = NULL;
    const char *pk_name = NULL;
    char *casename;
    int pk_autoincr = 1;
    char *xname;
    int pk_type = SQLITE_INTEGER;
    int pk_set;
    const char *alt_pk[10] =
	{ "PK_ALT0", "PK_ALT1", "PK_ALT2", "PK_ALT3", "PK_ALT4", "PK_ALT5",
	"PK_ALT6", "PK_ALT7", "PK_ALT8", "PK_ALT9"
    };
    gaiaOutBuffer sql_statement;
    if (!geo_column)
	geo_column = "Geometry";
    if (rows)
	*rows = -1;
    if (!xgtype)
	;
    else
      {
	  if (strcasecmp (xgtype, "LINESTRING") == 0)
	      xgtype = "LINESTRING";
	  else if (strcasecmp (xgtype, "LINESTRINGZ") == 0)
	      xgtype = "LINESTRINGZ";
	  else if (strcasecmp (xgtype, "LINESTRINGM") == 0)
	      xgtype = "LINESTRINGM";
	  else if (strcasecmp (xgtype, "LINESTRINGZM") == 0)
	      xgtype = "LINESTRINGZM";
	  else if (strcasecmp (xgtype, "MULTILINESTRING") == 0)
	      xgtype = "MULTILINESTRING";
	  else if (strcasecmp (xgtype, "MULTILINESTRINGZ") == 0)
	      xgtype = "MULTILINESTRINGZ";
	  else if (strcasecmp (xgtype, "MULTILINESTRINGM") == 0)
	      xgtype = "MULTILINESTRINGM";
	  else if (strcasecmp (xgtype, "MULTILINESTRINGZM") == 0)
	      xgtype = "MULTILINESTRINGZM";
	  else if (strcasecmp (xgtype, "POLYGON") == 0)
	      xgtype = "POLYGON";
	  else if (strcasecmp (xgtype, "POLYGONZ") == 0)
	      xgtype = "POLYGONZ";
	  else if (strcasecmp (xgtype, "POLYGONM") == 0)
	      xgtype = "POLYGONM";
	  else if (strcasecmp (xgtype, "POLYGONZM") == 0)
	      xgtype = "POLYGONZM";
	  else if (strcasecmp (xgtype, "MULTIPOLYGON") == 0)
	      xgtype = "MULTIPOLYGON";
	  else if (strcasecmp (xgtype, "MULTIPOLYGONZ") == 0)
	      xgtype = "MULTIPOLYGONZ";
	  else if (strcasecmp (xgtype, "MULTIPOLYGONM") == 0)
	      xgtype = "MULTIPOLYGONM";
	  else if (strcasecmp (xgtype, "MULTIPOLYGONZM") == 0)
	      xgtype = "MULTIPOLYGONZM";
	  else
	      xgtype = NULL;
      }
    qtable = gaiaDoubleQuotedSql (table);
/* checking if TABLE already exists */
    sql =
	sqlite3_mprintf ("SELECT name FROM sqlite_master WHERE type = 'table' "
			 "AND Lower(name) = Lower(%Q)", table);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: <%s>\n",
			    sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load shapefile error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  goto clean_up;
      }
    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	      already_exists = 1;
	  else
	    {
		spatialite_e ("load shapefile error: <%s>\n",
			      sqlite3_errmsg (sqlite));
		break;
	    }
      }
    sqlite3_finalize (stmt);
    if (already_exists)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: table '%s' already exists\n",
			    table);
	  else
	      sprintf (err_msg,
		       "load shapefile error: table '%s' already exists\n",
		       table);
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
/* checking if MetaData GEOMETRY_COLUMNS exists */
    sql =
	"SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'geometry_columns'";
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: <%s>\n",
			    sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load shapefile error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	      metadata = 1;
	  else
	    {
		spatialite_e ("load shapefile error: <%s>\n",
			      sqlite3_errmsg (sqlite));
		break;
	    }
      }
    sqlite3_finalize (stmt);
    shp = gaiaAllocShapefile ();
    if (mem_shape != NULL)
      {
	  /* initializing Memory based files */
	  shp->memShx = &(mem_shape->shx);
	  shp->memShp = &(mem_shape->shp);
	  shp->memDbf = &(mem_shape->dbf);
      }
    gaiaOpenShpRead (shp, shp_path, charset, "UTF-8");
    if (!(shp->Valid))
      {
	  if (!err_msg)
	    {
		spatialite_e
		    ("load shapefile error: cannot open shapefile '%s'\n",
		     shp_path);
		if (shp->LastError)
		    spatialite_e ("\tcause: %s\n", shp->LastError);
	    }
	  else
	    {
		char extra[512];
		*extra = '\0';
		if (shp->LastError)
		    sprintf (extra, "\n\tcause: %s\n", shp->LastError);
		sprintf (err_msg,
			 "load shapefile error: cannot open shapefile '%s'%s",
			 shp_path, extra);
	    }
	  gaiaFreeShapefile (shp);
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
/* checking for duplicate / illegal column names and antialising them */
    col_cnt = 0;
    dbf_field = shp->Dbf->First;
    while (dbf_field)
      {
	  /* counting DBF fields */
	  col_cnt++;
	  dbf_field = dbf_field->Next;
      }
    col_name = malloc (sizeof (char *) * col_cnt);
    cnt = 0;
    seed = 0;
    if (pk_column != NULL)
      {
	  /* validating the Primary Key column */
	  dbf_field = shp->Dbf->First;
	  while (dbf_field)
	    {
		if (strcasecmp (pk_column, dbf_field->Name) == 0)
		  {
		      /* ok, using this field as Primary Key */
		      pk_name = pk_column;
		      pk_autoincr = 0;
		      switch (dbf_field->Type)
			{
			case 'C':
			    pk_type = SQLITE_TEXT;
			    break;
			case 'N':
			    if (dbf_field->Decimals)
				pk_type = SQLITE_FLOAT;
			    else
			      {
				  if (dbf_field->Length <= 18)
				      pk_type = SQLITE_INTEGER;
				  else
				      pk_type = SQLITE_FLOAT;
			      }
			    break;
			case 'D':
			    if (text_dates)
				pk_type = SQLITE_TEXT;
			    else
				pk_type = SQLITE_FLOAT;
			    break;
			case 'F':
			    pk_type = SQLITE_FLOAT;
			    break;
			case 'L':
			    pk_type = SQLITE_INTEGER;
			    break;
			};
		  }
		dbf_field = dbf_field->Next;
	    }
      }
    if (pk_name == NULL)
      {
	  if (pk_column != NULL)
	      pk_name = pk_column;
	  else
	      pk_name = "PK_UID";
      }
    if (!do_check_shp_unique_pk_values
	(sqlite, shp, srid, text_dates, pk_name, pk_type))
      {
	  const char *old_pk = pk_name;
	  int antialias;
	  for (antialias = 0; antialias < 10; antialias++)
	    {
		/* searching an alternative Primary Key column name */
		int found = 0;
		pk_name = alt_pk[antialias];
		dbf_field = shp->Dbf->First;
		while (dbf_field)
		  {
		      if (strcasecmp (pk_name, dbf_field->Name) == 0)
			  found = 1;
		      dbf_field = dbf_field->Next;
		  }
		if (!found)
		  {
		      pk_autoincr = 1;
		      goto ok_pk;
		  }
	    }
	  pk_name = old_pk;
      }
  ok_pk:
    casename = convert_dbf_colname_case (pk_name, colname_case);
    qpk_name = gaiaDoubleQuotedSql (casename);
    free (casename);
    dbf_field = shp->Dbf->First;
    while (dbf_field)
      {
	  /* preparing column names */
	  char *xdummy = NULL;
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dummy = dbf_field->Name;
		len = strlen (dummy);
		*(col_name + cnt) = malloc (len + 1);
		strcpy (*(col_name + cnt), dummy);
		cnt++;
		dbf_field = dbf_field->Next;
		continue;
	    }
	  dummy = dbf_field->Name;
	  dup = 0;
	  for (idup = 0; idup < cnt; idup++)
	    {
		if (strcasecmp (dummy, *(col_name + idup)) == 0)
		    dup = 1;
	    }
	  if (strcasecmp (dummy, pk_name) == 0)
	      dup = 1;
	  if (strcasecmp (dummy, geo_column) == 0)
	      dup = 1;
	  if (dup)
	    {
		xdummy = sqlite3_mprintf ("COL_%d", seed++);
		spatialite_e
		    ("Warning: duplicated fieldName \"%s\" in shapefile \"%s\": "
		     "renaming the second occurrence in table \"%s\" as \"%s\".\n",
		     dummy, shp_path, table, xdummy);
		dummy = xdummy;
	    }
	  len = strlen (dummy);
	  *(col_name + cnt) = malloc (len + 1);
	  strcpy (*(col_name + cnt), dummy);
	  if (xdummy)
	      sqlite3_free (xdummy);
	  cnt++;
	  dbf_field = dbf_field->Next;
      }
    if (verbose)
	spatialite_e
	    ("========\nLoading shapefile at '%s' into SQLite table '%s'\n",
	     shp_path, table);
/* starting a transaction */
    if (verbose)
	spatialite_e ("\nBEGIN;\n");
    ret = sqlite3_exec (sqlite, "BEGIN", NULL, 0, &errMsg);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: <%s>\n", errMsg);
	  else
	      sprintf (err_msg, "load shapefile error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  sqlError = 1;
	  goto clean_up;
      }
/* creating the Table */
    gaiaOutBufferInitialize (&sql_statement);
    if (pk_type == SQLITE_TEXT)
      {
	  sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				 "TEXT PRIMARY KEY NOT NULL", qtable, qpk_name);
      }
    else if (pk_type == SQLITE_FLOAT)
      {
	  sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				 "DOUBLE PRIMARY KEY NOT NULL", qtable,
				 qpk_name);
      }
    else
      {
	  if (pk_autoincr)
	      sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				     "INTEGER PRIMARY KEY AUTOINCREMENT",
				     qtable, qpk_name);
	  else
	      sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				     "INTEGER NOT NULL PRIMARY KEY", qtable,
				     qpk_name);
      }
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    cnt = 0;
    dbf_field = shp->Dbf->First;
    while (dbf_field)
      {
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		cnt++;
		continue;
	    }
	  casename = convert_dbf_colname_case (*(col_name + cnt), colname_case);
	  xname = gaiaDoubleQuotedSql (casename);
	  free (casename);
	  sql = sqlite3_mprintf (",\n\"%s\"", xname);
	  free (xname);
	  gaiaAppendToOutBuffer (&sql_statement, sql);
	  sqlite3_free (sql);
	  cnt++;
	  switch (dbf_field->Type)
	    {
	    case 'C':
		gaiaAppendToOutBuffer (&sql_statement, " TEXT");
		break;
	    case 'N':
		if (dbf_field->Decimals)
		    gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		else
		  {
		      if (dbf_field->Length <= 18)
			  gaiaAppendToOutBuffer (&sql_statement, " INTEGER");
		      else
			  gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		  }
		break;
	    case 'D':
		if (text_dates)
		    gaiaAppendToOutBuffer (&sql_statement, "TEXT");
		else
		    gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		break;
	    case 'F':
		gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		break;
	    case 'L':
		gaiaAppendToOutBuffer (&sql_statement, " INTEGER");
		break;
	    };
	  dbf_field = dbf_field->Next;
      }
    if (metadata)
	gaiaAppendToOutBuffer (&sql_statement, ")");
    else
      {
	  casename = convert_dbf_colname_case (geo_column, colname_case);
	  xname = gaiaDoubleQuotedSql (casename);
	  free (casename);
	  sql = sqlite3_mprintf (",\n\"%s\" BLOB)", xname);
	  free (xname);
	  gaiaAppendToOutBuffer (&sql_statement, sql);
	  sqlite3_free (sql);
      }
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
      {
	  if (verbose)
	      spatialite_e ("%s;\n", sql_statement.Buffer);
	  ret = sqlite3_exec (sqlite, sql_statement.Buffer, NULL, 0, &errMsg);
      }
    else
	ret = SQLITE_ERROR;
    gaiaOutBufferReset (&sql_statement);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: <%s>\n", errMsg);
	  else
	      sprintf (err_msg, "load shapefile error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  sqlError = 1;
	  goto clean_up;
      }
    if (metadata)
      {
	  /* creating Geometry column */
	  switch (shp->Shape)
	    {
	    case GAIA_SHP_POINT:
	    case GAIA_SHP_POINTM:
	    case GAIA_SHP_POINTZ:
		geom_type = "POINT";
		break;
	    case GAIA_SHP_MULTIPOINT:
	    case GAIA_SHP_MULTIPOINTM:
	    case GAIA_SHP_MULTIPOINTZ:
		geom_type = "MULTIPOINT";
		break;
	    case GAIA_SHP_POLYLINE:
	    case GAIA_SHP_POLYLINEM:
	    case GAIA_SHP_POLYLINEZ:
		if (xgtype == NULL)
		  {
		      /* auto-decting if MULTILINESTRING is required */
		      gaiaShpAnalyze (shp);
		      if (shp->EffectiveType == GAIA_LINESTRING)
			  geom_type = "LINESTRING";
		      else
			  geom_type = "MULTILINESTRING";
		  }
		else
		  {
		      /* user-defined geometry type */
		      if (strcmp (xgtype, "LINESTRING") == 0)
			{
			    geom_type = "LINESTRING";
			    shp->EffectiveType = GAIA_LINESTRING;
			    shp->EffectiveDims = GAIA_XY;
			}
		      if (strcmp (xgtype, "LINESTRINGZ") == 0)
			{
			    geom_type = "LINESTRING";
			    shp->EffectiveType = GAIA_LINESTRING;
			    shp->EffectiveDims = GAIA_XY_Z;
			}
		      if (strcmp (xgtype, "LINESTRINGM") == 0)
			{
			    geom_type = "LINESTRING";
			    shp->EffectiveType = GAIA_LINESTRING;
			    shp->EffectiveDims = GAIA_XY_M;
			}
		      if (strcmp (xgtype, "LINESTRINGZM") == 0)
			{
			    geom_type = "LINESTRING";
			    shp->EffectiveType = GAIA_LINESTRING;
			    shp->EffectiveDims = GAIA_XY_Z_M;
			}
		      if (strcmp (xgtype, "MULTILINESTRING") == 0)
			{
			    geom_type = "MULTILINESTRING";
			    shp->EffectiveType = GAIA_MULTILINESTRING;
			    shp->EffectiveDims = GAIA_XY;
			}
		      if (strcmp (xgtype, "MULTILINESTRINGZ") == 0)
			{
			    geom_type = "MULTILINESTRING";
			    shp->EffectiveType = GAIA_MULTILINESTRING;
			    shp->EffectiveDims = GAIA_XY_Z;
			}
		      if (strcmp (xgtype, "MULTILINESTRINGM") == 0)
			{
			    geom_type = "MULTILINESTRING";
			    shp->EffectiveType = GAIA_MULTILINESTRING;
			    shp->EffectiveDims = GAIA_XY_M;
			}
		      if (strcmp (xgtype, "MULTILINESTRINGZM") == 0)
			{
			    geom_type = "MULTILINESTRING";
			    shp->EffectiveType = GAIA_MULTILINESTRING;
			    shp->EffectiveDims = GAIA_XY_Z_M;
			}
		  }
		break;
	    case GAIA_SHP_POLYGON:
	    case GAIA_SHP_POLYGONM:
	    case GAIA_SHP_POLYGONZ:
		if (xgtype == NULL)
		  {
		      /* auto-decting if MULTIPOLYGON is required */
		      gaiaShpAnalyze (shp);
		      if (shp->EffectiveType == GAIA_POLYGON)
			  geom_type = "POLYGON";
		      else
			  geom_type = "MULTIPOLYGON";
		  }
		else
		  {
		      /* user-defined geometry type */
		      if (strcmp (xgtype, "POLYGON") == 0)
			{
			    geom_type = "POLYGON";
			    shp->EffectiveType = GAIA_POLYGON;
			    shp->EffectiveDims = GAIA_XY;
			}
		      if (strcmp (xgtype, "POLYGONZ") == 0)
			{
			    geom_type = "POLYGON";
			    shp->EffectiveType = GAIA_POLYGON;
			    shp->EffectiveDims = GAIA_XY_Z;
			}
		      if (strcmp (xgtype, "POLYGONM") == 0)
			{
			    geom_type = "POLYGON";
			    shp->EffectiveType = GAIA_POLYGON;
			    shp->EffectiveDims = GAIA_XY_M;
			}
		      if (strcmp (xgtype, "POLYGONZM") == 0)
			{
			    geom_type = "POLYGON";
			    shp->EffectiveType = GAIA_POLYGON;
			    shp->EffectiveDims = GAIA_XY_Z_M;
			}
		      if (strcmp (xgtype, "MULTIPOLYGON") == 0)
			{
			    geom_type = "MULTIPOLYGON";
			    shp->EffectiveType = GAIA_MULTIPOLYGON;
			    shp->EffectiveDims = GAIA_XY;
			}
		      if (strcmp (xgtype, "MULTIPOLYGONZ") == 0)
			{
			    geom_type = "MULTIPOLYGON";
			    shp->EffectiveType = GAIA_MULTIPOLYGON;
			    shp->EffectiveDims = GAIA_XY_Z;
			}
		      if (strcmp (xgtype, "MULTIPOLYGONM") == 0)
			{
			    geom_type = "MULTIPOLYGON";
			    shp->EffectiveType = GAIA_MULTIPOLYGON;
			    shp->EffectiveDims = GAIA_XY_M;
			}
		      if (strcmp (xgtype, "MULTIPOLYGONZM") == 0)
			{
			    geom_type = "MULTIPOLYGON";
			    shp->EffectiveType = GAIA_MULTIPOLYGON;
			    shp->EffectiveDims = GAIA_XY_Z_M;
			}
		  }
		break;
	    };
	  if (coerce2d)
	      shp->EffectiveDims = GAIA_XY;
	  switch (shp->EffectiveDims)
	    {
	    case GAIA_XY_Z:
		txt_dims = "XYZ";
		break;
	    case GAIA_XY_M:
		txt_dims = "XYM";
		break;
	    case GAIA_XY_Z_M:
		txt_dims = "XYZM";
		break;
	    default:
		txt_dims = "XY";
		break;
	    };
	  if (geom_type == NULL)
	    {
		/* undefined Geometry Type */
		const char *gt = "UNKNOWN";
		switch (shp->Shape)
		  {
		  case GAIA_SHP_POINT:
		  case GAIA_SHP_POINTM:
		  case GAIA_SHP_POINTZ:
		      gt = "POINT";
		      break;
		  case GAIA_SHP_MULTIPOINT:
		  case GAIA_SHP_MULTIPOINTM:
		  case GAIA_SHP_MULTIPOINTZ:
		      gt = "MULTIPOINT";
		      break;
		  case GAIA_SHP_POLYLINE:
		  case GAIA_SHP_POLYLINEM:
		  case GAIA_SHP_POLYLINEZ:
		      if (shp->EffectiveType == GAIA_LINESTRING)
			  gt = "LINESTRING";
		      else
			  gt = "MULTILINESTRING";
		      break;
		  case GAIA_SHP_POLYGON:
		  case GAIA_SHP_POLYGONM:
		  case GAIA_SHP_POLYGONZ:
		      if (shp->EffectiveType == GAIA_POLYGON)
			  gt = "POLYGON";
		      else
			  gt = "MULTIPOLYGON";
		      break;
		  };
		if (!err_msg)
		    spatialite_e
			("Error: mismatching type: requested %s but Shape is %s %s\n",
			 xgtype, gt, txt_dims);
		else
		    sprintf (err_msg,
			     "Error: mismatching type: requested %s but Shape is %s %s\n",
			     xgtype, gt, txt_dims);
		sqlError = 1;
		goto clean_up;
	    }
	  casename = convert_dbf_colname_case (geo_column, colname_case);
	  sql = sqlite3_mprintf ("SELECT AddGeometryColumn(%Q, %Q, %d, %Q, %Q)",
				 table, casename, srid, geom_type, txt_dims);
	  free (casename);
	  if (verbose)
	      spatialite_e ("%s;\n", sql);
	  ret = sqlite3_exec (sqlite, sql, NULL, 0, &errMsg);
	  sqlite3_free (sql);
	  if (ret != SQLITE_OK)
	    {
		if (!err_msg)
		    spatialite_e ("load shapefile error: <%s>\n", errMsg);
		else
		    sprintf (err_msg, "load shapefile error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
		sqlError = 1;
		goto clean_up;
	    }
	  if (spatial_index)
	    {
		/* creating the Spatial Index */
		sql = sqlite3_mprintf ("SELECT CreateSpatialIndex(%Q, %Q)",
				       table, geo_column);
		ret = sqlite3_exec (sqlite, sql, NULL, 0, &errMsg);
		sqlite3_free (sql);
		if (ret != SQLITE_OK)
		  {
		      if (!err_msg)
			  spatialite_e ("load shapefile error: <%s>\n", errMsg);
		      else
			  sprintf (err_msg, "load shapefile error: <%s>\n",
				   errMsg);
		      sqlite3_free (errMsg);
		      sqlError = 1;
		      goto clean_up;
		  }
	    }
      }
    else
      {
	  /* no Metadata */
	  if (shp->Shape == GAIA_SHP_POLYLINE
	      || shp->Shape == GAIA_SHP_POLYLINEM
	      || shp->Shape == GAIA_SHP_POLYLINEZ
	      || shp->Shape == GAIA_SHP_POLYGON
	      || shp->Shape == GAIA_SHP_POLYGONM
	      || shp->Shape == GAIA_SHP_POLYGONZ)
	    {
		/* 
		   / fixing anyway the Geometry type for 
		   / LINESTRING/MULTILINESTRING or 
		   / POLYGON/MULTIPOLYGON
		 */
		gaiaShpAnalyze (shp);
	    }
      }
    /* preparing the INSERT INTO parametrerized statement */
    gaiaOutBufferInitialize (&sql_statement);
    sql = sqlite3_mprintf ("INSERT INTO \"%s\" (\"%s\",", qtable, qpk_name);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    cnt = 0;
    dbf_field = shp->Dbf->First;
    while (dbf_field)
      {
	  /* columns corresponding to some DBF attribute */
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		cnt++;
		continue;
	    }
	  xname = gaiaDoubleQuotedSql (*(col_name + cnt++));
	  sql = sqlite3_mprintf ("\"%s\" ,", xname);
	  free (xname);
	  gaiaAppendToOutBuffer (&sql_statement, sql);
	  sqlite3_free (sql);
	  dbf_field = dbf_field->Next;
      }
    xname = gaiaDoubleQuotedSql (geo_column);	/* the GEOMETRY column */
    sql = sqlite3_mprintf ("\"%s\")\n VALUES (?", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    dbf_field = shp->Dbf->First;
    while (dbf_field)
      {
	  /* column values */
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		continue;
	    }
	  gaiaAppendToOutBuffer (&sql_statement, ", ?");
	  dbf_field = dbf_field->Next;
      }
    gaiaAppendToOutBuffer (&sql_statement, ", ?)");	/* the GEOMETRY column */
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
	ret =
	    sqlite3_prepare_v2 (sqlite, sql_statement.Buffer,
				strlen (sql_statement.Buffer), &stmt, NULL);
    else
	ret = SQLITE_ERROR;
    gaiaOutBufferReset (&sql_statement);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load shapefile error: <%s>\n",
			    sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load shapefile error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  sqlError = 1;
	  goto clean_up;
      }
    current_row = 0;
    while (1)
      {
	  /* inserting rows from shapefile */
	  ret = gaiaReadShpEntity_ex (shp, current_row, srid, text_dates);
	  if (ret < 0)
	    {
		/* found a DBF deleted record */
		current_row++;
		deleted++;
		continue;
	    }
	  if (!ret)
	    {
		if (!(shp->LastError))	/* normal SHP EOF */
		    break;
		if (!err_msg)
		    spatialite_e ("%s\n", shp->LastError);
		else
		    sprintf (err_msg, "%s\n", shp->LastError);
		sqlError = 1;
		sqlite3_finalize (stmt);
		goto clean_up;
	    }
	  current_row++;
	  /* binding query params */
	  sqlite3_reset (stmt);
	  sqlite3_clear_bindings (stmt);
	  pk_set = 0;
	  cnt = 0;
	  dbf_field = shp->Dbf->First;
	  while (dbf_field)
	    {
		/* Primary Key value */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      if (pk_type == SQLITE_TEXT)
			  sqlite3_bind_text (stmt, 1,
					     dbf_field->Value->TxtValue,
					     strlen (dbf_field->
						     Value->TxtValue),
					     SQLITE_STATIC);
		      else if (pk_type == SQLITE_FLOAT)
			  sqlite3_bind_double (stmt, 1,
					       dbf_field->Value->DblValue);
		      else
			  sqlite3_bind_int64 (stmt, 1,
					      dbf_field->Value->IntValue);
		      pk_set = 1;
		  }
		dbf_field = dbf_field->Next;
	    }
	  if (!pk_set)
	      sqlite3_bind_int (stmt, 1, current_row);
	  cnt = 0;
	  dbf_field = shp->Dbf->First;
	  while (dbf_field)
	    {
		/* column values */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      /* skipping the Primary Key field */
		      dbf_field = dbf_field->Next;
		      continue;
		  }
		if (!(dbf_field->Value))
		    sqlite3_bind_null (stmt, cnt + 2);
		else
		  {
		      switch (dbf_field->Value->Type)
			{
			case GAIA_INT_VALUE:
			    sqlite3_bind_int64 (stmt, cnt + 2,
						dbf_field->Value->IntValue);
			    break;
			case GAIA_DOUBLE_VALUE:
			    sqlite3_bind_double (stmt, cnt + 2,
						 dbf_field->Value->DblValue);
			    break;
			case GAIA_TEXT_VALUE:
			    sqlite3_bind_text (stmt, cnt + 2,
					       dbf_field->Value->TxtValue,
					       strlen (dbf_field->
						       Value->TxtValue),
					       SQLITE_STATIC);
			    break;
			default:
			    sqlite3_bind_null (stmt, cnt + 2);
			    break;
			}
		  }
		cnt++;
		dbf_field = dbf_field->Next;
	    }
	  if (shp->Dbf->Geometry)
	    {
		if (compressed)
		    gaiaToCompressedBlobWkb (shp->Dbf->Geometry, &blob,
					     &blob_size);
		else
		    gaiaToSpatiaLiteBlobWkb (shp->Dbf->Geometry, &blob,
					     &blob_size);
		sqlite3_bind_blob (stmt, cnt + 2, blob, blob_size, free);
	    }
	  else
	    {
		/* handling a NULL-Geometry */
		sqlite3_bind_null (stmt, cnt + 2);
	    }
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
	      ;
	  else
	    {
		if (!err_msg)
		    spatialite_e ("load shapefile error: <%s>\n",
				  sqlite3_errmsg (sqlite));
		else
		    sprintf (err_msg, "load shapefile error: <%s>\n",
			     sqlite3_errmsg (sqlite));
		sqlite3_finalize (stmt);
		sqlError = 1;
		goto clean_up;
	    }
      }
    sqlite3_finalize (stmt);
  clean_up:
    if (qtable)
	free (qtable);
    if (qpk_name)
	free (qpk_name);
    gaiaFreeShapefile (shp);
    if (col_name)
      {
	  /* releasing memory allocation for column names */
	  for (cnt = 0; cnt < col_cnt; cnt++)
	      free (*(col_name + cnt));
	  free (col_name);
      }
    if (sqlError)
      {
	  /* some error occurred - ROLLBACK */
	  if (verbose)
	      spatialite_e ("ROLLBACK;\n");
	  ret = sqlite3_exec (sqlite, "ROLLBACK", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load shapefile error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
	    }
	  return 0;
      }
    else
      {
	  /* ok - confirming pending transaction - COMMIT */
	  if (verbose)
	      spatialite_e ("COMMIT;\n");
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		if (!err_msg)
		    spatialite_e ("load shapefile error: <%s>\n", errMsg);
		else
		    sprintf (err_msg, "load shapefile error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
		return 0;
	    }
	  if (rows)
	      *rows = current_row - deleted;
	  if (verbose)
	      spatialite_e
		  ("\nInserted %d rows into '%s' from SHAPEFILE\n========\n",
		   current_row - deleted, table);
	  if (err_msg)
	      sprintf (err_msg, "Inserted %d rows into '%s' from SHAPEFILE",
		       current_row - deleted, table);
	  return 1;
      }
}

SPATIALITE_DECLARE int
load_shapefile_ex3 (sqlite3 * sqlite, const char *shp_path, const char *table,
		    const char *charset, int srid, const char *g_column,
		    const char *gtype, const char *pk_column, int coerce2d,
		    int compressed, int verbose, int spatial_index,
		    int text_dates, int *rows, int colname_case, char *err_msg)
{
    return load_shapefile_common (NULL, sqlite, shp_path, table, charset, srid,
				  g_column, gtype, pk_column, coerce2d,
				  compressed, verbose, spatial_index,
				  text_dates, rows, colname_case, err_msg);
}

static int
do_check_dbf_unique_pk_values (sqlite3 * sqlite, gaiaDbfPtr dbf, int text_dates,
			       const char *pk_name, int pk_type)
{
/* checking for duplicate PK values */
    char *sql;
    sqlite3_stmt *stmt = NULL;
    gaiaDbfFieldPtr dbf_field;
    int ret;
    int deleted;
    int duplicates = 0;
    int current_row = 0;

    sql = "CREATE TABLE TEMP.check_unique_pk (pkey ANYVALUE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
	return 0;

    sql = "INSERT INTO TEMP.check_unique_pk (pkey) VALUES (?)";
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    if (ret != SQLITE_OK)
	goto error;

    sqlite3_exec (sqlite, "BEGIN", NULL, NULL, NULL);

    while (1)
      {
	  /* reading rows from shapefile */
	  int ok_insert = 0;
	  ret = gaiaReadDbfEntity_ex (dbf, current_row, &deleted, text_dates);
	  if (!ret)
	      break;
	  current_row++;
	  if (deleted)
	    {
		/* skipping DBF deleted row */
		continue;
	    }
	  /* binding query params */
	  sqlite3_reset (stmt);
	  sqlite3_clear_bindings (stmt);
	  dbf_field = dbf->Dbf->First;
	  while (dbf_field)
	    {
		/* Primary Key value */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      if (pk_type == SQLITE_TEXT)
			{
			    ok_insert = 1;
			    sqlite3_bind_text (stmt, 1,
					       dbf_field->Value->TxtValue,
					       strlen (dbf_field->
						       Value->TxtValue),
					       SQLITE_STATIC);
			}
		      else if (pk_type == SQLITE_FLOAT)
			{
			    ok_insert = 1;
			    sqlite3_bind_double (stmt, 1,
						 dbf_field->Value->DblValue);
			}
		      else
			{
			    ok_insert = 1;
			    sqlite3_bind_int64 (stmt, 1,
						dbf_field->Value->IntValue);
			}
		  }
		dbf_field = dbf_field->Next;
	    }
	  if (ok_insert)
	    {
		ret = sqlite3_step (stmt);
		if (ret == SQLITE_DONE || ret == SQLITE_ROW)
		    ;
		else
		    goto error;
	    }
      }
    sqlite3_finalize (stmt);
    stmt = NULL;
    sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);

    sql = "SELECT Count(*) FROM TEMP.check_unique_pk GROUP BY pkey";
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    if (ret != SQLITE_OK)
	goto error;

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		if (sqlite3_column_int (stmt, 0) > 1)
		    duplicates = 1;
	    }
      }
    sqlite3_finalize (stmt);

    sqlite3_exec (sqlite, "DROP TABLE TEMP.check_unique_pk", NULL, NULL, NULL);
    if (duplicates)
	return 0;
    else
	return 1;

  error:
    if (stmt != NULL)
	sqlite3_finalize (stmt);
    sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);
    sqlite3_exec (sqlite, "DROP TABLE TEMP.check_unique_pk", NULL, NULL, NULL);
    return 0;
}

SPATIALITE_DECLARE int
load_dbf_common (struct zip_mem_shapefile *mem_shape, sqlite3 * sqlite,
		 const char *dbf_path, const char *table, const char *pk_column,
		 const char *charset, int verbose, int text_dates, int *rows,
		 int colname_case, char *err_msg)
{
    sqlite3_stmt *stmt;
    int ret;
    char *errMsg = NULL;
    char *sql;
    char *dummy;
    char *xname;
    int already_exists = 0;
    int sqlError = 0;
    gaiaDbfPtr dbf = NULL;
    gaiaDbfFieldPtr dbf_field;
    int cnt;
    int col_cnt;
    int seed;
    int len;
    int dup;
    int idup;
    int current_row = 0;
    char **col_name = NULL;
    int deleted;
    char *qtable = NULL;
    char *qpk_name = NULL;
    const char *pk_name = NULL;
    char *casename;
    int pk_autoincr = 1;
    gaiaOutBuffer sql_statement;
    int pk_type = SQLITE_INTEGER;
    int pk_set;
    const char *alt_pk[10] =
	{ "PK_ALT0", "PK_ALT1", "PK_ALT2", "PK_ALT3", "PK_ALT4", "PK_ALT5",
	"PK_ALT6", "PK_ALT7", "PK_ALT8", "PK_ALT9"
    };
    qtable = gaiaDoubleQuotedSql (table);
    if (rows)
	*rows = -1;
/* checking if TABLE already exists */
    sql = sqlite3_mprintf ("SELECT name FROM sqlite_master WHERE "
			   "type = 'table' AND Lower(name) = Lower(%Q)", table);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load DBF error: <%s>\n", sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load DBF error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	      already_exists = 1;
	  else
	    {
		spatialite_e ("load DBF error: <%s>\n",
			      sqlite3_errmsg (sqlite));
		break;
	    }
      }
    sqlite3_finalize (stmt);
    if (already_exists)
      {
	  if (!err_msg)
	      spatialite_e ("load DBF error: table '%s' already exists\n",
			    table);
	  else
	      sprintf (err_msg, "load DBF error: table '%s' already exists\n",
		       table);
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
    dbf = gaiaAllocDbf ();
    if (mem_shape != NULL)
      {
	  /* initializing Memory based files */
	  dbf->memDbf = &(mem_shape->dbf);
      }
    gaiaOpenDbfRead (dbf, dbf_path, charset, "UTF-8");
    if (!(dbf->Valid))
      {
	  if (!err_msg)
	    {
		spatialite_e ("load DBF error: cannot open '%s'\n", dbf_path);
		if (dbf->LastError)
		    spatialite_e ("\tcause: %s\n", dbf->LastError);
	    }
	  else
	    {
		char extra[512];
		*extra = '\0';
		if (dbf->LastError)
		    sprintf (extra, "\n\tcause: %s", dbf->LastError);
		sprintf (err_msg, "load DBF error: cannot open '%s'%s",
			 dbf_path, extra);
	    }
	  gaiaFreeDbf (dbf);
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
/* checking for duplicate / illegal column names and antialising them */
    col_cnt = 0;
    dbf_field = dbf->Dbf->First;
    while (dbf_field)
      {
	  /* counting DBF fields */
	  col_cnt++;
	  dbf_field = dbf_field->Next;
      }
    col_name = malloc (sizeof (char *) * col_cnt);
    cnt = 0;
    seed = 0;
    if (pk_column != NULL)
      {
	  /* validating the Primary Key column */
	  dbf_field = dbf->Dbf->First;
	  while (dbf_field)
	    {
		if (strcasecmp (pk_column, dbf_field->Name) == 0)
		  {
		      /* ok, using this field as Primary Key */
		      pk_name = pk_column;
		      pk_autoincr = 0;
		      switch (dbf_field->Type)
			{
			case 'C':
			    pk_type = SQLITE_TEXT;
			    break;
			case 'N':
			    if (dbf_field->Decimals)
				pk_type = SQLITE_FLOAT;
			    else
			      {
				  if (dbf_field->Length <= 18)
				      pk_type = SQLITE_INTEGER;
				  else
				      pk_type = SQLITE_FLOAT;
			      }
			    break;
			case 'D':
			    if (text_dates)
				pk_type = SQLITE_TEXT;
			    else
				pk_type = SQLITE_FLOAT;
			    break;
			case 'F':
			    pk_type = SQLITE_FLOAT;
			    break;
			case 'L':
			    pk_type = SQLITE_INTEGER;
			    break;
			};
		  }
		dbf_field = dbf_field->Next;
	    }
      }
    if (pk_name == NULL)
      {
	  if (pk_column != NULL)
	      pk_name = pk_column;
	  else
	      pk_name = "PK_UID";
      }
    if (!do_check_dbf_unique_pk_values
	(sqlite, dbf, text_dates, pk_name, pk_type))
      {
	  const char *old_pk = pk_name;
	  int antialias;
	  for (antialias = 0; antialias < 10; antialias++)
	    {
		/* searching an alternative Primary Key column name */
		int found = 0;
		pk_name = alt_pk[antialias];
		dbf_field = dbf->Dbf->First;
		while (dbf_field)
		  {
		      if (strcasecmp (pk_name, dbf_field->Name) == 0)
			  found = 1;
		      dbf_field = dbf_field->Next;
		  }
		if (!found)
		  {
		      pk_autoincr = 1;
		      goto ok_pk;
		  }
	    }
	  pk_name = old_pk;
      }
  ok_pk:
    casename = convert_dbf_colname_case (pk_name, colname_case);
    qpk_name = gaiaDoubleQuotedSql (casename);
    free (casename);
    dbf_field = dbf->Dbf->First;
    while (dbf_field)
      {
	  /* preparing column names */
	  char *xdummy = NULL;
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dummy = dbf_field->Name;
		len = strlen (dummy);
		*(col_name + cnt) = malloc (len + 1);
		strcpy (*(col_name + cnt), dummy);
		cnt++;
		dbf_field = dbf_field->Next;
		continue;
	    }
	  dummy = dbf_field->Name;
	  dup = 0;
	  for (idup = 0; idup < cnt; idup++)
	    {
		if (strcasecmp (dummy, *(col_name + idup)) == 0)
		    dup = 1;
	    }
	  if (dup)
	    {
		xdummy = sqlite3_mprintf ("COL_%d", seed++);
		dummy = xdummy;
	    }
	  len = strlen (dummy);
	  *(col_name + cnt) = malloc (len + 1);
	  strcpy (*(col_name + cnt), dummy);
	  if (xdummy)
	      sqlite3_free (xdummy);
	  cnt++;
	  dbf_field = dbf_field->Next;
      }
    if (verbose)
	spatialite_e ("========\nLoading DBF at '%s' into SQLite table '%s'\n",
		      dbf_path, table);
/* starting a transaction */
    if (verbose)
	spatialite_e ("\nBEGIN;\n");
    ret = sqlite3_exec (sqlite, "BEGIN", NULL, 0, &errMsg);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load DBF error: <%s>\n", errMsg);
	  else
	      sprintf (err_msg, "load DBF error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  sqlError = 1;
	  goto clean_up;
      }
/* creating the Table */
    gaiaOutBufferInitialize (&sql_statement);
    if (pk_type == SQLITE_TEXT)
      {
	  sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				 "TEXT PRIMARY KEY NOT NULL", qtable, qpk_name);
      }
    else if (pk_type == SQLITE_FLOAT)
      {
	  sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				 "DOUBLE PRIMARY KEY NOT NULL", qtable,
				 qpk_name);
      }
    else
      {
	  if (pk_autoincr)
	      sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				     "INTEGER PRIMARY KEY AUTOINCREMENT",
				     qtable, qpk_name);
	  else
	      sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n\"%s\" "
				     "INTEGER NOT NULL PRIMARY KEY", qtable,
				     qpk_name);
      }
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    cnt = 0;
    dbf_field = dbf->Dbf->First;
    while (dbf_field)
      {
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		cnt++;
		continue;
	    }
	  casename = convert_dbf_colname_case (*(col_name + cnt), colname_case);
	  xname = gaiaDoubleQuotedSql (casename);
	  free (casename);
	  sql = sqlite3_mprintf (",\n\"%s\"", xname);
	  free (xname);
	  gaiaAppendToOutBuffer (&sql_statement, sql);
	  sqlite3_free (sql);
	  cnt++;
	  switch (dbf_field->Type)
	    {
	    case 'C':
		gaiaAppendToOutBuffer (&sql_statement, " TEXT");
		break;
	    case 'N':
		if (dbf_field->Decimals)
		    gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		else
		  {
		      if (dbf_field->Length <= 18)
			  gaiaAppendToOutBuffer (&sql_statement, " INTEGER");
		      else
			  gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		  }
		break;
	    case 'D':
		if (text_dates)
		    gaiaAppendToOutBuffer (&sql_statement, " TEXT");
		else
		    gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		break;
	    case 'F':
		gaiaAppendToOutBuffer (&sql_statement, " DOUBLE");
		break;
	    case 'L':
		gaiaAppendToOutBuffer (&sql_statement, " INTEGER");
		break;
	    };
	  dbf_field = dbf_field->Next;
      }
    gaiaAppendToOutBuffer (&sql_statement, ")");
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
      {
	  if (verbose)
	      spatialite_e ("%s;\n", sql_statement.Buffer);
	  ret = sqlite3_exec (sqlite, sql_statement.Buffer, NULL, 0, &errMsg);
      }
    else
	ret = SQLITE_ERROR;
    gaiaOutBufferReset (&sql_statement);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load DBF error: <%s>\n", errMsg);
	  else
	      sprintf (err_msg, "load DBF error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  sqlError = 1;
	  goto clean_up;
      }
    /* preparing the INSERT INTO parametrerized statement */
    sql = sqlite3_mprintf ("INSERT INTO \"%s\" (\"%s\"", qtable, qpk_name);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    cnt = 0;
    dbf_field = dbf->Dbf->First;
    while (dbf_field)
      {
	  /* columns corresponding to some DBF attribute */
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		cnt++;
		continue;
	    }
	  xname = gaiaDoubleQuotedSql (*(col_name + cnt++));
	  sql = sqlite3_mprintf (",\"%s\"", xname);
	  free (xname);
	  gaiaAppendToOutBuffer (&sql_statement, sql);
	  sqlite3_free (sql);
	  dbf_field = dbf_field->Next;
      }
    gaiaAppendToOutBuffer (&sql_statement, ")\nVALUES (?");
    dbf_field = dbf->Dbf->First;
    while (dbf_field)
      {
	  /* column values */
	  if (strcasecmp (pk_name, dbf_field->Name) == 0)
	    {
		/* skipping the Primary Key field */
		dbf_field = dbf_field->Next;
		continue;
	    }
	  gaiaAppendToOutBuffer (&sql_statement, ", ?");
	  dbf_field = dbf_field->Next;
      }
    gaiaAppendToOutBuffer (&sql_statement, ")");
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
	ret =
	    sqlite3_prepare_v2 (sqlite, sql_statement.Buffer,
				strlen (sql_statement.Buffer), &stmt, NULL);
    else
	ret = SQLITE_ERROR;
    gaiaOutBufferReset (&sql_statement);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load DBF error: <%s>\n", sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load DBF error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  sqlError = 1;
	  goto clean_up;
      }
    current_row = 0;
    while (1)
      {
	  /* inserting rows from DBF */
	  ret = gaiaReadDbfEntity_ex (dbf, current_row, &deleted, text_dates);
	  if (!ret)
	    {
		if (!(dbf->LastError))	/* normal DBF EOF */
		    break;
		if (!err_msg)
		    spatialite_e ("%s\n", dbf->LastError);
		else
		    sprintf (err_msg, "%s\n", dbf->LastError);
		sqlError = 1;
		goto clean_up;
	    }
	  current_row++;
	  if (deleted)
	    {
		/* skipping DBF deleted row */
		continue;
	    }
	  /* binding query params */
	  sqlite3_reset (stmt);
	  sqlite3_clear_bindings (stmt);
	  pk_set = 0;
	  cnt = 0;
	  dbf_field = dbf->Dbf->First;
	  while (dbf_field)
	    {
		/* Primary Key value */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      if (pk_type == SQLITE_TEXT)
			  sqlite3_bind_text (stmt, 1,
					     dbf_field->Value->TxtValue,
					     strlen (dbf_field->
						     Value->TxtValue),
					     SQLITE_STATIC);
		      else if (pk_type == SQLITE_FLOAT)
			  sqlite3_bind_double (stmt, 1,
					       dbf_field->Value->DblValue);
		      else
			  sqlite3_bind_int64 (stmt, 1,
					      dbf_field->Value->IntValue);
		      pk_set = 1;
		  }
		dbf_field = dbf_field->Next;
	    }
	  if (!pk_set)
	      sqlite3_bind_int (stmt, 1, current_row);
	  cnt = 0;
	  dbf_field = dbf->Dbf->First;
	  while (dbf_field)
	    {
		/* column values */
		if (strcasecmp (pk_name, dbf_field->Name) == 0)
		  {
		      /* skipping the Primary Key field */
		      dbf_field = dbf_field->Next;
		      continue;
		  }
		if (!(dbf_field->Value))
		    sqlite3_bind_null (stmt, cnt + 2);
		else
		  {
		      switch (dbf_field->Value->Type)
			{
			case GAIA_INT_VALUE:
			    sqlite3_bind_int64 (stmt, cnt + 2,
						dbf_field->Value->IntValue);
			    break;
			case GAIA_DOUBLE_VALUE:
			    sqlite3_bind_double (stmt, cnt + 2,
						 dbf_field->Value->DblValue);
			    break;
			case GAIA_TEXT_VALUE:
			    sqlite3_bind_text (stmt, cnt + 2,
					       dbf_field->Value->TxtValue,
					       strlen (dbf_field->
						       Value->TxtValue),
					       SQLITE_STATIC);
			    break;
			default:
			    sqlite3_bind_null (stmt, cnt + 2);
			    break;
			}
		  }
		cnt++;
		dbf_field = dbf_field->Next;
	    }
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
	      ;
	  else
	    {
		if (!err_msg)
		    spatialite_e ("load DBF error: <%s>\n",
				  sqlite3_errmsg (sqlite));
		else
		    sprintf (err_msg, "load DBF error: <%s>\n",
			     sqlite3_errmsg (sqlite));
		sqlite3_finalize (stmt);
		sqlError = 1;
		goto clean_up;
	    }
      }
    sqlite3_finalize (stmt);
  clean_up:
    if (qtable)
	free (qtable);
    qtable = NULL;
    if (qpk_name)
	free (qpk_name);
    qpk_name = NULL;
    gaiaFreeDbf (dbf);
    if (col_name)
      {
	  /* releasing memory allocation for column names */
	  for (cnt = 0; cnt < col_cnt; cnt++)
	      free (*(col_name + cnt));
	  free (col_name);
      }
    if (sqlError)
      {
	  /* some error occurred - ROLLBACK */
	  if (verbose)
	      spatialite_e ("ROLLBACK;\n");
	  ret = sqlite3_exec (sqlite, "ROLLBACK", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load DBF error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
	    };
	  if (qtable)
	      free (qtable);
	  if (qpk_name)
	      free (qpk_name);
	  return 0;
      }
    else
      {
	  /* ok - confirming pending transaction - COMMIT */
	  if (verbose)
	      spatialite_e ("COMMIT;\n");
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load DBF error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
		return 0;
	    }
	  if (rows)
	      *rows = current_row;
	  if (verbose)
	      spatialite_e ("\nInserted %d rows into '%s' from DBF\n========\n",
			    current_row, table);
	  if (err_msg)
	      sprintf (err_msg, "Inserted %d rows into '%s' from DBF",
		       current_row, table);
	  return 1;
      }
}

#ifdef ENABLE_MINIZIP		/* MINIZIP is enabled */

static struct zip_mem_shp_item *
alloc_zip_mem_shp_item (const char *basename, int wich, int dbf_only)
{
/* creating a Shapefile within a Zipfile */
    struct zip_mem_shp_item *item = malloc (sizeof (struct zip_mem_shp_item));
    if (basename == NULL)
	item->basename = NULL;
    else
      {
	  int len = strlen (basename);
	  item->basename = malloc (len + 1);
	  strcpy (item->basename, basename);
	  if (!dbf_only)
	    {
		if (*(item->basename + len - 4) == '.')
		    *(item->basename + len - 4) = '\0';
	    }
      }
    item->shp = 0;
    item->shx = 0;
    item->dbf = 0;
    item->prj = 0;
    switch (wich)
      {
      case GAIA_ZIPFILE_SHP:
	  item->shp = 1;
	  break;
      case GAIA_ZIPFILE_SHX:
	  item->shx = 1;
	  break;
      case GAIA_ZIPFILE_DBF:
	  item->dbf = 1;
	  break;
      case GAIA_ZIPFILE_PRJ:
	  item->prj = 1;
	  break;
      };
    item->next = NULL;
    return item;
}

static void
add_item_into_zip_mem_shp_list (struct zip_mem_shp_list *list,
				const char *filename, int wich, int dbf_only)
{
/* populating the list of Zipfile Shapefiles */
    struct zip_mem_shp_item *item;

    if (list == NULL)
	return;

    item = list->first;
    while (item != NULL)
      {
	  /* verifying an already existing basename */
	  char *name = NULL;
	  switch (wich)
	    {
	    case GAIA_ZIPFILE_SHP:
		name = sqlite3_mprintf ("%s.shp", item->basename);
		break;
	    case GAIA_ZIPFILE_SHX:
		name = sqlite3_mprintf ("%s.shx", item->basename);
		break;
	    case GAIA_ZIPFILE_DBF:
		name = sqlite3_mprintf ("%s.dbf", item->basename);
		break;
	    case GAIA_ZIPFILE_PRJ:
		name = sqlite3_mprintf ("%s.prj", item->basename);
		break;
	    };
	  if (name != NULL)
	    {
		if (strcasecmp (name, filename) == 0)
		  {
		      /* found a mathcing basename */
		      sqlite3_free (name);
		      switch (wich)
			{
			case GAIA_ZIPFILE_SHP:
			    item->shp = 1;
			    break;
			case GAIA_ZIPFILE_SHX:
			    item->shx = 1;
			    break;
			case GAIA_ZIPFILE_DBF:
			    item->dbf = 1;
			    break;
			case GAIA_ZIPFILE_PRJ:
			    item->prj = 1;
			    break;
			};
		      return;
		  }
		sqlite3_free (name);
	    }
	  item = item->next;
      }

/* inserting a new basename into the list */
    item = alloc_zip_mem_shp_item (filename, wich, dbf_only);
    if (list->first == NULL)
	list->first = item;
    if (list->last != NULL)
	list->last->next = item;
    list->last = item;
}

static struct zip_mem_shapefile *
alloc_zip_mem_shapefile ()
{
/* allocating a Memory Zip Shapefile */
    struct zip_mem_shapefile *mem_shp =
	malloc (sizeof (struct zip_mem_shapefile));
    mem_shp->shp.path = NULL;
    mem_shp->shp.buf = NULL;
    mem_shp->shp.size = 0;
    mem_shp->shp.offset = 0;
    mem_shp->shx.path = NULL;
    mem_shp->shx.buf = NULL;
    mem_shp->shx.size = 0;
    mem_shp->shx.offset = 0;
    mem_shp->dbf.path = NULL;
    mem_shp->dbf.buf = NULL;
    mem_shp->dbf.size = 0;
    mem_shp->dbf.offset = 0;
    mem_shp->prj.path = NULL;
    mem_shp->prj.buf = NULL;
    mem_shp->prj.size = 0;
    mem_shp->prj.offset = 0;
    return mem_shp;
}

static void
destroy_zip_mem_file (gaiaMemFilePtr mem)
{
/* memory cleanup: destroying a Memoroy Zip File */
    if (mem == NULL)
	return;
    if (mem->path != NULL)
	free (mem->path);
    if (mem->buf != NULL)
	free (mem->buf);
}

static void
destroy_zip_mem_shapefile (struct zip_mem_shapefile *mem_shp)
{
/* memory cleanup: destroying a Memory Zip Shapefile */
    if (mem_shp == NULL)
	return;
    destroy_zip_mem_file (&(mem_shp->shp));
    destroy_zip_mem_file (&(mem_shp->shx));
    destroy_zip_mem_file (&(mem_shp->dbf));
    destroy_zip_mem_file (&(mem_shp->prj));
    free (mem_shp);
}

static void
destroy_zip_mem_shp_item (struct zip_mem_shp_item *item)
{
/* destroying a Shapefile item */
    if (item == NULL)
	return;
    if (item->basename != NULL)
	free (item->basename);
    free (item);
}

static struct zip_mem_shp_list *
alloc_zip_mem_shp_list ()
{
/* creating a list of Shapefiles within a Shapefile */
    struct zip_mem_shp_list *list = malloc (sizeof (struct zip_mem_shp_list));
    list->first = NULL;
    list->last = NULL;
    return list;
}

static void
destroy_zip_mem_shp_list (struct zip_mem_shp_list *list)
{
/* memory cleanup: destroying a list of Shapefiles within a Shapefile */
    struct zip_mem_shp_item *item;
    struct zip_mem_shp_item *item_n;

    if (list == NULL)
	return;
    item = list->first;
    while (item != NULL)
      {
	  item_n = item->next;
	  destroy_zip_mem_shp_item (item);
	  item = item_n;
      }
    free (list);
}

static int
do_sniff_zipfile_dir (unzFile uf, struct zip_mem_shp_list *list, int dbf_only)
{
/* sniffing a Zipfile directory */
    int retval = 1;
    unz_global_info64 gi;
    int ret;
    unsigned int i;

    ret = unzGetGlobalInfo64 (uf, &gi);
    if (ret != UNZ_OK)
      {
	  spatialite_e ("Error %d within zipfile in unzGetGlobalInfo\n", ret);
	  return 0;
      }

    for (i = 0; i < gi.number_entry; i++)
      {
	  unsigned int len;
	  char suffix[6];
	  char filename[256];
	  unz_file_info64 file_info;

	  ret =
	      unzGetCurrentFileInfo64 (uf, &file_info, filename, 256, NULL, 0,
				       NULL, 0);
	  if (ret != UNZ_OK)
	    {
		spatialite_e
		    ("Error %d with zipfile in unzGetCurrentFileInfo\n", ret);
		retval = 0;
		goto stop;
	    }

	  suffix[0] = '\0';
	  len = strlen (filename);
	  if (len >= 5)
	    {
		suffix[0] = *(filename + len - 4);
		suffix[1] = *(filename + len - 3);
		suffix[2] = *(filename + len - 2);
		suffix[3] = *(filename + len - 1);
		suffix[4] = '\0';
	    }
	  if (strcasecmp (suffix, ".shp") == 0)
	      add_item_into_zip_mem_shp_list (list, filename, GAIA_ZIPFILE_SHP,
					      0);
	  if (strcasecmp (suffix, ".shx") == 0)
	      add_item_into_zip_mem_shp_list (list, filename, GAIA_ZIPFILE_SHX,
					      0);
	  if (strcasecmp (suffix, ".dbf") == 0)
	      add_item_into_zip_mem_shp_list (list, filename, GAIA_ZIPFILE_DBF,
					      dbf_only);
	  if (strcasecmp (suffix, ".prj") == 0)
	      add_item_into_zip_mem_shp_list (list, filename, GAIA_ZIPFILE_PRJ,
					      0);

	  if (i + 1 == gi.number_entry)
	      break;
	  ret = unzGoToNextFile (uf);
	  if (ret != UNZ_OK)
	    {
		spatialite_e ("Error %d with zipfile in unzGoToNextFile\n",
			      ret);
		retval = 0;
		goto stop;
	    }
      }
  stop:
    return retval;
}

static struct zip_mem_shapefile *
do_list_zipfile_dir (unzFile uf, const char *shp_path, int dbf_mode)
{
/* parsing a Zipfile directory */
    unz_global_info64 gi;
    int ret;
    unsigned int i;
    int len;
    int shp = 0;
    int dbf = 0;
    int shx = 0;
    int prj = 0;
    struct zip_mem_shapefile *mem_shp = NULL;

    ret = unzGetGlobalInfo64 (uf, &gi);
    if (ret != UNZ_OK)
      {
	  spatialite_e ("Error %d within zipfile in unzGetGlobalInfo\n", ret);
	  return 0;
      }

    mem_shp = alloc_zip_mem_shapefile ();
    for (i = 0; i < gi.number_entry; i++)
      {
	  char *path;
	  char filename[256];
	  unz_file_info64 file_info;

	  ret =
	      unzGetCurrentFileInfo64 (uf, &file_info, filename, 256, NULL, 0,
				       NULL, 0);
	  if (ret != UNZ_OK)
	    {
		spatialite_e
		    ("Error %d with zipfile in unzGetCurrentFileInfo\n", ret);
		goto error;
	    }
	  if (dbf_mode)
	    {
		/* searching for just a bare DBF */
		if (!dbf)
		  {
		      if (strcasecmp (shp_path, filename) == 0)
			{
			    dbf = 1;
			    len = strlen (filename);
			    mem_shp->dbf.path = malloc (len + 1);
			    strcpy (mem_shp->dbf.path, filename);
			}
		  }
	    }
	  else
	    {
		/* searching for a Shapefile */
		if (!shp)
		  {
		      path = sqlite3_mprintf ("%s.shp", shp_path);
		      if (strcasecmp (path, filename) == 0)
			{
			    shp = 1;
			    len = strlen (filename);
			    mem_shp->shp.path = malloc (len + 1);
			    strcpy (mem_shp->shp.path, filename);
			}
		      sqlite3_free (path);
		  }
		if (!shx)
		  {
		      path = sqlite3_mprintf ("%s.shx", shp_path);
		      if (strcasecmp (path, filename) == 0)
			{
			    shx = 1;
			    len = strlen (filename);
			    mem_shp->shx.path = malloc (len + 1);
			    strcpy (mem_shp->shx.path, filename);
			}
		      sqlite3_free (path);
		  }
		if (!dbf)
		  {
		      path = sqlite3_mprintf ("%s.dbf", shp_path);
		      if (strcasecmp (path, filename) == 0)
			{
			    dbf = 1;
			    len = strlen (filename);
			    mem_shp->dbf.path = malloc (len + 1);
			    strcpy (mem_shp->dbf.path, filename);
			}
		      sqlite3_free (path);
		  }
		if (!prj)
		  {
		      path = sqlite3_mprintf ("%s.prj", shp_path);
		      if (strcasecmp (path, filename) == 0)
			{
			    prj = 1;
			    len = strlen (filename);
			    mem_shp->prj.path = malloc (len + 1);
			    strcpy (mem_shp->prj.path, filename);
			}
		      sqlite3_free (path);
		  }
	    }

	  if (i + 1 == gi.number_entry)
	      break;
	  ret = unzGoToNextFile (uf);
	  if (ret != UNZ_OK)
	    {
		spatialite_e ("Error %d with zipfile in unzGoToNextFile\n",
			      ret);
		goto error;
	    }
      }
    if (dbf_mode && dbf)
	return mem_shp;
    if (shp && shx && dbf)
	return mem_shp;

  error:
    destroy_zip_mem_shapefile (mem_shp);
    return NULL;
}

static int
do_read_zipfile_file (unzFile uf, struct zip_mem_shapefile *mem_shape, int wich)
{
//
// impo
    int is_open = 0;
    int retval = 1;
    uint64_t size_buf;
    uint64_t rd_cnt;
    uint64_t unrd_cnt;
    unsigned char *buf = NULL;
    char filename[256];
    gaiaMemFilePtr mem_file;
    unz_file_info64 file_info;
    int err;

    switch (wich)
      {
      case GAIA_ZIPFILE_SHP:
	  mem_file = &(mem_shape->shp);
	  break;
      case GAIA_ZIPFILE_SHX:
	  mem_file = &(mem_shape->shx);
	  break;
      case GAIA_ZIPFILE_DBF:
	  mem_file = &(mem_shape->dbf);
	  break;
      case GAIA_ZIPFILE_PRJ:
	  mem_file = &(mem_shape->prj);
	  break;
      default:
	  mem_file = NULL;
      };
    if (mem_file == NULL)
	return 0;
    if (mem_file->path == NULL)
	return 0;

    err = unzLocateFile (uf, mem_file->path, 0);
    if (err != UNZ_OK)
      {
	  spatialite_e ("File %s not found within zipfile\n", mem_file->path);
	  retval = 0;
	  goto skip;
      }
    err =
	unzGetCurrentFileInfo64 (uf, &file_info, filename, 256, NULL, 0, NULL,
				 0);
    if (err != UNZ_OK)
      {
	  spatialite_e ("Error %d with zipfile in unzGetCurrentFileInfo\n",
			err);
	  retval = 0;
	  goto skip;
      }
    size_buf = file_info.uncompressed_size;
    buf = malloc (size_buf);
    err = unzOpenCurrentFile (uf);
    if (err != UNZ_OK)
      {
	  spatialite_e ("Error %d with zipfile in unzGetCurrentFileInfo\n",
			err);
	  retval = 0;
	  goto skip;
      }
    is_open = 1;
    rd_cnt = 0;
    while (rd_cnt < size_buf)
      {
	  /* reading big chunks so to avoid large file issues */
	  uint32_t max = 1000000000;	/* max chunk size */
	  uint32_t len;
	  unrd_cnt = size_buf - rd_cnt;
	  if (unrd_cnt < max)
	      len = unrd_cnt;
	  else
	      len = max;
	  err = unzReadCurrentFile (uf, buf + rd_cnt, len);
	  if (err < 0)
	    {
		spatialite_e ("Error %d with zipfile in unzReadCurrentFile\n",
			      err);
		retval = 0;
		goto skip;
	    }
	  rd_cnt += len;
      }
    mem_file->buf = buf;
    mem_file->size = size_buf;

  skip:
    if (buf != NULL && !retval)
	free (buf);
    if (is_open)
	unzCloseCurrentFile (uf);
    return retval;
}

SPATIALITE_DECLARE int
load_zip_shapefile (sqlite3 * sqlite, const char *zip_path,
		    const char *shp_path, const char *table,
		    const char *charset, int srid, const char *g_column,
		    const char *gtype, const char *pk_column, int coerce2d,
		    int compressed, int verbose, int spatial_index,
		    int text_dates, int *rows, int colname_case, char *err_msg)
{
/* loading a Shapefile from within a Zipfile */
    unzFile uf = NULL;
    int retval = 0;
    struct zip_mem_shapefile *mem_shape = NULL;

    if (zip_path == NULL)
      {
	  spatialite_e ("load zip shapefile error: <%s>\n",
			"NULL zipfile path");
	  return 0;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  return 0;
      }
/* parsing the Zipfile directory */
    mem_shape = do_list_zipfile_dir (uf, shp_path, 0);
    if (mem_shape == NULL)
      {
	  spatialite_e ("No SHP %s with Zipfile\n", shp_path);
	  goto stop;
      }
/* unzipping the SHP member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_SHP))
	goto stop;
/* unzipping the SHX member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_SHX))
	goto stop;
/* unzipping the DBF member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_DBF))
	goto stop;
/* unzipping the PRJ member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_PRJ))
	goto stop;

/* doing the hard work */
    if (load_shapefile_common
	(mem_shape, sqlite, shp_path, table, charset, srid, g_column, gtype,
	 pk_column, coerce2d, compressed, verbose, spatial_index, text_dates,
	 rows, colname_case, err_msg))
	retval = 1;

  stop:
    unzClose (uf);
    if (mem_shape != NULL)
	destroy_zip_mem_shapefile (mem_shape);
    return retval;
}

GAIAGEO_DECLARE gaiaDbfPtr
gaiaOpenZipDbf (const char *zip_path, const char *filename,
		const char *charFrom, const char *charTo)
{
/* creating a DBF object (just for checking its fields) */
    unzFile uf = NULL;
    struct zip_mem_shapefile *mem_shape = NULL;
    gaiaDbfPtr dbf = NULL;

    if (zip_path == NULL)
      {
	  spatialite_e ("open zip dbf error: <%s>\n", "NULL zipfile path");
	  return 0;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  return 0;
      }
/* parsing the Zipfile directory */
    mem_shape = do_list_zipfile_dir (uf, filename, 1);
    if (mem_shape == NULL)
      {
	  spatialite_e ("No DBF %s with Zipfile\n", filename);
	  goto stop;
      }
/* unzipping the DBF member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_DBF))
	goto stop;
/* attempting to create and initialize the DBF object */
    dbf = gaiaAllocDbf ();
    dbf->memDbf = &(mem_shape->dbf);
    gaiaOpenDbfRead (dbf, filename, charFrom, charTo);

  stop:
    unzClose (uf);
    destroy_zip_mem_shapefile (mem_shape);
    return dbf;
}

SPATIALITE_DECLARE int
load_zip_dbf (sqlite3 * sqlite, const char *zip_path, const char *dbf_path,
	      const char *table, const char *pk_column, const char *charset,
	      int verbose, int text_dates, int *rows,
	      int colname_case, char *err_msg)
{
/* loading a DBF file from within a Zipfile */
    unzFile uf = NULL;
    int retval = 0;
    struct zip_mem_shapefile *mem_shape = NULL;

    if (zip_path == NULL)
      {
	  spatialite_e ("load zip shapefile error: <%s>\n",
			"NULL zipfile path");
	  return 0;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  return 0;
      }
/* parsing the Zipfile directory */
    mem_shape = do_list_zipfile_dir (uf, dbf_path, 1);
    if (mem_shape == NULL)
      {
	  spatialite_e ("No DBF %s with Zipfile\n", dbf_path);
	  goto stop;
      }
/* unzipping the DBF member */
    if (!do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_DBF))
	goto stop;

/* doing the hard work */
    if (load_dbf_common
	(mem_shape, sqlite, dbf_path, table, pk_column, charset, verbose,
	 text_dates, rows, colname_case, err_msg))
	retval = 1;

  stop:
    unzClose (uf);
    if (mem_shape != NULL)
	destroy_zip_mem_shapefile (mem_shape);
    return retval;
}

#endif /* end MINIZIP */

#ifndef OMIT_PROJ		/* including PROJ */
#ifdef PROJ_NEW			/* only if new PROJ.6 is supported */
static int
output_prj_file_proj_new (int srid, const char *path, void *proj_ctx)
{
/* attempting to export a genuine ESRI WKT */
    PJ_CONTEXT *ctx = (PJ_CONTEXT *) proj_ctx;
    PJ *crs_def;
    FILE *out;
    const char *projPath;
    const char *auth_name = "EPSG";
    char dummy[64];
    const char *wkt;
    char *prj_path;
    char *options[4];
    options[0] = "MULTILINE=NO";
    options[1] = "INDENTATION_WIDTH=4";
    options[2] = "OUTPUT_AXIS=AUTO";
    options[3] = NULL;

    if (ctx == NULL)
	goto error;
    projPath = proj_context_get_database_path (ctx);
    if (projPath == NULL)
	goto error;		/* PROJ's own SQLite DB is undefined */

/* attemping to get an ESRI WKT from PROJ.6 */
    sprintf (dummy, "%d", srid);
    crs_def =
	proj_create_from_database (ctx, auth_name, dummy, PJ_CATEGORY_CRS, 0,
				   NULL);
    if (crs_def != NULL)
      {
	  wkt =
	      proj_as_wkt (ctx, crs_def, PJ_WKT1_ESRI,
			   (const char *const *) options);
	  if (wkt != NULL)
	    {
		/* generating the .PRJ file */
		prj_path = sqlite3_mprintf ("%s.prj", path);
#ifdef _WIN32
		out = gaia_win_fopen (prj_path, "wb");
#else
		out = fopen (prj_path, "wb");
#endif
		sqlite3_free (prj_path);
		if (out)
		  {
		      fprintf (out, "%s\r\n", wkt);
		      fclose (out);
		  }
	    }
	  proj_destroy (crs_def);
      }
    return 1;

  error:
    return 0;
}
#endif
#endif

static void
output_prj_file (sqlite3 * sqlite, char *path, char *table, char *column,
		 void *proj_ctx)
{
/* exporting [if possible] a .PRJ file */
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;
    int srid = -1;
    char *sql;
    int ret;
    int rs_srid = 0;
    int rs_srs_wkt = 0;
    int rs_srtext = 0;
    int has_srtext = 0;
    const char *name;
    char *srsWkt = NULL;
    FILE *out;

/* step I: retrieving the SRID */
    sql = sqlite3_mprintf ("SELECT srid FROM geometry_columns WHERE "
			   "Lower(f_table_name) = Lower(%Q) AND "
			   "Lower(f_geometry_column) = Lower(%Q)",
			   table, column);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump shapefile MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return;
      }
    for (i = 1; i <= rows; i++)
      {
	  srid = atoi (results[(i * columns) + 0]);
      }
    sqlite3_free_table (results);
    if (srid <= 0)
      {
	  /* srid still undefined, so we'll read VIEWS_GEOMETRY_COLUMNS */
	  sql = sqlite3_mprintf ("SELECT srid FROM views_geometry_columns "
				 "JOIN geometry_columns USING (f_table_name, f_geometry_column) "
				 "WHERE Lower(view_name) = Lower(%Q) AND "
				 "Lower(view_geometry) = Lower(%Q)",
				 table, column);
	  ret =
	      sqlite3_get_table (sqlite, sql, &results, &rows, &columns,
				 &errMsg);
	  sqlite3_free (sql);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("dump shapefile MetaData error: <%s>\n", errMsg);
		sqlite3_free (errMsg);
		return;
	    }
	  for (i = 1; i <= rows; i++)
	    {
		srid = atoi (results[(i * columns) + 0]);
	    }
	  sqlite3_free_table (results);
      }
    if (srid <= 0)
	return;

#ifndef OMIT_PROJ		/* including PROJ */
#ifdef PROJ_NEW
/* if new PROJ.6 is available we'll attempt first to export a genuine ESRI WKT */
    if (proj_ctx != NULL)
      {
	  if (output_prj_file_proj_new (srid, path, proj_ctx))
	      return;
      }
#else
/* suppressing stupid compiler warnings about unused args */
    if (proj_ctx == NULL)
	proj_ctx = NULL;
#endif
#endif

/* step II: checking if the SRS_WKT or SRTEXT column actually exists */
    ret =
	sqlite3_get_table (sqlite, "PRAGMA table_info(spatial_ref_sys)",
			   &results, &rows, &columns, &errMsg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump shapefile MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "srid") == 0)
		    rs_srid = 1;
		if (strcasecmp (name, "srs_wkt") == 0)
		    rs_srs_wkt = 1;
		if (strcasecmp (name, "srtext") == 0)
		    rs_srtext = 1;
	    }
      }
    sqlite3_free_table (results);
    if (rs_srs_wkt == 1 || rs_srtext == 1)
	has_srtext = 1;
    if (rs_srid == 0 || has_srtext == 0)
	return;

/* step III: fetching WKT SRS */
    if (rs_srtext)
      {
	  sql = sqlite3_mprintf ("SELECT srtext FROM spatial_ref_sys "
				 "WHERE srid = %d AND srtext IS NOT NULL",
				 srid);
      }
    else
      {
	  sql = sqlite3_mprintf ("SELECT srs_wkt FROM spatial_ref_sys "
				 "WHERE srid = %d AND srs_wkt IS NOT NULL",
				 srid);
      }
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump shapefile MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  goto end;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		char *srs = results[(i * columns) + 0];
		int len = strlen (srs);
		if (srsWkt)
		    free (srsWkt);
		srsWkt = malloc (len + 1);
		strcpy (srsWkt, srs);
	    }
      }
    sqlite3_free_table (results);
    if (srsWkt == NULL)
	goto end;

/* step IV: generating the .PRJ file */
    sql = sqlite3_mprintf ("%s.prj", path);
#ifdef _WIN32
    out = gaia_win_fopen (sql, "wb");
#else
    out = fopen (sql, "wb");
#endif
    sqlite3_free (sql);
    if (!out)
	goto end;
    fprintf (out, "%s\r\n", srsWkt);
    fclose (out);
  end:
    if (srsWkt)
	free (srsWkt);
    return;
}

static int
get_default_dbf_fields (sqlite3 * sqlite, const char *xtable,
			const char *db_prefix, const char *table_name,
			gaiaDbfListPtr * dbf_export_list)
{
/* creating DBF field definitions for an empty DBF */
    int row = 0;
    char *sql;
    sqlite3_stmt *stmt;
    int ret;
    int offset = 0;
    gaiaDbfListPtr list = NULL;

    if (db_prefix != NULL && table_name != NULL)
      {
	  char *xprefix = gaiaDoubleQuotedSql (db_prefix);
	  char *xxtable = gaiaDoubleQuotedSql (table_name);
	  sql =
	      sqlite3_mprintf ("PRAGMA \"%s\".table_info(\"%s\")", xprefix,
			       xxtable);
	  free (xprefix);
	  free (xxtable);
      }
    else
	sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xtable);
/*
/ compiling SQL prepared statement 
*/

    list = gaiaAllocDbfList ();
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;
    while (1)
      {
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a result set row */
		int xtype = SQLITE_TEXT;
		int length = 60;
		char *name = (char *) sqlite3_column_text (stmt, 1);
		const char *type = (const char *) sqlite3_column_text (stmt, 2);

		if (strcasecmp (type, "INT") == 0
		    || strcasecmp (type, "INTEGER") == 0
		    || strcasecmp (type, "SMALLINT") == 0
		    || strcasecmp (type, "BIGINT") == 0
		    || strcasecmp (type, "TINYINT") == 0)
		    xtype = SQLITE_INTEGER;
		if (strcasecmp (type, "DOUBLE") == 0
		    || strcasecmp (type, "REAL") == 0
		    || strcasecmp (type, "DOUBLE PRECISION") == 0
		    || strcasecmp (type, "NUMERIC") == 0
		    || strcasecmp (type, "FLOAT") == 0)
		    xtype = SQLITE_FLOAT;
		if (strncasecmp (type, "VARCHAR(", 8) == 0)
		    length = atoi (type + 8);
		if (strncasecmp (type, "CHAR(", 5) == 0)
		    length = atoi (type + 5);

		if (xtype == SQLITE_FLOAT)
		  {
		      gaiaAddDbfField (list, name, 'N', offset, 19, 6);
		      offset += 19;
		  }
		else if (xtype == SQLITE_INTEGER)
		  {
		      gaiaAddDbfField (list, name, 'N', offset, 18, 0);
		      offset += 18;
		  }
		else
		  {
		      gaiaAddDbfField (list, name, 'C', offset, (char) length,
				       0);
		      offset += length;
		  }
		row++;
	    }
	  else
	      goto sql_error;
      }
    sqlite3_finalize (stmt);
    if (row == 0)
	goto sql_error;
    *dbf_export_list = list;
    return 1;
  sql_error:
    gaiaFreeDbfList (list);
    *dbf_export_list = NULL;
    return 0;
}

static gaiaVectorLayersListPtr
recover_unregistered_geometry (sqlite3 * handle, const char *table,
			       const char *geometry)
{
/* attempting to recover an unregistered Geometry */
    int len;
    int error = 0;
    gaiaVectorLayersListPtr list;
    gaiaVectorLayerPtr lyr;

/* allocating a VectorLayersList */
    list = malloc (sizeof (gaiaVectorLayersList));
    list->First = NULL;
    list->Last = NULL;
    list->Current = NULL;

    lyr = malloc (sizeof (gaiaVectorLayer));
    lyr->LayerType = GAIA_VECTOR_UNKNOWN;
    len = strlen (table);
    lyr->TableName = malloc (len + 1);
    strcpy (lyr->TableName, table);
    len = strlen (geometry);
    lyr->GeometryName = malloc (len + 1);
    strcpy (lyr->GeometryName, geometry);
    lyr->Srid = 0;
    lyr->GeometryType = GAIA_VECTOR_UNKNOWN;
    lyr->Dimensions = GAIA_VECTOR_UNKNOWN;
    lyr->SpatialIndex = GAIA_SPATIAL_INDEX_NONE;
    lyr->ExtentInfos = NULL;
    lyr->AuthInfos = NULL;
    lyr->First = NULL;
    lyr->Last = NULL;
    lyr->Next = NULL;
    list->Current = NULL;
    if (list->First == NULL)
	list->First = lyr;
    if (list->Last != NULL)
	list->Last->Next = lyr;
    list->Last = lyr;

    if (!doComputeFieldInfos
	(handle, lyr->TableName, lyr->GeometryName,
	 SPATIALITE_STATISTICS_LEGACY, lyr))
	error = 1;

    if (list->First == NULL || error)
      {
	  gaiaFreeVectorLayersList (list);
	  return NULL;
      }
    return list;
}

static int
compute_max_int_length (sqlite3_int64 min, sqlite3_int64 max)
{
/* determining the buffer size for some INT */
    int pos_len = 0;
    int neg_len = 1;
    sqlite3_int64 value = max;
    if (value == 0)
	pos_len = 1;
    else
      {
	  while (value != 0)
	    {
		pos_len++;
		value /= 10;
	    }
      }
    if (min >= 0)
	return pos_len;
    value = min;
    while (value != 0)
      {
	  neg_len++;
	  value /= 10;
      }
    if (neg_len > pos_len)
	return neg_len;
    return pos_len;
}

static int
compute_max_dbl_length (double min, double max)
{
/* determining the buffer size for some DOUBLE */
    int pos_len = 0;
    int neg_len = 1;
    sqlite3_int64 value;
    if (max >= 0.0)
	value = (sqlite3_int64) floor (max);
    else
	value = (sqlite3_int64) ceil (max);
    while (value != 0)
      {
	  pos_len++;
	  value /= 10;
      }
    if (min >= 0.0)
	return pos_len + 7;
    value = (sqlite3_int64) ceil (min);
    while (value != 0)
      {
	  neg_len++;
	  value /= 10;
      }
    if (neg_len > pos_len)
	return neg_len + 7;
    return pos_len + 7;
}

static void
shp_parse_table_name (const char *tn, char **db_prefix, char **table_name)
{
/* attempting to extract an eventual DB prefix */
    int i;
    int len = strlen (tn);
    int i_dot = -1;
    int l_db;
    int l_tbl;
    for (i = 0; i < len; i++)
      {
	  if (tn[i] == '.')
	    {
		i_dot = i;
		break;
	    }
      }
    if (i_dot >= 1)
      {
	  l_db = i_dot;
	  l_tbl = len - (i_dot + 1);
	  *db_prefix = malloc (l_db + 1);
	  memset (*db_prefix, '\0', l_db + 1);
	  memcpy (*db_prefix, tn, l_db);
	  *table_name = malloc (l_tbl + 1);
	  strcpy (*table_name, tn + i_dot + 1);
	  return;
      }
    *table_name = malloc (len + 1);
    strcpy (*table_name, tn);
}

static int
get_attached_layer_v4 (sqlite3 * handle, const char *db_prefix,
		       const char *table, const char *geometry,
		       gaiaVectorLayersListPtr list)
{
/* creating a VectorLayersList object - v.4.0.0 DB layout */
    char *sql;
    int ret;
    sqlite3_stmt *stmt;
    int error = 0;
    char *xdb;

/* querying the vector_layers view */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT layer_type, table_name, geometry_column, geometry_type, "
	 "srid, spatial_index_enabled FROM \"%s\".vector_layers "
	 "WHERE Lower(table_name) = Lower(%Q) AND "
	 "Lower(geometry_column) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  error = 1;
	  goto stop;
      }
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		const char *layer_type =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 1);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 2);
		int geometry_type = sqlite3_column_int (stmt, 3);
		int srid = sqlite3_column_int (stmt, 4);
		int spatial_index = sqlite3_column_int (stmt, 5);
		addVectorLayer (list, layer_type, table_name, geometry_column,
				geometry_type, srid, spatial_index);
	    }
	  else
	      error = 1;
      }
    ret = sqlite3_finalize (stmt);

  stop:
    if (error)
	return 0;

/* querying the vector_layers_statistics view */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT table_name, geometry_column, row_count, extent_min_x, "
	 "extent_min_y, extent_max_x, extent_max_y FROM \"%s\".vector_layers_statistics "
	 "WHERE Lower(table_name) = Lower(%Q) AND "
	 "Lower(geometry_column) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  error = 1;
	  goto stop2;
      }
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		int is_null = 0;
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		int count = 0;
		double min_x = 0.0;
		double min_y = 0.0;
		double max_x = 0.0;
		double max_y = 0.0;
		if (sqlite3_column_type (stmt, 2) == SQLITE_NULL)
		    is_null = 1;
		else
		    count = sqlite3_column_int (stmt, 2);
		if (sqlite3_column_type (stmt, 3) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_x = sqlite3_column_double (stmt, 3);
		if (sqlite3_column_type (stmt, 4) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_y = sqlite3_column_double (stmt, 4);
		if (sqlite3_column_type (stmt, 5) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_x = sqlite3_column_double (stmt, 5);
		if (sqlite3_column_type (stmt, 6) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_y = sqlite3_column_double (stmt, 6);
		if (!is_null)
		    addVectorLayerExtent (list, table_name, geometry_column,
					  count, min_x, min_y, max_x, max_y);
	    }
      }
    ret = sqlite3_finalize (stmt);

  stop2:
/* querying the vector_layers_field_infos view */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT table_name, geometry_column, ordinal, column_name, "
	 "null_values, integer_values, double_values, text_values, blob_values,"
	 "max_size, integer_min, integer_max, double_min, double_max "
	 "FROM \"%s\".vector_layers_field_infos "
	 "WHERE Lower(table_name) = Lower(%Q) AND "
	 "Lower(geometry_column) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  error = 1;
	  goto stop4;
      }
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		int null_max_size = 0;
		int null_int_range = 0;
		int null_double_range = 0;
		int max_size = 0;
		sqlite3_int64 integer_min;
		sqlite3_int64 integer_max;
		double double_min = DBL_MAX;
		double double_max = 0.0 - DBL_MAX;
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		int ordinal = sqlite3_column_int (stmt, 2);
		const char *column_name =
		    (const char *) sqlite3_column_text (stmt, 3);
		int null_values = sqlite3_column_int (stmt, 4);
		int integer_values = sqlite3_column_int (stmt, 5);
		int double_values = sqlite3_column_int (stmt, 6);
		int text_values = sqlite3_column_int (stmt, 7);
		int blob_values = sqlite3_column_int (stmt, 8);
		if (sqlite3_column_type (stmt, 9) == SQLITE_NULL)
		    null_max_size = 1;
		else
		    max_size = sqlite3_column_int (stmt, 9);
		if (sqlite3_column_type (stmt, 10) == SQLITE_NULL
		    || sqlite3_column_type (stmt, 11) == SQLITE_NULL)
		    null_int_range = 1;
		else
		  {
		      integer_min = sqlite3_column_int64 (stmt, 10);
		      integer_max = sqlite3_column_int64 (stmt, 11);
		  }
		if (sqlite3_column_type (stmt, 12) == SQLITE_NULL
		    || sqlite3_column_type (stmt, 13) == SQLITE_NULL)
		    null_double_range = 1;
		else
		  {
		      double_min = sqlite3_column_double (stmt, 12);
		      double_max = sqlite3_column_double (stmt, 13);
		  }
		addLayerAttributeField (list, table_name, geometry_column,
					ordinal, column_name, null_values,
					integer_values, double_values,
					text_values, blob_values, null_max_size,
					max_size, null_int_range, &integer_min,
					&integer_max, null_double_range,
					double_min, double_max);
	    }
      }
    ret = sqlite3_finalize (stmt);

  stop4:
    if (error)
	return 0;
    return 1;
}

static int
check_spatial_metadata (const void *handle, const char *db_prefix)
{
/* internal utility function:
/
/ 0 - if no valid SpatialMetaData were found
/ 1 - if SpatiaLite-like (legacy) SpatialMetadata were found
/ 2 - if FDO-OGR-like SpatialMetadata were found
/ 3 - if SpatiaLite-like (current) SpatialMetadata were found
/
*/
    sqlite3 *sqlite = (sqlite3 *) handle;
    int spatialite_legacy_rs = 0;
    int spatialite_rs = 0;
    int fdo_rs = 0;
    int spatialite_legacy_gc = 0;
    int spatialite_gc = 0;
    int fdo_gc = 0;
    int rs_srid = 0;
    int auth_name = 0;
    int auth_srid = 0;
    int srtext = 0;
    int ref_sys_name = 0;
    int proj4text = 0;
    int f_table_name = 0;
    int f_geometry_column = 0;
    int geometry_type = 0;
    int coord_dimension = 0;
    int gc_srid = 0;
    int geometry_format = 0;
    int type = 0;
    int spatial_index_enabled = 0;
    char *sql;
    int ret;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
    char *xdb;
/* checking the GEOMETRY_COLUMNS table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql = sqlite3_mprintf ("PRAGMA \"%s\".table_info(geometry_columns)", xdb);
    free (xdb);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto unknown;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "f_table_name") == 0)
		    f_table_name = 1;
		if (strcasecmp (name, "f_geometry_column") == 0)
		    f_geometry_column = 1;
		if (strcasecmp (name, "geometry_type") == 0)
		    geometry_type = 1;
		if (strcasecmp (name, "coord_dimension") == 0)
		    coord_dimension = 1;
		if (strcasecmp (name, "srid") == 0)
		    gc_srid = 1;
		if (strcasecmp (name, "geometry_format") == 0)
		    geometry_format = 1;
		if (strcasecmp (name, "type") == 0)
		    type = 1;
		if (strcasecmp (name, "spatial_index_enabled") == 0)
		    spatial_index_enabled = 1;
	    }
      }
    sqlite3_free_table (results);
    if (f_table_name && f_geometry_column && type && coord_dimension
	&& gc_srid && spatial_index_enabled)
	spatialite_legacy_gc = 1;
    if (f_table_name && f_geometry_column && geometry_type && coord_dimension
	&& gc_srid && spatial_index_enabled)
	spatialite_gc = 1;
    if (f_table_name && f_geometry_column && geometry_type && coord_dimension
	&& gc_srid && geometry_format)
	fdo_gc = 1;
/* checking the SPATIAL_REF_SYS table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql = sqlite3_mprintf ("PRAGMA \"%s\".table_info(spatial_ref_sys)", xdb);
    free (xdb);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto unknown;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "srid") == 0)
		    rs_srid = 1;
		if (strcasecmp (name, "auth_name") == 0)
		    auth_name = 1;
		if (strcasecmp (name, "auth_srid") == 0)
		    auth_srid = 1;
		if (strcasecmp (name, "srtext") == 0)
		    srtext = 1;
		if (strcasecmp (name, "ref_sys_name") == 0)
		    ref_sys_name = 1;
		if (strcasecmp (name, "proj4text") == 0)
		    proj4text = 1;
		if (strcasecmp (name, "srtext") == 0)
		    srtext = 1;
	    }
      }
    sqlite3_free_table (results);
    if (rs_srid && auth_name && auth_srid && ref_sys_name && proj4text
	&& srtext)
	spatialite_rs = 1;
    if (rs_srid && auth_name && auth_srid && ref_sys_name && proj4text)
	spatialite_legacy_rs = 1;
    if (rs_srid && auth_name && auth_srid && srtext)
	fdo_rs = 1;
/* verifying the MetaData format */
    if (spatialite_legacy_gc && spatialite_legacy_rs)
	return 1;
    if (fdo_gc && fdo_rs)
	return 2;
    if (spatialite_gc && spatialite_rs)
	return 3;
  unknown:
    return 0;
}

static int
get_attached_table_layer_legacy (sqlite3 * handle, const char *db_prefix,
				 const char *table, const char *geometry,
				 gaiaVectorLayersListPtr list)
{
/* fetching Table-based geometries */
    int ret;
    char *sql;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
    int f_table = 0;
    int f_geometry = 0;
    int type = 0;
    int dims = 0;
    int srid = 0;
    int spatial_index = 0;
    sqlite3_stmt *stmt;
    int error = 0;
    char *xdb;

/* checking the GEOMETRY_COLUMN table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql = sqlite3_mprintf ("PRAGMA \"%s\".table_info(geometry_columns)", xdb);
    free (xdb);
    ret = sqlite3_get_table (handle, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "f_table_name") == 0)
		    f_table = 1;
		if (strcasecmp (name, "f_geometry_column") == 0)
		    f_geometry = 1;
		if (strcasecmp (name, "type") == 0)
		    type = 1;
		if (strcasecmp (name, "coord_dimension") == 0)
		    dims = 1;
		if (strcasecmp (name, "srid") == 0)
		    srid = 1;
		if (strcasecmp (name, "spatial_index_enabled") == 0)
		    spatial_index = 1;
	    }
      }
    sqlite3_free_table (results);
    if (f_table && f_geometry && type && dims && srid && spatial_index)
	;
    else
	return 1;


    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT f_table_name, f_geometry_column, type, coord_dimension, "
	 "srid, spatial_index_enabled FROM \"%s\".geometry_columns "
	 "WHERE Lower(f_table_name) = Lower(%Q) AND "
	 "Lower(f_geometry_column) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  error = 1;
	  goto stop;
      }
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		const char *type = (const char *) sqlite3_column_text (stmt, 2);
		const char *dims = (const char *) sqlite3_column_text (stmt, 3);
		int srid = sqlite3_column_int (stmt, 4);
		int spatial_index = sqlite3_column_int (stmt, 5);
		int geometry_type = -1;
		if (strcasecmp (type, "POINT") == 0)
		    geometry_type = 1;
		if (strcasecmp (type, "LINESTRING") == 0)
		    geometry_type = 2;
		if (strcasecmp (type, "POLYGON") == 0)
		    geometry_type = 3;
		if (strcasecmp (type, "MULTIPOINT") == 0)
		    geometry_type = 4;
		if (strcasecmp (type, "MULTILINESTRING") == 0)
		    geometry_type = 5;
		if (strcasecmp (type, "MULTIPOLYGON") == 0)
		    geometry_type = 6;
		if (strcasecmp (type, "GEOMETRYCOLLECTION") == 0)
		    geometry_type = 7;
		if (strcasecmp (type, "GEOMETRY") == 0)
		    geometry_type = 0;
		if (strcasecmp (dims, "XYZ") == 0
		    || strcasecmp (dims, "3") == 0)
		    geometry_type += 1000;
		if (strcasecmp (dims, "XYM") == 0)
		    geometry_type += 2000;
		if (strcasecmp (dims, "XYZM") == 0
		    || strcasecmp (dims, "4") == 0)
		    geometry_type += 3000;
		addVectorLayer (list, "SpatialTable", table_name,
				geometry_column, geometry_type, srid,
				spatial_index);
	    }
	  else
	      error = 1;
      }
    ret = sqlite3_finalize (stmt);

  stop:
    if (error)
	return 0;
    return 1;
}

static int
get_attached_view_layer_legacy (sqlite3 * handle, const char *db_prefix,
				const char *table, const char *geometry,
				gaiaVectorLayersListPtr list)
{
/* fetching View-based geometries */
    int ret;
    char *sql;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
    int f_table = 0;
    int f_geometry = 0;
    int type = 0;
    int dims = 0;
    int srid = 0;
    int spatial_index = 0;
    int view_name = 0;
    int view_geometry = 0;
    int fvw_table = 0;
    int fvw_geometry = 0;
    sqlite3_stmt *stmt;
    int error = 0;
    char *xdb;

/* checking the GEOMETRY_COLUMN table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql = sqlite3_mprintf ("PRAGMA \"%s\".table_info(geometry_columns)", xdb);
    free (xdb);
    ret = sqlite3_get_table (handle, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "f_table_name") == 0)
		    f_table = 1;
		if (strcasecmp (name, "f_geometry_column") == 0)
		    f_geometry = 1;
		if (strcasecmp (name, "type") == 0)
		    type = 1;
		if (strcasecmp (name, "coord_dimension") == 0)
		    dims = 1;
		if (strcasecmp (name, "srid") == 0)
		    srid = 1;
		if (strcasecmp (name, "spatial_index_enabled") == 0)
		    spatial_index = 1;
	    }
      }
    sqlite3_free_table (results);
    if (f_table && f_geometry && type && dims && srid && spatial_index)
	;
    else
	return 1;

/* checking the VIEWS_GEOMETRY_COLUMN table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf ("PRAGMA \"%s\".table_info(views_geometry_columns)",
			 xdb);
    free (xdb);
    ret = sqlite3_get_table (handle, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "view_name") == 0)
		    view_name = 1;
		if (strcasecmp (name, "view_geometry") == 0)
		    view_geometry = 1;
		if (strcasecmp (name, "f_table_name") == 0)
		    fvw_table = 1;
		if (strcasecmp (name, "f_geometry_column") == 0)
		    fvw_geometry = 1;
	    }
      }
    sqlite3_free_table (results);
    if (view_name && view_geometry && fvw_table && fvw_geometry)
	;
    else
	return 1;

    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT a.view_name, a.view_geometry, b.type, b.coord_dimension, "
	 "b.srid, b.spatial_index_enabled FROM \"%s\".views_geometry_columns AS a "
	 "JOIN \"%s\".geometry_columns AS b ON (Lower(a.f_table_name) = Lower(b.f_table_name) "
	 "AND Lower(a.f_geometry_column) = Lower(b.f_geometry_column)) "
	 "WHERE Lower(a.view_name) = Lower(%Q) AND "
	 "Lower(a.view_geometry) = Lower(%Q)", xdb, xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  error = 1;
	  goto stop;
      }
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		const char *type = (const char *) sqlite3_column_text (stmt, 2);
		const char *dims = (const char *) sqlite3_column_text (stmt, 3);
		int srid = sqlite3_column_int (stmt, 4);
		int spatial_index = sqlite3_column_int (stmt, 5);
		int geometry_type = -1;
		if (strcasecmp (type, "POINT") == 0)
		    geometry_type = 1;
		if (strcasecmp (type, "LINESTRING") == 0)
		    geometry_type = 2;
		if (strcasecmp (type, "POLYGON") == 0)
		    geometry_type = 3;
		if (strcasecmp (type, "MULTIPOINT") == 0)
		    geometry_type = 4;
		if (strcasecmp (type, "MULTILINESTRING") == 0)
		    geometry_type = 5;
		if (strcasecmp (type, "MULTIPOLYGON") == 0)
		    geometry_type = 6;
		if (strcasecmp (type, "GEOMETRYCOLLECTION") == 0)
		    geometry_type = 7;
		if (strcasecmp (type, "GEOMETRY") == 0)
		    geometry_type = 0;
		if (strcasecmp (dims, "XYZ") == 0
		    || strcasecmp (dims, "3") == 0)
		    geometry_type += 1000;
		if (strcasecmp (dims, "XYM") == 0)
		    geometry_type += 2000;
		if (strcasecmp (dims, "XYZM") == 0
		    || strcasecmp (dims, "4") == 0)
		    geometry_type += 3000;
		addVectorLayer (list, "SpatialView", table_name,
				geometry_column, geometry_type, srid,
				spatial_index);
	    }
	  else
	      error = 1;
      }
    ret = sqlite3_finalize (stmt);

  stop:
    if (error)
	return 0;
    return 1;
}

static int
get_attached_table_extent_legacy (sqlite3 * handle, const char *db_prefix,
				  const char *table, const char *geometry,
				  gaiaVectorLayersListPtr list)
{
/* fetching Table-based extents */
    int ret;
    char *sql;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
    int f_table = 0;
    int f_geometry = 0;
    int count = 0;
    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;
    sqlite3_stmt *stmt;
    char *xdb;

/* checking the LAYER_STATISTICS table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql = sqlite3_mprintf ("PRAGMA \"%s\".table_info(layer_statistics)", xdb);
    free (xdb);
    ret = sqlite3_get_table (handle, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "table_name") == 0)
		    f_table = 1;
		if (strcasecmp (name, "geometry_column") == 0)
		    f_geometry = 1;
		if (strcasecmp (name, "row_count") == 0)
		    count = 1;
		if (strcasecmp (name, "extent_min_x") == 0)
		    minx = 1;
		if (strcasecmp (name, "extent_min_y") == 0)
		    miny = 1;
		if (strcasecmp (name, "extent_max_x") == 0)
		    maxx = 1;
		if (strcasecmp (name, "extent_max_y") == 0)
		    maxy = 1;
	    }
      }
    sqlite3_free_table (results);
    if (f_table && f_geometry && count && minx && miny && maxx && maxy)
	;
    else
	return 1;

/* querying the layer_statistics table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT table_name, geometry_column, row_count, extent_min_x, "
	 "extent_min_y, extent_max_x, extent_max_y FROM \"%s\".layer_statistics "
	 "WHERE Lower(table_name) = Lower(%Q) AND "
	 "Lower(geometry_column) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto stop;
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		int is_null = 0;
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		int count = 0;
		double min_x = 0.0;
		double min_y = 0.0;
		double max_x = 0.0;
		double max_y = 0.0;
		if (sqlite3_column_type (stmt, 2) == SQLITE_NULL)
		    is_null = 1;
		else
		    count = sqlite3_column_int (stmt, 2);
		if (sqlite3_column_type (stmt, 3) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_x = sqlite3_column_double (stmt, 3);
		if (sqlite3_column_type (stmt, 4) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_y = sqlite3_column_double (stmt, 4);
		if (sqlite3_column_type (stmt, 5) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_x = sqlite3_column_double (stmt, 5);
		if (sqlite3_column_type (stmt, 6) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_y = sqlite3_column_double (stmt, 6);
		if (!is_null)
		    addVectorLayerExtent (list, table_name, geometry_column,
					  count, min_x, min_y, max_x, max_y);
	    }
      }
    ret = sqlite3_finalize (stmt);
  stop:
    return 1;
}

static int
get_attached_view_extent_legacy (sqlite3 * handle, const char *db_prefix,
				 const char *table, const char *geometry,
				 gaiaVectorLayersListPtr list)
{
/* fetching View-based extents */
    int ret;
    char *sql;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
    int f_table = 0;
    int f_geometry = 0;
    int count = 0;
    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;
    sqlite3_stmt *stmt;
    char *xdb;

/* checking the VIEWS_LAYER_STATISTICS table */
    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf ("PRAGMA \"%s\".table_info(views_layer_statistics)",
			 xdb);
    free (xdb);
    ret = sqlite3_get_table (handle, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, "view_name") == 0)
		    f_table = 1;
		if (strcasecmp (name, "view_geometry") == 0)
		    f_geometry = 1;
		if (strcasecmp (name, "row_count") == 0)
		    count = 1;
		if (strcasecmp (name, "extent_min_x") == 0)
		    minx = 1;
		if (strcasecmp (name, "extent_min_y") == 0)
		    miny = 1;
		if (strcasecmp (name, "extent_max_x") == 0)
		    maxx = 1;
		if (strcasecmp (name, "extent_max_y") == 0)
		    maxy = 1;
	    }
      }
    sqlite3_free_table (results);
    if (f_table && f_geometry && count && minx && miny && maxx && maxy)
	;
    else
	return 1;

/* querying the views_layer_statistics table */

    xdb = gaiaDoubleQuotedSql (db_prefix);
    sql =
	sqlite3_mprintf
	("SELECT view_name, view_geometry, row_count, extent_min_x, "
	 "extent_min_y, extent_max_x, extent_max_y FROM \"%s\".views_layer_statistics "
	 "WHERE Lower(view_name) = Lower(%Q) AND "
	 "Lower(view_geometry) = Lower(%Q)", xdb, table, geometry);
    free (xdb);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (handle, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto stop;
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a VectorLayer row */
		int is_null = 0;
		const char *table_name =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *geometry_column =
		    (const char *) sqlite3_column_text (stmt, 1);
		int count = 0;
		double min_x = DBL_MAX;
		double min_y = DBL_MAX;
		double max_x = 0.0 - DBL_MAX;
		double max_y = 0.0 - DBL_MAX;
		if (sqlite3_column_type (stmt, 2) == SQLITE_NULL)
		    is_null = 1;
		else
		    count = sqlite3_column_int (stmt, 2);
		if (sqlite3_column_type (stmt, 3) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_x = sqlite3_column_double (stmt, 3);
		if (sqlite3_column_type (stmt, 4) == SQLITE_NULL)
		    is_null = 1;
		else
		    min_y = sqlite3_column_double (stmt, 4);
		if (sqlite3_column_type (stmt, 5) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_x = sqlite3_column_double (stmt, 5);
		if (sqlite3_column_type (stmt, 6) == SQLITE_NULL)
		    is_null = 1;
		else
		    max_y = sqlite3_column_double (stmt, 6);
		if (!is_null)
		    addVectorLayerExtent (list, table_name, geometry_column,
					  count, min_x, min_y, max_x, max_y);
	    }
      }
    ret = sqlite3_finalize (stmt);
  stop:
    return 1;
}

static gaiaVectorLayersListPtr
attached_layer (sqlite3 * handle, const char *db_prefix, const char *table,
		const char *geometry)
{
/* attempting to support a "layer" from within an attached DB */
    gaiaVectorLayersListPtr list;
    int metadata_version;
    list = malloc (sizeof (gaiaVectorLayersList));
    list->First = NULL;
    list->Last = NULL;
    list->Current = NULL;

    metadata_version = check_spatial_metadata (handle, db_prefix);
    if (metadata_version == 3)
      {
	  /* current metadata style >= v.4.0.0 */
	  if (!get_attached_layer_v4 (handle, db_prefix, table, geometry, list))
	    {
		gaiaFreeVectorLayersList (list);
		return NULL;
	    }
	  if (list->First == NULL)
	    {
		gaiaFreeVectorLayersList (list);
		return NULL;
	    }
	  return list;
      }

/* legacy metadata style <= v.3.x.x */
    if (!get_attached_table_layer_legacy
	(handle, db_prefix, table, geometry, list))
	goto error;
    if (!get_attached_view_layer_legacy
	(handle, db_prefix, table, geometry, list))
	goto error;
    if (!get_attached_table_extent_legacy
	(handle, db_prefix, table, geometry, list))
	goto error;
    if (!get_attached_view_extent_legacy
	(handle, db_prefix, table, geometry, list))
	goto error;

    if (list->First == NULL)
      {
	  gaiaFreeVectorLayersList (list);
	  return NULL;
      }
    return list;

  error:
    gaiaFreeVectorLayersList (list);
    return NULL;
}

static int
compute_text_length (const char *string, const char *charset)
{
/* computing the actual length of TEXT fields */
    int len = 0;
    char *converted = sqlite3_malloc (strlen (string) + 1);
    strcpy (converted, string);
    if (gaiaConvertCharset (&converted, "UTF-8", charset))
	len = strlen (converted);
    sqlite3_free (converted);
    return len;
}

static void
compute_exact_text_max_length (sqlite3 * sqlite, gaiaDbfListPtr dbf_list,
			       const char *table, const char *charset)
{
/* computing the exact max length of TEXT fields */
    char *sql;
    char *prevsql;
    char *xtable;
    gaiaDbfFieldPtr fld;
    int first = 1;
    int ok = 0;
    int ret;
    sqlite3_stmt *stmt = NULL;
    int offset = 0;

/* preparing the SQL query */
    sql = sqlite3_mprintf ("SELECT");
    prevsql = sql;
    fld = dbf_list->First;
    while (fld)
      {
	  char *xcolumn;
	  if (fld->Type != 'C')
	      goto skip_field;
	  ok = 1;
	  xcolumn = gaiaDoubleQuotedSql (fld->Name);
	  if (first)
	    {
		sql = sqlite3_mprintf ("%s \"%s\"", prevsql, xcolumn);
		first = 0;
	    }
	  else
	      sql = sqlite3_mprintf ("%s, \"%s\"", prevsql, xcolumn);
	  free (xcolumn);
	  sqlite3_free (prevsql);
	  prevsql = sql;

	skip_field:
	  fld = fld->Next;
      }
    if (!ok)
      {
	  sqlite3_free (sql);
	  return;
      }

    xtable = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("%s FROM \"%s\"", prevsql, xtable);
    sqlite3_free (prevsql);
    free (xtable);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return;
    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		int col = 0;
		fld = dbf_list->First;
		while (fld)
		  {
		      if (fld->Type != 'C')
			  goto skip_field2;
		      if (sqlite3_column_type (stmt, col) == SQLITE_TEXT)
			{
			    const char *string = (const char *)
				sqlite3_column_text (stmt, col);
			    char *converted =
				sqlite3_malloc (strlen (string) + 1);
			    strcpy (converted, string);
			    if (gaiaConvertCharset
				(&converted, "UTF-8", charset))
			      {
				  /* we need to determine the field length _AFTER_ converting to the output charset */
				  int len = strlen (converted);
				  if (len > fld->Length)
				      fld->Length = len;
				  sqlite3_free (converted);
			      }
			    col++;
			}
		    skip_field2:
		      fld = fld->Next;
		  }
	    }
      }
    sqlite3_finalize (stmt);

    fld = dbf_list->First;
    while (fld)
      {
	  /* readjusting all offsets */
	  fld->Offset = offset;
	  offset += fld->Length;
	  fld = fld->Next;
      }
}

SPATIALITE_DECLARE int
dump_shapefile (sqlite3 * sqlite, char *table, char *column, char *shp_path,
		char *charset, char *geom_type, int verbose, int *xrows,
		char *err_msg)
{
    return dump_shapefile_ex (sqlite, table, column, shp_path, charset,
			      geom_type, verbose, xrows,
			      GAIA_DBF_COLNAME_CASE_IGNORE, err_msg);
}

SPATIALITE_DECLARE int
dump_shapefile_ex (sqlite3 * sqlite, char *table, char *column, char *shp_path,
		   char *charset, char *geom_type, int verbose, int *xrows,
		   int colname_case, char *err_msg)
{
    return dump_shapefile_ex2 (sqlite, NULL, table, column, shp_path, charset,
			       geom_type, verbose, xrows,
			       colname_case, err_msg);
}

SPATIALITE_DECLARE int
dump_shapefile_ex2 (sqlite3 * sqlite, void *proj_ctx, char *table, char *column,
		    char *shp_path, char *charset, char *geom_type, int verbose,
		    int *xrows, int colname_case, char *err_msg)
{
/* SHAPEFILE dump */
    char *sql;
    char *dummy;
    int shape = -1;
    int len;
    int ret;
    sqlite3_stmt *stmt;
    int n_cols = 0;
    int offset = 0;
    int i;
    int rows = 0;
    char buf[256];
    char *xtable;
    char *xcolumn;
    const void *blob_value;
    gaiaShapefilePtr shp = NULL;
    gaiaDbfListPtr dbf_list = NULL;
    gaiaDbfListPtr dbf_write;
    gaiaDbfFieldPtr dbf_field;
    gaiaVectorLayerPtr lyr = NULL;
    gaiaLayerAttributeFieldPtr fld;
    gaiaVectorLayersListPtr list;
    char *db_prefix = NULL;
    char *table_name = NULL;
    char *xprefix;
    char *xxtable;
    struct auxdbf_list *auxdbf = NULL;

    if (xrows)
	*xrows = -1;
    if (geom_type)
      {
	  /* normalizing required geometry type */
	  if (strcasecmp ((char *) geom_type, "POINT") == 0)
	      shape = GAIA_POINT;
	  if (strcasecmp ((char *) geom_type, "LINESTRING") == 0)
	      shape = GAIA_LINESTRING;
	  if (strcasecmp ((char *) geom_type, "POLYGON") == 0)
	      shape = GAIA_POLYGON;
	  if (strcasecmp ((char *) geom_type, "MULTIPOINT") == 0)
	      shape = GAIA_MULTIPOINT;
      }
/* is the datasource a genuine registered Geometry ?? */
    list = gaiaGetVectorLayersList (sqlite, table, column,
				    GAIA_VECTORS_LIST_PESSIMISTIC);
    if (list == NULL)
      {
	  /* attempting to recover an unregistered Geometry */
	  list = recover_unregistered_geometry (sqlite, table, column);
      }

    if (list == NULL)
      {
	  /* attempting to enucleate an eventual DB-prefix */
	  shp_parse_table_name (table, &db_prefix, &table_name);
	  if (db_prefix != NULL && table_name != NULL)
	      list = attached_layer (sqlite, db_prefix, table_name, column);
      }

    if (list != NULL)
	lyr = list->First;
    if (lyr == NULL)
      {
	  gaiaFreeVectorLayersList (list);
	  if (!err_msg)
	      spatialite_e
		  ("Unable to detect GeometryType for \"%s\".\"%s\" ... sorry\n",
		   table, column);
	  else
	      sprintf (err_msg,
		       "Unable to detect GeometryType for \"%s\".\"%s\" ... sorry\n",
		       table, column);
	  return 0;
      }

    switch (lyr->GeometryType)
      {
      case GAIA_VECTOR_POINT:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_POINT;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_POINTZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_POINTM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_POINTZM;
		break;
	    };
	  break;
      case GAIA_VECTOR_LINESTRING:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_LINESTRING;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_LINESTRINGZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_LINESTRINGM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_LINESTRINGZM;
		break;
	    };
	  break;
      case GAIA_VECTOR_POLYGON:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_POLYGON;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_POLYGONZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_POLYGONM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_POLYGONZM;
		break;
	    };
	  break;
      case GAIA_VECTOR_MULTIPOINT:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_MULTIPOINT;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_MULTIPOINTZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_MULTIPOINTM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_MULTIPOINTZM;
		break;
	    };
	  break;
      case GAIA_VECTOR_MULTILINESTRING:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_MULTILINESTRING;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_MULTILINESTRINGZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_MULTILINESTRINGM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_MULTILINESTRINGZM;
		break;
	    };
	  break;
      case GAIA_VECTOR_MULTIPOLYGON:
	  switch (lyr->Dimensions)
	    {
	    case GAIA_XY:
		shape = GAIA_MULTIPOLYGON;
		break;
	    case GAIA_XY_Z:
		shape = GAIA_MULTIPOLYGONZ;
		break;
	    case GAIA_XY_M:
		shape = GAIA_MULTIPOLYGONM;
		break;
	    case GAIA_XY_Z_M:
		shape = GAIA_MULTIPOLYGONZM;
		break;
	    };
	  break;
      };

    if (shape < 0)
      {
	  if (!err_msg)
	      spatialite_e
		  ("Unable to detect GeometryType for \"%s\".\"%s\" ... sorry\n",
		   table, column);
	  else
	      sprintf (err_msg,
		       "Unable to detect GeometryType for \"%s\".\"%s\" ... sorry\n",
		       table, column);
	  return 0;
      }
    if (verbose)
	spatialite_e
	    ("========\nDumping SQLite table '%s' into shapefile at '%s'\n",
	     table, shp_path);
    /* preparing SQL statement */
    xtable = gaiaDoubleQuotedSql (table);
    xcolumn = gaiaDoubleQuotedSql (column);
    if (shape == GAIA_LINESTRING || shape == GAIA_LINESTRINGZ
	|| shape == GAIA_LINESTRINGM || shape == GAIA_LINESTRINGZM ||
	shape == GAIA_MULTILINESTRING || shape == GAIA_MULTILINESTRINGZ
	|| shape == GAIA_MULTILINESTRINGM || shape == GAIA_MULTILINESTRINGZM)
      {
	  if (db_prefix != NULL && table_name != NULL)
	    {
		xprefix = gaiaDoubleQuotedSql (db_prefix);
		xxtable = gaiaDoubleQuotedSql (table_name);
		sql =
		    sqlite3_mprintf
		    ("SELECT * FROM \"%s\".\"%s\" WHERE GeometryAliasType(\"%w\") = "
		     "'LINESTRING' OR GeometryAliasType(\"%w\") = 'MULTILINESTRING' "
		     "OR \"%s\" IS NULL", xprefix, xxtable, column, column,
		     xcolumn);
		free (xprefix);
		free (xxtable);
	    }
	  else
	      sql =
		  sqlite3_mprintf
		  ("SELECT * FROM \"%s\" WHERE GeometryAliasType(\"%w\") = "
		   "'LINESTRING' OR GeometryAliasType(\"%w\") = 'MULTILINESTRING' "
		   "OR \"%s\" IS NULL", xtable, column, column, xcolumn);
      }
    else if (shape == GAIA_POLYGON || shape == GAIA_POLYGONZ
	     || shape == GAIA_POLYGONM || shape == GAIA_POLYGONZM ||
	     shape == GAIA_MULTIPOLYGON || shape == GAIA_MULTIPOLYGONZ
	     || shape == GAIA_MULTIPOLYGONM || shape == GAIA_MULTIPOLYGONZM)
      {
	  if (db_prefix != NULL && table_name != NULL)
	    {
		xprefix = gaiaDoubleQuotedSql (db_prefix);
		xxtable = gaiaDoubleQuotedSql (table_name);
		sql =
		    sqlite3_mprintf
		    ("SELECT * FROM \"%s\".\"%s\" WHERE GeometryAliasType(\"%w\") = "
		     "'POLYGON' OR GeometryAliasType(\"%w\") = 'MULTIPOLYGON'"
		     "OR \"%s\" IS NULL", xprefix, xxtable, column, column,
		     xcolumn);
		free (xprefix);
		free (xxtable);
	    }
	  else
	      sql =
		  sqlite3_mprintf
		  ("SELECT * FROM \"%s\" WHERE GeometryAliasType(\"%w\") = "
		   "'POLYGON' OR GeometryAliasType(\"%w\") = 'MULTIPOLYGON'"
		   "OR \"%s\" IS NULL", xtable, column, column, xcolumn);
      }
    else if (shape == GAIA_MULTIPOINT || shape == GAIA_MULTIPOINTZ
	     || shape == GAIA_MULTIPOINTM || shape == GAIA_MULTIPOINTZM)
      {
	  if (db_prefix != NULL && table_name != NULL)
	    {
		xprefix = gaiaDoubleQuotedSql (db_prefix);
		xxtable = gaiaDoubleQuotedSql (table_name);
		sql =
		    sqlite3_mprintf
		    ("SELECT * FROM \"%s\".\"%s\" WHERE GeometryAliasType(\"%w\") = "
		     "'POINT' OR GeometryAliasType(\"%w\") = 'MULTIPOINT'"
		     "OR \"%s\" IS NULL", xprefix, xxtable, column, column,
		     xcolumn);
		free (xprefix);
		free (xxtable);
	    }
	  else
	      sql =
		  sqlite3_mprintf
		  ("SELECT * FROM \"%s\" WHERE GeometryAliasType(\"%w\") = "
		   "'POINT' OR GeometryAliasType(\"%w\") = 'MULTIPOINT'"
		   "OR \"%s\" IS NULL", xtable, column, column, xcolumn);
      }
    else
      {
	  if (db_prefix != NULL && table_name != NULL)
	    {
		xprefix = gaiaDoubleQuotedSql (db_prefix);
		xxtable = gaiaDoubleQuotedSql (table_name);
		sql =
		    sqlite3_mprintf
		    ("SELECT * FROM \"%s\".\"%s\" WHERE GeometryAliasType(\"%w\") = "
		     "'POINT' OR \"%s\" IS NULL", xprefix, xxtable, column,
		     xcolumn);
		free (xprefix);
		free (xxtable);
	    }
	  else
	      sql =
		  sqlite3_mprintf
		  ("SELECT * FROM \"%s\" WHERE GeometryAliasType(\"%w\") = "
		   "'POINT' OR \"%s\" IS NULL", xtable, column, xcolumn);
      }
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;

    if (lyr->First == NULL)
      {
	  /* the datasource is probably empty - zero rows */
	  if (get_default_dbf_fields
	      (sqlite, xtable, db_prefix, table_name, &dbf_list))
	      goto continue_exporting;
      }

/* preparing the DBF fields list */
    dbf_list = gaiaAllocDbfList ();
    offset = 0;
    fld = lyr->First;
    while (fld)
      {
	  int sql_type = SQLITE_NULL;
	  int max_len = 0;
	  if (strcasecmp (fld->AttributeFieldName, column) == 0)
	    {
		/* ignoring the Geometry itself */
		fld = fld->Next;
		continue;
	    }
	  if (fld->IntegerValuesCount > 0 && fld->DoubleValuesCount == 0
	      && fld->TextValuesCount == 0)
	    {
		sql_type = SQLITE_INTEGER;
		max_len = 18;
		if (fld->IntRange)
		    max_len =
			compute_max_int_length (fld->IntRange->MinValue,
						fld->IntRange->MaxValue);
	    }
	  if (fld->DoubleValuesCount > 0 && fld->TextValuesCount == 0)
	    {
		sql_type = SQLITE_FLOAT;
		max_len = 19;
		if (fld->DoubleRange)
		    max_len =
			compute_max_dbl_length (fld->DoubleRange->MinValue,
						fld->DoubleRange->MaxValue);
	    }
	  if (fld->TextValuesCount > 0)
	    {
		sql_type = SQLITE_TEXT;
		max_len = 254;
		if (fld->MaxSize)
		    max_len = fld->MaxSize->MaxSize;
	    }
	  if (sql_type == SQLITE_NULL)
	    {
		/* considering as TEXT(1) */
		sql_type = SQLITE_TEXT;
		max_len = 1;
	    }
	  /* adding a DBF field */
	  if (sql_type == SQLITE_TEXT)
	    {
		if (max_len == 0)	/* avoiding ZERO-length fields */
		    max_len = 1;
		if (max_len > 254)
		  {
		      /* DBF C: max allowed lenght */
		      max_len = 254;
		  }
		gaiaAddDbfField (dbf_list, fld->AttributeFieldName, 'C', offset,
				 (unsigned char) max_len, 0);
		offset += max_len;
	    }
	  if (sql_type == SQLITE_FLOAT)
	    {
		if (max_len > 19)
		    max_len = 19;
		if (max_len < 8)
		    max_len = 8;
		gaiaAddDbfField (dbf_list, fld->AttributeFieldName, 'N', offset,
				 (unsigned char) max_len, 6);
		offset += max_len;
	    }
	  if (sql_type == SQLITE_INTEGER)
	    {
		if (max_len > 18)
		    max_len = 18;
		gaiaAddDbfField (dbf_list, fld->AttributeFieldName, 'N', offset,
				 (unsigned char) max_len, 0);
		offset += max_len;
	    }
	  fld = fld->Next;
      }

/* exact computation of TEXT fields max length */
    compute_exact_text_max_length (sqlite, dbf_list, table, charset);

/* resetting SQLite query */
  continue_exporting:
    ret = sqlite3_reset (stmt);
    if (ret != SQLITE_OK)
	goto sql_error;
/* trying to open shapefile files */
    shp = gaiaAllocShapefile ();
    gaiaOpenShpWriteEx (shp, shp_path, shape, dbf_list, "UTF-8", charset,
			colname_case);
    if (!(shp->Valid))
	goto no_file;
/* trying to export the .PRJ file */
    output_prj_file (sqlite, shp_path, table, column, proj_ctx);
    while (1)
      {
	  /* scrolling the result set to dump data into shapefile */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		if (n_cols == 0)
		    n_cols = sqlite3_column_count (stmt);
		rows++;
		dbf_write = gaiaCloneDbfEntity (dbf_list);
		auxdbf = alloc_auxdbf (dbf_write);
		for (i = 0; i < n_cols; i++)
		  {
		      if (strcasecmp
			  ((char *) column,
			   (char *) sqlite3_column_name (stmt, i)) == 0)
			{
			    /* this one is the internal BLOB encoded GEOMETRY to be exported */
			    if (sqlite3_column_type (stmt, i) != SQLITE_BLOB)
			      {
				  /* this one is a NULL Geometry */
				  dbf_write->Geometry = NULL;
			      }
			    else
			      {
				  blob_value = sqlite3_column_blob (stmt, i);
				  len = sqlite3_column_bytes (stmt, i);
				  dbf_write->Geometry =
				      gaiaFromSpatiaLiteBlobWkb (blob_value,
								 len);
			      }
			}
		      dummy = (char *) sqlite3_column_name (stmt, i);
		      dbf_field = getDbfField (auxdbf, dummy);
		      if (!dbf_field)
			  continue;
		      if (sqlite3_column_type (stmt, i) == SQLITE_NULL)
			{
			    /* handling NULL values */
			    gaiaSetNullValue (dbf_field);
			}
		      else
			{
			    switch (dbf_field->Type)
			      {
			      case 'N':
				  if (sqlite3_column_type (stmt, i) ==
				      SQLITE_INTEGER)
				      gaiaSetIntValue (dbf_field,
						       sqlite3_column_int64
						       (stmt, i));
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_FLOAT)
				      gaiaSetDoubleValue (dbf_field,
							  sqlite3_column_double
							  (stmt, i));
				  else
				      gaiaSetNullValue (dbf_field);
				  break;
			      case 'C':
				  if (sqlite3_column_type (stmt, i) ==
				      SQLITE_TEXT)
				    {
					dummy =
					    (char *) sqlite3_column_text (stmt,
									  i);
					gaiaSetStrValue (dbf_field, dummy);
				    }
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_INTEGER)
				    {
					sprintf (buf, FRMT64,
						 sqlite3_column_int64 (stmt,
								       i));
					gaiaSetStrValue (dbf_field, buf);
				    }
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_FLOAT)
				    {
					sql = sqlite3_mprintf ("%1.6f",
							       sqlite3_column_double
							       (stmt, i));
					gaiaSetStrValue (dbf_field, sql);
					sqlite3_free (sql);
				    }
				  else
				      gaiaSetNullValue (dbf_field);
				  break;
			      };
			}
		  }
		free_auxdbf (auxdbf);
		auxdbf = NULL;
		if (!gaiaWriteShpEntity (shp, dbf_write))
		    spatialite_e ("shapefile write error\n");
		gaiaFreeDbfList (dbf_write);
	    }
	  else
	      goto sql_error;
      }
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    sqlite3_finalize (stmt);
    gaiaFlushShpHeaders (shp);
    gaiaFreeShapefile (shp);
    free (xtable);
    free (xcolumn);
    gaiaFreeVectorLayersList (list);
    if (verbose)
	spatialite_e ("\nExported %d rows into SHAPEFILE\n========\n", rows);
    if (xrows)
	*xrows = rows;
    if (err_msg)
	sprintf (err_msg, "Exported %d rows into SHAPEFILE", rows);
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 1;
  sql_error:
/* some SQL error occurred */
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    sqlite3_finalize (stmt);
    free (xtable);
    free (xcolumn);
    gaiaFreeVectorLayersList (list);
    if (dbf_list)
	gaiaFreeDbfList (dbf_list);
    if (shp)
	gaiaFreeShapefile (shp);
    if (!err_msg)
	spatialite_e ("SELECT failed: %s", sqlite3_errmsg (sqlite));
    else
	sprintf (err_msg, "SELECT failed: %s", sqlite3_errmsg (sqlite));
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 0;
  no_file:
/* shapefile can't be created/opened */
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    free (xtable);
    free (xcolumn);
    gaiaFreeVectorLayersList (list);
    if (dbf_list)
	gaiaFreeDbfList (dbf_list);
    if (shp)
	gaiaFreeShapefile (shp);
    if (!err_msg)
	spatialite_e ("ERROR: unable to open '%s' for writing", shp_path);
    else
	sprintf (err_msg, "ERROR: unable to open '%s' for writing", shp_path);
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 0;
}

SPATIALITE_DECLARE int
load_dbf (sqlite3 * sqlite, char *dbf_path, char *table, char *charset,
	  int verbose, int *rows, char *err_msg)
{
    return load_dbf_ex (sqlite, dbf_path, table, NULL, charset, verbose, rows,
			err_msg);
}

SPATIALITE_DECLARE int
load_dbf_ex (sqlite3 * sqlite, char *dbf_path, char *table, char *pk_column,
	     char *charset, int verbose, int *rows, char *err_msg)
{
    return load_dbf_ex2 (sqlite, dbf_path, table, pk_column, charset, verbose,
			 0, rows, err_msg);
}

SPATIALITE_DECLARE int
load_dbf_ex2 (sqlite3 * sqlite, char *dbf_path, char *table, char *pk_column,
	      char *charset, int verbose, int text_dates, int *rows,
	      char *err_msg)
{
    return load_dbf_ex3 (sqlite, dbf_path, table, pk_column, charset, verbose,
			 text_dates, rows, GAIA_DBF_COLNAME_LOWERCASE, err_msg);
}

SPATIALITE_DECLARE int
load_dbf_ex3 (sqlite3 * sqlite, const char *dbf_path, const char *table,
	      const char *pk_column, const char *charset, int verbose,
	      int text_dates, int *rows, int colname_case, char *err_msg)
{
    return load_dbf_common (NULL, sqlite, dbf_path, table, pk_column, charset,
			    verbose, text_dates, rows, colname_case, err_msg);
}

SPATIALITE_DECLARE int
dump_dbf (sqlite3 * sqlite, char *table, char *dbf_path, char *charset,
	  char *err_msg)
{
    int rows;
    return dump_dbf_ex (sqlite, table, dbf_path, charset, &rows, err_msg);
}

SPATIALITE_DECLARE int
dump_dbf_ex (sqlite3 * sqlite, char *table, char *dbf_path, char *charset,
	     int *xrows, char *err_msg)
{
    return dump_dbf_ex2 (sqlite, table, dbf_path, charset, xrows,
			 GAIA_DBF_COLNAME_CASE_IGNORE, err_msg);
}

SPATIALITE_DECLARE int
dump_dbf_ex2 (sqlite3 * sqlite, char *table, char *dbf_path, char *charset,
	      int *xrows, int colname_case, char *err_msg)
{
/* DBF dump */
    int rows;
    int i;
    char *sql;
    char *xtable = NULL;
    sqlite3_stmt *stmt;
    int row1 = 0;
    int n_cols = 0;
    int offset = 0;
    int type;
    gaiaDbfPtr dbf = NULL;
    gaiaDbfListPtr dbf_export_list = NULL;
    gaiaDbfListPtr dbf_list = NULL;
    gaiaDbfListPtr dbf_write;
    gaiaDbfFieldPtr dbf_field;
    int *max_length = NULL;
    int *sql_type = NULL;
    char *dummy;
    char buf[256];
    int len;
    int ret;
    char *db_prefix = NULL;
    char *table_name = NULL;
    struct auxdbf_list *auxdbf = NULL;

    *xrows = -1;
    shp_parse_table_name (table, &db_prefix, &table_name);
/*
/ preparing SQL statement 
*/
    if (db_prefix != NULL && table_name != NULL)
      {
	  char *xdb = gaiaDoubleQuotedSql (db_prefix);
	  char *xxtable = gaiaDoubleQuotedSql (table_name);
	  sql = sqlite3_mprintf ("SELECT * FROM \"%s\".\"%s\"", xdb, xxtable);
	  free (xdb);
	  free (xxtable);
      }
    else
      {
	  xtable = gaiaDoubleQuotedSql (table);
	  sql = sqlite3_mprintf ("SELECT * FROM \"%s\"", xtable);
      }
/*
/ compiling SQL prepared statement 
*/
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;
    rows = 0;
    while (1)
      {
	  /*
	     / Pass I - scrolling the result set to compute real DBF attributes' sizes and types 
	   */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a result set row */
		row1++;
		if (n_cols == 0)
		  {
		      /* this one is the first row, so we are going to prepare the DBF Fields list */
		      n_cols = sqlite3_column_count (stmt);
		      dbf_export_list = gaiaAllocDbfList ();
		      max_length = (int *) malloc (sizeof (int) * n_cols);
		      sql_type = (int *) malloc (sizeof (int) * n_cols);
		      for (i = 0; i < n_cols; i++)
			{
			    /* initializes the DBF export fields */
			    dummy = (char *) sqlite3_column_name (stmt, i);
			    gaiaAddDbfField (dbf_export_list, dummy, '\0', 0, 0,
					     0);
			    max_length[i] = 0;
			    sql_type[i] = SQLITE_NULL;
			}
		  }
		for (i = 0; i < n_cols; i++)
		  {
		      /* update the DBF export fields analyzing fetched data */
		      type = sqlite3_column_type (stmt, i);
		      if (type == SQLITE_NULL || type == SQLITE_BLOB)
			  continue;
		      if (type == SQLITE_TEXT)
			{
			    const char *string =
				(const char *) sqlite3_column_text (stmt, i);
			    len = compute_text_length (string, charset);
			    if (len > 254)
			      {
				  /* DBF C type: max allowed length */
				  len = 254;
			      }
			    sql_type[i] = SQLITE_TEXT;
			    if (len > max_length[i])
				max_length[i] = len;
			}
		      else if (type == SQLITE_FLOAT
			       && sql_type[i] != SQLITE_TEXT)
			  sql_type[i] = SQLITE_FLOAT;	/* promoting a numeric column to be DOUBLE */
		      else if (type == SQLITE_INTEGER
			       && (sql_type[i] == SQLITE_NULL
				   || sql_type[i] == SQLITE_INTEGER))
			  sql_type[i] = SQLITE_INTEGER;	/* promoting a null column to be INTEGER */
		      if (type == SQLITE_INTEGER && max_length[i] < 18)
			  max_length[i] = 18;
		      if (type == SQLITE_FLOAT && max_length[i] < 24)
			  max_length[i] = 24;
		  }
	    }
	  else
	      goto sql_error;
      }
    if (!row1)
	goto empty_result_set;
    i = 0;
    offset = 0;
    dbf_list = gaiaAllocDbfList ();
    dbf_field = dbf_export_list->First;
    while (dbf_field)
      {
	  /* preparing the final DBF attribute list */
	  if (sql_type[i] == SQLITE_NULL || sql_type[i] == SQLITE_BLOB)
	    {
		i++;
		dbf_field = dbf_field->Next;
		continue;
	    }
	  if (sql_type[i] == SQLITE_TEXT)
	    {
		gaiaAddDbfField (dbf_list, dbf_field->Name, 'C', offset,
				 (unsigned char) (max_length[i]), 0);
		offset += max_length[i];
	    }
	  if (sql_type[i] == SQLITE_FLOAT)
	    {
		gaiaAddDbfField (dbf_list, dbf_field->Name, 'N', offset, 19, 6);
		offset += 19;
	    }
	  if (sql_type[i] == SQLITE_INTEGER)
	    {
		gaiaAddDbfField (dbf_list, dbf_field->Name, 'N', offset, 18, 0);
		offset += 18;
	    }
	  i++;
	  dbf_field = dbf_field->Next;
      }
    free (max_length);
    free (sql_type);
    gaiaFreeDbfList (dbf_export_list);
    dbf_export_list = NULL;

  continue_exporting:
/* resetting SQLite query */
    ret = sqlite3_reset (stmt);
    if (ret != SQLITE_OK)
	goto sql_error;
/* trying to open the DBF file */
    dbf = gaiaAllocDbf ();
/* xfering export-list ownership */
    dbf->Dbf = dbf_list;
    dbf_list = NULL;
    gaiaOpenDbfWriteEx (dbf, dbf_path, "UTF-8", charset, colname_case);
    if (!(dbf->Valid))
	goto no_file;
    while (1)
      {
	  /* Pass II - scrolling the result set to dump data into DBF */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		rows++;
		dbf_write = gaiaCloneDbfEntity (dbf->Dbf);
		auxdbf = alloc_auxdbf (dbf_write);
		for (i = 0; i < n_cols; i++)
		  {
		      dummy = (char *) sqlite3_column_name (stmt, i);
		      dbf_field = getDbfField (auxdbf, dummy);
		      if (!dbf_field)
			  continue;
		      if (sqlite3_column_type (stmt, i) == SQLITE_NULL
			  || sqlite3_column_type (stmt, i) == SQLITE_BLOB)
			{
			    /* handling NULL values */
			    gaiaSetNullValue (dbf_field);
			}
		      else
			{
			    switch (dbf_field->Type)
			      {
			      case 'N':
				  if (sqlite3_column_type (stmt, i) ==
				      SQLITE_INTEGER)
				      gaiaSetIntValue (dbf_field,
						       sqlite3_column_int64
						       (stmt, i));
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_FLOAT)
				      gaiaSetDoubleValue (dbf_field,
							  sqlite3_column_double
							  (stmt, i));
				  else
				      gaiaSetNullValue (dbf_field);
				  break;
			      case 'C':
				  if (sqlite3_column_type (stmt, i) ==
				      SQLITE_TEXT)
				    {
					dummy =
					    (char *) sqlite3_column_text (stmt,
									  i);
					gaiaSetStrValue (dbf_field, dummy);
				    }
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_INTEGER)
				    {
					sprintf (buf, FRMT64,
						 sqlite3_column_int64 (stmt,
								       i));
					gaiaSetStrValue (dbf_field, buf);
				    }
				  else if (sqlite3_column_type (stmt, i) ==
					   SQLITE_FLOAT)
				    {
					sql = sqlite3_mprintf ("%1.6f",
							       sqlite3_column_double
							       (stmt, i));
					gaiaSetStrValue (dbf_field, sql);
					sqlite3_free (sql);
				    }
				  else
				      gaiaSetNullValue (dbf_field);
				  break;
			      };
			}
		  }
		free_auxdbf (auxdbf);
		auxdbf = NULL;
		if (!gaiaWriteDbfEntity (dbf, dbf_write))
		    spatialite_e ("DBF write error\n");
		gaiaFreeDbfList (dbf_write);
	    }
	  else
	      goto sql_error;
      }
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    sqlite3_finalize (stmt);
    gaiaFlushDbfHeader (dbf);
    gaiaFreeDbf (dbf);
    free (xtable);
    if (!err_msg)
	spatialite_e ("Exported %d rows into the DBF file\n", rows);
    else
	sprintf (err_msg, "Exported %d rows into the DBF file\n", rows);
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    *xrows = rows;
    return 1;
  sql_error:
/* some SQL error occurred */
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    free (xtable);
    sqlite3_finalize (stmt);
    if (dbf_export_list)
	gaiaFreeDbfList (dbf_export_list);
    if (dbf_list)
	gaiaFreeDbfList (dbf_list);
    if (dbf)
	gaiaFreeDbf (dbf);
    if (!err_msg)
	spatialite_e ("dump DBF file error: %s\n", sqlite3_errmsg (sqlite));
    else
	sprintf (err_msg, "dump DBF file error: %s\n", sqlite3_errmsg (sqlite));
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 0;
  no_file:
/* DBF can't be created/opened */
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    free (xtable);
    if (dbf_export_list)
	gaiaFreeDbfList (dbf_export_list);
    if (dbf_list)
	gaiaFreeDbfList (dbf_list);
    if (dbf)
	gaiaFreeDbf (dbf);
    if (!err_msg)
	spatialite_e ("ERROR: unable to open '%s' for writing\n", dbf_path);
    else
	sprintf (err_msg, "ERROR: unable to open '%s' for writing\n", dbf_path);
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 0;
  empty_result_set:
/* the result set is empty - nothing to do */
    if (auxdbf != NULL)
	free_auxdbf (auxdbf);
    if (get_default_dbf_fields
	(sqlite, xtable, db_prefix, table_name, &dbf_list))
	goto continue_exporting;
    free (xtable);
    sqlite3_finalize (stmt);
    if (dbf_export_list)
	gaiaFreeDbfList (dbf_export_list);
    if (dbf_list)
	gaiaFreeDbfList (dbf_list);
    if (dbf)
	gaiaFreeDbf (dbf);
    if (!err_msg)
	spatialite_e
	    ("The SQL SELECT returned an empty result set ... there is nothing to export ...\n");
    else
	sprintf (err_msg,
		 "The SQL SELECT returned an empty result set ... there is nothing to export ...\n");
    if (db_prefix != NULL)
	free (db_prefix);
    if (table_name != NULL)
	free (table_name);
    return 0;
}

SPATIALITE_DECLARE int
is_kml_constant (sqlite3 * sqlite, char *table, char *column)
{
/* checking a possible column name for KML dump */
    char *sql;
    char *xname;
    int ret;
    int k = 1;
    const char *name;
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;

    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xname);
    free (xname);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 1;
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		name = results[(i * columns) + 1];
		if (strcasecmp (name, column) == 0)
		    k = 0;
	    }
      }
    sqlite3_free_table (results);
    return k;
}

SPATIALITE_DECLARE int
dump_kml (sqlite3 * sqlite, char *table, char *geom_col, char *kml_path,
	  char *name_col, char *desc_col, int precision)
{
    int rows;
    return dump_kml_ex (sqlite, table, geom_col, kml_path, name_col, desc_col,
			precision, &rows);
}

SPATIALITE_DECLARE int
dump_kml_ex (sqlite3 * sqlite, char *table, char *geom_col, char *kml_path,
	     char *name_col, char *desc_col, int precision, int *xrows)
{
/* dumping a  geometry table as KML */
    char *sql;
    char *xname;
    char *xdesc;
    char *xgeom_col;
    char *xtable;
    sqlite3_stmt *stmt = NULL;
    FILE *out = NULL;
    int ret;
    int rows = 0;
    int is_const = 1;

    *xrows = -1;
/* opening/creating the KML file */
#ifdef _WIN32
    out = gaia_win_fopen (kml_path, "wb");
#else
    out = fopen (kml_path, "wb");
#endif
    if (!out)
	goto no_file;

/* preparing SQL statement */
    if (name_col == NULL)
	xname = sqlite3_mprintf ("%Q", "name");
    else
      {
	  is_const = is_kml_constant (sqlite, table, name_col);
	  if (is_const)
	      xname = sqlite3_mprintf ("%Q", name_col);
	  else
	    {
		xname = gaiaDoubleQuotedSql (name_col);
		sql = sqlite3_mprintf ("\"%s\"", xname);
		free (xname);
		xname = sql;
	    }
      }
    if (desc_col == NULL)
	xdesc = sqlite3_mprintf ("%Q", "description");
    else
      {
	  is_const = is_kml_constant (sqlite, table, desc_col);
	  if (is_const)
	      xdesc = sqlite3_mprintf ("%Q", desc_col);
	  else
	    {
		xdesc = gaiaDoubleQuotedSql (desc_col);
		sql = sqlite3_mprintf ("\"%s\"", xdesc);
		free (xdesc);
		xdesc = sql;
	    }
      }
    xgeom_col = gaiaDoubleQuotedSql (geom_col);
    xtable = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("SELECT AsKML(%s, %s, %s, %d) FROM \"%s\" "
			   "WHERE \"%s\" IS NOT NULL", xname, xdesc,
			   xgeom_col, precision, xtable, xgeom_col);
    sqlite3_free (xname);
    sqlite3_free (xdesc);
    free (xgeom_col);
    free (xtable);
/* compiling SQL prepared statement */
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;

    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* processing a result set row */
		if (rows == 0)
		  {
		      fprintf (out,
			       "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
		      fprintf (out,
			       "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\r\n");
		      fprintf (out, "<Document>\r\n");
		  }
		rows++;
		fprintf (out, "\t%s\r\n", sqlite3_column_text (stmt, 0));
	    }
	  else
	      goto sql_error;
      }
    if (!rows)
	goto empty_result_set;


    fprintf (out, "</Document>\r\n");
    fprintf (out, "</kml>\r\n");
    sqlite3_finalize (stmt);
    fclose (out);
    *xrows = rows;
    return 1;

  sql_error:
/* some SQL error occurred */
    if (stmt)
	sqlite3_finalize (stmt);
    if (out)
	fclose (out);
    spatialite_e ("Dump KML error: %s\n", sqlite3_errmsg (sqlite));
    return 0;
  no_file:
/* KML file can't be created/opened */
    if (stmt)
	sqlite3_finalize (stmt);
    if (out)
	fclose (out);
    spatialite_e ("ERROR: unable to open '%s' for writing\n", kml_path);
    return 0;
  empty_result_set:
/* the result set is empty - nothing to do */
    if (stmt)
	sqlite3_finalize (stmt);
    if (out)
	fclose (out);
    spatialite_e
	("The SQL SELECT returned an empty result set\n... there is nothing to export ...\n");
    return 0;
}

SPATIALITE_DECLARE int
dump_geojson (sqlite3 * sqlite, char *table, char *geom_col,
	      char *outfile_path, int precision, int option)
{
    int rows;
    return dump_geojson_ex (sqlite, table, geom_col, outfile_path, precision,
			    option, &rows);
}

SPATIALITE_DECLARE int
dump_geojson_ex (sqlite3 * sqlite, char *table, char *geom_col,
		 char *outfile_path, int precision, int option, int *xrows)
{
/* dumping a  geometry table as GeoJSON - Brad Hards 2011-11-09 */
/* sandro furieri 2014-08-30: adding the "int *xrows" argument */
    char *sql;
    char *xgeom_col;
    char *xtable;
    sqlite3_stmt *stmt = NULL;
    FILE *out = NULL;
    int ret;
    int rows = 0;

    *xrows = -1;
/* opening/creating the GeoJSON output file */
#ifdef _WIN32
    out = gaia_win_fopen (outfile_path, "wb");
#else
    out = fopen (outfile_path, "wb");
#endif
    if (!out)
	goto no_file;

/* preparing SQL statement */
    xtable = gaiaDoubleQuotedSql (table);
    xgeom_col = gaiaDoubleQuotedSql (geom_col);
    sql =
	sqlite3_mprintf
	("SELECT AsGeoJSON(\"%s\", %d, %d) FROM \"%s\" WHERE \"%s\" IS NOT NULL",
	 xgeom_col, precision, option, xtable, xgeom_col);
    free (xtable);
    free (xgeom_col);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;

    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	    {
		break;		/* end of result set */
	    }
	  if (ret == SQLITE_ROW)
	    {
		rows++;
		fprintf (out, "%s\r\n", sqlite3_column_text (stmt, 0));
	    }
	  else
	    {
		goto sql_error;
	    }
      }
    if (rows == 0)
      {
	  goto empty_result_set;
      }

    sqlite3_finalize (stmt);
    fclose (out);
    *xrows = rows;
    return 1;

  sql_error:
/* an SQL error occurred */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    spatialite_e ("Dump GeoJSON error: %s\n", sqlite3_errmsg (sqlite));
    return 0;

  no_file:
/* Output file could not be created / opened */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    spatialite_e ("ERROR: unable to open '%s' for writing\n", outfile_path);
    return 0;

  empty_result_set:
/* the result set is empty - nothing to do */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    spatialite_e ("The SQL SELECT returned no data to export...\n");
    return 0;
}

static int
do_check_geometry (sqlite3 * sqlite, const char *table, const char *geom_col,
		   char **geoname, int *srid, int *dims)
{
/* checking Geometry Column, SRID and Dimensions */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;

    if (geom_col == NULL)
	sql = sqlite3_mprintf ("SELECT f_geometry_column, srid, geometry_type "
			       "FROM geometry_columns WHERE Lower(f_table_name) = Lower(%Q)",
			       table);
    else
	sql = sqlite3_mprintf ("SELECT f_geometry_column, srid, geometry_type "
			       "FROM geometry_columns WHERE Lower(f_table_name) = Lower(%Q) AND "
			       "Lower(f_geometry_column) = Lower(%Q)",
			       table, geom_col);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump GeoJSON2 MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows == 1)
	goto ok;

/* it could be a possible Spatial View */
    if (geom_col == NULL)
	sql =
	    sqlite3_mprintf
	    ("SELECT g.f_geometry_column, g.srid, g.geometry_type "
	     "FROM views_geometry_columns AS v "
	     "JOIN geometry_columns AS g ON (v.f_table_name = g.f_table_name) "
	     "WHERE Lower(v.view_name) = Lower(%Q)", table);
    else
	sql =
	    sqlite3_mprintf
	    ("SELECT g.f_geometry_column, g.srid, g.geometry_type "
	     "FROM views_geometry_columns AS v "
	     "JOIN geometry_columns AS g ON (v.f_table_name = g.f_table_name) "
	     "WHERE Lower(v.view_name) = Lower(%Q) AND "
	     "Lower(v.view_geometry) = Lower(%Q)", table, geom_col);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump GeoJSON2 MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows == 1)
	goto ok;

/* it could be a possible Spatial Virtual Table */
    if (geom_col == NULL)
	sql = sqlite3_mprintf ("SELECT virt_geometry, srid, geometry_type "
			       "FROM virts_geometry_columns "
			       "WHERE Lower(virt_name) = Lower(%Q)", table);
    else
	sql = sqlite3_mprintf ("SELECT virt_geometry, srid, geometry_type "
			       "FROM virts_geometry_columns "
			       "WHERE Lower(virt_name) = Lower(%Q) AND "
			       "Lower(virt_geometry) = Lower(%Q)",
			       table, geom_col);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump GeoJSON2 MetaData error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows != 1)
	goto error;

  ok:
    for (i = 1; i <= rows; i++)
      {
	  int gtype;
	  const char *x = results[(i * columns) + 0];
	  int len = strlen (x);
	  *geoname = malloc (len + 1);
	  strcpy (*geoname, x);
	  *srid = atoi (results[(i * columns) + 1]);
	  gtype = atoi (results[(i * columns) + 2]);
	  switch (gtype)
	    {
	    case GAIA_POINT:
	    case GAIA_LINESTRING:
	    case GAIA_POLYGON:
	    case GAIA_MULTIPOINT:
	    case GAIA_MULTILINESTRING:
	    case GAIA_MULTIPOLYGON:
	    case GAIA_GEOMETRYCOLLECTION:
		*dims = GAIA_XY;
		break;
	    case GAIA_POINTZ:
	    case GAIA_LINESTRINGZ:
	    case GAIA_POLYGONZ:
	    case GAIA_MULTIPOINTZ:
	    case GAIA_MULTILINESTRINGZ:
	    case GAIA_MULTIPOLYGONZ:
	    case GAIA_GEOMETRYCOLLECTIONZ:
		*dims = GAIA_XY_Z;
		break;
	    case GAIA_POINTM:
	    case GAIA_LINESTRINGM:
	    case GAIA_POLYGONM:
	    case GAIA_MULTIPOINTM:
	    case GAIA_MULTILINESTRINGM:
	    case GAIA_MULTIPOLYGONM:
	    case GAIA_GEOMETRYCOLLECTIONM:
		*dims = GAIA_XY_M;
		break;
	    case GAIA_POINTZM:
	    case GAIA_LINESTRINGZM:
	    case GAIA_POLYGONZM:
	    case GAIA_MULTIPOINTZM:
	    case GAIA_MULTILINESTRINGZM:
	    case GAIA_MULTIPOLYGONZM:
	    case GAIA_GEOMETRYCOLLECTIONZM:
		*dims = GAIA_XY_Z_M;
		break;
	    default:
		goto error;
	    };
      }
    sqlite3_free_table (results);
    return 1;

  error:
    sqlite3_free_table (results);
    return 0;
}

static char *
do_prepare_sql (sqlite3 * sqlite, const char *table, const char *geom_col,
		int srid, int dims, int precision, int lon_lat, int m_coords)
{
/* preparing the SQL statement */
    char *sql;
    char *prev;
    char *xtable;
    char *x_col;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;

    xtable = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", table);
    free (xtable);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("dump GeoJSON2 PRAGMA error: <%s>\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }

/* defining the Geometry first */
    x_col = gaiaDoubleQuotedSql (geom_col);
    if (lon_lat)
      {
	  /* exporting Geographic coordinates (lon-lat) */
	  if (srid == 0 || srid == 4326)
	    {
		/* already lon-lan, no transformation is required */
		if (m_coords)
		  {
		      /* exporting eventual M-Values */
		      sql =
			  sqlite3_mprintf
			  ("SELECT AsGeoJSON(ST_ForcePolygonCCW(\"%s\"), %d)",
			   x_col, precision);
		  }
		else
		  {
		      if (dims == GAIA_XY_M)
			{
			    /* exporting XYM as XY */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(CastToXY(ST_ForcePolygonCCW(\"%s\")), %d)",
				 x_col, precision);
			}
		      else if (dims == GAIA_XY_Z_M)
			{
			    /* exporting XYZM as XYZ */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(CastToXYZ(ST_ForcePolygonCCW(\"%s\")), %d)",
				 x_col, precision);
			}
		      else
			{
			    /* unchanged dimensions */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(ST_ForcePolygonCCW(\"%s\"), %d)",
				 x_col, precision);
			}
		  }
	    }
	  else
	    {
		/* converting to lon-lat WGS84 */
		if (m_coords)
		  {
		      /* exporting eventual M-Values */
		      sql =
			  sqlite3_mprintf
			  ("SELECT AsGeoJSON(ST_Transform(ST_ForcePolygonCCW(\"%s\"), 4326), %d)",
			   x_col, precision);
		  }
		else
		  {
		      if (dims == GAIA_XY_M)
			{
			    /* exporting XYM as XY */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(ST_Transform(CastToXY(ST_ForcePolygonCCW(\"%s\")), 4326), %d)",
				 x_col, precision);
			}
		      else if (dims == GAIA_XY_Z_M)
			{
			    /* exporting XYZM as XYZ */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(ST_TransformCastToXYZ(ST_ForcePolygonCCW(\"%s\")), 4326), %d)",
				 x_col, precision);
			}
		      else
			{
			    /* unchanged dimensions */
			    sql =
				sqlite3_mprintf
				("SELECT AsGeoJSON(ST_Transform(ST_ForcePolygonCCW(\"%s\"), 4326), %d)",
				 x_col, precision);
			}
		  }
	    }
      }
    else
      {
	  /* exporting coordinates as they are without any transformation */
	  if (m_coords)
	    {
		/* exporting eventual M-Values */
		sql =
		    sqlite3_mprintf
		    ("SELECT AsGeoJSON(ST_ForcePolygonCCW(\"%s\"), %d)", x_col,
		     precision);
	    }
	  else
	    {
		if (dims == GAIA_XY_M)
		  {
		      /* exporting XYM as XY */
		      sql =
			  sqlite3_mprintf
			  ("SELECT AsGeoJSON(CastToXY(ST_ForcePolygonCCW(\"%s\")), %d)",
			   x_col, precision);
		  }
		else if (dims == GAIA_XY_Z_M)
		  {
		      /* exporting XYZM as XYZ */
		      sql =
			  sqlite3_mprintf
			  ("SELECT AsGeoJSON(CastToXYZ(ST_ForcePolygonCCW(\"%s\")), %d)",
			   x_col, precision);
		  }
		else
		  {
		      /* unchanged dimensions */
		      sql =
			  sqlite3_mprintf
			  ("SELECT AsGeoJSON(ST_ForcePolygonCCW(\"%s\"), %d)",
			   x_col, precision);
		  }
	    }
      }
    free (x_col);

    for (i = 1; i <= rows; i++)
      {
	  /* then adding all Properties */
	  const char *col = results[(i * columns) + 1];
	  if (strcasecmp (col, geom_col) == 0)
	      continue;		/* skipping the Geometry itself */
	  x_col = gaiaDoubleQuotedSql (col);
	  prev = sql;
	  sql = sqlite3_mprintf ("%s, \"%s\"", prev, x_col);
	  free (x_col);
	  sqlite3_free (prev);
      }
    sqlite3_free_table (results);
    prev = sql;
    xtable = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("%s FROM \"%s\"", prev, xtable);
    free (xtable);
    sqlite3_free (prev);
    return sql;
}

static char *
do_normalize_case (const char *name, int colname_case)
{
/* transforming a name in full lower- or upper-case */
    int len = strlen (name);
    char *clean = malloc (len + 1);
    char *p = clean;
    strcpy (clean, name);
    while (*p != '\0')
      {
	  if (colname_case == GAIA_DBF_COLNAME_LOWERCASE)
	    {
		if (*p >= 'A' && *p <= 'Z')
		    *p = *p - 'A' + 'a';
	    }
	  if (colname_case == GAIA_DBF_COLNAME_UPPERCASE)
	    {
		if (*p >= 'a' && *p <= 'z')
		    *p = *p - 'a' + 'A';
	    }
	  p++;
      }
    return clean;
}

SPATIALITE_DECLARE int
dump_geojson2 (sqlite3 * sqlite, char *table, char *geom_col,
	       char *outfile_path, int precision, int lon_lat,
	       int m_coords, int indented, int colname_case, int *xrows,
	       char **error_message)
{
/* dumping a  geometry table as GeoJSON FeatureCollection (RFC 7946) */
/* sandro furieri 2018-11-25 */
    char *sql;
    sqlite3_stmt *stmt = NULL;
    FILE *out = NULL;
    int ret;
    int rows = 0;
    char *geoname = NULL;
    int srid;
    int dims;
    int first_feature = 1;
    sqlite3_int64 ival;
    double dval;
    const char *tval;
    char *xtval;
    *error_message = NULL;

/* checking Geometry Column, SRID and Dimensions */
    if (!do_check_geometry (sqlite, table, geom_col, &geoname, &srid, &dims))
	goto no_geom;

    *xrows = -1;
/* opening/creating the GeoJSON output file */
#ifdef _WIN32
    out = gaia_win_fopen (outfile_path, "wb");
#else
    out = fopen (outfile_path, "wb");
#endif
    if (!out)
	goto no_file;

/* preparing SQL statement */
    sql =
	do_prepare_sql (sqlite, table, geoname, srid, dims, precision, lon_lat,
			m_coords);
    if (sql == NULL)
	goto no_sql;
    free (geoname);

    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	goto sql_error;

    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	    {
		break;		/* end of result set */
	    }
	  if (ret == SQLITE_ROW)
	    {
		int c;
		int cols = sqlite3_column_count (stmt);
		if (first_feature)
		  {
		      /* FeatureCollection */
		      if (indented)
			  fprintf (out,
				   "{\r\n\t\"type\" : \"FeatureCollection\",\r\n\t\"features\" : [{\r\n");
		      else
			  fprintf (out,
				   "{\"type\":\"FeatureCollection\",\"features\":[{");
		      first_feature = 0;
		  }
		/* Feature */
		if (rows == 0)
		  {
		      /* first Feature */
		      if (indented)
			  fprintf (out,
				   "\t\t\"type\" : \"Feature\",\r\n\t\t\"properties\" : ");
		      else
			  fprintf (out, "\"type\":\"Feature\",\"properties\":");
		  }
		else
		  {
		      /* any other Feature except the first one */
		      if (indented)
			  fprintf (out,
				   ", {\r\n\t\t\"type\" : \"Feature\",\r\n\t\t\"properties\" : ");
		      else
			  fprintf (out,
				   ",{\"type\":\"Feature\",\"properties\":");
		  }
		for (c = 1; c < cols; c++)
		  {
		      /* Properties */
		      const char *col_name = sqlite3_column_name (stmt, c);
		      char *norm_name =
			  do_normalize_case (col_name, colname_case);
		      char *xcol_name = gaiaDoubleQuotedSql (norm_name);
		      free (norm_name);
		      if (c == 1)
			{
			    if (indented)
				fprintf (out, "{\r\n\t\t\t\"%s\" : ",
					 xcol_name);
			    else
				fprintf (out, "{\"%s\":", xcol_name);
			}
		      else
			{
			    if (indented)
				fprintf (out, ",\r\n\t\t\t\"%s\" : ",
					 xcol_name);
			    else
				fprintf (out, ",\"%s\":", xcol_name);
			}
		      free (xcol_name);
		      switch (sqlite3_column_type (stmt, c))
			{
			case SQLITE_INTEGER:
			    ival = sqlite3_column_int64 (stmt, c);
			    fprintf (out, "%lld", ival);
			    break;
			case SQLITE_FLOAT:
			    dval = sqlite3_column_double (stmt, c);
			    fprintf (out, "%f", dval);
			    break;
			case SQLITE_TEXT:
			    tval = (const char *) sqlite3_column_text (stmt, c);
			    xtval = gaiaDoubleQuotedSql (tval);
			    fprintf (out, "\"%s\"", xtval);
			    free (xtval);
			    break;
			case SQLITE_BLOB:
			    fprintf (out, "\"BLOB value\"");
			    break;
			case SQLITE_NULL:
			default:
			    fprintf (out, "null");
			    break;
			};
		  }
		/* geometry */
		if (indented)
		    fprintf (out, "\r\n\t\t},\r\n\t\t\"geometry\" : ");
		else
		    fprintf (out, "},\"geomety\":");
		switch (sqlite3_column_type (stmt, 0))
		  {
		  case SQLITE_NULL:
		      fprintf (out, "null");
		      break;
		  default:
		      tval = (const char *) sqlite3_column_text (stmt, 0);
		      fprintf (out, "%s", tval);
		      break;
		  };
		/* end Feature */
		if (indented)
		    fprintf (out, "\r\n\t}");
		else
		    fprintf (out, "}");
		rows++;
	    }
	  else
	    {
		goto sql_error;
	    }
      }
    if (rows == 0)
      {
	  goto empty_result_set;
      }
    if (indented)
	fprintf (out, "]\r\n}\r\n");
    else
	fprintf (out, "]}");

    sqlite3_finalize (stmt);
    fclose (out);
    *xrows = rows;
    return 1;

  sql_error:
/* an SQL error occurred */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    *error_message =
	sqlite3_mprintf ("Dump GeoJSON2 error: %s\n", sqlite3_errmsg (sqlite));
    return 0;

  no_file:
/* Output file could not be created / opened */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    if (geoname != NULL)
	free (geoname);
    *error_message =
	sqlite3_mprintf ("ERROR: unable to open '%s' for writing\n",
			 outfile_path);
    return 0;

  empty_result_set:
/* the result set is empty - nothing to do */
    if (stmt)
      {
	  sqlite3_finalize (stmt);
      }
    if (out)
      {
	  fclose (out);
      }
    *error_message =
	sqlite3_mprintf ("The SQL SELECT returned no data to export...\n");
    return 0;

  no_geom:
/* not a valid Geometry Column */
    if (out)
      {
	  fclose (out);
      }
    *error_message = sqlite3_mprintf ("Not a valid Geometry Column.\n");
    return 0;

  no_sql:
/* unable to create a valid SQL query */
    if (out)
      {
	  fclose (out);
      }
    *error_message = sqlite3_mprintf ("Unable to create a valid SQL query.\n");
    return 0;
}

SPATIALITE_DECLARE int
load_geojson (sqlite3 * sqlite, char *path, char *table, char *geom_col,
	      int spatial_index, int srid, int colname_case, int *rows,
	      char **error_message)
{
/* Loads an external GeoJSON file into a newly created table */
    FILE *in = NULL;
    sqlite3_stmt *stmt = NULL;
    geojson_parser_ptr parser = NULL;
    geojson_feature_ptr ft;
    int i;
    int ret;
    int pending = 0;
    char *sql;
    int ins_rows = 0;
    *error_message = NULL;

/* attempting to open the GeoJSON file for reading */
#ifdef _WIN32
    in = gaia_win_fopen (path, "rb");
#else
    in = fopen (path, "rb");
#endif
    if (in == NULL)
      {
	  *error_message =
	      sqlite3_mprintf
	      ("GeoJSON parser: unable to open %s for reading\n", path);
	  return 0;
      }

/* creating the GeoJSON parser */
    parser = geojson_create_parser (in);
    if (!geojson_parser_init (parser, error_message))
	goto err;
    if (!geojson_create_features_index (parser, error_message))
	goto err;
    if (!geojson_check_features (parser, error_message))
	goto err;

/* creating the output table */
    sql = geojson_sql_create_table (parser, table, colname_case);
    if (sql == NULL)
	goto err;
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  *error_message =
	      sqlite3_mprintf
	      ("GeoJSON import: unable to create the output table (%s)\n",
	       sqlite3_errmsg (sqlite));
	  goto err;
      }

/* adding the Geometry Column */
    sql =
	geojson_sql_add_geometry (parser, table, geom_col, colname_case, srid);
    if (sql == NULL)
	goto err;
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  *error_message =
	      sqlite3_mprintf
	      ("GeoJSON import: unable to create the Geometry column (%s)\n",
	       sqlite3_errmsg (sqlite));
	  goto err;
      }

    if (spatial_index)
      {
	  /* creating the Spatial Index */
	  sql = geojson_sql_create_rtree (table, geom_col, colname_case);
	  if (sql == NULL)
	      goto err;
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, NULL);
	  sqlite3_free (sql);
	  if (ret != SQLITE_OK)
	    {
		*error_message =
		    sqlite3_mprintf
		    ("GeoJSON import: unable to create the SpatialIndex (%s)\n",
		     sqlite3_errmsg (sqlite));
		goto err;
	    }
      }

/* the whole import will be enclosed in a single Transaction */
    ret = sqlite3_exec (sqlite, "SAVEPOINT import_geo_json", NULL, NULL, NULL);
    if (ret != SQLITE_OK)
      {
	  *error_message =
	      sqlite3_mprintf ("GeoJSON import: SAVEPOINT error (%s)\n",
			       sqlite3_errmsg (sqlite));
	  goto err;
      }
    pending = 1;

    sql = geojson_sql_insert_into (parser, table);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  *error_message =
	      sqlite3_mprintf ("GeoJSON import: INSERT INTO error (%s)\n",
			       sqlite3_errmsg (sqlite));
	  goto err;
      }

    for (i = 0; i < parser->count; i++)
      {
	  /* inserting all features into the output table */
	  ft = parser->features + i;
	  if (geojson_init_feature (parser, ft, error_message))
	    {
		/* inserting a single Feature */
		geojson_column_ptr col;
		int cnt = 1;
		sqlite3_reset (stmt);
		sqlite3_clear_bindings (stmt);
		col = parser->first_col;
		while (col != NULL)
		  {
		      /* binding column values */
		      geojson_property_ptr prop =
			  geojson_get_property_by_name (ft, col->name);
		      if (prop == NULL)
			  sqlite3_bind_null (stmt, cnt++);
		      else
			{
			    switch (prop->type)
			      {
			      case GEOJSON_TEXT:
				  sqlite3_bind_text (stmt, cnt++,
						     prop->txt_value,
						     strlen (prop->txt_value),
						     SQLITE_STATIC);
				  break;
			      case GEOJSON_INTEGER:
				  sqlite3_bind_int64 (stmt, cnt++,
						      prop->int_value);
				  break;
			      case GEOJSON_DOUBLE:
				  sqlite3_bind_double (stmt, cnt++,
						       prop->dbl_value);
				  break;
			      case GEOJSON_FALSE:
				  sqlite3_bind_int (stmt, cnt++, 0);
				  break;
			      case GEOJSON_TRUE:
				  sqlite3_bind_int (stmt, cnt++, 0);
				  break;
			      case GEOJSON_NULL:
			      default:
				  sqlite3_bind_null (stmt, cnt++);
				  break;
			      };
			}
		      col = col->next;
		  }
		if (ft->geometry == NULL)
		    sqlite3_bind_null (stmt, cnt++);
		else
		  {
		      gaiaGeomCollPtr geo =
			  gaiaParseGeoJSON ((const unsigned char
					     *) (ft->geometry));
		      if (geo != NULL)
			{
			    /* binding the Geometry BLOB */
			    unsigned char *blob;
			    int blob_size;
			    geo->Srid = srid;
			    gaiaToSpatiaLiteBlobWkb (geo, &blob, &blob_size);
			    sqlite3_bind_blob (stmt, cnt, blob, blob_size,
					       free);
			    gaiaFreeGeomColl (geo);
			}
		      else
			{
			    *error_message =
				sqlite3_mprintf
				("GeoJSON import: invalid Geometry (fid=%d)\n",
				 ft->fid);
			    goto err;
			}
		  }
		/* inserting the Feature into the DB Table */
		ret = sqlite3_step (stmt);
		if (ret == SQLITE_DONE || ret == SQLITE_ROW)
		    ins_rows++;
		else
		  {
		      *error_message =
			  sqlite3_mprintf
			  ("GeoJSON import: INSERT INTO failure (fid=%d) %s\n",
			   ft->fid, sqlite3_errmsg (sqlite));
		      goto err;
		  }
		geojson_reset_feature (ft);
	    }
	  else
	      goto err;
      }
    sqlite3_finalize (stmt);
    stmt = NULL;

/* Committing the still pending Transaction */
    ret =
	sqlite3_exec (sqlite, "RELEASE SAVEPOINT import_geo_json", NULL, NULL,
		      NULL);
    if (ret != SQLITE_OK)
      {
	  *error_message =
	      sqlite3_mprintf ("GeoJSON import: RELEASE SAVEPOINT error (%s)\n",
			       sqlite3_errmsg (sqlite));
	  goto err;
      }

    geojson_destroy_parser (parser);
    *rows = ins_rows;
    return 1;

  err:
    if (stmt != NULL)
	sqlite3_finalize (stmt);
    if (pending)
      {
	  /* Rolling back the Transaction */
	  sqlite3_exec (sqlite, "ROLLBAKC TO SAVEPOINT import_geo_json", NULL,
			NULL, NULL);
	  sqlite3_exec (sqlite, "RELEASE SAVEPOINT import_geo_json", NULL, NULL,
			NULL);
      }
    geojson_destroy_parser (parser);
    *rows = 0;
    return 0;
}

#ifdef ENABLE_MINIZIP		/* MINIZIP is enabled */

GAIAGEO_DECLARE char *
gaiaReadWktFromZipShp (const char *zip_path, const char *basename)
{
/* attempting to read the .PRJ member of some zipped Shapefile */
    char *wkt = NULL;
    unzFile uf = NULL;
    struct zip_mem_shapefile *mem_shape = NULL;

    if (zip_path == NULL)
      {
	  spatialite_e ("read_wkt_from_zipshp error: <%s>\n",
			"NULL zipfile path");
	  return NULL;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  return NULL;
      }
/* parsing the Zipfile directory */
    mem_shape = do_list_zipfile_dir (uf, basename, 0);
    if (mem_shape == NULL)
      {
	  spatialite_e ("No SHP %s with Zipfile\n", basename);
	  goto stop;
      }
/* unzipping the PRJ member (if any) */
    do_read_zipfile_file (uf, mem_shape, GAIA_ZIPFILE_PRJ);
    if (mem_shape->prj.buf != NULL)
      {
	  wkt = malloc (mem_shape->prj.size + 1);
	  memcpy (wkt, mem_shape->prj.buf, mem_shape->prj.size);
	  *(wkt + mem_shape->prj.size) = '\0';
      }

  stop:
    unzClose (uf);
    if (mem_shape != NULL)
	destroy_zip_mem_shapefile (mem_shape);
    return wkt;
}

GAIAGEO_DECLARE int
gaiaZipfileNumSHP (const char *zip_path, int *count)
{
/* attempting to count how many Shapefiles are within a given Zipfile */
    int ret;
    int retval = 1;
    struct zip_mem_shp_item *item;
    struct zip_mem_shp_list *list = alloc_zip_mem_shp_list ();
    unzFile uf = NULL;

    *count = 0;
    if (zip_path == NULL)
      {
	  spatialite_e ("zipfile NumSHP error: <%s>\n", "NULL zipfile path");
	  retval = 0;
	  goto stop;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  retval = 0;
	  goto stop;
      }
/* sniffing the Zipfile */
    ret = do_sniff_zipfile_dir (uf, list, 0);
    if (!ret)
      {
	  retval = 0;
	  goto stop;
      }
    item = list->first;
    while (item != NULL)
      {
	  /* counting how many Shapefiles are there */
	  if (item->shp && item->shx && item->dbf)
	      *count += 1;
	  item = item->next;
      }
  stop:
    unzClose (uf);
    destroy_zip_mem_shp_list (list);
    return retval;
}

GAIAGEO_DECLARE char *
gaiaZipfileShpN (const char *zip_path, int idx)
{
/* attempting to return the Nth Shapefile's basename fromt within a given Zipfile */
    int ret;
    int count = 0;
    char *basename = NULL;
    struct zip_mem_shp_item *item;
    struct zip_mem_shp_list *list = alloc_zip_mem_shp_list ();
    unzFile uf = NULL;

    count = 0;
    if (zip_path == NULL)
      {
	  spatialite_e ("zipfile NumSHP error: <%s>\n", "NULL zipfile path");
	  goto stop;
      }
/* opening the Zipfile expected to contain the Shapefile */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  goto stop;
      }
/* sniffing the Zipfile */
    ret = do_sniff_zipfile_dir (uf, list, 0);
    if (!ret)
	goto stop;
    item = list->first;
    while (item != NULL)
      {
	  /* counting how many Shapefiles are there */
	  if (item->shp && item->shx && item->dbf)
	      count++;
	  if (count == idx)
	    {
		int len = strlen (item->basename);
		basename = malloc (len + 1);
		strcpy (basename, item->basename);
		break;
	    }
	  item = item->next;
      }
  stop:
    unzClose (uf);
    destroy_zip_mem_shp_list (list);
    return basename;
}

GAIAGEO_DECLARE int
gaiaZipfileNumDBF (const char *zip_path, int *count)
{
/* attempting to count how many DBF files are within a given Zipfile */
    int ret;
    int retval = 1;
    struct zip_mem_shp_item *item;
    struct zip_mem_shp_list *list = alloc_zip_mem_shp_list ();
    unzFile uf = NULL;

    *count = 0;
    if (zip_path == NULL)
      {
	  spatialite_e ("zipfile NumDBF error: <%s>\n", "NULL zipfile path");
	  retval = 0;
	  goto stop;
      }
/* opening the Zipfile expected to contain the DBF file */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  retval = 0;
	  goto stop;
      }
/* sniffing the Zipfile */
    ret = do_sniff_zipfile_dir (uf, list, 1);
    if (!ret)
      {
	  retval = 0;
	  goto stop;
      }
    item = list->first;
    while (item != NULL)
      {
	  /* counting how many DBF files are there */
	  if (item->dbf)
	      *count += 1;
	  item = item->next;
      }
  stop:
    unzClose (uf);
    destroy_zip_mem_shp_list (list);
    return retval;
}

GAIAGEO_DECLARE char *
gaiaZipfileDbfN (const char *zip_path, int idx)
{
/* attempting to return the Nth DBF filename fromt within a given Zipfile */
    int ret;
    int count = 0;
    char *filename = NULL;
    struct zip_mem_shp_item *item;
    struct zip_mem_shp_list *list = alloc_zip_mem_shp_list ();
    unzFile uf = NULL;

    count = 0;
    if (zip_path == NULL)
      {
	  spatialite_e ("zipfile NumDBF error: <%s>\n", "NULL zipfile path");
	  goto stop;
      }
/* opening the Zipfile expected to contain the DBF file */
    uf = unzOpen64 (zip_path);
    if (uf == NULL)
      {
	  spatialite_e ("Unable to Open %s\n", zip_path);
	  goto stop;
      }
/* sniffing the Zipfile */
    ret = do_sniff_zipfile_dir (uf, list, 1);
    if (!ret)
	goto stop;
    item = list->first;
    while (item != NULL)
      {
	  /* counting how many DBF files are there */
	  if (item->dbf)
	      count++;
	  if (count == idx)
	    {
		int len = strlen (item->basename);
		filename = malloc (len + 1);
		strcpy (filename, item->basename);
		break;
	    }
	  item = item->next;
      }
  stop:
    unzClose (uf);
    destroy_zip_mem_shp_list (list);
    return filename;
}

#endif /* end MINIZIP */

#endif /* end ICONV (SHP) */

static int
is_table (sqlite3 * sqlite, const char *table)
{
/* check if this one really is a TABLE */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    char *errMsg = NULL;
    int ok = 0;

    sql = sqlite3_mprintf ("SELECT tbl_name FROM sqlite_master "
			   "WHERE type = 'table' AND Lower(tbl_name) = Lower(%Q)",
			   table);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQLite SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return ok;
      }
    if (rows < 1)
	;
    else
	ok = 1;
    sqlite3_free_table (results);
    return ok;
}

SPATIALITE_DECLARE void
check_duplicated_rows (sqlite3 * sqlite, char *table, int *dupl_count)
{
/* Checking a Table for Duplicate rows */
    char *sql;
    int first = 1;
    char *xname;
    int pk;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;
    sqlite3_stmt *stmt = NULL;
    gaiaOutBuffer sql_statement;
    gaiaOutBuffer col_list;

    *dupl_count = 0;

    if (is_table (sqlite, table) == 0)
      {
	  spatialite_e (".chkdupl %s: no such table\n", table);
	  *dupl_count = -1;
	  return;
      }
/* extracting the column names (excluding any Primary Key) */
    gaiaOutBufferInitialize (&col_list);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xname);
    free (xname);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQLite SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		sql = results[(i * columns) + 1];
		pk = atoi (results[(i * columns) + 5]);
		if (!pk)
		  {
		      xname = gaiaDoubleQuotedSql (sql);
		      if (first)
			{
			    sql = sqlite3_mprintf ("\"%s\"", xname);
			    first = 0;
			}
		      else
			  sql = sqlite3_mprintf (", \"%s\"", xname);
		      free (xname);
		      gaiaAppendToOutBuffer (&col_list, sql);
		      sqlite3_free (sql);
		  }
	    }
      }
    sqlite3_free_table (results);
    /* preparing the SQL statement */
    gaiaOutBufferInitialize (&sql_statement);
    gaiaAppendToOutBuffer (&sql_statement,
			   "SELECT Count(*) AS \"[dupl-count]\", ");
    if (col_list.Error == 0 && col_list.Buffer != NULL)
	gaiaAppendToOutBuffer (&sql_statement, col_list.Buffer);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("\nFROM \"%s\"\nGROUP BY ", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    if (col_list.Error == 0 && col_list.Buffer != NULL)
	gaiaAppendToOutBuffer (&sql_statement, col_list.Buffer);
    gaiaOutBufferReset (&col_list);
    gaiaAppendToOutBuffer (&sql_statement, "\nHAVING \"[dupl-count]\" > 1");
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
      {
	  ret =
	      sqlite3_prepare_v2 (sqlite, sql_statement.Buffer,
				  strlen (sql_statement.Buffer), &stmt, NULL);
	  gaiaOutBufferReset (&sql_statement);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
		return;
	    }
      }
    while (1)
      {
	  /* fetching the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* fetching a row */
		*dupl_count += sqlite3_column_int (stmt, 0) - 1;
	    }
	  else
	    {
		spatialite_e ("SQL error: %s", sqlite3_errmsg (sqlite));
		sqlite3_finalize (stmt);
		return;
	    }
      }
    sqlite3_finalize (stmt);
    if (*dupl_count)
	spatialite_e ("%d duplicated rows found !!!\n", *dupl_count);
    else
	spatialite_e ("No duplicated rows have been identified\n");
}

static int
do_delete_duplicates2 (sqlite3 * sqlite, sqlite3_int64 rowid,
		       sqlite3_stmt * stmt1)
{
/* deleting duplicate rows [actual delete] */
    int ret;

    sqlite3_reset (stmt1);
    sqlite3_clear_bindings (stmt1);
    sqlite3_bind_int64 (stmt1, 1, rowid);
    ret = sqlite3_step (stmt1);
    if (ret == SQLITE_DONE || ret == SQLITE_ROW)
	;
    else
      {
	  spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
	  return 0;
      }
    return 1;
}

static int
do_delete_duplicates (sqlite3 * sqlite, const char *sql1, const char *sql2,
		      int *count, int transaction)
{
/* deleting duplicate rows */
    struct resultset_comparator *rs_obj = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int ret;
    int cnt = 0;
    char *sql_err = NULL;

    *count = 0;

    if (transaction)
      {
	  /* the complete operation is handled as an unique SQL Transaction */
	  ret = sqlite3_exec (sqlite, "BEGIN", NULL, NULL, &sql_err);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("BEGIN TRANSACTION error: %s\n", sql_err);
		sqlite3_free (sql_err);
		return 0;
	    }
      }

/* preparing the main SELECT statement */
    ret = sqlite3_prepare_v2 (sqlite, sql1, strlen (sql1), &stmt1, NULL);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
	  return 0;
      }
/* preparing the DELETE statement */
    ret = sqlite3_prepare_v2 (sqlite, sql2, strlen (sql2), &stmt2, NULL);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
	  goto error;
      }

    rs_obj = create_resultset_comparator (sqlite3_column_count (stmt1) - 1);
    while (1)
      {
	  /* fetching the result set rows */
	  ret = sqlite3_step (stmt1);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		/* fetching a row */
		save_row_from_resultset (rs_obj, stmt1);
		if (resultset_rows_equals (rs_obj))
		  {
		      if (do_delete_duplicates2
			  (sqlite, get_current_resultset_rowid (rs_obj), stmt2))
			{
			    cnt += 1;
			    reset_resultset_current_row (rs_obj);
			    continue;
			}
		      else
			  goto error;
		  }
	    }
	  else
	    {
		spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
		goto error;
	    }
	  swap_resultset_rows (rs_obj);
      }

    sqlite3_finalize (stmt1);
    sqlite3_finalize (stmt2);
    destroy_resultset_comparator (rs_obj);

    if (transaction)
      {
	  /* confirm the still pending Transaction */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, NULL, &sql_err);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("COMMIT TRANSACTION error: %s\n", sql_err);
		sqlite3_free (sql_err);
		return 0;
	    }
      }

    *count = cnt;
    return 1;

  error:
    *count = 0;
    if (stmt1)
	sqlite3_finalize (stmt1);
    if (stmt2)
	sqlite3_finalize (stmt2);

    if (transaction)
      {
	  /* performing a ROLLBACK anyway */
	  ret = sqlite3_exec (sqlite, "ROLLBACK", NULL, NULL, &sql_err);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("ROLLBACK TRANSACTION error: %s\n", sql_err);
		sqlite3_free (sql_err);
		return 0;
	    }
      }

    return 0;
}

SPATIALITE_DECLARE void
remove_duplicated_rows_ex2 (sqlite3 * sqlite, char *table, int *removed,
			    int transaction)
{
/* attempting to delete Duplicate rows from a table */
    char *sql;
    char *sql2;
    int first = 1;
    char *xname;
    int pk;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *errMsg = NULL;
    int count;
    gaiaOutBuffer sql_statement;
    gaiaOutBuffer col_list;

    if (removed != NULL)
	*removed = 0;

    if (is_table (sqlite, table) == 0)
      {
	  spatialite_e (".remdupl %s: no such table\n", table);
	  if (removed != NULL)
	      *removed = -1;
	  return;
      }
/* extracting the column names (excluding any Primary Key) */
    gaiaOutBufferInitialize (&col_list);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xname);
    free (xname);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQLite SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		sql = results[(i * columns) + 1];
		pk = atoi (results[(i * columns) + 5]);
		if (!pk)
		  {
		      if (first)
			  first = 0;
		      else
			  gaiaAppendToOutBuffer (&col_list, ", ");
		      xname = gaiaDoubleQuotedSql (sql);
		      sql = sqlite3_mprintf ("\"%s\"", xname);
		      free (xname);
		      gaiaAppendToOutBuffer (&col_list, sql);
		      sqlite3_free (sql);
		  }
	    }
      }
    sqlite3_free_table (results);
/* preparing the SQL statement (identifying duplicated rows) */
    gaiaOutBufferInitialize (&sql_statement);
    gaiaAppendToOutBuffer (&sql_statement, "SELECT ROWID, ");
    if (col_list.Error == 0 && col_list.Buffer != NULL)
	gaiaAppendToOutBuffer (&sql_statement, col_list.Buffer);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("\nFROM \"%s\"\nORDER BY ", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    if (col_list.Error == 0 && col_list.Buffer != NULL)
	gaiaAppendToOutBuffer (&sql_statement, col_list.Buffer);
    gaiaOutBufferReset (&col_list);
    gaiaAppendToOutBuffer (&sql_statement, ", ROWID");
/* preparing the SQL statement [delete] */
    xname = gaiaDoubleQuotedSql (table);
    sql2 = sqlite3_mprintf ("DELETE FROM \"%s\" WHERE ROWID = ?", xname);
    free (xname);

    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
	sql = sql_statement.Buffer;
    else
	sql = "NULL-SELECT";
    if (do_delete_duplicates (sqlite, sql, sql2, &count, transaction))
      {
	  if (removed == NULL)
	    {
		if (!count)
		    spatialite_e ("No duplicated rows have been identified\n");
		else
		    spatialite_e ("%d duplicated rows deleted from: %s\n",
				  count, table);
	    }
	  else
	      *removed = count;
      }
    gaiaOutBufferReset (&sql_statement);
    sqlite3_free (sql2);
}

SPATIALITE_DECLARE void
remove_duplicated_rows_ex (sqlite3 * sqlite, char *table, int *removed)
{
/* attempting to delete Duplicate rows from a table */
    remove_duplicated_rows_ex2 (sqlite, table, removed, 1);
}

SPATIALITE_DECLARE void
remove_duplicated_rows (sqlite3 * sqlite, char *table)
{
/* attempting to delete Duplicate rows from a table */
    remove_duplicated_rows_ex (sqlite, table, NULL);
}

static int
check_elementary (sqlite3 * sqlite, const char *inTable, const char *geom,
		  const char *outTable, const char *pKey, const char *multiID,
		  char *type, int *srid, char *coordDims)
{
/* preliminary check for ELEMENTARY GEOMETRIES */
    char *sql;
    char *xtable;
    int ret;
    char **results;
    int rows;
    int columns;
    char *errMsg = NULL;
    int ok = 0;
    int i;
    char *gtp;
    char *dims;
    int metadata_version = checkSpatialMetaData (sqlite);

/* fetching metadata */
    if (metadata_version == 3)
      {
	  /* current metadata style >= v.4.0.0 */
	  sql = sqlite3_mprintf ("SELECT geometry_type, srid "
				 "FROM geometry_columns WHERE Lower(f_table_name) = Lower(%Q) "
				 "AND Lower(f_geometry_column) = Lower(%Q)",
				 inTable, geom);
      }
    else
      {
	  /* legacy metadata style <= v.3.1.0 */
	  sql = sqlite3_mprintf ("SELECT type, coord_dimension, srid "
				 "FROM geometry_columns WHERE Lower(f_table_name) = Lower(%Q) "
				 "AND Lower(f_geometry_column) = Lower(%Q)",
				 inTable, geom);
      }
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		if (metadata_version == 3)
		  {
		      /* current metadata style >= v.4.0.0 */
		      gtp = "UNKNOWN";
		      dims = "UNKNOWN";
		      switch (atoi (results[(i * columns) + 0]))
			{
			case 0:
			    gtp = "GEOMETRY";
			    dims = "XY";
			    break;
			case 1:
			    gtp = "POINT";
			    dims = "XY";
			    break;
			case 2:
			    gtp = "LINESTRING";
			    dims = "XY";
			    break;
			case 3:
			    gtp = "POLYGON";
			    dims = "XY";
			    break;
			case 4:
			    gtp = "MULTIPOINT";
			    dims = "XY";
			    break;
			case 5:
			    gtp = "MULTILINESTRING";
			    dims = "XY";
			    break;
			case 6:
			    gtp = "MULTIPOLYGON";
			    dims = "XY";
			    break;
			case 7:
			    gtp = "GEOMETRYCOLLECTION";
			    dims = "XY";
			    break;
			case 1000:
			    gtp = "GEOMETRY";
			    dims = "XYZ";
			    break;
			case 1001:
			    gtp = "POINT";
			    dims = "XYZ";
			    break;
			case 1002:
			    gtp = "LINESTRING";
			    dims = "XYZ";
			    break;
			case 1003:
			    gtp = "POLYGON";
			    dims = "XYZ";
			    break;
			case 1004:
			    gtp = "MULTIPOINT";
			    dims = "XYZ";
			    break;
			case 1005:
			    gtp = "MULTILINESTRING";
			    dims = "XYZ";
			    break;
			case 1006:
			    gtp = "MULTIPOLYGON";
			    dims = "XYZ";
			    break;
			case 1007:
			    gtp = "GEOMETRYCOLLECTION";
			    dims = "XYZ";
			    break;
			case 2000:
			    gtp = "GEOMETRY";
			    dims = "XYM";
			    break;
			case 2001:
			    gtp = "POINT";
			    dims = "XYM";
			    break;
			case 2002:
			    gtp = "LINESTRING";
			    dims = "XYM";
			    break;
			case 2003:
			    gtp = "POLYGON";
			    dims = "XYM";
			    break;
			case 2004:
			    gtp = "MULTIPOINT";
			    dims = "XYM";
			    break;
			case 2005:
			    gtp = "MULTILINESTRING";
			    dims = "XYM";
			    break;
			case 2006:
			    gtp = "MULTIPOLYGON";
			    dims = "XYM";
			    break;
			case 2007:
			    gtp = "GEOMETRYCOLLECTION";
			    dims = "XYM";
			    break;
			case 3000:
			    gtp = "GEOMETRY";
			    dims = "XYZM";
			    break;
			case 3001:
			    gtp = "POINT";
			    dims = "XYZM";
			    break;
			case 3002:
			    gtp = "LINESTRING";
			    dims = "XYZM";
			    break;
			case 3003:
			    gtp = "POLYGON";
			    dims = "XYZM";
			    break;
			case 3004:
			    gtp = "MULTIPOINT";
			    dims = "XYZM";
			    break;
			case 3005:
			    gtp = "MULTILINESTRING";
			    dims = "XYZM";
			    break;
			case 3006:
			    gtp = "MULTIPOLYGON";
			    dims = "XYZM";
			    break;
			case 3007:
			    gtp = "GEOMETRYCOLLECTION";
			    dims = "XYZM";
			    break;
			};
		      *srid = atoi (results[(i * columns) + 1]);
		  }
		else
		  {
		      /* legacy metadata style <= v.3.1.0 */
		      gtp = results[(i * columns) + 0];
		      dims = results[(i * columns) + 1];
		      *srid = atoi (results[(i * columns) + 2]);
		  }
		if (strcasecmp (gtp, "POINT") == 0
		    || strcasecmp (gtp, "MULTIPOINT") == 0)
		    strcpy (type, "POINT");
		else if (strcasecmp (gtp, "LINESTRING") == 0
			 || strcasecmp (gtp, "MULTILINESTRING") == 0)
		    strcpy (type, "LINESTRING");
		else if (strcasecmp (gtp, "POLYGON") == 0
			 || strcasecmp (gtp, "MULTIPOLYGON") == 0)
		    strcpy (type, "POLYGON");
		else
		    strcpy (type, "GEOMETRY");
		strcpy (coordDims, dims);
		ok = 1;
	    }
      }
    sqlite3_free_table (results);
    if (!ok)
	return 0;

/* checking if PrimaryKey already exists */
    xtable = gaiaDoubleQuotedSql (inTable);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xtable);
    free (xtable);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		if (strcasecmp (pKey, results[(i * columns) + 1]) == 0)
		    ok = 0;
	    }
      }
    sqlite3_free_table (results);
    if (!ok)
	return 0;

/* checking if MultiID already exists */
    xtable = gaiaDoubleQuotedSql (inTable);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xtable);
    free (xtable);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		if (strcasecmp (multiID, results[(i * columns) + 1]) == 0)
		    ok = 0;
	    }
      }
    sqlite3_free_table (results);
    if (!ok)
	return 0;

/* cheching if Output Table already exists */
    sql = sqlite3_mprintf ("SELECT Count(*) FROM sqlite_master "
			   "WHERE type = 'table' AND Lower(tbl_name) = Lower(%Q)",
			   outTable);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  return 0;
      }
    if (rows < 1)
	;
    else
      {
	  for (i = 1; i <= rows; i++)
	    {
		if (atoi (results[(i * columns) + 0]) != 0)
		    ok = 0;
	    }
      }
    sqlite3_free_table (results);

    return ok;
}

static gaiaGeomCollPtr
elemGeomFromPoint (gaiaPointPtr pt, int srid)
{
/* creating a Geometry containing a single Point */
    gaiaGeomCollPtr g = NULL;
    switch (pt->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  g = gaiaAllocGeomCollXYZM ();
	  break;
      case GAIA_XY_Z:
	  g = gaiaAllocGeomCollXYZ ();
	  break;
      case GAIA_XY_M:
	  g = gaiaAllocGeomCollXYM ();
	  break;
      default:
	  g = gaiaAllocGeomColl ();
	  break;
      };
    if (!g)
	return NULL;
    g->Srid = srid;
    g->DeclaredType = GAIA_POINT;
    switch (pt->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  gaiaAddPointToGeomCollXYZM (g, pt->X, pt->Y, pt->Z, pt->M);
	  break;
      case GAIA_XY_Z:
	  gaiaAddPointToGeomCollXYZ (g, pt->X, pt->Y, pt->Z);
	  break;
      case GAIA_XY_M:
	  gaiaAddPointToGeomCollXYM (g, pt->X, pt->Y, pt->M);
	  break;
      default:
	  gaiaAddPointToGeomColl (g, pt->X, pt->Y);
	  break;
      };
    return g;
}

static gaiaGeomCollPtr
elemGeomFromLinestring (gaiaLinestringPtr ln, int srid)
{
/* creating a Geometry containing a single Linestring */
    gaiaGeomCollPtr g = NULL;
    gaiaLinestringPtr ln2;
    int iv;
    double x;
    double y;
    double z;
    double m;
    switch (ln->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  g = gaiaAllocGeomCollXYZM ();
	  break;
      case GAIA_XY_Z:
	  g = gaiaAllocGeomCollXYZ ();
	  break;
      case GAIA_XY_M:
	  g = gaiaAllocGeomCollXYM ();
	  break;
      default:
	  g = gaiaAllocGeomColl ();
	  break;
      };
    if (!g)
	return NULL;
    g->Srid = srid;
    g->DeclaredType = GAIA_LINESTRING;
    ln2 = gaiaAddLinestringToGeomColl (g, ln->Points);
    switch (ln->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  for (iv = 0; iv < ln->Points; iv++)
	    {
		gaiaGetPointXYZM (ln->Coords, iv, &x, &y, &z, &m);
		gaiaSetPointXYZM (ln2->Coords, iv, x, y, z, m);
	    }
	  break;
      case GAIA_XY_Z:
	  for (iv = 0; iv < ln->Points; iv++)
	    {
		gaiaGetPointXYZ (ln->Coords, iv, &x, &y, &z);
		gaiaSetPointXYZ (ln2->Coords, iv, x, y, z);
	    }
	  break;
      case GAIA_XY_M:
	  for (iv = 0; iv < ln->Points; iv++)
	    {
		gaiaGetPointXYM (ln->Coords, iv, &x, &y, &m);
		gaiaSetPointXYM (ln2->Coords, iv, x, y, m);
	    }
	  break;
      default:
	  for (iv = 0; iv < ln->Points; iv++)
	    {
		gaiaGetPoint (ln->Coords, iv, &x, &y);
		gaiaSetPoint (ln2->Coords, iv, x, y);
	    }
	  break;
      };
    return g;
}

static gaiaGeomCollPtr
elemGeomFromPolygon (gaiaPolygonPtr pg, int srid)
{
/* creating a Geometry containing a single Polygon */
    gaiaGeomCollPtr g = NULL;
    gaiaPolygonPtr pg2;
    gaiaRingPtr rng;
    gaiaRingPtr rng2;
    int ib;
    int iv;
    double x;
    double y;
    double z;
    double m;
    switch (pg->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  g = gaiaAllocGeomCollXYZM ();
	  break;
      case GAIA_XY_Z:
	  g = gaiaAllocGeomCollXYZ ();
	  break;
      case GAIA_XY_M:
	  g = gaiaAllocGeomCollXYM ();
	  break;
      default:
	  g = gaiaAllocGeomColl ();
	  break;
      };
    if (!g)
	return NULL;
    g->Srid = srid;
    g->DeclaredType = GAIA_POLYGON;
    rng = pg->Exterior;
    pg2 = gaiaAddPolygonToGeomColl (g, rng->Points, pg->NumInteriors);
    rng2 = pg2->Exterior;
    switch (pg->DimensionModel)
      {
      case GAIA_XY_Z_M:
	  for (iv = 0; iv < rng->Points; iv++)
	    {
		gaiaGetPointXYZM (rng->Coords, iv, &x, &y, &z, &m);
		gaiaSetPointXYZM (rng2->Coords, iv, x, y, z, m);
	    }
	  for (ib = 0; ib < pg->NumInteriors; ib++)
	    {
		rng = pg->Interiors + ib;
		rng2 = gaiaAddInteriorRing (pg2, ib, rng->Points);
		for (iv = 0; iv < rng->Points; iv++)
		  {
		      gaiaGetPointXYZM (rng->Coords, iv, &x, &y, &z, &m);
		      gaiaSetPointXYZM (rng2->Coords, iv, x, y, z, m);
		  }
	    }
	  break;
      case GAIA_XY_Z:
	  for (iv = 0; iv < rng->Points; iv++)
	    {
		gaiaGetPointXYZ (rng->Coords, iv, &x, &y, &z);
		gaiaSetPointXYZ (rng2->Coords, iv, x, y, z);
	    }
	  for (ib = 0; ib < pg->NumInteriors; ib++)
	    {
		rng = pg->Interiors + ib;
		rng2 = gaiaAddInteriorRing (pg2, ib, rng->Points);
		for (iv = 0; iv < rng->Points; iv++)
		  {
		      gaiaGetPointXYZ (rng->Coords, iv, &x, &y, &z);
		      gaiaSetPointXYZ (rng2->Coords, iv, x, y, z);
		  }
	    }
	  break;
      case GAIA_XY_M:
	  for (iv = 0; iv < rng->Points; iv++)
	    {
		gaiaGetPointXYM (rng->Coords, iv, &x, &y, &m);
		gaiaSetPointXYM (rng2->Coords, iv, x, y, m);
	    }
	  for (ib = 0; ib < pg->NumInteriors; ib++)
	    {
		rng = pg->Interiors + ib;
		rng2 = gaiaAddInteriorRing (pg2, ib, rng->Points);
		for (iv = 0; iv < rng->Points; iv++)
		  {
		      gaiaGetPointXYM (rng->Coords, iv, &x, &y, &m);
		      gaiaSetPointXYM (rng2->Coords, iv, x, y, m);
		  }
	    }
	  break;
      default:
	  for (iv = 0; iv < rng->Points; iv++)
	    {
		gaiaGetPoint (rng->Coords, iv, &x, &y);
		gaiaSetPoint (rng2->Coords, iv, x, y);
	    }
	  for (ib = 0; ib < pg->NumInteriors; ib++)
	    {
		rng = pg->Interiors + ib;
		rng2 = gaiaAddInteriorRing (pg2, ib, rng->Points);
		for (iv = 0; iv < rng->Points; iv++)
		  {
		      gaiaGetPoint (rng->Coords, iv, &x, &y);
		      gaiaSetPoint (rng2->Coords, iv, x, y);
		  }
	    }
	  break;
      };
    return g;
}

SPATIALITE_DECLARE void
elementary_geometries (sqlite3 * sqlite,
		       char *inTable, char *geometry, char *outTable,
		       char *pKey, char *multiId)
{
/* attempting to create a derived table surely containing elemetary Geoms */
    int rows;
    elementary_geometries_ex (sqlite, inTable, geometry, outTable, pKey,
			      multiId, &rows);
}

SPATIALITE_DECLARE void
elementary_geometries_ex (sqlite3 * sqlite,
			  char *inTable, char *geometry, char *outTable,
			  char *pKey, char *multiId, int *xrows)
{
/* attempting to create a derived table surely containing elemetary Geoms */
    elementary_geometries_ex2 (sqlite, inTable, geometry, outTable, pKey,
			       multiId, xrows, 1);
}

SPATIALITE_DECLARE void
elementary_geometries_ex2 (sqlite3 * sqlite,
			   char *inTable, char *geometry, char *outTable,
			   char *pKey, char *multiId, int *xrows,
			   int transaction)
{
/* attempting to create a derived table surely containing elemetary Geoms */
    elementary_geometries_ex3 (sqlite, inTable, geometry, outTable, pKey,
			       multiId, NULL, xrows, transaction);
}

static int
test_elemgeom_ignore (struct aux_elemgeom_options *options, const char *column)
{
/* testing for a column to be ignored */
    struct aux_elemgeom_ignore *ign;
    if (options == NULL)
	return 0;

    ign = options->first;
    while (ign != NULL)
      {
	  if (strcasecmp (ign->column, column) == 0)
	      return 1;
	  ign = ign->next;
      }
    return 0;
}

SPATIALITE_DECLARE void
elementary_geometries_ex3 (sqlite3 * sqlite,
			   char *inTable, char *geometry, char *outTable,
			   char *pKey, char *multiId, const void *opts,
			   int *xrows, int transaction)
{
/* attempting to create a derived table surely containing elemetary Geoms */
    char type[128];
    int srid;
    char dims[64];
    char *sql;
    char *xname;
    char *xpk;
    char *xmulti;
    gaiaOutBuffer sql_statement;
    gaiaOutBuffer sql2;
    gaiaOutBuffer sql3;
    gaiaOutBuffer sql4;
    char *sql_geom;
    int ret;
    int comma = 0;
    char *errMsg = NULL;
    int i;
    char **results;
    int rows;
    int columns;
    int i_col;
    int geom_idx = -1;
    sqlite3_stmt *stmt_in = NULL;
    sqlite3_stmt *stmt_out = NULL;
    int n_columns;
    sqlite3_int64 id = 0;
    int inserted = 0;
    struct aux_elemgeom_options *options = (struct aux_elemgeom_options *) opts;

    if (check_elementary
	(sqlite, inTable, geometry, outTable, pKey, multiId, type, &srid,
	 dims) == 0)
      {
	  spatialite_e (".elemgeo: invalid args\n");
	  *xrows = 0;
	  return;
      }

    if (transaction)
      {
	  /* starts a transaction */
	  ret = sqlite3_exec (sqlite, "BEGIN", NULL, NULL, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", errMsg);
		sqlite3_free (errMsg);
		goto abort;
	    }
      }

    gaiaOutBufferInitialize (&sql_statement);
    gaiaOutBufferInitialize (&sql2);
    gaiaOutBufferInitialize (&sql3);
    gaiaOutBufferInitialize (&sql4);

    gaiaAppendToOutBuffer (&sql_statement, "SELECT ");
    xname = gaiaDoubleQuotedSql (outTable);
    xpk = gaiaDoubleQuotedSql (pKey);
    xmulti = gaiaDoubleQuotedSql (multiId);
    sql =
	sqlite3_mprintf ("INSERT INTO \"%s\" (\"%s\", \"%s\", ", xname, xpk,
			 xmulti);
    free (xname);
    free (xpk);
    free (xmulti);
    gaiaAppendToOutBuffer (&sql2, sql);
    sqlite3_free (sql);
    gaiaAppendToOutBuffer (&sql3, ") VALUES (NULL, ?");
    xname = gaiaDoubleQuotedSql (outTable);
    xpk = gaiaDoubleQuotedSql (pKey);
    xmulti = gaiaDoubleQuotedSql (multiId);
    sql = sqlite3_mprintf ("CREATE TABLE \"%s\" (\n"
			   "\t\"%s\" INTEGER PRIMARY KEY AUTOINCREMENT"
			   ",\n\t\"%s\" INTEGER NOT NULL", xname, xpk, xmulti);
    free (xname);
    free (xpk);
    free (xmulti);
    gaiaAppendToOutBuffer (&sql4, sql);
    sqlite3_free (sql);

    xname = gaiaDoubleQuotedSql (inTable);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xname);
    free (xname);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &errMsg);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  goto abort;
      }
    if (rows < 1)
	;
    else
      {
	  i_col = 0;
	  for (i = 1; i <= rows; i++)
	    {
		if (test_elemgeom_ignore (options, results[(i * columns) + 1])
		    && strcasecmp (geometry, results[(i * columns) + 1]) != 0)
		  {
		      /* skipping a column to be ignored */
		      continue;
		  }
		i_col++;
		xname = gaiaDoubleQuotedSql (results[(i * columns) + 1]);
		if (comma)
		    sql = sqlite3_mprintf (", \"%s\"", xname);
		else
		  {
		      comma = 1;
		      sql = sqlite3_mprintf ("\"%s\"", xname);
		  }
		free (xname);
		gaiaAppendToOutBuffer (&sql_statement, sql);
		gaiaAppendToOutBuffer (&sql2, sql);
		if (strcasecmp (geometry, results[(i * columns) + 1]) == 0)
		  {
		      int cast2multi = 0;
		      if (options != NULL)
			  cast2multi = options->cast2multi;
		      if (cast2multi)
			  gaiaAppendToOutBuffer (&sql3, ", CastToMulti(?)");
		      else
			  gaiaAppendToOutBuffer (&sql3, ", ?");
		  }
		else
		    gaiaAppendToOutBuffer (&sql3, ", ?");
		sqlite3_free (sql);

		if (strcasecmp (geometry, results[(i * columns) + 1]) == 0)
		    geom_idx = i_col - 1;
		else
		  {
		      xname = gaiaDoubleQuotedSql (results[(i * columns) + 1]);
		      if (atoi (results[(i * columns) + 3]) != 0)
			  sql =
			      sqlite3_mprintf (",\n\t\"%s\" %s NOT NULL", xname,
					       results[(i * columns) + 2]);
		      else
			  sql =
			      sqlite3_mprintf (",\n\t\"%s\" %s", xname,
					       results[(i * columns) + 2]);
		      free (xname);
		      gaiaAppendToOutBuffer (&sql4, sql);
		      sqlite3_free (sql);
		  }
	    }
      }
    sqlite3_free_table (results);
    if (geom_idx < 0)
	goto abort;

    xname = gaiaDoubleQuotedSql (inTable);
    sql = sqlite3_mprintf (" FROM \"%s\"", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    gaiaAppendToOutBuffer (&sql2, sql3.Buffer);
    gaiaAppendToOutBuffer (&sql2, ")");
    gaiaAppendToOutBuffer (&sql4, ")");
    gaiaOutBufferReset (&sql3);

    if (options != NULL)
      {
	  if (options->cast2multi)
	    {
		char multi_type[128];
		if (strcasecmp (type, "POINT") == 0)
		    strcpy (multi_type, "MULTIPOINT");
		else if (strcasecmp (type, "LINESTRING") == 0)
		    strcpy (multi_type, "MULTILINESTRING");
		else if (strcasecmp (type, "POLYGON") == 0)
		    strcpy (multi_type, "MULTIPOLYGON");
		else
		    strcpy (multi_type, type);
		sql_geom =
		    sqlite3_mprintf
		    ("SELECT AddGeometryColumn(%Q, %Q, %d, %Q, %Q)", outTable,
		     geometry, srid, multi_type, dims);
	    }
	  else
	    {
		sql_geom =
		    sqlite3_mprintf
		    ("SELECT AddGeometryColumn(%Q, %Q, %d, %Q, %Q)", outTable,
		     geometry, srid, type, dims);
	    }
      }
    else
      {
	  sql_geom =
	      sqlite3_mprintf ("SELECT AddGeometryColumn(%Q, %Q, %d, %Q, %Q)",
			       outTable, geometry, srid, type, dims);
      }

/* creating the output table */
    ret = sqlite3_exec (sqlite, sql4.Buffer, NULL, NULL, &errMsg);
    gaiaOutBufferReset (&sql4);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  goto abort;
      }
/* creating the output Geometry */
    ret = sqlite3_exec (sqlite, sql_geom, NULL, NULL, &errMsg);
    sqlite3_free (sql_geom);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  goto abort;
      }

/* preparing the INPUT statement */
    ret =
	sqlite3_prepare_v2 (sqlite, sql_statement.Buffer,
			    strlen (sql_statement.Buffer), &stmt_in, NULL);
    gaiaOutBufferReset (&sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
	  goto abort;
      }

/* preparing the OUTPUT statement */
    ret =
	sqlite3_prepare_v2 (sqlite, sql2.Buffer, strlen (sql2.Buffer),
			    &stmt_out, NULL);
    gaiaOutBufferReset (&sql2);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", sqlite3_errmsg (sqlite));
	  goto abort;
      }

/* data transfer */
    n_columns = sqlite3_column_count (stmt_in);
    while (1)
      {
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;
	  if (ret == SQLITE_ROW)
	    {
		gaiaGeomCollPtr g =
		    gaiaFromSpatiaLiteBlobWkb ((const unsigned char *)
					       sqlite3_column_blob (stmt_in,
								    geom_idx),
					       sqlite3_column_bytes (stmt_in,
								     geom_idx));
		if (!g)
		  {
		      /* NULL input geometry */
		      sqlite3_reset (stmt_out);
		      sqlite3_clear_bindings (stmt_out);
		      sqlite3_bind_int64 (stmt_out, 1, id);
		      sqlite3_bind_null (stmt_out, geom_idx + 2);

		      for (i = 0; i < n_columns; i++)
			{
			    int type = sqlite3_column_type (stmt_in, i);
			    if (i == geom_idx)
				continue;
			    switch (type)
			      {
			      case SQLITE_INTEGER:
				  sqlite3_bind_int64 (stmt_out, i + 2,
						      sqlite3_column_int64
						      (stmt_in, i));
				  break;
			      case SQLITE_FLOAT:
				  sqlite3_bind_double (stmt_out, i + 2,
						       sqlite3_column_double
						       (stmt_in, i));
				  break;
			      case SQLITE_TEXT:
				  sqlite3_bind_text (stmt_out, i + 2,
						     (const char *)
						     sqlite3_column_text
						     (stmt_in, i),
						     sqlite3_column_bytes
						     (stmt_in, i),
						     SQLITE_STATIC);
				  break;
			      case SQLITE_BLOB:
				  sqlite3_bind_blob (stmt_out, i + 2,
						     sqlite3_column_blob
						     (stmt_in, i),
						     sqlite3_column_bytes
						     (stmt_in, i),
						     SQLITE_STATIC);
				  break;
			      case SQLITE_NULL:
			      default:
				  sqlite3_bind_null (stmt_out, i + 2);
				  break;
			      };
			}

		      ret = sqlite3_step (stmt_out);
		      if (ret == SQLITE_DONE || ret == SQLITE_ROW)
			  ;
		      else
			{
			    spatialite_e ("[OUT]step error: %s\n",
					  sqlite3_errmsg (sqlite));
			    goto abort;
			}
		      inserted++;
		  }
		else
		  {
		      /* separating Elementary Geoms */
		      gaiaPointPtr pt;
		      gaiaLinestringPtr ln;
		      gaiaPolygonPtr pg;
		      gaiaGeomCollPtr outGeom;
		      pt = g->FirstPoint;
		      while (pt)
			{
			    /* separating Points */
			    outGeom = elemGeomFromPoint (pt, g->Srid);
			    sqlite3_reset (stmt_out);
			    sqlite3_clear_bindings (stmt_out);
			    sqlite3_bind_int64 (stmt_out, 1, id);
			    if (!outGeom)
				sqlite3_bind_null (stmt_out, geom_idx + 2);
			    else
			      {
				  unsigned char *blob;
				  int size;
				  gaiaToSpatiaLiteBlobWkb (outGeom, &blob,
							   &size);
				  sqlite3_bind_blob (stmt_out, geom_idx + 2,
						     blob, size, free);
				  gaiaFreeGeomColl (outGeom);
			      }

			    for (i = 0; i < n_columns; i++)
			      {
				  int type = sqlite3_column_type (stmt_in, i);
				  if (i == geom_idx)
				      continue;
				  switch (type)
				    {
				    case SQLITE_INTEGER:
					sqlite3_bind_int64 (stmt_out, i + 2,
							    sqlite3_column_int64
							    (stmt_in, i));
					break;
				    case SQLITE_FLOAT:
					sqlite3_bind_double (stmt_out, i + 2,
							     sqlite3_column_double
							     (stmt_in, i));
					break;
				    case SQLITE_TEXT:
					sqlite3_bind_text (stmt_out, i + 2,
							   (const char *)
							   sqlite3_column_text
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_BLOB:
					sqlite3_bind_blob (stmt_out, i + 2,
							   sqlite3_column_blob
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_NULL:
				    default:
					sqlite3_bind_null (stmt_out, i + 2);
					break;
				    };
			      }

			    ret = sqlite3_step (stmt_out);
			    if (ret == SQLITE_DONE || ret == SQLITE_ROW)
				;
			    else
			      {
				  spatialite_e ("[OUT]step error: %s\n",
						sqlite3_errmsg (sqlite));
				  goto abort;
			      }
			    inserted++;
			    pt = pt->Next;
			}
		      ln = g->FirstLinestring;
		      while (ln)
			{
			    /* separating Linestrings */
			    outGeom = elemGeomFromLinestring (ln, g->Srid);
			    sqlite3_reset (stmt_out);
			    sqlite3_clear_bindings (stmt_out);
			    sqlite3_bind_int64 (stmt_out, 1, id);
			    if (!outGeom)
				sqlite3_bind_null (stmt_out, geom_idx + 2);
			    else
			      {
				  unsigned char *blob;
				  int size;
				  gaiaToSpatiaLiteBlobWkb (outGeom, &blob,
							   &size);
				  sqlite3_bind_blob (stmt_out, geom_idx + 2,
						     blob, size, free);
				  gaiaFreeGeomColl (outGeom);
			      }

			    for (i = 0; i < n_columns; i++)
			      {
				  int type = sqlite3_column_type (stmt_in, i);
				  if (i == geom_idx)
				      continue;
				  switch (type)
				    {
				    case SQLITE_INTEGER:
					sqlite3_bind_int64 (stmt_out, i + 2,
							    sqlite3_column_int64
							    (stmt_in, i));
					break;
				    case SQLITE_FLOAT:
					sqlite3_bind_double (stmt_out, i + 2,
							     sqlite3_column_double
							     (stmt_in, i));
					break;
				    case SQLITE_TEXT:
					sqlite3_bind_text (stmt_out, i + 2,
							   (const char *)
							   sqlite3_column_text
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_BLOB:
					sqlite3_bind_blob (stmt_out, i + 2,
							   sqlite3_column_blob
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_NULL:
				    default:
					sqlite3_bind_null (stmt_out, i + 2);
					break;
				    };
			      }

			    ret = sqlite3_step (stmt_out);
			    if (ret == SQLITE_DONE || ret == SQLITE_ROW)
				;
			    else
			      {
				  spatialite_e ("[OUT]step error: %s\n",
						sqlite3_errmsg (sqlite));
				  goto abort;
			      }
			    inserted++;
			    ln = ln->Next;
			}
		      pg = g->FirstPolygon;
		      while (pg)
			{
			    /* separating Polygons */
			    outGeom = elemGeomFromPolygon (pg, g->Srid);
			    sqlite3_reset (stmt_out);
			    sqlite3_clear_bindings (stmt_out);
			    sqlite3_bind_int64 (stmt_out, 1, id);
			    if (!outGeom)
				sqlite3_bind_null (stmt_out, geom_idx + 2);
			    else
			      {
				  unsigned char *blob;
				  int size;
				  gaiaToSpatiaLiteBlobWkb (outGeom, &blob,
							   &size);
				  sqlite3_bind_blob (stmt_out, geom_idx + 2,
						     blob, size, free);
				  gaiaFreeGeomColl (outGeom);
			      }

			    for (i = 0; i < n_columns; i++)
			      {
				  int type = sqlite3_column_type (stmt_in, i);
				  if (i == geom_idx)
				      continue;
				  switch (type)
				    {
				    case SQLITE_INTEGER:
					sqlite3_bind_int64 (stmt_out, i + 2,
							    sqlite3_column_int64
							    (stmt_in, i));
					break;
				    case SQLITE_FLOAT:
					sqlite3_bind_double (stmt_out, i + 2,
							     sqlite3_column_double
							     (stmt_in, i));
					break;
				    case SQLITE_TEXT:
					sqlite3_bind_text (stmt_out, i + 2,
							   (const char *)
							   sqlite3_column_text
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_BLOB:
					sqlite3_bind_blob (stmt_out, i + 2,
							   sqlite3_column_blob
							   (stmt_in, i),
							   sqlite3_column_bytes
							   (stmt_in, i),
							   SQLITE_STATIC);
					break;
				    case SQLITE_NULL:
				    default:
					sqlite3_bind_null (stmt_out, i + 2);
					break;
				    };
			      }

			    ret = sqlite3_step (stmt_out);
			    if (ret == SQLITE_DONE || ret == SQLITE_ROW)
				;
			    else
			      {
				  spatialite_e ("[OUT]step error: %s\n",
						sqlite3_errmsg (sqlite));
				  goto abort;
			      }
			    inserted++;
			    pg = pg->Next;
			}
		      gaiaFreeGeomColl (g);
		  }
		id++;
	    }
	  else
	    {
		spatialite_e ("[IN]step error: %s\n", sqlite3_errmsg (sqlite));
		goto abort;
	    }
      }
    sqlite3_finalize (stmt_in);
    sqlite3_finalize (stmt_out);

    if (transaction)
      {
	  /* commits the transaction */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, NULL, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", errMsg);
		sqlite3_free (errMsg);
		goto abort;
	    }
      }
    *xrows = inserted;
    return;

  abort:
    if (stmt_in)
	sqlite3_finalize (stmt_in);
    if (stmt_out)
	sqlite3_finalize (stmt_out);

    if (transaction)
      {
	  /* rolling bacj the transaction */
	  ret = sqlite3_exec (sqlite, "ROLLBACK", NULL, NULL, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", errMsg);
		sqlite3_free (errMsg);
	    }
      }
    *xrows = 0;
}

#ifndef OMIT_FREEXL		/* including FreeXL */

SPATIALITE_DECLARE int
load_XL (sqlite3 * sqlite, const char *path, const char *table,
	 unsigned int worksheetIndex, int first_titles, unsigned int *rows,
	 char *err_msg)
{
/* loading an XL spreadsheet as a new DB table */
    sqlite3_stmt *stmt;
    unsigned int current_row;
    int ret;
    char *errMsg = NULL;
    char *xname;
    char *dummy;
    char *xdummy;
    char *sql;
    int sqlError = 0;
    const void *xl_handle;
    unsigned int info;
    unsigned short columns;
    unsigned short col;
    gaiaOutBuffer sql_statement;
    FreeXL_CellValue cell;
    int already_exists = 0;

    *rows = 0;
/* checking if TABLE already exists */
    sql =
	sqlite3_mprintf ("SELECT name FROM sqlite_master WHERE type = 'table' "
			 "AND Lower(name) = Lower(%Q)", table);
    ret = sqlite3_prepare_v2 (sqlite, sql, strlen (sql), &stmt, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
      {
	  if (!err_msg)
	      spatialite_e ("load XL error: <%s>\n", sqlite3_errmsg (sqlite));
	  else
	      sprintf (err_msg, "load XL error: <%s>\n",
		       sqlite3_errmsg (sqlite));
	  return 0;
      }
    while (1)
      {
	  /* scrolling the result set */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	      already_exists = 1;
	  else
	    {
		spatialite_e ("load XL error: <%s>\n", sqlite3_errmsg (sqlite));
		break;
	    }
      }
    sqlite3_finalize (stmt);
    if (already_exists)
      {
	  if (!err_msg)
	      spatialite_e ("load XL error: table '%s' already exists\n",
			    table);
	  else
	      sprintf (err_msg, "load XL error: table '%s' already exists\n",
		       table);
	  return 0;
      }
/* opening the .XLS file [Workbook] */
    ret = freexl_open (path, &xl_handle);
    if (ret != FREEXL_OK)
	goto error;
/* checking if Password protected */
    ret = freexl_get_info (xl_handle, FREEXL_BIFF_PASSWORD, &info);
    if (ret != FREEXL_OK)
	goto error;
    if (info != FREEXL_BIFF_PLAIN)
	goto error;
/* Worksheet entries */
    ret = freexl_get_info (xl_handle, FREEXL_BIFF_SHEET_COUNT, &info);
    if (ret != FREEXL_OK)
	goto error;
    if (info == 0)
	goto error;
    if (worksheetIndex < info)
	;
    else
	goto error;
    ret =
	freexl_select_active_worksheet (xl_handle,
					(unsigned short) worksheetIndex);
    if (ret != FREEXL_OK)
	goto error;
    ret = freexl_worksheet_dimensions (xl_handle, rows, &columns);
    if (ret != FREEXL_OK)
	goto error;
/* starting a transaction */
    ret = sqlite3_exec (sqlite, "BEGIN", NULL, 0, &errMsg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("load XL error: %s\n", errMsg);
	  sqlite3_free (errMsg);
	  sqlError = 1;
	  goto clean_up;
      }
/* creating the Table */
    gaiaOutBufferInitialize (&sql_statement);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("CREATE TABLE \"%s\"", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    gaiaAppendToOutBuffer (&sql_statement,
			   " (\nPK_UID INTEGER PRIMARY KEY AUTOINCREMENT");
    for (col = 0; col < columns; col++)
      {
	  if (first_titles)
	    {
		/* fetching column names */
		for (col = 0; col < columns; col++)
		  {
		      ret = freexl_get_cell_value (xl_handle, 0, col, &cell);
		      if (ret != FREEXL_OK)
			  dummy = sqlite3_mprintf ("col_%d", col);
		      else
			{
			    if (cell.type == FREEXL_CELL_INT)
				dummy =
				    sqlite3_mprintf ("%d",
						     cell.value.int_value);
			    else if (cell.type == FREEXL_CELL_DOUBLE)
				dummy = sqlite3_mprintf ("%1.2f ",
							 cell.
							 value.double_value);
			    else if (cell.type == FREEXL_CELL_TEXT
				     || cell.type == FREEXL_CELL_SST_TEXT
				     || cell.type == FREEXL_CELL_DATE
				     || cell.type == FREEXL_CELL_DATETIME
				     || cell.type == FREEXL_CELL_TIME)
			      {
				  int len = strlen (cell.value.text_value);
				  if (len < 256)
				      dummy =
					  sqlite3_mprintf ("%s",
							   cell.
							   value.text_value);
				  else
				      dummy = sqlite3_mprintf ("col_%d", col);
			      }
			    else
				dummy = sqlite3_mprintf ("col_%d", col);
			}
		      xdummy = gaiaDoubleQuotedSql (dummy);
		      sqlite3_free (dummy);
		      sql = sqlite3_mprintf (", \"%s\"", xdummy);
		      free (xdummy);
		      gaiaAppendToOutBuffer (&sql_statement, sql);
		      sqlite3_free (sql);
		  }
	    }
	  else
	    {
		/* setting default column names */
		for (col = 0; col < columns; col++)
		  {
		      dummy = sqlite3_mprintf ("col_%d", col);
		      xdummy = gaiaDoubleQuotedSql (dummy);
		      sqlite3_free (dummy);
		      sql = sqlite3_mprintf (", \"%s\"", xdummy);
		      free (xdummy);
		      gaiaAppendToOutBuffer (&sql_statement, sql);
		      sqlite3_free (sql);
		  }
	    }
      }
    gaiaAppendToOutBuffer (&sql_statement, ")");
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
      {
	  ret = sqlite3_exec (sqlite, sql_statement.Buffer, NULL, 0, &errMsg);
	  gaiaOutBufferReset (&sql_statement);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load XL error: %s\n", errMsg);
		sqlite3_free (errMsg);
		sqlError = 1;
		goto clean_up;
	    }
      }
/* preparing the INSERT INTO parameterized statement */
    gaiaOutBufferReset (&sql_statement);
    xname = gaiaDoubleQuotedSql (table);
    sql = sqlite3_mprintf ("INSERT INTO \"%s\" (PK_UID", xname);
    free (xname);
    gaiaAppendToOutBuffer (&sql_statement, sql);
    sqlite3_free (sql);
    for (col = 0; col < columns; col++)
      {
	  if (first_titles)
	    {
		ret = freexl_get_cell_value (xl_handle, 0, col, &cell);
		if (ret != FREEXL_OK)
		    dummy = sqlite3_mprintf ("col_%d", col);
		else
		  {
		      if (cell.type == FREEXL_CELL_INT)
			  dummy = sqlite3_mprintf ("%d", cell.value.int_value);
		      else if (cell.type == FREEXL_CELL_DOUBLE)
			  dummy =
			      sqlite3_mprintf ("%1.2f",
					       cell.value.double_value);
		      else if (cell.type == FREEXL_CELL_TEXT
			       || cell.type == FREEXL_CELL_SST_TEXT
			       || cell.type == FREEXL_CELL_DATE
			       || cell.type == FREEXL_CELL_DATETIME
			       || cell.type == FREEXL_CELL_TIME)
			{
			    int len = strlen (cell.value.text_value);
			    if (len < 256)
				dummy =
				    sqlite3_mprintf ("%s",
						     cell.value.text_value);
			    else
				dummy = sqlite3_mprintf ("col_%d", col);
			}
		      else
			  dummy = sqlite3_mprintf ("col_%d", col);
		  }
		xdummy = gaiaDoubleQuotedSql (dummy);
		sqlite3_free (dummy);
		sql = sqlite3_mprintf (", \"%s\"", xdummy);
		free (xdummy);
		gaiaAppendToOutBuffer (&sql_statement, sql);
		sqlite3_free (sql);
	    }
	  else
	    {
		/* setting default column names  */
		dummy = sqlite3_mprintf ("col_%d", col);
		xdummy = gaiaDoubleQuotedSql (dummy);
		sqlite3_free (dummy);
		sql = sqlite3_mprintf (", \"%s\"", xdummy);
		free (xdummy);
		gaiaAppendToOutBuffer (&sql_statement, sql);
		sqlite3_free (sql);
	    }
      }
    gaiaAppendToOutBuffer (&sql_statement, ")\nVALUES (NULL");
    for (col = 0; col < columns; col++)
      {
	  /* column values */
	  gaiaAppendToOutBuffer (&sql_statement, ", ?");
      }
    gaiaAppendToOutBuffer (&sql_statement, ")");
    if (sql_statement.Error == 0 && sql_statement.Buffer != NULL)
      {
	  ret =
	      sqlite3_prepare_v2 (sqlite, sql_statement.Buffer,
				  strlen (sql_statement.Buffer), &stmt, NULL);
	  gaiaOutBufferReset (&sql_statement);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load XL error: %s\n", sqlite3_errmsg (sqlite));
		sqlError = 1;
		goto clean_up;
	    }
      }
    if (first_titles)
	current_row = 1;
    else
	current_row = 0;
    while (current_row < *rows)
      {
	  /* binding query params */
	  sqlite3_reset (stmt);
	  sqlite3_clear_bindings (stmt);
	  for (col = 0; col < columns; col++)
	    {
		/* column values */
		ret =
		    freexl_get_cell_value (xl_handle, current_row, col, &cell);
		if (ret != FREEXL_OK)
		    sqlite3_bind_null (stmt, col + 1);
		else
		  {
		      switch (cell.type)
			{
			case FREEXL_CELL_INT:
			    sqlite3_bind_int (stmt, col + 1,
					      cell.value.int_value);
			    break;
			case FREEXL_CELL_DOUBLE:
			    sqlite3_bind_double (stmt, col + 1,
						 cell.value.double_value);
			    break;
			case FREEXL_CELL_TEXT:
			case FREEXL_CELL_SST_TEXT:
			case FREEXL_CELL_DATE:
			case FREEXL_CELL_DATETIME:
			case FREEXL_CELL_TIME:
			    sqlite3_bind_text (stmt, col + 1,
					       cell.value.text_value,
					       strlen (cell.value.text_value),
					       SQLITE_STATIC);
			    break;
			default:
			    sqlite3_bind_null (stmt, col + 1);
			    break;
			};
		  }
	    }
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
	      ;
	  else
	    {
		spatialite_e ("load XL error: %s\n", sqlite3_errmsg (sqlite));
		sqlite3_finalize (stmt);
		sqlError = 1;
		goto clean_up;
	    }
	  current_row++;
      }
    sqlite3_finalize (stmt);
  clean_up:
    if (sqlError)
      {
	  /* some error occurred - ROLLBACK */
	  ret = sqlite3_exec (sqlite, "ROLLBACK", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("load XL error: %s\n", errMsg);
		sqlite3_free (errMsg);
	    }
	  spatialite_e
	      ("XL not loaded\n\n\na ROLLBACK was automatically performed\n");
      }
    else
      {
	  /* ok - confirming pending transaction - COMMIT */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, 0, &errMsg);
	  if (ret != SQLITE_OK)
	    {
		if (!err_msg)
		    spatialite_e ("load XL error: %s\n", errMsg);
		else
		    sprintf (err_msg, "load XL error: %s\n", errMsg);
		sqlite3_free (errMsg);
		return 0;
	    }
	  if (first_titles)
	      *rows = *rows - 1;	/* allow for header row */
	  spatialite_e ("XL loaded\n\n%d inserted rows\n", *rows);
      }
    freexl_close (xl_handle);
    return 1;

  error:
    freexl_close (xl_handle);
    if (!err_msg)
	spatialite_e ("XL datasource '%s' is not valid\n", path);
    else
	sprintf (err_msg, "XL datasource '%s' is not valid\n", path);
    *rows = 0;
    return 0;
}

#endif /* FreeXL enabled/disabled */
