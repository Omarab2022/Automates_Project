#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_TAILLE_MOT 50


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  structure de donnes    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

struct automateinformation {
    int nb_etats;
    char *etats;
    int nb_etats_initiaux;
    char *etats_initiaux;
    int nb_etats_finaux;
    char *etats_finaux;
    int nb_transitions;
    struct transition *transitions;
    
};

struct transition {
    char etat_depart;
    char etat_arrivee;
    char etiquette;
};


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   lire l'automate d'apres fichier .txt     \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


struct automateinformation lire_automate(char *nom_fichier) {
    struct automateinformation automate;

    FILE *fichier = fopen(nom_fichier, "r");


    if (fichier == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(1);
    }

    char ligne[256];

    // Lecture du nombre d'états
    fgets(ligne, sizeof(ligne), fichier);
    automate.nb_etats = atoi(ligne);

    // Lecture des états
    automate.etats = (char*) malloc(automate.nb_etats * sizeof(char));
    fgets(ligne, sizeof(ligne), fichier);
    char *token = strtok(ligne, " ");
    int i = 0;
    while (token != NULL) {
        automate.etats[i] = *token;
        token = strtok(NULL, " ");
        i++;
    }

    // Lecture du nombre de transitions
    fgets(ligne, sizeof(ligne), fichier);
    automate.nb_transitions = atoi(ligne);

    // Lecture des transitions
     automate.transitions = (struct transition*) malloc(automate.nb_transitions * sizeof(struct transition));
    for (i = 0; i < automate.nb_transitions; i++) {
        fgets(ligne, sizeof(ligne), fichier);
        automate.transitions[i].etat_depart = ligne[0];
        automate.transitions[i].etat_arrivee = ligne[2];
        automate.transitions[i].etiquette = ligne[4];
    }

     // Lecture du nombre d'états initiaux
    fgets(ligne, sizeof(ligne), fichier);
    automate.nb_etats_initiaux = atoi(ligne);

    // Lecture du nombre d'états finaux
    fgets(ligne, sizeof(ligne), fichier);
    automate.nb_etats_finaux = atoi(ligne);

    // Lecture des états initiaux
    automate.etats_initiaux = (char*) malloc(automate.nb_etats_initiaux * sizeof(char));
    fgets(ligne, sizeof(ligne), fichier);
    token = strtok(ligne, " ");
    i = 0;
    while (token != NULL) {
        automate.etats_initiaux[i] = *token;
        token = strtok(NULL, " ");
        i++;
    }
 // Lecture des états finaux
    automate.etats_finaux = (char*) malloc(automate.nb_etats_finaux * sizeof(char));
    fgets(ligne, sizeof(ligne), fichier);
    token = strtok(ligne, " ");
    i = 0;
    while (token != NULL) {
        automate.etats_finaux[i] = *token;
        token = strtok(NULL, " ");
        i++;
    }

    fclose(fichier);

    return automate;
}



/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\generer fichier .dot\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

void generer_fichier_dot(struct automateinformation automate, char*nom_fichier) {


    FILE* fichier_dot = fopen("automate.dot", "w");

    fprintf(fichier_dot, "digraph {\n");

    // Ajout des états
    for (int i = 0; i < automate.nb_etats; i++) {
        fprintf(fichier_dot, "    %c", automate.etats[i]);

        // Couleur pour les états initiaux
        for (int j = 0; j < automate.nb_etats_initiaux; j++) {
            if (automate.etats[i] == automate.etats_initiaux[j]) {
                fprintf(fichier_dot, " [style=filled, color=green]");
                break;
            }
        }

        // Entourer les états finaux avec deux cercles rouges
        for (int j = 0; j < automate.nb_etats_finaux; j++) {
            if (automate.etats[i] == automate.etats_finaux[j]) {
                fprintf(fichier_dot, " [peripheries=2, color=red]");
                break;
            }
        }

        fprintf(fichier_dot, "\n");
    }

    // Ajout des transitions
    for (int i = 0; i < automate.nb_transitions; i++) {
        fprintf(fichier_dot, "    %c -> %c [label=\"%c\"];\n", automate.transitions[i].etat_depart, automate.transitions[i].etat_arrivee, automate.transitions[i].etiquette);
    }

    fprintf(fichier_dot, "}");

    fclose(fichier_dot);

    // Affichage sur l'écran
   
     system("dot -Tpng automate.dot -o automate.png");
     system("open automate.png");
}


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   tester si un mot est un mot est engendre   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

