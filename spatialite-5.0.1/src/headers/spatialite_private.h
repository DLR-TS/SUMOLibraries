/* 
 spatialite.h -- Gaia spatial support for SQLite 
  
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

#include <time.h>
#include <stdarg.h>

#include <zlib.h>

#include "spatialite/gg_sequence.h"
#include "spatialite/sqlite.h"

/**
 \file spatialite_private.h

 SpatiaLite private header file
 */
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef _WIN32
#ifdef DLL_EXPORT
#define SPATIALITE_PRIVATE
#else
#define SPATIALITE_PRIVATE
#endif
#else
#define SPATIALITE_PRIVATE __attribute__ ((visibility("hidden")))
#endif
#endif

#ifndef _SPATIALITE_PRIVATE_H
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _SPATIALITE_PRIVATE_H
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/** spatial_ref_sys_init2: will create the "spatial_ref_sys" table
 and will populate this table with any supported EPSG SRID definition */
#define GAIA_EPSG_ANY -9999
/** spatial_ref_sys_init2: will create the "spatial_ref_sys" table
 and will populate this table only inserting WGS84-related definitions */
#define GAIA_EPSG_WGS84_ONLY -9998
/** spatial_ref_sys_init2: will create the "spatial_ref_sys" table
 but will avoid to insert any row at all */
#define GAIA_EPSG_NONE -9997

#define SPATIALITE_STATISTICS_GENUINE	1
#define SPATIALITE_STATISTICS_VIEWS	2
#define SPATIALITE_STATISTICS_VIRTS	3
#define SPATIALITE_STATISTICS_LEGACY	4

#define SPATIALITE_CACHE_MAGIC1	0xf8
#define SPATIALITE_CACHE_MAGIC2 0x8f

    struct vxpath_ns
    {
	/* a Namespace definition */
	char *Prefix;
	char *Href;
	struct vxpath_ns *Next;
    };

    struct vxpath_namespaces
    {
	/* Namespace container */
	struct vxpath_ns *First;
	struct vxpath_ns *Last;
    };

    struct splite_geos_cache_item
    {
	unsigned char gaiaBlob[64];
	int gaiaBlobSize;
	uLong crc32;
	void *geosGeom;
	void *preparedGeosGeom;
    };

    struct splite_xmlSchema_cache_item
    {
	time_t timestamp;
	char *schemaURI;
	void *schemaDoc;
	void *parserCtxt;
	void *schema;
    };

    struct splite_savepoint
    {
	char *savepoint_name;
	struct splite_savepoint *prev;
	struct splite_savepoint *next;
    };

    struct splite_vtable_extent
    {
	char *table;
	double minx;
	double maxx;
	double miny;
	double maxy;
	int srid;
	struct splite_vtable_extent *prev;
	struct splite_vtable_extent *next;
    };

    struct gaia_variant_value
    {
	/* a struct/union intended to store a SQLite Variant Value */
	int dataType;		/* one of SQLITE_NULL, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT or SQLITE_BLOB */
	sqlite3_int64 intValue;
	double dblValue;
	char *textValue;
	unsigned char *blobValue;
	int size;
    };

