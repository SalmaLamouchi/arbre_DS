
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de nœud pour les arbres
typedef struct Noeud {
    char produit[50];
    int quantite;
    struct Noeud *gauche;
    struct Noeud *droite;
} Noeud;

// Structure de pile pour les stocks
typedef struct Pile {
    char produit[50];
    int quantite;
    struct Pile *suivant;
} Pile;

// Structure de file pour les commandes
typedef struct File {
    char commande[50];
    int priorite; // Nouveau champ pour la priorité
    struct File *suivant;
} File;

// Structure pour les utilisateurs
typedef struct Utilisateur {
    char nom[50];
    int id;
    char historique_modifications[100][100]; // Tableau d'historique des modifications
    int nb_modifications; // Nombre de modifications effectuées
} Utilisateur;

// Initialisation de la pile de stock
void init_pile(Pile **top) {
    *top = NULL;
}

// Initialisation de la file de commandes
void init_file(File **debut, File **fin) {
    *debut = *fin = NULL;
}

// Initialisation de l'arbre de produits
void init_arbre(Noeud **racine) {
    *racine = NULL;
}

// Création d'un nouveau nœud pour l'arbre
Noeud* creer_noeud(char produit[], int quantite) {
    Noeud* nouveau = (Noeud*)malloc(sizeof(Noeud));
    strcpy(nouveau->produit, produit);
    nouveau->quantite = quantite;
    nouveau->gauche = nouveau->droite = NULL;
    return nouveau;
}

// Insertion dans l'arbre
Noeud* inserer_noeud(Noeud* racine, char produit[], int quantite) {
    if (racine == NULL)
        return creer_noeud(produit, quantite);
    if (strcmp(produit, racine->produit) < 0)
        racine->gauche = inserer_noeud(racine->gauche, produit, quantite);
    else if (strcmp(produit, racine->produit) > 0)
        racine->droite = inserer_noeud(racine->droite, produit, quantite);
    else
        racine->quantite += quantite; // Mise à jour de la quantité si le produit existe déjà
    return racine;
}

// Empilement d'un nouveau produit dans le stock
void empiler_produit(Pile **top, char produit[], int quantite) {
    Pile *nouveau = (Pile*)malloc(sizeof(Pile));
    strcpy(nouveau->produit, produit);
    nouveau->quantite = quantite;
    nouveau->suivant = *top;
    *top = nouveau;
}

// Dépilement d'un produit du stock
void depiler_produit(Pile **top) {
    if (*top == NULL) {
        printf("Le stock est vide.\n");
        return;
    }
    Pile *temp = *top;
    *top = (*top)->suivant;
    free(temp);
}

// Enfiler une nouvelle commande
void enfiler_commande(File **debut, File **fin, char commande[], int priorite) {
    File *nouveau = (File*)malloc(sizeof(File));
    strcpy(nouveau->commande, commande);
    nouveau->priorite = priorite; // Assignation de la priorité
    nouveau->suivant = NULL;
    if (*debut == NULL)
        *debut = *fin = nouveau;
    else {
        (*fin)->suivant = nouveau;
        *fin = nouveau;
    }
}

// Tri des commandes par priorité
void trier_commandes_par_priorite(File **debut) {
    // Tri par insertion en fonction de la priorité
    if (*debut == NULL || (*debut)->suivant == NULL)
        return;

    File *sorted = NULL;
    File *current = *debut;

    while (current != NULL) {
        File *next = current->suivant;
        if (sorted == NULL || current->priorite > sorted->priorite) {
            current->suivant = sorted;
            sorted = current;
        } else {
            File *temp = sorted;
             while (temp->suivant != NULL && temp->suivant->priorite > current->priorite) {
                temp = temp->suivant;
            }
            current->suivant = temp->suivant;
            temp->suivant = current;
        }
        current = next;
    }
    *debut = sorted;
}

// Mise à jour de la priorité d'une commande
void mettre_a_jour_priorite(File *debut, char commande[], int priorite) {
    while (debut != NULL) {
        if (strcmp(debut->commande, commande) == 0) {
            debut->priorite = priorite;
            return;
        }
        debut = debut->suivant;
    }
    printf("La commande spécifiée n'a pas été trouvée dans la file.\n");
}

