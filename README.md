# Simple-Lang-Interpreter
A low-level C++ project that implements a lexer, parser and interpreter for a simple programming language.

The grammar for this language is defined in the following manner:

`Program` → **begin** StatementList **end**  
`StatementList` → `Statement`**;** { `Statement`**;** (...) }  
`Statement` → `PrintStatement` OR `AssignStatement` OR `IfStatement`  
`PrintStmt` → print `ExprList`