/**
   Copyright James H. Bullard 2009

   Modified by Bernhard Haubold on 14th September 2012:
	- added QUOTEDWORD using the code given in the flex documentation
	- added the comments delineated by square brackets; this is required
	  by Gary Olson's version of the Newick Standard:
	  http://evolution.genetics.washington.edu/phylip/newick_doc.html

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
#include "tree.h"
#include "y.tab.h"

#define YY_NO_INPUT

%}
%option              nounput
%%
([a-zA-Z]+[-._a-zA-Z0-9]*)              {
				       		       yylval.nameVal = strdup(yytext);
						       return WORD; 
						     }
\'([^\']+)\'                                           {
							char *buf;
							int l;
							buf = strdup(yytext);
							l = strlen(buf);
							buf[l-1] = '\0';
							buf++;
							yylval.nameVal = buf;
							return QUOTEDWORD;
						     }

(\+|-)?([0-9]+\.?[0-9]*|\.[0-9]+)([eE](\+|-)?[0-9]+)?                              {
						       yylval.numberVal = atof(yytext);
						       return NUMBER;
						  }

[-()<>=+*/;{}:,.]				       return *yytext;

[ \t\n]+					        ;       /* ignore whitespace */
"["[^\]]*"]"						;       /* ignore comments */ 

.                                     		       yyerror(yytext);
<<EOF>>						       return END;
%%



