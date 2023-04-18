//
// Created by lst19 on 4/16/2023.
//
#include <cassert>
#include "depth.h"
#include "util.h"

Depth::Depth(string htnFile, int length) {
    this->readHTNFile(htnFile);
    this->htn->calcSCCs();
    this->htn->calcSCCGraph();
    this->htn->analyseSCCcyclicity();
    int maxSubtasks = 0;
    for (int m = 0; m < this->htn->numMethods; m++)
        if (maxSubtasks < this->htn->numSubTasks[m])
            maxSubtasks = this->htn->numSubTasks[m];
    this->depth.resize(this->htn->numTasks);
    for (int t = 0; t < this->htn->numTasks; t++)
        this->depth[t].assign(length + 1, -1);
    this->cache.resize(length + 1);
    for (int i = 0; i < length + 1; i++)
        this->cache[i].assign(maxSubtasks, nullptr);
    for (int i = 0; i < this->htn->numSCCs; i++) {
        int scc = htn->sccTopOrder[i];
        this->depthPerSCC(length, scc, this->htn);
    }
}

void Depth::depthPerSCC(
        int length,
        int scc,
        Model *htn) {
    int numTasks = htn->sccSize[scc];
    this->update(length, scc, htn, false);
    for (int i = 0; i < numTasks; i++)
        this->update(length, scc, htn, true);
}

void Depth::update(
        int length,
        int scc,
        Model *htn,
        bool empty) {
    int numTasks = htn->sccSize[scc];
    for (int l = 0; l < length; l++) {
        for (int tInd = 0; tInd < numTasks; tInd++) {
            int task = htn->sccToTasks[scc][tInd];
            if (htn->isPrimitive[task]) {
                assert(numTasks == 1);
                string taskName = htn->taskNames[task];
                if (Util::isPrecondition(taskName))
                    this->depth[task][0] = 0;
                else
                    this->depth[task][1] = 0;
                return;
            }
            int numMethods = htn->numMethodsForTask[task];
            for (int mInd = 0; mInd < numMethods; mInd++) {
                int m = htn->taskToMethods[task][mInd];
                int numSubtasks = htn->numSubTasks[m];
                if (this->cache[l][numSubtasks - 1] == nullptr)
                    this->cache[l][numSubtasks - 1] = new LengthDistributions(
                            l, numSubtasks);
                LengthDistributions *distributions = this->cache[l][numSubtasks - 1];
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
                        int local = this->depth[subtask][len];
                        if (local == -1) {
                            candidate = local;
                            break;
                        }
                        if (candidate < local)
                            candidate = local;
                    }
                    if (candidate != -1)
                        candidate += 1;
                    this->depth[task][l] = max(
                            candidate, this->depth[task][l]);
                }
                distributions->reset();
            }
        }
    }
}