#define MAX_XMLSCHEMA_CACHE	16

    struct splite_internal_cache
    {
	unsigned char magic1;
	int gpkg_mode;
	int gpkg_amphibious_mode;
	int decimal_precision;
	void *GEOS_handle;
	void *PROJ_handle;
	void *RTTOPO_handle;
	void *xmlParsingErrors;
	void *xmlSchemaValidationErrors;
	void *xmlXPathErrors;
	char *cutterMessage;
	char *storedProcError;
	char *createRoutingError;
	struct splite_geos_cache_item cacheItem1;
	struct splite_geos_cache_item cacheItem2;
	struct splite_xmlSchema_cache_item xmlSchemaCache[MAX_XMLSCHEMA_CACHE];
	int pool_index;
	void (*geos_warning) (const char *fmt, ...);
	void (*geos_error) (const char *fmt, ...);
	char *gaia_geos_error_msg;
	char *gaia_geos_warning_msg;
	char *gaia_geosaux_error_msg;
	char *gaia_rttopo_error_msg;
	char *gaia_rttopo_warning_msg;
	char *gaia_proj_error_msg;
	int silent_mode;
	void *firstTopology;
	void *lastTopology;
	void *firstNetwork;
	void *lastNetwork;
	unsigned int next_topo_savepoint;
	struct splite_savepoint *first_topo_svpt;
	struct splite_savepoint *last_topo_svpt;
	unsigned int next_network_savepoint;
	struct splite_savepoint *first_net_svpt;
	struct splite_savepoint *last_net_svpt;
	gaiaSequencePtr first_seq;
	gaiaSequencePtr last_seq;
	struct splite_vtable_extent *first_vtable_extent;
	struct splite_vtable_extent *last_vtable_extent;
	int ok_last_used_sequence;
	int last_used_sequence_val;
	char *SqlProcLogfile;
	int SqlProcLogfileAppend;
	FILE *SqlProcLog;
	int SqlProcContinue;
	struct gaia_variant_value *SqlProcRetValue;
	int tinyPointEnabled;
	unsigned char magic2;
	char *lastPostgreSqlError;
	int buffer_end_cap_style;
	int buffer_join_style;
	double buffer_mitre_limit;
	int buffer_quadrant_segments;
	int proj6_cached;
	void *proj6_cached_pj;
	char *proj6_cached_string_1;
	char *proj6_cached_string_2;
	void *proj6_cached_area;
	int is_pause_enabled;
    };

    struct epsg_defs
    {
	int srid;
	char *auth_name;
	int auth_srid;
	char *ref_sys_name;
	char *proj4text;
	char *srs_wkt;
	int is_geographic;
	int flipped_axes;
	char *spheroid;
	char *prime_meridian;
	char *datum;
	char *projection;
	char *unit;
	char *axis_1;
	char *orientation_1;
	char *axis_2;
	char *orientation_2;
	struct epsg_defs *next;
    };

    struct gaia_control_points
    {
	/* a struct to implement ATM_ControlPoints - aggregate function */
	int count;
	int allocation_incr;
	int allocated_items;
	int has3d;
	int tps;
	int order;
	/* point set A */
	double *x0;
	double *y0;
	double *z0;
	/* point set B */
	double *x1;
	double *y1;
	double *z1;
	/* affine transform coefficients */
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double h;
	double i;
	double xoff;
	double yoff;
	double zoff;
	int affine_valid;
    };

    SPATIALITE_PRIVATE void free_internal_cache (struct splite_internal_cache
						 *cache);

    SPATIALITE_PRIVATE void free_internal_cache_topologies (void *first);

    SPATIALITE_PRIVATE void free_internal_cache_networks (void *first);

    SPATIALITE_PRIVATE struct epsg_defs *add_epsg_def (int filter_srid,
						       struct epsg_defs
						       **first,
						       struct epsg_defs
						       **last, int srid,
						       const char *auth_name,
						       int auth_srid,
						       const char
						       *ref_sys_name);

    SPATIALITE_PRIVATE struct epsg_defs *add_epsg_def_ex (int filter_srid,
							  struct epsg_defs
							  **first,
							  struct epsg_defs
							  **last, int srid,
							  const char
							  *auth_name,
							  int auth_srid,
							  const char
							  *ref_sys_name,
							  int is_geographic,
							  int flipped_axes,
							  const char
							  *spheroid,
							  const char
							  *prime_meridian,
							  const char *datum,
							  const char
							  *projection,
							  const char *unit,
							  const char *axis_1,
							  const char
							  *orientation_1,
							  const char *axis_2,
							  const char
							  *orientation_2);

    SPATIALITE_PRIVATE void add_proj4text (struct epsg_defs *p, int count,
					   const char *text);

    SPATIALITE_PRIVATE void add_srs_wkt (struct epsg_defs *p, int count,
					 const char *text);

    SPATIALITE_PRIVATE void initialize_epsg (int filter,
					     struct epsg_defs **first,
					     struct epsg_defs **last);

    SPATIALITE_PRIVATE void free_epsg (struct epsg_defs *first);

    SPATIALITE_PRIVATE int exists_spatial_ref_sys (void *handle);

    SPATIALITE_PRIVATE int checkSpatialMetaData (const void *sqlite);

    SPATIALITE_PRIVATE int checkSpatialMetaData_ex (const void *sqlite,
						    const char *db_prefix);

    SPATIALITE_PRIVATE int delaunay_triangle_check (void *pg);

    SPATIALITE_PRIVATE void *voronoj_build (int pgs, void *first,
					    double extra_frame_size);

    SPATIALITE_PRIVATE void *voronoj_build_r (const void *p_cache, int pgs,
					      void *first,
					      double extra_frame_size);

    SPATIALITE_PRIVATE void *voronoj_export (void *voronoj, void *result,
					     int only_edges);

    SPATIALITE_PRIVATE void *voronoj_export_r (const void *p_cache,
					       void *voronoj, void *result,
					       int only_edges);

    SPATIALITE_PRIVATE void voronoj_free (void *voronoj);

    SPATIALITE_PRIVATE void *concave_hull_build (void *first,
						 int dimension_model,
						 double factor,
						 int allow_holes);

    SPATIALITE_PRIVATE void *concave_hull_build_r (const void *p_cache,
						   void *first,
						   int dimension_model,
						   double factor,
						   int allow_holes);

    SPATIALITE_PRIVATE int createAdvancedMetaData (void *sqlite);

    SPATIALITE_PRIVATE void updateSpatiaLiteHistory (void *sqlite,
						     const char *table,
						     const char *geom,
						     const char *operation);

    SPATIALITE_PRIVATE int createGeometryColumns (void *p_sqlite);

    SPATIALITE_PRIVATE int createTemporarySpatialRefSys (void *p_sqlite,
							 const char *db_prefix);

    SPATIALITE_PRIVATE int createTemporaryGeometryColumns (void *p_sqlite,
							   const char
							   *db_prefix);

    SPATIALITE_PRIVATE int check_layer_statistics (void *p_sqlite);

    SPATIALITE_PRIVATE int check_views_layer_statistics (void *p_sqlite);

    SPATIALITE_PRIVATE int check_virts_layer_statistics (void *p_sqlite);

    SPATIALITE_PRIVATE void updateGeometryTriggers (void *p_sqlite,
						    const char *table,
						    const char *column);

    SPATIALITE_PRIVATE void updateTemporaryGeometryTriggers (void *p_sqlite,
							     const char
							     *db_prefix,
							     const char *table,
							     const char
							     *column);

    SPATIALITE_PRIVATE int upgradeGeometryTriggers (void *p_sqlite);

    SPATIALITE_PRIVATE int getRealSQLnames (void *p_sqlite, const char *table,
					    const char *column,
					    char **real_table,
					    char **real_column);

    SPATIALITE_PRIVATE int getRealSQLnamesTemporary (void *p_sqlite,
						     const char *db_prefix,
						     const char *table,
						     const char *column,
						     char **real_table,
						     char **real_column);

    /* DEPRECATED - always use buildSpatialIndexEx */
    SPATIALITE_PRIVATE void buildSpatialIndex (void *p_sqlite,
					       const unsigned char *table,
					       const char *column);

    SPATIALITE_PRIVATE int buildSpatialIndexEx (void *p_sqlite,
						const unsigned char *table,
						const char *column);

    SPATIALITE_PRIVATE int buildTemporarySpatialIndex (void *p_sqlite,
						       const char *db_prefix,
						       const unsigned char
						       *table,
						       const char *column);

    SPATIALITE_PRIVATE int validateRowid (void *p_sqlite, const char *table);

    SPATIALITE_PRIVATE int validateTemporaryRowid (void *p_sqlite,
						   const char *db_prefix,
						   const char *table);

    SPATIALITE_PRIVATE int doComputeFieldInfos (void *p_sqlite,
						const char *table,
						const char *column,
						int stat_type, void *p_lyr);

    SPATIALITE_PRIVATE void getProjParams (void *p_sqlite, int srid,
					   char **params);

    SPATIALITE_PRIVATE void getProjWkt (void *p_sqlite, int srid, char **wkt);

    SPATIALITE_PRIVATE void getProjAuthNameSrid (void *p_sqlite, int srid,
						 char **auth_name_srid);

    SPATIALITE_PRIVATE int getEllipsoidParams (void *p_sqlite, int srid,
					       double *a, double *b,
					       double *rf);

    SPATIALITE_PRIVATE void addVectorLayer (void *list,
					    const char *layer_type,
					    const char *table_name,
					    const char *geometry_column,
					    int geometry_type, int srid,
					    int spatial_index);

    SPATIALITE_PRIVATE void addVectorLayerExtent (void *list,
						  const char *table_name,
						  const char *geometry_column,
						  int count, double min_x,
						  double min_y, double max_x,
						  double max_y);

    SPATIALITE_PRIVATE void addLayerAttributeField (void *list,
						    const char *table_name,
						    const char
						    *geometry_column,
						    int ordinal,
						    const char *column_name,
						    int null_values,
						    int integer_values,
						    int double_values,
						    int text_values,
						    int blob_values,
						    int null_max_size,
						    int max_size,
						    int null_int_range,
						    void *integer_min,
						    void *integer_max,
						    int null_double_range,
						    double double_min,
						    double double_max);

    SPATIALITE_PRIVATE int createStylingTables (void *p_sqlite, int relaxed);
    SPATIALITE_PRIVATE int createStylingTables_ex (void *p_sqlite,
						   int relaxed,
						   int transaction);

    SPATIALITE_PRIVATE int reCreateStylingTriggers (void *p_sqlite,
						    int relaxed,
						    int transaction);

    SPATIALITE_PRIVATE int register_external_graphic (void *p_sqlite,
						      const char *xlink_href,
						      const unsigned char
						      *p_blob, int n_bytes,
						      const char *title,
						      const char *abstract,
						      const char *file_name);

    SPATIALITE_PRIVATE int unregister_external_graphic (void *p_sqlite,
							const char *xlink_href);

    SPATIALITE_PRIVATE int register_vector_style (void *p_sqlite,
						  const unsigned char *p_blob,
						  int n_bytes);

    SPATIALITE_PRIVATE int unregister_vector_style (void *p_sqlite,
						    int style_id,
						    const char *style_name,
						    int remove_all);

    SPATIALITE_PRIVATE int reload_vector_style (void *p_sqlite, int style_id,
						const char *style_name,
						const unsigned char *p_blob,
						int n_bytes);

    /* DEPRECATED - always use register_vector_styled_layer_ex */
    SPATIALITE_PRIVATE int register_vector_styled_layer (void *p_sqlite,
							 const char
							 *f_table_name,
							 const char
							 *f_geometry_column,
							 int style_id,
							 const unsigned char
							 *p_blob, int n_bytes);

    SPATIALITE_PRIVATE int register_vector_styled_layer_ex (void *p_sqlite,
							    const char
							    *coverage_name,
							    int style_id,
							    const char
							    *style_name);

    SPATIALITE_PRIVATE int unregister_vector_styled_layer (void *p_sqlite,
							   const char
							   *coverage_name,
							   int style_id,
							   const char
							   *style_name);

    SPATIALITE_PRIVATE int register_raster_style (void *p_sqlite,
						  const unsigned char *p_blob,
						  int n_bytes);

    SPATIALITE_PRIVATE int unregister_raster_style (void *p_sqlite,
						    int style_id,
						    const char *style_name,
						    int remove_all);

    SPATIALITE_PRIVATE int reload_raster_style (void *p_sqlite, int style_id,
						const char *style_name,
						const unsigned char *p_blob,
						int n_bytes);

    /* DEPRECATED - always use register_raster_styled_layer_ex */
    SPATIALITE_PRIVATE int register_raster_styled_layer (void *p_sqlite,
							 const char
							 *coverage_name,
							 int style_id,
							 const unsigned char
							 *p_blob, int n_bytes);

    SPATIALITE_PRIVATE int register_raster_styled_layer_ex (void *p_sqlite,
							    const char
							    *coverage_name,
							    int style_id,
							    const char
							    *style_name);

    SPATIALITE_PRIVATE int unregister_raster_styled_layer (void *p_sqlite,
							   const char
							   *coverage_name,
							   int style_id,
							   const char
							   *style_name);

    SPATIALITE_PRIVATE int register_raster_coverage_srid (void *p_sqlite,
							  const char
							  *coverage_name,
							  int srid);

    SPATIALITE_PRIVATE int unregister_raster_coverage_srid (void *p_sqlite,
							    const char
							    *coverage_name,
							    int srid);

    SPATIALITE_PRIVATE int register_raster_coverage_keyword (void *p_sqlite,
							     const char
							     *coverage_name,
							     const char
							     *keyword);

    SPATIALITE_PRIVATE int unregister_raster_coverage_keyword (void *p_sqlite,
							       const char
							       *coverage_name,
							       const char
							       *keyword);

    SPATIALITE_PRIVATE int update_raster_coverage_extent (void *p_sqlite,
							  const void *cache,
							  const char
							  *coverage_name,
							  int transaction);

    SPATIALITE_PRIVATE int register_map_configuration (void *p_sqlite,
						       const unsigned char
						       *p_blob, int n_bytes);

    SPATIALITE_PRIVATE int unregister_map_configuration (void *p_sqlite,
							 int id,
							 const char *name);

    SPATIALITE_PRIVATE int reload_map_configuration (void *p_sqlite, int id,
						     const char *name,
						     const unsigned char
						     *p_blob, int n_bytes);

    SPATIALITE_PRIVATE int count_map_configurations (void *p_sqlite);

    SPATIALITE_PRIVATE char *get_map_configuration_name (void *p_sqlite,
							 int ind);

    SPATIALITE_PRIVATE char *get_map_configuration_title (void *p_sqlite,
							  int ind);

    SPATIALITE_PRIVATE char *get_map_configuration_abstract (void *p_sqlite,
							     int ind);

    SPATIALITE_PRIVATE int createIsoMetadataTables (void *p_sqlite,
						    int relaxed);

    SPATIALITE_PRIVATE int get_iso_metadata_id (void *p_sqlite,
						const char *fileIdentifier,
						void *p_id);

    SPATIALITE_PRIVATE int register_iso_metadata (void *p_sqlite,
						  const char *scope,
						  const unsigned char *p_blob,
						  int n_bytes, void *p_id,
						  const char *fileIdentifier);

    SPATIALITE_PRIVATE int createRasterCoveragesTable (void *p_sqlite);
    SPATIALITE_PRIVATE int reCreateRasterCoveragesTriggers (void *p_sqlite);

    SPATIALITE_PRIVATE int checkPopulatedCoverage (void *p_sqlite,
						   const char *db_prefix,
						   const char *coverage_name);

    SPATIALITE_PRIVATE int createVectorCoveragesTable (void *p_sqlite);
    SPATIALITE_PRIVATE int reCreateVectorCoveragesTriggers (void *p_sqlite);

    SPATIALITE_PRIVATE int register_vector_coverage (void *p_sqlite,
						     const char
						     *coverage_name,
						     const char *f_table_name,
						     const char
						     *f_geometry_column,
						     const char *title,
						     const char *abstract,
						     int is_queryable,
						     int is_editable);

    SPATIALITE_PRIVATE int register_spatial_view_coverage (void *p_sqlite,
							   const char
							   *coverage_name,
							   const char
							   *view_name,
							   const char
							   *view_geometry,
							   const char *title,
							   const char *abstract,
							   int is_queryable,
							   int is_editable);

    SPATIALITE_PRIVATE int register_virtual_table_coverage (void *p_sqlite,
							    const char
							    *coverage_name,
							    const char
							    *virt_name,
							    const char
							    *virt_geometry,
							    const char *title,
							    const char
							    *abstract,
							    int is_queryable);

    SPATIALITE_PRIVATE int register_topogeo_coverage (void *p_sqlite,
						      const char
						      *coverage_name,
						      const char *topogeo_name,
						      const char *title,
						      const char *abstract,
						      int is_queryable,
						      int is_editable);

    SPATIALITE_PRIVATE int register_toponet_coverage (void *p_sqlite,
						      const char
						      *coverage_name,
						      const char *toponet_name,
						      const char *title,
						      const char *abstract,
						      int is_queryable,
						      int is_editable);

    SPATIALITE_PRIVATE int unregister_vector_coverage (void *p_sqlite,
						       const char
						       *coverage_name);

    SPATIALITE_PRIVATE int set_vector_coverage_infos (void *p_sqlite,
						      const char
						      *coverage_name,
						      const char *title,
						      const char *abstract,
						      int is_queryable,
						      int is_editable);

    SPATIALITE_PRIVATE int set_vector_coverage_copyright (void *p_sqlite,
							  const char
							  *coverage_name,
							  const char *copyright,
							  const char *license);

    SPATIALITE_PRIVATE int register_vector_coverage_srid (void *p_sqlite,
							  const char
							  *coverage_name,
							  int srid);

    SPATIALITE_PRIVATE int unregister_vector_coverage_srid (void *p_sqlite,
							    const char
							    *coverage_name,
							    int srid);

    SPATIALITE_PRIVATE int register_vector_coverage_keyword (void *p_sqlite,
							     const char
							     *coverage_name,
							     const char
							     *keyword);

    SPATIALITE_PRIVATE int unregister_vector_coverage_keyword (void *p_sqlite,
							       const char
							       *coverage_name,
							       const char
							       *keyword);

    SPATIALITE_PRIVATE int update_vector_coverage_extent (void *p_sqlite,
							  const void *cache,
							  const char
							  *coverage_name,
							  int transaction);

    SPATIALITE_PRIVATE int createWMSTables (void *p_sqlite);

    SPATIALITE_PRIVATE int register_wms_getcapabilities (void *p_sqlite,
							 const char *url,
							 const char *title,
							 const char *abstract);

    SPATIALITE_PRIVATE int unregister_wms_getcapabilities (void *p_sqlite,
							   const char *url);

    SPATIALITE_PRIVATE int set_wms_getcapabilities_infos (void *p_sqlite,
							  const char *url,
							  const char *title,
							  const char *abstract);

    SPATIALITE_PRIVATE int register_wms_getmap (void *p_sqlite,
						const char *getcapabilities_url,
						const char *getmap_url,
						const char *layer_name,
						const char *title,
						const char *abstract,
						const char *version,
						const char *ref_sys,
						const char *image_format,
						const char *style,
						int transparent,
						int flip_axes,
						int tiled,
						int cached,
						int tile_width,
						int tile_height,
						const char *bgcolor,
						int is_queryable,
						const char *getfeatureinfo_url);

    SPATIALITE_PRIVATE int unregister_wms_getmap (void *p_sqlite,
						  const char *url,
						  const char *layer_name);

    SPATIALITE_PRIVATE int set_wms_getmap_infos (void *p_sqlite,
						 const char *url,
						 const char *layer_name,
						 const char *title,
						 const char *abstract);

    SPATIALITE_PRIVATE int set_wms_getmap_copyright (void *p_sqlite,
						     const char *url,
						     const char *layer_name,
						     const char *copyright,
						     const char *license);

    SPATIALITE_PRIVATE int set_wms_getmap_bgcolor (void *p_sqlite,
						   const char *url,
						   const char *layer_name,
						   const char *bgcolor);

    SPATIALITE_PRIVATE int set_wms_getmap_queryable (void *p_sqlite,
						     const char *url,
						     const char *layer_name,
						     int is_queryable,
						     const char
						     *getfeatureifo_url);

    SPATIALITE_PRIVATE int set_wms_getmap_options (void *p_sqlite,
						   const char *url,
						   const char *layer_name,
						   int transparent,
						   int flip_axes);

    SPATIALITE_PRIVATE int set_wms_getmap_tiled (void *p_sqlite,
						 const char *url,
						 const char *layer_name,
						 int tiled, int cached,
						 int tile_width,
						 int tile_height);

    SPATIALITE_PRIVATE int register_wms_setting (void *p_sqlite,
						 const char *url,
						 const char *layer_name,
						 const char *key,
						 const char *value,
						 int is_default);

    SPATIALITE_PRIVATE int unregister_wms_setting (void *p_sqlite,
						   const char *url,
						   const char *layer_name,
						   const char *key,
						   const char *value);

    SPATIALITE_PRIVATE int set_wms_default_setting (void *p_sqlite,
						    const char *url,
						    const char *layer_name,
						    const char *key,
						    const char *value);

    SPATIALITE_PRIVATE int register_wms_srs (void *p_sqlite,
					     const char *url,
					     const char *layer_name,
					     const char *ref_sys, double minx,
					     double miny, double maxx,
					     double maxy, int is_default);

    SPATIALITE_PRIVATE int unregister_wms_srs (void *p_sqlite,
					       const char *url,
					       const char *layer_name,
					       const char *ref_sys);

    SPATIALITE_PRIVATE int set_wms_default_srs (void *p_sqlite,
						const char *url,
						const char *layer_name,
						const char *ref_sys);

    SPATIALITE_PRIVATE char *wms_getmap_request_url (void *p_sqlite,
						     const char *getmap_url,
						     const char *layer_name,
						     int width, int height,
						     double minx, double miny,
						     double maxx, double maxy);

    SPATIALITE_PRIVATE char *wms_getfeatureinfo_request_url (void *p_sqlite,
							     const char
							     *getmap_url,
							     const char
							     *layer_name,
							     int width,
							     int height, int x,
							     int y, double minx,
							     double miny,
							     double maxx,
							     double maxy,
							     int feature_count);

    SPATIALITE_PRIVATE int register_data_license (void *p_sqlite,
						  const char *license_name,
						  const char *url);

    SPATIALITE_PRIVATE int unregister_data_license (void *p_sqlite,
						    const char *license_name);

    SPATIALITE_PRIVATE int rename_data_license (void *p_sqlite,
						const char *old_name,
						const char *new_name);

    SPATIALITE_PRIVATE int set_data_license_url (void *p_sqlite,
						 const char *license_name,
						 const char *url);

    SPATIALITE_PRIVATE const char *splite_rttopo_version (void);

    SPATIALITE_PRIVATE void splite_free_geos_cache_item (struct
							 splite_geos_cache_item
							 *p);

    SPATIALITE_PRIVATE void splite_free_geos_cache_item_r (const void
							   *p_cache,
							   struct
							   splite_geos_cache_item
							   *p);

    SPATIALITE_PRIVATE void splite_free_xml_schema_cache_item (struct
							       splite_xmlSchema_cache_item
							       *p);

    SPATIALITE_PRIVATE void vxpath_free_namespaces (struct vxpath_namespaces
						    *ns_list);

    SPATIALITE_PRIVATE struct vxpath_namespaces *vxpath_get_namespaces (void
									*p_xml_doc);

    SPATIALITE_PRIVATE int vxpath_eval_expr (const void *p_cache,
					     void *xml_doc,
					     const char *xpath_expr,
					     void *p_xpathCtx,
					     void *p_xpathObj);

    SPATIALITE_PRIVATE void *register_spatialite_sql_functions (void *db,
								const void
								*cache);

    SPATIALITE_PRIVATE void init_spatialite_virtualtables (void *p_db,
							   const void *p_cache);

    SPATIALITE_PRIVATE void spatialite_splash_screen (int verbose);

    SPATIALITE_PRIVATE void geos_error (const char *fmt, ...);

    SPATIALITE_PRIVATE void geos_warning (const char *fmt, ...);

    SPATIALITE_PRIVATE void splite_cache_semaphore_lock (void);

    SPATIALITE_PRIVATE void splite_cache_semaphore_unlock (void);

    SPATIALITE_PRIVATE const void *gaiaAuxClonerCreate (const void *sqlite,
							const char *db_prefix,
							const char *in_table,
							const char *out_table);

    SPATIALITE_PRIVATE const void *gaiaAuxClonerCreateEx (const void *sqlite,
							  const char
							  *db_prefix,
							  const char
							  *in_table,
							  const char
							  *out_table,
							  int create_only);

    SPATIALITE_PRIVATE void gaiaAuxClonerDestroy (const void *cloner);

    SPATIALITE_PRIVATE void gaiaAuxClonerAddOption (const void *cloner,
						    const char *option);

    SPATIALITE_PRIVATE int gaiaAuxClonerCheckValidTarget (const void *cloner);

    SPATIALITE_PRIVATE int gaiaAuxClonerExecute (const void *cloner);

    SPATIALITE_PRIVATE const void *gaiaElemGeomOptionsCreate ();

    SPATIALITE_PRIVATE void gaiaElemGeomOptionsAdd (const void *options,
						    const char *option);

    SPATIALITE_PRIVATE void gaiaElemGeomOptionsDestroy (const void *options);

    SPATIALITE_PRIVATE int gaia_matrix_to_arrays (const unsigned char *blob,
						  int blob_sz, double *E,
						  double *N, double *Z);

