/**
   Copyright James H. Bullard 2009 

   Modifiecd by Bernhard Haubold, 14th September 2012

   This program is free software: you can redistribute it and/or modify
   it under the terms of the Lesser GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the Lesser GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/


%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"

#define yyterminate() return token::END

  extern FILE *yyin;
  void yyerror(const char *str); 
  int yywrap();
  int yylex(void);  
  Node *root, *tmpNode;
  char *output;

%}
%union{
	double numberVal;
	char *nameVal;
	Node *nPtr;
}

/* Bison declarations.  */
%token <numberVal>   NUMBER END
%token <nameVal>     WORD QUOTEDWORD
%type <nPtr>         tree subtree leaf internal branchlist branch
%type <nameVal>      name
%type <numberVal>    length 

%%  
/* The grammar follows.  */
tree: subtree  ';'	{
			   $$ = $1;
			   root = $1;
			   return(1);
			}
	| END		{
			  return(0);
			}
		;

subtree: leaf		{
			  $$ = $1;
			}
       | internal	{ 
			  $$ = $1; 
			}
       ;

leaf: name		{ 
			  tmpNode = newNode();
			  tmpNode->label = $1;
			  $$ = tmpNode;
			}
	;

internal: '(' branchlist ')' name 	{ 
					  $2->label = $4;
					  $$ = $2;
					}
        ;

branchlist: branch			{
					  tmpNode = newNode();
					  tmpNode->label = "";
					  addNode(tmpNode,$1);
					  $$ = tmpNode;
                                        }
          | branchlist  ','  branch  	{ 
					  addNode($1,$3);
					  $$ = $1; 
					}
          ;

branch: subtree length		        { 
					  $1->dist = $2;
					  $$ = $1; 
					}
      ;

name: /* empty */			{$$ = "";}
    | WORD				{ 
					  $$ = $1; 
					}
    | NUMBER                            {
                                          output = (char *)malloc(14*sizeof(char));
                                          sprintf(output,"%g",$1);
                                          $$ = output;
                                        }
    | QUOTEDWORD			{
					  $$ = $1;
					}
    ;

length: /* empty */			{ 
					  $$ = 1; 
					}
      | ':' NUMBER			{ 
					  $$ = $2; 
					}
      ;

%%

/* int main() */
/* { */
/*     printf(" ---- Welcome To the Newick Parser ---- \n"); */
/*     while (1) yyparse(); */
/*     return 1; */
/* }  */

void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
        return 1;
}

Node *getTree(){
	return root;
}

void setBisonFile(FILE *fp){
  yyin = fp;
}
