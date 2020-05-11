#include "Kruskal.h"

Kruskal::Kruskal(vvi dist)
{
	for (int i = 1; i < dist.size(); ++i)
	{
		for (int j = 1; j < dist[i].size(); ++j)
		{
			graph.push(make_pair(-dist[i][j], make_pair(i, j)));
		}
	}
	this->dist = dist;
}

void Kruskal::initDisjoint(int n)
{
	pset.resize(n);
	for (int i = 0; i < n; ++i)
	{
		pset[i] = i;
	}
}

int Kruskal::findSet(int i)
{
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j)
{
	pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j)
{
	return (findSet(i) == findSet(j)) ? true : false;
}

vii Kruskal::getEdges()
{
	return edges;
}

double Kruskal::MST(int nodes)
{
	initDisjoint(nodes);

	double cost = 0;

	while (!graph.empty())
	{
		pair<double, ii> p = graph.top();
		graph.pop();

		if (!isSameSet(p.second.first, p.second.second))
		{
			edges.push_back(make_pair(p.second.first, p.second.second));
			cost += (-p.first);
			unionSet(p.second.first, p.second.second);
		}
	}

	int n = 0;
	pair<int,int> vertex(1,2);
	pair<double,double> distance(dist[n][vertex.first],dist[n][vertex.second]);

	for(int i=1; i < dist.size(); i++){
		if (distance.first > dist[i][n])
		{
				distance.second = distance.first;
                vertex.second = vertex.first;
                
                distance.first = dist[i][n];
                vertex.first = i;
            }
            else if (distance.second > dist[i][n])
            {
                distance.second = dist[i][n];
                vertex.second = i;
            }
	}

	edges.push_back(make_pair(n, vertex.first));
	edges.push_back(make_pair(n, vertex.second));
	cost += distance.first + distance.second;
	return cost;
}