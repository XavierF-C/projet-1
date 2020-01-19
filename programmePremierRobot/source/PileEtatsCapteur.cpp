////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de sauvegarder les dernières données du capteur frontal du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "PileEtatsCapteur.h"

PileEtatsCapteur::PileEtatsCapteur(CapteurDeLigne& capteurDeLigne)
{
	index=0;
	
	for (uint8_t i=0; i<TAILLE_LISTE; ++i)
	{
		donnees[i].placerDonneeBrute(0); // Initialiser la pile avec des 0
	}
	
	donnees[0].placerDonneeBrute( capteurDeLigne.etatCapteur(0b100) );// Placer une donnée initialement
}

//place notre donnée brute dans la pile ainsi que le segment associé
void DonneesCapteurCompactes::placerDonneeBrute(uint8_t donneesBrute)
{
	brute = donneesBrute;
	segment = identifierSegment(brute);
}

// Analyse les nouvelles données et modifie potentiellement le dessus du sommet
void PileEtatsCapteur::miseAJour(CapteurDeLigne& capteurDeLigne)
{
	DonneesCapteurCompactes donnee;
	donnee.placerDonneeBrute(capteurDeLigne.etatCapteur(obtenirSommet().brute) );
	
	capteurDeLigne.miseAJour(); // Système d'antirebond
	if (donnee.brute != capteurDeLigne.etatCapteur(obtenirSommet().brute))
		return; // Si la donnée est différent, on n'effectue aucun changement
	
	changementSommet = false; // Le sommet n'est pas encore changé
	
	if (donnee.brute != donnees[index].brute)
	{// Si donnee est différente, on fait soit un push ou un pop de la pile
		
		// Comparer avec la donnee precedente
		if ( index > 0 && donnee.brute == donnees[index-1].brute)
		{
			--index;
			// pop une donnee sur le dessus de la pile
		}
		else
		{
			ajouterDonnee(donnee);
			// push une donnee sur le dessus de la pile
			changementSommet = true; // Le sommet a changé
		}
	}
}

// retourne la valeur du sommet
DonneesCapteurCompactes PileEtatsCapteur::obtenirSommet()
{
	return donnees[index];
}

// retourner la valeur sous le sommet si elle exite, sinon le sommet lui-même
DonneesCapteurCompactes PileEtatsCapteur::obtenirDessousSommet()
{
	uint8_t indexPrecedent = index-1;
	
	if ( index == 0)
		indexPrecedent = index;
	
	return donnees[indexPrecedent];
}

// Indique si le sommet de la pile a changé à la dernière mise à jour
bool PileEtatsCapteur::sommetAChange()
{
	return changementSommet;
}

// Permet d'allumer les bonnes dels si un ruban noir est détecté par le capteur correpondant
void PileEtatsCapteur::allumerDELSiNoirDetecte()
{
	AllumeDel allumes[NOMBRE_CAPTEURS_IR];
	
	for (uint8_t i=0; i<NOMBRE_CAPTEURS_IR; ++i)
	{
		allumes[i] = OFF;
		
		if (estMemeCouleur(NOIR, i, obtenirSommet().brute))
			allumes[i] = ON;
	}
	
	miseJour5DELS(allumes[0], allumes[1], allumes[2], allumes[3], allumes[4]);
}

//Fonction pour deboguer la pile au complet
void PileEtatsCapteur::afficherPile(){
	initialisationUART();
	for(uint8_t i = 0; i < TAILLE_LISTE; i++){
		transmissionUART(donnees[i].brute);
	}
}
	
PileEtatsCapteur::PileEtatsCapteur(){}// Ce constructeur ne doit pas être appelé
	
// pop la plus ancienne et ajoute la donnee
void PileEtatsCapteur::decalage(DonneesCapteurCompactes donnee)
{
	for(uint8_t i=0; i<TAILLE_LISTE-1; ++i)
	{
		donnees[i] = donnees[i+1];
	}
	
	donnees[index] = donnee;
}
	
// push une donnee sur le dessus de la pile
void PileEtatsCapteur::ajouterDonnee(DonneesCapteurCompactes donnee)
{
	if (index >= TAILLE_LISTE-1)
	{
		decalage(donnee);
	}
	else
	{
		++index;
		donnees[index] = donnee;
	}
	
}
