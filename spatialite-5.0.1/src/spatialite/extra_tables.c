/*

 extra_tables.c -- Creating all SLD/SE and ISO Metadata extra tables

 version 5.0, 2020 August 1

 Author: Sandro Furieri a.furieri@lqt.it

 ------------------------------------------------------------------------------
 
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

Contributor(s):

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

/*
 
CREDITS:

this module has been partly funded by:
Regione Toscana - Settore Sistema Informativo Territoriale ed Ambientale
(implementing XML support - ISO Metadata and SLD/SE Styles) 

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32) && !defined(__MINGW32__)
#include "config-msvc.h"
#else
#include "config.h"
#endif

#include <spatialite/sqlite.h>
#include <spatialite/debug.h>

#include <spatialite.h>
#include <spatialite_private.h>
#include <spatialite/gaiaaux.h>
#include <spatialite/stored_procedures.h>

#ifdef _WIN32
#define strcasecmp	_stricmp
#endif /* not WIN32 */

static int
check_splite_metacatalog (sqlite3 * sqlite)
{
/* checks if "splite_metacatalog" really exists */
    int table_name = 0;
    int column_name = 0;
    int table_name2 = 0;
    int column_name2 = 0;
    int value = 0;
    int count = 0;
    char sql[1024];
    int ret;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
/* checking the "splite_metacatalog" table */
    strcpy (sql, "PRAGMA table_info(splite_metacatalog)");
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
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
		    table_name = 1;
		if (strcasecmp (name, "column_name") == 0)
		    column_name = 1;
	    }
      }
    sqlite3_free_table (results);
/* checking the "splite_metacatalog_statistics" table */
    strcpy (sql, "PRAGMA table_info(splite_metacatalog_statistics)");
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
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
		    table_name2 = 1;
		if (strcasecmp (name, "column_name") == 0)
		    column_name2 = 1;
		if (strcasecmp (name, "value") == 0)
		    value = 1;
		if (strcasecmp (name, "count") == 0)
		    count = 1;
	    }
      }
    sqlite3_free_table (results);
    if (table_name && column_name && table_name2 && column_name2 && value
	&& count)
	return 1;
    return 0;
}

static int
metacatalog_statistics (sqlite3 * sqlite, sqlite3_stmt * stmt_out,
			sqlite3_stmt * stmt_del, const char *table,
			const char *column)
{
/* auxiliary - updating "splite_metacatalog_statistics" */
    char *xtable;
    char *xcolumn;
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;

    xtable = gaiaDoubleQuotedSql (table);
    xcolumn = gaiaDoubleQuotedSql (column);
    sql_statement = sqlite3_mprintf ("SELECT \"%s\", Count(*) FROM \"%s\" "
				     "GROUP BY \"%s\"", xcolumn, xtable,
				     xcolumn);
    free (xcolumn);
    free (xtable);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("Update MetaCatalog Statistics(4) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

/* deleting all existing rows */
    sqlite3_reset (stmt_del);
    sqlite3_clear_bindings (stmt_del);
    sqlite3_bind_text (stmt_del, 1, table, strlen (table), SQLITE_STATIC);
    sqlite3_bind_text (stmt_del, 2, column, strlen (column), SQLITE_STATIC);
    ret = sqlite3_step (stmt_del);
    if (ret == SQLITE_DONE || ret == SQLITE_ROW)
	;
    else
      {
	  spatialite_e ("populate MetaCatalog Statistics(5) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  sqlite3_finalize (stmt_in);
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		sqlite3_reset (stmt_out);
		sqlite3_clear_bindings (stmt_out);
		sqlite3_bind_text (stmt_out, 1, table, strlen (table),
				   SQLITE_STATIC);
		sqlite3_bind_text (stmt_out, 2, column, strlen (column),
				   SQLITE_STATIC);
		switch (sqlite3_column_type (stmt_in, 0))
		  {
		  case SQLITE_INTEGER:
		      sqlite3_bind_int64 (stmt_out, 3,
					  sqlite3_column_int (stmt_in, 0));
		      break;
		  case SQLITE_FLOAT:
		      sqlite3_bind_double (stmt_out, 3,
					   sqlite3_column_double (stmt_in, 0));
		      break;
		  case SQLITE_TEXT:
		      sqlite3_bind_text (stmt_out, 3,
					 (const char *)
					 sqlite3_column_text (stmt_in, 0),
					 sqlite3_column_bytes (stmt_in, 0),
					 SQLITE_STATIC);
		      break;
		  case SQLITE_BLOB:
		      sqlite3_bind_blob (stmt_out, 3,
					 sqlite3_column_blob (stmt_in, 0),
					 sqlite3_column_bytes (stmt_in, 0),
					 SQLITE_STATIC);
		      break;
		  default:
		      sqlite3_bind_null (stmt_out, 3);
		      break;
		  };
		sqlite3_bind_int (stmt_out, 4, sqlite3_column_int (stmt_in, 1));
		ret = sqlite3_step (stmt_out);
		if (ret == SQLITE_DONE || ret == SQLITE_ROW)
		    ;
		else
		  {
		      spatialite_e
			  ("populate MetaCatalog Statistics(6) error: \"%s\"\n",
			   sqlite3_errmsg (sqlite));
		      sqlite3_finalize (stmt_in);
		      return 0;
		  }
	    }
      }
    sqlite3_finalize (stmt_in);

    return 1;
}

SPATIALITE_DECLARE int
gaiaUpdateMetaCatalogStatistics (sqlite3 * sqlite, const char *table,
				 const char *column)
{
/* Updates the "splite_metacalog_statistics" table */
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;
    sqlite3_stmt *stmt_out;
    sqlite3_stmt *stmt_del;

    if (!check_splite_metacatalog (sqlite))
      {
	  spatialite_e
	      ("invalid or not existing \"splite_metacatalog_statistics\" table\n");
	  return 0;
      }

/* updating the MetaCatalog statistics */
    sql_statement = sqlite3_mprintf ("SELECT table_name, column_name "
				     "FROM splite_metacatalog WHERE "
				     "Lower(table_name) = Lower(%Q) "
				     "AND Lower(column_name) = Lower(%Q)",
				     table, column);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("Update MetaCatalog Statistics(1) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    sql_statement = "INSERT INTO splite_metacatalog_statistics "
	"(table_name, column_name, value, count) " "VALUES (?, ?, ?, ?)";
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_out, NULL);
    if (ret != SQLITE_OK)
      {
	  sqlite3_finalize (stmt_in);
	  spatialite_e ("Update MetaCatalog Statistics(2) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    sql_statement = "DELETE FROM splite_metacatalog_statistics "
	"WHERE Lower(table_name) = Lower(?) AND Lower(column_name) = Lower(?)";
    ret =
	sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			    &stmt_del, NULL);
    if (ret != SQLITE_OK)
      {
	  sqlite3_finalize (stmt_in);
	  sqlite3_finalize (stmt_out);
	  spatialite_e ("Update MetaCatalog Statistics(3) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *table =
		    (const char *) sqlite3_column_text (stmt_in, 0);
		const char *column =
		    (const char *) sqlite3_column_text (stmt_in, 1);
		if (!metacatalog_statistics
		    (sqlite, stmt_out, stmt_del, table, column))
		  {
		      sqlite3_finalize (stmt_in);
		      sqlite3_finalize (stmt_out);
		      sqlite3_finalize (stmt_del);
		      return 0;
		  }
	    }
      }
    sqlite3_finalize (stmt_in);
    sqlite3_finalize (stmt_out);
    sqlite3_finalize (stmt_del);
    return 1;
}

static int
check_master_table (sqlite3 * sqlite, const char *master_table,
		    const char *table, const char *column)
{
/* checks if the Master Table could be accessed */
    int table_name = 0;
    int column_name = 0;
    char *sql;
    int ret;
    char *xmaster;
    const char *name;
    int i;
    char **results;
    int rows;
    int columns;
/* checking the Master table */
    xmaster = gaiaDoubleQuotedSql (master_table);
    sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xmaster);
    free (xmaster);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
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
		if (strcasecmp (name, table) == 0)
		    table_name = 1;
		if (strcasecmp (name, column) == 0)
		    column_name = 1;
	    }
      }
    sqlite3_free_table (results);
    if (table_name && column_name)
	return 1;
    return 0;
}

SPATIALITE_DECLARE int
gaiaUpdateMetaCatalogStatisticsFromMaster (sqlite3 * sqlite,
					   const char *master_table,
					   const char *table_name,
					   const char *column_name)
{
/* Updates the "splite_metacalog_statistics" table (using a Master Table) */
    int ret;
    char *sql_statement;
    sqlite3_stmt *stmt;
    char *xmaster;
    char *xtable;
    char *xcolumn;
    if (!check_master_table (sqlite, master_table, table_name, column_name))
      {
	  spatialite_e
	      ("UpdateMetaCatalogStatisticsFromMaster: mismatching or not existing Master Table\n");
	  return 0;
      }

/* scanning the Master Table */
    xmaster = gaiaDoubleQuotedSql (master_table);
    xtable = gaiaDoubleQuotedSql (table_name);
    xcolumn = gaiaDoubleQuotedSql (column_name);
    sql_statement =
	sqlite3_mprintf ("SELECT \"%s\", \"%s\" FROM \"%s\"", xtable, xcolumn,
			 xmaster);
    free (xmaster);
    free (xtable);
    free (xcolumn);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("UpdateMetaCatalogStatisticsFromMaster(1) error: \"%s\"\n",
	       sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *table =
		    (const char *) sqlite3_column_text (stmt, 0);
		const char *column =
		    (const char *) sqlite3_column_text (stmt, 1);
		if (!gaiaUpdateMetaCatalogStatistics (sqlite, table, column))
		  {
		      sqlite3_finalize (stmt);
		      return 0;
		  }
	    }
      }
    sqlite3_finalize (stmt);
    return 1;
}

static int
check_unique_index (sqlite3 * sqlite, const char *index, const char *column)
{
/* checks if a column has any Unique constraint - pass two */
    char *xindex;
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;
    int is_unique = 0;
    int index_parts = 0;

    xindex = gaiaDoubleQuotedSql (index);
    sql_statement = sqlite3_mprintf ("PRAGMA index_info(\"%s\")", xindex);
    free (xindex);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("populate MetaCatalog(8) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *colname =
		    (const char *) sqlite3_column_text (stmt_in, 2);
		if (strcasecmp (colname, column) == 0)
		    is_unique = 1;
		index_parts++;
	    }
      }
    sqlite3_finalize (stmt_in);

    if (index_parts > 1)
      {
	  /* ignoring any multi-column index */
	  is_unique = 0;
      }
    return is_unique;
}

static int
check_unique (sqlite3 * sqlite, const char *table, const char *column)
{
/* checks if a column has any Unique constraint */
    char *xtable;
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;
    int is_unique = 0;

    xtable = gaiaDoubleQuotedSql (table);
    sql_statement = sqlite3_mprintf ("PRAGMA index_list(\"%s\")", xtable);
    free (xtable);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("populate MetaCatalog(7) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *idxname =
		    (const char *) sqlite3_column_text (stmt_in, 1);
		if (sqlite3_column_int (stmt_in, 2) == 1)
		  {
		      /* Unique Index */
		      if (check_unique_index (sqlite, idxname, column))
			  is_unique = 1;
		  }
	    }
      }
    sqlite3_finalize (stmt_in);

    return is_unique;
}

static int
check_foreign_key (sqlite3 * sqlite, const char *table, const char *column)
{
/* checks if a column is part of any Foreign Key */
    char *xtable;
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;
    int is_foreign_key = 0;

    xtable = gaiaDoubleQuotedSql (table);
    sql_statement = sqlite3_mprintf ("PRAGMA foreign_key_list(\"%s\")", xtable);
    free (xtable);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("populate MetaCatalog(6) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *colname =
		    (const char *) sqlite3_column_text (stmt_in, 3);
		if (strcasecmp (colname, column) == 0)
		    is_foreign_key = 1;
	    }
      }
    sqlite3_finalize (stmt_in);

    return is_foreign_key;
}

static int
table_info (sqlite3 * sqlite, sqlite3_stmt * stmt_out, const char *table)
{
/* auxiliary - populating "splite_metacatalog" */
    char *xtable;
    char *sql_statement;
    int ret;
    sqlite3_stmt *stmt_in;

    xtable = gaiaDoubleQuotedSql (table);
    sql_statement = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", xtable);
    free (xtable);
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("populate MetaCatalog(3) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		int is_foreign_key;
		int is_unique;
		sqlite3_reset (stmt_out);
		sqlite3_clear_bindings (stmt_out);
		sqlite3_bind_text (stmt_out, 1, table, strlen (table),
				   SQLITE_STATIC);
		sqlite3_bind_text (stmt_out, 2,
				   (const char *) sqlite3_column_text (stmt_in,
								       1),
				   sqlite3_column_bytes (stmt_in, 1),
				   SQLITE_STATIC);
		sqlite3_bind_text (stmt_out, 3,
				   (const char *) sqlite3_column_text (stmt_in,
								       2),
				   sqlite3_column_bytes (stmt_in, 2),
				   SQLITE_STATIC);
		sqlite3_bind_int (stmt_out, 4, sqlite3_column_int (stmt_in, 3));
		sqlite3_bind_int (stmt_out, 5, sqlite3_column_int (stmt_in, 5));
		is_foreign_key =
		    check_foreign_key (sqlite, table,
				       (const char *)
				       sqlite3_column_text (stmt_in, 1));
		sqlite3_bind_int (stmt_out, 6, is_foreign_key);
		is_unique =
		    check_unique (sqlite, table,
				  (const char *) sqlite3_column_text (stmt_in,
								      1));
		sqlite3_bind_int (stmt_out, 7, is_unique);
		ret = sqlite3_step (stmt_out);
		if (ret == SQLITE_DONE || ret == SQLITE_ROW)
		    ;
		else
		  {
		      spatialite_e ("populate MetaCatalog(4) error: \"%s\"\n",
				    sqlite3_errmsg (sqlite));
		      sqlite3_finalize (stmt_in);
		      return 0;
		  }
	    }
      }
    sqlite3_finalize (stmt_in);

    return 1;
}

