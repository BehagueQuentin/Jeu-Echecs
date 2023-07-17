//########################################################################################################################################################// 
//######################################################//     Implémentation des déplacements    //######################################################//
//########################################################################################################################################################//


/* Dans toute la suite, on créera un tableau contenant la totalité des informations sur l'état de l'échiquier,
telle que la prise en passant, la possibilité de roquer, le tour, ... :

info = {tour, trait, en_passant, petit_roque_blanc, grand_roque_blanc, petit_roque_noir, grand_roque_noir}

Cette variable globale permettra de ne pas rallonger la taille des en-têtes de fonction.
L'échiquier quand a lui sera représenté par un tableau 8x8 contenant un entier, 
positif ou négatif selon la couleur, qui renseignera la pièce qui se trouve dans cette case.

Pour les fonctions de déplacement, on utilise en tableau passé en argument, généralement dont les cases sont toutes nulles,
qu'on modifie en ajoutant un lorsque la case est accessible.

Have fun ! */

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Importation du fichier .h :

#include "deplacement.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Fonctions simples de gestion du plateau :

int** creer_plateau(){
    int** rep = (int**)malloc(8*sizeof(int*));
    for(int i = 0; i < 8; i++){
        rep[i] = (int*)malloc(8*sizeof(int));
        for(int j = 0; j < 8; j++){
            rep[i][j] = 0;
        }
    }
    return rep;
}

void liberer_plateau(int** plateau){
    for(int i = 0; i < 8; i++){
        free(plateau[i]);
    }
    free(plateau);
}

void afficher_plateau (int** plateau){
    printf("\n-------------/ Plateau /-------------\n");
    for(int i = 7; i >= 0; i--){
        for(int j = 0; j < 8; j++){
            printf(" %d ", plateau[i][j]);
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
}

void reinit_plateau (int** plateau){
    for (int i= 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            plateau[i][j] = 0;
        }
    }
}

void afficher_plateau_user(int** plateau){
    printf("\n-------------/ Plateau /-------------\n\n");
    int type;
    for(int i = 7; i >= 0; i--){
        printf("     |");
        for(int j = 0; j < 8; j++){
            type = plateau[i][j];
            switch(type){
                case CaseVide :
                    printf("  ");
                    break;
                case PionB :
                    printf("Pb");
                    break;
                case PionN :
                    printf("Pn");
                    break;
                case CavalierB:
                    printf("Cb");
                    break;
                case CavalierN:
                    printf("Cn");
                    break;
                case FouB:
                    printf("Fb");
                    break;
                case FouN:
                    printf("Fn");
                    break;
                case TourB:
                    printf("Tb");
                    break;
                case TourN:
                    printf("Tn");
                    break;
                case DameB:
                    printf("Db");
                    break;
                case DameN:
                    printf("Dn");
                    break;
                case RoiB:
                    printf("Rb");
                    break;
                case RoiN:
                    printf("Rn");
                    break;
                default:
                    printf("ERREUR : 'afficher_plateau_user' est appelée avec une valeur ne correspondant pas à une pièce.\n");
                    break;
            }
            printf("|");
        }
        printf("\n     -------------------------\n");
    }
    printf("-------------------------------------\n\n");
}

int** copier_plateau(int** plateau){
    int** rep = (int**)malloc(8*sizeof(int*));
    for(int i = 0; i < 8; i++){
        rep[i] = (int*)malloc(8*sizeof(int));
        for(int j = 0; j < 8; j++){
            rep[i][j] = plateau[i][j];
        }
    }
    return rep; 
}

int valeur_piece (int piece){
    switch (piece){
        case CaseVide :
            printf("ERREUR : 'valeur_piece' ne peux pas etre appelee avec une case vide.\n");
        case PionB :
            return 1;
            break;
        case PionN :
            return -1;
            break;
        case CavalierB:
            return 3;
            break;
        case CavalierN:
            return -3;
            break;
        case FouB:
            return 3;
            break;
        case FouN:
            return -3;
            break;
        case TourB:
            return 6;
            break;
        case TourN:
            return -6;
            break;
        case DameB:
            return 9;
            break;
        case DameN:
            return -9;
            break;
        case RoiB:
            return 4000;
            break;
        case RoiN:
            return -4000;
            break;
        default:
            printf("ERREUR : 'valeur_piece' est appelée avec une valeur ne correspondant pas à une pièce.\n");
            break;
    }
}

void initialiser_plateau (int** plateau){
    for(int j = 0; j < 8; j++){
        plateau[1][j] = PionB;
        plateau[6][j] = PionN;
    }
    plateau[0][0] = TourB;
    plateau[0][7] = TourB;
    plateau[0][1] = CavalierB;
    plateau[0][6] = CavalierB;
    plateau[0][2] = FouB;
    plateau[0][5] = FouB;
    plateau[0][3] = DameB;
    plateau[0][4] = RoiB;

    plateau[7][0] = TourN;
    plateau[7][7] = TourN;
    plateau[7][1] = CavalierN;
    plateau[7][6] = CavalierN;
    plateau[7][2] = FouN;
    plateau[7][5] = FouN;
    plateau[7][3] = DameN;
    plateau[7][4] = RoiN;

}

int* creer_etat_plateau (){
    int* rep = (int*)malloc(7*sizeof(int));
    rep[0] = 1;  // Tour,
    rep[1] = 1;  // Trait (ici trait au blanc), 
    rep[2] = -1; // Si elle existe, colonne où une double poussée de pion s'est produite au tour précédent,
    rep[3] = 1;  // Petit roque blanc possible,
    rep[4] = 1;  // Petit roque blanc possible,
    rep[5] = 1;  // Grand roque noir possible,
    rep[6] = 1;  // Grand roque noir possible.
    return rep;
}

void afficher_etat_plateau (int* etat){
    printf("Etat = {Tour n°%d, Trait : %d, En passant : %d, Petit roque blanc : %d, ", etat[0], etat[1], etat[2], etat[3]);
    printf("Grand roque blanc : %d, Petit roque noir : %d, Grand roque noir : %d}\n", etat[4], etat[5], etat[6]);
}

int* copier_etat_plateau(int* etat_plateau){
    int* rep = (int*)malloc(7*sizeof(int));
    for(int i = 0; i < 7; i++){
        rep[i] = etat_plateau[i];
    }
    return rep;
}

int somme_plateau (int** plateau){
    int somme = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            somme += plateau[i][j];
        }
    }
    return somme;
}

