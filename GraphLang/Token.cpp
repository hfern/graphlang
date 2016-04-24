#include "stdafx.h"
#include "Token.h"
#include <cassert>
#include <memory>

using namespace std;
using Checkpoint = GraphLang::Parser::Checkpoint;

namespace GraphLang
{
namespace Tokenizer
{
	const TokenVisitor::RootTag TokenVisitor::Root = {};

	void TokenVisitor::operator()(Token& tok)
	{
		auto directive = visit(tok);

		switch(directive)
		{
		case Status::Stop:
			throw Impl::FullStopException{};
			return;
		case Status::StopBranch:
			return;
		case Status::Continue:
			tok.expose_children(*this);
		}
	}

	void TokenVisitor::operator()(Token& tok, RootTag)
	{
		try
		{
			operator()(tok);
		}
		catch(Impl::FullStopException)
		{
			// that's fine :)
		}

		return; // end of input
	}

	Token::Token()
	{
	}

	TokenParseResult Token::Parse(Parser& p)
	{
		assert(false);
		return TokenParseResult{ false, "CANT TRY TO PARSE A RAW (TOKEN)!!!!!" };
	}

	Token::~Token()
	{
	}

	Token::PrintablePosition Token::pos() const
	{
		return PrintablePosition(startpos, endpos);
	}

	TokenParseResult Identifier::Parse(Parser& input)
	{
		auto& in = input.i();
		Parser::Checkpoint ch(input, this);

		bool digitsOk = false;

		char c = in.peek();
		while(
			c == '_' 
			|| ('a' <= c && c <= 'z') 
			|| ('A' <= c && c <= 'Z') 
			|| (digitsOk && '0' <= c && c <= '9')
			)
		{
			digitsOk = true;
			id += in.get();
			c = in.peek();
		}

		if (id.size() > 0)
		{
			ch.commit();
			return TokenParseResult{true, ""};
		}

		return TokenParseResult{false, "Couldn't match an ID."};
	}


	TokenParseResult Number::Parse(Parser& p)
	{
		Checkpoint ch(p, this);

		bool makeNegative = false;

		if (p.i().peek() == '-')
		{
			p.i().get();
			makeNegative = true;
			p.readws();
		}


		char firstChar = p.i().peek();
		if ('0' <= firstChar && firstChar <= '9')
		{
			p.i() >> num;
			if (makeNegative)
			{
				num *= -1;
			}

			ch.commit();

			return make_tuple(true, "");
		}

		return make_tuple(false, "Can't interpret as begining of number: "+firstChar);
	}

	TokenParseResult String::Parse(Parser& p)
	{
		Checkpoint ch(p, this);
		val = "";
		auto& i = p.i();

		char buf;

		i >> buf;
		if (buf != '"')
		{
			return TokenParseResult{false, "Expected '\"' to open string."};
		}

		while (i.peek() != '"')
		{
			i >> buf;

			if (buf == '\\')
			{
				i >> buf;

				switch (buf)
				{
				case '\\': val += '\\'; break;
				case 'n': val += '\n'; break;
				case 't': val += '\t'; break;
				case 'r': val += '\r'; break;
				case '"' : val += '"'; break;
				default:
					// This is such a dumb idea!
					throw string("Woah! Unknown escape sequence in string: \\"+buf);
				
				}

				continue;
			}

			val += buf;
		}

		i >> buf;
		if (buf != '"')
		{
			return TokenParseResult{ false, "Expected '\"' to close! string." };
		}

		ch.commit();
		return TokenParseResult{true, ""};
	}

	TokenParseResult Attribute::Parse(Parser& p)
	{
		Checkpoint ch(p, this);

		if (!get<0>(p.require(id)))
		{
			return make_tuple(false, "Couldn't match ID in AttributeKV-Pair");
		}

		if (!p.matchLiteral(":"))
		{
			return make_tuple(false, "Couldn't match colon in AttributeKV-Pair");
		}

		auto r = p.require(val);

		if (!get<0>(r))
		{
			return make_tuple(false, "Couldn't match val: (" + get<1>(r) + ") in AttributeKV-Pair");
		}

		ch.commit();

		return make_tuple(true, "");
	}

	void Attribute::expose_children(TokenVisitor& vis)
	{
		vis(id);
		vis(val);
	}

