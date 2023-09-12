// recursive-descent parser + interpreter
#include <iostream>
#include <map>
#include <string>
#include "lex.h"
#include "parseRun.h"
#include "lex.cpp"

using namespace std;

inline bool Prog(istream& in, int& line)
{
    bool status;
    LexItem t = Parser::GetNextToken(in,line);
    if (t.GetToken() == BEGIN)
    {
        status = StmtList(in,line);
        t = Parser::GetNextToken(in,line);
        if (t.GetToken() == END)
            status = true;
        else if (t.GetToken() == DONE)
        {
            ParseError(line, "missing END");
            status = false;
        }
    }
    else
    {
        ParseError(line, "missing BEGIN");
        status = false;
    }
    return status;
}

inline bool StmtList(istream& in, int& line)
{
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);
    status = Stmt(in,line);
    if (status)
    {
        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == SCOMA)
            status = true;
        else
        {
            ParseError(line, "missing SCOMA");
            return false;
        }
        status = Stmt(in,line);
        while (status)
        {
            t = Parser::GetNextToken(in, line);
            if (t.GetToken() == SCOMA)
                status = Stmt(in,line);
            else
            {
                ParseError(line, "missing SCOMA");
                return false;
            }
        }
    }
    else if (t.GetToken() == END || t.GetToken() == DONE)
    {
        ParseError(line, "no statements found");
        return false;
    }
    return status;
}

inline bool Stmt(istream& in, int& line)
{
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == END)
    {
        Parser::PushBackToken(t);
        return false;
    }
    switch (t.GetToken())
    {
        default:
            ParseError(line, "invalid statement");
            return false;
        case PRINT:
            status = PrintStmt(in,line);
            break;
        case IF:
            Parser::PushBackToken(t);
            status = IfStmt(in,line);
            break;
        case IDENT:
            Parser::PushBackToken(t);
            status = AssignStmt(in,line);
            break;
    }
    return status;
}

inline bool PrintStmt(istream& in, int& line)
{
    ValQue = new queue<Value>;
    bool status = ExprList(in, line);
    if (!status)
    {
        ParseError(line, "Missing expression(s) after print");
        // empty the queue and delete
        while (!(*ValQue).empty())
        {
            ValQue->pop();
        }
        delete ValQue;
        status = false;
    }
    else
        status = true;

    LexItem t = Parser::GetNextToken(in,line);
    if (t.GetToken() == SCOMA)
    {
        while (!(*ValQue).empty())
        {
            Value nextVal = (*ValQue).front();
            cout << nextVal;
            ValQue->pop();
        }
        cout << endl;
    }
    Parser::PushBackToken(t);
    return status;
}

inline bool AssignStmt(istream& in, int& line)
{
    bool status;
    Value val;
    LexItem t = Parser::GetNextToken(in, line);
    string identstr = t.GetLexeme();
    Parser::PushBackToken(t);
    if (Var(in,line,t))
    {
        t = Parser::GetNextToken(in, line);
        if (t.GetToken() == EQ)
        {
            status = Expr(in,line,val);

            if (status)
                status = true;
            else
            {
                ParseError(line, "Missing expression after EQ sign");
                status = false;
            }
        }
        else
        {
            ParseError(line,"missing EQ sign in assignstmt");
            status = false;
        }
    }
    else
    {
        ParseError(line, "missing expression in assignstmt");
        status = false;
    }
    if (symbolTable[identstr].GetType() == val.GetType() || (symbolTable[identstr].GetType() != VSTR))
    {
        if (symbolTable[identstr].GetType()==VINT)
        {
            if (val.GetType()==VREAL)
            {
                val = Value(int(val.GetReal()));
            }
        }
        symbolTable[identstr] = val;
    }
    else
    {
        ParseError(line, "Runtime Error: Illegal Assignment Operation");
        status = false;
    }
    return status;
}

