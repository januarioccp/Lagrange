#include "node.h"
#include "Kruskal.h"
#include <list>
#include <algorithm>
#include <cmath>
#define INF (0x7FFFFFFF)
using namespace std;

Node::Node(int dimension)
{
    this->isFeasible = false;
    this->pruning = false;
    lambda.resize(dimension);
    degree.resize(dimension);
    g.resize(dimension);
    star.resize(dimension);
    fill(lambda.begin(), lambda.end(), 0);
    indexStar = 0;
    DFS = false;
}

void Node::calculateLB(Data *input, const double &UB)
{
    // Cria matriz de distâncias com n vertices
    vector<vector<double>> cost(input->getDimension(), vector<double>(input->getDimension(), 0));

    // Valor fixo para reinicialização: quanto menor, mais rápido :-)
    const int p0 = 30;

    // Quantidade máxima de tentativas/passos fracassados
    int p = p0;

    double ek = 1;
    // O valor de ek será reduzido até o limite abaixo obtido do código do Carlos
    double e_k_lim = 1.0 / 10000.0;

    // No problema em questão, um LB negativo não faz sentido!!!
    double bestLB = 0.0;
    currentNodeCost = 0.0;

    // É preciso guardar o valor do melhor lambda
    vector<double> bestLambda;

    // Bloqueia os arcos proibidos
    for (const auto &arco : arcosProibidos)
    {
        cost[arco.first][arco.second] = INF;
        cost[arco.second][arco.first] = INF;
    }

    while (true)
    {
        // Os valores das distancias sao atualizados com base nos valores de lambda
        for (int i = 0; i < input->getDimension(); i++)
        {
            for (int j = 0; j < input->getDimension(); j++)
                if (cost[i][j] < INF)
                    cost[i][j] = input->getDistance(i, j) - lambda[i] - lambda[j];
        }

        Kruskal kr(cost);
        LB = kr.MST(input->getDimension());

        arestas = kr.getEdges();

        // Compute node degree
        fill(degree.begin(), degree.end(), 0);
        for (int i = 0; i < arestas.size(); i++)
        {
            degree[arestas[i].first]++;
            degree[arestas[i].second]++;
        }

        // É preciso guardar o melhor LB, melhor lambda e melhores graus
        if (LB > bestLB)
        {
            bestLB = LB;
            bestLambda = lambda;
        }
        else // Se não melhorou, decrementa o valor de p
            p--;
        if (p <= 0) // reinicia a contagem com o valor de ek pela metade quando as possibilidades de falhas forem esgotadas
        {
            ek /= 2;
            p = p0;
        }

        // Recuperar o valor original do LB
        for (int i = 1; i < input->getDimension(); i++)
            LB += 2 * lambda[i];
        currentNodeCost = (LB > currentNodeCost) ? LB : currentNodeCost;

        // Inicio do calculo so subgradiente

        isFeasible = true;
        // Subgradiente
        double penalidade = 0.0;
        for (int i = 0; i < g.size(); i++)
        {
            g[i] = 2 - degree[i];
            penalidade += pow(g[i], 2);
            if (g[i] > 0)
                isFeasible = false;
        }

        if (isFeasible || abs(UB - LB) <= 0.000001)
        {
            pruning = true;
            break;
        }

        for (int i = 0; i < lambda.size(); i++)
        {
            lambda[i] += ((ek * (UB - LB)) / penalidade) * g[i];
        }

        if (ek < e_k_lim)
        {
            pruning = false;
            isFeasible = false;
            break;
        }
    }
    lambda = bestLambda;
    LB = bestLB;

    if (DFS)
    {
        star.resize(0);
        int chosen = max_element(degree.begin() + 0, degree.end()) - degree.begin();

        for (pair<int, int> aresta : arestas)
            if (aresta.first == chosen || aresta.second == chosen)
                star.push_back(aresta);
        indexStar = 0;
    }
}