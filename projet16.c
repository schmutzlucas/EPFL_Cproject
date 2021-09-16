/**
 * \file		projet16.c
 * \version		finale-1
 * \date		2016-12-08
 * \author		Schmutz Lucas 217498
 * \brief		Program for the autumn 2016 project of CS-111(c)
 */

// *******************************************************************
// 		inclusion de fichiers en-tête avec la directives include

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// *******************************************************************
//			   Symboles définies avec la directive define

#define L_MAX			500
#define C_MAX			500
#define SEUIL_MAX		1
#define SEUIL_MIN		0
#define NB_INFOS		6
#define IMG1			0
#define IMG2			1
#define CHAR_MAX_LGN		70
#define NORM			-1

//             Symboles définies avec l'instruction enum

enum Proprietes_image {D_COL, D_LIGNE, PROF, NBC, NBL, INTENSITE};

// *******************************************************************
//						Functions declaration

// ########   ne pas modifier ces déclarations de fonctions    #######

/**
 * \brief	Si le seuil n'est pas compris dans [0,1]
 * \param seuil		la valeur du seuil invalide
 */
static void erreur_seuil(double seuil);

/**
 * \brief	Si la taille de l'image n'est pas strictement positive
 * \param nbC_s		le nombre de colonnes invalide
 * \param nbL_s		le nombre de lignes invalide
 */
static void erreur_dim_s(int nbC_s, int nbL_s);

/**
 * \brief	Si la taille de l'image n'est pas inferieure à C_MAX ou L_MAX
 * \param nbC_s		le nombre de colonnes invalide
 * \param nbL_s		le nombre de lignes invalide
 */
static void erreur_taille_s(int nbC_s, int nbL_s);

/**
 * \brief	Si l'origine de l'image n'est pas positive
 * \param o_col		la colonne d'origine invalide
 * \param o_ligne	la ligne d'origine invalide
 */
static void erreur_o_s(int o_col, int o_ligne);

/**
 * \brief	Si une profondeur n'est pas unique
 * \param prof		la profondeur present a double
 */
static void erreur_prof(int prof);

void proprietes_sortie(double *seuil, int *o_col, int *o_ligne,
					   int *nbC_s, int *nbL_s, int *nb_in);

void erreur_sortie(double seuil, int nbL_s, int nbC_s, int o_ligne, int o_col);

void infos_images(int infos[NB_INFOS]);

void read_img(int nb_in, int nbL_s, int nbC_s,  int o_lgn, int o_col,
			  int infos[nb_in][NB_INFOS], int img_final[nbL_s][nbC_s],
			  int img[nb_in][nbL_s][nbC_s]);

int pgdc(int a, int b);

int ppmc(int a, int b);

int def_new_max(int nb_in, int infos[nb_in][NB_INFOS]);

void tri_prof(int nb_in, int prof[nb_in], int ordre[nb_in],
			  int infos[nb_in][NB_INFOS]);

void compostion(double seuil, int nb_in,int nbL_s,int nbC_s,int new_max,
				int ordre[nb_in], int img[nb_in][nbL_s][nbC_s],
				int infos[nb_in][NB_INFOS],
				int img_final[nbL_s][nbC_s]);

void impression(int new_max, int nbL_s, int nbC_s,
				int img_final[nbL_s][nbC_s]);

// ############################ END ##################################

// *******************************************************************
//						Global variable

/** a boolean to know whether to output verbose messages or not */
static bool	verbose;

// *******************************************************************
//						MAIN

int main(void)
{
	double seuil;
	int o_col, o_lgn, nbC_s, nbL_s, nb_in, new_max;

	proprietes_sortie(&seuil, &o_col, &o_lgn, &nbC_s, &nbL_s, &nb_in);

	erreur_sortie(seuil, nbL_s, nbC_s, o_lgn, o_col);

	int infos[nb_in][NB_INFOS];
	int img[nb_in][nbL_s][nbC_s];
	int img_final[nbL_s][nbC_s];
	int prof[nb_in];
	int ordre[nb_in];

	read_img(nb_in, nbL_s, nbC_s,  o_lgn, o_col, infos, img_final, img);

	new_max = def_new_max(nb_in, infos);

	tri_prof(nb_in, prof, ordre, infos);

	compostion(seuil, nb_in, nbL_s, nbC_s, new_max, ordre, img, infos, img_final);

	impression(new_max, nbL_s, nbC_s, img_final);

	return EXIT_SUCCESS;
}