// Défiler une commande
void defiler_commande(File **debut) {
    if (*debut == NULL) {
        printf("La file est vide.\n");
        return;
    }
    File *temp = *debut;
    *debut = (*debut)->suivant;
    free(temp);
}

// Supprimer une commande de la file
void supprimer_commande(File **debut, char commande[]) {
    if (*debut == NULL) {
        printf("La file est vide.\n");
        return;
    }
    File *courant = *debut;
    File *precedent = NULL;
    while (courant != NULL) {
        if (strcmp(courant->commande, commande) == 0) {
            if (precedent == NULL) {
                *debut = courant->suivant;
                free(courant);
                return;
            } else {
                precedent->suivant = courant->suivant;
                free(courant);
                return;
            }
        }
        precedent = courant;
        courant = courant->suivant;
    }
    printf("La commande spécifiée n'a pas été trouvée dans la file.\n");
}

// Recherche d'un produit dans le stock
Noeud* rechercher_produit(Noeud* racine, char produit[]) {
    if (racine == NULL || strcmp(racine->produit, produit) == 0)
        return racine;
    if (strcmp(produit, racine->produit) < 0)
        return rechercher_produit(racine->gauche, produit);
    else
        return rechercher_produit(racine->droite, produit);
}

// Fonction d'affichage des détails d'un produit
void afficher_details_produit(Noeud* produit) {
    if (produit != NULL)
        printf("Produit : %s, Quantité : %d\n", produit->produit, produit->quantite);
    else
        printf("Le produit n'a pas été trouvé dans le stock.\n");
}

// Affichage de l'arbre (infixe)
void afficher_arbre(Noeud* racine) {
    if (racine != NULL) {
        afficher_arbre(racine->gauche);
        printf("Produit : %s, Quantité : %d\n", racine->produit, racine->quantite);
        afficher_arbre(racine->droite);
    }
}

// Affichage de la pile de stock
void afficher_stock(Pile *top) {
    if (top == NULL) {
        printf("Le stock est vide.\n");
        return;
    }
    printf("Produits dans le stock :\n");
    while (top != NULL) {
        printf("Produit : %s, Quantité : %d\n", top->produit, top->quantite);
        top = top->suivant;
    }
}

// Affichage de la file de commandes
void afficher_commandes(File *debut) {
    if (debut == NULL) {
        printf("La file de commandes est vide.\n");
        return;
    }
    printf("Commandes en attente :\n");
    while (debut != NULL) {
        printf("%s (Priorite : %d)\n", debut->commande, debut->priorite);
        debut = debut->suivant;
    }
}

// Enregistrer un nouvel utilisateur
void enregistrer_utilisateur(Utilisateur *utilisateurs[], int *nb_utilisateurs, char nom[]) {
    Utilisateur *nouveau = (Utilisateur*)malloc(sizeof(Utilisateur));
    strcpy(nouveau->nom, nom);
    nouveau->id = *nb_utilisateurs + 1; // ID unique pour chaque utilisateur
    nouveau->nb_modifications = 0; // Initialisation du nombre de modifications
    utilisateurs[*nb_utilisateurs] = nouveau;
    (*nb_utilisateurs)++;
}

// Suivre les modifications effectuées par un utilisateur
void suivre_modifications(Utilisateur *utilisateur, char modification[]) {
    strcpy(utilisateur->historique_modifications[utilisateur->nb_modifications], modification);
    utilisateur->nb_modifications++;
    printf("L'utilisateur %s a effectué la modification suivante : %s\n", utilisateur->nom, modification);
}

// Afficher l'historique des modifications pour un utilisateur donné
void afficher_historique_modifications(Utilisateur *utilisateur) {
    printf("Historique des modifications de l'utilisateur %s :\n", utilisateur->nom);
    for (int i = 0; i < utilisateur->nb_modifications; i++) {
        printf("%s\n", utilisateur->historique_modifications[i]);
    }
}

