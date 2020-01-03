#include "tspstat.h"

void f_ppv(instance_t instance, tour_t *tournee)
{
    int voisins_restants = instance.dimension;//nb de voisins qu'il reste
    int *tab = creer_tab_int(instance.dimension);//tableau intermediaire des points par lesquels nous sommes deja passe
    tournee->tour[0] = 0;//point de depart
    //calcul du plus proche voisin
    int ppv = init_indice_plus_proche_voisin(instance,tournee[0]);
    //premier voisin trouve : sommet numero "ppv"
    voisins_restants -= 1;
    tab[0] = ppv;
    tournee->tour[1] = ppv;
    for ( int i =1; i < instance.dimension; i++)
    {
        int Inddep = ppv;
        ppv = indice_plus_proche_voisin(instance,tournee[0],tab,Inddep,voisins_restants);
        voisins_restants -=1;
        tab[i] = ppv;
        tournee->tour[i+1] = ppv;
    }
}

int indice_plus_proche_voisin(instance_t instance, tour_t tournee,int *tab,int indDepart,int voisins_restants)
{
    int xA = instance.tabCoord[0][indDepart-1];//Coord Abs du point de depart (A)
    int yA = instance.tabCoord[1][indDepart-1];//Coord Ord du point de depart (A)
    double dist = 100000;
    int ppv;
    for (int i =1; i < tournee.dimension; i++)
    {
        //on passe seulement par les sommets pas encore visites
        int z = 0;
        for ( int j = 0; j<instance.dimension - voisins_restants; j++)
        {
            if ( i == tab[j])
            {
                z=1;
            }
        }
        double distAB;
        if (z==0)//sommet pas encore visite
        {
            int xB = instance.tabCoord[0][i-1];//Coord Abs du point B
            int yB = instance.tabCoord[1][i-1];//Coord Ord du point B
            distAB = sqrt((pow((xB - xA),2))+(pow((yB - yA),2)));
        }

        if (distAB < dist && z==0)
        {
            dist = distAB;
            ppv = i;
        }
    }
    return ppv;

}

int init_indice_plus_proche_voisin(instance_t instance, tour_t tournee)
{
    int xA = 0;//Coord Abs du point de depart (A)
    int yA = 0;//Coord Ord du point de depart (A)
    double dist = 100000;
    int ppv;
    for (int i =1; i < tournee.dimension; i++)
    {
        double distAB;
        int xB = instance.tabCoord[0][i-1];//Coord Abs du point B
        int yB = instance.tabCoord[1][i-1];//Coord Ord du point B
        distAB = sqrt((pow((xB - xA),2))+(pow((yB - yA),2)));
        //printf("i:%d   distAB : %lf",i,distAB);
        if (distAB < dist)
        {
            dist = distAB;
            ppv = i;
        }
    }
    return ppv;
}
