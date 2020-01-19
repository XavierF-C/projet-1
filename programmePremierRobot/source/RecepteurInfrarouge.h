////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet la réception infrarouge du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef RECEPTION_INFRAROUGE_H
#define RECEPTION_INFRAROUGE_H

#include <librairie.h>

enum EtatsReception { SCRUTATION, START, ERREUR, SYNCHRONISATION, LECTURE};

class RecepteurInfrarouge {

public:
	// Constructeur
	RecepteurInfrarouge();
	
	// retourne la commande transmit par infrarouge
	uint8_t receptionInfrarouge();	

private:
	
	
	void routineInterruptionMinuterie();
	uint8_t convertirCommandeAdresseEnNombre () const;
	bool frontBasDetecte(bool& estFrontHaut) const;	
	uint8_t receptionInfrarouge_frame();
	void routineInterruptionBouton();	
	uint8_t infrarougePlanB();
	uint8_t comparerFrames(uint8_t* valeurs);
	
	//Constantes
	static const uint8_t LONGEUR_COMMANDE = 12;
	static const uint8_t VALEUR_ERRONEE = 10;
	static const uint8_t LONGEUR_ANTIREBOND = 50; // Représente un temps en microsecondes
	static const uint16_t LONGEUR_ANTIREBOND_SCRUTATION = 300; // Représente un temps en microsecondes
	static const uint8_t LONGEUR_ENTRE_RECEPTION = 16; // Représente un temps en millisecondes
	
	//Attributs
	EtatsReception etat_courant;
	bool message[LONGEUR_COMMANDE]; // Tableau pour lire chaque bit du message
	uint8_t compteurBits; // Compte le nombre de bit qui a été lue
	uint8_t compteurPlanB; // Compte la valeur de la commande si la réception a échouée
	
	ControleurInterruptions minuterie;
	
	
};

#endif /* RECEPTION_INFRAROUGE_H */
