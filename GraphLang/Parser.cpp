#include "stdafx.h"
#include "Parser.h"
#include <string>
#include <ios>

using namespace std;
using GraphLang::Tokenizer::TokenParseResult;
using GraphLang::Tokenizer::Token;

GraphLang::Parser::Parser(Tokenizer::InputStream& ins_): ins(ins_ >> noskipws)
{
}

GraphLang::Parser::~Parser()
{
}

GraphLang::Tokenizer::InputStream& GraphLang::Parser::i()
{
    return ins;
}

int GraphLang::Parser::readws()
{
    int count = 0;

    for (; true; count++)
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            ins >> c;
            continue;
        }

        if (c == '/' && peek(1) == '/')
        {
            string s;
            getline(ins, s);
            count += s.size() - 1;

            continue;
        }

        break;
    }

    return count;
}

int GraphLang::Parser::peek(int n)
{
    if (n == 0)
    {
        return ins.peek();
    }
    else
    {
        int c;
        ins.seekg(n, ::std::ios_base::cur);
        c = ins.peek();
        ins.seekg(-n, ::std::ios_base::cur);
        return c;
    }
}

GraphLang::Tokenizer::TokenParseResult GraphLang::Parser::require(char c)
{
    Checkpoint ch(*this);
    readws();
    char k;
    ins >> k;

    if (k != c)
    {
        return TokenParseResult{false, "Expected '" + string(1, c) + "'"};
    }

    ch.commit();
    return TokenParseResult{ true, "Couldn't find literal character"};
}

TokenParseResult GraphLang::Parser::require(Tokenizer::Token& tok)
{
    readws();
    Checkpoint ch(*this, &tok);

    auto r = tok.Parse(*this);

    if (get<0>(r))
    {
        ch.commit();
    }

    return r;
    
}

bool GraphLang::Parser::matchLiteral(std::string str)
{
    readws();
    Checkpoint ch(*this);

    for (char c: str)
    {
        char k;
        ins >> k;

        if (c != k)
        {
            return false;
        }
    }

    ch.commit();

    return true;
}

bool GraphLang::Parser::eof()
{
    readws();
    return ins.eof();
}

GraphLang::Parser::Checkpoint::Checkpoint(Parser& parser_, Tokenizer::Token* tok_):
    parser(parser_), tok(tok_)
{
    initial = parser.ins.tellg();
    if (tok != nullptr)
    {
        tok->startpos = parser.ins.tellg();
    }

}

void GraphLang::Parser::Checkpoint::commit()
{
    willCommit = true;
}

GraphLang::Parser::Checkpoint::~Checkpoint()
{
    if (tok != nullptr)
    {
        tok->endpos = parser.ins.tellg();
    }

    // If parsing failed somehow, 
    // pretend we didn't read anything.
    if (!willCommit)
    {
        parser.ins.seekg(initial);
    }
}