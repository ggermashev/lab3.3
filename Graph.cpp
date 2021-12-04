#pragma once

#include "Graph.h"
#include "Sequance.h"
#include "PriorityQueue.h"
#include "DynamicArray.cpp"
#include "Sequance.cpp"
#include "LinkedList.h"
#include "LinkedList.cpp"

Link::Link(Link* l)
{
	num = l->num;
	weight = l->weight;
}

G::Node::Node(int num_, int linkNum, double weight_)
{
	num = num_;
	colour = 0;
	link = new ArrayVector<Link>();
	Link* l = new Link(linkNum, weight_);
	link->Append(*l);
}

G::Node::Node()
{
	num = -1;
	colour = 0;
	link = new ArrayVector<Link>();
}

G::Node::Node(Node* nd_)
{
	num = nd_->num;
	colour = nd_->colour;
	link = new ArrayVector<Link>(*nd_->link);
}

bool G::Node::AddToLink(int linkNum, double weight_)
{
	Link* l = new Link(linkNum, weight_);
	int flag = 0;
	for (int i = 0; i < link->GetLength(); i++)
	{
		if (link->Get(i).num == linkNum)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 0)
	{
		link->Append(*l);
		return true;
	}
	else return false;
}

Graph::Graph()
{
	nodes = new ArrayVector<G::Node*>();
}

void Graph::Add(int num1, int num2, double weight)
{
	int length = nodes->GetLength();
	if (num1 + 1 > nodes->GetLength()) nodes->Resize(num1 + 1);
	if (num2 + 1 > nodes->GetLength()) nodes->Resize(num2 + 1);
	for (int i = length; i < nodes->GetLength(); i++)
	{
		G::Node* nd = new G::Node();
		nd->num = i;
		nodes->Set(nd, i);
	}

	if (!nodes->Get(num1))
	{
		G::Node* nd = new G::Node(num1, num2, weight);
		nodes->Set(nd, num1);
	}
	else
	{
		G::Node* nd = new G::Node(nodes->Get(num1));
		delete nodes->Get(num1);
		nd->AddToLink(num2, weight);
		nodes->Set(nd, num1);
	}

}

void Graph::GenerateGraph(int numV, int numE)
{
	if (numE < numV) return;
	srand(time(NULL));
	nodes->Resize(numV);
	for (int i = 0; i < numV; i++)
	{
		G::Node* nd = new G::Node();
		nd->num = i;
		nodes->Set(nd, i);
	}
	int e = numE;
	for (int i = 0; i < numV; i++)
	{
		if (e == 0) return;
		int k = (rand() % (numV ));
		k = k % (e + 1);
		while (e - k > (numV - i - 1) * (numV - 1))
		{
			k = (rand() % (numV));
			k = k % (e + 1);
		}
		for (int j = 0; j < k; j++)
		{
			if (j == 0)
			{
				int num2 = rand() % (numV);
				while (num2 == i) num2 = rand() % (numV);
				G::Node* nd = new G::Node(i, num2 , rand());
				delete nodes->Get(i);
				nodes->Set(nd, i);
			}
			else
			{
				G::Node* nd = new G::Node(nodes->Get(i));
				delete nodes->Get(i);
				int num2 = rand() % (numV);
				while (num2 == i) num2 = rand() % (numV);
				while (nd->AddToLink(num2, rand()) == false) 
				{ 
					num2 = rand() % (numV); 
					while (num2 == i) num2 = rand() % (numV); 
				}
				nodes->Set(nd, i);
			}
		}
		e = e - k;
	}
}

ArrayVector<Link>* Graph::GetLinks(int num)
{
	return nodes->Get(num)->link;
}

int* Graph::GetLinksNums(int num)
{
	ArrayVector<Link>* link = new ArrayVector<Link>();
	link = GetLinks(num);
	int size = link->GetLength();
	int* a = new int[size];
	for (int i = 0; i < size; i++)
	{
		a[i] = link->Get(i).num;
	}
	return a;
}

