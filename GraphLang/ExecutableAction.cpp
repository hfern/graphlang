#include "stdafx.h"
#include "ExecutableAction.h"

using std::make_unique;
using GraphLang::Action::Impl::canDowncastTo;
using GraphLang::Tokenizer::Relation;

namespace GraphLang
{
	namespace Action
	{
		ExecutableAction::ExecutableAction()
		{
		}


		ExecutableAction::~ExecutableAction()
		{
		}

		ExecutableAction::ExecArgs::ExecArgs(Graph& graph_)
			: graph(graph_)
		{
			
		}

		std::vector<std::unique_ptr<ExecutableAction>>& ActionYielder::getActions()
		{
			return actions;
		}

		AttachAttribute::AttachAttribute(std::string node, std::string prop, NodeValue v)
			: nodeName(node), propertyName(prop), val(v)
		{
		}

		void AttachAttribute::execute(ExecArgs args)
		{
			// TODO(hunter): Implement!
		}

		const std::string& AttachAttribute::node_name() const
		{
			return nodeName;
		}

		const std::string& AttachAttribute::property_name() const
		{
			return propertyName;
		}

		const NodeValue& AttachAttribute::value() const
		{
			return val;
		}

		Tokenizer::TokenVisitor::Status AttachAttributeYielder::visit(Tokenizer::Token& token)
		{
			auto nodeptr = dynamic_cast<Tokenizer::Node*>(&token);

			if (nodeptr != nullptr)
			{
				for (auto& attr : nodeptr->attributes)
				{
					actions.emplace_back(make_unique<AttachAttribute>(
						nodeptr->identifier.id,
						attr->id.id,
						attr->val.toNodeValue()
					));
				}
			}

			return Status::Continue;
		}

		AttachRelation::AttachRelation(std::string fromNode, std::string relName, std::string toNode)
			: from(fromNode), relation(relName), to(toNode)
		{
		}

		void AttachRelation::execute(ExecArgs args)
		{
			// TODO(hunter): Implement!
			// do nothing right now :)
		}

		const std::string& AttachRelation::fromNode() const
		{
			return from;
		}

		const std::string& AttachRelation::relName() const
		{
			return relation;
		}

		const std::string& AttachRelation::toNode() const
		{
			return to;
		}

		Tokenizer::TokenVisitor::Status AttachRelationYielder::visit(Tokenizer::Token& token)
		{
			auto relptr = dynamic_cast<Tokenizer::RelationStatement*>(&token);
			if (relptr == nullptr)
			{
				// a relation stmt can't exist beneath a NodeArray
				// so might as well save a potentially expensive node
				// tree parsing
				if (std::get<bool>(canDowncastTo<Tokenizer::NodeArray>(token)))
				{
					return Status::StopBranch;
				}

				return Status::Continue;
			}

			auto leftGroup = relptr->nodeSets.begin();
			auto rightGroup = relptr->nodeSets.begin(); ++rightGroup;
			auto curRelation = relptr->relations.begin();
			auto relEnd = relptr->relations.end();


			while (curRelation != relEnd)
			{
				auto& left = *leftGroup;
				auto& right = *rightGroup;
				auto& rel = *curRelation;

				for (auto& leftNode: left.nodes)
				{
					for (auto& rightNode: right.nodes)
					{
						if (rel.direction.is(Relation::Direction::Right))
						{
							actions.emplace_back(make_unique<AttachRelation>(
								leftNode.identifier.id,
								rel.relationName.id,
								rightNode.identifier.id
							));
						}

						if (rel.direction.is(Relation::Direction::Left))
						{
							actions.emplace_back(make_unique<AttachRelation>(
								rightNode.identifier.id,
								rel.relationName.id,
								leftNode.identifier.id
							));
						}
					}
				}

				++leftGroup;
				++rightGroup;
				++curRelation;
			}


		}
	} // /Action
} // /GraphLang
