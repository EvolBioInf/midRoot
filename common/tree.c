/***** tree.c *************************************
 * Description: 
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Wed Sep 12 10:38:36 2012
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "tree.h"
#include "eprintf.h"
#include "y.tab.h"

int nodeId = 0;

Node *parseTree(){
  nodeId = 0;
  if(yyparse())
    return getTree();
  else
    return NULL;
}

Node *addNode(Node *curN, Node *newN){
  Node *parent;

  if(curN == NULL){
    curN = newN;
  }else if(curN->left == NULL){
    curN->left = newN;
    newN->parent = curN;
    curN = newN;
  }else{
    parent = curN;
    curN = curN->left;
    while(curN->right != NULL)
      curN = curN->right;
    curN->right = newN;
    curN = curN->right;
    curN->parent = parent;
  }

  return curN;
}

/* addChild takes parent and adds child to its existing children */
void addChild(Node *parent, Node *child) {
  Node *np = parent;
  child->parent = parent;
  if (np->left == NULL) {
    np->left = child;
    return;
  }
  np = np->left;
  while (np->right != NULL)
    np = np->right;
  np->right = child;
}

/* rmChild removes child from the children of parent; it returns 1
 * upon success, 0 upon failure.
 */
int rmChild(Node *parent, Node *child) {
  if (parent == NULL)
    return 0;
  Node *c = parent->left;
  Node *p = c;
  if (c->id == child->id) {
    parent->left = c->right;
    c->right = NULL;
    c->parent = NULL;
    return 1;
  }
  while (c != NULL) {
    c = c->right;
    if (c->id == child->id) {
      p->right = c->right;
      c->right = NULL;
      c->parent = NULL;
      return 1;
    }
    p = c;
  }
  return 0;
}

Node *newNode(){
  Node *np;

  np = (Node *)emalloc(sizeof(Node));
  np->left = NULL;
  np->right = NULL;
  np->parent = NULL;
  np->label = NULL;

  np->dist = 0;
  np->id = ++nodeId;
  return np;
}

void countLeaves(Node *np){
  Node *cp;

  if(np != NULL){
    countLeaves(np->left);
    countLeaves(np->right);
    if(np->left == NULL)
      np->l = 1;
    else{
      np->l = 0;
      cp = np->left;
      while(cp != NULL){
	np->l += cp->l;
	cp = cp->right;
      }
    }
  }
}

void freeTree(Node *np) {
  if (np != NULL) {
    freeTree(np->left);
    freeTree(np->right);
    free(np);
    np = NULL;
  }
}

int deg(Node *np){
  int d;
  Node *n;

  if(np->parent)
    d = 1;
  else
    d = 0;
  n = np->left;
  while(n != NULL){
    d++;
    n = n->right;
  }
  return d;
}

void printNode(Node *node){
  if(node->label)
    printf("%s",node->label);
  if(node->dist > -FLT_MAX && node->parent)
    printf(":%.6f",node->dist);
}

void printSubtree(Node *node){
  Node *child;
  if(node){
    if(node->left){  /* internal node */
      printf("(");
      child = node->left;
      while(child->right){
	printSubtree(child);
	printf(",");
	child = child->right;
      }
      printSubtree(child);
      printf(")");
      printNode(node);
    }else            /* leaf */
      printNode(node);
  }
}

void printNewick(Node *node){
  printSubtree(node);
  printf(";\n");
}

/* recPrintDot recursively prints nodes in graphviz dot notation */
void recPrintDot(Node *n) {
  if (n != NULL) {
    Node *p = n->left;
    while (p != NULL) {
      if (p->label != NULL && strlen(p->label) > 0)
	printf("  %d -> %s\n", n->id, p->label);
      else
	printf("  %d -> %d\n", n->id, p->id);
      p = p->right;
    }
    recPrintDot(n->left);
    recPrintDot(n->right);
  }
}

/* printDot prints a tree in graphviz dot notation */
void printDot(Node *node) {
  printf("digraph DG {\n");
  printf("  edge [arrowhead = none]\n");
  recPrintDot(node);
  printf("}\n");
}

/* zeroPath sets the leaf counters from node to root zero */
void zeroPath(Node *n) {
  Node *np = n;
  while (np != NULL) {
    np->l = 0;
    np = np->parent;
  }
}

/* lca returns the lowest common ancestor between two nodes or NULL upon failure. */
Node *lca(Node *n1, Node *n2) {
  Node *np1 = n1;
  Node *np2 = n2;
  zeroPath(n1);
  zeroPath(n2);
  while (np1 != NULL) {
    np1->l = 1;
    np1 = np1->parent;
  }
  while (np2 != NULL) {
    np2->l++;
    if (np2->l == 2)
      return np2;
    np2 = np2->parent;
  }
  return NULL;
}

/* upDist returns the distance between a node and one of its ancestors */
double ancDist(Node *node, Node *ancestor) {
  double s = 0.;
  Node *np = node;
  while (np->id != ancestor->id) {
    s += np->dist;
    np = np->parent;
  }
  return s;
}

