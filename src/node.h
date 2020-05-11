#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "node.h"
#include "data.h"
#include "input.h"
#include <vector>
using namespace std;
class Node
{
  public:
    Node(int dimension);
    vector<pair<int, int> > arcosProibidos;
    vector<pair<int, int> > star;
    vector<pair<int, int> > arestas;
    vector<double> lambda;
    vector<int> g;
    vector<int> degree;
    int noEscolhido;
    int indexStar;
    double LB;
    double currentNodeCost;
    bool isFeasible;
    bool pruning;
    bool DFS;
    void calculateLB(Data* input, const double &);
};

#endif //NODE_H_INCLUDED