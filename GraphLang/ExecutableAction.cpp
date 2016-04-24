#include "stdafx.h"
#include "ExecutableAction.h"

using std::make_unique;

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
					actions.push_back(make_unique<AttachAttribute>(
						nodeptr->identifier.id,
						attr->id.id,
						attr->val.toNodeValue()
					));
				}
			}

			return Status::Continue;
		}
	} // /Action
} // /GraphLang
