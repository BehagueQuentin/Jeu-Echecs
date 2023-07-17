//########################################################################################################################################################// 
//############################################################//     Arbre des ouvertures    //###########################################################//
//########################################################################################################################################################//

#ifndef OUVERTURE_H
#define OUVERTURE_H

#define NbCoupsMoyen 10
#define NBouvertures 50
#define ProfondeurOuverture 12

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Importation des bibliothèques :

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "deplacement.h"

struct arb {int* coup; struct arb** sous_arbres; int nb_fils;};
typedef struct arb arb;

int ouvertures [NBouvertures][12][4];

arb* init_arb () ;

void ajouter_branche (arb* arbre, int* coup);

void afficher_arb (arb* arbre);

void liberer_arb (arb* arbre);

void ajouter_fils_arb (arb* arbre, arb* arbre_fils);

int est_fils_coup (arb* arbre, int* coup);

bool dans_arb_partie (arb* arbre, ListeCoups* l, int n);

bool dans_arb_partie (arb* arbre, ListeCoups* l, int n);

bool appel_dans_arb_partie (arb* arbre, ListeCoups* l);

ListeCoups* coups_suivant(arb* arbre, ListeCoups* l, int n);

ListeCoups* appel_coups_suivant(arb* arbre, ListeCoups* l);

int dans_arbre (arb* arbre, int* coups);

arb* generer_arb_tableau (int ouverture[ProfondeurOuverture][4]);

void ajouter_fils_arb_sans_doublons (arb* arbre, arb* ouverture);

arb* creer_arbre_ouvertures (int ouvertures[NBouvertures][ProfondeurOuverture][4]);

int* coup_ouverture (ListeCoups* partie, arb* ouvertures);

#endif