double* Graph::GetLinksWeights(int num)
{
	ArrayVector<Link>* link = new ArrayVector<Link>();
	link = GetLinks(num);
	int size = link->GetLength();
	double* a = new double[size];
	for (int i = 0; i < size; i++)
	{
		a[i] = link->Get(i).weight;
	}
	return a;
}

void Graph::GetShortestValue(int num1, int num2, double count, int prev) 
{
	if (count == 0)
	{
		
		for (int i = 0; i < nodes->GetLength(); i++)
		{
			//G::Node* nd = new G::Node(nodes->Get(i));
			G::Node* nd = nodes->Get(i);
			nd->min = -1;
			//delete nodes->Get(i);
			nodes->Set(nd, i);
			//nd = nullptr;

		}
		usedCost = 0;
		min = -1;
	}
	
	double c = count;
	if (num1 == num2)
	{

		if (min == -1)
		{
			min = count;
		}
		if (count < min)
			min = count;
	}


	if (nodes->Get(num1) != nullptr)
		for (int i = 0; i < nodes->Get(num1)->link->GetLength(); i++)
		{
			c = count + nodes->Get(num1)->link->Get(i).weight;
			if ((nodes->Get(nodes->Get(num1)->link->Get(i).num)->min == -1) || (nodes->Get(nodes->Get(num1)->link->Get(i).num)->min > c))
			{
				//G::Node* nd = new G::Node(nodes->Get(nodes->Get(num1)->link->Get(i).num));
				G::Node* nd = nodes->Get(nodes->Get(num1)->link->Get(i).num);
				nd->min = c;
				//delete nodes->Get(nodes->Get(num1)->link->Get(i).num);
				nodes->Set(nd, nodes->Get(num1)->link->Get(i).num);
				//nd = nullptr;

				GetShortestValue(nodes->Get(num1)->link->Get(i).num, num2, c, num1);
			}
		}
}

ListVector<int>* Graph::GetShortestWay(int num1, int num2, ListVector<int>* solution, double count, int prev)
{
	if (solution == nullptr)
	{
		GetShortestValue(num1, num2, 0);

		solution = new ListVector<int>();
		count = 0;
		//if (used) delete[] used;
		//used = new int[nodes->GetLength()];

		for (int i = 0; i < nodes->GetLength(); i++)
		{
			//G::Node* nd = new G::Node(nodes->Get(i));
			G::Node* nd = nodes->Get(i);
			nd->min = -1;
			//delete nodes->Get(i);
			nodes->Set(nd, i);
			//nd = nullptr;
		}
		usedCost = 0;
	}
	
	double c = count;
	
	if (nodes->Get(num1) != nullptr)
	for (int i = 0; i < nodes->Get(num1)->link->GetLength(); i++)
	{
		c = count + nodes->Get(num1)->link->Get(i).weight;
		if ( (nodes->Get(nodes->Get(num1)->link->Get(i).num)->min == -1) || (nodes->Get(nodes->Get(num1)->link->Get(i).num)->min > c))
		{
			//G::Node* nd = new G::Node(nodes->Get(nodes->Get(num1)->link->Get(i).num));
			G::Node * nd = nodes->Get(nodes->Get(num1)->link->Get(i).num);
			nd->min = c;
			//delete nodes->Get(nodes->Get(num1)->link->Get(i).num);
			nodes->Set(nd, nodes->Get(num1)->link->Get(i).num);
			//nd = nullptr;

			solution = GetShortestWay(nodes->Get(num1)->link->Get(i).num, num2, solution, c, num1);
		}
	}

	if ((num1 == num2) && (min == count))
	{
		solution->Prepend(num1);
		return solution;
	}

	if (solution->GetLength() != 0)
		if (nodes->Get(num1) != nullptr)
	for (int i = 0; i < nodes->Get(num1)->link->GetLength(); i++) {
		if (nodes->Get(num1)->link->Get(i).num == solution->GetFirst())
				if (min - nodes->Get(num1)->link->Get(i).weight - usedCost == count)
				{
					usedCost += nodes->Get(num1)->link->Get(i).weight;
					solution->Prepend(num1);
					return solution;
				}
	}

	return solution;
}