int est_etat_final(struct automateinformation automate, int etat) {
    int i;
    for (i = 0; i < automate.nb_etats_finaux; i++) {
        if (automate.etats_finaux[i] == etat) {
            return 1;
        }
    }
    return 0;
}

int est_etat_initial(struct automateinformation automate, int etat) {
    int i;
    for (i = 0; i < automate.nb_etats_initiaux; i++) {
        if (automate.etats_initiaux[i] == etat) {
            return 1;
        }
    }
    return 0;
}

int tester_mot(struct automateinformation automate, char* mot) {
    int i, j, k;
    int longueur_mot = strlen(mot);
    
    if (longueur_mot == 0) {
       
        for (i = 0; i < automate.nb_etats_initiaux; i++) {
            if (est_etat_initial(automate,automate.etats_initiaux[i])) {
                if (est_etat_final(automate, automate.etats_initiaux[i])) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
        return 0;
    }

    
    for (i = 0; i < automate.nb_etats_initiaux; i++) {

        int etat_actuel = automate.etats_initiaux[i];

          for (j = 0; j < strlen(mot); j++) {
       

         int transition_trouvee = 0;
        for (k = 0; k < automate.nb_transitions; k++) {
            if (automate.transitions[k].etat_depart == etat_actuel && automate.transitions[k].etiquette == mot[j]) {
                etat_actuel = automate.transitions[k].etat_arrivee;
                  transition_trouvee = 1;
                break;
            }
        }
            if (!transition_trouvee) {
               
                break;
            }
        }

        
        if (est_etat_final(automate, etat_actuel)) {
            return 1;
        }
    }
    return 0;
    
}


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\                  afficher les mots engendres              \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/    

void afficher_mots_engendres(struct automateinformation automate) {
    FILE* fichier = fopen("mots.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;
    }

    char mot[MAX_TAILLE_MOT];
    
    while (fgets(mot, MAX_TAILLE_MOT, fichier)) {
        // Supprime le caractère de fin de ligne ('\n') du mot
        mot[strcspn(mot, "\n")] = '\0';
        if (tester_mot(automate, mot)) {
            printf("%s\n", mot);
        }
    }

    fclose(fichier);
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\               union-deux-automate                \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


void union_automates(char* fichier1, char* fichier2) {

    struct automateinformation automate1, automate2, automate_union;
    automate1 = lire_automate(fichier1);
    automate2 = lire_automate(fichier2);


    // Ajout de l'état initial S et final P

    automate_union.nb_etats = automate1.nb_etats + automate2.nb_etats + 2;
    automate_union.etats = malloc(sizeof(char) * automate_union.nb_etats);
    strcpy(automate_union.etats, "S");
    strcat(automate_union.etats, automate1.etats);
    strcat(automate_union.etats, automate2.etats);
    strcat(automate_union.etats, "P");

    // Ajout des états initiaux et finaux

    automate_union.nb_etats_initiaux = 1;
    automate_union.etats_initiaux = malloc(sizeof(char));
    automate_union.etats_initiaux[0] = 'S';
    automate_union.nb_etats_finaux = 1;
    automate_union.etats_finaux = malloc(sizeof(char));
    automate_union.etats_finaux[0] = 'P';

    // Ajout des transitions
    automate_union.nb_transitions = automate1.nb_transitions + automate2.nb_transitions + (automate1.nb_etats_initiaux+automate1.nb_etats_finaux+automate2.nb_etats_initiaux+automate2.nb_etats_finaux);
    automate_union.transitions = malloc(sizeof(struct transition) * automate_union.nb_transitions);

 
    // Transitions depuis S

    int index=0;
    for (int i = 0; i < automate1.nb_etats_initiaux; i++) {
    automate_union.transitions[i].etat_depart = 'S';
    automate_union.transitions[i].etat_arrivee = automate1.etats_initiaux[i];
    automate_union.transitions[i].etiquette = '$';
    index++;

    }

    for (int i = 0; i < automate2.nb_etats_initiaux; i++) {
    automate_union.transitions[index].etat_depart = 'S';
    automate_union.transitions[index].etat_arrivee = automate2.etats_initiaux[i];
    automate_union.transitions[index].etiquette = ' $';
    index++;
    }
    // Transitions vers P

    for (int i = 0; i < automate1.nb_etats_finaux; i++) {
    automate_union.transitions[index].etat_depart = automate1.etats_finaux[i];
    automate_union.transitions[index].etat_arrivee = 'P';
    automate_union.transitions[index].etiquette = '$ ';
  index++;
 }
    for (int i = 0; i < automate2.nb_etats_finaux; i++) {
    automate_union.transitions[index].etat_depart = automate2.etats_finaux[i];
    automate_union.transitions[index].etat_arrivee = 'P';
    automate_union.transitions[index].etiquette = ' $';
  index++;
}

// Transitions de l'automate1

for (int i = 0; i < automate1.nb_transitions; i++) {
automate_union.transitions[index].etat_depart = automate1.transitions[i].etat_depart;
automate_union.transitions[index].etat_arrivee = automate1.transitions[i].etat_arrivee;
automate_union.transitions[index].etiquette = automate1.transitions[i].etiquette;
index++;
}

// Transitions de l'automate2

for (int i = 0; i < automate2.nb_transitions; i++) {
automate_union.transitions[index].etat_depart = automate2.transitions[i].etat_depart;
automate_union.transitions[index].etat_arrivee = automate2.transitions[i].etat_arrivee;
automate_union.transitions[index].etiquette = automate2.transitions[i].etiquette;
index++;

} 

    printf("Informations de l'automate résultant :\n");
    printf("Nombre d'états : %d\n", automate_union.nb_etats);
    printf("États : %s\n", automate_union.etats);
    printf("Nombre d'états initiaux : %d\n", automate_union.nb_etats_initiaux);
    printf("États initiaux : %s\n", automate_union.etats_initiaux);
    printf("Nombre d'états finaux : %d\n", automate_union.nb_etats_finaux);
    printf("États finaux : %s\n", automate_union.etats_finaux);
    printf("Nombre de transitions : %d\n", automate_union.nb_transitions);
    printf("Transitions :\n");
    for (int i = 0; i < automate_union.nb_transitions; i++) {
        printf("%c -%c-> %c\n", automate_union.transitions[i].etat_depart,
                                         automate_union.transitions[i].etiquette, 
                                              automate_union.transitions[i].etat_arrivee);

    } 

 FILE *fichier = fopen("union.txt", "w");

    fprintf(fichier, "%d\n", automate_union.nb_etats);

    for (int i = 0; i < automate_union.nb_etats; i++) {
        fprintf(fichier, "%c ",automate_union.etats[i]);
    }
    fprintf(fichier, "\n");

    fprintf(fichier, "%d\n", automate_union.nb_transitions);

    for (int i =automate_union.nb_transitions - 1; i >= 0; i--) {
    fprintf(fichier, "%c %c %c\n", automate_union.transitions[i].etat_depart, automate_union.transitions[i].etiquette, automate_union.transitions[i].etat_arrivee);
    }

   fprintf(fichier, "%d\n", automate_union.nb_etats_initiaux); 

   for (int i = 0; i < automate_union.nb_etats_initiaux; i++) {
    fprintf(fichier, "%c ", automate_union.etats_initiaux[i]);
   }
   fprintf(fichier, "\n");
   fprintf(fichier, "%d\n", automate_union.nb_etats_finaux);

   for (int i = 0; i < automate_union.nb_etats_finaux; i++) {
    fprintf(fichier, "%c ", automate_union.etats_finaux[i]);
   }
   fprintf(fichier, "\n");

   fclose(fichier);
   printf("L'automate a été écrit dans le fichier union.txt avec succès.\n");


        generer_fichier_dot(automate_union, "output.dot");
        printf("Le fichier UNION a été généré.\n");

        // Générer l'image à partir du fichier .dot
        system("dot -Tpng output.dot -o output.png");
        printf("Le fichier UNION.png a été généré.\n");
   
    
}
int etat_present(int* etats, int nb_etats, int etat) {
    for (int i = 0; i < nb_etats; i++) {
        if (etats[i] == etat) {
            return 1;
        }
    }
    return 0;
}

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   intersection  ou produit   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


void intersection_automates(char* fichier1, char* fichier2) {
    struct automateinformation automate1, automate2, automate_intersection;
    automate1 = lire_automate(fichier1);
    automate2 = lire_automate(fichier2);

    // Création de l'automate d'intersection
    automate_intersection.nb_etats = 0;
    automate_intersection.etats = malloc(sizeof(char) * automate1.nb_etats);
    automate_intersection.nb_etats_initiaux = 0;
    automate_intersection.etats_initiaux = malloc(sizeof(char));
    automate_intersection.nb_etats_finaux = 0;
    automate_intersection.etats_finaux = malloc(sizeof(char));
    automate_intersection.nb_transitions = 0;
    automate_intersection.transitions = malloc(sizeof(struct transition));



    // Parcours des transitions des deux automates pour trouver les transitions communes
    for (int i = 0; i < automate1.nb_transitions; i++) {
        for (int j = 0; j < automate2.nb_transitions; j++) {
            if (automate1.transitions[i].etiquette == automate2.transitions[j].etiquette) {
                // Ajout de la transition commune dans l'automate d'intersection
                struct transition t;
                t.etat_depart = automate1.transitions[i].etat_depart;
                t.etiquette = automate1.transitions[i].etiquette;
                t.etat_arrivee = automate1.transitions[i].etat_arrivee;
                automate_intersection.nb_transitions++;
                automate_intersection.transitions = realloc(automate_intersection.transitions, sizeof(struct transition) * automate_intersection.nb_transitions);
                automate_intersection.transitions[automate_intersection.nb_transitions - 1] = t;

                // Ajout des états concernés dans l'automate d'intersection
                if (!etat_present(automate_intersection.etats, automate_intersection.nb_etats, automate1.transitions[i].etat_depart)) {
                    automate_intersection.nb_etats++;
                    automate_intersection.etats = realloc(automate_intersection.etats, sizeof(char) * automate_intersection.nb_etats);
                    automate_intersection.etats[automate_intersection.nb_etats - 1] = automate1.transitions[i].etat_depart;
                }
                if (!etat_present(automate_intersection.etats, automate_intersection.nb_etats, automate1.transitions[i].etat_arrivee)) {
                    automate_intersection.nb_etats++;
                    automate_intersection.etats = realloc(automate_intersection.etats, sizeof(char) * automate_intersection.nb_etats);
                    automate_intersection.etats[automate_intersection.nb_etats - 1] = automate1.transitions[i].etat_arrivee;
                }
            }
        }
    }

   
    automate_intersection.nb_etats_initiaux = 1;
    automate_intersection.etats_initiaux[0] = automate1.etats_initiaux[0];

    automate_intersection.nb_etats_finaux = 0;
    for (int i = 0; i < automate_intersection.nb_etats; i++) {
        if (etat_present(automate1.etats_finaux, automate1.nb_etats_finaux, automate_intersection.etats[i]) && etat_present(automate2.etats_finaux, automate2.nb_etats_finaux, automate_intersection.etats[i])) {
            automate_intersection.nb_etats_finaux= automate_intersection.nb_etats_finaux + 1;
automate_intersection.etats_finaux[automate_intersection.nb_etats_finaux-1] = automate_intersection.etats[i];
}
}


    printf("\n");
    printf("//////////////  AUTOMATE D'INTERSECTION  ///////////////\n");
    printf("\n");
    printf("Nombre d'états : %d\n", automate_intersection.nb_etats);
    printf("Etats : %s\n", automate_intersection.etats);
    printf("Nombre de transitions : %d\n", automate_intersection.nb_transitions);
    for (int i = 0; i < automate_intersection.nb_transitions; i++) {
        printf("Transition %d : %c -%c-> %c\n", i+1, automate_intersection.transitions[i].etat_depart, automate_intersection.transitions[i].etiquette, automate_intersection.transitions[i].etat_arrivee);
    }
 
    printf("\n");
     FILE *fichier = fopen("produit.txt", "w");

    fprintf(fichier, "%d\n", automate_intersection.nb_etats);

    for (int i = 0; i < automate_intersection.nb_etats; i++) {
        fprintf(fichier, "%c ",automate_intersection.etats[i]);
    }
    fprintf(fichier, "\n");

    fprintf(fichier, "%d\n", automate_intersection.nb_transitions);

    for (int i =automate_intersection.nb_transitions - 1; i >= 0; i--) {
    fprintf(fichier, "%c %c %c\n", automate_intersection.transitions[i].etat_depart, automate_intersection.transitions[i].etiquette, automate_intersection.transitions[i].etat_arrivee);
    }

   
   fprintf(fichier, "\n");

   fclose(fichier);
   printf("L'automate a été écrit dans le fichier produit.txt avec succès.\n");


        generer_fichier_dot(automate_intersection, "output.dot");
        printf("Le fichier output.dot a été généré.\n");

        // Générer l'image à partir du fichier .dot
        system("dot -Tpng output.dot -o output.png");
        printf("Le fichier output.png a été généré.\n");
   
 }
        

///////////////////////////////////////////////////  AUTOMATE - ETOILE    ////////////////////////////////////////////////////////////////////


    void etoile_automate(char *nom_fichier) {
    
    struct automateinformation automate = lire_automate(nom_fichier);

    // Ajout d'un nouvel état initial qui est également un état final
    automate.nb_etats++;
    automate.etats = realloc(automate.etats, sizeof(char) * automate.nb_etats);
    automate.etats[automate.nb_etats - 1] = 'q';

    automate.nb_etats_initiaux++;
    automate.etats_initiaux = realloc(automate.etats_initiaux, sizeof(char) * automate.nb_etats_initiaux);
    automate.etats_initiaux[automate.nb_etats_initiaux - 1] = 'q';

    automate.nb_etats_finaux++;
    automate.etats_finaux = realloc(automate.etats_finaux, sizeof(char) * automate.nb_etats_finaux);
    automate.etats_finaux[automate.nb_etats_finaux - 1] = 'q';

    // Ajout de nouvelles transitions

    automate.nb_transitions += 4;
    automate.transitions = realloc(automate.transitions, sizeof(struct transition) * automate.nb_transitions);

    automate.transitions[automate.nb_transitions - 4].etat_depart = 'q';
    automate.transitions[automate.nb_transitions - 4].etat_arrivee = automate.etats_initiaux[0];
    automate.transitions[automate.nb_transitions - 4].etiquette = '$';

    automate.transitions[automate.nb_transitions - 3].etat_depart = automate.etats_finaux[0];
    automate.transitions[automate.nb_transitions - 3].etat_arrivee = 'q';
    automate.transitions[automate.nb_transitions - 3].etiquette = '$';

    automate.transitions[automate.nb_transitions - 2].etat_depart = automate.etats_finaux[0];
    automate.transitions[automate.nb_transitions - 2].etat_arrivee = automate.etats_initiaux[0];
    automate.transitions[automate.nb_transitions - 2].etiquette = '$';

    automate.transitions[automate.nb_transitions - 1].etat_depart = automate.etats_initiaux[0];
    automate.transitions[automate.nb_transitions - 1].etat_arrivee = automate.etats_finaux[0];
    automate.transitions[automate.nb_transitions - 1].etiquette = '$';

    
    FILE *fichier = fopen("etoile.txt", "w");

    fprintf(fichier, "%d\n", automate.nb_etats);

    for (int i = 0; i < automate.nb_etats; i++) {
        fprintf(fichier, "%c ", automate.etats[i]);
    }
    fprintf(fichier, "\n");

    fprintf(fichier, "%d\n", automate.nb_transitions);

    for (int i =automate.nb_transitions - 1; i >= 0; i--) {
    fprintf(fichier, "%c %c %c\n", automate.transitions[i].etat_depart, automate.transitions[i].etiquette, automate.transitions[i].etat_arrivee);
    }

   fprintf(fichier, "%d\n", automate.nb_etats_initiaux); 

   for (int i = 0; i < automate.nb_etats_initiaux; i++) {
    fprintf(fichier, "%c ", automate.etats_initiaux[i]);
   }
   fprintf(fichier, "\n");
   fprintf(fichier, "%d\n", automate.nb_etats_finaux);

   for (int i = 0; i < automate.nb_etats_finaux; i++) {
    fprintf(fichier, "%c ", automate.etats_finaux[i]);
   }
   fprintf(fichier, "\n");

   fclose(fichier);
   printf("L'automate a été écrit dans le fichier etoile.txt avec succès.\n");


        generer_fichier_dot(automate, "output.dot");
        printf("Le fichier output.dot a été généré.\n");

        // Générer l'image à partir du fichier .dot
        system("dot -Tpng output.dot -o output.png");
        printf("Le fichier output.png a été généré.\n");
   
   }

   ////////////////////////////////////////////////////        suppression epsilon      ////////////////////////////////////////////////////////////////////


struct automateinformation supprimer_epsilon_transition(char *nom_fichier) {

    struct automateinformation automate = lire_automate(nom_fichier);

    // Recherche de toutes les epsilon-transition et stockage dans une liste

    int nb_epsilon_transitions = 0;
    struct transition *epsilon_transitions = NULL;

    for (int i = 0; i < automate.nb_transitions; i++) {
        if (automate.transitions[i].etiquette == '$') {
            nb_epsilon_transitions++;
            epsilon_transitions = (struct transition*) realloc(epsilon_transitions, nb_epsilon_transitions * sizeof(struct transition));
            epsilon_transitions[nb_epsilon_transitions - 1].etat_depart = automate.transitions[i].etat_depart;
            epsilon_transitions[nb_epsilon_transitions - 1].etat_arrivee = automate.transitions[i].etat_arrivee;
            epsilon_transitions[nb_epsilon_transitions - 1].etiquette = automate.transitions[i].etiquette;
        }
    }

 
    
    for (int i = 0; i < nb_epsilon_transitions; i++) {

        // Recherche de toutes les transitions partant de l'état d'arrivée de l'epsilon-transition

        int nb_transitions = 0;
        struct transition *transitions = NULL;
        for (int j = 0; j < automate.nb_transitions; j++) {
            if (automate.transitions[j].etat_depart == epsilon_transitions[i].etat_arrivee) {
                nb_transitions++;
                transitions = (struct transition*) realloc(transitions, nb_transitions * sizeof(struct transition));
                transitions[nb_transitions - 1].etat_depart = automate.transitions[j].etat_depart;
                transitions[nb_transitions - 1].etat_arrivee = automate.transitions[j].etat_arrivee;
                transitions[nb_transitions - 1].etiquette = automate.transitions[j].etiquette;
            }
        }

        // Ajout de nouvelles transitions pour chaque transition trouvée
        for (int j = 0; j < nb_transitions; j++) {
            struct transition nouvelle_transition;
            nouvelle_transition.etat_depart = epsilon_transitions[i].etat_depart;
            nouvelle_transition.etat_arrivee = transitions[j].etat_arrivee;
            nouvelle_transition.etiquette = transitions[j].etiquette;
            automate.nb_transitions++;
            automate.transitions = (struct transition*) realloc(automate.transitions, automate.nb_transitions * sizeof(struct transition));
            automate.transitions[automate.nb_transitions - 1] = nouvelle_transition;
        }
    }

    // Suppression de toutes les epsilon-transitions de l'automate

    int i = 0;
    while (i < automate.nb_transitions) {
        if (automate.transitions[i].etiquette == '$') {
            for (int j = i; j < automate.nb_transitions - 1; j++) {
                automate.transitions[j] = automate.transitions[j + 1];
            }
            automate.nb_transitions--;
            automate.transitions = (struct transition*) realloc(automate.transitions, automate.nb_transitions * sizeof(struct transition));
        } else {
            i++;
        }
    }

    // Ajout des états finaux sans transition sortante
    for (int i = 0; i < automate.nb_etats; i++) {
        int a_des_transitions_sortantes = 0;
        for (int j = 0; j < automate.nb_transitions; j++) {
            if (automate.transitions[j].etat_depart == automate.etats[i] && automate.transitions[j].etiquette != '$') {
                a_des_transitions_sortantes = 1;
                break;
            }
        }
        if (!a_des_transitions_sortantes) {
            automate.nb_etats_finaux++;
            automate.etats_finaux = (int*) realloc(automate.etats_finaux, automate.nb_etats_finaux * sizeof(int));
            automate.etats_finaux[automate.nb_etats_finaux - 1] = automate.etats[i];
        }
    }

    // Ajout des états initiaux sans transition entrante
for (int i = 0; i < automate.nb_etats; i++) {
    int a_des_transitions_entrantes = 0;
    for (int j = 0; j < automate.nb_transitions; j++) {
        if (automate.transitions[j].etat_arrivee == automate.etats[i] && automate.transitions[j].etiquette != '$') {
            a_des_transitions_entrantes = 1;
            break;
        }
    }
    if (!a_des_transitions_entrantes) {
        automate.nb_etats_initiaux++;
        automate.etats_initiaux = (int*) realloc(automate.etats_initiaux, automate.nb_etats_initiaux * sizeof(int));
        automate.etats_initiaux[automate.nb_etats_initiaux - 1] = automate.etats[i];
    }
}

    printf("Informations de l'automate résultant :\n");
    printf("Nombre d'états : %d\n", automate.nb_etats);
    printf("États : %s\n", automate.etats);
    printf("Nombre d'états initiaux : %d\n", automate.nb_etats_initiaux);
    printf("États initiaux : %s\n", automate.etats_initiaux);
    printf("Nombre d'états finaux : %d\n", automate.nb_etats_finaux);
    printf("États finaux : %s\n", automate.etats_finaux);
    printf("Nombre de transitions : %d\n", automate.nb_transitions);
    printf("Transitions :\n");
    for (int i = 0; i < automate.nb_transitions; i++) {
        printf("%c -%c-> %c\n", automate.transitions[i].etat_depart,
                                         automate.transitions[i].etiquette, 
                                              automate.transitions[i].etat_arrivee);

    } 
 FILE *fichier = fopen("supression.txt", "w");

    fprintf(fichier, "%d\n", automate.nb_etats);

    for (int i = 0; i < automate.nb_etats; i++) {
        fprintf(fichier, "%c ", automate.etats[i]);
    }
    fprintf(fichier, "\n");

    fprintf(fichier, "%d\n", automate.nb_transitions);

    for (int i =automate.nb_transitions - 1; i >= 0; i--) {
    fprintf(fichier, "%c %c %c\n", automate.transitions[i].etat_depart, automate.transitions[i].etiquette, automate.transitions[i].etat_arrivee);
    }

   fprintf(fichier, "%d\n", automate.nb_etats_initiaux); 

   for (int i = 0; i < automate.nb_etats_initiaux; i++) {
    fprintf(fichier, "%c ", automate.etats_initiaux[i]);
   }
   fprintf(fichier, "\n");
   fprintf(fichier, "%d\n", automate.nb_etats_finaux);

   for (int i = 0; i < automate.nb_etats_finaux; i++) {
    fprintf(fichier, "%c ", automate.etats_finaux[i]);
   }
   fprintf(fichier, "\n");

   fclose(fichier);
   printf("L'automate a été écrit dans le fichier suppression.txt avec succès.\n");


        generer_fichier_dot(automate, "output.dot");
        printf("Le fichier output.dot a été généré.\n");

        // Générer l'image à partir du fichier .dot
        system("dot -Tpng output.dot -o output.png");
        printf("Le fichier output.png a été généré.\n");
   
    
}

   


////////////////////////////////////////////////////////////////////           MAIN         ////////////////////////////////////////////////////////////////////

int main() {

    struct automateinformation automate = lire_automate("automate2.txt");
   

    int choice;

    printf("Veuillez choisir une option :\n");
    printf("1 : exécuter partie 1\n");
    printf("2 : exécuter partie 2\n");
    printf("3 : union\n");
    printf("4 : produit ou intersection\n");
    printf("5 : étoile\n");
     printf("6: suppression etoile\n");
    scanf("%d", &choice);


    if (choice == 1) {

            // Affichage des informations de l'automate 
    printf("\n");
    printf("///////////////  PARTIE 1 ////////////////////\n");
    printf("\n");
    printf("Nombre d'états : %d\n", automate.nb_etats);
    printf("Etats : %s\n", automate.etats);
    printf("Nombre de transitions : %d\n", automate.nb_transitions);
    for (int i = 0; i < automate.nb_transitions; i++) {
        printf("Transition %d : %c -%c-> %c\n", i+1, automate.transitions[i].etat_depart, automate.transitions[i].etiquette, automate.transitions[i].etat_arrivee);
    }
    printf("Nombre d'états initiaux : %d\n", automate.nb_etats_initiaux);
    printf("Nombre d'états finaux : %d\n", automate.nb_etats_finaux);
    printf("Etats initiaux : %s\n", automate.etats_initiaux);
    printf("Etats finaux : %s\n", automate.etats_finaux);
    printf("\n");

   
     /*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   fichier .dot     \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   // Générer le fichier .dot
        generer_fichier_dot(automate, "output.dot");
        printf("Le fichier output.dot a été généré.\n");

        // Générer l'image à partir du fichier .dot
        system("dot -Tpng output.dot -o output.png");
        printf("Le fichier output.png a été généré.\n");
   
       
    }
    else if (choice == 2) {
           /*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      tester si un mot est engendre       \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

   printf("////////////   PARTIE 2   ////////////////// \n");
   printf("\n");


    char mot[100];
    printf("Entrez un mot : ");
    scanf("%s", mot);

    if (tester_mot(automate, mot)) {
        printf("Le mot %s est engendré par l'automate.\n", mot);
    } else {
        printf("Le mot %s n'est pas engendré par l'automate.\n", mot);
    }

       /*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\    la liste des mots engendre      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
    printf("\n");
    printf("les mots qui sont engendre par votre automate sont : \n");
    printf("\n");

    afficher_mots_engendres(automate);
   
    }
    else if (choice == 3){
      
    char fichier1[] = "mon_automate.txt";
    char fichier2[] = "automatepourunion.txt";

    union_automates(fichier1, fichier2);
    
    }
    else if (choice == 4){

    char fichier1[] = "mon_automate.txt";
    char fichier2[] = "automate2.txt";

     intersection_automates(fichier1, fichier2);
    
   
    }

   else if (choice == 5){

    char fichier1[] = "mon_automate.txt";
    etoile_automate(fichier1);
  
    }

    else if (choice == 6){

   
      char fichier1[] = "automateasupprimer.txt";
      supprimer_epsilon_transition(fichier1);
    }



    return 0;
}

