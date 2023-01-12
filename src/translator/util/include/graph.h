#ifndef _graph_inc_h_
#define _graph_inc_h_

#include "Model.h"

class DGraph {
    public:
        DGraph(int nV) {
            this->nV = nV;
            this->mask.assign(this->nV, false);
            this->adj.resize(this->nV);
        }
        void addEdge(int v, int w) {this->adj[v].push_back(w);}
        void remove(int v) {this->mask[v] = true;}
        bool removed(int v) {return this->mask[v];}
        int V() {return this->nV;}
        vector<int>::iterator adjBegin(int v) {return this->adj[v].begin();}
        vector<int>::iterator adjEnd(int v) {return this->adj[v].end();}
        DGraph* reverse() {
            DGraph *g = new DGraph(this->nV);
            for (int v = 0; v < this->nV; v++) {
                vector<int>::iterator iter;
                for (iter = this->adjBegin(v); iter < this->adjEnd(v); iter++)
                    g->addEdge(*iter, v);
            }
            return g;
        }

    private:
        int nV;
        vector<bool> mask;
        vector<vector<int>> adj;
};

class GraphDFS {
    public:
        GraphDFS(DGraph *g, int s) {
            this->g = g;
            this->s = s;
            connected.assign(this->g->V(), false);
            visited.assign(this->g->V(), false);
            dfs(s);
        }
        bool isConnected(int v) {return connected[v];}
        int S() {return s;} 

    private:
        DGraph *g;
        int s;
        vector<bool> connected;
        vector<bool> visited;
        void dfs(int v) {
            if (visited[v] || g->removed(v)) return;
            visited[v] = true;
            connected[v] = true;
            vector<int>::iterator iter;
            for (iter = g->adjBegin(v); iter < g->adjEnd(v); iter++)
                dfs(*iter);
        }
};

class TDG {
    public:
        TDG(Model *htn) {
            this->g = new DGraph(htn->numTasks + htn->numMethods);
            this->nTask = htn->numTasks;
            for (int m = 0; m < htn->numMethods; m++) {
                int cTask = htn->decomposedTask[m];
                this->g->addEdge(this->vT(cTask), this->vM(m));
                for (int i = 0; i < htn->numSubTasks[m]; i++) {
                    int t = htn->subTasks[m][i];
                    this->g->addEdge(this->vM(m), this->vT(t));
                }
            }
        }
        int vT(int t) {return t;}
        int vM(int m) {return this->nTask + m;}
        int M(int v) {return v - this->nTask;}
        int T(int v) {return v;}
        void maskTask(int t) {this->g->remove(vT(t));}
        void maskMethod(int m) {this->g->remove(vM(m));}
        void calcReachability(int v) {this->dfs = new GraphDFS(this->g, v);}
        bool reachable(int v) {return this->dfs->isConnected(v);}
        void reverse() {this->g = this->g->reverse();}
        vector<int>::iterator adjBegin(int v) {return this->g->adjBegin(v);}
        vector<int>::iterator adjEnd(int v) {return this->g->adjEnd(v);}

    private:
        int nTask;
        DGraph *g;
        GraphDFS *dfs;
};
#endif