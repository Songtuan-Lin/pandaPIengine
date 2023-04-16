//
// Created by lst19 on 4/16/2023.
//

#ifndef PANDAPIENGINE_DEPTH_H
#define PANDAPIENGINE_DEPTH_H
#include "Model.h"
class Depth {
public:
    Depth(Model *htn, int length);
private:
    vector<vector<int>> depthPerTask;
};
#endif //PANDAPIENGINE_DEPTH_H
