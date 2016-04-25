// GraphLang.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <sstream>
#include "../GraphLang/Token.h"
#include "../GraphLang/Parser.h"
#include "../GraphLang/ExecutableAction.h"

using namespace std;
using namespace GraphLang;
using GraphLang::Action::ActionYielder;


bool doForString(string input)
{
	if (input == ":q")
	{
		return false;
	}

	GraphLang::Tokenizer::String s1, s2;
	istringstream iss(input);
	GraphLang::Parser parser(iss);

	auto got = parser.oneOf <
		GraphLang::Tokenizer::RelationStatement,
		GraphLang::Tokenizer::SingleNodeStatement,
		GraphLang::Tokenizer::NodeArrayDeclarationStatement,
		GraphLang::Tokenizer::Number,
		GraphLang::Tokenizer::String,
		GraphLang::Tokenizer::Attribute
	>();
	auto res = get<1>(got);

	if (!get<0>(res))
	{
		cout << "Didn't successfully parse any: " << get<1>(res) << endl;
		return true;
	}

	auto tok = std::move(get<0>(got));

	if (parser.eof() || parser.matchLiteral(";"))
	{
		cout << "Got a " << tok->name() << endl;
		cout << "Yielding Exec Actions..." << endl;
		Action::AttachAttributeYielder yielder;

		yielder(*tok.get(), Tokenizer::TokenVisitor::Root);

		cout << "Will attach attributes [" << yielder.getActions().size() << "]" << endl;

		for (auto& action: yielder.getActions())
		{
			auto aaptr = dynamic_cast<Action::AttachAttribute*>(action.get());
			cout << aaptr->node_name()
				<< "\t: `" << aaptr->property_name()
				<< "`\t=\t" << aaptr->value().getType() << endl;
		}
	}
	else
	{
		cout << "First consumable was " << tok->name()
			<< ", but characters were left unread!!!";
	}

	return true;
}


int main()
{
	bool quit = false;
	while (!quit)
	{
		string line;
		cout << "\n\n$$$ ";
		getline(cin, line);
		quit = !doForString(line);
	}

	cout << "Hasta la vista, baby. :)" << endl;
}