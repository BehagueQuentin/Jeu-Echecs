//########################################################################################################################################################// 
//############################################################//     Arbre des ouvertures    //###########################################################//
//########################################################################################################################################################//


/* Le présent fichier implémente l'arbre d'ouverture à l'aide d'une banque d'ouvertures (de profondeur 12) sous la forme d'une suite de coups représentés eux-mêmes par
la case de départ et la case d'arrivée de leur mouvement. Il est donc possible d'augmenter le nombre d'ouvertures ainsi que leur profondeur.
(Il faudra alors veiller à changer la valeur des constantes dans le fichier .h correspondant.)


/!\ Aucune fonction n'est implémentée pour transformer la notation classique :
    1 - e4, e5  
    2 - Nf3, Nc6
    3 - ...
Dans la notation utilisée dans ce fichier, alors à moins de vouloir les jouées manuellement, il est de votre responsabilité de la coder ! */



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Importation du fichier .h :

#include "ouverture.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Variables Globales :

int ouvertures [NBouvertures][12][4] = {
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 6, 2, 6}, {6, 3, 4, 3}, {0, 5, 1, 6}, {6, 6, 5, 6}, {1, 3, 3, 3}, {7, 5, 6, 6}, {0, 4, 0, 6}, {7, 4, 7, 6}, {1, 2, 3, 2}, {4, 3, 3, 2}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 5, 3, 2}, {7, 6, 5, 5}, {1, 3, 2, 3}, {7, 1, 5, 2}, {0, 1, 2, 2}, {5, 2, 4, 0}, {0, 3, 2, 5}, {7, 5, 6, 4}, {0, 6, 1, 4}, {6, 3, 5, 3}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 3, 3, 3}, {7, 6, 5, 5}, {1, 7, 2, 7}, {6, 4, 5, 4}, {1, 2, 3, 2}, {6, 7, 5, 7}, {0, 3, 1, 2}, {6, 1, 5, 1}, {3, 2, 4, 3}, {5, 4, 4, 3}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 4, 5, 4}, {0, 1, 2, 2}, {6, 2, 4, 2}, {3, 2, 4, 3}, {5, 4, 4, 3}, {1, 3, 3, 3}, {7, 1, 5, 2}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {1, 2, 2, 2}, {7, 6, 5, 5}, {3, 4, 4, 4}, {5, 5, 4, 3}, {1, 3, 3, 3}, {4, 2, 3, 3}, {0, 3, 3, 3}, {6, 4, 5, 4}, {0, 6, 2, 5}, {7, 1, 5, 2}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 1, 2, 1}, {6, 6, 5, 6}, {0, 2, 1, 1}, {7, 5, 6, 6}, {1, 3, 3, 3}, {6, 3, 5, 3}, {1, 4, 2, 4}, {7, 4, 7, 6}, {0, 5, 1, 4}, {6, 4, 4, 4}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 5, 3, 2}, {7, 6, 5, 5}, {1, 3, 2, 3}, {7, 1, 5, 2}, {0, 1, 2, 2}, {7, 5, 3, 1}, {0, 6, 1, 4}, {7, 4, 7, 6}, {0, 4, 0, 6}, {5, 2, 4, 0}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 1, 2, 1}, {6, 6, 5, 6}, {1, 4, 2, 4}, {7, 5, 6, 6}, {0, 2, 1, 1}, {7, 4, 7, 6}, {1, 3, 3, 3}, {6, 3, 4, 3}, {1, 2, 3, 2}, {6, 2, 4, 2}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 1, 2, 1}, {6, 3, 4, 3}, {0, 2, 1, 1}, {6, 4, 5, 4}, {1, 2, 3, 2}, {7, 5, 6, 4}, {3, 2, 4, 3}, {5, 4, 4, 3}, {1, 4, 2, 4}, {7, 4, 7, 6}},
{{0, 6, 2, 5}, {6, 5, 4, 5}, {1, 3, 2, 3}, {7, 1, 5, 2}, {1, 4, 3, 4}, {6, 4, 4, 4}, {0, 1, 2, 2}, {7, 6, 5, 5}, {3, 4, 4, 5}, {6, 3, 4, 3}, {2, 5, 3, 7}, {7, 5, 3, 1}},
{{1, 4, 3, 4}, {6, 0, 5, 0}, {1, 3, 3, 3}, {6, 4, 5, 4}, {1, 0, 2, 0}, {6, 1, 5, 1}, {0, 5, 2, 3}, {6, 3, 5, 3}, {1, 5, 3, 5}, {6, 6, 5, 6}, {0, 6, 2, 5}, {7, 6, 6, 4}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {1, 3, 3, 3}, {4, 2, 3, 3}, {1, 2, 2, 2}, {3, 3, 2, 2}, {0, 1, 2, 2}, {7, 1, 5, 2}, {0, 6, 2, 5}, {6, 3, 5, 3}, {0, 5, 3, 2}, {6, 4, 5, 4}},
{{1, 3, 3, 3}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {0, 5, 2, 3}, {6, 2, 4, 2}, {0, 6, 2, 5}, {7, 1, 5, 2}, {1, 7, 2, 7}, {6, 4, 5, 4}, {1, 1, 2, 1}, {4, 2, 3, 3}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 2, 5, 2}, {1, 3, 3, 3}, {6, 4, 5, 4}, {0, 5, 2, 3}, {7, 1, 6, 3}, {0, 1, 1, 3}, {7, 5, 6, 4}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {6, 3, 4, 3}, {1, 3, 3, 3}, {7, 2, 4, 5}, {1, 2, 3, 2}, {6, 2, 5, 2}, {0, 1, 2, 2}, {6, 4, 5, 4}, {2, 5, 3, 7}, {4, 5, 3, 6}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {6, 2, 5, 2}, {1, 2, 3, 2}, {6, 4, 5, 4}, {1, 3, 3, 3}, {7, 1, 6, 3}, {0, 5, 2, 3}, {6, 6, 5, 6}, {2, 4, 3, 4}, {4, 3, 3, 4}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 1, 2, 1}, {6, 6, 5, 6}, {0, 2, 1, 1}, {7, 5, 6, 6}, {1, 3, 3, 3}, {7, 4, 7, 6}, {1, 4, 2, 4}, {6, 3, 4, 3}, {1, 2, 3, 2}, {6, 2, 5, 2}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {0, 6, 2, 5}, {6, 3, 5, 3}, {1, 2, 2, 2}, {7, 6, 5, 5}, {0, 5, 2, 3}, {6, 6, 5, 6}, {2, 3, 1, 2}, {7, 1, 5, 2}, {1, 3, 3, 3}, {4, 2, 3, 3}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {6, 6, 5, 6}, {1, 1, 2, 1}, {7, 5, 6, 6}, {0, 2, 1, 1}, {7, 4, 7, 6}, {1, 3, 3, 3}, {6, 2, 4, 2}, {3, 3, 4, 2}, {7, 3, 4, 0}},
{{1, 4, 3, 4}, {6, 2, 5, 2}, {0, 6, 2, 5}, {6, 3, 4, 3}, {1, 3, 2, 3}, {4, 3, 3, 4}, {2, 3, 3, 4}, {7, 3, 0, 3}, {0, 4, 0, 3}, {7, 6, 5, 5}, {0, 1, 1, 3}, {6, 6, 4, 6}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {6, 6, 5, 6}, {1, 1, 2, 1}, {7, 5, 6, 6}, {0, 2, 1, 1}, {7, 4, 7, 6}, {1, 3, 3, 3}, {6, 3, 5, 3}, {0, 5, 1, 4}, {7, 1, 6, 3}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {4, 3, 3, 2}, {0, 5, 3, 2}, {6, 4, 5, 4}, {1, 3, 3, 3}, {6, 0, 5, 0}, {0, 3, 1, 4}, {6, 1, 4, 1}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {6, 6, 5, 6}, {1, 1, 2, 1}, {7, 5, 6, 6}, {0, 2, 1, 1}, {6, 3, 5, 3}, {1, 3, 3, 3}, {7, 4, 7, 6}, {0, 5, 1, 4}, {7, 1, 6, 3}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 2, 5, 2}, {1, 3, 3, 3}, {7, 2, 3, 6}, {1, 7, 2, 7}, {3, 6, 2, 5}, {0, 3, 2, 5}, {6, 6, 5, 6}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 6, 2, 6}, {6, 4, 5, 4}, {0, 5, 1, 6}, {6, 2, 4, 2}, {1, 3, 2, 3}, {7, 6, 5, 5}, {0, 1, 1, 3}, {7, 1, 5, 2}, {1, 4, 3, 4}, {7, 5, 6, 4}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 6, 2, 6}, {6, 6, 5, 6}, {0, 5, 1, 6}, {7, 5, 6, 6}, {1, 3, 2, 3}, {6, 4, 4, 4}, {0, 4, 0, 6}, {7, 1, 5, 2}, {1, 4, 3, 4}, {4, 3, 3, 4}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 4, 5, 4}, {0, 1, 2, 2}, {6, 1, 5, 1}, {1, 3, 2, 3}, {7, 2, 6, 1}, {3, 2, 4, 3}, {5, 4, 4, 3}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 6, 5, 5}, {2, 5, 4, 4}, {6, 3, 5, 3}, {4, 4, 2, 3}, {5, 5, 3, 4}, {0, 3, 1, 4}, {7, 3, 6, 4}, {2, 3, 3, 5}, {7, 1, 5, 2}},
{{0, 6, 2, 5}, {6, 3, 4, 3}, {1, 4, 2, 4}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 4, 5, 4}, {1, 1, 2, 1}, {7, 5, 5, 3}, {0, 2, 1, 1}, {7, 4, 7, 6}, {0, 1, 2, 2}, {7, 5, 7, 4}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {1, 2, 2, 2}, {6, 3, 4, 3}, {3, 4, 4, 3}, {7, 3, 4, 3}, {0, 1, 2, 0}, {7, 1, 5, 2}, {0, 6, 2, 5}, {7, 2, 4, 5}, {1, 3, 3, 3}, {7, 6, 5, 5}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {1, 1, 2, 1}, {7, 6, 5, 5}, {3, 4, 4, 4}, {5, 5, 4, 3}, {0, 1, 2, 2}, {4, 3, 6, 2}, {0, 2, 1, 1}, {7, 1, 5, 2}, {2, 2, 3, 4}, {6, 3, 5, 3}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {0, 6, 2, 5}, {6, 3, 5, 3}, {1, 2, 2, 2}, {7, 6, 5, 5}, {0, 5, 2, 3}, {7, 1, 5, 2}, {2, 3, 1, 2}, {7, 2, 3, 6}, {1, 3, 2, 3}, {6, 4, 5, 4}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 6, 5, 5}, {2, 5, 4, 4}, {6, 3, 5, 3}, {4, 4, 2, 3}, {5, 5, 3, 4}, {0, 3, 1, 4}, {7, 3, 6, 4}, {2, 3, 3, 5}, {6, 2, 5, 2}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {1, 2, 2, 2}, {6, 3, 4, 3}, {3, 4, 4, 3}, {7, 3, 4, 3}, {0, 1, 2, 0}, {7, 6, 5, 5}, {1, 3, 3, 3}, {6, 4, 5, 4}, {0, 6, 2, 5}, {4, 2, 3, 3}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {1, 5, 3, 5}, {4, 4, 3, 5}, {0, 6, 2, 5}, {6, 7, 5, 7}, {1, 3, 3, 3}, {6, 6, 4, 6}, {1, 6, 2, 6}, {7, 1, 5, 2}, {1, 2, 2, 2}, {3, 5, 2, 6}},
{{1, 3, 3, 3}, {6, 3, 4, 3}, {0, 2, 3, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {6, 2, 4, 2}, {1, 2, 2, 2}, {7, 1, 5, 2}, {0, 1, 1, 3}, {6, 4, 5, 4}, {0, 6, 2, 5}, {5, 5, 4, 7}},
{{1, 1, 2, 1}, {6, 3, 4, 3}, {0, 6, 2, 5}, {7, 2, 4, 5}, {0, 2, 2, 0}, {7, 1, 6, 3}, {1, 2, 3, 2}, {6, 4, 5, 4}, {2, 0, 7, 5}, {6, 3, 7, 5}, {1, 3, 2, 3}, {7, 6, 5, 5}},
{{1, 6, 2, 6}, {6, 3, 4, 3}, {0, 6, 2, 5}, {7, 6, 5, 5}, {0, 5, 1, 6}, {6, 4, 5, 4}, {0, 4, 0, 6}, {7, 5, 5, 3}, {1, 2, 3, 2}, {7, 4, 7, 6}, {1, 1, 2, 1}, {7, 1, 5, 2}},
{{1, 5, 2, 5}, {6, 4, 4, 4}, {0, 1, 2, 2}, {6, 3, 4, 3}, {1, 3, 3, 3}, {4, 4, 3, 3}, {0, 3, 3, 3}, {7, 2, 5, 4}, {1, 4, 3, 4}, {7, 1, 5, 2}, {0, 5, 4, 1}, {4, 3, 3, 4}},
{{1, 0, 2, 0}, {6, 0, 5, 0}, {1, 3, 3, 3}, {6, 3, 4, 3}, {0, 2, 3, 5}, {7, 6, 5, 5}, {1, 4, 2, 4}, {7, 2, 3, 6}, {1, 5, 2, 5}, {3, 6, 4, 5}, {1, 6, 3, 6}, {4, 5, 5, 6}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {0, 6, 2, 5}, {6, 4, 5, 4}, {0, 5, 2, 3}, {7, 1, 5, 2}, {0, 4, 0, 6}, {6, 6, 4, 6}, {2, 3, 4, 1}, {7, 5, 6, 6}, {4, 1, 5, 2}, {6, 1, 5, 2}},
{{1, 3, 3, 3}, {7, 6, 5, 5}, {1, 2, 3, 2}, {6, 4, 5, 4}, {0, 6, 2, 5}, {6, 3, 4, 3}, {1, 6, 2, 6}, {7, 5, 6, 4}, {0, 5, 1, 6}, {7, 4, 7, 6}, {0, 4, 0, 6}, {4, 3, 3, 2}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 1, 5, 2}, {0, 5, 4, 1}, {7, 6, 5, 5}, {1, 3, 2, 3}, {7, 5, 4, 2}, {4, 1, 5, 2}, {6, 3, 5, 2}, {0, 4, 0, 6}, {5, 5, 6, 3}},
{{1, 1, 2, 1}, {6, 3, 4, 3}, {0, 6, 2, 5}, {7, 2, 4, 5}, {0, 2, 1, 1}, {6, 4, 5, 4}, {1, 6, 2, 6}, {6, 7, 5, 7}, {0, 5, 1, 6}, {7, 1, 5, 2}, {0, 4, 0, 6}, {6, 0, 4, 0}},
{{1, 3, 3, 3}, {6, 3, 4, 3}, {1, 2, 3, 2}, {6, 2, 5, 2}, {0, 6, 2, 5}, {7, 6, 5, 5}, {3, 2, 4, 3}, {5, 2, 4, 3}, {0, 1, 2, 2}, {7, 1, 5, 2}, {0, 2, 3, 5}, {7, 2, 4, 5}},
{{1, 4, 3, 4}, {6, 2, 4, 2}, {0, 6, 2, 5}, {6, 1, 5, 1}, {1, 3, 3, 3}, {4, 2, 3, 3}, {2, 5, 3, 3}, {7, 2, 6, 1}, {0, 1, 2, 2}, {6, 0, 5, 0}, {0, 2, 4, 6}, {7, 3, 6, 2}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 6, 5, 5}, {2, 5, 4, 4}, {6, 3, 5, 3}, {4, 4, 2, 5}, {5, 5, 3, 4}, {1, 3, 3, 3}, {5, 3, 4, 3}, {0, 5, 2, 3}, {7, 5, 5, 3}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 1, 5, 2}, {0, 5, 4, 1}, {6, 0, 5, 0}, {4, 1, 3, 0}, {7, 6, 5, 5}, {0, 4, 0, 6}, {6, 1, 4, 1}, {3, 0, 2, 1}, {7, 5, 4, 2}},
{{0, 6, 2, 5}, {7, 6, 5, 5}, {1, 3, 2, 3}, {6, 3, 4, 3}, {1, 6, 2, 6}, {6, 6, 5, 6}, {0, 5, 1, 6}, {7, 5, 6, 6}, {0, 4, 0, 6}, {7, 4, 7, 6}, {1, 0, 3, 0}, {6, 0, 4, 0}},
{{1, 4, 3, 4}, {6, 4, 4, 4}, {0, 6, 2, 5}, {7, 1, 5, 2}, {0, 5, 4, 1}, {7, 6, 5, 5}, {1, 3, 2, 3}, {6, 3, 5, 3}, {0, 4, 0, 6}, {6, 6, 5, 6}, {1, 2, 2, 2}, {7, 5, 6, 6}}
};


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://  Fonctions de gestion du type de donnée "arb" :