	TokenParseResult LiteralValue::Parse(Parser& p)
	{
		Checkpoint ch(p, this);

		// std::move({ make_unique<Number>(), make_unique<String>() })
		//vector<unique_ptr<Token>> tokens;

		//auto r = p.requireOneOf(std::move(tokens));
		
		auto r = p.oneOf<Number, String>();
		auto nodeptr = std::move(get<0>(r));


		if (nodeptr == nullptr)
		{
			return TokenParseResult{false, "Could not match a literal-value token!"};
		}

		ch.commit();
		val = std::move(nodeptr);


		return std::get<1>(r);
	}

	LiteralValue::LiteralValue(const LiteralValue& other)
	{
		val = nullptr;

		if (other.val != nullptr)
		{
			val = unique_ptr<Token>(other.val->clone());
		}
	}

	void LiteralValue::expose_children(TokenVisitor& v)
	{
		v(*val.get());
	}

	NodeValue LiteralValue::toNodeValue() const
	{
		if (String* strptr = dynamic_cast<String*>(val.get()))
		{
			return NodeValue(make_unique<std::string>(strptr->val));
		}

		if (Number* numptr = dynamic_cast<Number*>(val.get()))
		{
			return NodeValue(numptr->num);
		}

		return NodeValue{};
	}

	TokenParseResult Node::Parse(Parser& p)
	{
		// (hunter name:"Hunter" age:20)
		Checkpoint ch(p, this);

		TokenParseResult r;

		if (!p.matchLiteral("("))
		{
			return TokenParseResult{ false, "Nodes need to open with a left-paren." };
		}

		// TODO(Hunter): enable anonymous node support
		if (false && p.matchLiteral("?"))
		{
			// anonymous node!!
			identifier = Identifier();
			identifier.id = "";
		}
		else
		{
			// named node
			r = p.require(identifier);
			if (!get<0>(r))
			{
				return TokenParseResult{ false, "Couldn't locate an identifier for the node"};
			}
		}

		while (true)
		{
			Attribute attr;
			r = p.require(attr);

			if (! get<0>(r))
			{
				break;
			}

			attributes.push_back(make_unique<Attribute>(attr));
		}

		if (!p.matchLiteral(")"))
		{
			return TokenParseResult{ false, "Expected node to close with right-paren." };
		}

		ch.commit();
		return TokenParseResult{ true, ""};
	}

	Node::Node()
	{
	}

	Node::Node(const Node& other)
	{
		attributes.reserve(other.attributes.size());

		for (auto& p: other.attributes)
		{
			attributes.push_back(unique_ptr<Attribute>(static_cast<Attribute*>(p->clone())));
		}
	}

	void Node::expose_children(TokenVisitor& vis)
	{
		vis(identifier);
		for (auto& attr: attributes)
		{
			vis(*attr.get());
		}
	}

	::std::ostream& operator<<(::std::ostream& os, const Token::PrintablePosition& pos)
	{
		os << "(" << pos.b << ", " << pos.e << ")";
		return os;
	}


	TokenParseResult NodeArray::Parse(Parser& p)
	{
		Checkpoint ch(p, this);
		if (!p.matchLiteral("["))
		{
			// Single node
			Node node;
			auto r = p.require(node);

			if (!get<0>(r))
			{
				return TokenParseResult{ false,
					"Node-Array expected a single node in lieu of bracketed-array, but got (" + get<1>(r) + ")."
				};
			}

			nodes.push_back(node);

			ch.commit();
			return TokenParseResult{ true, "" };
		}

		if (p.matchLiteral("]"))
		{
			// empty vec of nodes
			ch.commit();
			return TokenParseResult{ true, "Empty NodeArray" };
		}

		Node primaryNode;
		auto r = p.require(primaryNode);
		if (!get<0>(r))
		{
			return TokenParseResult{false, "NodeArray expects non-empty NodeArray internals to start with a Node."};
		}

		nodes.push_back(primaryNode);

		while(p.matchLiteral(","))
		{
			Node extraNode;
			auto xnr = p.require(extraNode);

			if (!get<0>(xnr))
			{
				return TokenParseResult{ false, "NodeArray expects node to follow comma, but got: " + get<1>(xnr) };
			}

			nodes.push_back(extraNode);
		}

		// before consume ]

		if (!p.matchLiteral("]"))
		{
			return{false, "Couldn't match closing ] to NodeArray"};
		}

		ch.commit();

		return{ true, "" };
	}

