#include "stdafx.h"
#include "Graph.h"


Graph::Graph() : node_counter(0)
{
}


Graph::~Graph()
{
}

NodeID Graph::reserve_nodeid()
{
	node_counter++;
	return node_counter;
}