int somme_plateau_correspondance (int** plateau){
    int somme = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(plateau[i][j] != 0){
                somme += valeur_piece(plateau[i][j]);

            }
        }
    }
    return somme;
}

bool est_vide (int** plateau){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if (plateau[i][j] != 0){
                return false;
            }
        }
    }
    return true;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Déplacements autorisés :

void deplacement_PionB (int** p, int** coups, int x, int y, int* etat_plateau){
    int en_passant = etat_plateau[2];
    // Gestion de la prise en passant :
    if (en_passant > -1 && x == 4 && (y == en_passant + 1  || y == en_passant - 1)){
        coups[x + 1][en_passant] ++;
    }
    // Gestion de la double poussée :
    if (x == 1 && p[x + 1][y] == 0 && p[x + 2][y] == 0){
        coups[x + 2][y] ++;
    }
    // Gestion de la simple poussée :
    if (x < 7 && p[x + 1][y] == 0) {
        coups[x + 1][y] ++;
    }
    // Gestion de la prise gauche :
    if (x < 7 && y > 0 && p[x + 1][y - 1] < 0) {
        coups[x + 1][y - 1] ++;
    }
    // Gestion de la prise droite :
    if (x < 7 && y < 7 && p[x + 1][y + 1] < 0) {
        coups[x + 1][y + 1] ++;
    }
}

void deplacement_PionN (int** p, int** coups, int x, int y, int* etat_plateau){
    int en_passant = etat_plateau[2];
    // Gestion de la prise en passant :
    if (en_passant > -1 && x == 3 && (y == en_passant + 1  || y == en_passant - 1)){
        coups[x - 1][en_passant] ++;
    }
    // Gestion de la double poussée :
    if (x == 6 && p[x - 1][y] == 0 && p[x - 2][y] == 0){
        coups[x - 2][y] ++;
    }
    // Gestion de la simple poussée :
    if (x > 0 && p[x - 1][y] == 0) {
        coups[x - 1][y] ++;
    }
    // Gestion de la prise gauche :
    if (x > 0 && y > 0 && p[x - 1][y - 1] > 0) {
        coups[x - 1][y - 1] ++;
    }
    // Gestion de la prise droite :
    if (x > 0 && y < 7 && p[x - 1][y + 1] > 0) {
        coups[x - 1][y + 1] ++;
    }
}

void deplacement_CavalierB(int** p, int** coups, int x, int y){
    int deplacements[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            if (p[dep_x][dep_y] <= 0){
                coups[dep_x][dep_y] ++;
            }
        }
    }
}

void deplacement_CavalierN(int** p, int** coups, int x, int y){
    int deplacements[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            if (p[dep_x][dep_y] >= 0){
                coups[dep_x][dep_y] ++;
            }
        }
    }
}

void deplacement_TourB(int** p, int** coups, int x, int y){
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } 
        else if (p[dep_x][y] < 0) {
            coups[dep_x][y] ++;
            break;
        } 
        else if (p[dep_x][y] > 0){
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } 
        else if (p[dep_x][y] < 0) {
            coups[dep_x][y] ++;
            break;
        } 
        else if (p[dep_x][y] > 0){
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else if (p[x][dep_y] < 0) {
            coups[x][dep_y] ++;
            break;
        } 
        else if (p[x][dep_y] > 0){
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else if (p[x][dep_y] < 0) {
            coups[x][dep_y] ++;
            break;
        }
        else if (p[x][dep_y] > 0){
            break;
        }
        dep_y++;
    }
}

