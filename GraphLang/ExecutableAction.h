#pragma once
#include <string>
#include "NodeValue.h"
#include "Graph.h"
#include "Token.h"
#include <vector>
#include <tuple>
#include <memory>

// TODO(hunter): This can get a lot more optimal.
				// Maybe add a field to indicate which actions are
				// parallelizable.
				// And maybe which require others.
				// Generalizing this will allow making it a query planner.

namespace GraphLang
{
	namespace Action
	{
		// ExecutableAction are actions that will be taken
		// because of a input statement.
		// That is, generally a user statement will be broken
		// down into these actions to preform on the database.

		
		class ExecutableAction
		{
		public:
			class ExecArgs;

			ExecutableAction();
			virtual void execute(ExecArgs args) = 0;
			virtual ~ExecutableAction();

			class ExecArgs
			{
				Graph& graph;
				ExecArgs(Graph& graph_);
			};
		};

		// Is something that reads a token tree and produces 
		// zero, one, or many ExecutableActions based on that token.
		class ActionYielder: public Tokenizer::TokenVisitor
		{
		protected:
			std::vector<std::unique_ptr<ExecutableAction>> actions;
		public:
			std::vector<std::unique_ptr<ExecutableAction>>& getActions();
		};

		// And some common executable actions derived from 
		// statements are....

		// For attaching attributes to nodes in the db
		class AttachAttribute: public ExecutableAction
		{
			std::string nodeName;
			std::string propertyName;
			NodeValue val;
		public:
			AttachAttribute(std::string node, std::string prop, NodeValue val);
			void execute(ExecArgs args) override;


			const std::string& node_name() const;
			const std::string& property_name() const;
			const NodeValue& value() const;
		};

		// And the ast tree parser that yields them...
		class AttachAttributeYielder: public ActionYielder
		{
		public: Status visit(Tokenizer::Token& token) override;
		};


		class AttachRelation : public ExecutableAction
		{
			std::string from;
			std::string relation;
			std::string to;

		public:
			AttachRelation(std::string fromNode, std::string relName, std::string toNode);
			void execute(ExecArgs args) override;

			const std::string& fromNode() const;
			const std::string& relName() const;
			const std::string& toNode() const;
		};

		// catlogues relations to be built in a query.
		class AttachRelationYielder: public ActionYielder
		{
		public: Status visit(Tokenizer::Token& token) override;
		};

		namespace Impl
		{
			template <typename T>
			std::tuple<bool, T*> canDowncastTo(Tokenizer::Token& n)
			{
				T* tempPtr = dynamic_cast<T*>(&n);
				return {tempPtr == nullptr, tempPtr};
			}
		} // /Impl

	} // /Action
} // /GraphLang