int main() {
    Noeud *stock = NULL;
    Pile *stock_pile = NULL;
    File *commandes_debut = NULL, *commandes_fin = NULL;
    Utilisateur *utilisateurs[10];
    int nb_utilisateurs = 0;
    int choix, quantite;
    char produit[50], commande[50], nom_utilisateur[50];
    int priorite;
    int utilisateur_enregistre = 0; // Variable pour vérifier si un utilisateur est enregistré

    printf("--------------------------------Bienvenue dans le systeme de gestion de stock du magasin--------------------------------.\n");
    do {
        printf("\n1. Ajouter un produit au stock\n");
        printf("2. Enregistrer une commande\n");
        //printf("3. Preparer une commande\n");
        printf("4. Afficher le stock\n");
        printf("5. Afficher les commandes\n");
        printf("6. Supprimer une commande\n");
        printf("7. Rechercher un produit dans le stock\n");
        printf("8. Enregistrer un nouvel utilisateur\n");
        printf("9. Mettre a jour la priorite d'une commande\n");
        printf("10. Voir l'historique des modifications\n");
        printf("0. Quitter\n");

        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                printf("Entrez le nom du produit : ");
                scanf("%s", produit);
                printf("Entrez la quantite : ");
                scanf("%d", &quantite);
                stock = inserer_noeud(stock, produit, quantite);
                empiler_produit(&stock_pile, produit, quantite);
                suivre_modifications(utilisateurs[0], "Ajout de produit au stock"); // Suivi de la modification
                break;
            case 2:
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                printf("Entrez la commande à enregistrer : ");
                scanf("%s", commande);
                printf("Entrez la priorite de la commande : ");
                scanf("%d", &priorite);
                enfiler_commande(&commandes_debut, &commandes_fin, commande, priorite);
                trier_commandes_par_priorite(&commandes_debut); // Tri des commandes par priorité
                suivre_modifications(utilisateurs[0], "Enregistrement d'une nouvelle commande"); // Suivi de la modification
                break;
            case 3:
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                if (stock_pile == NULL) {
                    printf("Le stock est vide, impossible de preparer une commande.\n");
                } else {
                    printf("La commande \"%s\" est en cours de preparation.\n", stock_pile->produit);
                    depiler_produit(&stock_pile);
                }
                break;
            case 4:
                printf("----- Stock -----\n");
                afficher_arbre(stock);
                printf("-----------------\n");
                break;
            case 5:
                printf("----- Commandes -----\n");
                afficher_commandes(commandes_debut);
                printf("---------------------\n");
                break;
            case 6:
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                printf("Entrez la commande à supprimer : ");
                scanf("%s", commande);
                supprimer_commande(&commandes_debut, commande);
                suivre_modifications(utilisateurs[0], "Suppression de commande"); // Suivi de la modification
                break;
            case 7:
                printf("Entrez le nom du produit à rechercher : ");
                scanf("%s", produit);
                Noeud* produit_trouve = rechercher_produit(stock, produit);
                afficher_details_produit(produit_trouve);
                break;
            case 8:
                printf("Entrez le nom de l'utilisateur a enregistrer : ");
                scanf("%s", nom_utilisateur);
                enregistrer_utilisateur(utilisateurs, &nb_utilisateurs, nom_utilisateur);
                utilisateur_enregistre = 1; // Mettre à jour le statut de l'utilisateur enregistré
                break;
            case 9: // mise à jour de la priorité d'une commande
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                printf("Entrez la commande à mettre à jour : ");
                scanf("%s", commande);
                printf("Entrez la nouvelle priorite : ");
                scanf("%d", &priorite);
                mettre_a_jour_priorite(commandes_debut, commande, priorite);
                suivre_modifications(utilisateurs[0], "Mise a jour de la priorite d'une commande"); // Suivi de la modification
                break;
            case 10: // Nouveau cas pour voir l'historique des modifications
                if (!utilisateur_enregistre) {
                    printf("Veuillez d'abord enregistrer un utilisateur.\n");
                    break;
                }
                // Affichage de l'historique des modifications pour l'utilisateur 1
                afficher_historique_modifications(utilisateurs[0]);
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
