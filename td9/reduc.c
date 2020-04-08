#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>
#include <limits.h>

// Enumération des différents opérateurs possibles
typedef enum {
	OCD_SOMME,
	OCD_MOYENNE,
	OCD_MAX,
	OCD_MIN,
	OCD_TOTAL
} opcode_t;

// Structure contenant les arguments du programme
typedef struct {
	int nbThreads;				//< Nombre de threads à utiliser
	int tailleTableau;		//< Taille du tableau utilisé pour la réduction
	opcode_t code;				//< Opération à réaliser
} arg_t;

// Structure contenant les informations nécessaires au thread
typedef struct {
	int *tab;							//< Tableau d'entiers à traiter
	int start;						//< Indice de début de traitement
	int stop;							//< Indice de fin de traitement (non compris)
	int res;							//< Résultat local
} message_t;

// Alias de pointeurs de fonction
typedef int (* ptrVerif) (int *, int, int);

// Fonction thread -- calcule la somme locale d'un tableau
// \param	arg 				Message transmis par le thread père
// \return						NULL
void * sommeTableau (void * arg) {

	//
	message_t *msg = (message_t *)arg;

	//
	for( int i = msg->start ; i < msg->stop ; i++)
		msg->res += msg->tab[i];

	return NULL;
}

// Fin de la réduction -- calcule la somme globale
// \param	msg					Messages issus de l'exécution des threads,
//										contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return						Résultat global
int reducSomme (message_t * msg, int nbThreads) {
	int res = 0;

	for( int i = 0 ; i < nbThreads ; i++)
		res += msg[i].res;

	return res;
}

// Fonction thread -- calcule la moyenne locale d'un tableau
// \param	arg 				Message transmis par le thread père
// \return						NULL
void * moyenneTableau (void * arg) {

	//
	message_t *msg = (message_t *)arg;

	//
	for( int i = msg->start ; i < msg->stop ; i++)
		msg->res += msg->tab[i];

	msg->res /= (msg->stop - msg->start);

	return NULL;
}

// Fin de la réduction -- calcule la moyenne globale
// \param	msg							Messages issus de l'exécution des threads,
//												contiennent les résultats locaux
// \param	nbThreads				Nombre de threads, et donc de messages
// \return								Résultat global
int reducMoyenne (message_t * msg, int nbThreads) {
	int res = 0;

	for( int i = 0 ; i < nbThreads ; i++)
		res += msg[i].res;

	res /= nbThreads;

	return res;
}

// Fonction thread -- calcule le maximum local d'un tableau
// \param	arg 				Message transmis par le thread père
// \return						NULL
void * maxTableau (void * arg) {

	//
	message_t *msg = (message_t *)arg;

	//
	msg->res = 0;

	//
	for( int i = msg->start ; i < msg->stop ; i++) {
		if( msg->tab[i]  >=  msg->res ) msg->res = msg->tab[i];
	}

	return NULL;
}

// Fin de la réduction -- calcule le maximum global
// \param	msg							Messages issus de l'exécution des threads,
//												contiennent les résultats locaux
// \param	nbThreads				Nombre de threads, et donc de messages
// \return								Résultat global
int reducMax (message_t * msg, int nbThreads) {
	int res = msg[0].res;

	for( int i = 1 ; i < nbThreads ; i++) {
		if( msg[i].res  >=  res ) res = msg[i].res;
	}

	return res;
}

// Fonction thread -- calcule le minimum local d'un tableau
// \param	arg 				Message transmis par le thread père
// \return						NULL
void * minTableau (void * arg) {

	//
	message_t *msg = (message_t *)arg;

	//
	msg->res = INT_MAX;

	//
	for( int i = msg->start ; i < msg->stop ; i++) {
		if( msg->tab[i]  <=  msg->res ) msg->res = msg->tab[i];
	}

	return NULL;
}

