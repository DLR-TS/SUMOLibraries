//
// Test Suite for C-API GEOSOffsetCurve

#include <tut/tut.hpp>
// geos
#include <geos_c.h>
// std
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "capi_test_utils.h"

namespace tut {
//
// Test Group
//

// Common data used in test cases.
struct test_capioffsetcurve_data : public capitest::utility {
    test_capioffsetcurve_data() {
        GEOSWKTWriter_setTrim(wktw_, 1);
    }
};

typedef test_group<test_capioffsetcurve_data> group;
typedef group::object object;

group test_capioffsetcurve_group("capi::GEOSOffsetCurve");

//
// Test Cases
//

// Straight, left
template<>
template<>
void object::test<1>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 0)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, 2, 0, GEOSBUF_JOIN_ROUND, 2);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string("LINESTRING (0 2, 10 2)"));

}

// Straight, right
template<>
template<>
void object::test<2>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 0)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, -2, 0, GEOSBUF_JOIN_ROUND, 2);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string(
                      "LINESTRING (10 -2, 0 -2)"
                  ));
}

// Outside curve
template<>
template<>
void object::test<3>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 0, 10 10)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, -2, 1, GEOSBUF_JOIN_ROUND, 2);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string(
                      "LINESTRING (12 10, 12 0, 10 -2, 0 -2)"
                  ));
}

// Inside curve
template<>
template<>
void object::test<4>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 0, 10 10)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, 2, 1, GEOSBUF_JOIN_ROUND, 2);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string(
                      "LINESTRING (0 2, 8 2, 8 10)"
                  ));
}

// See http://trac.osgeo.org/postgis/ticket/413
template<>
template<>
void object::test<5>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(33282908 6005055,33282900 6005050,33282892 6005042,33282876 6005007,33282863 6004982,33282866 6004971,33282876 6004975,33282967 6005018,33282999 6005031)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, 44, 1, GEOSBUF_JOIN_MITRE, 1);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string(
                      "LINESTRING EMPTY"
                  ));
}

// 0 distance
// See http://trac.osgeo.org/postgis/ticket/454
template<>
template<>
void object::test<6>
()
{
    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0, 10 0)");

    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, 0, 0, GEOSBUF_JOIN_ROUND, 2);

    ensure(nullptr != geom2_);

    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);

    ensure_equals(std::string(wkt_), std::string(
                      "LINESTRING (0 0, 10 0)"
                  ));
}

// left-side and right-side curve
// See http://trac.osgeo.org/postgis/ticket/633
template<>
template<>
void object::test<7>
()
{
    std::string wkt0("LINESTRING ("
                     "665.7317504882812500 133.0762634277343700,"
                     "1774.4752197265625000 19.9391822814941410,"
                     "756.2413940429687500 466.8306579589843700,"
                     "626.1337890625000000 1898.0147705078125000,"
                     "433.8007202148437500 404.6052856445312500)");

    geom1_ = GEOSGeomFromWKT(wkt0.c_str());
    ensure(nullptr != geom1_);

    double width = 57.164000837203;

    // left-sided
    {
        geom2_ = GEOSOffsetCurve(geom1_, width, 8, GEOSBUF_JOIN_MITRE, 5.57);
        ensure(nullptr != geom2_);
        // likely, 5 >= 5
        ensure(GEOSGeomGetNumPoints(geom2_) >= GEOSGeomGetNumPoints(geom1_));
        wkt_ = GEOSWKTWriter_write(wktw_, geom2_);
        GEOSGeom_destroy(geom2_);
        GEOSFree(wkt_);
        //ensure_equals(std::string(wkt_), ...);
    }

    // right-sided
    {
        width = -width;
        geom2_ = GEOSOffsetCurve(geom1_, width, 8, GEOSBUF_JOIN_MITRE, 5.57);
        ensure(nullptr != geom2_);
        // likely, 5 >= 7
        ensure(GEOSGeomGetNumPoints(geom2_) >= GEOSGeomGetNumPoints(geom1_));
        wkt_ = GEOSWKTWriter_write(wktw_, geom2_);
        //ensure_equals(std::string(wkt_), ...);
    }
}

// Test duplicated inner vertex in input
// See http://trac.osgeo.org/postgis/ticket/602
template<>
template<>
void object::test<8>
()
{
    double width = -1;

    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0,0 10,0 10,10 10)");
    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, width, 8, GEOSBUF_JOIN_ROUND, 0);
    ensure("Unexpected exception", nullptr != geom2_);
    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);
    ensure_equals(std::string(wkt_), "LINESTRING (10 9, 1 9, 1 0)");
}

// Test duplicated final vertex in input
// See http://trac.osgeo.org/postgis/ticket/602
template<>
template<>
void object::test<9>
()
{
    double width = -1;

    geom1_ = GEOSGeomFromWKT("LINESTRING(0 0,0 10,0 10)");
    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, width, 8, GEOSBUF_JOIN_ROUND, 0);
    ensure("Unexpected exception", nullptr != geom2_);
    wkt_ = GEOSWKTWriter_write(wktw_, geom2_);
    ensure_equals(std::string(wkt_), "LINESTRING (1 10, 1 0)");
}

// Test only duplicated vertex in input
// See http://trac.osgeo.org/postgis/ticket/602
template<>
template<>
void object::test<10>
()
{
    double width = -1;

    geom1_ = GEOSGeomFromWKT("LINESTRING(0 10,0 10,0 10)");
    ensure(nullptr != geom1_);

    geom2_ = GEOSOffsetCurve(geom1_, width, 8, GEOSBUF_JOIN_ROUND, 0);
    ensure("Missing expected exception", nullptr == geom2_);
}

} // namespace tut

