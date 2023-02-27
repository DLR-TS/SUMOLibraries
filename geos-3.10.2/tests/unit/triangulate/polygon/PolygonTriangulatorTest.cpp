//
// Test Suite for geos::triangulate::polygon::PolygonTriangulator
//
// tut
#include <tut/tut.hpp>
#include <utility.h>

// geos
#include <geos/geom/Geometry.h>
#include <geos/io/WKTReader.h>
// #include <geos/io/WKTWriter.h>
#include <geos/triangulate/polygon/PolygonTriangulator.h>

// std
#include <stdio.h>

using geos::triangulate::polygon::PolygonTriangulator;
using geos::geom::Geometry;

namespace tut {
//
// Test Group
//

// dummy data, not used
struct test_polygontriangulator_data {

    geos::io::WKTReader r;

    test_polygontriangulator_data() {}

    void checkTri(const std::string& wkt, const std::string& wktExpected)
    {
        std::unique_ptr<Geometry> geom = r.read(wkt);
        std::unique_ptr<Geometry> actual = PolygonTriangulator::triangulate(geom.get());
        std::unique_ptr<Geometry> expected = r.read(wktExpected);
        ensure_equals_geometry(expected.get(), actual.get());
    }

    /**
    * Check union of result equals original geom
    * @param wkt
    */
    void checkTri(const std::string& wkt)
    {
        std::unique_ptr<Geometry> geom = r.read(wkt);
        std::unique_ptr<Geometry> actual = PolygonTriangulator::triangulate(geom.get());
        std::unique_ptr<Geometry> actualUnion = actual->Union();
        // std::cout << std::endl << "actual" << std::endl << *actual << std::endl;
        // std::cout << std::endl << "actualUnion" << std::endl << *actualUnion << std::endl;
        // std::cout << std::endl << "geom" << std::endl << *geom << std::endl;
        ensure_equals_geometry(geom.get(), actualUnion.get());
    }

};


typedef test_group<test_polygontriangulator_data> group;
typedef group::object object;

group test_polygontriangulator_group("geos::triangulate::polygon::PolygonTriangulator");


// testQuad
template<>
template<>
void object::test<1>()
{
    checkTri(
        "POLYGON ((10 10, 20 40, 90 90, 90 10, 10 10))",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 20 40, 90 90, 10 10)), POLYGON ((90 90, 90 10, 10 10, 90 90)))"
        );
}

// testPent
template<>
template<>
void object::test<2>()
{
    checkTri(
        "POLYGON ((10 10, 20 40, 90 90, 100 50, 90 10, 10 10))",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 20 40, 90 90, 10 10)), POLYGON ((90 90, 100 50, 90 10, 90 90)), POLYGON ((90 10, 10 10, 90 90, 90 10)))"
        );
}

// testHoleCW
template<>
template<>
void object::test<3>()
{
    checkTri(
        "POLYGON ((10 90, 90 90, 90 20, 10 10, 10 90), (30 70, 80 70, 50 30, 30 70))",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 10 90, 30 70, 10 10)), POLYGON ((80 70, 30 70, 10 90, 80 70)), POLYGON ((10 10, 30 70, 50 30, 10 10)), POLYGON ((80 70, 10 90, 90 90, 80 70)), POLYGON ((90 20, 10 10, 50 30, 90 20)), POLYGON ((80 70, 90 90, 90 20, 80 70)), POLYGON ((90 20, 50 30, 80 70, 90 20)))"
        );
}

