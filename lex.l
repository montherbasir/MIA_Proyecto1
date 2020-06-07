%option noyywrap
%option caseless
%option debug
%option c++


%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "gram.tab.hh"
  int linea=0;
%}

comentario "#".*
digito [0-9]
Numero {digito}+

%%

{Numero}      { sscanf(yytext, "%s", yylval.TEXT); return (num);}
[-]            {  return (guion); }
[=]            {   return (igual); }
"mkdisk"      {   return (mkdisk); }
"exec"      {   return (exec); }
"rmdisk"      { return (rmdisk);}
"fdisk"        { return (fdisk);}
"mount"        {  return (mount);}
"unmount"      {  return (unmount);}
"rep"         {  return (rep); }
"size"         { return (size); }
"unit"         {  return (unit);}
"path"         { return (path); }
"fit"           {  return (fit); }
"type"          {  return (type); }
"name"          {  return (name); }
"delete"       {  return (del); }
"add"          {   return (add); }
"exit_"            {  return (exit_); }
"id"            {  return (id); }
\"[^\"]*\"      { sscanf(yytext, "%s", yylval.TEXT); return (cadena); }
{comentario}    { /*Se ignoran*/}
"/"[^ \n\t\r]+     { sscanf(yytext, "%s", yylval.TEXT);  return (ruta);}
[_a-zA-Z][_a-zA-Z0-9]*  { sscanf(yytext, "%s", yylval.TEXT); return (identifier); }
[ \n\t\r]+            { /*Se ignoran*/ }
%%