void deplacement_TourN(int** p, int** coups, int x, int y){
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else if (p[dep_x][y] > 0) {
            coups[dep_x][y] ++;
            break;
        } 
        else if (p[dep_x][y] < 0){
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else if (p[dep_x][y] > 0) {
            coups[dep_x][y] ++;
            break;
        } 
        else if (p[dep_x][y] < 0){
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else if (p[x][dep_y] > 0) {
            coups[x][dep_y] ++;
            break;
        } 
        else if (p[x][dep_y] < 0){
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }  
        else if (p[x][dep_y] > 0) {
            coups[x][dep_y] ++;
            break;
        } 
        else if (p[x][dep_y] < 0){
            break;
        }
        dep_y++;
    }
}

void deplacement_FouN(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    int dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void deplacement_FouB(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    int dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void deplacement_DameB(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } else if (p[dep_x][y] < 0) {
            coups[dep_x][y] ++;
            break;
        } else if (p[dep_x][y] > 0) {
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } else if (p[dep_x][y] < 0) {
            coups[dep_x][y] ++;
            break;
        } else if (p[dep_x][y] > 0) {
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else if (p[x][dep_y] < 0) {
            coups[x][dep_y] ++;
            break;
        } else if (p[x][dep_y] > 0) {
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else if (p[x][dep_y] < 0) {
            coups[x][dep_y] ++;
            break;
        } else if (p[x][dep_y] > 0) {
            break;
        }
        dep_y++;
    }
    dep_x = x - 1;
    dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] < 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] > 0) {
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void deplacement_DameN(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } else if (p[dep_x][y] > 0) {
            coups[dep_x][y] ++;
            break;
        } else if (p[dep_x][y] < 0) {
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } else if (p[dep_x][y] > 0) {
            coups[dep_x][y] ++;
            break;
        } else if (p[dep_x][y] < 0) {
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else if (p[x][dep_y] > 0) {
            coups[x][dep_y] ++;
            break;
        } else if (p[x][dep_y] < 0) {
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else if (p[x][dep_y] > 0) {
            coups[x][dep_y] ++;
            break;
        } else if (p[x][dep_y] < 0) {
            break;
        }
        dep_y++;
    }
    dep_x = x - 1;
    dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else if (p[dep_x][dep_y] > 0) {
            coups[dep_x][dep_y] ++;
            break;
        } else if (p[dep_x][dep_y] < 0) {
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void deplacement_RoiB(int** p, int** coups, int x, int y, int* etat_plateau){
    int deplacements[8][2] = {{-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            if (p[dep_x][dep_y] <= 0){
                coups[dep_x][dep_y] ++;
            }
        }
    }
    // Gestion du petit roque :
    if (etat_plateau[3] == 1 && p[0][5] == 0 && p[0][6] == 0 && !case_menacee(p, 0, 5, TraitNoir)&& !case_menacee(p, 0, 4, TraitNoir)){
        coups[0][6] ++;
    }
    // Gestion du grand roque :
    if (etat_plateau[4] == 1 && p[0][1] == 0 && p[0][2] == 0 && p[0][3] == 0 && !case_menacee(p, 0, 4, TraitNoir)&& !case_menacee(p, 0, 3, TraitNoir)){
        coups[0][2] ++;
    }
}

void deplacement_RoiN(int** p, int** coups, int x, int y, int* etat_plateau){
    int deplacements[8][2] = {{-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            if (p[dep_x][dep_y] >= 0){
                coups[dep_x][dep_y] = 1;
            }
        }
    }
    if (etat_plateau[5] == 1 && p[7][5] == 0 && p[7][6] == 0  && !case_menacee(p, 7, 5, TraitBlanc) && !case_menacee(p, 7, 4, TraitBlanc)){
        coups[7][6] = 1;
    }
    if (etat_plateau[6] == 1 &&  p[7][1] == 0 && p[7][2] == 0 && p[7][3] == 0 && !case_menacee(p, 7, 4, TraitBlanc)&& !case_menacee(p, 7, 3, TraitBlanc)){
        coups[7][2] = 1;
    }
}

void deplacement_piece(int** plateau, int x, int y, int* etat_plateau, int** coups){
    reinit_plateau(coups);
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    int piece = plateau[x][y];
    switch(piece){
        case CaseVide :
            printf("Erreur, 'deplacement_piece' ne peux pas etre appelee avec une case vide");
        case PionB :
            deplacement_PionB(plateau, coups, x, y, etat_plateau);
            break;
        case PionN :
            deplacement_PionN(plateau, coups, x, y, etat_plateau);
            break;
        case CavalierB:
            deplacement_CavalierB(plateau, coups, x, y);
            break;
        case CavalierN:
            deplacement_CavalierN(plateau, coups, x, y);
            break;
        case FouB:
            deplacement_FouB(plateau, coups, x, y);
            break;
        case FouN:
            deplacement_FouN(plateau, coups, x, y);
            break;
        case TourB:
            deplacement_TourB(plateau, coups, x, y);
            break;
        case TourN:
            deplacement_TourN(plateau, coups, x, y);
            break;
        case DameB:
            deplacement_DameB(plateau, coups, x, y);
            break;
        case DameN:
            deplacement_DameN(plateau, coups, x, y);
            break;
        case RoiB:
            deplacement_RoiB(plateau, coups, x, y, etat_plateau);
            break;
        case RoiN:
            deplacement_RoiN(plateau, coups, x, y, etat_plateau);
            break;
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Déplacements autorisés :

void defense_PionB (int** p, int** coups, int x, int y, int* etat_plateau){
    int en_passant = etat_plateau[2];
    // Gestion de la prise en passant :
    if (en_passant > -1 && x == 4 && (y == en_passant + 1  || y == en_passant - 1)){
        coups[x + 1][en_passant] ++;
    }
    // Gestion de la prise gauche :
    if (x < 7 && y > 0 ) {
        coups[x + 1][y - 1] ++;
    }
    // Gestion de la prise droite :
    if (x < 7 && y < 7) {
        coups[x + 1][y + 1] ++;
    }
}

void defense_PionN (int** p, int** coups, int x, int y, int* etat_plateau){
    int en_passant = etat_plateau[2];
    // Gestion de la prise en passant :
    if (en_passant > -1 && x == 3 && (y == en_passant + 1  || y == en_passant - 1)){
        coups[x - 1][en_passant] ++;
    }
    // Gestion de la prise gauche :
    if (x > 0 && y > 0) {
        coups[x - 1][y - 1] ++;
    }
    // Gestion de la prise droite :
    if (x > 0 && y < 7) {
        coups[x - 1][y + 1] ++;
    }
}

void defense_CavalierB(int** p, int** coups, int x, int y){
    int deplacements[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            coups[dep_x][dep_y] ++;
        }
    }
}

void defense_CavalierN(int** p, int** coups, int x, int y){
    int deplacements[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            coups[dep_x][dep_y] ++;
        }
    }
}

void defense_TourB(int** p, int** coups, int x, int y){
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } 
        else{
            coups[dep_x][y] ++;
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } 
        else{
            coups[dep_x][y] ++;
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else {
            coups[x][dep_y] ++;
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else {
            coups[x][dep_y] ++;
            break;
        }
        dep_y++;
    }
}

void defense_TourN(int** p, int** coups, int x, int y){
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else{
            coups[dep_x][y] ++;
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else {
            coups[dep_x][y] ++;
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else{
            coups[x][dep_y] ++;
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }  
        else {
            coups[x][dep_y] ++;
            break;
        } 
        dep_y++;
    }
}

void defense_FouN(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    int dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else{
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else{
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        } 
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else{
            coups[dep_x][dep_y] ++;
            break;
        } 
        dep_x++;
        dep_y++;
    }
}

void defense_FouB(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    int dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        } 
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else  {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else{
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void defense_DameB(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } else {
            coups[dep_x][y] ++;
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else{
            coups[dep_x][y] ++;
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else {
            coups[x][dep_y] ++;
            break;
        } 
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } else {
            coups[x][dep_y] ++;
            break;
        }
        dep_y++;
    }
    dep_x = x - 1;
    dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else {
            coups[dep_x][dep_y] ++;
            break;
        } 
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else
        {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else{
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } else{
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void defense_DameN(int** p, int** coups, int x, int y) {
    int dep_x = x - 1;
    while (dep_x >= 0) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        }
        else{
            coups[dep_x][y] ++;
            break;
        }
        dep_x--;
    }
    dep_x = x + 1;
    while (dep_x < 8) {
        if (p[dep_x][y] == 0) {
            coups[dep_x][y] ++;
        } 
        else {
            coups[dep_x][y] ++;
            break;
        }
        dep_x++;
    }
    int dep_y = y - 1;
    while (dep_y >= 0) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        } 
        else{
            coups[x][dep_y] ++;
            break;
        }
        dep_y--;
    }
    dep_y = y + 1;
    while (dep_y < 8) {
        if (p[x][dep_y] == 0) {
            coups[x][dep_y] ++;
        }
        else {
            coups[x][dep_y] ++;
            break;
        }
        dep_y++;
    }
    dep_x = x - 1;
    dep_y = y - 1;
    while (dep_x >= 0 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y--;
    }
    dep_x = x - 1;
    dep_y = y + 1;
    while (dep_x >= 0 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x--;
        dep_y++;
    }
    dep_x = x + 1;
    dep_y = y - 1;
    while (dep_x < 8 && dep_y >= 0) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        } 
        else {
            coups[dep_x][dep_y] ++;
            break;
        } 
        dep_x++;
        dep_y--;
    }
    dep_x = x + 1;
    dep_y = y + 1;
    while (dep_x < 8 && dep_y < 8) {
        if (p[dep_x][dep_y] == 0) {
            coups[dep_x][dep_y] ++;
        }
        else  {
            coups[dep_x][dep_y] ++;
            break;
        }
        dep_x++;
        dep_y++;
    }
}

void defense_RoiB(int** p, int** coups, int x, int y, int* etat_plateau){
    int deplacements[8][2] = {{-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            coups[dep_x][dep_y] ++;
        }
    }
}

void defense_RoiN(int** p, int** coups, int x, int y, int* etat_plateau){
    int deplacements[8][2] = {{-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
    for (int i = 0; i < 8; i++) {
        int dep_x = x + deplacements[i][0];
        int dep_y = y + deplacements[i][1];
        if (dep_x >= 0 && dep_x < 8 && dep_y >= 0 && dep_y < 8) {
            coups[dep_x][dep_y] = 1;
        }
    }
}

void defense_piece(int** plateau, int x, int y, int* etat_plateau, int** coups){
    int piece = plateau[x][y];
    switch(piece){
        case CaseVide :
            break;
        case PionB :
            defense_PionB(plateau, coups, x, y, etat_plateau);
            break;
        case PionN :
            defense_PionN(plateau, coups, x, y, etat_plateau);
            break;
        case CavalierB:
            defense_CavalierB(plateau, coups, x, y);
            break;
        case CavalierN:
            defense_CavalierN(plateau, coups, x, y);
            break;
        case FouB:
            defense_FouB(plateau, coups, x, y);
            break;
        case FouN:
            defense_FouN(plateau, coups, x, y);
            break;
        case TourB:
            defense_TourB(plateau, coups, x, y);
            break;
        case TourN:
            defense_TourN(plateau, coups, x, y);
            break;
        case DameB:
            defense_DameB(plateau, coups, x, y);
            break;
        case DameN:
            defense_DameN(plateau, coups, x, y);
            break;
        case RoiB:
            defense_RoiB(plateau, coups, x, y, etat_plateau);
            break;
        case RoiN:
            defense_RoiN(plateau, coups, x, y, etat_plateau);
            break;
    }
}

int** positions_defense(int** plateau, int* etat_plateau, int camp){
    int** rep = creer_plateau();
    for(int i = 0; i<8; i++){
        for(int j = 0 ; j < 8; j++){
            if(plateau[i][j]*camp > 0){
                printf("1");
                defense_piece(plateau, i, j, etat_plateau, rep);
            }
        }
    }
    return rep;
}

void positions_defense_double(int** plateau,int* etat_plateau, int** blanc, int** noir){
    for(int i = 0; i<8; i++){
        for(int j = 0 ; j < 8; j++){
            if(plateau[i][j]*TraitBlanc > 0){
                defense_piece(plateau, i, j, etat_plateau, blanc);
            }
            else if(plateau[i][j]*TraitNoir > 0){
                defense_piece(plateau, i, j, etat_plateau, noir);
            }
        }
    }
}





//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Application des déplacements et gestion des échecs :

int appliquer_coup (int** plateau, int x_init, int y_init, int x_fin, int y_fin, int* etat_plateau){
    int promotion = -1;
    int type = plateau[x_init][y_init];
    switch (type){
        case PionB :
            if(y_init != y_fin && plateau[x_fin][y_fin] == 0){
                plateau[x_fin - 1][y_fin] = CaseVide;
                etat_plateau[2] = -1;
            }
            else if (x_fin == 7){
                promotion = y_fin;
                etat_plateau[2] = -1;
            }
            else if (x_fin == x_init + 2){
                etat_plateau[2] = y_fin;
            }
            break;
        case PionN :
            if(y_init != y_fin && plateau[x_fin][y_fin] == 0){
                plateau[x_fin + 1][y_fin] = CaseVide;
                etat_plateau[2] = -1;
            }
            else if (x_fin == 0){
                promotion = y_fin;
                etat_plateau[2] = -1;
            }
            else if (x_fin == x_init - 2){
                etat_plateau[2] = y_fin;
            }
            break;
        case RoiB :
            if (y_fin == y_init + 2){
                plateau[0][7] = CaseVide;
                plateau[0][5] = TourB;
                etat_plateau[3] = 0;
            }
            else if (y_fin == y_init - 2){
                plateau[0][0] = CaseVide;
                plateau[0][3] = TourB;
                etat_plateau[4] = 0;
            }
            etat_plateau[3] = 0;
            etat_plateau[4] = 0;
            etat_plateau[2] = -1;
            break;
        case RoiN :
            if (y_fin == y_init + 2){
                plateau[7][7] = CaseVide;
                plateau[7][5] = TourN;
                etat_plateau[5] = 0;
            }
            else if (y_fin == y_init - 2){
                plateau[7][0] = CaseVide;
                plateau[7][3] = TourN;
                etat_plateau[6] = 0;
            }
            etat_plateau[5] = 0;
            etat_plateau[6] = 0;
            etat_plateau[2] = -1;
            break;
        case TourB :
            if(x_init == 0  && y_init == 0){
                etat_plateau[4] = 0;
            }
            else if(x_init == 0  && y_init == 7){
                etat_plateau[3] = 0;
            }
        case TourN :
            if(x_init == 7  && y_init == 0){
                etat_plateau[6] = 0;
            }
            else if(x_init == 7  && y_init == 7){
                etat_plateau[5] = 0;
            }
        default:
            etat_plateau[2] = -1;
            break;
    }

    // Adaptations valables pour tous les coups :

    plateau[x_init][y_init] = 0;
    plateau[x_fin][y_fin] = type;

    // Gestion du mouvement ou de la prise des tours (roque) :
    if(x_fin == 0){
        if(y_fin == 0){
            etat_plateau[4] = 0;
        }
        else if(y_fin == 7){
            etat_plateau[3] = 0;
        }
    }
    else if (x_fin == 7){
        if(y_fin == 0){
            etat_plateau[6] = 0;
        }
        else if(y_fin == 7){
            etat_plateau[5] = 0;
        }
    }
    /*if((x_init == 0  && y_init == 0) || (x_fin == 0 && y_fin == 0)){
        etat_plateau[4] = 0;
    }
    if((x_init == 0  && y_init == 7) || (x_fin == 0 && y_fin == 7)){
        etat_plateau[3] = 0;
    }
    if((x_init == 7  && y_init == 0) || (x_fin == 7 && y_fin == 0)){
        etat_plateau[6] = 0;
    }
    if((x_init == 7  && y_init == 7) || (x_fin == 7 && y_fin == 7)){
        etat_plateau[5] = 0;
    }*/

    // Incrémentation du nombre de tour :
    etat_plateau[0] ++;
    etat_plateau[1] = - etat_plateau[1];
    return promotion;
}

bool case_menacee(int** plateau, int x, int y, int camp){
    /* Peu importe les valeurs prise par en_passant, ou les variables de roque,
    on ne peux pas capturer un roi en roquant ni en effetuant une prise en passant.
    On n'a donc besoin d'aucune des variables contenues dans 'etat_plateau',
    cependant, on doit tout de même le passer en argument, 
    on fait donc le choix arbitraire de mettre toutes les valeur de roque à nulle,
    et 'en_passant' à -1. */
    if(plateau[x][y]*camp > 0){
        printf("ERREUR : Une piece ne peut pas etre menacee par une piece de meme couleur ! (case_menacee)\n");
        return false;
    }

    int* etat_plateau = creer_etat_plateau();
    etat_plateau[2] = -1;
    etat_plateau[3] = 0;
    etat_plateau[4] = 0;
    etat_plateau[5] = 0;
    etat_plateau[6] = 0;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(plateau[i][j]*camp > 0){
                int** menaces = creer_plateau();
                deplacement_piece(plateau, i, j, etat_plateau, menaces);
                if (menaces[x][y] > 0){
                    liberer_plateau(menaces);
                    return true;
                }
                liberer_plateau(menaces);
            }
        }
    }
    free(etat_plateau);
    return false;
}

void deplacement_legal_piece (int** plateau, int x, int y, int* etat_plateau, int** coups){
    int type = plateau[x][y];
    int camp = 1;
    if(type > 0){ 
        camp = 1;
    }
    else if(type < 0){
        camp = -1;
    }
    else{
        printf("ERREUR : 'deplacement_legal_piece' est appelée avec une case vide ou une pièce inexistante !\n");
    }
    reinit_plateau(coups);
    deplacement_piece(plateau, x, y, etat_plateau, coups);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if (coups[i][j] != 0){
                int** plateau_post_mouv = copier_plateau(plateau);
                int* etat_plateau_post_mouv = copier_etat_plateau(etat_plateau);
                int promotion = appliquer_coup(plateau_post_mouv, x, y, i, j, etat_plateau_post_mouv);
                int x_roi = -1;
                int y_roi = -1;
                for(int i = 0; i < 8; i++){
                    for(int j = 0; j < 8; j++){
                        if(plateau_post_mouv[i][j] == RoiB*camp){
                            x_roi = i;
                            y_roi = j;
                        }
                        
                    }
                }
                if(case_menacee(plateau_post_mouv, x_roi, y_roi, -camp)){
                    coups[i][j] = 0;
                }
                free(etat_plateau_post_mouv);
                liberer_plateau(plateau_post_mouv);
            }
        }
    }
}

int mat_ou_pat (int** plateau, int* etat_plateau){
    /* Cette fonction renvoie 0 si la partie n'est pas terminée, elle renvoie 1 ou -1 en cas de pat,
    Elle renvoie 2 si les blancs gagne par échec et mat, -2 si ce sont les noirs. */
    int camp = etat_plateau[1];
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            int type = plateau[i][j];
            if(type*camp > 0){
                int** mouvement = creer_plateau();
                deplacement_legal_piece(plateau, i, j, etat_plateau, mouvement);
                if (!est_vide(mouvement)){
                    liberer_plateau(mouvement);
                    return 0;
                }
                liberer_plateau(mouvement);
            }
        }
    }
    int roi_x = -1;
    int roi_y = -1;
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            if(plateau[i][j] == RoiB*camp){
                roi_x = i;
                roi_y = j;
            }
        }
    }
    if (case_menacee(plateau, roi_x, roi_y, -camp)){
        return camp*2;
    }
    else{
        return camp;
    }
}
 
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Gestion des tableaux dynamiques :

