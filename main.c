#include "tspstat.h"
//#include "bruteforce.h"

#define NBCHARMAX 75

FILE *logfp;
FILE *resfile;

//fonctions definies dans tspstat.h :
char *concat_string(char *s1,char *s2)
{
     char *s3=NULL;
     s3=(char *)malloc((strlen(s1)+strlen(s2))*sizeof(char));
     strcpy(s3,s1);
     strcat(s3,s2);
     return s3;
}

void affiche_tab_int_ecran(int *tab,int dim){
  printf("[");
  for (int i=0;i<dim-1;i++)
    printf("%d,",tab[i]);
  printf("%d]\n",tab[dim-1]);
}

void print_solution_fichier(char * methode,instance_t instance,tour_t t,double longueur,double tps,FILE *resfile)
{
    fprintf(resfile,"%s ;   %lf ;   %lf ; ",methode,longueur,tps);
    affiche_tab_int(instance.tabTour,instance.dimension,resfile);
    fprintf(resfile,"\n");
}

void print_solution(char * methode,instance_t instance,tour_t t,double longueur,double tps)
{
    printf("%s ;   %lf ;   %lf ; ",methode,longueur,tps);
    affiche_tab_int(instance.tabTour,instance.dimension,resfile);
    fprintf(resfile,"\n");
}

int ecriture_resultats_1(instance_t inst,char *chemin)
{
    char premierMot[NBCHARMAX];
    char ligne[NBCHARMAX];
    char *type = "";

    FILE* fichier = fopen(chemin, "r");
    if(fichier != NULL) //si l'ouverture du fichier se passe correctement
    {
        printf("J'ouvre le fichier : %s\n",chemin);
        int count = 0;//conteur de lignes
        //on lit tout le fichier ligne par ligne
        while(fgets(ligne, NBCHARMAX, fichier) != NULL)
        {
            int i =0;
            while ( ligne[i] != " " && ligne[i] != '\n' )
            {
                premierMot[i] = ligne[i];
                i++;
            }
            premierMot[i] = '\0';
            if (strcmp(premierMot,"COMMENT"))
            {
                printf("Commentaire : ");
                int depart = 10;
                while (ligne[depart] != '\n' && depart-10 <= NBCHARMAX)
                {
                    printf("%c",ligne[depart]);
                    depart++;
                }
                printf("\nEOF\n");
            }
            if (strcmp(premierMot,"TYPE"))
            {
                int depart = 7;
                while (ligne[depart] != '\n' && depart-7 <= NBCHARMAX)
                {
                    type[depart-7] = ligne[depart];
                    depart++;
                }
                type[depart] = '\0';
            }
            count++;
        }
        fclose(fichier);
        printf("%d lignes lues\n\n*** Instance ***\nnom de l'instance ; %s\nNb villes (avec (0,0)) ; %d\nType %s\nPoint ; Abs ; Ord\n",count,inst.name,inst.dimension+1,type);
        printf("   0 ;      0;     0\n");
        for ( int i = 1; i < inst.dimension + 1; i++)
        {
            if ( i < 10 )
            {
                printf("   %d",i);
            }
            else
            {
                printf("  %d",i);
            }
            if (inst.tabCoord[0][i]>999)
            {
                printf("   ");
            }
            else if(inst.tabCoord[0][i]>99)
            {
                printf("    ");
            }
            else if(inst.tabCoord[0][i]>9)
            {
                printf("     ");
            }
            else
            {
                printf("      ");
            }
            printf("%d;",inst.tabCoord[0][i]);
            if (inst.tabCoord[1][i]>999)
            {
                printf("  ");
            }
            else if(inst.tabCoord[1][i]>99)
            {
                printf("   ");
            }
            else if(inst.tabCoord[1][i]>9)
            {
                printf("    ");
            }
            else
            {
                printf("     ");
            }
            printf("%d\n",inst.tabCoord[0][i]);
        }
        printf("\nMethode ; longueur ; Temps CPU (sec) ; Tour\n");

    }
    else
    {
        erreur("pas de fichier");
        fclose(fichier);
        return 0;
    }
    return 1;
}