// Fin de la réduction -- calcule le minimum global
// \param	msg							Messages issus de l'exécution des threads,
//												contiennent les résultats locaux
// \param	nbThreads				Nombre de threads, et donc de messages
// \return								Résultat global
int reducMin (message_t * msg, int nbThreads) {
	int res = msg[0].res;

	for( int i = 1 ; i < nbThreads ; i++) {
		if( msg[i].res  <=  res ) res = msg[i].res;
	}

	return res;
}

// NE PAS TOUCHER
// Fonction de vérification -- 	somme des éléments du tableau
// \param	tableau								Tableau d'entiers
// \param	tailleTableau					Taille du tableau d'entiers
// \param	resultat							Résultat obtenu par la réduction
// \return											VRAI si le résultat est juste, FAUX sinon
int verifSomme (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- 	moyenne des éléments du tableau
// \param	tableau								Tableau d'entiers
// \param	tailleTableau					Taille du tableau d'entiers
// \param	resultat							Résultat obtenu par la réduction
// \return											VRAI si le résultat est juste, FAUX sinon
int verifMoyenne (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	if (tailleTableau == 0)
		return resultat == 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == (tmp / tailleTableau);
}

// NE PAS TOUCHER
// Fonction de vérification -- 	maximum du tableau
// \param	tableau								Tableau d'entiers
// \param	tailleTableau					Taille du tableau d'entiers
// \param	resultat							Résultat obtenu par la réduction
// \return											VRAI si le résultat est juste, FAUX sinon
int verifMax (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp < tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// NE PAS TOUCHER
// Fonction de vérification -- 	minimum du tableau
// \param	tableau								Tableau d'entiers
// \param	tailleTableau					Taille du tableau d'entiers
// \param	resultat							Résultat obtenu par la réduction
// \return											VRAI si le résultat est juste, FAUX sinon
int verifMin (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 101;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp > tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// Analyse des arguments du programme. Quitte si le nombre d'arguments est
// différent de 4. Remplit la structure de données des arguments.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Structure de données des arguments
arg_t analyseArguments (int argc, char ** argv) {

	// Init var
	arg_t a;
	a.nbThreads = -1;
	a.tailleTableau = -1;
	a.code = -1;

	// Check argc
	if( argc  <  4 )
		return a;

	/*
	int opt = 0;
	while ((opt = getopt(argc, argv, "sto:")) != -1) {
		switch( opt )
	*/


	if( isdigit( *argv[1] ) )
		a.nbThreads = atoi( argv[1] );
	else
		printf("analyseArguments: wrong format: argv[1]\n");

	if( isdigit( *argv[2] ) )
		a.tailleTableau = atoi( argv[2] );
	else
		printf("analyseArguments: wrong format: argv[2]\n");

	if( *argv[3]  ==  '+' ) a.code = OCD_SOMME;
	else if( *argv[3]  ==  '/' ) a.code = OCD_MOYENNE;
	else if( *argv[3]  ==  'M' ) a.code = OCD_MAX;
	else if( *argv[3]  ==  'm' ) a.code = OCD_MIN;
	else {
		printf("analyseArguments: wrong operator");
		exit(1);
	}

	return a;
}

// NE PAS TOUCHER
// Récupération de la fonction de vérification à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return						Pointeur de la fonction de vérification
ptrVerif decodeOpcodeVerif (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return verifSomme;
		case OCD_MOYENNE:	return verifMoyenne;
		case OCD_MAX:			return verifMax;
		case OCD_MIN:			return verifMin;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Génération du tableau avec des entiers compris entre 1 et 100.
// \param	tailleTableau		Taille du tableau d'entiers
// \return								Tableau d'entiers
int * genereTableau (int tailleTableau) {
	// Init tab
	int *tab = NULL;

	// Check tailleTableau
	if( tailleTableau  <=  0) {
		printf("genereTableau: wrong table size\n");
		return NULL;
	}

	// Allocation
	tab = malloc( tailleTableau * sizeof(int) );

	// Check tab
	if( !tab ) {
		printf("genereTableau: wrong alloc 'tab'\n");
		return NULL;
	}

	// Init alea number
	for( int i = 0 ; i < tailleTableau ; i++)
		tab[i] = rand() % 100;

	/*
	for( int i = 0 ; i < tailleTableau ; i++)
		printf("%d  ", tab[i]);
	printf("\n");
	*/

	return tab;

}

// Fonction chargée de la réduction multi-threadé, elle va initialiser les
// différentes variables utilisées par les threads (tableau d'entier, messages,
// etc.) puis créer les threads. Une fois l'exécution des threads terminée et
// la réduction opérée, la vérification du résultat est faite.
// \param	arg 			Arguments du programme décodés
void programmePrincipal (const arg_t arg) {
	// Déclaration des variables
	int * tab, res;

	// Pthread_t tab
	pthread_t *tab_tid;

	// Init msg
	message_t *tab_msg;

	// Allocation de la mémoire
	tab = genereTableau( arg.tailleTableau );

	// Messages
	tab_msg = malloc( arg.nbThreads * sizeof(message_t) );
	if( !tab_msg ) {
		printf("programmePrincipal: wrong alloc 'tab_msg'\n");
		return;
	}

	// Threads
	tab_tid = malloc( arg.nbThreads * sizeof(pthread_t) );
	if( !tab_tid ) {
		printf("programmePrincipal: wrong alloc 'tab_tid'\n");
		return;
	}

	// Initialisation des variables et création des threads

	// Messages
	int tab_size_thread = arg.tailleTableau / arg.nbThreads;
	for( int i = 0 ; i < arg.nbThreads ; i++) {
		tab_msg[i].tab   = tab;
		tab_msg[i].start = i * tab_size_thread;

		if( ( (i+1) * tab_size_thread )  >  arg.tailleTableau )
			tab_msg[i].stop  = arg.tailleTableau;
		else tab_msg[i].stop = (i+1) * tab_size_thread;

		tab_msg[i].res   = 0;
	}

	// Fonction operator
	void * op_func;
	switch( arg.code ) {
		case 0:
			op_func = sommeTableau;
			break;
		case 1:
			op_func = moyenneTableau;
			break;
		case 2:
			op_func = maxTableau;
			break;
		case 3:
			op_func = minTableau;
			break;
		default:
			op_func = NULL;
	}

	// threads
	for( int i = 0 ; i < arg.nbThreads ; i++) {
		if( pthread_create( &tab_tid[i], NULL, (void *)op_func, &tab_msg[i] ) ) {
			perror("programmePrincipal: pthread_create failed\n");
			return;
		}
	}

	// Jointure
	for( int i = 0 ; i < arg.nbThreads ; i++) {
		if( pthread_join( tab_tid[i], NULL ) ) {
			perror("programmePrincipal: pthread_join failed\n");
			return;
		}
	}

	// Réduction et affichage du résultat
	switch( arg.code ) {
		case 0:
			res = reducSomme( tab_msg, arg.nbThreads );
			break;
		case 1:
			res = reducMoyenne( tab_msg, arg.nbThreads );
			break;
		case 2:
			res = reducMax( tab_msg, arg.nbThreads );
			break;
		case 3:
			res = reducMin( tab_msg, arg.nbThreads );
			break;
		default:
			res = -1;
	}

	// NE PAS TOUCHER
	if ( (* (decodeOpcodeVerif (arg.code) ) ) (tab, arg.tailleTableau, res) )
		printf ("Le resultat est juste.\n");
	else printf ("Le resultat est faux.\n");
	// FIN

	// Libération de la mémoire
	free(tab);
	free(tab_tid);
	free(tab_msg);
}

// NE PAS TOUCHER
// Fonction main -- analyse les arguments donnés au
// programme, puis exécute le programme principal, chargé de la réduction
// multi-threadée.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Code de retour du programme (0 en cas de réussite)
int main (int argc, char ** argv) {
	arg_t a;

	srand (time (NULL) );

	a = analyseArguments (argc, argv);
	programmePrincipal (a);

	return 0;
}