arb* init_arb () {
    arb* rep = (arb*)malloc(sizeof(arb));
    rep->nb_fils = 0;
    rep->coup = (int*)malloc(4*sizeof(int));
    rep->coup[0] = -1;
    rep->coup[1] = -1;
    rep->coup[2] = -1;
    rep->coup[3] = -1;
    rep->sous_arbres = (arb**)malloc(NbCoupsMoyen*sizeof(arb*));
    for (int i = 0; i < NbCoupsMoyen; i++){
        rep->sous_arbres[i] = NULL;
    }
    return rep;
}

void ajouter_branche (arb* arbre, int* coup){
    arbre->sous_arbres[arbre->nb_fils] = init_arb();
    arbre->sous_arbres[arbre->nb_fils]->coup[0] = coup[0];
    arbre->sous_arbres[arbre->nb_fils]->coup[1] = coup[1];
    arbre->sous_arbres[arbre->nb_fils]->coup[2] = coup[2];
    arbre->sous_arbres[arbre->nb_fils]->coup[3] = coup[3];
    arbre->nb_fils ++;
}

void afficher_arb (arb* arbre){
    if(arbre != NULL){
        printf(" [%d, %d, %d, %d] -> (", arbre->coup[0], arbre->coup[1], arbre->coup[2], arbre->coup[3]);
        for(int i = 0; i < arbre->nb_fils; i++){
            afficher_arb(arbre->sous_arbres[i]);
            printf("; ");
        }
        printf(")");
    }
    else{
        printf("(N); ");
    }
}