// testTouchingHoles
template<>
template<>
void object::test<4>()
{
    checkTri(
        "POLYGON ((10 90, 90 90, 90 10, 10 10, 10 90), (20 80, 50 70, 30 30, 20 80), (70 20, 50 70, 80 80, 70 20))",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 10 90, 20 80, 10 10)), POLYGON ((50 70, 20 80, 10 90, 50 70)), POLYGON ((10 10, 20 80, 30 30, 10 10)), POLYGON ((30 30, 50 70, 70 20, 30 30)), POLYGON ((80 80, 50 70, 10 90, 80 80)), POLYGON ((90 10, 10 10, 30 30, 90 10)), POLYGON ((80 80, 10 90, 90 90, 80 80)), POLYGON ((90 10, 30 30, 70 20, 90 10)), POLYGON ((70 20, 80 80, 90 90, 70 20)), POLYGON ((90 90, 90 10, 70 20, 90 90)))"
        );
}

// testRepeatedPoints
template<>
template<>
void object::test<5>()
{
    checkTri(
        "POLYGON ((71 195, 178 335, 178 335, 239 185, 380 210, 290 60, 110 70, 71 195))",
        "GEOMETRYCOLLECTION (POLYGON ((71 195, 178 335, 239 185, 71 195)), POLYGON ((71 195, 239 185, 290 60, 71 195)), POLYGON ((71 195, 290 60, 110 70, 71 195)), POLYGON ((239 185, 380 210, 290 60, 239 185)))"
        );
}

// testMultiPolygon
template<>
template<>
void object::test<6>()
{
    checkTri(
        "MULTIPOLYGON (((10 10, 20 50, 50 50, 40 20, 10 10)), ((20 60, 60 60, 90 20, 90 90, 20 60)), ((10 90, 10 70, 40 70, 50 90, 10 90)))",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 20 50, 50 50, 10 10)), POLYGON ((50 50, 40 20, 10 10, 50 50)), POLYGON ((90 90, 90 20, 60 60, 90 90)), POLYGON ((60 60, 20 60, 90 90, 60 60)), POLYGON ((10 70, 10 90, 50 90, 10 70)), POLYGON ((50 90, 40 70, 10 70, 50 90)))"
        );
}

// testCeeShape
template<>
template<>
void object::test<7>()
{
    checkTri(
        "POLYGON ((110 170, 138 272, 145 286, 152 296, 160 307, 303 307, 314 301, 332 287, 343 278, 352 270, 385 99, 374 89, 359 79, 178 89, 167 91, 153 99, 146 107, 173 157, 182 163, 191 170, 199 176, 208 184, 218 194, 226 203, 198 252, 188 247, 182 239, 175 231, 167 223, 161 213, 156 203, 155 198, 110 170))"
        );
}

// Linestring input
template<>
template<>
void object::test<8>()
{
    checkTri(
        "LINESTRING (110 170, 138 272, 145 286, 152 296, 160 307, 303 307, 314 301, 332 287, 343 278, 352 270, 385 99, 374 89, 359 79, 178 89, 167 91, 153 99, 146 107, 173 157, 182 163, 191 170, 199 176, 208 184, 218 194, 226 203, 198 252, 188 247, 182 239, 175 231, 167 223, 161 213, 156 203, 155 198, 110 170)",
        "GEOMETRYCOLLECTION EMPTY"
        );
}

// Empty input
template<>
template<>
void object::test<9>()
{
    checkTri(
        "POLYGON EMPTY",
        "GEOMETRYCOLLECTION EMPTY"
        );
}

// Empty input in collection
template<>
template<>
void object::test<10>()
{
    checkTri(
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 20 40, 90 90, 90 10, 10 10)), POLYGON EMPTY)",
        "GEOMETRYCOLLECTION (POLYGON ((10 10, 20 40, 90 90, 10 10)), POLYGON ((90 90, 90 10, 10 10, 90 90)))"
        );
}

// Empty mistyped input
template<>
template<>
void object::test<11>()
{
    checkTri(
        "POINT EMPTY",
        "GEOMETRYCOLLECTION EMPTY"
        );
}


// testCollapsedCorner
template<>
template<>
void object::test<12>()
{
    checkTri(
        "POLYGON ((186 90, 71 17, 74 10, 65 0, 0 121, 186 90), (73 34, 67 41, 71 17, 73 34))"
        );
}



} // namespace tut