ListeCoups* init_liste_coups (){
    ListeCoups* rep = (ListeCoups*)malloc(sizeof(ListeCoups));
    rep->taille = NbCoupsPossibleMax;
    rep->occupe = 0;
    rep->coups = (int**)malloc(NbCoupsPossibleMax*sizeof(int*));
    for(int i = 0; i < NbCoupsPossibleMax; i++){
        rep->coups[i] = (int*)malloc(5*sizeof(int));
    }
    return rep;
}

void ajouter_coup (ListeCoups* l, int x_init, int y_init, int x_fin, int y_fin, int valeur){
    int i = 0;
    while (i < l->occupe && l->coups[i][4] > valeur) {
        i++;
    }
    for (int j = l->occupe - 1; j >= i; j--) {
        l->coups[j + 1][0] = l->coups[j][0];
        l->coups[j + 1][1] = l->coups[j][1];
        l->coups[j + 1][2] = l->coups[j][2];
        l->coups[j + 1][3] = l->coups[j][3];
        l->coups[j + 1][4] = l->coups[j][4];

    }
    l->coups[i][0] = x_init;
    l->coups[i][1] = y_init;
    l->coups[i][2] = x_fin;
    l->coups[i][3] = y_fin;
    l->coups[i][4] = valeur;
    l->occupe ++;
}