/* Topology SQL functions */
    SPATIALITE_PRIVATE void *fromRTGeom (const void *ctx, const void *rtgeom,
					 const int dimension_model,
					 const int declared_type);

    SPATIALITE_PRIVATE void *toRTGeom (const void *ctx, const void *gaia);

    SPATIALITE_PRIVATE void fnctaux_GetLastTopologyException (const void
							      *context,
							      int argc,
							      const void *argv);

    SPATIALITE_PRIVATE void fnctaux_CreateTopoTables (const void *context,
						      int argc,
						      const void *argv);
    SPATIALITE_PRIVATE void fnctaux_ReCreateTopoTriggers (const void *context,
							  int argc,
							  const void *argv);

    SPATIALITE_PRIVATE int do_create_topologies (void *sqlite_handle);
    SPATIALITE_PRIVATE void drop_topologies_triggers (void *sqlite_handle);
    SPATIALITE_PRIVATE int do_create_topologies_triggers (void *sqlite_handle);

    SPATIALITE_PRIVATE int do_create_networks (void *sqlite_handle);
    SPATIALITE_PRIVATE void drop_networks_triggers (void *sqlite_handle);
    SPATIALITE_PRIVATE int do_create_networks_triggers (void *sqlite_handle);

    SPATIALITE_PRIVATE void fnctaux_CreateTopology (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_DropTopology (const void *context,
						  int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddIsoNode (const void *context, int argc,
						const void *argv);

    SPATIALITE_PRIVATE void fnctaux_MoveIsoNode (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemIsoNode (const void *context,
						int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddIsoEdge (const void *context, int argc,
						const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddEdgeModFace (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddEdgeNewFaces (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemEdgeNewFace (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemEdgeModFace (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ChangeEdgeGeom (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemIsoEdge (const void *context,
						int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddIsoEdge (const void *context, int argc,
						const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ModEdgeSplit (const void *context,
						  int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_NewEdgesSplit (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ModEdgeHeal (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_NewEdgeHeal (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetFaceEdges (const void *context,
						  int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetFaceGeometry (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ValidateTopoGeo (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_CreateTopoGeo (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetNodeByPoint (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetEdgeByPoint (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetFaceByPoint (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_AddPoint (const void *context,
						      int argc,
						      const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_AddLineString (const void
							   *context, int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_AddLineStringNoFace (const void
								 *context,
								 int argc,
								 const void
								 *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_Polygonize (const void *context,
							int argc,
							const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_FromGeoTable (const void *context,
							  int argc,
							  const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_FromGeoTableNoFace (const void
								*context,
								int argc,
								const void
								*argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_FromGeoTableExt (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_FromGeoTableNoFaceExt (const void
								   *context,
								   int argc,
								   const void
								   *argv);

    SPATIALITE_PRIVATE void fnctaux_Polygonize (const void
						*context, int argc,
						const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_TopoSnap (const void
						      *context, int argc,
						      const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_SnappedGeoTable (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_ToGeoTable (const void *context,
							int argc,
							const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_PolyFacesList (const void *context,
							   int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_LineEdgesList (const void *context,
							   int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_ToGeoTableGeneralize (const void
								  *context,
								  int argc,
								  const void
								  *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_RemoveSmallFaces (const void
							      *context,
							      int argc,
							      const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_RemoveDanglingEdges (const void
								 *context,
								 int argc,
								 const void
								 *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_RemoveDanglingNodes (const void
								 *context,
								 int argc,
								 const void
								 *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_NewEdgeHeal (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_ModEdgeHeal (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_NewEdgesSplit (const void *context,
							   int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_ModEdgeSplit (const void *context,
							  int argc,
							  const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_CreateTopoLayer (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_InitTopoLayer (const void
							   *context, int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_RemoveTopoLayer (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_ExportTopoLayer (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_InsertFeatureFromTopoLayer (const
									void
									*context,
									int
									argc,
									const
									void
									*argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_Clone (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_SubdivideLines (const void
							    *context,
							    int argc,
							    const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_DisambiguateSegmentEdges (const void
								      *context,
								      int argc,
								      const void
								      *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_GetEdgeSeed (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_GetFaceSeed (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_UpdateSeeds (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_SnapPointToSeed (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoGeo_SnapLineToSeed (const void
							    *context,
							    int argc,
							    const void *argv);

    SPATIALITE_PRIVATE void start_topo_savepoint (const void *handle,
						  const void *cache);

    SPATIALITE_PRIVATE void release_topo_savepoint (const void *handle,
						    const void *cache);

    SPATIALITE_PRIVATE void rollback_topo_savepoint (const void *handle,
						     const void *cache);

    SPATIALITE_PRIVATE void add_vtable_extent (const char *table, double minx,
					       double miny, double maxx,
					       double maxy, int srid,
					       const void *cache);

    SPATIALITE_PRIVATE void remove_vtable_extent (const char *table,
						  const void *cache);

    SPATIALITE_PRIVATE int get_vtable_extent (const char *table, double *minx,
					      double *miny, double *maxx,
					      double *maxy, int *srid,
					      const void *cache);

/* Topology-Network SQL functions */
    SPATIALITE_PRIVATE void fnctaux_GetLastNetworkException (const void
							     *context,
							     int argc,
							     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_CreateNetwork (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_DropNetwork (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddIsoNetNode (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_MoveIsoNetNode (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemIsoNetNode (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_AddLink (const void *context, int argc,
					     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ChangeLinkGeom (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_RemoveLink (const void *context, int argc,
						const void *argv);

    SPATIALITE_PRIVATE void fnctaux_NewLogLinkSplit (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ModLogLinkSplit (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_NewGeoLinkSplit (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ModGeoLinkSplit (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_NewLinkHeal (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ModLinkHeal (const void *context,
						 int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_LogiNetFromTGeo (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_SpatNetFromTGeo (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_SpatNetFromGeom (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ValidLogicalNet (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_ValidSpatialNet (const void *context,
						     int argc,
						     const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetNetNodeByPoint (const void *context,
						       int argc,
						       const void *argv);

    SPATIALITE_PRIVATE void fnctaux_GetLinkByPoint (const void *context,
						    int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_FromGeoTable (const void *context,
							  int argc,
							  const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_ToGeoTable (const void *context,
							int argc,
							const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_ToGeoTableGeneralize (const void
								  *context,
								  int argc,
								  const void
								  *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_ToGeoTableGeneralize (const void
								  *context,
								  int argc,
								  const void
								  *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_Clone (const void *context,
						   int argc, const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_GetLinkSeed (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_UpdateSeeds (const void *context,
							 int argc,
							 const void *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_DisambiguateSegmentLinks (const void
								      *context,
								      int argc,
								      const void
								      *argv);

    SPATIALITE_PRIVATE void fnctaux_TopoNet_LineLinksList (const void *context,
							   int argc,
							   const void *argv);

    SPATIALITE_PRIVATE void start_net_savepoint (const void *handle,
						 const void *cache);

    SPATIALITE_PRIVATE void release_net_savepoint (const void *handle,
						   const void *cache);

    SPATIALITE_PRIVATE void rollback_net_savepoint (const void *handle,
						    const void *cache);

    SPATIALITE_PRIVATE int test_inconsistent_topology (const void *handle);

    SPATIALITE_PRIVATE char *url_toUtf8 (const char *url,
					 const char *in_charset);

    SPATIALITE_PRIVATE char *url_fromUtf8 (const char *url,
					   const char *out_charset);

    SPATIALITE_PRIVATE int gaia_check_reference_geo_table (const void *handle,
							   const char
							   *db_prefix,
							   const char *table,
							   const char *column,
							   char **xtable,
							   char **xcolumn,
							   int *srid,
							   int *family);

    SPATIALITE_PRIVATE int gaia_check_output_table (const void *handle,
						    const char *table);

    SPATIALITE_PRIVATE int gaia_check_spatial_index (const void *handle,
						     const char *db_prefix,
						     const char *ref_table,
						     const char *ref_column);

    SPATIALITE_PRIVATE int gaia_do_eval_disjoint (const void *handle,
						  const char *matrix);

    SPATIALITE_PRIVATE int gaia_do_eval_overlaps (const void *handle,
						  const char *matrix);

    SPATIALITE_PRIVATE int gaia_do_eval_covers (const void *handle,
						const char *matrix);

    SPATIALITE_PRIVATE int gaia_do_eval_covered_by (const void *handle,
						    const char *matrix);

    SPATIALITE_PRIVATE void gaia_do_check_direction (const void *x1,
						     const void *x2,
						     char *direction);

    SPATIALITE_PRIVATE int gaia_do_check_linestring (const void *geom);

    SPATIALITE_PRIVATE void spatialite_internal_init (void *db_handle,
						      const void *ptr);

    SPATIALITE_PRIVATE void spatialite_internal_cleanup (const void *ptr);

    SPATIALITE_PRIVATE void gaia_sql_proc_set_error (const void *p_cache,
						     const char *errmsg);

    SPATIALITE_PRIVATE struct gaia_variant_value *gaia_alloc_variant ();

    SPATIALITE_PRIVATE void gaia_free_variant (struct gaia_variant_value
					       *variant);

    SPATIALITE_PRIVATE void gaia_set_variant_null (struct gaia_variant_value
						   *variant);

    SPATIALITE_PRIVATE void gaia_set_variant_int64 (struct gaia_variant_value
						    *variant,
						    sqlite3_int64 value);

    SPATIALITE_PRIVATE void gaia_set_variant_double (struct gaia_variant_value
						     *variant, double value);

    SPATIALITE_PRIVATE int gaia_set_variant_text (struct gaia_variant_value
						  *variant, const char *value,
						  int size);

    SPATIALITE_PRIVATE int gaia_set_variant_blob (struct gaia_variant_value
						  *variant,
						  const unsigned char *value,
						  int size);

#ifdef _WIN32
    SPATIALITE_PRIVATE void splite_pause_windows (void);
#else
    SPATIALITE_PRIVATE void splite_pause_signal (void);
#endif

    SPATIALITE_PRIVATE void finalize_topologies (const void *p_cache);

    SPATIALITE_PRIVATE int create_data_licenses (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_geometry_columns_time (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_spatial_ref_sys_aux (sqlite3 * handle);

    SPATIALITE_PRIVATE int create_raster_coverages (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_vector_coverages (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_wms_tables (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_external_graphics (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_fonts (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_raster_styles (sqlite3 * sqlite, int relaxed);

    SPATIALITE_PRIVATE int create_raster_styled_layers (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_vector_styles (sqlite3 * sqlite, int relaxed);

    SPATIALITE_PRIVATE int create_vector_styled_layers (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_iso_metadata (sqlite3 * sqlite, int relaxed);

    SPATIALITE_PRIVATE int create_iso_metadata_reference (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_iso_metadata_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_rl2map_configurations (sqlite3 * sqlite,
							 int relaxed);

    SPATIALITE_PRIVATE int create_rl2map_configurations_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_external_graphics_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_fonts_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_raster_styled_layers_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_raster_styles_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_vector_styles_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int create_vector_styled_layers_view (sqlite3 * sqlite);

    SPATIALITE_PRIVATE int createMissingSystemTables (sqlite3 * sqlite,
						      const void *cache,
						      int relaxed,
						      int transaction,
						      char **err_msg);

#ifdef __cplusplus
}
#endif

#endif				/* _SPATIALITE_PRIVATE_H */
