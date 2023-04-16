//
// Created by lst19 on 4/16/2023.
//
#include "depth.h"
#include "marker.h"

Depth::Depth(Model *htn, int length) {
    htn->calcSCCs();
    htn->calcSCCGraph();
    htn->analyseSCCcyclicity();
    TaskMarker *marker = new TaskMarker(htn);
}