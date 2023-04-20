//
// Created by u6162630 on 4/18/23.
//
#include "catch.hpp"
#include "depth.h"
#include "fstream"

TEST_CASE("TEST DEPTH SIMPLE") {
    string htnFile = "/home/users/u6162630/Projects/ongoing/pandaPIengine/src/verifier/test/test-task.sas";
    Depth *depth = new Depth(htnFile, 3);
    REQUIRE(depth->get(5, 3) == 2);
}