// *******************************************************************

// ################## Ne pas modifier ces fonctions ##################

static void erreur_seuil(double seuil)
{
	printf("[1] - Seuil invalide : %lf n'est pas dans l'intervalle [0,1]\n", seuil);
	exit(EXIT_FAILURE);
}

static void erreur_dim_s(int nbC_s, int nbL_s)
{
	printf("[2] - Dimensions d'image invalides : "
	       "[%d %d] n'est pas strictement positif\n", nbL_s, nbC_s);
	exit(EXIT_FAILURE);
}

static void erreur_taille_s(int nbC_s, int nbL_s)
{
	printf("[3] - Dimensions d'image invalides : "
	       "[%d %d] est trop grand : max = [%d %d]\n", nbC_s, nbL_s, C_MAX, L_MAX);
	exit(EXIT_FAILURE);
}

static void erreur_o_s(int o_col, int o_ligne)
{
	printf("[4] - Origine invalide : {%d,%d} n'est pas positif\n", o_col, o_ligne);
	exit(EXIT_FAILURE);
}

static void erreur_prof(int prof)
{
	printf("[5] - Profondeur non unique : %d\n", prof);
	exit(EXIT_FAILURE);
}

void proprietes_sortie(double *seuil, int *o_col, int *o_ligne,
					   int *nbC_s, int *nbL_s, int *nb_input)
{
	int verbose_temp;
	scanf(" %d", &verbose_temp);
	verbose = verbose_temp;
	if(verbose)
		printf("Entrer le seuil, l'origine et la taille de l'image de sortie : \n");
	scanf(" %lf %d %d %d %d", &*seuil, &*o_col, &*o_ligne, &*nbC_s, &*nbL_s);
	if(verbose)
		printf("Entrer le nombre d'images: \n");
	scanf(" %d", &*nb_input);
}

void erreur_sortie(double seuil, int nbL_s, int nbC_s, int o_ligne, int o_col)
{
	if((seuil < SEUIL_MIN) || (seuil > SEUIL_MAX))
		erreur_seuil(seuil);
	if((nbC_s <= 0) || (nbL_s <= 0))
		erreur_dim_s(nbC_s, nbL_s);
	if((nbC_s > C_MAX) || (nbL_s > L_MAX))
		erreur_taille_s(nbC_s, nbL_s);
	if((o_col < 0) || (o_ligne < 0))
		erreur_o_s(o_col, o_ligne);
}

void infos_images(int infos[NB_INFOS])
{
	char dummy;
	int trash;
	if(verbose)
			printf("Entrer une image : \n");

	scanf(" %d %d %d %c %d %d %d %d",
		  &infos[D_COL], &infos[D_LIGNE],
		  &infos[PROF],
		  &dummy, &trash,
		  &infos[NBC], &infos[NBL],
		  &infos[INTENSITE]);
}

void read_img(int nb_in, int nbL_s, int nbC_s,  int o_lgn, int o_col,
			  int infos[nb_in][NB_INFOS], int img_final[nbL_s][nbC_s],
			  int img[nb_in][nbL_s][nbC_s])
{
	int i, c, l, n, trash;
	for(n = 0; n < nb_in; n++)
	{
		//Lecture et stockage des paramètres des images
		infos_images(&infos[n][0]);
		//Test de l'erreur de profondeur
		for(i=0; i < n; i++)
			if(((infos[n][PROF]) == (infos[i][PROF])))
				erreur_prof(infos[i][PROF]);
		//Formatage tableau img
		for(l = 0; l < nbL_s; l++)
			for(c = 0; c < nbC_s; c++)
				img[n][l][c] = NORM;

		for(l = 0; l < infos[n][NBL]; l++)
			for(c = 0; c < infos[n][NBC]; c++)
			{
				//Initialisation des coordonnées dans le référentiel du décalage
				int col_rel = infos[n][D_COL] + c;
				int lgn_rel = infos[n][D_LIGNE] + l;
				//Test : le pixel est-il dans l'image de sortie
				if ((col_rel >= o_col) && (col_rel < o_col + nbC_s)
					&& (lgn_rel >= o_lgn) && (lgn_rel < o_lgn + nbL_s))
					//Correction des coordonnées dans le référentiel de l'img_final
					scanf(" %d",&img[n][lgn_rel - o_lgn][col_rel - o_col]);
					else
						scanf(" %d",&trash);
			}
	}
}

