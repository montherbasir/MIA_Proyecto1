
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <FlexLexer.h>
using namespace std;
int main(int argc, char *argv[])
{

    cout << "hola" << endl;
    ifstream in("Bichulga.txt");
    yyFlexLexer* lex = new yyFlexLexer(&in);
    while(lex->yylex()!=0)
        ;
    return 0;
}
