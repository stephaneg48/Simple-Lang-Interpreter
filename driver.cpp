// driver for simple language interpreter
#include <iostream>
#include <fstream>
#include "parse.cpp"
using namespace std;

// variables
ifstream fin;
int linenumber = 1;

void argument_parsing(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    argument_parsing(argc,argv);
    bool status = Prog(fin, linenumber);
    if (status)
    {
        cout << "Successful execution" << endl;
    }
    else
    {
        cout << "Unsuccessful Interpretation" << endl;
        cout << "Number of Errors: " << error_count;
    }
    return 0;
}

void argument_parsing(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "no file provided...";
        exit(-1);
    }
    fin.open(argv[1]);
    // a valid file should always be the last and only argument
    if (!fin.is_open()) // this includes any flags that are not valid files (i.e. not .txt)...
    {
        cout << "CANNOT OPEN THE FILE " << argv[1];
        exit(-1);
    }
    for (int i = 1; i < argc; i++)
    {
        string argString = argv[i];
        if (i > 1)
        {
            cout << "ONLY ONE FILE NAME ALLOWED";
            exit(-1);
        }
    }
    cout << "file opened" << endl;
}