SPATIALITE_DECLARE int
gaiaCreateMetaCatalogTables (sqlite3 * sqlite)
{
/* Creates both "splite_metacatalog" and "splite_metacalog_statistics" tables */
    char *sql_statement;
    char *err_msg = NULL;
    int ret;
    sqlite3_stmt *stmt_in;
    sqlite3_stmt *stmt_out;

/* creating "splite_metacatalog" */
    sql_statement = "CREATE TABLE splite_metacatalog (\n"
	"table_name TEXT NOT NULL,\n"
	"column_name TEXT NOT NULL,\n"
	"type TEXT NOT NULL,\n"
	"not_null INTEGER NOT NULL,\n"
	"primary_key INTEGER NOT NULL,\n"
	"foreign_key INTEGER NOT NULL,\n"
	"unique_value INTEGER NOT NULL,\n"
	"CONSTRAINT pk_splite_metacatalog PRIMARY KEY (table_name, column_name))";
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE TABLE splite_metacatalog - error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating "splite_metacatalog_statistics" */
    sql_statement = "CREATE TABLE splite_metacatalog_statistics (\n"
	"table_name TEXT NOT NULL,\n"
	"column_name TEXT NOT NULL,\n"
	"value TEXT,\n"
	"count INTEGER NOT NULL,\n"
	"CONSTRAINT pk_splite_metacatalog_statistics PRIMARY KEY (table_name, column_name, value),\n"
	"CONSTRAINT fk_splite_metacatalog_statistics FOREIGN KEY (table_name, column_name) "
	"REFERENCES splite_metacatalog (table_name, column_name))";
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE TABLE splite_metacatalog_statistics - error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* populating the MetaCatalog table */
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND sql NOT LIKE 'CREATE VIRTUAL TABLE%'";
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_in, NULL);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("populate MetaCatalog(1) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    sql_statement = "INSERT INTO splite_metacatalog "
	"(table_name, column_name, type, not_null, primary_key, foreign_key, unique_value) "
	"VALUES (?, ?, ?, ?, ?, ?, ?)";
    ret = sqlite3_prepare_v2 (sqlite, sql_statement, strlen (sql_statement),
			      &stmt_out, NULL);
    if (ret != SQLITE_OK)
      {
	  sqlite3_finalize (stmt_in);
	  spatialite_e ("populate MetaCatalog(2) error: \"%s\"\n",
			sqlite3_errmsg (sqlite));
	  return 0;
      }

    while (1)
      {
	  /* scrolling the result set rows */
	  ret = sqlite3_step (stmt_in);
	  if (ret == SQLITE_DONE)
	      break;		/* end of result set */
	  if (ret == SQLITE_ROW)
	    {
		const char *table =
		    (const char *) sqlite3_column_text (stmt_in, 0);
		if (!table_info (sqlite, stmt_out, table))
		  {
		      sqlite3_finalize (stmt_in);
		      sqlite3_finalize (stmt_out);
		      return 0;
		  }
	    }
      }
    sqlite3_finalize (stmt_in);
    sqlite3_finalize (stmt_out);
    return 1;
}

static int
check_raster_coverages (sqlite3 * sqlite)
{
/* checking if the "raster_coverages" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('raster_coverages')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_raster_coverages_srid (sqlite3 * sqlite)
{
/* checking if the "raster_coverages_srid" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('raster_coverages_srid')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_raster_coverages_ref_sys (sqlite3 * sqlite)
{
/* checking if the "raster_coverages_ref_sys" view already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'view' "
	"AND Upper(name) = Upper('raster_coverages_ref_sys')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_raster_coverages_keyword (sqlite3 * sqlite)
{
/* checking if the "raster_coverages_keyword" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('raster_coverages_keyword')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static void
drop_raster_coverages_triggers (sqlite3 * sqlite)
{
/* dropping all "raster_coverages" triggers */
    char *sql;
    int ret;
    char *err_msg = NULL;
    char **results;
    int rows;
    int columns;
    int i;

/* checking for existing tables */
    sql =
	"SELECT name FROM sqlite_master WHERE type = 'trigger' AND tbl_name "
	"IN ('raster_coverages', 'raster_coverages_srid', 'raster_coverages_keyword')";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  sql = sqlite3_mprintf ("DROP TRIGGER %s", name);
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return;
	    }
	  sqlite3_free (sql);
      }
    sqlite3_free_table (results);
}

