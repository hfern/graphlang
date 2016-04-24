#pragma once
#include "Node.h"
#include <string>
#include <memory>
#include "BidirectionalIndexed.hpp"

class Graph
{
public:
	using StringMap			= BidirectionalIndexed<StringID, std::string>;
	using RelationNameMap	= BidirectionalIndexed<RelationID, StringID>;
	using PropertyNameMap	= BidirectionalIndexed<RelationID, StringID>;
	using NodeAliasMap		= BidirectionalIndexed<StringID, NodeID>;

	StringMap strings;
	RelationNameMap relations;
	PropertyNameMap properties;

private:
	// allows assigning each node a unique id
	NodeID node_counter;

	// master list of node id to node
	std::map<NodeID, std::shared_ptr<Node>> nodeList;



public:
	Graph();
	~Graph();

	// Reserve a node id to use.
	NodeID reserve_nodeid();


};

class NodeManipilator
{
	Graph&	graph;
	Node&	node;

public:
	NodeManipilator();
	~NodeManipilator();

};