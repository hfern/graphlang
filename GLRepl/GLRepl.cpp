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

// Get the repl parse function that is ordered
// to GraphLang's default parse order.
GraphLang::Parser::OneOfRType orderedParse(GraphLang::Parser& p);


bool doForString(string input)
{
    if (input == ":q")
    {
        return false;
    }

    GraphLang::Tokenizer::String s1, s2;
    istringstream iss(input);
    GraphLang::Parser parser(iss);

    auto got = orderedParse(parser);
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

        Action::AttachAttributeYielder attrYielder;
        attrYielder(*tok.get(), Tokenizer::TokenVisitor::Root);
        cout << "Will attach attributes [" << attrYielder.getActions().size() << "]" << endl;
        for (auto& action: attrYielder.getActions())
        {
            auto aaptr = dynamic_cast<Action::AttachAttribute*>(action.get());
            cout << aaptr->node_name()
                << "\t: `" << aaptr->property_name()
                << "`\t=\t" << aaptr->value().getType() << endl;
        }

        cout << "\n\n\n";

        Action::AttachRelationYielder relYielder;
        relYielder(*tok.get(), Tokenizer::TokenVisitor::Root);
        cout << "Building the following relations: [" << relYielder.getActions().size() << "]\n";
        for (auto& action : relYielder.getActions())
        {
            auto arptr = dynamic_cast<Action::AttachRelation*>(action.get());
            cout << arptr->fromNode() << " \t-" << arptr->relName()
                << "-> \t" << arptr->toNode() << endl;
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

// The godless function that abuses macros.
GraphLang::Parser::OneOfRType orderedParse(GraphLang::Parser& p)
{
#define GRL_TOKEN_ENTRY(PRI,TOKT,TOKNAME)        ::GraphLang::Tokenizer::##TOKNAME,
#define GRL_TOKEN_ENTRY_LAST(PRI,TOKT,TOKNAME)  ::GraphLang::Tokenizer::##TOKNAME

    return p.oneOf<
#include "../GraphLang/parsepriority.h"    
    >();
}