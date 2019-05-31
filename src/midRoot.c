#include "interface.h"
#include "eprintf.h"
#include <stdlib.h>
#include "tree.h"
#include <assert.h>
void scanFile(FILE *fp, Args *args);
Node *midRoot(Node *r);
Node **getLeaves(Node *r, Node **leaves, int *n);
void parentToChild(Node *n);
void scanFile(FILE *fp, Args *args) {
  setBisonFile(fp);
  Node *r, *m;
  while ((r = parseTree()) != NULL) {
    m = midRoot(r);
    printNewick(m);
    freeTree(m);
  }
}
Node *midRoot(Node *r) {
  int n = 0;
  Node **leaves = NULL;
  leaves = getLeaves(r, leaves, &n);
  double **dlca = (double **)emalloc(n * sizeof(double *));
  for (int i = 0; i < n; i++)
    dlca[i] = (double *)emalloc(n * sizeof(double));
  for (int i = 0; i < n - 1; i++)
    for (int j = i; j < n; j++) {
      Node *la = lca(leaves[i], leaves[j]);
      dlca[i][j] = ancDist(leaves[i], la);
      dlca[j][i] = ancDist(leaves[j], la);
    }
  double d, dm = 0;
  int im = 0, jm = 0;
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      d = dlca[i][j] + dlca[j][i];
      if (d > dm) {
          dm = d;
          im = i;
          jm = j;
      }
    }
  }

  Node *c, *p;  /* child, parent */
  double dh = dm / 2.;
  if (dlca[im][jm] > dh)
    c = leaves[im];
  else
    c = leaves[jm];
  double s = c->dist;
  while(c->parent && s < dh) {
    c = c->parent;
    s += c->dist;
  }
  p = c->parent;
  Node *R = newNode(); /* new root */
  addChild(p, R);
  rmChild(p, c);
  addChild(R, c);
  float x2 = s - dm / 2.;
  float x1 = c->dist - x2;
  c->dist = x1;
  R->dist = x2;
  parentToChild(R);
  r = R;
  free(leaves);
  for (int i = 0; i < n; i++)
    free(dlca[i]);
  free(dlca);
  return r;
}
Node **getLeaves(Node *r, Node **leaves, int *n) {
  if (r != NULL) {
    leaves = getLeaves(r->left, leaves, n);
    leaves = getLeaves(r->right, leaves, n);
    if (r->left == NULL) {
        *n = *n + 1;
        leaves = (Node **)erealloc(leaves, *n * sizeof(Node *));
        leaves[*n - 1] = r;
    }
  }
  return leaves;
}
  void parentToChild(Node *n) {
    if(n->parent->parent != NULL)
        parentToChild(n->parent);
    Node *p = n->parent;
    rmChild(p, n);
    addChild(n, p);
    p->dist = n->dist;
  }
int main(int argc, char **argv) {
  Args *args = getArgs(argc, argv);
  setProgName(argv[0]);
  if (args->v)
    printSplash(args);
  if (args->h || args->err)
    printUsage();
  if (args->nf == 0) {
    FILE *fp = stdin;
    scanFile(fp, args);
  } else {
    for (int i = 0; i < args->nf; i++) {
      FILE *fp = efopen(args->fi[i], "r");
      scanFile(fp, args);
      fclose(fp);
    }
  }
  freeArgs(args);
  free(getProgName());
  return 0;
}