void liberer_arb (arb* arbre){
    for(int i = 0; i < arbre->nb_fils; i++){
        liberer_arb (arbre->sous_arbres[i]);
    }
    free(arbre->coup);
    free (arbre);
}

void ajouter_fils_arb (arb* arbre, arb* arbre_fils){
    arbre->sous_arbres[arbre->nb_fils] = arbre_fils;
    arbre->nb_fils ++;
}

int est_fils_coup (arb* arbre, int* coup){
    for(int i = 0; i < arbre->nb_fils; i++){
        if (arbre->sous_arbres[i]->coup[0] == coup[0] && arbre->sous_arbres[i]->coup[1] == coup[1] &&
            arbre->sous_arbres[i]->coup[2] == coup[2] && arbre->sous_arbres[i]->coup[3] == coup[3]){
            return i;
        }
    }
    return  -1;
}

bool dans_arb_partie (arb* arbre, ListeCoups* l, int n){
    if (n == l->occupe){
        return true;
    }
    else if(arbre == NULL){
        printf("NULL, ");
        return false;
    }
    else if(est_fils_coup(arbre, l->coups[n]) != -1){
        dans_arb_partie(arbre->sous_arbres[est_fils_coup(arbre, l->coups[n])], l, n+1);
    }
    else{
        printf("est_fils_coup == -1, ");
        return false;
    }
}