inline bool IfStmt(istream& in, int& line){
    bool status;
    Value val;
    LexItem t = Parser::GetNextToken(in,line);
    if(t.GetToken() == IF)
    {
        t = Parser::GetNextToken(in,line);
        if (t.GetToken() == LPAREN)
        {
            status = Expr(in,line,val);
            if (status)
            {
                if (val.IsInt())
                {
                    t = Parser::GetNextToken(in,line);
                    if (t.GetToken() == RPAREN)
                    {
                        status = true;
                    }
                    else
                    {
                        ParseError(line, "Missing right parenthesis after expression");
                        return false;
                    }
                }
                else
                {
                    ParseError(line, "Illegal IfStmt Expression Type");
                    return false;
                }
            }
            else
            {
                ParseError(line, "Missing expression in IF statement");
                return false;
            }
        }
        else
        {
            ParseError(line, "Missing left parenthesis before expression");
            return false;
        }

        t = Parser::GetNextToken(in,line);
        if (t.GetToken() == THEN)
        {
            if (val.GetInt() == 0)
            {
                while (t.GetToken() != SCOMA)
                {
                    t = Parser::GetNextToken(in,line);
                }
                Parser::PushBackToken(t);
                return true;
            }
            status = Stmt(in, line);
        }
        else
        {
            ParseError(line, "missing THEN in IF statement");
            return false;
        }
    }
    return status;
}

inline bool Var(istream& in, int& line, LexItem & t)
{
    //cout << "in Var " << endl;
    string identstr;
    t = Parser::GetNextToken(in,line);

    if(t.GetToken() == IDENT)
    {
        identstr=t.GetLexeme();
        if (!(defVar[identstr]))
        {
            defVar[identstr]=true;
            Value newVal = identstr;
            symbolTable[identstr];
        }
        return true;
    }
    return false;
}

inline bool ExprList(istream& in, int& line)
{
    bool status;
    Value val;
    LexItem t = Parser::GetNextToken(in,line);
    Parser::PushBackToken(t);
    status = Expr(in,line,val);
    if (status)
    {
        t = Parser::GetNextToken(in, line);
        while (t.GetToken() == COMA)
        {
            if (Expr(in,line,val))
            {
                //cout << "found more than one expression" << endl;
                t = Parser::GetNextToken(in, line);
                status = true;
            }
        }
    }
    if (t.GetToken() == ERR)
    {
        ParseError(line, "missing expression");
        return false;
    }
    Parser::PushBackToken(t);
    return status;
}

inline bool Expr(istream& in, int& line, Value & retVal)
{
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);
    status = Term(in,line,retVal);
    if (status)
    {
        t = Parser::GetNextToken(in, line);

        while (t.GetToken() == PLUS || t.GetToken() == MINUS)
        {
            Value nextVal;
            status = Term(in,line,nextVal);
            if (status)
            {
                if (t.GetToken() == PLUS)
                {
                    if ((retVal.IsInt() || retVal.IsReal()) && (nextVal.IsInt() || nextVal.IsReal()))
                    {
                        //cout << "values to add were either ints or reals - OK" << endl;
                        retVal = retVal + nextVal;
                    }
                    else if (retVal.GetType() == VSTR && nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to add that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal String operation");
                        return false;
                    }
                    else if (retVal.GetType() == VSTR || nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to add that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal Mixed Type operation");
                        return false;
                    }
                }
                if (t.GetToken() == MINUS)
                {
                    status = Term(in,line,nextVal);
                    if ((retVal.IsInt() || retVal.IsReal()) && (nextVal.IsInt() || nextVal.IsReal()))
                    {
                        //cout << "values to subtract were either ints or reals - OK" << endl;
                        retVal = retVal - nextVal;
                    }
                    else if (retVal.GetType() == VSTR && nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to subtract that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal String operation");
                        return false;
                    }
                    else if (retVal.GetType() == VSTR || nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to subtract that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal Mixed Type operation");
                        return false;
                    }
                }
                t = Parser::GetNextToken(in, line);
                status = true;
            }
        }
        if (t.GetToken() == ERR)
        {
            ParseError(line, "unrecognized input ("+t.GetLexeme()+")");
            return false;
        }
    }
    (*ValQue).push(retVal);
    if (status)
    {
        Parser::PushBackToken(t);
    }
    return status;
}