void afficher_liste_coups (ListeCoups* l){
    printf("::::::::::::::::: Liste des coups :::::::::::::::::\n");
    printf("taille = %d\n", l->taille);
    printf("occupe = %d\n", l->occupe);
    for(int i = 0; i < l->occupe; i++){
        printf("[ (%d, %d) -> (%d, %d) | v = %d ]\n", l->coups[i][0], l->coups[i][1], l->coups[i][2], l->coups[i][3], l->coups[i][4]);

    }
    printf(":::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");
}

void afficher_liste_coups2 (ListeCoups* l){
    printf("::::::::::::::::: Liste des coups :::::::::::::::::\n");
    printf("taille = %d\n", l->taille);
    printf("occupe = %d\n{", l->occupe);
    for(int i = 0; i < l->occupe -1; i++){
        printf("{%d, %d, %d, %d}, ", l->coups[i][0], l->coups[i][1], l->coups[i][2], l->coups[i][3]);
    }
    printf("{%d, %d, %d, %d}", l->coups[l->occupe -1][0], l->coups[l->occupe -1][1], l->coups[l->occupe -1][2], l->coups[l->occupe -1][3]);
    printf("}\n :::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");
}

void liberer_liste_coups(ListeCoups* l){
    for(int i = 0; i < l->taille; i++){
        free(l->coups[i]);
    }
    free(l->coups);
    free(l);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Calcul des coups légaux :

/* int heuristique_elagage0(int** plateau, int* etat_plateau, int i_init, int j_init, int i_fin, int j_fin){
    int h = 0;
    int** attaque_adverse = creer_plateau();
    int** attaque_perso = creer_plateau();
    if(etat_plateau[1] == TraitBlanc){
        positions_defense_double(plateau, etat_plateau, attaque_perso, attaque_adverse);
    }
    else{
        positions_defense_double(plateau, etat_plateau, attaque_adverse, attaque_perso);
    }
    //int** attaque_adverse = positions_defense(plateau, etat_plateau, -etat_plateau[1]);
    //int** attaque_perso = positions_defense(plateau, etat_plateau, etat_plateau[1]);

    if(plateau[i_fin][j_fin] != 0){ // Si la case finale contient une pièce adverse :
        
        // Si la pièce ennemie est de valeur supérieure à la pièce attaquante :
        if(valeur_piece(plateau[i_fin][j_fin]) > valeur_piece(plateau[i_init][j_init])){
            //printf("| %d > %d | piece initiale : %d | piece finale : %d \n", points[abs(p[i_fin][j_fin])], points[abs(p[i_init][i_fin])], p[i_fin][j_fin], p[i_init][i_init]);
            h += 500;
        }

        // Si la pièce ennemie n'est pas défendue :
        if (attaque_adverse[i_fin][j_fin] == 0){
            h += 303;
        }
        // Si la pièce ennemie est de valeur inférieure à la pièce attaquante :
        if(valeur_piece(plateau[i_fin][j_fin]) < valeur_piece(plateau[i_init][j_init])){
            h -= 101;
        }
        // Si la pièce ennemie a moins de défenseurs qu'il n'y a d'attaquant :
        if (attaque_perso[i_fin][j_fin] > attaque_adverse[i_fin][j_fin]){
            h += 50;
        }
    }
    if(attaque_perso[i_init][j_init] < attaque_adverse[i_init][j_init] && plateau[i_init][j_init] != 1 && plateau[i_init][j_init] != -1){
        if (attaque_adverse[i_fin][j_fin] == 0){
            h += 200;
        }
        if (attaque_perso[i_fin][j_fin] > attaque_adverse[i_fin][j_fin]){
            h += 100;
        }
    }


    //Dans tout les cas, si une case à plus d'attaquant que de défenseur, on évitera de se placer sur une case trop attaquée.
    if (attaque_perso[i_fin][j_fin] <= attaque_adverse[i_fin][j_fin]){
        h -= 57;
    }
    
    //h += preference(p[i_init][j_init], i_fin, j_fin);
    liberer_plateau(attaque_adverse);
    liberer_plateau(attaque_perso);
    return h;
}*/

int heuristique_elagage(int** plateau, int* etat_plateau, int i_init, int j_init, int i_fin, int j_fin){
    int** copie_plateau = copier_plateau(plateau);
    int* copie_etat = creer_etat_plateau();
    appliquer_coup(copie_plateau, i_init, j_init, i_fin, j_fin, copie_etat);
    int** blanc = creer_plateau();
    int** noir = creer_plateau();    
    positions_defense_double(plateau, etat_plateau, blanc, noir);
    int h = somme_plateau(blanc) - somme_plateau(noir);
    liberer_plateau(blanc);
    liberer_plateau(noir);
    liberer_plateau(copie_plateau);
    free(copie_etat);
    return etat_plateau[1]*30*somme_plateau_correspondance(copie_plateau) + h;
}

ListeCoups* generer_coups_possibles (int** plateau, int* etat_plateau){
    ListeCoups* rep = init_liste_coups();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j ++){
            int type = plateau[i][j];
            if (type*etat_plateau[1] > 0){
                int** coups_piece = creer_plateau();
                deplacement_legal_piece(plateau, i, j, etat_plateau, coups_piece);
                for (int k = 0; k < 8; k ++){
                    for(int l = 0; l < 8; l ++){
                        if(coups_piece[k][l] == 1){
                            int h = heuristique_elagage(plateau, etat_plateau, i, j, k, l);
                            ajouter_coup(rep, i, j, k, l, h );
                        }
                    }
                }
                liberer_plateau(coups_piece);
            }
        }
    }
    return rep;
}

