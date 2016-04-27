#// This is a really hacky way to give a compile time
#// list of the priority order to parse a line.
#//
#// Usage: Define a macro called GRL_TOKEN_ENTRY and GRL_TOKEN_ENTRY_LAST
#// that takes the arguments (absolute priority, tokenType, baseTokenName)
#//	GRL_TOKEN_ENTRY_LAST will be called on the last token.
#//
#// tokenType can be one of 
#//	* Statement -- for complete statements
#//	* Primitive -- For composite tokens like nodes or arrays.
#//	* Literal -- for just literals and identifiers. Very basic unit tokens.
#// baseTokenName will be unqualified.
#//
#//
#ifndef GRL_TOKEN_ENTRY
#	define GRL_TOKEN_ENTRY(absPriority, tokType, baseTokName) /*do nothing*/
#endif
#ifndef GRL_TOKEN_ENTRY_LAST
#	define GRL_TOKEN_ENTRY_LAST(absPriority, tokType, baseTokName) /*do nothing*/
#endif
#//
#//
GRL_TOKEN_ENTRY(		  1, Statement, RelationStatement)
GRL_TOKEN_ENTRY(		  2, Statement, NodeArrayDeclarationStatement)
GRL_TOKEN_ENTRY(		  3, Statement, SingleNodeStatement)
GRL_TOKEN_ENTRY(		101, Primitive, Relation)
GRL_TOKEN_ENTRY(		102, Primitive, NodeArray)
GRL_TOKEN_ENTRY(		103, Primitive, Attribute)
GRL_TOKEN_ENTRY(		200, Literal, Number)
GRL_TOKEN_ENTRY(		201, Literal, String)
GRL_TOKEN_ENTRY(		202, Literal, Identifier)
GRL_TOKEN_ENTRY_LAST(	203, Literal, LiteralValue)
#//