/* Lit le fichier donne par @chemin et stocke l'instance de probleme dans @probleme, renvoie 0 si il y a un probleme, 1 sinon */
int lecture_fichier(char* chemin, instance_t* problemeTsp)
{

    char premierMot[NBCHARMAX];
    char ligne[NBCHARMAX];

    FILE* fichier = fopen(chemin, "r");
    if(fichier != NULL) //si l'ouverture du fichier se passe correctement
    {
        //on lit tout le fichier ligne par ligne
        while(fgets(ligne, NBCHARMAX, fichier) != NULL)
        {
            int i =0;
            while ( ligne[i] != " " && ligne[i] != '\n' )
            {
                premierMot[i] = ligne[i];
                i++;
            }
            premierMot[i] = '\0';
            if (strcmp(premierMot,"NAME"))
            {
                int depart = 7;
                while (ligne[depart] != '\n' && depart-7 <= TAILLENOM)
                {
                    problemeTsp->name[depart-7] = ligne[depart];
                    depart++;
                }
                problemeTsp->name[depart]='\0';
            }
            if (strcmp(premierMot,"DIMENSION"))
            {
                int depart = 12;
                char dim[10];
                while (ligne[depart] != '\n')
                {
                    dim[depart-12] = ligne[depart];
                    depart++;
                }
                dim[depart]='\0';
                problemeTsp->dimension=atoi(dim);

            }
            if (strcmp(premierMot,"EDGE_WEIGHT_TYPE"))
            {
                int depart = 19;
                while (ligne[depart] != '\n' && depart-19 <= TAILLENOM)
                {
                    problemeTsp->EDGE_TYPE[depart-19] = ligne[depart];
                    depart++;
                }
                problemeTsp->EDGE_TYPE[depart]='\0';
            }
            if (strcmp(premierMot,"NODE_COORD_SECTION"))
            {
                problemeTsp->tabCoord = creer_mat_int(2,problemeTsp->dimension);
                int indCoords;
                for(int i = 0; i<problemeTsp->dimension ; i++)
                {
                    fgets(ligne, NBCHARMAX, fichier);
                    char x[10];
                    char y[10];
                    int j=0;
                    while ( ligne[j] != ' ')
                    {
                        j++;
                    }
                    j++;
                    indCoords = 0;
                    while ( ligne[j] != ' ')
                    {
                        x[indCoords] = ligne[j];
                        j++;
                        indCoords++;
                    }
                    x[indCoords] = '\0';
                    problemeTsp->tabCoord[0][i]= atoi(x);
                    j++;
                    indCoords =0;
                    while ( ligne[j] != '\n')
                    {
                        y[indCoords] = ligne[j];
                        j++;
                        indCoords++;
                    }
                    y[indCoords]='\0';
                    problemeTsp->tabCoord[1][i]= atoi(y);

                }
            }
        }
        fclose(fichier);

    }
    else
    {
        erreur("pas de fichier");
        fclose(fichier);
        return 0;
    }
    return 1;
}

void mode_normal(instance_t *inst)
{

}

void mode_verbose(instance_t *inst)
{

}

void mode_csv(instance_t *inst)
{

}


