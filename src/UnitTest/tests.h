#pragma once

#include "catch.hpp"

#include "CppHashMap/test.h"


TEST_CASE("Test of unit test system", "") {
    REQUIRE(test() == 2);
}