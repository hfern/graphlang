#pragma once
#include <vector>
#include "NodeValue.h"
#include <map>
#include <tuple>
#include <vector>
#include <set>

// Individual nodes are given a unique ID each.
using NodeID = std::size_t;

// Represents a relation type.
// For instance, might be an id corresponding to
// `works_at` or `friend`
using RelationID = std::size_t;

// Represents and ID of a property string.
// For instance, might be the id of `age`.
using PropertyID = std::size_t;

// String alias ids
using StringID = std::size_t;

struct Node
{
    NodeID id;
    std::vector<Node> outlinks;

    NodeValue value;
    std::map<PropertyID, NodeValue> attributes;

    std::map<RelationID, std::set<NodeID>> outgoing;
    std::map<RelationID, std::set<NodeID>> incoming;

public:
    Node();
    ~Node();

    void add_attribute(PropertyID pid, NodeValue v);
    void add_linkto(RelationID typ, NodeID toNode);
    void add_linkfrom(RelationID typ, NodeID fromNode);
};

