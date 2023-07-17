//########################################################################################################################################################// 
//######################################################//     Implémentation des déplacements    //######################################################//
//########################################################################################################################################################//

#ifndef DEPLACEMENTPIECE_H
#define DEPLACEMENTPIECE_H


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Importation des bibliothèques :

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Variables globales de structures :

#define NbCoupsPossibleMax 300

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Valeur des pièces et du trait :

#define CaseVide 0
#define PionB 1
#define PionN -1
#define CavalierB 2
#define CavalierN -2
#define FouB 3
#define FouN -3
#define TourB 4
#define TourN -4
#define DameB 5
#define DameN -5
#define RoiB 6
#define RoiN -6
#define TraitBlanc 1
#define TraitNoir -1


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Structure de tableau dynamique :

struct liste_coups {int taille; int occupe; int** coups;};
typedef struct liste_coups ListeCoups;

int** creer_plateau();

void liberer_plateau(int** plateau);

void afficher_plateau (int** plateau);

void reinit_plateau (int** plateau);

void afficher_plateau_user(int** plateau);

int** copier_plateau(int** plateau);

int valeur_piece (int piece);

void initialiser_plateau (int** plateau);

int* creer_etat_plateau ();

void afficher_etat_plateau (int* etat);

int* copier_etat_plateau(int* etat_plateau);

int somme_plateau (int** plateau);

int somme_plateau_correspondance (int** plateau);

bool est_vide (int** plateau);

void deplacement_PionB (int** p, int** coups, int x, int y, int* etat_plateau);

void deplacement_PionN (int** p, int** coups, int x, int y, int* etat_plateau);

void deplacement_CavalierB(int** p, int** coups, int x, int y);

void deplacement_CavalierN(int** p, int** coups, int x, int y);

void deplacement_TourB(int** p, int** coups, int x, int y);

void deplacement_TourN(int** p, int** coups, int x, int y);

void deplacement_FouN(int** p, int** coups, int x, int y);

void deplacement_FouB(int** p, int** coups, int x, int y);

void deplacement_DameB(int** p, int** coups, int x, int y);

void deplacement_DameN(int** p, int** coups, int x, int y);

void deplacement_RoiB(int** p, int** coups, int x, int y, int* etat_plateau);

void deplacement_RoiN(int** p, int** coups, int x, int y, int* etat_plateau);

void deplacement_piece(int** plateau, int x, int y, int* etat_plateau, int** coups);



void defense_PionB (int** p, int** coups, int x, int y, int* etat_plateau);

void defense_PionN (int** p, int** coups, int x, int y, int* etat_plateau);

void defense_CavalierB(int** p, int** coups, int x, int y);

void defense_CavalierN(int** p, int** coups, int x, int y);

void defense_TourB(int** p, int** coups, int x, int y);

void defense_TourN(int** p, int** coups, int x, int y);

void defense_FouN(int** p, int** coups, int x, int y);

void defense_FouB(int** p, int** coups, int x, int y);

void defense_DameB(int** p, int** coups, int x, int y);

void defense_DameN(int** p, int** coups, int x, int y);

void defense_RoiB(int** p, int** coups, int x, int y, int* etat_plateau);

void defense_RoiN(int** p, int** coups, int x, int y, int* etat_plateau);

void defense_piece(int** plateau, int x, int y, int* etat_plateau, int** coups);

int** positions_defense(int** plateau, int* etat_plateau, int camp);

void positions_defense_double(int** plateau,int* etat_plateau, int** blanc,int** noir);

int appliquer_coup (int** plateau, int x_init, int y_init, int x_fin, int y_fin, int* etat_plateau);

bool case_menacee(int** plateau, int x, int y, int camp);

void deplacement_legal_piece (int** plateau, int x, int y, int* etat_plateau, int** coups);

int mat_ou_pat (int** plateau, int* etat_plateau);

ListeCoups* init_liste_coups ();

void ajouter_coup (ListeCoups* l, int x_init, int y_init, int x_fin, int y_fin, int valeur);

void afficher_liste_coups (ListeCoups* l);

void afficher_liste_coups2 (ListeCoups* l);

void liberer_liste_coups(ListeCoups* l);

ListeCoups* generer_coups_possibles (int** plateau, int* etat_plateau);

char* case_lettre(int j);

void  afficher_coup_user (int i_init, int j_init, int i_fin, int j_fin, int** plateau, int* etat_plateau);

void afficher_partie_user (ListeCoups* l);

#endif