double Graph::GetShortestWayValue(int num1, int num2, double count)
{
	GetShortestValue(num1, num2, 0);
	return min;
}


bool Comp(G::Node* nd1, G::Node* nd2)
{
	if (!nd1) return false;
	if (!nd2) return true;
	return (nd1->link->GetLength() > nd2->link->GetLength());
}

bool Compl(Link l1, Link l2)
{
	return l1.num > l2.num;
}

void Graph::Paint()
{

	ArrayVector<G::Node*>* arr = new ArrayVector<G::Node*>(nodes->GetLength());
	for (int i = 0; i < nodes->GetLength(); i++)
	{
		arr->Set(nodes->Get(i), i);
	}

	arr->FastSort(true, 0, GetSize() - 1, Comp);
	
	int colour = 0;

	for (int i = arr->GetLength() - 1; i >= 0; i--)
	{
		int flag2 = 0;
		
		if (arr->Get(i)->colour == 0)
		{
			colour++;
			//G::Node* nd = new G::Node(arr->Get(i));
			G::Node* nd = arr->Get(i);
			nd->colour = colour;
			//delete arr->Get(i);
			arr->Set(nd, i);
			//nd = nullptr;
		}
		else flag2 = 1;
		if (!nodes->Get(i)->link) continue;
		for (int k = arr->GetLength() - 1; k >= 0; k--)
		{
			int flag = 0;
			if (k == i) continue;

			for (int j = 0; j < arr->Get(i)->link->GetLength(); j++)
			{
				int num = arr->Get(i)->link->Get(j).num;
				if (arr->Get(k)->num == num) flag = 1;
			}
			if (flag == 0)
			{
				if ((k < i) && (arr->Get(k)->colour == 0) && (flag2 == 0) )
				{
					//G::Node* nd = new G::Node(arr->Get(k));
					G::Node* nd = arr->Get(k);
					nd->colour = arr->Get(i)->colour;
					//delete arr->Get(k);
					arr->Set(nd, k);
					//nd = nullptr;
				}

			}
			else
			{
				if ((k > i)   && (flag2 == 1))
				{
					if (arr->Get(i)->colour == arr->Get(k)->colour)
					{
						colour++;
						//G::Node* nd = new G::Node(arr->Get(i)); //i
						G::Node* nd = arr->Get(i);
						nd->colour = colour;
						//delete arr->Get(i);
						arr->Set(nd, i);
						//nd = nullptr;
						flag2 = 0;
					}
				}
				if ((k < i) && (arr->Get(i)->colour == arr->Get(k)->colour))
				{
					//G::Node* nd = new G::Node(arr->Get(k));
					G::Node* nd = arr->Get(k);
					nd->colour = 0;
					//delete arr->Get(k);
					arr->Set(nd, k);
					//nd = nullptr;
				}
				
			}
		}
	}

	for (int i = 0; i < nodes->GetLength(); i++)
	{
		for (int j = 0; j < arr->GetLength(); j++)
		{
			if (i == arr->Get(j)->num)
			{
				nodes->Set(arr->Get(j), i);
				break;
			}
		}
	}
}


int Graph::GetSize()
{
	return nodes->GetLength();
}

int Graph::GetColour(int num)
{
	if (!nodes->Get(num)) return 0;
	return nodes->Get(num)->colour;
}

int Graph::GetNum(int num)
{
	if (!nodes->Get(num)) return num;
	return nodes->Get(num)->num;
}

void Graph::Clear()
{
	for (int i = 0; i < nodes->GetLength(); i++)
	{
		if (nodes->Get(i)) delete nodes->Get(i)->link;
	}
	delete nodes;
	delete this;
}