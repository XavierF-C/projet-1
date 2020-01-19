////////////////////////////////////////////////////////////////////////
/* Description :
 * Ce fichier regroupe plusieurs énumérations utilisées par les programmes
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

// Augmente la lisibilité du code pour l'état du bouton
enum EtatBouton
{
	Appuye,
	Relache
};

// Augmente la lisibilité du code pour l'état de la del sur la carte mère
enum CouleurDEL
{
	ROUGE,
	VERT,
	ETEINT,
};

// Augmente la lisibilité du code pour l'état d'une del sur le breadboard
enum AllumeDel
{
	OFF=0,
	ON=1
};

// Augmente la lisibilité du code pour le capteur de ligne
enum CouleurCapteur
{
	NOIR=0,
	BLANC=1
};

#endif /* ENUMERATIONS_H */