char* piece_lettre (int** plateau, int i, int j){
    int piece = plateau[i][j];
    switch(piece){
        case(PionB):
            return "";
        case(PionN):
            return "";
        case(FouB):
            return "B";
        case(FouN):
            return "B";
        case(TourB):
            return "R";
        case(TourN):
            return "R";
        case(CavalierB):
            return "N";
        case(CavalierN):
            return "N";
        case(RoiB):
            return "K";
        case(RoiN):
            return "K";
        case(DameB):
            return "Q";
        case(DameN):
            return "Q";
        default :
            printf ("ERREUR : Dans 'piece_lettre', l'appel par %d ne correspond pas à une piece existante", piece);
    }
}

char* case_lettre(int j){
    switch(j){
        case 0 :
            return "a";
        case 1 :
            return "b";
        case 2 :
            return "c";
        case 3 :
            return "d";
        case 4 :
            return "e";
        case 5 :
            return "f";
        case 6 :
            return "g";
        case 7 :
            return "h";
        default : 
            printf("ERREUR : Dans 'correspondance_case_lettre', %d ne correspond pas a une case de l'echiquier !", j);
    }
}

void afficher_coup_user (int i_init, int j_init, int i_fin, int j_fin, int** plateau, int* etat_plateau){
    if(plateau[i_fin][j_fin] == 0){
        printf("%s%s%d", piece_lettre(plateau, i_init, j_init), case_lettre(j_fin), (i_fin + 1));
    }
    else if (plateau[i_init][j_init] == PionN || plateau[i_init][j_init] == PionB){
        printf("%sx%s%d", case_lettre(j_init), case_lettre(j_fin), (i_fin + 1));
    }
    else{
        printf("%sx%s%d", piece_lettre(plateau, i_init, j_init), case_lettre(j_fin), (i_fin + 1));
    }
    appliquer_coup(plateau, i_init, j_init, i_fin, j_fin, etat_plateau);
}

void afficher_partie_user (ListeCoups* l){
    int** plateau = creer_plateau();
    initialiser_plateau(plateau);
    int* etat_plateau = creer_etat_plateau();
    for(int i = 0; i < l->occupe / 2; i++){
        printf(" %d - ", (i + 1));
        afficher_coup_user(l->coups[2*i][0], l->coups[2*i][1], l->coups[2*i][2], l->coups[2*i][3], plateau, etat_plateau);
        printf(" | ");
        afficher_coup_user(l->coups[2*i+1][0], l->coups[2*i+1][1], l->coups[2*i+1][2], l->coups[2*i+1][3], plateau, etat_plateau);
        printf("\n");
    }
    if(l->occupe%2 == 1){
        printf(" %d - ", (l->occupe/2 + 1));
        afficher_coup_user(l->coups[l->occupe-1][0], l->coups[l->occupe-1][1], l->coups[l->occupe-1][2], l->coups[l->occupe-1][3], plateau, etat_plateau);
        printf("\n");
    }
    liberer_plateau(plateau);
    free(etat_plateau);
}