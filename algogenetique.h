#ifndef ALGOGENETIQUE_H
#define ALGOGENETIQUE_H

//SD rajoutees
typedef struct tab {
  int dim;//nb d'elements
  int *tab;//tableau
} tab_t2;

tab_t2 dpx(instance_t instance,tab_t2 dad,tab_t2 mum);
int indice_ppv(instance_t instance,tab_t2 tab,tab_t2 fille,int indDepart,int voisins_restants);

#endif