bool appel_dans_arb_partie (arb* arbre, ListeCoups* l){
    int n = l->occupe;
    if(n == 0){
        printf("partie vide !!!\n");
        return false;
    }
    return dans_arb_partie(arbre, l, 0);
}

ListeCoups* coups_suivant(arb* arbre, ListeCoups* l, int n){
    if (n == l->occupe){
        ListeCoups* rep = init_liste_coups();
        for(int i = 0; i < arbre->nb_fils; i++){
            ajouter_coup(rep, arbre->sous_arbres[i]->coup[0], arbre->sous_arbres[i]->coup[1], arbre->sous_arbres[i]->coup[2], arbre->sous_arbres[i]->coup[3], 0);
        }
        return rep;
    }
    else if(arbre == NULL){
        ListeCoups* rep = init_liste_coups();
        return rep;
    }
    else if(est_fils_coup(arbre, l->coups[n]) != -1){
        return coups_suivant(arbre->sous_arbres[est_fils_coup(arbre, l->coups[n])], l, n+1);
    }
    else{
        ListeCoups* rep = init_liste_coups();
        return rep;
    }
}

ListeCoups* appel_coups_suivant(arb* arbre, ListeCoups* l){
    int n = l->occupe;
    if(n == 0){
        printf("ERREUR : Dans 'appel_coup_suivant', le champ partie dest vide !\n");
    }
    return coups_suivant(arbre, l, 0);
}

