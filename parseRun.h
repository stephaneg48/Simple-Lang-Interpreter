#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
#include <map>
#include "val.h"
using namespace std;

#include "lex.h"

map<string, bool> defVar;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) 
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) 
			abort();
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

extern bool Prog(istream& in, int& line);
extern bool StmtList(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line);
extern bool IfStmt(istream& in, int& line);
extern bool Var(istream& in, int& line, LexItem & tok);
extern bool AssignStmt(istream& in, int& line);
extern bool ExprList(istream& in, int& line);
extern bool Expr(istream& in, int& line, Value & retVal);
extern bool Term(istream& in, int& line, Value & retVal);
extern bool Factor(istream& in, int& line, Value & retVal);

#endif /* PARSE_H_ */
