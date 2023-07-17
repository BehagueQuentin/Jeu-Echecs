//########################################################################################################################################################// 
//################################################################//     Jeu d'échec     //###############################################################//
//########################################################################################################################################################// 

/* Cette partie ne concerne que l'affichage des modifications, la plupart des calculs intéressants sont effectués dans le fichier "deplacement.c".
Ce dossier contient aussi le bot qui contre qui l'utilisateur joue (sûrement un tentinet plus intéressant)*/

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Importation des bibliothèques :

#include <SDL.h>
#include <windows.h>
#include "deplacement.h"
#include <time.h>
#include "ouverture.h"

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Déclaration des variables globales d'affichage :

#define Largeur 700
#define Hauteur 700
#define LargeurCase 80
#define HauteurCase 80
#define CoinSupX 30
#define CoinSupY 30
#define FPS_LIMIT 33
#define AlphaSurlign 120
#define Incolore 0
#define Rouge 1
#define RougeR 255
#define RougeG 0
#define RougeB 0
#define Jaune 2
#define JauneR 255
#define JauneG 247
#define JauneB 74
#define NbCoups 2000
#define NbPossibiliteMax 500
#define Profondeur 3
#define FacteurProfondeur 2
#define AmeliorationMax 3

int nb_c = 0;
int* nb_coups_envisage = &nb_c;

int cps = 0;
int* cps_tot = &cps;

double tmps = 0.0;
double* temps_total = &tmps;

bool ouv = true;
bool* ouverture_connue = &ouv;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Tableau contenant les coordonnées des cases :

int coord[8][16] = {
    {30, 30, 30, 110, 30, 190, 30, 270, 30, 350, 30, 430, 30, 510, 30, 590},
    {110, 30, 110, 110, 110, 190, 110, 270, 110, 350, 110, 430, 110, 510, 110, 590},
    {190, 30, 190, 110, 190, 190, 190, 270, 190, 350, 190, 430, 190, 510, 190, 590},
    {270, 30, 270, 110, 270, 190, 270, 270, 270, 350, 270, 430, 270, 510, 270, 590},
    {350, 30, 350, 110, 350, 190, 350, 270, 350, 350, 350, 430, 350, 510, 350, 590},
    {430, 30, 430, 110, 430, 190, 430, 270, 430, 350, 430, 430, 430, 510, 430, 590},
    {510, 30, 510, 110, 510, 190, 510, 270, 510, 350, 510, 430, 510, 510, 510, 590},
    {590, 30, 590, 110, 590, 190, 590, 270, 590, 350, 590, 430, 590, 510, 590, 590}
    };

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Gestion des erreurs :

