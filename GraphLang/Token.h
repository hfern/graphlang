#pragma once
#include <istream>
#include "stdafx.h"
#include "Parser.h"
#include "NodeValue.h"
#include <memory>
#include <string>
#include <ostream>
#include <cstdint>

namespace GraphLang
{
	class Parser;

	namespace Tokenizer
	{
		class Token
		{
		public:
			class PrintablePosition;
		public:
			// character positions in the file the tokens are located at.
			std::size_t		startpos = 0;
			std::size_t		endpos = 0;

			Token();
			virtual TokenParseResult Parse(Parser& p);
			virtual ~Token();
			virtual Token* clone() const { return new Token(); };
			virtual std::string name() const { return "(RAW TOKEN BASE)"; };

			PrintablePosition pos() const;

			class PrintablePosition
			{
				std::size_t b, e;
			public:
				friend ::std::ostream& operator<< (::std::ostream& os, const PrintablePosition& pos);
				PrintablePosition(std::size_t b_, std::size_t e_) : b(b_), e(e_) {};
			};
		};

		class Identifier: public Token
		{
		public:
			std::string id;
			TokenParseResult Parse(Parser& input) override;
			virtual Token* clone() const override { return new Identifier(*this); };
			std::string name() const override { return "Identifier"; };
		};

		class Number: public Token
		{
		public:
			double num;
			TokenParseResult Parse(Parser& input) override;
			virtual Token* clone() const override { return new Number(*this); };
			std::string name() const override { return "Number"; };
		};

		class String: public Token
		{
		public:
			std::string val;
			TokenParseResult Parse(Parser& input) override;
			virtual Token* clone() const override { return new String(*this); };
			std::string name() const override { return "String"; };
		};

		// Any data type that can be literally 
		class LiteralValue: public Token
		{
		public:
			std::unique_ptr<Token> val;

			TokenParseResult Parse(Parser& input) override;

			explicit LiteralValue(std::unique_ptr<Token> v = nullptr): val(std::move(v)) {};
			LiteralValue(const LiteralValue& other);
			virtual Token* clone() const override { return new LiteralValue(*this); };
			std::string name() const override { return "LiteralValue"; };
		};

		class Attribute : public Token
		{
		public:
			Identifier id;
			LiteralValue val;
			TokenParseResult Parse(Parser& input) override;
			virtual Token* clone() const override { return new Attribute(*this); };
			std::string name() const override { return "Attribute"; };
		};


		class Node: public Token
		{
		public:
			Identifier identifier;
			std::vector<std::unique_ptr<Attribute>> attributes;

			TokenParseResult Parse(Parser& input) override;
			virtual Token* clone() const override { return new Node(*this); };
			std::string name() const override { return "Node"; };

			Node();
			Node(const Node& other);
		};

		class NodeArray: public Token
		{
		public:
			std::vector<Node> nodes;

			TokenParseResult Parse(Parser& p) override;
			Token* clone() const override { return new NodeArray(*this); };
			std::string name() const override { return "NodeArray"; };
		};

		class Relation: public Token
		{
		public: 
			class Direction
			{
				std::uint8_t field;
			public:
				enum Dir { Left = 1 << 0, Right = 1 << 1 };

				Direction() : field(0) {};
				Direction(const Direction& other): field(other.field) {};
				bool is(Dir d) const { return field & d; }
				void set(Dir d) { field |= d; };
				void clear() { field = 0; };
				std::string pretty() const;
			};

			Identifier relationName;
			Direction direction;

			TokenParseResult Parse(Parser& p) override;
			Token* clone() const override { return new Relation(*this); };
			std::string name() const override { return "Relation"; };
		};

		class RelationStatement : public Token
		{
		public:
			std::vector<NodeArray> nodeSets;
			std::vector<Relation> relations;

			TokenParseResult Parse(Parser& p) override;
			Token* clone() const override { return new RelationStatement(*this); };
			std::string name() const override { return "RelationStatement"; };
		};

		class SingleNodeStatement : public Token
		{
		public:
			Node node;

			TokenParseResult Parse(Parser& p) override;
			Token* clone() const override { return new SingleNodeStatement(*this); };
			std::string name() const override { return "SingleNodeStatement"; };
		};

		class NodeArrayDeclarationStatement : public Token
		{
		public:
			NodeArray nodearr;

			TokenParseResult Parse(Parser& p) override;
			Token* clone() const override { return new NodeArrayDeclarationStatement(*this); };
			std::string name() const override { return "NodeArrayDeclarationStatement"; };
		};
	}; // /namespace Tokenizer
}; // /namespace GraphLang