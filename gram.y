
%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <unistd.h>

  extern int yylex(void);
  extern char *yytext;
  extern int linea;
  extern FILE *yyin;

  void yyerror(const char *s);

  char* ruta_ar;
%}

%define parse.trace
%define parse.error verbose
%union{
//se especifican los tipo de valores para los no terminales y lo terminales
char TEXT[250];
}


%token mkdisk
%token rmdisk
%token fdisk
%token mount
%token unmount
%token rep
%token size
%token unit
%token path
%token fit
%token type
%token name
%token del
%token add
%token id
%token cadena
%token identifier
%token ruta
%token num
%token guion
%token igual
%token exit_

%type<TEXT> ruta
%type<TEXT> identifier
%type<TEXT> num
%type<TEXT> rep
%start L
%%

L: mkdisk guion path igual ruta {return 0;}
  | rep ruta{ruta_ar=$2; return -3;}
  | exit_ {return -1;}
  |
     /* fclose(stdin);
     yyin=fopen("temp.txt","r");
     yyparse();

     yyin=fdopen(0, "r");
     yyparse();} */
;
%%

void yyerror(const char *s)
{
  printf("%s \n",s);
}

int main(int argc,char **argv)
{
  int hayr=0;
  while(hayr==0){
    yyin=stdin;
    hayr=yyparse();
    if(hayr==-3){
      yyin = fopen(ruta_ar,"r");
      hayr=yyparse();
    }
  }
  return 0;
}
