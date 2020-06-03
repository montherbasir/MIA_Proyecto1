%option noyywrap
%option caseless
%option debug
%option outfile="scanner.cpp"
%option c++
%{
#include <iostream>
%}

comentario "#".*
digito [0-9]
Numero {digito}+

%%
[ \n\t\r]+            { /*Se ignoran*/ }
{Numero}      { std::cout <<yytext<<"  "<<"num"<<std::endl;return 'a';}
[-]            { std::cout <<yytext<<"  "<<"guion"<<std::endl;return 'a';}
[=]            { std::cout <<yytext<<"  "<<"igual"<<std::endl; return 'a'; }
"mkdisk"      { std::cout <<yytext<<"  "<<"mkdisk"<<std::endl; return 'a';}
"rmdisk"      { std::cout <<yytext<<"  "<<"rmdisk"<<std::endl;return 'a';}
"fdisk"        { std::cout <<yytext<<"  "<<"fdisk"<<std::endl; return 'a';}
"mount"        {  std::cout <<yytext<<"  "<<"mount"<<std::endl; return 'a';}
"unmount"      { std::cout <<yytext<<"  "<<"unmount"<<std::endl;return 'a';}
"rep"         { std::cout <<yytext<<"  "<<"rep"<<std::endl;return 'a'; }
"size"         { std::cout <<yytext<<"  "<<"size"<<std::endl; return 'a'; }
"unit"         { std::cout <<yytext<<"  "<<"unit"<<std::endl; return 'a'; }
"path"         { std::cout <<yytext<<"  "<<"path"<<std::endl;return 'a';}
"fit"           { std::cout <<yytext<<"  "<<"fit"<<std::endl;return 'a';}
"type"          { std::cout <<yytext<<"  "<<"type"<<std::endl; return 'a';}
"name"          { std::cout <<yytext<<"  "<<"name"<<std::endl;return 'a';}
"delete"       {std::cout <<yytext<<"  "<<"delete"<<std::endl; return 'a';}
"add"          {  std::cout <<yytext<<"  "<<"add"<<std::endl;return 'a';}
"id"            { std::cout <<yytext<<"  "<<"id"<<std::endl;  return 'a';}
\"[^\"]*\"      { std::cout <<yytext<<"  "<<"cadena"<<std::endl; return 'a';}
{comentario}    { /*Se ignoran*/std::cout <<yytext<<"  "<<"comentario"<<std::endl; return 'a';}
[_a-zA-Z][_a-zA-Z0-9]*  { std::cout <<yytext<<"  "<<"id"<<std::endl; return 'a'; }
"/"[^ \n\t\r]+     { std::cout <<yytext<<"  "<<"ruta"<<std::endl; return 'a'; }

%%
