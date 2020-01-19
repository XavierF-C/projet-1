////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe permet d'automatiser plusieurs actions liées aux moteurs du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef CONTROLEUR_ROUES_H
#define CONTROLEUR_ROUES_H

#include "SegmentDetecte.h"
#include "PileEtatsCapteur.h"
#include "Chronometre.h"

class ControleurRoues
{
public:
	
	ControleurRoues();
	

	// Commandes principales du controlleur de roues 
	void avancerC2C3();	//Permet au robot d'avancer avec des ajustements utiliser pour le segment du parcours entre C2 et C3
	void avancer();	//Permet au robot d'avancer avec des ajustement
	
	void devierGauche();	//Permet au robot de dévier vers la gauche
	void devierDroite();	//Permet au robot de dévier vers la droite
		
	void avancerToutDroit();	//Permet au robot d'avancer sans ajustement dynamique
	void avancerToutDroitRapidement();	//Permet au robot d'avancer sans ajustement dynamique plus rapidement
	
	void tournerGauche();	//Permet au robot de tourner sur lui même vers la gauche
	void tournerGaucheRapidement();	//Permet au robot de tourner sur lui même vers la gauche à vitesse élevée

	void tournerDroiteRapidement();	//Permet au robot de tourner sur lui même vers la droite à vitesse élevée
	void tournerDroite();	//Permet au robot de tourner sur lui même vers la droite
	
	void reculer();	//Permet au robot de reculer

	
	void miseAJour(PileEtatsCapteur& pileEtatsCapteur, Chronometre& chronometre);	//Met à jour le segment, l'ajustement et le facteur dynamique
	
	void mettreMarche();	//Donne la vitesse au robot selon les différents ajustement
	
	// Pour arrêter les moteurs
	void eteindreMoteurs();
	
	// Nos booléens peuvent briser dans private...
	bool directionRoueGauche;
	bool directionRoueDroite;
private:
	// Etat du controlleur de Roue
	enum Etat{ 
		AUCUN_AJUSTEMENT,
		AVANCER_AVEC_AJUSTEMENTS,
		AVANCER_AVEC_AJUSTEMENTS_C2_C3,
	};
	Etat etat;
	
	// Sert à savoir si la position du capteur par rapport au ruban
	enum PositionCapteur {
		AU_CENTRE,
		A_DROITE,
		A_GAUCHE
	};
	PositionCapteur positionCapteur;
	
	// Vue de dos
	uint8_t pwmRoueGauche;
	uint8_t pwmRoueDroite;
	int8_t ajustementRoueGauche;
	int8_t ajustementRoueDroite;
	uint8_t facteurDynamique;
	
	
	// Constantes pour le contrôle des roues
	const bool AVANCER = false;
	const bool RECULER = true;
	const uint8_t ARRET = 0;
	const uint8_t VITESSE_NORMALE = 120;
	const uint8_t VITESSE_ROTATION = 130; // Lorsque le robot tourne sur lui-même
	const uint8_t VITESSE_ELEVE = 180;
	const uint8_t VITESSE_RALENTIE = 120;
	
	// Variable utile pour déterminer si les capteurs sont au centre, à gauche ou à droite
	uint16_t compteurMemePositionCapteur = 0;
	
	// Poids pour l'ajustement dynamique
	const uint8_t AJUSTEMENT_AUCUN = 0;
	const uint8_t AJUSTEMENT_LEGER = 20;
	const uint8_t AJUSTEMENT_MODERE = 30;
	const uint8_t AJUSTEMENT_ELEVE = 40;
	const uint8_t AJUSTEMENT_MAXIMAL = 50;
	
	// Position des pins qui controlent le sens des roues
	const uint8_t DECALAGE_B3 = 2;
	const uint8_t DECALAGE_B6 = 5;
	
	// Configure le signal PWM sur le timer0 pour les roues
	void initialisationPWM_timer0(void);
	
	// Ajuster le sens de rotation des roues, vue de dos
	void ajusterSens_roues( bool directionGauche, bool directionDroite);
	
	int8_t poidsGaucheDuSegment(SegmentDetecte& segment);	//Met des poids sur les différents segment de notre roue gauche
	int8_t poidsDroitDuSegment(SegmentDetecte& segment);	//Met des poids sur les différents segment de notre roue droite
	int8_t calculerCible(int8_t poidSegmentCourant, int8_t poidSegmentPrecedent);	//Retourne notre ajustement maximal que le robot tentera d'atteindre dynamiquement
	void tendreAjustementVers(int8_t& ajustement, int8_t cible);	//Fait tendre l'ajustement vers la cible
	
	// Méthodes pour mettre à jour diverses données du contrôlleur de roues
	void miseAJourPositionCapteur(PileEtatsCapteur& pileEtatsCapteur, Chronometre& chronometre);	//Met à jour l'état PositionCapteur du robot selon le segment détecté
	void miseAJourEtat(PileEtatsCapteur& pileEtatsCapteur);	//Met à jour les ajustements selon le segment où se situe le robot
	void miseAJourFacteurDynamique(PileEtatsCapteur& pileEtatsCapteur);	//ajoute un facteur dynamique maximal lors d'un changement de sommet et ralentie le robot s'il a déja un facteur dynamique
	void miseAJourAvancer(PileEtatsCapteur& pileEtatsCapteur);	//Met à jour la cible que l'ajustement veut atteindre et fait tendre l'ajustement vers cette cible
	
};

#endif /* CONTROLEUR_ROUES_H */