inline bool Term(istream& in, int& line, Value & retVal)
{
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    Parser::PushBackToken(t);
    status = Factor(in,line,retVal);
    if (status)
    {
        t = Parser::GetNextToken(in, line);
        while (t.GetToken() == MULT || t.GetToken() == DIV)
        {
            Value nextVal;
            status = Factor(in,line,nextVal);
            if (status)
            {
                if (t.GetToken() == MULT)
                {
                    if ((retVal.IsInt() || retVal.IsReal()) && (nextVal.IsInt() || nextVal.IsReal()))
                    {
                        //cout << "values to multiply were either ints or reals - OK" << endl;
                        retVal = retVal * nextVal;
                    }
                    else if (retVal.GetType() == VSTR && nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to multiply that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal String operation");
                        return false;
                    }
                    else if (retVal.GetType() == VSTR || nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to multiply that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal Mixed Type operation");
                        return false;
                    }
                }
                if (t.GetToken() == DIV)
                {
                    if ((retVal.IsInt() || retVal.IsReal()) && (nextVal.IsInt() || nextVal.IsReal()))
                    {
                        //cout << "values to divide were either ints or reals - OK" << endl;
                        retVal = retVal / nextVal;
                    }
                    else if (retVal.GetType() == VSTR && nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to divide that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal String operation");
                        return false;
                    }
                    else if (retVal.GetType() == VSTR || nextVal.GetType() == VSTR)
                    {
                        //cout << "found something to divide that was a string" << endl;
                        ParseError(line, "Runtime Error: Illegal Mixed Type operation");
                        return false;
                    }
                }
                t = Parser::GetNextToken(in, line);
                status = true;
            }
            else
            {
                // no longer in a factor?
                //cout << "no more mult. or div. signs" << endl;
            }
        }
        if (t.GetToken() == ERR)
        {
            ParseError(line, "unrecognized input ("+t.GetLexeme()+")");
            return false;
        }
    }
    if (status)
        Parser::PushBackToken(t);

    return status;
}

inline bool Factor(istream& in, int& line, Value & retVal)
{
    bool status;
    LexItem t = Parser::GetNextToken(in,line);

    if (t.GetToken() == IDENT)
    {
        if (defVar[t.GetLexeme()])
        {
            retVal = symbolTable.find(t.GetLexeme())->second;
            status = true;
            return status;
        }
        else
        {
            ParseError(line, "variable not previously assigned ("+t.GetLexeme()+")");
            Parser::PushBackToken(t);
            return false;
        }
    }
    else if (t.GetToken() == ICONST)
    {
        int val;
        val = stoi(t.GetLexeme());
        Value newVal(val);
        retVal = newVal;
        status = true;
        return status;
    }
    else if (t.GetToken() == RCONST)
    {
        float val;
        val = stof(t.GetLexeme());
        Value newVal(val);
        retVal = newVal;
        status = true;
        return status;
    }
    else if (t.GetToken() == SCONST)
    {
        string val;
        val = t.GetLexeme();
        Value newVal(val);
        retVal = newVal;
        status = true;
        return status;
    }
    else if (t.GetToken() == LPAREN)
    {
        status = Expr(in,line,retVal);
        if (status)
        {
            t = Parser::GetNextToken(in, line);
            if (t.GetToken() == RPAREN)
            {
                status = true;
                return status;
            }
            else
            {
                ParseError(line, "missing right parenthesis after expression");
                status = false;
                return status;
            }
        }
        else
        {
            ParseError(line, "missing expression inside IF");
            return false;
        }
    }
    else
    {
        ParseError(line, "unknown factor ("+t.GetLexeme()+")");
        return false;
    }
}

