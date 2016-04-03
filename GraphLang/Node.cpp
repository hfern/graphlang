#include "stdafx.h"
#include "Node.h"

using namespace std;

Node::Node()
{
}


Node::~Node()
{
}

void Node::add_attribute(PropertyID pid, NodeValue v)
{
	attributes[pid] = v;
}

void Node::add_linkto(RelationID typ, NodeID toNode)
{

	if (outgoing.find(typ) == outgoing.end())
	{
		outgoing[typ] = set<NodeID>({toNode});
		return;
	}

	auto list = outgoing[typ];

	if (list.find(toNode) == list.end())
	{
		list.insert(toNode);
	}
}

void Node::add_linkfrom(RelationID typ, NodeID fromNode)
{
	if (incoming.find(typ) == incoming.end())
	{
		incoming[typ] = set<NodeID>({ fromNode });
		return;
	}

	auto& list = incoming[typ];

	if (list.find(fromNode) == list.end())
	{
		list.insert(fromNode);
	}
}