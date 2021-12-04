#pragma once

#include "Sequance.h"
#include "PriorityQueue.h"


class Link {
public:
	int num;
	double weight;
	double used = -1;
	Link(int num_, double weight_) { num = num_, weight = weight_; }
	Link() { num = 5; weight = 5; }
	Link(Link* l);
};

namespace G
{
	class Node
	{
	public:
		int num;
		ArrayVector<Link>* link;
		int colour = 0;
		double min = -1;
		Node(int num_, int linkNum, double weight_);
		Node(Node* nd_);
		Node();
		bool AddToLink(int linkNum, double weight_);
	};
}

class Graph
{
private:
	ArrayVector<G::Node*>* nodes;

	double min;
	int* used;
	double usedCost;
	void GetShortestValue(int num1, int num2, double count, int prev = -1);

public:
	Graph(); //пустой
	void Add(int num1, int num2, double weight); //num1 и num2 больше размера - resize(). к node1 сделать link2. 
	void GenerateGraph(int numV, int numE);

	int GetColour(int num);
	int GetNum(int num);
	int GetSize();
	ArrayVector<Link>* GetLinks(int num); // возвращает все связи узла
	int* GetLinksNums(int num);
	double* GetLinksWeights(int num);

	ListVector<int>* GetShortestWay(int num1, int num2, ListVector<int>* solution, double count = 0, int prev = -1);
	double GetShortestWayValue(int num1, int num2, double count = 0);
	void Paint();
	
	void Clear();
};

bool Comp(G::Node* nd1, G::Node* nd2);