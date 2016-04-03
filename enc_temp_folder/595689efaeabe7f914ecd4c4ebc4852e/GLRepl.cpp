// GraphLang.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <sstream>
#include "../GraphLang/Token.h"
#include "../GraphLang/Parser.h"

using namespace std;


void doForString(string input)
{
	using RelationStmt = GraphLang::Tokenizer::RelationStatement;
	using Number = GraphLang::Tokenizer::Number;
	using String = GraphLang::Tokenizer::String;
	using Attribute = GraphLang::Tokenizer::Attribute;
	using NodeArray = GraphLang::Tokenizer::NodeArray;

	GraphLang::Tokenizer::String s1, s2;
	istringstream iss(input);
	GraphLang::Parser parser(iss);

	auto got = parser.oneOf<RelationStmt, NodeArray, Number, String, Attribute>();
	auto res = get<1>(got);

	if (!get<0>(res))
	{
		cout << "Didn't successfully parse any: " << get<1>(res) << endl;
		return;
	}

	auto tok = std::move(get<0>(got));

	if (parser.eof())
	{
		cout << "Got a " << tok->name();
	}
	else
	{
		cout << "First consumable was " << tok->name()
			<< ", but characters were left unread!!!";
	}

}


int main()
{
	while (true)
	{
		string line;
		cout << "\n\n$$$ ";
		getline(cin, line);
		doForString(line);
	}
}