
%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <unistd.h>
  #include <cstring>

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
char* TEXT;
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
%token exec

%type<TEXT> ruta
%type<TEXT> identifier
%type<TEXT> num
%type<TEXT> rep


%start L
%%

L: %empty
  | mkdisk MKD MKD1{return 0;}
  | rmdisk RUTA
  | fdisk FD FD1
  | mount NAME RUTA
  | mount RUTA NAME
  | unmount guion id igual identifier
  | exec RUTA{ return -3;}
  | exit_ {return -1;}
  ;

RUTA : guion path igual RU
    ;

RU: cadena {}
    | ruta
    ;

SIZE: guion size igual num
    ;

UNIT: guion unit igual identifier
    ;

FIT: guion fit igual identifier
    ;

NAME: guion name igual NA
    ;

NA: cadena
    | identifier
    ;

MKD: SIZE
    | FIT
    | UNIT
    | RUTA
    ;

MKD1: %empty
    | MKD MKD1
    ;

FD: SIZE
    | FIT
    | UNIT
    | RUTA
    | guion type igual identifier
    | guion del igual identifier
    | NAME
    | guion add igual num
    ;

FD1: %empty
    | FD FD1
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
    char string[300];
    char *po=string;
    FILE *newstdin1;
        printf("Ingrese un comando: \n");
        scanf(" %[^\n]s",po);
        newstdin1 = fmemopen (po, strlen (po), "r");
    yyin=newstdin1;
    hayr=yyparse();

    fclose(newstdin1);
    if(hayr==-3){
          FILE *fp = fopen("Bichulga.txt", "r");
          if(fp == NULL) {
             perror("error");
             exit(1);
          }
          char chunk[300];
          FILE *newstdin;
          char* po1 = chunk;
          while(fgets(chunk, sizeof(chunk), fp) != NULL) {
            newstdin = fmemopen (po1, strlen (po1), "r");
            yyin=newstdin;
            hayr=yyparse();
            if(hayr!=0){break;}
          }
          fclose(newstdin);
          fclose(fp);
          /* yyin = fopen("Bichulga.txt","r"); */
    }
  }

  return 0;
}