static int
create_raster_coverages_triggers (sqlite3 * sqlite)
{
/* creating the "raster_coverages" triggers */
    char *sql;
    int ret;
    char *err_msg = NULL;
    char **results;
    int rows;
    int columns;
    int i;
    int ok_raster_coverages = 0;
    int ok_raster_coverages_srid = 0;
    int ok_raster_coverages_keyword = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name "
	"IN ('raster_coverages', 'raster_coverages_srid', 'raster_coverages_keyword')";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "raster_coverages") == 0)
	      ok_raster_coverages = 1;
	  if (strcasecmp (name, "raster_coverages_srid") == 0)
	      ok_raster_coverages_srid = 1;
	  if (strcasecmp (name, "raster_coverages_keyword") == 0)
	      ok_raster_coverages_keyword = 1;
      }
    sqlite3_free_table (results);

    if (ok_raster_coverages)
      {
	  /* creating the raster_coverages triggers */
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_name_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_sample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "sample_type must be one of ''1-BIT'' | ''2-BIT'' | ''4-BIT'' | "
	      "''INT8'' | ''UINT8'' | ''INT16'' | ''UINT16'' | ''INT32'' | "
	      "''UINT32'' | ''FLOAT'' | ''DOUBLE''')\n"
	      "WHERE NEW.sample_type NOT IN ('1-BIT', '2-BIT', '4-BIT', "
	      "'INT8', 'UINT8', 'INT16', 'UINT16', 'INT32', "
	      "'UINT32', 'FLOAT', 'DOUBLE');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_sample_update\n"
	      "BEFORE UPDATE OF 'sample_type' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "sample_type must be one of ''1-BIT'' | ''2-BIT'' | ''4-BIT'' | "
	      "''INT8'' | ''UINT8'' | ''INT16'' | ''UINT16'' | ''INT32'' | "
	      "''UINT32'' | ''FLOAT'' | ''DOUBLE''')\n"
	      "WHERE NEW.sample_type NOT IN ('1-BIT', '2-BIT', '4-BIT', "
	      "'INT8', 'UINT8', 'INT16', 'UINT16', 'INT32', "
	      "'UINT32', 'FLOAT', 'DOUBLE');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_pixel_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "pixel_type must be one of ''MONOCHROME'' | ''PALETTE'' | "
	      "''GRAYSCALE'' | ''RGB'' | ''MULTIBAND'' | ''DATAGRID''')\n"
	      "WHERE NEW.pixel_type NOT IN ('MONOCHROME', 'PALETTE', "
	      "'GRAYSCALE', 'RGB', 'MULTIBAND', 'DATAGRID');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_pixel_update\n"
	      "BEFORE UPDATE OF 'pixel_type' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "pixel_type must be one of ''MONOCHROME'' | ''PALETTE'' | "
	      "''GRAYSCALE'' | ''RGB'' | ''MULTIBAND'' | ''DATAGRID''')\n"
	      "WHERE NEW.pixel_type NOT IN ('MONOCHROME', 'PALETTE', "
	      "'GRAYSCALE', 'RGB', 'MULTIBAND', 'DATAGRID');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_bands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "num_bands must be >= 1')\nWHERE NEW.num_bands < 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_bands_update\n"
	      "BEFORE UPDATE OF 'num_bands' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "num_bands must be >= 1')\nWHERE NEW.num_bands < 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_compression_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "compression must be one of ''NONE'' | ''DEFLATE'' | ''DEFLATE_NO'' | "
	      "''LZMA'' | ''LZMA_NO'' | ''LZ4'' | ''LZ4_NO'' | ''ZSTD'' | ''ZSTD_NO'' | "
	      "''PNG'' | ''JPEG'' | ''LOSSY_WEBP'' | ''LOSSLESS_WEBP'' | ''CCITTFAX4'' | "
	      "''LOSSY_JP2'' | ''LOSSLESS_JP2''')\n"
	      "WHERE NEW.compression NOT IN ('NONE', 'DEFLATE',  'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', 'CCITTFAX4', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_compression_update\n"
	      "BEFORE UPDATE OF 'compression' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "compression must be one of ''NONE'' | ''DEFLATE'' | ''DEFLATE_NO'' | "
	      "''LZMA'' | ''LZMA_NO'' | ''LZ4'' | ''LZ4_NO'' | ''ZSTD'' | ''ZSTD_NO'' | "
	      "''PNG'' | ''JPEG'' | ''LOSSY_WEBP'' | ''LOSSLESS_WEBP'' | ''CCITTFAX4'' | "
	      "''LOSSY_JP2'' | ''LOSSLESS_JP2''')\n"
	      "WHERE NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', 'CCITTFAX4', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_quality_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "quality must be between 0 and 100')\n"
	      "WHERE NEW.quality NOT BETWEEN 0 AND 100;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_quality_update\n"
	      "BEFORE UPDATE OF 'quality' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "quality must be between 0 and 100')\n"
	      "WHERE NEW.quality NOT BETWEEN 0 AND 100;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_tilew_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "tile_width must be an exact multiple of 8 between 256 and 1024')\n"
	      "WHERE CastToInteger(NEW.tile_width) IS NULL OR "
	      "NEW.tile_width NOT BETWEEN 256 AND 1024 OR (NEW.tile_width % 8) <> 0;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_tilew_update\n"
	      "BEFORE UPDATE OF 'tile_width' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "tile_width must be an exact multiple of 8 between 256 and 1024')\n"
	      "WHERE CastToInteger(NEW.tile_width) IS NULL OR "
	      "NEW.tile_width NOT BETWEEN 256 AND 1024 OR (NEW.tile_width % 8) <> 0;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_tileh_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "tile_height must be an exact multiple of 8 between 256 and 1024')\n"
	      "WHERE CastToInteger(NEW.tile_height) IS NULL OR "
	      "NEW.tile_height NOT BETWEEN 256 AND 1024 OR (NEW.tile_height % 8) <> 0;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_tileh_update\n"
	      "BEFORE UPDATE OF 'tile_height' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "tile_height must be an exact multiple of 8 between 256 and 1024')\n"
	      "WHERE CastToInteger(NEW.tile_height) IS NULL OR "
	      "NEW.tile_height NOT BETWEEN 256 AND 1024 OR (NEW.tile_height % 8) <> 0;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_horzres_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "horz_resolution must be positive')\n"
	      "WHERE NEW.horz_resolution IS NOT NULL AND "
	      "(NEW.horz_resolution <= 0.0 OR CastToDouble(NEW.horz_resolution) IS NULL);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_horzres_update\n"
	      "BEFORE UPDATE OF 'horz_resolution' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "horz_resolution must be positive')\n"
	      "WHERE NEW.horz_resolution IS NOT NULL AND "
	      "(NEW.horz_resolution <= 0.0 OR CastToDouble(NEW.horz_resolution) IS NULL);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_vertres_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "vert_resolution must be positive')\n"
	      "WHERE NEW.vert_resolution IS NOT NULL AND "
	      "(NEW.vert_resolution <= 0.0 OR CastToDouble(NEW.vert_resolution) IS NULL);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_vertres_update\n"
	      "BEFORE UPDATE OF 'vert_resolution' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "vert_resolution must be positive')\n"
	      "WHERE NEW.vert_resolution IS NOT NULL AND "
	      "(NEW.vert_resolution <= 0.0 OR CastToDouble(NEW.vert_resolution) IS NULL);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_nodata_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "invalid nodata_pixel')\nWHERE NEW.nodata_pixel IS NOT NULL AND "
	      "IsValidPixel(NEW.nodata_pixel, NEW.sample_type, NEW.num_bands) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_nodata_update\n"
	      "BEFORE UPDATE OF 'nodata_pixel' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "invalid nodata_pixel')\nWHERE NEW.nodata_pixel IS NOT NULL AND "
	      "IsValidPixel(NEW.nodata_pixel, NEW.sample_type, NEW.num_bands) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_palette_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "invalid palette')\nWHERE NEW.palette IS NOT NULL AND "
	      "(NEW.pixel_type <> 'PALETTE' OR NEW.num_bands <> 1 OR "
	      "IsValidRasterPalette(NEW.palette, NEW.sample_type) <> 1);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_palette_update\n"
	      "BEFORE UPDATE OF 'palette' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "invalid palette')\nWHERE NEW.palette IS NOT NULL AND "
	      "(NEW.pixel_type <> 'PALETTE' OR NEW.num_bands <> 1 OR "
	      "IsValidRasterPalette(NEW.palette, NEW.sample_type) <> 1);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_statistics_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "invalid statistics')\nWHERE NEW.statistics IS NOT NULL AND "
	      "IsValidRasterStatistics(NEW.statistics, NEW.sample_type, NEW.num_bands) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_statistics_update\n"
	      "BEFORE UPDATE OF 'statistics' ON 'raster_coverages'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "invalid statistics')\nWHERE NEW.statistics IS NOT NULL AND "
	      "IsValidRasterStatistics(NEW.statistics, NEW.sample_type, NEW.num_bands) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monosample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME sample_type')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.sample_type <> '1-BIT';\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monosample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME sample_type')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.sample_type <>'1-BIT';\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monocompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME compression')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'CCITTFAX4');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monocompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME compression')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', 'CCITTFAX4');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monobands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME num_bands')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_monobands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MONOCHROME num_bands')\nWHERE NEW.pixel_type = 'MONOCHROME' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltsample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent PALETTE sample_type')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.sample_type NOT IN ('1-BIT', '2-BIT', '4-BIT', 'UINT8');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltsample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent PALETTE sample_type')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.sample_type NOT IN ('1-BIT', '2-BIT', '4-BIT', 'UINT8');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltcompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent PALETTE compression')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltcompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent PALETTE compression')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltbands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent PALETTE num_bands')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_pltbands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent PALETTE num_bands')\nWHERE NEW.pixel_type = 'PALETTE' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graysample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE sample_type')\nWHERE NEW.pixel_type = 'GRAYSCALE' "
	      "AND NEW.sample_type NOT IN ('2-BIT', '4-BIT', 'UINT8');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graysample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE sample_type')\nWHERE NEW.pixel_type = 'GRAYSCALE' "
	      "AND NEW.sample_type NOT IN ('2-BIT', '4-BIT', 'UINT8');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graybands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE num_bands')\nWHERE NEW.pixel_type = 'GRAYSCALE' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graybands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE num_bands')\nWHERE NEW.pixel_type = 'GRAYSCALE' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graycompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE compression')\nWHERE NEW.pixel_type = "
	      "'GRAYSCALE' AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', 'LOSSY_JP2', "
	      "'LOSSLESS_JP2');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_graycompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent GRAYSCALE compression')\nWHERE NEW.pixel_type = "
	      "'GRAYSCALE' AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', 'LOSSY_JP2', "
	      "'LOSSLESS_JP2');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbsample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent RGB sample_type')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND NEW.sample_type NOT IN ('UINT8', 'UINT16');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbsample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent RGB sample_type')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND NEW.sample_type NOT IN ('UINT8', 'UINT16');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbcompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent RGB compression')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND ((NEW.sample_type = 'UINT8' AND NEW.compression NOT IN ("
	      "'NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', "
	      "'ZSTD', 'ZSTD_NO', 'PNG', 'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2') OR (NEW.sample_type = "
	      "'UINT16' AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2'))));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbcompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent RGB compression')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND ((NEW.sample_type = 'UINT8' AND NEW.compression NOT IN ("
	      "'NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', "
	      "'ZSTD', 'ZSTD_NO', 'PNG', 'JPEG', 'LOSSY_WEBP', 'LOSSLESS_WEBP', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2') OR (NEW.sample_type = "
	      "'UINT16' AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', "
	      "'LZMA', 'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2'))));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbbands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent RGB num_bands')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND NEW.num_bands <> 3;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_rgbbands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent RGB num_bands')\nWHERE NEW.pixel_type = 'RGB' "
	      "AND NEW.num_bands <> 3;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multisample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND sample_type')\nWHERE NEW.pixel_type = 'MULTIBAND' "
	      "AND NEW.sample_type NOT IN ('UINT8', 'UINT16');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multisample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND sample_type')\nWHERE NEW.pixel_type = 'MULTIBAND' "
	      "AND NEW.sample_type NOT IN ('UINT8', 'UINT16');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multicompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND compression')\nWHERE NEW.pixel_type = "
	      "'MULTIBAND' AND ((NEW.num_bands NOT IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO')) OR	"
	      "(NEW.sample_type <> 'UINT16' AND NEW.num_bands IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_WEBP', 'LOSSLESS_WEBP', 'LOSSY_JP2', 'LOSSLESS_JP2')) OR "
	      "(NEW.sample_type = 'UINT16' AND NEW.num_bands IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2')));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multicompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND compression')\nWHERE NEW.pixel_type = "
	      "'MULTIBAND' AND ((NEW.num_bands NOT IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO')) OR	"
	      "(NEW.sample_type <> 'UINT16' AND NEW.num_bands IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_WEBP', 'LOSSLESS_WEBP', 'LOSSY_JP2', 'LOSSLESS_JP2')) OR "
	      "(NEW.sample_type = 'UINT16' AND NEW.num_bands IN (3, 4) AND "
	      "NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2')));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multibands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND num_bands')\nWHERE NEW.pixel_type = 'MULTIBAND' "
	      "AND NEW.num_bands < 2;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_multibands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent MULTIBAND num_bands')\nWHERE NEW.pixel_type = 'MULTIBAND' "
	      "AND NEW.num_bands < 2;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridsample_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent DATAGRID sample_type')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND NEW.sample_type NOT IN ('INT8', 'UINT8', 'INT16', 'UINT16', "
	      "'INT32', 'UINT32', 'FLOAT', 'DOUBLE');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridsample_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent DATAGRID sample_type')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND NEW.sample_type NOT IN ('INT8', 'UINT8', 'INT16', 'UINT16', "
	      "'INT32', 'UINT32', 'FLOAT', 'DOUBLE');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridcompr_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent DATAGRID compression')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND (((NEW.sample_type NOT IN ('UINT8', 'UINT16')) AND NEW.compression "
	      "NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', 'LZMA_NO', 'LZ4', "
	      "'LZ4_NO', 'ZSTD', 'ZSTD_NO')) OR ((NEW.sample_type IN ('UINT8', 'UINT16')) "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2')));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridcompr_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent DATAGRID compression')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND (((NEW.sample_type NOT IN ('UINT8', 'UINT16')) AND NEW.compression "
	      "NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', 'LZMA_NO', 'LZ4', "
	      "'LZ4_NO', 'ZSTD', 'ZSTD_NO')) OR ((NEW.sample_type IN ('UINT8', 'UINT16')) "
	      "AND NEW.compression NOT IN ('NONE', 'DEFLATE', 'DEFLATE_NO', 'LZMA', "
	      "'LZMA_NO', 'LZ4', 'LZ4_NO', 'ZSTD', 'ZSTD_NO', 'PNG', "
	      "'LOSSY_JP2', 'LOSSLESS_JP2')));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridbands_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent DATAGRID num_bands')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_gridbands_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "inconsistent DATAGRID num_bands')\nWHERE NEW.pixel_type = 'DATAGRID' "
	      "AND NEW.num_bands <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_georef_insert\n"
	      "BEFORE INSERT ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages violates constraint: "
	      "inconsistent georeferencing infos')\n"
	      "WHERE NOT ((NEW.horz_resolution IS NULL AND NEW.vert_resolution IS NULL "
	      "AND NEW.srid IS NULL) OR (NEW.horz_resolution IS NOT NULL "
	      "AND NEW.vert_resolution IS NOT NULL AND NEW.srid IS NOT NULL));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_georef_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages violates constraint: "
	      "inconsistent georeferencing infos')\n"
	      "WHERE NOT ((NEW.horz_resolution IS NULL AND NEW.vert_resolution IS NULL "
	      "AND NEW.srid IS NULL) OR (NEW.horz_resolution IS NOT NULL "
	      "AND NEW.vert_resolution IS NOT NULL AND NEW.srid IS NOT NULL));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_update\n"
	      "BEFORE UPDATE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on raster_coverages violates constraint: "
	      "attempting to change the definition of an already populated Coverage')\n"
	      "WHERE IsPopulatedCoverage(NULL, OLD.coverage_name) = 1 AND "
	      "((OLD.sample_type <> NEW.sample_type) AND (OLD.pixel_type <> NEW.sample_type) "
	      "OR (OLD.num_bands <> NEW.num_bands) OR (OLD.compression <> NEW.compression) "
	      "OR (OLD.quality <> NEW.quality) OR (OLD.tile_width <> NEW.tile_width) "
	      "OR (OLD.tile_height <> NEW.tile_height) OR (OLD.horz_resolution <> NEW.horz_resolution) "
	      "OR (OLD.vert_resolution <> NEW.vert_resolution) OR "
	      "(OLD.srid <> NEW.srid) OR (OLD.nodata_pixel <> NEW.nodata_pixel));\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS raster_coverages_delete\n"
	      "BEFORE DELETE ON 'raster_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'delete on raster_coverages violates constraint: "
	      "attempting to delete the definition of an already populated Coverage')\n"
	      "WHERE IsPopulatedCoverage(NULL, OLD.coverage_name) = 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }

    if (ok_raster_coverages_srid)
      {
	  /* creating the raster_coverages_srid triggers */
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_srid_name_insert\n"
	      "BEFORE INSERT ON 'raster_coverages_srid'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_srid violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_srid violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_srid violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_srid_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'raster_coverages_srid'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_srid violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_srid violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_srid violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }

    if (ok_raster_coverages_keyword)
      {
	  /* creating the raster_coverages_keyword triggers */
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_keyword_name_insert\n"
	      "BEFORE INSERT ON 'raster_coverages_keyword'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on raster_coverages_keyword violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS raster_coverages_keyword_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'raster_coverages_keyword'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on raster_coverages_keyword violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_raster_coverages (sqlite3 * sqlite)
{
/* creating the "raster_coverages" table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE IF NOT EXISTS raster_coverages (\n"
	"coverage_name TEXT NOT NULL PRIMARY KEY,\n"
	"title TEXT NOT NULL DEFAULT '*** missing Title ***',\n"
	"abstract TEXT NOT NULL DEFAULT '*** missing Abstract ***',\n"
	"sample_type TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"pixel_type TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"num_bands INTEGER NOT NULL DEFAULT 1,\n"
	"compression TEXT NOT NULL DEFAULT 'NONE',\n"
	"quality INTEGER NOT NULL DEFAULT 100,\n"
	"tile_width INTEGER NOT NULL DEFAULT 512,\n"
	"tile_height INTEGER NOT NULL DEFAULT 512,\n"
	"horz_resolution DOUBLE NOT NULL,\n"
	"vert_resolution DOUBLE NOT NULL,\n"
	"srid INTEGER NOT NULL,\n"
	"nodata_pixel BLOB NOT NULL,\n"
	"palette BLOB,\n"
	"statistics BLOB,\n"
	"geo_minx DOUBLE,\n"
	"geo_miny DOUBLE,\n"
	"geo_maxx DOUBLE,\n"
	"geo_maxy DOUBLE,\n"
	"extent_minx DOUBLE,\n"
	"extent_miny DOUBLE,\n"
	"extent_maxx DOUBLE,\n"
	"extent_maxy DOUBLE,\n"
	"strict_resolution INTEGER NOT NULL,\n"
	"mixed_resolutions INTEGER NOT NULL,\n"
	"section_paths INTEGER NOT NULL,\n"
	"section_md5 INTEGER NOT NULL,\n"
	"section_summary INTEGER NOT NULL,\n"
	"is_queryable INTEGER NOT NULL,\n"
	"red_band_index INTEGER,\n"
	"green_band_index INTEGER,\n"
	"blue_band_index INTEGER,\n"
	"nir_band_index INTEGER,\n"
	"enable_auto_ndvi INTEGER,\n"
	"copyright TEXT NOT NULL DEFAULT '*** unknown ***',\n"
	"license INTEGER NOT NULL DEFAULT 0,\n"
	"CONSTRAINT fk_rc_srs FOREIGN KEY (srid) "
	"REFERENCES spatial_ref_sys (srid),\n"
	"CONSTRAINT fk_rc_lic FOREIGN KEY (license) "
	"REFERENCES data_licenses (id))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'raster_coverages' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the raster_coverages_srid table */
    sql = "CREATE TABLE IF NOT EXISTS raster_coverages_srid (\n"
	"coverage_name TEXT NOT NULL,\n"
	"srid INTEGER NOT NULL,\n"
	"extent_minx DOUBLE,\n"
	"extent_miny DOUBLE,\n"
	"extent_maxx DOUBLE,\n"
	"extent_maxy DOUBLE,\n"
	"CONSTRAINT pk_raster_coverages_srid PRIMARY KEY (coverage_name, srid),\n"
	"CONSTRAINT fk_raster_coverages_srid FOREIGN KEY (coverage_name) "
	"REFERENCES raster_coverages (coverage_name) ON DELETE CASCADE,\n"
	"CONSTRAINT fk_raster_srid FOREIGN KEY (srid) "
	"REFERENCES spatial_ref_sys (srid))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'raster_coverages_srid' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the raster_coverages_ref_sys view */
    sql = "CREATE VIEW IF NOT EXISTS raster_coverages_ref_sys AS\n"
	"SELECT c.coverage_name AS coverage_name, c.title AS title, "
	"c.abstract AS abstract, c.sample_type AS sample_type, "
	"c.pixel_type AS pixel_type, c.num_bands AS num_bands, "
	"c.compression AS compression, c.quality AS quality, "
	"c.tile_width AS tile_width, c.tile_height AS tile_height, "
	"c.horz_resolution AS horz_resolution, c.vert_resolution AS vert_resolution, "
	"c.nodata_pixel AS nodata_pixel, c.palette AS palette, "
	"c.statistics AS statistics, c.geo_minx AS geo_minx, "
	"c.geo_miny AS geo_miny, c.geo_maxx AS geo_maxx, "
	"c.geo_maxy AS geo_maxy, c.extent_minx AS extent_minx, "
	"c.extent_miny AS extent_miny, c.extent_maxx AS extent_maxx, "
	"c.extent_maxy AS extent_maxy, c.srid AS srid, 1 AS native_srid, "
	"s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text, "
	"c.strict_resolution AS strict_resolution, "
	"c.mixed_resolutions AS mixed_resolutions, "
	"c.section_paths AS section_paths, c.section_md5 AS section_md5, "
	"c.section_summary AS section_summary, c.is_queryable AS is_queryable, "
	"c.red_band_index, c.green_band_index, c.blue_band_index, "
	"c.nir_band_index, c.enable_auto_ndvi\n"
	"FROM raster_coverages AS c\n"
	"LEFT JOIN spatial_ref_sys AS s ON (c.srid = s.srid)\n"
	"UNION\nSELECT c.coverage_name AS coverage_name, c.title AS title, "
	"c.abstract AS abstract,  c.sample_type AS sample_type, "
	"c.pixel_type AS pixel_type, c.num_bands AS num_bands, "
	"c.compression AS compression, c.quality AS quality, "
	"c.tile_width AS tile_width, c.tile_height AS tile_height, "
	"c.horz_resolution AS horz_resolution, c.vert_resolution AS vert_resolution, "
	"c.nodata_pixel AS nodata_pixel, c.palette AS palette, "
	"c.statistics AS statistics, c.geo_minx AS geo_minx, "
	"c.geo_miny AS geo_miny, c.geo_maxx AS geo_maxx, "
	"c.geo_maxy AS geo_maxy, x.extent_minx AS extent_minx, "
	"x.extent_miny AS extent_miny, x.extent_maxx AS extent_maxx, "
	"x.extent_maxy AS extent_maxy, s.srid AS srid, 0 AS native_srid, "
	"s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text, "
	"c.strict_resolution AS strict_resolution, "
	"c.mixed_resolutions AS mixed_resolutions, "
	"c.section_paths AS section_paths, c.section_md5 AS section_md5, "
	"c.section_summary AS section_summary, c.is_queryable AS is_queryable, "
	"c.red_band_index, c.green_band_index, c.blue_band_index, "
	"c.nir_band_index, c.enable_auto_ndvi\n"
	"FROM raster_coverages AS c\n"
	"JOIN raster_coverages_srid AS x ON (c.coverage_name = x.coverage_name)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE VIEW 'raster_coverages_ref_sys' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the raster_coverages_keyword table */
    sql = "CREATE TABLE IF NOT EXISTS raster_coverages_keyword (\n"
	"coverage_name TEXT NOT NULL,\n"
	"keyword TEXT NOT NULL,\n"
	"CONSTRAINT pk_raster_coverages_keyword PRIMARY KEY (coverage_name, keyword),\n"
	"CONSTRAINT fk_raster_coverages_keyword FOREIGN KEY (coverage_name) "
	"REFERENCES raster_coverages (coverage_name) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'raster_coverages_keyword' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the Triggers */
    if (!create_raster_coverages_triggers (sqlite))
	return 0;
    return 1;
}

SPATIALITE_PRIVATE int
createRasterCoveragesTable (void *p_sqlite)
{
/* Creating the main RasterCoverages table */
    int ok_table;
    sqlite3 *sqlite = p_sqlite;

/* checking if already defined */
    ok_table = check_raster_coverages (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateRasterCoveragesTable() error: table 'raster_coverages' already exists\n");
	  goto error;
      }
    ok_table = check_raster_coverages_srid (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateRasterCoveragesTable() error: table 'raster_coverages_srid' already exists\n");
	  goto error;
      }
    ok_table = check_raster_coverages_ref_sys (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateRasterCoveragesTable() error: view 'raster_coverages_ref_sys' already exists\n");
	  goto error;
      }
    ok_table = check_raster_coverages_keyword (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateRasterCoveragesTable() error: table 'raster_coverages_keyword' already exists\n");
	  goto error;
      }

/* creating the main RasterCoverages table */
    if (!create_raster_coverages (sqlite))
	goto error;
    return 1;

  error:
    return 0;
}

SPATIALITE_PRIVATE int
reCreateRasterCoveragesTriggers (void *p_sqlite)
{
/* (re)Creating RasterCoverages triggers */
    sqlite3 *sqlite = p_sqlite;

    drop_raster_coverages_triggers (sqlite);
    if (!create_raster_coverages_triggers (sqlite))
	return 0;
    return 1;
}

static int
create_rl2map_configurations_triggers (sqlite3 * sqlite, int relaxed)
{
/* creating the rl2map_configurations triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_rl2map_config = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'rl2map_configurations'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "rl2map_configurations") == 0)
	      ok_rl2map_config = 1;
      }
    sqlite3_free_table (results);

    if (ok_rl2map_config)
      {
	  /* creating the rl2map_configurations triggers */
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER rl2map_config_insert\n"
		    "BEFORE INSERT ON 'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on rl2map_configurations violates constraint: "
		    "not a valid RL2MapConfig')\n"
		    "WHERE XB_IsMapConfig(NEW.config) <> 1;\n"
		    "SELECT RAISE(ABORT,'insert on rl2map_configurations violates constraint: "
		    "not an XML Schema Validated RL2MapConfig')\n"
		    "WHERE XB_IsSchemaValidated(NEW.config) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER rl2map_config_insert\n"
		    "BEFORE INSERT ON 'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on rl2map_configurations violates constraint: "
		    "not a valid RL2MapConfig')\n"
		    "WHERE XB_IsMapConfig(NEW.config) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER rl2map_config_update\n"
		    "BEFORE UPDATE ON 'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on rl2map_configurations violates constraint: "
		    "not a valid RL2MapConfig')\n"
		    "WHERE XB_IsMapConfig(NEW.config) <> 1;\n"
		    "SELECT RAISE(ABORT,'update on rl2map_configurations violates constraint: "
		    "not an XML Schema Validated RL2MapConfig')\n"
		    "WHERE XB_IsSchemaValidated(NEW.config) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER rl2map_config_update\n"
		    "BEFORE UPDATE ON 'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on rl2map_configurations violates constraint: "
		    "not a valid RL2MapConfig')\n"
		    "WHERE XB_IsMapConfig(NEW.config) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after inserting */
	  sql = "CREATE TRIGGER rl2map_config_name_ins\n"
	      "AFTER INSERT ON 'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
	      "UPDATE rl2map_configurations "
	      "SET name = XB_GetName(NEW.config) " "WHERE id = NEW.id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after updating */
	  sql = "CREATE TRIGGER rl2map_config_name_upd\n"
	      "AFTER UPDATE OF config ON "
	      "'rl2map_configurations'\nFOR EACH ROW BEGIN\n"
	      "UPDATE rl2map_configurations "
	      "SET name = XB_GetName(NEW.config) " "WHERE id = NEW.id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_rl2map_configurations (sqlite3 * sqlite, int relaxed)
{
/* creating the "rl2map_configurations" table */
    char *sql;
    int ret;
    char *err_msg = NULL;

    sql = "CREATE TABLE rl2map_configurations (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"name TEXT NOT NULL DEFAULT 'missing_name' UNIQUE,\n"
	"config BLOB NOT NULL)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'rl2map_configurations' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the Triggers */
    if (!create_rl2map_configurations_triggers (sqlite, relaxed))
	return 0;
    return 1;
}

static int
check_if_coverage_exists (sqlite3 * sqlite, const char *db_prefix,
			  const char *coverage)
{
/* checking if a Coverage table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *xdb_prefix;

    if (db_prefix == NULL)
	db_prefix = "MAIN";
    xdb_prefix = gaiaDoubleQuotedSql (db_prefix);
    sql_statement =
	sqlite3_mprintf
	("SELECT name FROM \"%s\".sqlite_master WHERE type = 'table' "
	 "AND Upper(name) = Upper(%Q)", xdb_prefix, coverage);
    free (xdb_prefix);
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

SPATIALITE_PRIVATE int
checkPopulatedCoverage (void *p_sqlite, const char *db_prefix,
			const char *coverage_name)
{
/* checking if a Coverage table is already populated */
    int is_populated = 0;
    char *xdb_prefix;
    char *xname;
    char *xxname;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sqlite3 *sqlite = p_sqlite;

    xname = sqlite3_mprintf ("%s_tile_data", coverage_name);
    if (!check_if_coverage_exists (sqlite, db_prefix, xname))
      {
	  sqlite3_free (xname);
	  return 0;
      }
    if (db_prefix == NULL)
	db_prefix = "MAIN";
    xdb_prefix = gaiaDoubleQuotedSql (db_prefix);
    xxname = gaiaDoubleQuotedSql (xname);
    sqlite3_free (xname);
    sql_statement =
	sqlite3_mprintf ("SELECT ROWID FROM \"%s\".\"%s\" LIMIT 10", xdb_prefix,
			 xxname);
    free (xdb_prefix);
    free (xxname);
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	is_populated = 1;
    sqlite3_free_table (results);
    return is_populated;
}

static int
check_vector_coverages (sqlite3 * sqlite)
{
/* checking if the "vector_coverages" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('vector_coverages')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_vector_coverages_srid (sqlite3 * sqlite)
{
/* checking if the "vector_coverages_srid" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('vector_coverages_srid')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_vector_coverages_ref_sys (sqlite3 * sqlite)
{
/* checking if the "vector_coverages_ref_sys" view already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'view' "
	"AND Upper(name) = Upper('vector_coverages_ref_sys')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_vector_coverages_keyword (sqlite3 * sqlite)
{
/* checking if the "vector_coverages_keyword" table already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table' "
	"AND Upper(name) = Upper('vector_coverages_keyword')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static void
drop_vector_coverages_triggers (sqlite3 * sqlite)
{
/* dropping all "vector_coverages" triggers */
    char *sql;
    int ret;
    char *err_msg = NULL;
    char **results;
    int rows;
    int columns;
    int i;

/* checking for existing tables */
    sql =
	"SELECT name FROM sqlite_master WHERE type = 'trigger' AND tbl_name "
	"IN ('vector_coverages', 'vector_coverages_srid', 'vector_coverages_keyword')";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  sql = sqlite3_mprintf ("DROP TRIGGER %s", name);
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return;
	    }
	  sqlite3_free (sql);
      }
    sqlite3_free_table (results);
}

static int
create_vector_coverages_triggers (sqlite3 * sqlite)
{
/* creating the "vector_coverages" triggers */
    char *sql;
    int ret;
    char *err_msg = NULL;
    char **results;
    int rows;
    int columns;
    int i;
    int ok_vector_coverages = 0;
    int ok_vector_coverages_srid = 0;
    int ok_vector_coverages_keyword = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name "
	"IN ('vector_coverages', 'vector_coverages_srid', 'vector_coverages_keyword')";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "vector_coverages") == 0)
	      ok_vector_coverages = 1;
	  if (strcasecmp (name, "vector_coverages_srid") == 0)
	      ok_vector_coverages_srid = 1;
	  if (strcasecmp (name, "vector_coverages_keyword") == 0)
	      ok_vector_coverages_keyword = 1;
      }
    sqlite3_free_table (results);

    if (ok_vector_coverages)
      {
	  /* creating the vector_coverages triggers */
	  sql = "CREATE TRIGGER IF NOT EXISTS vector_coverages_name_insert\n"
	      "BEFORE INSERT ON 'vector_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on layer_vectors violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER IF NOT EXISTS vector_coverages_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'vector_coverages'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }

    if (ok_vector_coverages_srid)
      {
	  /* creating the vector_coverages_srid triggers */
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS vector_coverages_srid_name_insert\n"
	      "BEFORE INSERT ON 'vector_coverages_srid'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_srid violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_srid violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_srid violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS vector_coverages_srid_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'vector_coverages_srid'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_srid violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_srid violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_srid violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }

    if (ok_vector_coverages_keyword)
      {
	  /* creating the vector_coverages_keyword triggers */
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS vector_coverages_keyword_name_insert\n"
	      "BEFORE INSERT ON 'vector_coverages_keyword'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on vector_coverages_keyword violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql =
	      "CREATE TRIGGER IF NOT EXISTS vector_coverages_keyword_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'vector_coverages_keyword'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_keyword violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on vector_coverages_keyword violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_vector_coverages (sqlite3 * sqlite)
{
/* creating the "vector_coverages" table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE IF NOT EXISTS vector_coverages (\n"
	"coverage_name TEXT NOT NULL PRIMARY KEY,\n"
	"f_table_name TEXT,\n"
	"f_geometry_column TEXT,\n"
	"view_name TEXT,\n"
	"view_geometry TEXT,\n"
	"virt_name TEXT,\n"
	"virt_geometry TEXT,\n"
	"topology_name TEXT,\n"
	"network_name TEXT,\n"
	"geo_minx DOUBLE,\n"
	"geo_miny DOUBLE,\n"
	"geo_maxx DOUBLE,\n"
	"geo_maxy DOUBLE,\n"
	"extent_minx DOUBLE,\n"
	"extent_miny DOUBLE,\n"
	"extent_maxx DOUBLE,\n"
	"extent_maxy DOUBLE,\n"
	"title TEXT NOT NULL DEFAULT '*** missing Title ***',\n"
	"abstract TEXT NOT NULL DEFAULT '*** missing Abstract ***',\n"
	"is_queryable INTEGER NOT NULL,\n"
	"is_editable INTEGER NOT NULL,\n"
	"copyright TEXT NOT NULL DEFAULT '*** unknown ***',\n"
	"license INTEGER NOT NULL DEFAULT 0,\n"
	"CONSTRAINT fk_vc_gc FOREIGN KEY (f_table_name, f_geometry_column) "
	"REFERENCES geometry_columns (f_table_name, f_geometry_column) "
	"ON DELETE CASCADE,\n"
	"CONSTRAINT fk_vc_sv FOREIGN KEY (view_name, view_geometry) "
	"REFERENCES views_geometry_columns (view_name, view_geometry) "
	"ON DELETE CASCADE,\n"
	"CONSTRAINT fk_vc_vt FOREIGN KEY (virt_name, virt_geometry) "
	"REFERENCES virts_geometry_columns (virt_name, virt_geometry) "
	"ON DELETE CASCADE,\n"
	"CONSTRAINT fk_vc_lic FOREIGN KEY (license) "
	"REFERENCES data_licenses (id))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'vector_coverages' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the VectorLayers index */
    sql =
	"CREATE UNIQUE INDEX IF NOT EXISTS idx_vector_coverages ON vector_coverages "
	"(f_table_name, f_geometry_column)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_vector_coverages' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the vector_coverages_srid table */
    sql = "CREATE TABLE IF NOT EXISTS vector_coverages_srid (\n"
	"coverage_name TEXT NOT NULL,\n"
	"srid INTEGER NOT NULL,\n"
	"extent_minx DOUBLE,\n"
	"extent_miny DOUBLE,\n"
	"extent_maxx DOUBLE,\n"
	"extent_maxy DOUBLE,\n"
	"CONSTRAINT pk_vector_coverages_srid PRIMARY KEY (coverage_name, srid),\n"
	"CONSTRAINT fk_vector_coverages_srid FOREIGN KEY (coverage_name) "
	"REFERENCES vector_coverages (coverage_name) ON DELETE CASCADE,\n"
	"CONSTRAINT fk_vector_srid FOREIGN KEY (srid) "
	"REFERENCES spatial_ref_sys (srid))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'vector_coverages_srid' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the vector_coverages_ref_sys view */
    sql = "CREATE VIEW IF NOT EXISTS vector_coverages_ref_sys AS\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, v.extent_minx AS extent_minx, "
	"v.extent_miny AS extent_miny, v.extent_maxx AS extent_maxx, v.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 1 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN geometry_columns AS x ON (v.topology_name IS NULL AND v.network_name IS NULL AND "
	"v.f_table_name IS NOT NULL AND v.f_geometry_column IS NOT NULL AND "
	"v.f_table_name = x.f_table_name AND v.f_geometry_column = x.f_geometry_column)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)\n"
	"UNION\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, v.extent_minx AS extent_minx, "
	"v.extent_miny AS extent_miny, v.extent_maxx AS extent_maxx, v.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 1 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN views_geometry_columns AS y ON (v.view_name IS NOT NULL AND "
	"v.view_geometry IS NOT NULL AND v.view_name = y.view_name AND "
	"v.view_geometry = y.view_geometry)\n"
	"JOIN geometry_columns AS x ON (y.f_table_name = x.f_table_name AND "
	"y.f_geometry_column = x.f_geometry_column)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)\n"
	"UNION\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, v.extent_minx AS extent_minx, "
	"v.extent_miny AS extent_miny, v.extent_maxx AS extent_maxx, v.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 1 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN virts_geometry_columns AS x ON (v.virt_name IS NOT NULL "
	"AND v.virt_geometry IS NOT NULL AND v.virt_name = x.virt_name "
	"AND v.virt_geometry = x.virt_geometry)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)\n"
	"UNION\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, v.extent_minx AS extent_minx, "
	"v.extent_miny AS extent_miny, v.extent_maxx AS extent_maxx, v.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 1 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN topologies AS x ON (v.topology_name IS NOT NULL AND v.topology_name = x.topology_name)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)\n"
	"UNION\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, v.extent_minx AS extent_minx, "
	"v.extent_miny AS extent_miny, v.extent_maxx AS extent_maxx, v.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 1 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN networks AS x ON (v.network_name IS NOT NULL AND v.network_name = x.network_name)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)\n"
	"UNION\n"
	"SELECT v.coverage_name AS coverage_name, v.title AS title, v.abstract AS abstract, "
	"v.is_queryable AS is_queryable, v.geo_minx AS geo_minx, v.geo_miny AS geo_miny, "
	"v.geo_maxx AS geo_maxx, v.geo_maxy AS geo_maxy, x.extent_minx AS extent_minx, "
	"x.extent_miny AS extent_miny, x.extent_maxx AS extent_maxx, x.extent_maxy AS extent_maxy, "
	"s.srid AS srid, 0 AS native_srid, s.auth_name AS auth_name, s.auth_srid AS auth_srid, "
	"s.ref_sys_name AS ref_sys_name, s.proj4text AS proj4text\n"
	"FROM vector_coverages AS v\n"
	"JOIN vector_coverages_srid AS x ON (v.coverage_name = x.coverage_name)\n"
	"LEFT JOIN spatial_ref_sys AS s ON (x.srid = s.srid)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE VIEW 'vector_coverages_ref_sys' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

/* creating the vector_coverages_keyword table */
    sql = "CREATE TABLE IF NOT EXISTS vector_coverages_keyword (\n"
	"coverage_name TEXT NOT NULL,\n"
	"keyword TEXT NOT NULL,\n"
	"CONSTRAINT pk_vector_coverages_keyword PRIMARY KEY (coverage_name, keyword),\n"
	"CONSTRAINT fk_vector_coverages_keyword FOREIGN KEY (coverage_name) "
	"REFERENCES vector_coverages (coverage_name) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'vector_coverages_keyword' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_vector_coverages_triggers (sqlite))
	return 0;
    return 1;
}

SPATIALITE_PRIVATE int
createVectorCoveragesTable (void *p_sqlite)
{
/* Creating the main VectorCoverages table */
    int ok_table;
    sqlite3 *sqlite = p_sqlite;

#ifdef ENABLE_RTTOPO		/* only if RTTOPO is enabled */

/* attempting to create Topologies and Networks tables */
    do_create_topologies (sqlite);
    do_create_networks (sqlite);

/* checking if already defined */
    ok_table = check_vector_coverages (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateVectorCoveragesTable() error: table 'vector_coverages' already exists\n");
	  goto error;
      }
    ok_table = check_vector_coverages_srid (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateVectorCoveragesTable() error: table 'vector_coverages_srid' already exists\n");
	  goto error;
      }
    ok_table = check_vector_coverages_ref_sys (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateVectorCoveragesTable() error: view 'vector_coverages_ref_sys' already exists\n");
	  goto error;
      }
    ok_table = check_vector_coverages_keyword (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("CreateVectorCoveragesTable() error: table 'vector_coverages_keyword' already exists\n");
	  goto error;
      }

/* creating the main VectorCoverages table and triggers */
    if (!create_vector_coverages (sqlite))
	goto error;
    return 1;

#else

    spatialite_e
	("CreateVectorCoveragesTable() error: libspatialite was built by disabling Topology\n");

#endif /* end ENABLE_RTTOPO conditionals */

  error:
    return 0;
}

SPATIALITE_PRIVATE int
reCreateVectorCoveragesTriggers (void *p_sqlite)
{
/* (re)Creating VectorCoverages triggers */
    sqlite3 *sqlite = p_sqlite;

    drop_vector_coverages_triggers (sqlite);
    if (!create_vector_coverages_triggers (sqlite))
	return 0;
    return 1;
}

static int
check_wms_getcapabilities (sqlite3 * sqlite)
{
/* checking if the "wms_getcapabilities" table already exists */
    int exists = 0;
    const char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table'"
	"AND Upper(name) = Upper('wms_getcapabilities')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_wms_getmap (sqlite3 * sqlite)
{
/* checking if the "wms_getmap" table already exists */
    int exists = 0;
    const char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table'"
	"AND Upper(name) = Upper('wms_getmap')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_wms_settings (sqlite3 * sqlite)
{
/* checking if the "wms_settings" table already exists */
    int exists = 0;
    const char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table'"
	"AND Upper(name) = Upper('wms_settings')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
check_wms_ref_sys (sqlite3 * sqlite)
{
/* checking if the "wms_ref_sys" table already exists */
    int exists = 0;
    const char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement = "SELECT name FROM sqlite_master WHERE type = 'table'"
	"AND Upper(name) = Upper('wms_ref_sys')";
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

SPATIALITE_PRIVATE int
create_wms_tables (sqlite3 * sqlite)
{
/* creating the WMS support tables */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE IF NOT EXISTS wms_getcapabilities (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"url TEXT NOT NULL,\n"
	"title TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"abstract TEXT NOT NULL DEFAULT '*** undefined ***')";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'wms_getcapabilities' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql =
	"CREATE UNIQUE INDEX IF NOT EXISTS idx_wms_getcapabilities ON wms_getcapabilities (url)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_wms_getcapabilities' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

    sql = "CREATE TABLE IF NOT EXISTS wms_getmap (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"parent_id INTEGER NOT NULL,\n"
	"url TEXT NOT NULL,\n"
	"layer_name TEXT NOT NULL,\n"
	"title TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"abstract TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"version TEXT NOT NULL,\n"
	"srs TEXT NOT NULL,\n"
	"format TEXT NOT NULL,\n"
	"style TEXT NOT NULL,\n"
	"transparent INTEGER NOT NULL CHECK (transparent IN (0, 1)),\n"
	"flip_axes INTEGER NOT NULL CHECK (flip_axes IN (0, 1)),\n"
	"is_queryable INTEGER NOT NULL CHECK (is_queryable IN (0, 1)),\n"
	"getfeatureinfo_url TEXT,\n"
	"bgcolor TEXT,\n"
	"tiled INTEGER NOT NULL CHECK (tiled IN (0, 1)),\n"
	"tile_width INTEGER NOT NULL CHECK (tile_width BETWEEN 256 AND 5000),\n"
	"tile_height INTEGER NOT NULL CHECK (tile_width BETWEEN 256 AND 5000),\n"
	"is_cached INTEGER NOT NULL CHECK (is_cached IN (0, 1)),\n"
	"copyright TEXT NOT NULL DEFAULT '*** unknown ***',\n"
	"license INTEGER NOT NULL DEFAULT 0,\n"
	"CONSTRAINT fk_wms_getmap FOREIGN KEY (parent_id) "
	"REFERENCES wms_getcapabilities (id) ON DELETE CASCADE,\n"
	"CONSTRAINT fk_wms_lic FOREIGN KEY (license) "
	"REFERENCES data_licenses (id))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'wms_getmap' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE UNIQUE INDEX idx_wms_getmap ON wms_getmap (url, layer_name)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_wms_getmap' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

    sql = "CREATE TABLE IF NOT EXISTS wms_settings (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"parent_id INTEGER NOT NULL,\n"
	"key TEXT NOT NULL CHECK (Lower(key) IN ('version', 'format', 'style')),\n"
	"value TEXT NOT NULL,\n"
	"is_default INTEGER NOT NULL CHECK (is_default IN (0, 1)),\n"
	"CONSTRAINT fk_wms_settings FOREIGN KEY (parent_id) "
	"REFERENCES wms_getmap (id) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'wms_settings' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql =
	"CREATE UNIQUE INDEX idx_wms_settings ON wms_settings (parent_id, key, value)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_wms_settings' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

    sql = "CREATE TABLE IF NOT EXISTS wms_ref_sys (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"parent_id INTEGER NOT NULL,\n"
	"srs TEXT NOT NULL,\n"
	"minx DOUBLE NOT NULL,\n"
	"miny DOUBLE NOT NULL,\n"
	"maxx DOUBLE NOT NULL,\n"
	"maxy DOUBLE NOT NULL,\n"
	"is_default INTEGER NOT NULL CHECK (is_default IN (0, 1)),\n"
	"CONSTRAINT fk_wms_ref_sys FOREIGN KEY (parent_id) "
	"REFERENCES wms_getmap (id) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'wms_ref_sys' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql =
	"CREATE UNIQUE INDEX IF NOT EXISTS idx_wms_ref_sys ON wms_ref_sys (parent_id, srs)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_wms_ref_sys' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }

    return 1;
}

SPATIALITE_PRIVATE int
createWMSTables (void *p_sqlite)
{
/* Creating all WMS support tables */
    int ok_table;
    sqlite3 *sqlite = p_sqlite;

/* checking if already defined */
    ok_table = check_wms_getcapabilities (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("WMS_CreateTables() error: table 'wms_getcapabilities' already exists\n");
	  goto error;
      }
    ok_table = check_wms_getmap (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("WMS_CreateTables() error: table 'wms_getmap' already exists\n");
	  goto error;
      }
    ok_table = check_wms_settings (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("WMS_CreateTables() error: table 'wms_settings' already exists\n");
	  goto error;
      }
    ok_table = check_wms_ref_sys (sqlite);
    if (ok_table)
      {
	  spatialite_e
	      ("WMS_CreateTables() error: table 'wms_ref_sys' already exists\n");
	  goto error;
      }

/* creating the WMS support tables */
    if (!create_wms_tables (sqlite))
	goto error;
    return 1;

  error:
    return 0;
}

#ifdef ENABLE_LIBXML2		/* including LIBXML2 */

static void
drop_styling_triggers (sqlite3 * sqlite)
{
/* dropping all "styling" triggers */
    char *sql;
    int ret;
    char *err_msg = NULL;
    char **results;
    int rows;
    int columns;
    int i;

/* checking for existing tables */
    sql =
	"SELECT name FROM sqlite_master WHERE type = 'trigger' AND tbl_name "
	"IN ('SE_external_graphics', 'SE_fonts', 'SE_vector_styles', 'SE_raster_styles', "
	"'SE_vector_styled_layers', 'SE_raster_styled_layers', 'rl2map_configurations')";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  sql = sqlite3_mprintf ("DROP TRIGGER %s", name);
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return;
	    }
	  sqlite3_free (sql);
      }
    sqlite3_free_table (results);
}

static int
check_styling_table (sqlite3 * sqlite, const char *table, int is_view)
{
/* checking if some SLD/SE Styling-related table/view already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement =
	sqlite3_mprintf ("SELECT name FROM sqlite_master WHERE type = '%s'"
			 "AND Upper(name) = Upper(%Q)",
			 (!is_view) ? "table" : "view", table);
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

static int
create_external_graphics_triggers (sqlite3 * sqlite)
{
/* creating the SE_external_graphics triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_external_graphics = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_external_graphics'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "SE_external_graphics") == 0)
	      ok_external_graphics = 1;
      }
    sqlite3_free_table (results);

    if (ok_external_graphics)
      {
	  /* creating the SE_external_graphics triggers */
	  sql = "CREATE TRIGGER sextgr_mime_type_insert\n"
	      "BEFORE INSERT ON 'SE_external_graphics'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on SE_external_graphics violates constraint: "
	      "GetMimeType(resource) must be one of ''image/gif'' | ''image/png'' | "
	      "''image/jpeg'' | ''image/svg+xml''')\n"
	      "WHERE GetMimeType(NEW.resource) NOT IN ('image/gif', 'image/png', "
	      "'image/jpeg', 'image/svg+xml');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER sextgr_mime_type_update\n"
	      "BEFORE UPDATE OF 'mime_type' ON 'SE_external_graphics'"
	      "\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT, 'update on SE_external_graphics violates constraint: "
	      "GetMimeType(resource) must be one of ''image/gif'' | ''image/png'' | "
	      "''image/jpeg'' | ''image/svg+xml''')\n"
	      "WHERE GetMimeType(NEW.resource) NOT IN ('image/gif', 'image/png', "
	      "'image/jpeg', 'image/svg+xml');\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_external_graphics (sqlite3 * sqlite)
{
/* creating the SE_external_graphics table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_external_graphics (\n"
	"xlink_href TEXT NOT NULL PRIMARY KEY,\n"
	"title TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"abstract TEXT NOT NULL DEFAULT '*** undefined ***',\n"
	"resource BLOB NOT NULL,\n"
	"file_name TEXT NOT NULL DEFAULT '*** undefined ***')";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_external_graphics' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_external_graphics_triggers (sqlite))
	return 0;
    return 1;
}

static int
create_fonts_triggers (sqlite3 * sqlite)
{
/* creating the SE_fonts triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_fonts = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_fonts'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "topologies") == 0)
	      ok_fonts = 1;
      }
    sqlite3_free_table (results);

    if (ok_fonts)
      {
	  /* creating the SE_fonts triggers */
	  sql = "CREATE TRIGGER se_font_insert1\n"
	      "BEFORE INSERT ON 'SE_fonts'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on SE_Fonts violates constraint: "
	      "invalid Font')\nWHERE IsValidFont(NEW.font) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER se_font_insert2\n"
	      "BEFORE INSERT ON 'SE_fonts'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on SE_Fonts violates constraint: "
	      "mismatching FontFacename')\nWHERE "
	      "CheckFontFacename(NEW.font_facename, NEW.font) <> 1;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* rejecting any possible UPDATE */
	  sql = "CREATE TRIGGER se_font_update\n"
	      "BEFORE UPDATE ON 'SE_fonts'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'UPDATE on SE_Fonts is always forbidden')\n;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_fonts (sqlite3 * sqlite)
{
/* creating the SE_fonts table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_fonts (\n"
	"font_facename TEXT NOT NULL PRIMARY KEY,\n" "font BLOB NOT NULL)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_fonts' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_fonts_triggers (sqlite))
	return 0;
    return 1;
}

static int
create_vector_styles_triggers (sqlite3 * sqlite, int relaxed)
{
/* creating the SE_vector_styles triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_vector_styles = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_vector_styles'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "SE_vector_styles") == 0)
	      ok_vector_styles = 1;
      }
    sqlite3_free_table (results);

    if (ok_vector_styles)
      {
	  /* creating the SE_vector_styles triggers */
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER sevector_style_insert\n"
		    "BEFORE INSERT ON 'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on SE_vector_styles violates constraint: "
		    "not a valid SLD/SE Vector Style')\n"
		    "WHERE XB_IsSldSeVectorStyle(NEW.style) <> 1;\n"
		    "SELECT RAISE(ABORT,'insert on SE_vector_styles violates constraint: "
		    "not an XML Schema Validated SLD/SE Vector Style')\n"
		    "WHERE XB_IsSchemaValidated(NEW.style) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER sevector_style_insert\n"
		    "BEFORE INSERT ON 'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on SE_vector_styles violates constraint: "
		    "not a valid SLD/SE Vector Style')\n"
		    "WHERE XB_IsSldSeVectorStyle(NEW.style) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER sevector_style_update\n"
		    "BEFORE UPDATE ON 'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on SE_vector_styles violates constraint: "
		    "not a valid SLD/SE Vector Style')\n"
		    "WHERE XB_IsSldSeVectorStyle(NEW.style) <> 1;\n"
		    "SELECT RAISE(ABORT,'update on SE_vector_styles violates constraint: "
		    "not an XML Schema Validated SLD/SE Vector Style')\n"
		    "WHERE XB_IsSchemaValidated(NEW.style) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER sevector_style_update\n"
		    "BEFORE UPDATE ON 'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on SE_vector_styles violates constraint: "
		    "not a valid SLD/SE Vector Style')\n"
		    "WHERE XB_IsSldSeVectorStyle(NEW.style) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after inserting */
	  sql = "CREATE TRIGGER sevector_style_name_ins\n"
	      "AFTER INSERT ON 'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
	      "UPDATE SE_vector_styles "
	      "SET style_name = XB_GetName(NEW.style) "
	      "WHERE style_id = NEW.style_id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after updating */
	  sql = "CREATE TRIGGER sevector_style_name_upd\n"
	      "AFTER UPDATE OF style ON "
	      "'SE_vector_styles'\nFOR EACH ROW BEGIN\n"
	      "UPDATE SE_vector_styles "
	      "SET style_name = XB_GetName(NEW.style) "
	      "WHERE style_id = NEW.style_id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_vector_styles (sqlite3 * sqlite, int relaxed)
{
/* creating the SE_vector_styles table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_vector_styles (\n"
	"style_id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"style_name TEXT NOT NULL DEFAULT 'missing_name' UNIQUE,\n"
	"style BLOB NOT NULL)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_vector_styles' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the style Name index */
    sql = "CREATE INDEX idx_vector_styles ON SE_vector_styles (style_name)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_vector_styles' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_vector_styles_triggers (sqlite, relaxed))
	return 0;
    return 1;
}

static int
create_vector_styled_layers_triggers (sqlite3 * sqlite)
{
/* creating the SE_vector_styled_layers triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_vector_styled = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_vector_styled_layers'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "SE_vector_styled_layers") == 0)
	      ok_vector_styled = 1;
      }
    sqlite3_free_table (results);

    if (ok_vector_styled)
      {
	  /* creating the SE_vector_styled_layers triggers */
	  sql = "CREATE TRIGGER sevstl_coverage_name_insert\n"
	      "BEFORE INSERT ON 'SE_vector_styled_layers'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER sevstl_coverage_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'SE_vector_styled_layers'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on SE_vector_styled_layers violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_vector_styled_layers (sqlite3 * sqlite)
{
/* creating the SE_vector_styled_layers table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_vector_styled_layers (\n"
	"coverage_name TEXT NOT NULL,\n"
	"style_id INTEGER NOT NULL,\n"
	"CONSTRAINT pk_sevstl PRIMARY KEY "
	"(coverage_name, style_id),\n"
	"CONSTRAINT fk_sevstl_cvg FOREIGN KEY (coverage_name) "
	"REFERENCES vector_coverages (coverage_name) ON DELETE CASCADE,\n"
	"CONSTRAINT fk_sevstl_stl FOREIGN KEY (style_id) "
	"REFERENCES SE_vector_styles (style_id) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_vector_styled_layers' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the style_id index */
    sql = "CREATE INDEX idx_sevstl_style ON SE_vector_styled_layers (style_id)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_svstl_style' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_vector_styled_layers_triggers (sqlite))
	return 0;
    return 1;
}

static int
create_raster_styles_triggers (sqlite3 * sqlite, int relaxed)
{
/* creating the SE_raster_styles triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_raster_styles = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_raster_styles'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "SE_raster_styles") == 0)
	      ok_raster_styles = 1;
      }
    sqlite3_free_table (results);

    if (ok_raster_styles)
      {
	  /* creating the SE_raster_styles triggers */
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER seraster_style_insert\n"
		    "BEFORE INSERT ON 'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on SE_raster_styles violates constraint: "
		    "not a valid SLD/SE Raster Style')\n"
		    "WHERE XB_IsSldSeRasterStyle(NEW.style) <> 1;\n"
		    "SELECT RAISE(ABORT,'insert on SE_raster_styles violates constraint: "
		    "not an XML Schema Validated SLD/SE Raster Style')\n"
		    "WHERE XB_IsSchemaValidated(NEW.style) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER seraster_style_insert\n"
		    "BEFORE INSERT ON 'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'insert on SE_raster_styles violates constraint: "
		    "not a valid SLD/SE Raster Style')\n"
		    "WHERE XB_IsSldSeRasterStyle(NEW.style) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  if (relaxed == 0)
	    {
		/* strong trigger - imposing XML schema validation */
		sql = "CREATE TRIGGER seraster_style_update\n"
		    "BEFORE UPDATE ON 'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on SE_raster_styles violates constraint: "
		    "not a valid SLD/SE Raster Style')\n"
		    "WHERE XB_IsSldSeRasterStyle(NEW.style) <> 1;\n"
		    "SELECT RAISE(ABORT,'update on SE_raster_styles violates constraint: "
		    "not an XML Schema Validated SLD/SE Raster Style')\n"
		    "WHERE XB_IsSchemaValidated(NEW.style) <> 1;\nEND";
	    }
	  else
	    {
		/* relaxed trigger - not imposing XML schema validation */
		sql = "CREATE TRIGGER seraster_style_update\n"
		    "BEFORE UPDATE ON 'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
		    "SELECT RAISE(ABORT,'update on SE_raster_styles violates constraint: "
		    "not a valid SLD/SE Raster Style')\n"
		    "WHERE XB_IsSldSeRasterStyle(NEW.style) <> 1;\nEND";
	    }
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after inserting */
	  sql = "CREATE TRIGGER seraster_style_name_ins\n"
	      "AFTER INSERT ON 'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
	      "UPDATE SE_raster_styles "
	      "SET style_name = XB_GetName(NEW.style) "
	      "WHERE style_id = NEW.style_id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
/* automatically setting the style_name after updating */
	  sql = "CREATE TRIGGER seraster_style_name_upd\n"
	      "AFTER UPDATE OF style ON "
	      "'SE_raster_styles'\nFOR EACH ROW BEGIN\n"
	      "UPDATE SE_raster_styles "
	      "SET style_name = XB_GetName(NEW.style) "
	      "WHERE style_id = NEW.style_id;\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_raster_styles (sqlite3 * sqlite, int relaxed)
{
/* creating the SE_raster_styles table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_raster_styles (\n"
	"style_id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"style_name TEXT NOT NULL DEFAULT 'missing_name' UNIQUE,\n"
	"style BLOB NOT NULL)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_raster_styles' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the style Name index */
    sql = "CREATE INDEX idx_raster_styles ON SE_raster_styles (style_name)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_raster_styles' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_raster_styles_triggers (sqlite, relaxed))
	return 0;
    return 1;
}

static int
create_raster_styled_layers_triggers (sqlite3 * sqlite)
{
/* creating the SE_raster_styled_layers triggers */
    char *sql;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    char *err_msg = NULL;
    int ok_raster_styled = 0;

/* checking for existing tables */
    sql =
	"SELECT tbl_name FROM sqlite_master WHERE type = 'table' AND tbl_name = 'SE_raster_styled_layers'";
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
      {
	  const char *name = results[(i * columns) + 0];
	  if (strcasecmp (name, "SE_raster_styled_layers") == 0)
	      ok_raster_styled = 1;
      }
    sqlite3_free_table (results);

    if (ok_raster_styled)
      {
	  /* creating the SE_raster_styled_layers triggers */
	  sql = "CREATE TRIGGER serstl_coverage_name_insert\n"
	      "BEFORE INSERT ON 'SE_raster_styled_layers'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'insert on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'insert on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
	  sql = "CREATE TRIGGER serstl_coverage_name_update\n"
	      "BEFORE UPDATE OF 'coverage_name' ON 'SE_raster_styled_layers'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must not contain a single quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%''%');\n"
	      "SELECT RAISE(ABORT,'update on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must not contain a double quote')\n"
	      "WHERE NEW.coverage_name LIKE ('%\"%');\n"
	      "SELECT RAISE(ABORT,'update on SE_raster_styled_layers violates constraint: "
	      "coverage_name value must be lower case')\n"
	      "WHERE NEW.coverage_name <> lower(NEW.coverage_name);\nEND";
	  ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
	  if (ret != SQLITE_OK)
	    {
		spatialite_e ("SQL error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return 0;
	    }
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_raster_styled_layers (sqlite3 * sqlite)
{
/* creating the SE_raster_styled_layers table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE SE_raster_styled_layers (\n"
	"coverage_name TEXT NOT NULL,\n"
	"style_id INTEGER NOT NULL,\n"
	"CONSTRAINT pk_serstl PRIMARY KEY (coverage_name, style_id),\n"
	"CONSTRAINT fk_serstl_cov FOREIGN KEY (coverage_name) "
	"REFERENCES raster_coverages (coverage_name) ON DELETE CASCADE,\n"
	"CONSTRAINT fk_serstl_stl FOREIGN KEY (style_id) "
	"REFERENCES SE_raster_styles (style_id) ON DELETE CASCADE)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'SE_raster_styled_layers' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the style_id index */
    sql = "CREATE INDEX idx_serstl_style ON SE_raster_styled_layers (style_id)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE INDEX 'idx_serstl_style' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (!create_raster_styled_layers_triggers (sqlite))
	return 0;
    return 1;
}

SPATIALITE_PRIVATE int
create_external_graphics_view (sqlite3 * sqlite)
{
/* creating the SE_external_graphics_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf
	("CREATE VIEW SE_external_graphics_view AS\n"
	 "SELECT xlink_href AS xlink_href, title AS title, "
	 "abstract AS abstract, resource AS resource, "
	 "file_name AS file_name, GetMimeType(resource) AS mime_type\n"
	 "FROM SE_external_graphics");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'SE_external_graphics_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_fonts_view (sqlite3 * sqlite)
{
/* creating the SE_fonts_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf
	("CREATE VIEW SE_fonts_view AS\n"
	 "SELECT font_facename AS font_facename, "
	 "GetFontFamily(font) AS family_name, "
	 "IsFontBold(font) AS bold, IsFontItalic(font) AS italic, "
	 "font AS font\nFROM SE_fonts");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE VIEW 'SE_fonts_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_vector_styles_view (sqlite3 * sqlite)
{
/* creating the SE_vector_styles_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf ("CREATE VIEW SE_vector_styles_view AS \n"
			 "SELECT style_name AS name, XB_GetTitle(style) AS title, "
			 "XB_GetAbstract(style) AS abstract, style AS style, "
			 "XB_IsSchemaValidated(style) AS schema_validated, "
			 "XB_GetSchemaURI(style) AS schema_uri\n"
			 "FROM SE_vector_styles");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'SE_vector_styles_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_raster_styles_view (sqlite3 * sqlite)
{
/* creating the SE_raster_styles_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf ("CREATE VIEW SE_raster_styles_view AS \n"
			 "SELECT style_name AS name, XB_GetTitle(style) AS title, "
			 "XB_GetAbstract(style) AS abstract, style AS style, "
			 "XB_IsSchemaValidated(style) AS schema_validated, "
			 "XB_GetSchemaURI(style) AS schema_uri\n"
			 "FROM SE_raster_styles");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'SE_raster_styles_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_rl2map_configurations_view (sqlite3 * sqlite)
{
/* creating the rl2map_configurations_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf ("CREATE VIEW rl2map_configurations_view AS \n"
			 "SELECT name AS name, XB_GetTitle(config) AS title, "
			 "XB_GetAbstract(config) AS abstract, config AS config, "
			 "XB_IsSchemaValidated(config) AS schema_validated, "
			 "XB_GetSchemaURI(config) AS schema_uri\n"
			 "FROM rl2map_configurations");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'rl2map_configurations_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_vector_styled_layers_view (sqlite3 * sqlite)
{
/* creating the SE_vector_styled_layers_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf ("CREATE VIEW SE_vector_styled_layers_view AS \n"
			 "SELECT l.coverage_name AS coverage_name, v.f_table_name AS f_table_name, "
			 "v.f_geometry_column AS f_geometry_column, l.style_id AS style_id, "
			 "s.style_name AS name, XB_GetTitle(s.style) AS title, "
			 "XB_GetAbstract(s.style) AS abstract, s.style AS style, "
			 "XB_IsSchemaValidated(s.style) AS schema_validated, "
			 "XB_GetSchemaURI(s.style) AS schema_uri\n"
			 "FROM SE_vector_styled_layers AS l\n"
			 "JOIN vector_coverages AS v ON (l.coverage_name = v.coverage_name) "
			 "JOIN SE_vector_styles AS s ON (l.style_id = s.style_id)");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'SE_vector_styled_layers_view' error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

static int
auto_register_standard_brushes (sqlite3 * sqlite)
{
/* AutoRegistering all Graphic Standard Brushes reguired by RasterLite2 */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement = sqlite3_mprintf ("SELECT SE_AutoRegisterStandardBrushes()");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("SELECT SE_AutoRegisterStandardBrushes() error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_raster_styled_layers_view (sqlite3 * sqlite)
{
/* creating the SE_raster_styled_layers_view view */
    char *sql_statement;
    int ret;
    char *err_msg = NULL;
    sql_statement =
	sqlite3_mprintf ("CREATE VIEW SE_raster_styled_layers_view AS \n"
			 "SELECT l.coverage_name AS coverage_name, l.style_id AS style_id, "
			 "s.style_name AS name, XB_GetTitle(s.style) AS title, "
			 "XB_GetAbstract(s.style) AS abstract, s.style AS style, "
			 "XB_IsSchemaValidated(s.style) AS schema_validated, "
			 "XB_GetSchemaURI(s.style) AS schema_uri\n"
			 "FROM SE_raster_styled_layers AS l\n"
			 "JOIN SE_raster_styles AS s ON (l.style_id = s.style_id)");
    ret = sqlite3_exec (sqlite, sql_statement, NULL, NULL, &err_msg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CREATE VIEW 'SE_raster_styled_layers_view' error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
createStylingTables_ex (void *p_sqlite, int relaxed, int transaction)
{
/* Creating the SE Styling tables */
    const char *tables[15];
    int views[15];
    const char **p_tbl;
    int *p_view;
    int ok_table;
    sqlite3 *sqlite = p_sqlite;
    int ret;

    if (transaction)
      {
	  /* starting a Transaction */
	  ret = sqlite3_exec (p_sqlite, "BEGIN", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	      goto error;
      }

/* checking SLD/SE Styling tables */
    tables[0] = "SE_external_graphics";
    tables[1] = "SE_fonts";
    tables[2] = "SE_vector_styles";
    tables[3] = "SE_raster_styles";
    tables[4] = "RL2map_configurations";
    tables[5] = "SE_vector_styled_layers";
    tables[6] = "SE_raster_styled_layers";
    tables[7] = "SE_external_graphics_view";
    tables[8] = "SE_fonts_view";
    tables[9] = "SE_vector_styles_view";
    tables[10] = "SE_raster_styles_view";
    tables[11] = "RL2map_configurations_view";
    tables[12] = "SE_vector_styled_layers_view";
    tables[13] = "SE_raster_styled_layers_view";
    tables[14] = NULL;
    views[0] = 0;
    views[1] = 0;
    views[2] = 0;
    views[3] = 0;
    views[4] = 0;
    views[5] = 0;
    views[6] = 0;
    views[7] = 1;
    views[8] = 1;
    views[9] = 1;
    views[10] = 1;
    views[11] = 1;
    views[12] = 1;
    views[13] = 1;
    p_tbl = tables;
    p_view = views;
    while (*p_tbl != NULL)
      {
	  ok_table = check_styling_table (sqlite, *p_tbl, *p_view);
	  if (ok_table)
	      goto error;
	  p_tbl++;
	  p_view++;
      }

/* creating the SLD/SE Styling tables */
    if (!check_raster_coverages (sqlite))
      {
	  /* creating the main RasterCoverages table as well */
	  if (!create_raster_coverages (sqlite))
	      goto error;
      }
#ifdef ENABLE_RTTOPO		/* only if RTTOPO is enabled */
    if (!check_vector_coverages (sqlite))
      {
	  /* creating both TOPOLOGIES and NETWORKS tables */
	  do_create_topologies (sqlite);
	  do_create_networks (sqlite);
	  /* creating the main VectorCoverages table as well */
	  if (!create_vector_coverages (sqlite))
	      goto error;
      }
#endif /* end TOPOLOGY conditionals */
    if (!create_external_graphics (sqlite))
	goto error;
    if (!create_fonts (sqlite))
	goto error;
    if (!create_vector_styles (sqlite, relaxed))
	goto error;
    if (!create_raster_styles (sqlite, relaxed))
	goto error;
    if (!create_rl2map_configurations (sqlite, relaxed))
	goto error;
    if (!create_vector_styled_layers (sqlite))
	goto error;
    if (!create_raster_styled_layers (sqlite))
	goto error;
    if (!create_external_graphics_view (sqlite))
	goto error;
    if (!auto_register_standard_brushes (sqlite))
	goto error;
    if (!create_fonts_view (sqlite))
	goto error;
    if (!create_vector_styles_view (sqlite))
	goto error;
    if (!create_raster_styles_view (sqlite))
	goto error;
    if (!create_rl2map_configurations_view (sqlite))
	goto error;
    if (!create_vector_styled_layers_view (sqlite))
	goto error;
    if (!create_raster_styled_layers_view (sqlite))
	goto error;

    if (transaction)
      {
	  /* confirming the still pending Transaction */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	      goto error;
      }
    return 1;

  error:
    return 0;
}

SPATIALITE_PRIVATE int
reCreateStylingTriggers (void *p_sqlite, int relaxed, int transaction)
{
/* (re)Creating the SE Styling triggers */
    sqlite3 *sqlite = p_sqlite;
    int ret;

    if (transaction)
      {
	  /* starting a Transaction */
	  ret = sqlite3_exec (p_sqlite, "BEGIN", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	      goto error;
      }

/* (re)creating the main RasterCoverages trigger as well */
    drop_raster_coverages_triggers (sqlite);
    if (!create_raster_coverages_triggers (sqlite))
	goto error;

#ifdef ENABLE_RTTOPO		/* only if RTTOPO enabled */
/* (re)creating both TOPOLOGIES and NETWORKS triggers */
    drop_topologies_triggers (sqlite);
    if (!do_create_topologies_triggers (sqlite))
	goto error;
    drop_networks_triggers (sqlite);
    if (!do_create_networks_triggers (sqlite))
	goto error;

/* (re)creating the main VectorCoverages table as well */
    drop_vector_coverages_triggers (sqlite);
    if (!create_vector_coverages_triggers (sqlite))
	goto error;
#endif /* end RTTOPO conditional */

    drop_styling_triggers (sqlite);
    if (!create_external_graphics_triggers (sqlite))
	goto error;
    if (!create_fonts_triggers (sqlite))
	goto error;
    if (!create_vector_styles_triggers (sqlite, relaxed))
	goto error;
    if (!create_raster_styles_triggers (sqlite, relaxed))
	goto error;
    if (!create_vector_styled_layers_triggers (sqlite))
	goto error;
    if (!create_raster_styled_layers_triggers (sqlite))
	goto error;

    if (transaction)
      {
	  /* confirming the still pending Transaction */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	      goto error;
      }
    return 1;

  error:
    return 0;
}

SPATIALITE_PRIVATE int
createStylingTables (void *p_sqlite, int relaxed)
{
/* Creating the SE Styling tables */
    return createStylingTables_ex (p_sqlite, relaxed, 0);
}

static int
check_iso_metadata_table (sqlite3 * sqlite, const char *table, int is_view)
{
/* checking if some ISO Metadata-related table/view already exists */
    int exists = 0;
    char *sql_statement;
    char *errMsg = NULL;
    int ret;
    char **results;
    int rows;
    int columns;
    int i;
    sql_statement =
	sqlite3_mprintf ("SELECT name FROM sqlite_master WHERE type = '%s'"
			 "AND Upper(name) = Upper(%Q)",
			 (!is_view) ? "table" : "view", table);
    ret =
	sqlite3_get_table (sqlite, sql_statement, &results, &rows, &columns,
			   &errMsg);
    sqlite3_free (sql_statement);
    if (ret != SQLITE_OK)
      {
	  sqlite3_free (errMsg);
	  return 0;
      }
    for (i = 1; i <= rows; i++)
	exists = 1;
    sqlite3_free_table (results);
    return exists;
}

SPATIALITE_PRIVATE int
create_iso_metadata (sqlite3 * sqlite, int relaxed)
{
/* creating the ISO_metadata table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE ISO_metadata (\n"
	"id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
	"md_scope TEXT NOT NULL DEFAULT 'dataset',\n"
	"metadata BLOB NOT NULL DEFAULT (zeroblob(4)),\n"
	"fileId TEXT,\nparentId TEXT)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'ISO_metadata' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* adding the Geometry column */
    sql =
	"SELECT AddGeometryColumn('ISO_metadata', 'geometry', 4326, 'MULTIPOLYGON', 'XY')";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      (" AddGeometryColumn 'ISO_metadata'.'geometry' error:%s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* adding a Spatial Index */
    sql = "SELECT CreateSpatialIndex ('ISO_metadata', 'geometry')";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("CreateSpatialIndex 'ISO_metadata'.'geometry' error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the ISO_metadata triggers */
    sql = "CREATE TRIGGER 'ISO_metadata_md_scope_insert'\n"
	"BEFORE INSERT ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'insert on table ISO_metadata violates constraint: "
	"md_scope must be one of ''undefined'' | ''fieldSession'' | ''collectionSession'' "
	"| ''series'' | ''dataset'' | ''featureType'' | ''feature'' | ''attributeType'' "
	"| ''attribute'' | ''tile'' | ''model'' | ''catalogue'' | ''schema'' "
	"| ''taxonomy'' | ''software'' | ''service'' | ''collectionHardware'' "
	"| ''nonGeographicDataset'' | ''dimensionGroup''')\n"
	"WHERE NOT(NEW.md_scope IN ('undefined','fieldSession','collectionSession',"
	"'series','dataset','featureType','feature','attributeType','attribute',"
	"'tile','model','catalogue','schema','taxonomy','software','service',"
	"'collectionHardware','nonGeographicDataset','dimensionGroup'));\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_md_scope_update'\n"
	"BEFORE UPDATE OF 'md_scope' ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'update on table ISO_metadata violates constraint: "
	"md_scope must be one of ''undefined'' | ''fieldSession'' | ''collectionSession'' "
	"| ''series'' | ''dataset'' | ''featureType'' | ''feature'' | ''attributeType'' |"
	" ''attribute'' | ''tile'' | ''model'' | ''catalogue'' | ''schema'' "
	"| ''taxonomy'' | ''software'' | ''service'' | ''collectionHardware'' "
	"| ''nonGeographicDataset'' | ''dimensionGroup''')\n"
	"WHERE NOT(NEW.md_scope IN ('undefined','fieldSession','collectionSession',"
	"'series','dataset','featureType','feature','attributeType','attribute',"
	"'tile','model','catalogue','schema','taxonomy','software','service',"
	"'collectionHardware','nonGeographicDataset','dimensionGroup'));\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_fileIdentifier_insert'\n"
	"AFTER INSERT ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	"UPDATE ISO_metadata SET fileId = XB_GetFileId(NEW.metadata), "
	"parentId = XB_GetParentId(NEW.metadata), "
	"geometry = XB_GetGeometry(NEW.metadata) WHERE id = NEW.id;\n"
	"UPDATE ISO_metadata_reference "
	"SET md_parent_id = GetIsoMetadataId(NEW.parentId) "
	"WHERE md_file_id = NEW.id;\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_fileIdentifier_update'\n"
	"AFTER UPDATE ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	"UPDATE ISO_metadata SET fileId = XB_GetFileId(NEW.metadata), "
	"parentId = XB_GetParentId(NEW.metadata), "
	"geometry = XB_GetGeometry(NEW.metadata) WHERE id = NEW.id;\n"
	"UPDATE ISO_metadata_reference "
	"SET md_parent_id = GetIsoMetadataId(NEW.parentId) "
	"WHERE md_file_id = NEW.id;\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (relaxed == 0)
      {
	  /* strong trigger - imposing XML schema validation */
	  sql = "CREATE TRIGGER ISO_metadata_insert\n"
	      "BEFORE INSERT ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on ISO_metadata violates constraint: "
	      "not a valid ISO Metadata XML')\n"
	      "WHERE XB_IsIsoMetadata(NEW.metadata) <> 1 AND NEW.id <> 0;\n"
	      "SELECT RAISE(ABORT,'insert on ISO_metadata violates constraint: "
	      "not an XML Schema Validated ISO Metadata')\n"
	      "WHERE XB_IsSchemaValidated(NEW.metadata) <> 1 AND NEW.id <> 0;\nEND";
      }
    else
      {
	  /* relaxed trigger - not imposing XML schema validation */
	  sql = "CREATE TRIGGER ISO_metadata_insert\n"
	      "BEFORE INSERT ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'insert on ISO_metadata violates constraint: "
	      "not a valid ISO Metadata XML')\n"
	      "WHERE XB_IsIsoMetadata(NEW.metadata) <> 1 AND NEW.id <> 0;\nEND";
      }
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    if (relaxed == 0)
      {
	  /* strong trigger - imposing XML schema validation */
	  sql = "CREATE TRIGGER ISO_metadata_update\n"
	      "BEFORE UPDATE ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on ISO_metadata violates constraint: "
	      "not a valid ISO Metadata XML')\n"
	      "WHERE XB_IsIsoMetadata(NEW.metadata) <> 1 AND NEW.id <> 0;\n"
	      "SELECT RAISE(ABORT,'update on ISO_metadata violates constraint: "
	      "not an XML Schema Validated ISO Metadata')\n"
	      "WHERE XB_IsSchemaValidated(NEW.metadata) <> 1 AND NEW.id <> 0;\nEND";
      }
    else
      {
	  /* relaxed trigger - not imposing XML schema validation */
	  sql = "CREATE TRIGGER ISO_metadata_update\n"
	      "BEFORE UPDATE ON 'ISO_metadata'\nFOR EACH ROW BEGIN\n"
	      "SELECT RAISE(ABORT,'update on ISO_metadata violates constraint: "
	      "not a valid ISO Metadata XML')\n"
	      "WHERE XB_IsIsoMetadata(NEW.metadata) <> 1 AND NEW.id <> 0;\nEND";
      }
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating any Index on ISO_metadata */
    sql = "CREATE UNIQUE INDEX idx_ISO_metadata_ids ON "
	"ISO_metadata (fileId)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("Create Index 'idx_ISO_metadata_ids' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE INDEX idx_ISO_metadata_parents ON " "ISO_metadata (parentId)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("Create Index 'idx_ISO_metadata_parents' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_iso_metadata_reference (sqlite3 * sqlite)
{
/* creating the ISO_metadata_reference table */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE TABLE ISO_metadata_reference (\n"
	"reference_scope TEXT NOT NULL DEFAULT 'table',\n"
	"table_name TEXT NOT NULL DEFAULT 'undefined',\n"
	"column_name TEXT NOT NULL DEFAULT 'undefined',\n"
	"row_id_value INTEGER NOT NULL DEFAULT 0,\n"
	"timestamp TEXT NOT NULL DEFAULT ("
	"strftime('%Y-%m-%dT%H:%M:%fZ',CURRENT_TIMESTAMP)),\n"
	"md_file_id INTEGER NOT NULL DEFAULT 0,\n"
	"md_parent_id INTEGER NOT NULL DEFAULT 0,\n"
	"CONSTRAINT fk_isometa_mfi FOREIGN KEY (md_file_id) "
	"REFERENCES ISO_metadata(id),\n"
	"CONSTRAINT fk_isometa_mpi FOREIGN KEY (md_parent_id) "
	"REFERENCES ISO_metadata(id))";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE TABLE 'ISO_metadata_reference' error: %s\n",
			err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating the ISO_metadata_reference triggers */
    sql = "CREATE TRIGGER 'ISO_metadata_reference_scope_insert'\n"
	"BEFORE INSERT ON 'ISO_metadata_reference'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'insert on table ISO_metadata_reference violates constraint: "
	"reference_scope must be one of ''table'' | ''column'' | ''row'' | ''row/col''')\n"
	"WHERE NOT NEW.reference_scope IN ('table','column','row','row/col');\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_scope_update'\n"
	"BEFORE UPDATE OF 'reference_scope' ON 'ISO_metadata_reference'\n"
	"FOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'update on table ISO_metadata_reference violates constraint: "
	"referrence_scope must be one of ''table'' | ''column'' | ''row'' | ''row/col''')\n"
	"WHERE NOT NEW.reference_scope IN ('table','column','row','row/col');\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_table_name_insert'\n"
	"BEFORE INSERT ON 'ISO_metadata_reference'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'insert on table ISO_metadata_reference violates constraint: "
	"table_name must be the name of a table in geometry_columns')\n"
	"WHERE NOT NEW.table_name IN (\n"
	"SELECT f_table_name AS table_name FROM geometry_columns);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_table_name_update'\n"
	"BEFORE UPDATE OF 'table_name' ON 'ISO_metadata_reference'\n"
	"FOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'update on table ISO_metadata_reference violates constraint: "
	"table_name must be the name of a table in geometry_columns')\n"
	"WHERE NOT NEW.table_name IN (\n"
	"SELECT f_table_name AS table_name FROM geometry_columns);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_row_id_value_insert'\n"
	"BEFORE INSERT ON 'ISO_metadata_reference'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'insert on ISO_table ISO_metadata_reference violates constraint: "
	"row_id_value must be 0 when reference_scope is ''table'' or ''column''')\n"
	"WHERE NEW.reference_scope IN ('table','column') AND NEW.row_id_value <> 0;\n"
	"SELECT RAISE(ROLLBACK, 'insert on table ISO_metadata_reference violates constraint: "
	"row_id_value must exist in specified table when reference_scope is ''row'' or ''row/col''')\n"
	"WHERE NEW.reference_scope IN ('row','row/col') AND NOT EXISTS\n"
	"(SELECT rowid FROM (SELECT NEW.table_name AS table_name) "
	"WHERE rowid = NEW.row_id_value);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_row_id_value_update'\n"
	"BEFORE UPDATE OF 'row_id_value' ON 'ISO_metadata_reference'\n"
	"FOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'update on table ISO_metadata_reference violates constraint: "
	"row_id_value must be 0 when reference_scope is ''table'' or ''column''')\n"
	"WHERE NEW.reference_scope IN ('table','column') AND NEW.row_id_value <> 0;\n"
	"SELECT RAISE(ROLLBACK, 'update on ISO_table metadata_reference violates constraint: "
	"row_id_value must exist in specified table when reference_scope is ''row'' or ''row/col''')\n"
	"WHERE NEW.reference_scope IN ('row','row/col') AND NOT EXISTS\n"
	"(SELECT rowid FROM (SELECT NEW.table_name AS table_name) "
	"WHERE rowid = NEW.row_id_value);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_timestamp_insert'\n"
	"BEFORE INSERT ON 'ISO_metadata_reference'\nFOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'insert on table ISO_metadata_reference violates constraint: "
	"timestamp must be a valid time in ISO 8601 ''yyyy-mm-ddThh:mm:ss.cccZ'' form')\n"
	"WHERE NOT (NEW.timestamp GLOB'[1-2][0-9][0-9][0-9]-[0-1][0-9]-[1-3][0-9]T"
	"[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]Z' AND strftime('%s',"
	"NEW.timestamp) NOT NULL);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE TRIGGER 'ISO_metadata_reference_timestamp_update'\n"
	"BEFORE UPDATE OF 'timestamp' ON 'ISO_metadata_reference'\n"
	"FOR EACH ROW BEGIN\n"
	"SELECT RAISE(ROLLBACK, 'update on table ISO_metadata_reference violates constraint: "
	"timestamp must be a valid time in ISO 8601 ''yyyy-mm-ddThh:mm:ss.cccZ'' form')\n"
	"WHERE NOT (NEW.timestamp GLOB'[1-2][0-9][0-9][0-9]-[0-1][0-9]-[1-3][0-9]T"
	"[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]Z' AND strftime('%s',"
	"NEW.timestamp) NOT NULL);\nEND";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("SQL error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
/* creating any Index on ISO_metadata_reference */
    sql = "CREATE INDEX idx_ISO_metadata_reference_ids ON "
	"ISO_metadata_reference (md_file_id)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("Create Index 'idx_ISO_metadata_reference_ids' error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    sql = "CREATE INDEX idx_ISO_metadata_reference_parents ON "
	"ISO_metadata_reference (md_parent_id)";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("Create Index 'idx_ISO_metadata_reference_parents' error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
create_iso_metadata_view (sqlite3 * sqlite)
{
/* creating the ISO_metadata_view view */
    char *sql;
    int ret;
    char *err_msg = NULL;
    sql = "CREATE VIEW ISO_metadata_view AS\n"
	"SELECT id AS id, md_scope AS md_scope, XB_GetTitle(metadata) AS title, "
	"XB_GetAbstract(metadata) AS abstract, geometry AS geometry, "
	"fileId AS fileIdentifier, parentId AS parentIdentifier, metadata AS metadata, "
	"XB_IsSchemaValidated(metadata) AS schema_validated, "
	"XB_GetSchemaURI(metadata) AS metadata_schema_URI\n"
	"FROM ISO_metadata";
    ret = sqlite3_exec (sqlite, sql, NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e ("CREATE VIEW 'ISO_metadata_view' error: %s\n", err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;
}

SPATIALITE_PRIVATE int
createIsoMetadataTables (void *p_sqlite, int relaxed)
{
/* Creating the ISO Metadata tables */
    const char *tables[4];
    int views[3];
    const char **p_tbl;
    int *p_view;
    int ok_table;
    int ret;
    char *err_msg = NULL;
    sqlite3 *sqlite = p_sqlite;

/* checking ISO Metadata tables */
    tables[0] = "ISO_metadata";
    tables[1] = "ISO_metadata_reference";
    tables[2] = "ISO_metadata_view";
    tables[3] = NULL;
    views[0] = 0;
    views[1] = 0;
    views[2] = 1;
    p_tbl = tables;
    p_view = views;
    while (*p_tbl != NULL)
      {
	  ok_table = check_iso_metadata_table (sqlite, *p_tbl, *p_view);
	  if (ok_table)
	    {
		spatialite_e
		    ("CreateIsoMetadataTables() error: table '%s' already exists\n",
		     *p_tbl);
		goto error;
	    }
	  p_tbl++;
	  p_view++;
      }

/* creating the ISO Metadata tables */
    if (!create_iso_metadata (sqlite, relaxed))
	goto error;
    if (!create_iso_metadata_reference (sqlite))
	goto error;
    if (!create_iso_metadata_view (sqlite))
	goto error;
/* inserting the default "undef" row into ISO_metadata */
    ret =
	sqlite3_exec (sqlite,
		      "INSERT INTO ISO_metadata (id, md_scope) VALUES (0, 'undefined')",
		      NULL, NULL, &err_msg);
    if (ret != SQLITE_OK)
      {
	  spatialite_e
	      ("Insert default 'undefined' ISO_metadata row - error: %s\n",
	       err_msg);
	  sqlite3_free (err_msg);
	  return 0;
      }
    return 1;

  error:
    return 0;
}

#endif /* end including LIBXML2 */

static int
do_check_if_table_exists (sqlite3 * sqlite, const char *table)
{
/* checking if a Table is already defined */
    int ret;
    int i;
    char **results;
    int rows;
    int columns;
    int count = 0;
    char *qtable = gaiaDoubleQuotedSql (table);
    char *sql = sqlite3_mprintf ("PRAGMA table_info(\"%s\")", qtable);
    free (qtable);
    ret = sqlite3_get_table (sqlite, sql, &results, &rows, &columns, NULL);
    sqlite3_free (sql);
    if (ret != SQLITE_OK)
	return 0;
    for (i = 1; i <= rows; i++)
	count++;
    sqlite3_free_table (results);
    if (count == 0)
	return 0;
    return 1;
}

SPATIALITE_PRIVATE int
createMissingSystemTables (sqlite3 * sqlite, const void *cache, int relaxed,
			   int transaction, char **err_msg)
{
/* attempting to create all missing System Tables required by version 5 */
#ifndef ENABLE_LIBXML2		/* unsupported LIBXML2 */
    *err_msg =
	sqlite3_mprintf
	("this build does not support LIBXML2 ... cowardly quitting");
    return 0;
#endif
#ifndef ENABLE_RTTOPO		/* unsipported RTTOPO */
    *err_msg =
	sqlite3_mprintf
	("this build does not support RTTOPO ... cowardly quitting");
    return 0;
#endif

#ifdef ENABLE_LIBXML2		/* only if LibXML2 support is available */
#ifdef ENABLE_RTTOPO		/* RTTOPO is supported */
    int ret;
    struct str_tables
    {
	const char *table_name;
	int (*creator) (sqlite3 * sqlite);
	int (*creator_void) (void *sqlite);
	int (*creator_relaxed) (sqlite3 * sqlite, int relaxed);
	int (*creator_cache) (sqlite3 * sqlite, const void *cache);
    };
    static struct str_tables tables[] = {
	{"data_licenses", create_data_licenses, NULL, NULL, NULL},
	{"raster_coverages", create_raster_coverages, NULL, NULL, NULL},
	{"raster_coverages_keyword", create_raster_coverages, NULL, NULL, NULL},
	{"raster_coverages_srid", create_raster_coverages, NULL, NULL, NULL},
	{"raster_coverages_ref_sys", create_raster_coverages, NULL, NULL, NULL},
	{"vector_coverages", create_vector_coverages, NULL, NULL, NULL},
	{"vector_coverages_keyword", create_vector_coverages, NULL, NULL, NULL},
	{"vector_coverages_srid", create_vector_coverages, NULL, NULL, NULL},
	{"vector_coverages_ref_sys", create_vector_coverages, NULL, NULL, NULL},
	{"wms_getcapabilities", create_wms_tables, NULL, NULL, NULL},
	{"wms_getmap", create_wms_tables, NULL, NULL, NULL},
	{"wms_ref_sys", create_wms_tables, NULL, NULL, NULL},
	{"wms_settings", create_wms_tables, NULL, NULL, NULL},
	{"topologies", NULL, do_create_topologies, NULL, NULL},
	{"networks", NULL, do_create_networks, NULL, NULL},
	{"SE_external_graphics", create_external_graphics, NULL, NULL, NULL},
	{"SE_external_graphics_view", create_external_graphics_view, NULL, NULL,
	 NULL},
	{"SE_fonts", create_fonts, NULL, NULL, NULL},
	{"SE_fonts_view", create_fonts_view, NULL, NULL, NULL},
	{"SE_raster_styles", NULL, NULL, create_raster_styles, NULL},
	{"SE_raster_styles_view", create_raster_styles_view, NULL, NULL, NULL},
	{"SE_raster_styled_layers", create_raster_styled_layers, NULL, NULL,
	 NULL},
	{"SE_raster_styled_layers_view", create_raster_styled_layers_view, NULL,
	 NULL, NULL},
	{"SE_vector_styles", NULL, NULL, create_vector_styles, NULL},
	{"SE_vector_styles_view", create_vector_styles_view, NULL, NULL, NULL},
	{"SE_vector_styled_layers", create_vector_styled_layers, NULL, NULL,
	 NULL},
	{"SE_vector_styled_layers_view", create_vector_styled_layers_view, NULL,
	 NULL, NULL},
	{"ISO_metadata", NULL, NULL, create_iso_metadata, NULL},
	{"ISO_metadata_reference", create_iso_metadata_reference, NULL, NULL,
	 NULL},
	{"ISO_metadata_view", create_iso_metadata_view, NULL, NULL, NULL},
	{"stored_procedures", NULL, NULL, NULL, gaia_stored_proc_create_tables},
	{"stored_variables", NULL, NULL, NULL, gaia_stored_proc_create_tables},
	{"rl2map_configurations", NULL, NULL, create_rl2map_configurations,
	 NULL},
	{"rl2map_configurations_view", create_rl2map_configurations_view, NULL,
	 NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL}
    };
    struct str_tables *p_table = tables;

    if (transaction)
      {
	  /* starting a Transaction */
	  ret = sqlite3_exec (sqlite, "BEGIN", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	    {
		*err_msg =
		    sqlite3_mprintf ("Unable to start a Transaction (BEGIN)");
		return 0;
	    }
      }

    while (p_table->table_name != NULL)
      {
	  int exists = do_check_if_table_exists (sqlite, p_table->table_name);
	  if (!exists)
	    {
		if (p_table->creator != NULL)
		    ret = p_table->creator (sqlite);
		if (p_table->creator_void != NULL)
		    ret = p_table->creator_void ((void *) sqlite);
		if (p_table->creator_relaxed != NULL)
		    ret = p_table->creator_relaxed (sqlite, relaxed);
		if (p_table->creator_cache != NULL)
		    ret = p_table->creator_cache (sqlite, cache);
		if (!ret)
		  {
		      *err_msg =
			  sqlite3_mprintf ("Unable to create \"%s\"",
					   p_table->table_name);
		      return 0;
		  }
	    }
	  if (strcmp (p_table->table_name, "SE_external_graphics") == 0)
	    {
		/* autoregistering all Standard Brushes */
		ret =
		    sqlite3_exec (sqlite,
				  "SELECT SE_AutoRegisterStandardBrushes()",
				  NULL, NULL, NULL);
		if (ret != SQLITE_OK)
		  {
		      *err_msg =
			  sqlite3_mprintf
			  ("Unexpected failure when registering Standard Brushes");
		      return 0;
		  }
	    }
	  p_table++;
      }

    if (transaction)
      {
	  /* confirming the still pending Transaction */
	  ret = sqlite3_exec (sqlite, "COMMIT", NULL, NULL, NULL);
	  if (ret != SQLITE_OK)
	    {
		*err_msg =
		    sqlite3_mprintf
		    ("Unable to confirm a Transaction (COMMIT)");
		return 0;
	    }
      }

/* full success */
    *err_msg = NULL;
    return 1;
#endif /* end RTTOPO */
#endif /* end LIBXML2 */
}
