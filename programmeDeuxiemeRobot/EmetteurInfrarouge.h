////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de gérer l'émission de l'infrarouge
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef EMETTEUR_INFRAROUGE_H
#define EMETTEUR_INFRAROUGE_H

#include <librairie.h>

class EmetteurInfrarouge {
	

public:
	//Constructeur
	EmetteurInfrarouge();
	
	//Pour compter le nombre d'appuis sur le bouton poussoir
	void routineInterruptionBouton();
	//Pour effectuer l'émission infrarouge une fois les 2 secondes écoulées
	void routineInterruptionMinuterie();

private:
	//Converti le compteur en binaire et place chaque bit dans un tableau
	void convertirNombreEnCommandeAdresse(bool* listeBits, uint8_t nombre) const;
	//Parcourt le tableau pour l'emission infrarouge trois fois
	void emissionInfrarouge();
	void emissionInfrarougeFrame(bool* listeBits);
		
	//Attributs
	uint8_t compteur;
	ControleurInterruptions minuterie;
	static const uint8_t LONGEUR_COMMANDE = 12;
	};


#endif /* EMETTEUR_INFRAROUGE_H */
