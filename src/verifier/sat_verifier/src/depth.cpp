//
// Created by lst19 on 4/16/2023.
//
#include <cassert>
#include "depth.h"

Depth::Depth(Model *htn, int length) {
    htn->calcSCCs();
    htn->calcSCCGraph();
    htn->analyseSCCcyclicity();
    TaskMarker *marker = new TaskMarker(htn);
    this->depth.resize(htn->numTasks);
    for (int t = 0; t < htn->numTasks; t++)
        this->depth[t].assign(length, INT_MAX);
    for (int i = 0; i < htn->numSCCs; i++) {
        int scc = htn->sccTopOrder[i];
        this->depthPerSCC(length, scc, htn, marker);
    }
}

void Depth::depthPerSCC(
        int length,
        int scc, Model *htn,
        TaskMarker *marker) {
    int numTasks = htn->sccSize[scc];
    for (int i = 0; i < numTasks; i++) {
        this->update(length, scc, htn, marker, false);
        for (int j = 0; j < numTasks - 1; j++)
            this->update(length, scc, htn, marker, true);
    }
}

void Depth::update(
        int length,
        int scc,
        Model *htn,
        TaskMarker *marker,
        bool empty) {
    int numTasks = htn->sccSize[scc];
    for (int l = 0; l < length; l++) {
        for (int tInd = 0; tInd < numTasks; tInd++) {
            int task = htn->sccToTasks[scc][tInd];
            if (htn->isPrimitive[task]) {
                assert(numTasks == 1);
                this->depth[task][1] = 0;
                return;
            }
            int numMethods = htn->numMethodsForTask[task];
            for (int mInd = 0; mInd < numMethods; mInd++) {
                int m = htn->taskToMethods[task][mInd];
                int numSubtasks = htn->numSubTasks[m];
                LengthDistributions *distributions = new LengthDistributions(
                        l, numSubtasks);
                for (;;) {
                    int *distribution = distributions->next();
                    if (distribution == nullptr) break;
                    bool hasEmptyCycles = false;
                    for (int i = 0; i < numSubtasks; i++)
                        if (distribution[i] == l)
                            hasEmptyCycles = true;
                    if (hasEmptyCycles != empty) continue;
                    int candidate = -1;
                    for (int t = 0; t < numSubtasks; t++) {
                        int subtask = htn->subTasks[m][t];
                        int len = distribution[t];
                        if (candidate < this->depth[subtask][len])
                            candidate = this->depth[subtask][len];
                    }
                    candidate += 1;
                    this->depth[task][l] = max(
                            candidate, this->depth[task][l]);
                }
            }
        }
    }
}