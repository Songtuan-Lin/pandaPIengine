//
// Created by lst19 on 4/16/2023.
//

#ifndef PANDAPIENGINE_DEPTH_H
#define PANDAPIENGINE_DEPTH_H
#include "Model.h"
#include "marker.h"
#include "distribution.h"
#include "fstream"
#include <string.h>
class Depth {
public:
    Depth(string htnFile, int length);
    int get(int task, int length) {return this->depth[task][length];}
private:
    Model *htn;
    vector<Distributions*> distributions;
    vector<vector<int>> depth;
    vector<vector<LengthDistributions*>> cache;
    void depthPerSCC(int length, int scc);
    void update(int length, int scc, bool allowEmptiness);
    void readHTNFile(string htnFile) {
        cout << "read model from" << htnFile << "\n";
        std::ifstream *fileInput = new std::ifstream(htnFile);
        if(!fileInput->good()) {
            std::cerr << "Unable to open input file " << htnFile << ": " << strerror (errno) << std::endl;
            exit(-1);
        }
        std::istream * inputStream;
        inputStream = fileInput;

        bool useTaskHash = true;
        /* Read model */
        // todo: the correct value of maintainTaskRechability depends on the heuristic
        eMaintainTaskReachability reachability = mtrALL;
        bool trackContainedTasks = useTaskHash;
        this->htn = new Model(
                trackContainedTasks,
                reachability,
                true,
                true);
        this->htn->filename = htnFile;
        this->htn->read(inputStream);
        cout << "reading htn file completed" << endl;
    }
};
#endif //PANDAPIENGINE_DEPTH_H