int pgdc(int a, int b)
{
	if(b == 0)
		return a;
	return pgdc(b, a % b);
}

int ppmc(int a, int b)
{
	return (a * b)/(pgdc(a, b));
}

int def_new_max(int nb_in, int infos[nb_in][NB_INFOS])
{
	int new_max;
	//new_max prend la valeur de la première profondeur pour être injectée dans ppmc
	new_max = infos[IMG1][INTENSITE];
	int n;
	for(n = IMG2; n < nb_in; n++)
		new_max = ppmc(new_max,infos[n][INTENSITE]);
	return new_max;
}

void tri_prof(int nb_in, int prof[nb_in], int ordre[nb_in],
			  int infos[nb_in][NB_INFOS])
{
	int temp;
	int n, i, j;
	//Copie les profondeurs dans un nouveau tableau
	for(n = 0; n < nb_in; n++)
	{
		prof[n] = infos[n][PROF];
		ordre[n] = n;
	}

	for(i = nb_in - 1; i > 0; i--)
		for(j = 0; j < i; j++)
			if(prof[j] > prof[j+1])
			{
				temp = prof[j];
				prof[j] = prof[j+1];
				prof[j+1] = temp;
				temp = ordre[j];
				ordre[j] = ordre[j+1];
				ordre[j+1] = temp;
			}
}

void compostion(double seuil, int nb_in,int nbL_s,int nbC_s,int new_max,
				int ordre[nb_in], int img[nb_in][nbL_s][nbC_s],
				int infos[nb_in][NB_INFOS], int img_final[nbL_s][nbC_s])
{
	int last_img = nb_in - 1;
	int n, l, c;

	for(l = 0; l < nbL_s; l++)
		for(c = 0; c < nbC_s; c++)
			for(n = 0; n < nb_in; n++)
			{
				int prof = ordre[n] ;
				int pixel = img[prof][l][c];
				double intensite_rel = (double)pixel/
									   (double)(infos[prof][INTENSITE]);

				if(pixel != NORM && intensite_rel < seuil)
					{
						img_final[l][c] = (double)new_max * (double)intensite_rel;
						break; //pixel suivant
					}
					else if (n == last_img)
					{
						if(pixel == NORM)
							img_final[l][c] = new_max;
							else
								img_final[l][c] = (double)new_max *
												  (double)intensite_rel;
					}
			}
}

void impression(int new_max, int nbL_s, int nbC_s, int img_final[nbL_s][nbC_s])
{
	int l, c, temp = 0, nb_char = 0;
	int espace = 1;  //un espace est imprimée à la fin de chaque pixel, cc ligne 351.
	double i;

	i = (double)new_max;
	do
	{
		i = i/10;
		nb_char++;
	}while(i >= 10);

	printf("P2\n");
	printf("%d %d\n", nbC_s, nbL_s);
	printf("%d \n", new_max);

	for(l=0; l < nbL_s; l++) {
		for(c=0; c < nbC_s; c++)
		{
			//Retour à la ligne juste avant de dépasser 70 char et temp = 0
			if(temp >= CHAR_MAX_LGN-(nb_char + espace))
			{
				printf("\n");
				temp = 0;
			}
			temp = temp + printf("%d ", img_final[l][c]);

		}
	printf("\n");
	}
}

// ############################ END ##################################
