//
// Created by lst19 on 4/16/2023.
//

#ifndef PANDAPIENGINE_DEPTH_H
#define PANDAPIENGINE_DEPTH_H
#include "Model.h"
#include "marker.h"
#include "distribution.h"
class Depth {
public:
    Depth(Model *htn, int length);
private:
    vector<vector<int>> depth;
    void depthPerSCC(int length, int scc, Model *htn, TaskMarker *marker);
    void update(int length, int scc, Model *htn, TaskMarker *marker, bool empty);
};
#endif //PANDAPIENGINE_DEPTH_H
