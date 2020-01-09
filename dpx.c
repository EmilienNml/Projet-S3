#include "tspstat.h"
#include "algogentique.h"

int indice_ppv(instance_t instance,tab_t2 tab,tab_t2 fille,int indDepart,int voisins_restants)
{
    int xA,yA;
    if (indDepart == 0)
    {
        xA = 0;
        yA = 0;
    }
    else
    {
        xA = instance.tabCoord[0][indDepart-1];//Coord Abs du point de depart (A)
        yA = instance.tabCoord[1][indDepart-1];//Coord Ord du point de depart (A)
    }
    //printf(" sommet %d : [ %d , %d ]\n ",indDepart,xA,yA);//DEBUG
    double dist = 100000;
    int ppv;
    for (int i = 0; i < tab.dim; i++)
    {
        //on passe seulement par les sommets pas encore visites
        int z = 0;
        for ( int j = 0; j<instance.dimension - voisins_restants; j++)
        {
            if ( i == fille.tab[j])
            {
                z=1;
            }
        }
        double distAB;
        if (z==0)//sommet pas encore visite
        {
            int xB,yB;
            if (i == 0)
            {
                xB = 0;
                yB = 0;
            }
            else
            {
                xB = instance.tabCoord[0][i-1];//Coord Abs du point B
                yB = instance.tabCoord[1][i-1];//Coord Ord du point B
            }
            distAB = sqrt((pow((xB - xA),2))+(pow((yB - yA),2)));
            //printf ("sommet %d : x= %d ; y = %d  distAB = %lf\n",i,xB,yB,distAB);//DEBUG
        }

        if (distAB < dist && z==0)
        {
            dist = distAB;
            ppv = i;
        }
    }
    return ppv;

}

tab_t2 dpx(instance_t instance,tab_t2 dad,tab_t2 mum)
{
    tab_t2 fille_coupe;//initialisation fille_coupe (tableau = dad avec les destructions d'arretes)
    fille_coupe.dim = dad.dim;
    fille_coupe.tab = creer_tab_int(dad.dim*2);
    int arretes_coupees = 0;
    for ( int i = 0; i < dad.dim  ; i++)//parcours du tableau dad pour detruire toutes les arretes qui ne sont pas en commum avec le tab mum 
    {
        int val = dad.tab[i];
        fille_coupe.tab[i + arretes_coupees] = val;
        int next_val = dad.tab[i+1];
        int ind_mum;
        int oui = 0;// oui = 0 si next_val n'est pas a cote de val dans le tableau mum, oui = 1 dans le cas inverse
        for ( int j = 0; j < mum.dim ; j++)//parcours de mum pour recuperer l'indice ou on a la valeur val
        {
            if ( mum.tab[j] == val)
            {
                ind_mum = j;
            }
        }
        if ( ind_mum < mum.dim -1 && ind_mum > 0)
        {
            if ( mum.tab[ind_mum - 1] == next_val || mum.tab[ind_mum + 1] == next_val)//si next_val est a cote de val dans le tableau mum
            {
                oui = 1;
            }
        }
        else if (ind_mum < mum.dim -1)//ind_mum = 0 (premier valeur du tab)
        {
            if ( mum.tab[ind_mum + 1] == next_val)//si next_val est a cote de val dans le tableau mum
            {
                oui = 1;
            }
        }
        else//ind_mum == mum.dim - 1 ( derniere valeur du tab)
        {
            if ( mum.tab[ind_mum - 1] == next_val)//si next_val est a cote de val dans le tableau mum
            {
                oui = 1;
            }
        }
        if ( oui == 0)
        {
            arretes_coupees += 1;
            fille_coupe.dim += 1;
            fille_coupe.tab[i + arretes_coupees] = 100;
        }
    }
    //printf("fill_coupe.dim : %d",fille_coupe.dim);//DEBUG
    //affiche_tab_int_ecran(fille_coupe.tab,fille_coupe.dim);//DEBUG
    tab_t2 fille;//initialisation fille_coupe (tableau = dad avec les destructions d'arretes)
    fille.dim = dad.dim;
    fille.tab = creer_tab_int(dad.dim);
    int voisins_restants = dad.dim;//nb de voisins qu'il reste
    int ppv;
    int i = 0;
    while (fille_coupe.tab[i] != 100)
    {
        fille.tab[i] = fille_coupe.tab[i];
        voisins_restants -= 1;
        i++;
    }
    i -= 1;
    //affiche_tab_int_ecran(fille.tab,fille.dim);//DEBUG
    for ( i ; i < fille.dim ; i++)
    {
        //printf("i = %d\n",i);//DEBUG
        //affiche_tab_int_ecran(fille.tab,fille.dim - voisins_restants);//DEBUG
        int Inddep = fille.tab[i];// derniere valeur avant l'arrete coupee
        ppv = indice_ppv(instance,dad,fille,Inddep,voisins_restants);
        //printf("voisins restants : %d      Inddep = %d        ppv = %d\n",voisins_restants,Inddep,ppv);//DEBUG
        int pass =0;
        for ( int z = 0; z<fille_coupe.dim; z++)
        {
            
            if ( fille_coupe.tab[z] == ppv && pass == 0)//indice du tab ou se trouve le plus proche voisin
            {
                if ( fille_coupe.tab[z - 1] == 100 && fille_coupe.tab[z + 1] == 100)// cas isole
                {
                    //printf("fille.tab[i+1] = %d  ------->  ",fille.tab[i+1]);
                    fille.tab[i+1] = fille_coupe.tab[z];
                    voisins_restants -= 1;
                    pass = 1;
                    //printf("fille.tab[i+1] = %d\n ",fille.tab[i+1]);
                }
                else if (fille_coupe.tab[z - 1] == 100)//tab de gauche vers la droite
                {
                    while ( fille_coupe.tab[z] != 100 && z<fille_coupe.dim)
                    {
                        //printf("z = %d // fille.tab[i+1] = %d  ------->  ",z,fille.tab[i+1]);
                        fille.tab[i+1] = fille_coupe.tab[z];
                        voisins_restants -= 1;
                        z++;
                        i++;
                        //printf("fille.tab[i+1] = %d\n ",fille.tab[i]);
                    }
                    pass = 1;
                    i--;
                }
                else// tab de droite vers la gauche
                {
                    while ( fille_coupe.tab[z] != 100 && z>0)
                    {
                        //printf("z = %d // fille.tab[i+1] = %d  ------->  ",z,fille.tab[i+1]);//DEBUG
                        fille.tab[i+1] = fille_coupe.tab[z];
                        voisins_restants -= 1;
                        z--;
                        i++;
                        //printf("fille.tab[i+1] = %d\n ",fille.tab[i]);//DEBUG
                    }
                    pass = 1;
                    i--;
                }
                
            }
        }
    }
    return fille;
    
}
