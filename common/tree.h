/***** tree.h *************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 10:35:29 2012
 **************************************************/
#ifndef TREE
#define TREE

/* define tree node */
typedef struct node{
  struct node *left;
  struct node *right;
  struct node *parent;
  float dist;
  char *label;
  int id;
  int l;              /* number of leaves in subtree */
} Node;

Node *parseTree();
void freeTree(Node *np);
Node *newNode();
void printNewick(Node *np);
void printDot(Node *node);
Node *addNode(Node *parent, Node *child);
void addChild(Node *parent, Node *child);
int rmChild(Node *parent, Node *child);
int deg(Node *np);
void countLeaves(Node *np);
Node *lca(Node *n1, Node *n2);
double ancDist(Node *node, Node *ancestor);

/* flex/bison declarations */
void setBisonFile(FILE *fp);
void yyerror(const char *str); 
Node *getTree();
int yyparse();

#endif