void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_ExitWithErrorAndDestroy(const char *message, SDL_Window *window, SDL_Renderer *renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Gestion des FPS :

void SDL_LimitFPS(unsigned int limit){
    unsigned int ticks = SDL_GetTicks();
    if(limit < ticks){
        return;
    }
    else if (limit>ticks + FPS_LIMIT){
        SDL_Delay(FPS_LIMIT);
    }
    else {
        SDL_Delay(limit - ticks);
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Fonctions d'affichage :

SDL_Rect generer_texture_rectange_fond(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture * texture){
    SDL_Surface *image = NULL;
    image = SDL_LoadBMP("Images/Echiquier.bmp");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_Rect rectangle;
    if (SDL_QueryTexture(texture, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;
    return rectangle;
}

void creer_rectangle(SDL_Window *window, SDL_Renderer* renderer, int i, int j, int r, int g, int b){
    /* Permet d'afficher les surlignements de couleur (rouge ou jaune) en case (i,j) */
    if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)!= 0){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de la transparence", window, renderer);
    }
    if(SDL_SetRenderDrawColor(renderer, r, g, b, AlphaSurlign)!=0){
        SDL_ExitWithError("Echec changement de couleur de tracer");
    }

    SDL_Rect case_couleur;
    case_couleur.x = coord[i][2*j];
    case_couleur.y = coord[i][2*(7-j)+1];
    case_couleur.w = LargeurCase;
    case_couleur.h = HauteurCase;

    if(SDL_RenderFillRect(renderer, &case_couleur)!=0){
        SDL_ExitWithError("Echec de la creation du rectangle");
    }
}

void afficher_piece (SDL_Window *window, SDL_Renderer* renderer, int i, int j, int type){
    /* Affiche une piece précise en case (i, j) de l'échiquier. */
    SDL_Surface *imageP = NULL;
    SDL_Texture *textureP = NULL;
    switch (type)
    {
    case PionN :
        imageP = SDL_LoadBMP("Images/PionNoir.bmp");
        break;
    case FouN :
        imageP = SDL_LoadBMP("Images/FouNoir.bmp");
        break;
    case CavalierN :
        imageP = SDL_LoadBMP("Images/CavalierNoir.bmp");
        break;
    case TourN :
        imageP = SDL_LoadBMP("Images/TourNoir.bmp");
        break;
    case DameN :
        imageP = SDL_LoadBMP("Images/DameNoir.bmp");
        break;
    case RoiN :
        imageP = SDL_LoadBMP("Images/RoiNoir.bmp");
        break;
    case PionB :
        imageP = SDL_LoadBMP("Images/PionBlanc.bmp");
        break;
    case FouB :
        imageP = SDL_LoadBMP("Images/FouBlanc.bmp");
        break;
    case CavalierB :
        imageP = SDL_LoadBMP("Images/CavalierBlanc.bmp");
        break;
    case TourB :
        imageP = SDL_LoadBMP("Images/TourBlanc.bmp");
        break;
    case DameB :
        imageP = SDL_LoadBMP("Images/DameBlanc.bmp");
        break;
    case RoiB :
        imageP = SDL_LoadBMP("Images/RoiBlanc.bmp");
        break;
    default:
        break;
    }
    if(imageP == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image", window, renderer);
    }
    textureP = SDL_CreateTextureFromSurface(renderer, imageP);
    SDL_FreeSurface(imageP);
    if(textureP == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    // Rectangle contenant notre image :
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureP, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (CoinSupX + j*LargeurCase);
    rectangle.y = (CoinSupY + (7-i)*HauteurCase); ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GROSSE MODIF CONNARD

    // Affichage final de la texture :
    if(SDL_RenderCopy(renderer, textureP, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture", window, renderer);
    }
    SDL_DestroyTexture(textureP);
}

void afficher_piece_mouvement (SDL_Window *window, SDL_Renderer* renderer, int type, int pos_x, int pos_y){
    /* Affiche une pièce au position exacte renseignée, ne correspondant pas nécessairement à une case.
    Cette fonction est utilisées pour les déplacements de type 'grab and drop' pour que l'image suive le curseur*/
    SDL_Surface *imageP = NULL;
    SDL_Texture *textureP = NULL;
    switch (type){
        case PionN :
            imageP = SDL_LoadBMP("Images/PionNoir.bmp");
            break;
        case FouN :
            imageP = SDL_LoadBMP("Images/FouNoir.bmp");
            break;
        case CavalierN :
            imageP = SDL_LoadBMP("Images/CavalierNoir.bmp");
            break;
        case TourN :
            imageP = SDL_LoadBMP("Images/TourNoir.bmp");
            break;
        case DameN :
            imageP = SDL_LoadBMP("Images/DameNoir.bmp");
            break;
        case RoiN :
            imageP = SDL_LoadBMP("Images/RoiNoir.bmp");
            break;
        case PionB :
            imageP = SDL_LoadBMP("Images/PionBlanc.bmp");
            break;
        case FouB :
            imageP = SDL_LoadBMP("Images/FouBlanc.bmp");
            break;
        case CavalierB :
            imageP = SDL_LoadBMP("Images/CavalierBlanc.bmp");
            break;
        case TourB :
            imageP = SDL_LoadBMP("Images/TourBlanc.bmp");
            break;
        case DameB :
            imageP = SDL_LoadBMP("Images/DameBlanc.bmp");
            break;
        case RoiB :
            imageP = SDL_LoadBMP("Images/RoiBlanc.bmp");
            break;
        default:
            break;
    }
    
    if(imageP == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image", window, renderer);
    }
    textureP = SDL_CreateTextureFromSurface(renderer, imageP);
    SDL_FreeSurface(imageP);
    if(textureP == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    // Rectangle contenant notre image :
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureP, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = pos_x - LargeurCase/2;
    rectangle.y = pos_y - HauteurCase/2; 

    // Affichage final de la texture :
    if(SDL_RenderCopy(renderer, textureP, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 1", window, renderer);
    }
    SDL_DestroyTexture(textureP);
}

void afficher_mouvement_possible(SDL_Window *window, SDL_Renderer* renderer, int** p, int** coups){
    /* Affiche les déplacements et prises possibles d'une pièce.
    Elle affiche un point transparent si le déplacement est une prise, 
    et un cercle transparent si la case est occupée par une pièce adverse. */
    SDL_Surface *imageDepl = NULL;
    SDL_Surface *imagePrise = NULL;
    SDL_Texture *textureDepl = NULL;
    SDL_Texture *texturePrise = NULL;
    imageDepl = SDL_LoadBMP("Images/PositionAtteignable.bmp");
    imagePrise = SDL_LoadBMP("Images/PriseAtteignable.bmp");
    if(imageDepl == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 3", window, renderer);
    }
    if(imagePrise == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 4", window, renderer);
    }
    textureDepl = SDL_CreateTextureFromSurface(renderer, imageDepl);
    texturePrise = SDL_CreateTextureFromSurface(renderer, imagePrise);
    SDL_FreeSurface(imageDepl);
    SDL_FreeSurface(imagePrise);

    if(textureDepl == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    if(texturePrise == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    // Rectangles contenant nos images :
    SDL_Rect rectangle1;
    if (SDL_QueryTexture(textureDepl, NULL,NULL, &rectangle1.w, &rectangle1.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    SDL_Rect rectangle2;
    if (SDL_QueryTexture(texturePrise, NULL,NULL, &rectangle2.w, &rectangle2.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    
    for(int i = 0; i<8; i++){
        for(int j = 0; j <8; j++){
           if (coups[i][j] == 1 && p[i][j] == 0){
                rectangle1.x = (CoinSupX + j*LargeurCase);
                rectangle1.y = (CoinSupY + (7-i)*HauteurCase);
                if(SDL_RenderCopy(renderer, textureDepl, NULL, &rectangle1) != 0){
                    SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture", window, renderer);
                }
            }
            else if (coups[i][j] == 1 && p[i][j] != 0){
                rectangle2.x = (CoinSupX + j*LargeurCase); 
                rectangle2.y = (CoinSupY + (7-i)*HauteurCase); 
                if(SDL_RenderCopy(renderer, texturePrise, NULL, &rectangle2) != 0){
                    SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture", window, renderer);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(textureDepl);
    SDL_DestroyTexture(texturePrise);
}

void rafraichir_plateau(SDL_Texture *texture, SDL_Window *window, SDL_Renderer* renderer, int** plateau, int** couleur_case, int i, int j, int** coups){
    // Affichage de l'image de fond :
    SDL_Rect rectangle;
    if (SDL_QueryTexture(texture, NULL,NULL, &rectangle.w, &rectangle.h) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;
    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 2", window, renderer);
    }
    // Affichage des cases coloriée :
    for(int k = 0; k < 8; k++){
        for(int l = 0; l < 8; l++){
            if(couleur_case[l][k] == Rouge){
                creer_rectangle(window, renderer, k, l, RougeR, RougeG, RougeB);
            }
            else if (couleur_case[l][k] == Jaune){
                creer_rectangle(window, renderer, k, l, JauneR, JauneG, JauneB);
            }
        }
    }
    // Affichage des pièces d'échecs :
    for(int k = 0; k < 8; k++){
        for(int l = 0; l < 8; l++){
            if(plateau[k][l] != 0){
                afficher_piece(window, renderer, k, l, plateau[k][l]);
            }
        }
    }
    // Affichage des coups possibles :
    if( i != -1){
        afficher_mouvement_possible(window, renderer, plateau, coups);
    }
    SDL_RenderPresent(renderer);
}

void rafraichir_plateau_mouvement (SDL_Texture *texture, SDL_Window *window, SDL_Renderer* renderer, int** plateau, int** couleur_case, int i, int j, int** coups, int pos_x, int pos_y){
    // Affichage de l'image de fond :
    SDL_Rect rectangle;
    if (SDL_QueryTexture(texture, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;
    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 2", window, renderer);
    }
    // Affichage des cases coloriée :
    for(int k = 0; k < 8; k++){
        for(int l = 0; l < 8; l++){
            if(couleur_case[l][k] == Rouge){
                creer_rectangle(window, renderer, k, l, RougeR, RougeG, RougeB);
            }
            else if (couleur_case[l][k] == Jaune){
                creer_rectangle(window, renderer, k, l, JauneR, JauneG, JauneB);
            }
        }
    }
    for(int k = 0; k < 8; k++){
        for(int l = 0; l < 8; l++){
            if(plateau[k][l] != 0 && (l != j || k != i) ){
                afficher_piece(window, renderer, k, l, plateau[k][l]);
            }
        }
    }
    // Affichage des coups possibles :
    afficher_mouvement_possible(window, renderer, plateau, coups);
    // Affichage de la pièce tenue :
    if (plateau[i][j] != 0){
        afficher_piece_mouvement(window, renderer, plateau[i][j], pos_x, pos_y);
    }
    SDL_RenderPresent(renderer);
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Fonctions d'interactions plateau-utilisateur :

void trouver_case(int x, int y, int* retour_i, int* retour_j){
    /* Fonction qui prend en entrée la position du curseur et renvoie
    la case de l'échiquier correspondante en modifiant les pointeur en entrée.
    Cette fonction renvoie (-1, -1) si le curseur est hors de l'échiquier. */
    bool trouver = false;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if (x >= coord[i][2*j] && x < coord[i][2*j] + LargeurCase && y >= coord[i][2*j+1] && y < coord[i][2*j+1] + HauteurCase){
                *retour_i = (7-j); 
                *retour_j = i;
                trouver = true;
            }
        }
    }
    if(!trouver){
        *retour_i = -1;
        *retour_j = -1;
    }
}

void colorier_case(int i, int j, int** couleurs, int c){
    /* Fonction qui effectue un changement de couleur en case (i, j)*/
    if (c == Rouge && couleurs[i][j] == c){
        couleurs[i][j] = Incolore;
    }
    else {
        couleurs[i][j] = c;
    }
}

void supprimer_couleur(int** couleurs, int c){
    /* Fonction retirant les couleur de type 'c' de l'échiquier */
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if (couleurs[i][j] == c){
                couleurs[i][j] = Incolore;
            }
        }
    }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Fonctions d'affichage de fin de partie et de promotion :

void afficher_mat(SDL_Window *window, SDL_Renderer* renderer){ 
    printf("Echec et mat !\n");
    SDL_Surface *imageMat = NULL;
    SDL_Texture *textureMat = NULL;
    imageMat = SDL_LoadBMP("Images/Defaite.bmp");
    if(imageMat == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 1", window, renderer);
    }
    textureMat = SDL_CreateTextureFromSurface(renderer, imageMat);
    SDL_FreeSurface(imageMat);
    if(textureMat == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureMat, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;

    if(SDL_RenderCopy(renderer, textureMat, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 1", window, renderer);
    }
    SDL_DestroyTexture(textureMat);
    SDL_Delay(500);
    SDL_RenderPresent(renderer);
}

void afficher_pat(SDL_Window *window, SDL_Renderer* renderer){
    printf ("Pat !\n");
    SDL_Surface *imageMat = NULL;
    SDL_Texture *textureMat = NULL;
    imageMat = SDL_LoadBMP("Images/Pat.bmp");
    if(imageMat == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 1", window, renderer);
    }
    textureMat = SDL_CreateTextureFromSurface(renderer, imageMat);
    SDL_FreeSurface(imageMat);
    if(textureMat == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureMat, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;

    if(SDL_RenderCopy(renderer, textureMat, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 1", window, renderer);
    }
    SDL_DestroyTexture(textureMat);
    SDL_Delay(500);
    SDL_RenderPresent(renderer);
}

int afficher_promotion(SDL_Window *window, SDL_Renderer* renderer, int colonne, int* etat_plateau){
    int choix_piece = -1; // Choix de la pièce
    int trait = etat_plateau[1];

    // Importation de l'image et gestion texture, erreurs, ... :
    SDL_Surface *imagePromo = NULL;
    SDL_Texture *texturePromo = NULL;
    if(trait == -TraitBlanc){
        imagePromo = SDL_LoadBMP("Images/PromotionBlanc.bmp");
    }
    else{
        imagePromo = SDL_LoadBMP("Images/PromotionNoir.bmp");
    }
    if(imagePromo == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image", window, renderer);
    }
    texturePromo = SDL_CreateTextureFromSurface(renderer, imagePromo);
    SDL_FreeSurface(imagePromo);
    if(texturePromo == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(texturePromo, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }

    // Gestion des coordonnées de blitage du rectangle
    if(trait == TraitBlanc){
        rectangle.x = (CoinSupX + colonne*LargeurCase -5);
        rectangle.y = CoinSupY + 4*HauteurCase - 5;
    }
    else{
        rectangle.x = (CoinSupX + colonne*LargeurCase -5);
        rectangle.y = CoinSupY - 5;
    }
    if(SDL_RenderCopy(renderer, texturePromo, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 1", window, renderer);
    }
    SDL_DestroyTexture(texturePromo);
    SDL_RenderPresent(renderer);


    // Boucle d'événement
    bool wait = true;
    int i = -1; int* pi = &i;
    int j = -1; int* pj = &j;
    int oldTime = SDL_GetTicks();
    while(wait){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                trouver_case(event.motion.x, event.motion.y, pi, pj);
                if(trait == -TraitBlanc && *pj == colonne){
                    if(*pi == 7){
                        choix_piece = DameB;
                        wait = false;
                    }
                    else if(*pi == 6){
                        choix_piece = TourB;
                        wait = false;
                    }
                    else if(*pi == 5){
                        choix_piece = FouB;
                        wait = false;
                    }
                    else if(*pi == 4){
                        choix_piece = CavalierB;
                        wait = false;
                    }
                }
                else if (trait = -TraitNoir && *pj == colonne){
                    if(*pi == 0){
                        choix_piece = DameN;
                        wait = false;
                    }
                    else if(*pi == 1){
                        choix_piece = TourN;
                        wait = false;
                    }
                    else if(*pi == 2){
                        choix_piece = FouN;
                        wait = false;
                    }
                    else if(*pi == 3){
                        choix_piece = CavalierN;
                        wait = false;
                    }
                }
            }
            if (event.type == SDL_QUIT){
                return -1;
            }
        }
        int newTime = SDL_GetTicks();
        if (newTime - oldTime < FPS_LIMIT)
        SDL_Delay(FPS_LIMIT - (newTime - oldTime));
        oldTime = SDL_GetTicks();
    }
    if(choix_piece == -1){
        printf("ERREUR : promotion !\n");
    }
    return choix_piece;
}

int attente_menu_de_base (SDL_Window *window, SDL_Renderer* renderer){
    int bot = 0;
    SDL_Surface *imageMenu = NULL;
    SDL_Texture *textureMenu = NULL;
    imageMenu = SDL_LoadBMP("Images/Acceuil.bmp");
    if(imageMenu == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image", window, renderer);
    }
    textureMenu = SDL_CreateTextureFromSurface(renderer, imageMenu);
    SDL_FreeSurface(imageMenu);
    if(textureMenu == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureMenu, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;

    if(SDL_RenderCopy(renderer, textureMenu, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture", window, renderer);
    }
    SDL_DestroyTexture(textureMenu);
    SDL_RenderPresent(renderer);

    bool wait = true;
    int oldTime = SDL_GetTicks();
    while(wait){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(event.motion.x > 50 && event.motion.y > 180 && event.motion.x < 300 && event.motion.y < 240){
                    bot = -1;
                    wait = false;
                }
                else if(event.motion.x > 150 && event.motion.y > 270 && event.motion.x < 400 && event.motion.y < 330){
                    bot = 1;
                    wait = false;
                }
            }
            
            if (event.type == SDL_QUIT){
                return bot;
            }
        }
        int newTime = SDL_GetTicks();
        if (newTime - oldTime < FPS_LIMIT)
        SDL_Delay(FPS_LIMIT - (newTime - oldTime));
        oldTime = SDL_GetTicks();
    }
    return bot;
}

int choix_trait (SDL_Window *window, SDL_Renderer* renderer){
    int trait;
    // Affichage du fond de menu :
    SDL_Surface *imageMenu = NULL;
    SDL_Texture *textureMenu = NULL;
    imageMenu = SDL_LoadBMP("Images/ChoixTrait.bmp");
    if(imageMenu == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 1", window, renderer);
    }
    textureMenu = SDL_CreateTextureFromSurface(renderer, imageMenu);
    SDL_FreeSurface(imageMenu);
    if(textureMenu == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(textureMenu, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;

    if(SDL_RenderCopy(renderer, textureMenu, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture 1", window, renderer);
    }
    SDL_DestroyTexture(textureMenu);
    SDL_RenderPresent(renderer);

    bool wait = true;
    int oldTime = SDL_GetTicks();
    while(wait){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                if(event.motion.x > 350 && event.motion.y > 250 && event.motion.x < 600 && event.motion.y < 310){
                    trait = TraitBlanc;
                    wait = false;
                }
                else if(event.motion.x > 100 && event.motion.y >500 && event.motion.x < 350 && event.motion.y < 560){
                    trait = TraitNoir;
                    wait = false;
                }
            }
            if(event.type == SDL_QUIT ) {
                return 0;
            } 
        }
        int newTime = SDL_GetTicks();
        if (newTime - oldTime < FPS_LIMIT)
        SDL_Delay(FPS_LIMIT - (newTime - oldTime));
        oldTime = SDL_GetTicks();
    }
    return trait;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Bot :

int heuristique(int** plateau, int* etat_plateau){
    int** blanc = creer_plateau();
    int** noir = creer_plateau();    
    positions_defense_double(plateau, etat_plateau, blanc, noir);
    int h = somme_plateau(blanc) - somme_plateau(noir);
    liberer_plateau(blanc);
    liberer_plateau(noir);
    return 150*somme_plateau_correspondance(plateau) + h;
}

int* minimax(int** plateau, int* etat_plateau, int profondeur, int alpha, int beta, bool minimise, int amelioration){
    int* rep = (int*)malloc(5*sizeof(int));
    switch(mat_ou_pat(plateau, etat_plateau)){
        case 0 :
            rep[4] = -1000000;
            break;
        case -1 :
            rep[4] = 0;
            return rep;
        case 1 :
            rep[4] = 0;
            return rep;
        case -2 :
            rep[4] = 40000;
            return rep;
        case 2 :
            rep[4] = -40000;
            return rep;
        default :
            printf("ERREUR : Dans 'maximise', probleme lors de l'appel a 'mat_ou_pat' !\n");
            return rep;
    }
    if(profondeur == 0){
        rep[4] = heuristique(plateau, etat_plateau);
        return rep;
    }
    else {
        if (minimise){
            ListeCoups* fils = generer_coups_possibles(plateau, etat_plateau);
            *nb_coups_envisage += fils->occupe;
            int nouvelle_profondeur = profondeur - 1;
            if(fils->occupe == 1){
                nouvelle_profondeur = profondeur;
            }
            else if(fils->occupe < FacteurProfondeur*profondeur && amelioration < AmeliorationMax){
                nouvelle_profondeur = profondeur;
                amelioration ++;
            }
            for(int i = 0; i < fils->occupe; i++){
                int** nouveau_plateau = copier_plateau(plateau);
                int* nouvel_etat_plateau = copier_etat_plateau(etat_plateau);
                appliquer_coup(nouveau_plateau, fils->coups[i][0], fils->coups[i][1], fils->coups[i][2], fils->coups[i][3], nouvel_etat_plateau);
                int* score = minimax (nouveau_plateau, nouvel_etat_plateau, nouvelle_profondeur, alpha, beta, false, amelioration);
                liberer_plateau(nouveau_plateau);
                free(nouvel_etat_plateau);
                if (score[4] > rep[4]){
                    rep[4] = score[4];
                    rep[0] = fils->coups[i][0];
                    rep[1] = fils->coups[i][1];
                    rep[2] = fils->coups[i][2];
                    rep[3] = fils->coups[i][3];
                }
                if (score[4] > beta) {
                    liberer_liste_coups(fils);
                    free(score);
                    return rep;
                }
                if (score[4] > alpha){
                    alpha = score[4];
                };
                free(score);
            }
            liberer_liste_coups(fils);
            return rep;
        }
        else {
            rep[4] = 1000000;
            ListeCoups* fils = generer_coups_possibles(plateau, etat_plateau);
            *nb_coups_envisage += fils->occupe;
            int nouvelle_profondeur = profondeur - 1;
            if(fils->occupe == 1){
                nouvelle_profondeur = profondeur;
            }
            else if(fils->occupe < FacteurProfondeur*profondeur && amelioration < AmeliorationMax){
                nouvelle_profondeur = profondeur;
                amelioration ++;
            }
            for(int i = 0; i < fils->occupe; i++){
                int** nouveau_plateau = copier_plateau(plateau);
                int* nouvel_etat_plateau = copier_etat_plateau(etat_plateau);
                appliquer_coup(nouveau_plateau, fils->coups[i][0], fils->coups[i][1], fils->coups[i][2], fils->coups[i][3], nouvel_etat_plateau);
                int* score = minimax (nouveau_plateau, nouvel_etat_plateau, nouvelle_profondeur, alpha, beta, true, amelioration);
                liberer_plateau(nouveau_plateau);
                free(nouvel_etat_plateau);
                if (score[4] < rep[4]){
                    rep[4] = score[4];
                    rep[0] = fils->coups[i][0];
                    rep[1] = fils->coups[i][1];
                    rep[2] = fils->coups[i][2];
                    rep[3] = fils->coups[i][3];
                }
                if (score[4] < alpha) {
                    liberer_liste_coups(fils);
                    free(score);
                    return rep;
                }
                if (score[4] < beta){
                    beta = score[4];
                };
                free(score);
            }
            liberer_liste_coups(fils);
            return rep;
        }
    }
}

int* bisif4(int** plateau, int* etat_plateau, ListeCoups* partie, arb* arbre){
    if (*ouverture_connue){
        int* repouverture = coup_ouverture(partie, arbre);
        if(repouverture[0] == -1){
            *ouverture_connue = false;
        }
        else{
            return repouverture;
        }
    }
    int* rep = (int*)malloc(4*sizeof(int));
    if(etat_plateau[1] == TraitBlanc){
        int* meilleur_coup = minimax(plateau, etat_plateau, Profondeur, -1000000, 1000000, true, 0);
        rep[0] = meilleur_coup[0];
        rep[1] = meilleur_coup[1];
        rep[2] = meilleur_coup[2];
        rep[3] = meilleur_coup[3];
        free(meilleur_coup);
        return rep;
    }
    else{
        int* meilleur_coup = minimax(plateau, etat_plateau, Profondeur, -1000000, 1000000, false, 0);
        rep[0] = meilleur_coup[0];
        rep[1] = meilleur_coup[1];
        rep[2] = meilleur_coup[2];
        rep[3] = meilleur_coup[3];
        free(meilleur_coup);
        return rep;
    }
}

bool appel_bot (SDL_Texture *texture, SDL_Rect rectangle, SDL_Window *window, SDL_Renderer* renderer, int**plateau, int* etat_plateau, int** couleur_case, ListeCoups* sauvegarde_partie, arb* arbre){
    // Récupération et application du coup joué par le bot :
    double time_spent = 0.0;
    clock_t begin = clock();
    int* mouvement = bisif4(plateau, etat_plateau, sauvegarde_partie, arbre);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    *temps_total += time_spent;
    *cps_tot += *nb_coups_envisage;
    //printf("Nb coups envisage = %d !\n", *nb_coups_envisage);
    //printf("Execution en %f sec\n\n", time_spent);
    *nb_coups_envisage = 0;
    
    if(mouvement[0] == -1 || mouvement[1] == -1 || mouvement[2] == -1 || mouvement[3] == -1){
        printf("ERREUR : Le bot ne renvoie pas de coup valide !\n");
    }
    if(mouvement[0] == mouvement[2] && mouvement[1] == mouvement[3]){
        printf("ERREUR : Le bot n'a pas effectué de coup lors de ce tour !\n");
    }
    int promotion = appliquer_coup(plateau, mouvement[0], mouvement[1], mouvement[2], mouvement[3], etat_plateau);
    ajouter_coup(sauvegarde_partie, mouvement[0], mouvement[1], mouvement[2], mouvement[3], -etat_plateau[0]);
    if(promotion != -1){
        plateau[mouvement[2]][mouvement[3]] = DameB*(-etat_plateau[1]);
    }
    // Affichage du plateau :
    colorier_case(mouvement[0], mouvement[1], couleur_case, Jaune);
    colorier_case(mouvement[2], mouvement[3], couleur_case, Jaune);
    int** coups = creer_plateau();
    rafraichir_plateau(texture, window, renderer, plateau, couleur_case, -1, -1, coups);
    liberer_plateau(coups);
    free(mouvement);
    

    switch (mat_ou_pat(plateau, etat_plateau)){
        case 0:
            return false;
            break;
        case 1:
            afficher_pat(window, renderer);
            afficher_partie_user(sauvegarde_partie);
            return true;
            break;
        case -1:
            afficher_pat(window, renderer);
            afficher_partie_user(sauvegarde_partie);
            return true;
            break;
        
        case 2:
            afficher_mat(window, renderer);
            afficher_partie_user(sauvegarde_partie);
            return true;
            break; 

        case -2:
            afficher_mat(window, renderer);
            afficher_partie_user(sauvegarde_partie);
            return true;
            break; 
        
        default:
            printf("ERREUR : Appel de 'mat_ou_pat' ne renvoie pas un résultat valide !\n");
            return true;
            break;    
    }
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Main :

int main(int argc, char** argv){
    
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Détache la console du processus :

    FreeConsole();

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Affichage de la version SDL employée :
    SDL_version nb;
    SDL_VERSION(&nb);
    
    //printf("Importation SDL V %d.%d.%d !\n", nb.major, nb.minor, nb.patch);
    
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Graine temporelle pour l'aléatoire :
    
    srand(time(NULL));

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Création de l'arbre des ouvertures :
    
    arb* arbre = creer_arbre_ouvertures(ouvertures);
    
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Pointeur sur les structures d'affichage :
    
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Gestion de l'initialisation Audio/Video de la SDL :

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        SDL_ExitWithError("Echec d'initialisation Audio/Video");
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Création de la fenêtre et du rendu :

    if(SDL_CreateWindowAndRenderer(Largeur, Hauteur, 0, &window, &renderer) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la fenetre et du rendu", window, renderer);
    }
    SDL_SetWindowTitle(window, "My first chess algorithm (MiniMax algorithm with Alpha-beta pruning)");

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Création de l'image de fond pour la fenêtre :

    SDL_Texture *texture = NULL;
    SDL_Surface *image = NULL;
    image = SDL_LoadBMP("Images/Echiquier.bmp");
    if(image == NULL){
        SDL_ExitWithErrorAndDestroy("Echec de chargement de l'image 5 ", window, renderer);
    }
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    if(texture == NULL){
        SDL_ExitWithErrorAndDestroy("Echec chargement de la texture", window, renderer);
    }
    SDL_Rect rectangle;
    if (SDL_QueryTexture(texture, NULL,NULL, &rectangle.w, &rectangle.h)!=0){
        SDL_ExitWithErrorAndDestroy("Echec de creation de la texture", window, renderer);
    }
    rectangle.x = (Largeur-rectangle.w)/2;
    rectangle.y = (Hauteur-rectangle.h)/2;
    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_ExitWithErrorAndDestroy("Echec de l'affichage de la texture", window, renderer);
    }

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Gestion des FPS :
    unsigned int frame_limit = 0;
    frame_limit = SDL_GetTicks() + FPS_LIMIT;
    SDL_LimitFPS(frame_limit);
    frame_limit = SDL_GetTicks() + FPS_LIMIT;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Création du plateau :

    int** plateau = creer_plateau();
    int* etat_plateau = creer_etat_plateau();
    int** coups = creer_plateau();
    int** couleur_case = creer_plateau();
    ListeCoups* sauvegarde_partie = init_liste_coups();
    initialiser_plateau(plateau);
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Varibles d'état du curseur :

    int caseInitialeI = -1;    
    int* case_initiale_i = &caseInitialeI;
    int caseInitialeJ = -1;    
    int* case_initiale_j = &caseInitialeJ;
    int caseFinaleI = -1;    
    int* case_finale_i = &caseFinaleI;
    int caseFinaleJ = -1;    
    int* case_finale_j = &caseFinaleJ;

    bool tien_piece = false;
    bool selectionne_piece = false;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Variables de fin :

    bool programme_ouvert = true;
    bool partie_terminee = false;
    bool coup_joue = false;

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Premier affichage du plateau :

    int trait_humain = 0;
    int bot = 0;
    rafraichir_plateau(texture, window, renderer, plateau, couleur_case, -1, -1, coups);

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Boucle d'événements :
    int oldTime = SDL_GetTicks();
    while(programme_ouvert){
        // Séléction de l'adversaire (bot ou deux joueurs) //
        if (bot == 0 && trait_humain == 0){
            bot = attente_menu_de_base(window, renderer);
            if(bot == 0){
                programme_ouvert = false;
                break;
            }
            rafraichir_plateau(texture, window, renderer, plateau, couleur_case, -1, -1, coups);
        }

        // Séléction du trait du joueur humain //
        if (bot == 1 && trait_humain == 0){
            trait_humain = choix_trait(window, renderer);
            if(trait_humain == 0){
                programme_ouvert = false;
                break;
            }
            rafraichir_plateau(texture, window, renderer, plateau, couleur_case, -1, -1, coups);
        }

        // Gestion des mouvements du bot //
        if(bot && etat_plateau[1] == -trait_humain && !partie_terminee){
            partie_terminee = appel_bot(texture, rectangle, window, renderer, plateau, etat_plateau, couleur_case, sauvegarde_partie, arbre);
        }

        // Boucle événements //
        SDL_Event event;
        while(SDL_PollEvent(&event) && programme_ouvert){
            switch(event.type){
                case SDL_MOUSEBUTTONDOWN :
                    if (event.button.button == SDL_BUTTON_LEFT && !partie_terminee){
                        if (!selectionne_piece){

                            supprimer_couleur(couleur_case, Jaune);
                            supprimer_couleur(couleur_case, Rouge);

                            trouver_case(event.motion.x, event.motion.y, case_initiale_i, case_initiale_j);
                            if(*case_initiale_i == -1){
                                // Si on se trouve hors du plateau :
                                reinit_plateau(coups);
                            }
                            else{
                                // Si on est dans le plateau :
                                if (plateau[*case_initiale_i][*case_initiale_j]*etat_plateau[1] > 0){
                                    // Si on sélectionne une pièce de notre camp :
                                    deplacement_legal_piece(plateau, *case_initiale_i, *case_initiale_j, etat_plateau, coups);
                                    tien_piece = true;
                                    colorier_case(*case_initiale_i, *case_initiale_j, couleur_case, Jaune);
                                }
                                else{
                                    // Si on clique sur une case vide ou du camp adverse :
                                    colorier_case(*case_initiale_i, *case_initiale_j, couleur_case, Jaune);
                                    *case_initiale_i = -1; *case_initiale_j = -1; 
                                    tien_piece = false;
                                }
                            }
                            // Affichage de la pièce sélectionnée et des coups licite de celle-ci : 
                            rafraichir_plateau(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups);
                        }

                        //:::::::::::::::::::::::::::::::::::::::// Bouton gauche enfoncé en ayant sélectionné une pièce !
                        else if (selectionne_piece){
                            trouver_case(event.motion.x, event.motion.y, case_finale_i, case_finale_j);
                            selectionne_piece = false;
                            
                            // Si on clique à l'extérieur :
                            if(*case_initiale_i == -1 || *case_finale_i == -1 || *case_finale_i == -1){
                                supprimer_couleur(couleur_case, Jaune);
                                supprimer_couleur(couleur_case, Rouge);
                                reinit_plateau(coups);
                            }

                            // Si la case correspond à un mouvement licite :
                            else if(coups[*case_finale_i][*case_finale_j] == 1){
                                int promotion = appliquer_coup(plateau, *case_initiale_i, *case_initiale_j, *case_finale_i, *case_finale_j, etat_plateau);
                                ajouter_coup(sauvegarde_partie, *case_initiale_i, *case_initiale_j, *case_finale_i, *case_finale_j, -etat_plateau[0]);
                                coup_joue = true;
                                if (promotion != -1){
                                    int nouvelle_piece = afficher_promotion(window, renderer, *case_finale_j, etat_plateau);
                                    if(nouvelle_piece == -1){
                                        programme_ouvert = false;
                                    }
                                    plateau[*case_finale_i][*case_finale_j] = nouvelle_piece;
                                }
                                colorier_case(*case_finale_i, *case_finale_j, couleur_case, Jaune);
                                *case_initiale_i = -1; *case_initiale_j = -1;
                                reinit_plateau(coups);
                                selectionne_piece = false;
                            }
                            // Si on a eu sélection en cours, mais qu'on souhaite changer de pièce :
                            else if(plateau[*case_finale_i][*case_finale_j]*(etat_plateau[1]) > 0){
                                supprimer_couleur(couleur_case, Jaune);
                                supprimer_couleur(couleur_case, Rouge);
                                trouver_case(event.motion.x, event.motion.y, case_initiale_i, case_initiale_j);
                                deplacement_legal_piece(plateau, *case_initiale_i, *case_initiale_j, etat_plateau, coups);
                                colorier_case(*case_initiale_i, *case_initiale_j, couleur_case, Jaune);
                                tien_piece = true;
                                //selectionne_piece = true;
                            }
                            // Si le mouvement demandé est impossible :
                            else{
                                supprimer_couleur(couleur_case, Jaune);
                                supprimer_couleur(couleur_case, Rouge);
                                *case_initiale_i = -1; *case_initiale_j = -1;
                                reinit_plateau(coups);
                                selectionne_piece = false;
                            }
                            if (coup_joue){         
                                coup_joue = false;
                                if (mat_ou_pat(plateau, etat_plateau) != 0 ){
                                    rafraichir_plateau(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups);
                                    afficher_mat(window, renderer);
                                    partie_terminee = true;
                                    afficher_partie_user(sauvegarde_partie);
                                    break;
                                }
                            }
                            rafraichir_plateau(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups);
                        }
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT && !partie_terminee){
                        trouver_case(event.motion.x, event.motion.y, case_initiale_i, case_initiale_j);
                        
                        if(*case_initiale_i == -1 || *case_initiale_j == -1){
                            supprimer_couleur(couleur_case, Jaune);
                            supprimer_couleur(couleur_case, Rouge);
                        }
                        else {
                            supprimer_couleur(couleur_case, Jaune);
                            colorier_case(*case_initiale_i, *case_initiale_j, couleur_case, Rouge);
                        }

                        rafraichir_plateau(texture, window, renderer, plateau, couleur_case, -1, -1, coups);
                        selectionne_piece = false;
                        tien_piece = false;
                    }
                    break;

                case SDL_MOUSEBUTTONUP :
                    if (event.button.button == SDL_BUTTON_LEFT && !partie_terminee){
                        if (tien_piece){
                            trouver_case(event.motion.x, event.motion.y, case_finale_i, case_finale_j);

                            tien_piece = false;

                            if(*case_initiale_i == -1 || *case_initiale_j == -1 || *case_finale_i == -1 || *case_finale_i == -1){
                                supprimer_couleur(couleur_case, Rouge);
                            }
                            else if (*case_finale_i == *case_initiale_i && *case_finale_j == *case_initiale_j){
                                *case_finale_i = -1; *case_finale_j = -1;
                                selectionne_piece = true;
                            }
                            else if(coups[*case_finale_i][*case_finale_j] == 1){
                                int promotion = appliquer_coup(plateau, *case_initiale_i, *case_initiale_j, *case_finale_i, *case_finale_j, etat_plateau);
                                ajouter_coup(sauvegarde_partie, *case_initiale_i, *case_initiale_j, *case_finale_i, *case_finale_j, -etat_plateau[0]);
                                coup_joue = true;
                                if (promotion != -1){
                                    int nouvelle_piece = afficher_promotion(window, renderer, *case_finale_j, etat_plateau);
                                    if(nouvelle_piece == -1){
                                        programme_ouvert = false;
                                    }
                                    plateau[*case_finale_i][*case_finale_j] = nouvelle_piece;
                                }
                                colorier_case(*case_finale_i, *case_finale_j, couleur_case, Jaune);
                                *case_initiale_i = -1; *case_initiale_j = -1;
                                reinit_plateau(coups);
                                selectionne_piece = false;
                            }
                            rafraichir_plateau(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups);
                        }
                        if (coup_joue){
                            if(mat_ou_pat(plateau, etat_plateau) != 0){
                                rafraichir_plateau(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups);
                                afficher_mat(window, renderer);
                                partie_terminee = true;
                                afficher_partie_user(sauvegarde_partie);
                            }
                            coup_joue = false;
                        }
                        break;
                    }
                


                //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Code pour gérer les déplacement de la souris :

                case SDL_MOUSEMOTION :
                    if(tien_piece && *case_initiale_i != -1 && *case_initiale_j != -1 && !partie_terminee){
                        rafraichir_plateau_mouvement(texture, window, renderer, plateau, couleur_case, *case_initiale_i, *case_initiale_j, coups, event.motion.x, event.motion.y);
                    }
                    break;

                case SDL_QUIT :
                    programme_ouvert = false;
                    break;
                
                default:
                    break;
            }
            
        }
        int newTime = SDL_GetTicks();
        if (newTime - oldTime < FPS_LIMIT)
        SDL_Delay(FPS_LIMIT - (newTime - oldTime));
        oldTime = SDL_GetTicks();
    }
    
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::// Libération de la mémoire :

    //printf("\n Temps total d'execution : %f, Nombre de coups total : %d\n\n", *temps_total, *cps_tot);
    liberer_arb(arbre);
    liberer_liste_coups(sauvegarde_partie);
    liberer_plateau(plateau);
    liberer_plateau(coups);
    liberer_plateau(couleur_case);
    free(etat_plateau);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("\n Sortie sans erreur.");
    return EXIT_SUCCESS;
}

/* Bugs to fix :

 - Afficher correctement la promotion                                                                       \(^o^)/
 - Gérer correctement les échecs                                                                            \(^o^)/
 - Problème avec la prise du pion (liée à la double poussée, il me semble)                                  \(^o^)/
 - Trop d'utilisation CPU lors de l'attente (simple à corriger)                                             \(^o^)/
 - Permettre la sortie lors de l'affichage de l'échec et mat                                                \(^o^)/
 - REcoder le bot, ...                                                                                      \(^o^)/
 - Revoir toute la gestion mémoire de l'affichage                                                           \(^o^)/
 - Revoir la gestion mémoire de l'appel à minimax, IMPORTANT !                                              \(^o^)/
 - REcoder l'heuristique d'ordonnencement pour l'élagage                                                    \(^o^)/
 - Stopper la prise en compte des entrées après echec et mat par le bot                                     \(^o^)/
 - Faire correctement l'affichage du pat et du mat dans la boucle bot                                       \(^o^)/
 - Tester la diffèrence avec/sans heuristique d'élagage, s'assurer de la diminution des calculs             \(^o^)/
 - Tenter d'augmenter la profondeur en coupant les branches de faible heuristique d'élagage                 \(^o^)/
 - REcoder les ouvertures, ...                                                                              -(°O°)-
 - Sélectionner les meilleures valeurs des réglages de profondeur pour optimiser le bot                     -(O_O)-
*/

/* LEGENDE :

    \(^o^)/     :   Bug réparé !!! Hourra, c'est rare !

    -(O_O)-     :   Bug en cours de réparation, il faut laisser le temps au temps...

    -(°O°)-     :   Bug qui attends depuis un moment que tu t'en occupes alors HURGE !!!

    -/O_o\-     :   Bug d'une difficulté inexprimable, qu'on préférerai cacher sous le tapis,
.                   mais qu'on affronte parce qu'on a pas le choix, et qu'on a des échéances...
*/

/* Commande de compilation :
cd ..\..\Users\windows\Documents\Personnel\Programmes\Chess\bin
gcc ..\src\plateau.c ..\src\deplacement.c -o prog -I ..\include -L ..\lib -lmingw32 -lSDL2main -lSDL2 && prog.exe
*/