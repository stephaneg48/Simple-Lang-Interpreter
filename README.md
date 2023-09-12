# Simple-Lang-Interpreter  

A low-level C++ project that implements a lexer, parser and interpreter for a simple programming language.  

The purpose of this project was to get a better understanding of programming language semantics and implementations, as well as weighing the pros and cons of different languages in multiple use cases. **Several sample programs are provided to allow a better understanding of the syntax of this simple language.**

The executable for this project can be compiled using `g++` or similar:  

> g++ -g driver.cpp -o (desired executable name)

The executable must be used in a terminal and takes one text file (assumed to be syntatically correct) as an argument:  

> ./(desired executable name).exe (file name).txt

In this case, the executable and text file are assumed to exist in the same directory, but this is not required.  

The grammar for this language is defined in the following manner:  

`Program` → **begin** `StatementList` **end**  
`StatementList` → `Statement`**;** { `Statement`**;** (...) }  
`Statement` → `PrintStatement` OR `AssignStatement` OR `IfStatement`  
`PrintStmt` → **print** `ExprList`  
`IfStmt` → **if** (`Expression`) **then** `Statement`  
`AssignStatement` → `Variable` = `Expression`  
`ExpressionList` → `Expression` {, `Expression`, (...)}  
`Expression` → `Term` {( + OR - ) `Term`}  
`Term` → `Factor` { ( * OR / ) `Factor` }  
`Variable` → `ident`  
`Factor` → `ident` OR `iconst` OR `rconst` OR `sconst` OR (`Expression`)  

The simple language is constrained by the following:  

1. Three data types: **Integer** (`iconst`), **Real** (`rconst`) and **String** (`sconst`).  
2. The (+) and (-) operators in an expression represent addition and subtraction.  
3. The (*) and (/) operators in a term represent multiplication and division.  
4. The above arithmetic operators are left associative.  
5. Multiplication and division have higher precedence than addition and subtraction.  
6. The (=) operator in an assignment statement assigns a value to a variable. It evaluates the expression on the right-hand side, determines the type of the value and saves its value in memory associated with the left-hand side (an identifier, or `ident`). If the identifier does not exist, it is created. Otherwise, its value is replaced.  
7. The type of a variable is the type of the value assigned to it in an assignment statement. So, either **Integer**, **Real**, or **String** type; this is determined from the first time it has been assigned a value.  
8. Any reassignments for the same variable have to be for either a value of the same type or a compatible one. This means it is an error to assign a numeric value to a variable of a string type, or to assign a string value to a numeric variable (i.e., **Integer** or **Real**). However, an integer variable can be assigned a real value, and a real variable can be assigned an integer value. Conversion of the value to the type of the variable is applied at run-time.  
9. An if statement evaluates an expression, which must be an integer. If the expression value is **nonzero**, then the statement is executed; otherwise, it is not.  
10. An if statement whose expression is not of an integer type will yield an error.  
11. A print statement evaluates a list of expressions and prints their values in sequential order, followed by a newline.  
12. Arithmetic operations are performed upon two numeric values (i.e., integer or real) only. If the operands are of the same type, the type of the result is the same type as the operator’s operands. Otherwise, the type of the result is **real**. It is an error to have a string as an operand to any of the arithmetic operations.  
13. It is an error to use a variable in an expression before it has been assigned.  

