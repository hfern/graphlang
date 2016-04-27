#pragma once
#include "InputStream.h"
#include "Token.h"
#include "stdafx.h"
#include <vector>
#include <memory>

namespace GraphLang
{
	namespace Tokenizer
	{
		class Token;
	};

class Parser
{

	Tokenizer::InputStream& ins;
public:
	using UTokenPtr = std::unique_ptr<Tokenizer::Token>;
	using OneOfRType = std::tuple<UTokenPtr, ::GraphLang::Tokenizer::TokenParseResult>;

	class Checkpoint;
	friend class Parser::Checkpoint;

	Parser(Tokenizer::InputStream& ins_);
	~Parser();

	Tokenizer::InputStream&	i();
	int	readws();
	int peek(int n = 0);

	Tokenizer::TokenParseResult require(char c);
	Tokenizer::TokenParseResult require(Tokenizer::Token& tok);

	// Match a literal sequence of characters
	bool	matchLiteral(std::string str);

	bool	eof();

	// Try to match one of multiple options
	// Returns {nodeptr, {matched:bool, reason:string}}
	template <typename... Args>
	struct Impl;

	template <typename T1, typename... Ts>
	struct Impl<T1, Ts...>
	{
		static OneOfRType oneOf(Parser& p)
		{
			T1 token;
			auto r = p.require(token);

			if (std::get<0>(r))
			{
				// matched
				return std::make_tuple(
					std::move(std::unique_ptr<Tokenizer::Token>(new T1(token))),
					r
					);
			}

			return Impl<Ts...>::oneOf(p);
		}
	};

	template <>
	struct Impl<>
	{
		static OneOfRType oneOf(Parser& p)
		{
			return std::make_tuple(
				std::unique_ptr<Tokenizer::Token>(nullptr),
				::GraphLang::Tokenizer::TokenParseResult{ false, "Couldn't match one of multiple." }
			);
		}
	};

	template <typename... Args>
	OneOfRType oneOf()
	{
		return Impl<Args...>::oneOf(*this);
	}

public:
	class Checkpoint
	{
		Parser&		parser;
		std::size_t initial;
		bool		willCommit = false;

		// Checkpoint will keep track of where the 
		// tokens were read from.
		Tokenizer::Token*	tok = nullptr;

	public:
		Checkpoint(Parser& parser_, Tokenizer::Token* tok_ = nullptr);
		void commit();

		~Checkpoint();
	};

};
}; // /namespace GraphLang