int main(int n, char* param[])
{
    instance_t problemeTsp;
    tour_t problemeTour;
    printf("n = %d\n",n);
    for ( int l = 0; l < n; l++)

    {
        printf("param %d = %s\n",l,param[l]);
    }
    char* methodes = "";//liste des methodes utilisees
    int nbMethodes = 0;//nombre de methodes utilisees

    //chemins de la ligne de commande
    char* cheminTsp = NULL;
    char* cheminSolution = NULL;
    char* cheminVerbose = NULL;
    char* cheminCsv = NULL;

    //boolean de la ligne de commande
    int modeSolution = 0;
    int modeVerbose = 0;
    int resultatsFichierCsv = 0;

    //methodes de calcul
    int forceBrute = 0;
    int forceBruteMatrice = 0;
    int plusProcheVoisin = 0;
    int randomWalk = 0;
    int opt2 = 0;
    int algoGenetique = 0;
    //verification que nous avons une bonne base sur la ligne de commande ( n > 3 et -f <chemin> + 1 option minimum (methode de calcul))
    if ( n < 4)
    {
        erreur("nb d'arguments trop faible");
    }
    if (!strcmp(param[1],"-f"))
    {
        cheminTsp = param[2];
    }
    // recuperation des informations de la ligne de commande
    int i = 3;
    while ( i < n )
    {
        // option -t -v et -h 
        if (!strcmp(param[i],"-t"))
        {
            if (i+1 < n)
            {
                modeSolution = 1;
                cheminSolution = param[i+1];
                i++;
            }
            else
            {
                erreur("pas de chemin pour le fichier Solution");
            }
            
        }
        else if (!strcmp(param[i],"-o"))
        {
            if (i+1 < n)
            {
                resultatsFichierCsv = 1;
                cheminCsv = param[i+1];
                i++;
            }
            else
            {
                erreur("pas de chemin pour le fichier Csv");
            }
        }
        else if (!strcmp(param[i],"-v"))
        {
            modeVerbose = 1;
        }
        else if (!strcmp(param[i],"-h"))
        {
            printf("\nUsage :  ./tsp -f <file> [-t <tour>] [-v [<file>]] -<méthode> [-h]\n\n-f <file> : nom du fichier tsp (obligatoire)\n-t <file> : nom du fichier solution (optionnel)\n-v [<file>] : mode verbose (optionnel), écrit des informations intermédiaires à l’écran ou dans un fichier si un nom de fichier est présent.\n-o <file> : export des résultats sans un fichier csv\n-h : help, affiche ce texte\n\n<méthode> : méthodes de calcul (cumulables) :\n\n-bf : brute force,\n-bfm : brute force en utilisant la matrice des distances,\n-ppv : plus proche voisin,\n-rw : random walk,\n-2opt : 2 optimisation. Si -ppv ou -rw ne sont pas présentes on utilise -rw,\n-ga <nombre d’individus> <nombre de générations> <taux de mutation> : algorithme génétique,défaut = 20 individus, 200 générations, 0.3 mutation.\n\n");
            exit(1);
        }

        //methodes de calcul
        else if(!strcmp(param[i], "-bf")) //force brute
        {
            forceBrute = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);

        }
        else if(!strcmp(param[i], "-bfm")) //force brute avec matrice
        {
            forceBruteMatrice = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);
        }
        else if(!strcmp(param[i], "-ppv")) //plus proche voisin
        {
            plusProcheVoisin = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);
        }
        else if(!strcmp(param[i], "-rw")) //random walk
        {
            randomWalk = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);
        }
        else if(!strcmp(param[i], "-2opt")) //2 opti
        {
            opt2 = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);
        }
        else if(!strcmp(param[i], "-ga")) //algo genetique
        {
            algoGenetique = 1;
            nbMethodes ++;
            methodes = concat_string(methodes,param[i]);
        }

        //fichier chemin du mode verbose
        else if(!strcmp(param[i-1], "-v"))
        {
            cheminVerbose = param[i];
        }
        
        i++;
    }
    //verification qu'il y ai au moins 1 commande
    if ((forceBrute + forceBruteMatrice + plusProcheVoisin + randomWalk + opt2 + algoGenetique) == 0)
    {
        erreur("Pas de methode de calcul donne dans la ligne de commande");
    }
    //fin du parcours de la ligne de commande, si nous sommes arrives la, c'est quelle est correcte
    printf("ligne de commande validé\n");
    //lecture_fichier(cheminTsp,&problemeTsp);
    if ((modeVerbose + resultatsFichierCsv + modeSolution) == 0)
    {
        mode_normal(&problemeTsp);
        ecriture_resultats_1(problemeTsp,param[2]);
    }
    //lecture_fichier("Data/att10.tsp",&problemeTsp);
    //printf("%d",problemeTsp.dimension);
}

    