int dans_arbre (arb* arbre, int* coups){
    /* Cette fonction renvoie l'indice du premier fils de "arbre" qui contient le
    tableau coup comme étiquette, elle renvoie -1 s'il n'est pas dans les fils*/
    for (int i = 0; i < arbre->nb_fils; i++){
        if (arbre->sous_arbres[i]->coup[0] == coups[0] && arbre->sous_arbres[i]->coup[1] == coups[1] &&
            arbre->sous_arbres[i]->coup[2] == coups[2] && arbre->sous_arbres[i]->coup[3] == coups[3]){
            return i;
        }
    }
    return -1;
}

arb* generer_arb_tableau (int ouverture[ProfondeurOuverture][4]){
    arb* rep = init_arb();
    rep->coup[0] = -1;
    rep->coup[1] = -1;
    rep->coup[2] = -1;
    rep->coup[3] = -1;
    arb* actuel = rep;
    int i = 0;
    while(i < ProfondeurOuverture){
        ajouter_branche(actuel, ouverture[i]);
        actuel = actuel->sous_arbres[0];
        i++;
    }
    return rep;
}

void ajouter_fils_arb_sans_doublons (arb* arbre, arb* ouverture){
    /* Prends en entrée un arbre peigne ouverture qui correspond à un unique enchainement de coups*/
    if (ouverture->sous_arbres[0] != NULL){
        if (dans_arbre(arbre, ouverture->sous_arbres[0]->coup) >= 0){
            ajouter_fils_arb_sans_doublons (arbre->sous_arbres[dans_arbre(arbre, ouverture->sous_arbres[0]->coup)], ouverture->sous_arbres[0]);
        }
        else if (dans_arbre(arbre, ouverture->sous_arbres[0]->coup) == -1){
            ajouter_fils_arb(arbre, ouverture->sous_arbres[0]);
        }
    }
    else if (ouverture->sous_arbres[0] == NULL){
        printf("Déjà dans l'arbre !\n");
        return;
    }
    else if (arbre == NULL || ouverture == NULL){
        printf("Un des arbres est vide !\n");
        return;
    }
}

arb* creer_arbre_ouvertures (int ouvertures[NBouvertures][ProfondeurOuverture][4]){
    arb* arb_ouvertures = init_arb();
    for (int i = 0; i < NBouvertures; i++){
        arb* ouverture = generer_arb_tableau(ouvertures[i]);
        ajouter_fils_arb_sans_doublons(arb_ouvertures, ouverture);
    }
    return arb_ouvertures;
}

int* coup_ouverture (ListeCoups* partie, arb* ouvertures){
    ListeCoups* suites_enregistrees = appel_coups_suivant(ouvertures, partie);
    int* rep = (int*)malloc(4*sizeof(int));
    if(suites_enregistrees->occupe == 0){
        liberer_liste_coups(suites_enregistrees);
        rep[0] = -1; rep[1] = -1; rep[2] = -1; rep[3] = -1;
        return rep;
    }
    int r = rand();
    int n = r % suites_enregistrees->occupe;
    rep[0] = suites_enregistrees->coups[n][0];
    rep[1] = suites_enregistrees->coups[n][1];
    rep[2] = suites_enregistrees->coups[n][2];
    rep[3] = suites_enregistrees->coups[n][3];
    liberer_liste_coups(suites_enregistrees);
    return rep;
}

