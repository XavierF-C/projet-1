////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de sauvegarder les dernières données du capteur frontal du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef PILE_ETATS_CAPTEUR_H
#define PILE_ETATS_CAPTEUR_H

#include <librairie.h>
#include "SegmentDetecte.h"
#include "CapteurDeLigne.h"

// Format des données dans PileEtats
struct DonneesCapteurCompactes
{
	uint8_t brute; // Représente 5 couleurs des capteurs
	SegmentDetecte segment; // Représente le segment associé
	
	void placerDonneeBrute(uint8_t donneesBrute);
};

// Permet de sauvegarder les états précédents du capteur
class PileEtatsCapteur
{	
public:
	
	PileEtatsCapteur(CapteurDeLigne& capteurDeLigne);	//place notre donnée brute dans la pile ainsi que le segment associé
	
	// Analyse les nouvelles données et modifie potentiellement le dessus du sommet
	void miseAJour(CapteurDeLigne& capteurDeLigne);
	
	// retourne la valeur du sommet
	DonneesCapteurCompactes obtenirSommet();
	
	// retourner la valeur sous le sommet si elle exite, sinon le sommet lui-même
	DonneesCapteurCompactes obtenirDessousSommet();
	
	// Indique si le sommet de la pile a changé à la dernière mise à jour
	bool sommetAChange();
	
	// Permet d'allumer les bonnes dels si un ruban noir est détecté par le capteur correpondant
	void allumerDELSiNoirDetecte();
	
	//Fonction pour deboguer la pile au complet
	void afficherPile();
	
private:
	
	const static uint8_t TAILLE_LISTE = 8;
	DonneesCapteurCompactes donnees[TAILLE_LISTE];
	uint8_t index;
	bool changementSommet;
	
	PileEtatsCapteur(); // Ce constructeur ne doit pas être appelé
	
	// pop la plus ancienne et ajoute la donnee
	void decalage(DonneesCapteurCompactes donnee);
	
	// push une donnee sur le dessus de la pile
	void ajouterDonnee(DonneesCapteurCompactes donnee);
};
#endif /* PILE_ETATS_CAPTEUR_H */