	void NodeArray::expose_children(TokenVisitor& vis)
	{
		for (auto& node : nodes)
		{
			vis(node);
		}
	}

	TokenParseResult Relation::Parse(Parser& p)
	{
		// <-id- left relation
		// -id-> right relation
		// <-id-> both relation
		// -id- wtf is this???

		Checkpoint ch(p, this);

		if (p.matchLiteral("<-"))
		{
			direction.set(Direction::Left);
		}
		else
		{
			// at least need a - on the left
			if (!p.matchLiteral("-"))
			{
				return TokenParseResult{false, "Relation expected left to have either a <- or a -"};
			}
		}

		auto r = p.require(relationName);
		if (!get<0>(r))
		{
			return TokenParseResult{
				false, 
				"Relation expects there to be an identifier, but: " + get<1>(r)
			};
		}

		if (p.matchLiteral("->"))
		{
			direction.set(Direction::Right);
		}
		else
		{
			// at least need a - on the right
			if (!p.matchLiteral("-"))
			{
				return TokenParseResult{ false, "Relation expected right to have either a -> or a -" };
			}
		}

		if (!direction.is(Direction::Left) && !direction.is(Direction::Right))
		{
			return TokenParseResult{ false, "Relation must be either left, right, or both!" };
		}

		ch.commit();
		return TokenParseResult{true, ""};
	}

	void Relation::expose_children(TokenVisitor& vis)
	{
		vis(relationName);
	}

	std::string Relation::Direction::pretty() const
	{
		switch (field)
		{
		case 0x0:						return "NONE?";
		case Dir::Left:					return "LEFT";
		case Dir::Right:				return "RIGHT";
		case Dir::Left | Dir::Right:	return "LEFT-RIGHT";
		default:
			assert(0);
			return "UNKNOWN!!!";
		}
	}

	TokenParseResult RelationStatement::Parse(Parser& p)
	{
		Checkpoint ch(p, this);
		NodeArray firstNode;

		auto r = p.require(firstNode);

		if (!get<0>(r))
		{
			return{false, "RelationStatement expects a nodearray but failed b/c: " + get<1>(r)};
		}

		nodeSets.push_back(std::move(firstNode));

		bool atLeast1 = false;

		while(true)
		{
			Relation rel;

			auto rrel = p.require(rel);

			if (!get<0>(rrel))
			{
				// no next relation
				break;
			}

			NodeArray arr;
			auto rarr = p.require(arr);

			if (!get<0>(rarr))
			{
				// should be a fatal error really
				return {false, "RelationStatement: expected NodeArray to follow relation, but got: " + get<1>(rarr)};
			}

			relations.emplace_back(std::move(rel));
			nodeSets.emplace_back(std::move(arr));

			atLeast1 = true;
		}

		if (!atLeast1)
		{
			return{false, "RelationStatement: Expected at least a single relation between nodes."};
		}

		if (!p.matchLiteral(";"))
		{
			return{ false, "RelationStatement expects to end with ';' and didn't." };
		}

		ch.commit();
		return {true, ""};
	}

	void RelationStatement::expose_children(TokenVisitor& vis)
	{
		for (auto& node : nodeSets) vis(node);
		for (auto& relation : relations) vis(relation);
	}

	TokenParseResult SingleNodeStatement::Parse(Parser& p)
	{
		Checkpoint ch(p, this);

		if (!get<0>(p.require(node)))
		{
			return{ false, "SingleNodeStatement requires a single node!" };
		}

		if (!p.matchLiteral(";"))
		{
			return{ false, "SingleNodeStatement expects a node to be followed by ';'" };
		}

		ch.commit();
		return{ true, "" };
	}

	void SingleNodeStatement::expose_children(TokenVisitor& vis)
	{
		vis(node);
	}

	TokenParseResult NodeArrayDeclarationStatement::Parse(Parser& p)
	{
		Checkpoint ch(p, this);

		if (!get<0>(p.require(nodearr)))
		{
			return{ false, "NodeArrayDeclarationStatement requires a node array!" };
		}

		if (!p.matchLiteral(";"))
		{
			return{ false, "NodeArrayDeclarationStatement expects a nodearray to be followed by ';'" };
		}

		ch.commit();
		return{ true, "" };
	}

	void NodeArrayDeclarationStatement::expose_children(TokenVisitor& vis)
	{
		vis(nodearr);
	}
}; // /namespace Tokenizer
}; // /namespace GraphLang

