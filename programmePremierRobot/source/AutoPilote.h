////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe dicte le comportement du robot selon la méthode appelée.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "inclusionsPremierRobot.h"

#ifndef AUTOPILOTE_H
#define AUTOPILOTE_H

class AutoPilote {
	
	
public:
	AutoPilote();
	
	void miseAJourDesCapteurs();
	
	//Pour suivre la ligne lors du parcours en général 
	void suivreLaLigne();
	
	// -- Section C1 -- //
	// Permet au robot d'avoir une bonne orientation par rapport à un «T», avant de se placer sur un des 9 points
	bool alignerSelonUnT();
	
	// -- Section C2 -- //
	//Pour suivre la ligne en ralentissant sur les courbes
	void suivreLaLigneC2C3();
	
	// -- Section C3 -- //
	//Pour identifier la droite parmi les 4
	uint8_t suivreDroiteADectecter();
	
	// -- Section C4 -- //
	//Pour suivre la ligne, mais en entrant dans les rectangles
	void suivreLaLigneAvecRectangles(bool& estDansRectangle);
	// Pour éviter des problèmes de transition entre suivreLaLigne et les rectangles
	void allerToutDroit();
	void unRectangleComplete();
	bool tousRectanglesCompletes();
	void tournerCoinGaucheC1(); //Permet d'ignorer la ligne S1 pour se rendre à la section C1
	
	void eteindreMoteurs();
	
	//Les getters 
	SegmentDetecte getSegmentCourant(); //Pour savoir ce que le capteur détecte présentement
	Chronometre& getChronometre();
	ControleurRoues& getControleurRoues();
	ControleurInterruptions getControleurInterruptions();
	
	//Pour tourner à gauche de façon à ce que le robot soit droit sur la ligne ensuite
	void tournerCoinGauche();	
	
	void getPileEtatCapteur(); //Affiche le contenu de la pile pour déboguer

	////////////////////////////////
	// Attributs pour identifier la droite de section C3 (Ne fonctionnent pas lorsque privés)
	bool debutEnregistrement;
	bool premierCoinDroit;
	bool premierCoinGauche;
	////////////////////////////////
	
private:
	//void tournerCoinGauche();
	void suivreLeTrajet();
	void suivreLeTrajetC2C3();
	void suivreLeTrajetAvecRectangles(bool& estDansRectangle);
	
	//Méthodes et attribut pour l'identification des droites 1 à 4
	uint8_t droiteDetectee = 0;
	void detecterDroite1ou2();
	void detecterDroite3ou4();
	uint8_t detecterDroite();
	
	CapteurDeLigne capteurDeLigne;
	PileEtatsCapteur pileEtatsCapteur;
	ControleurRoues controleurRoues;
	ControleurInterruptions controleurInterruptions;
	Chronometre chronometre;	
	
	// Attributs pour les rectangles de C4 à C1
	uint8_t nombreRectangleDetecte = 0;
	

	//Constantes
	static const uint16_t SEUIL_DIFFERENCE_DROITE = 400; //Identification droite
	static const uint16_t TEMPS_CENTRE_SUR_COIN_MS = 1600; //Constante pour mettre le centre de masse sur le coin gauche une fois détecté
	static const uint16_t TEMPS_IGNORER_S1 = 1000; //Pour tourner au coin C1
};


#endif /* AUTOPILOTE_H */

