////////////////////////////////////////////////////////////////////////
/* Description :
 * Le programme du deuxieme robot permet d'envoyer des signaux infrarouges
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "EmetteurInfrarouge.h"

int main() {
	//--INITIALISATION--//
	// Configure le signal PWM sur le timer0 pour l'émission infrarouge
	initialisationPWM_timer0();
	 
	initialisationUART(); //Pour debug
	
	EmetteurInfrarouge emetteurInfrarouge;
	
	ControleurInterruptions controleurInterruption; //Nous permet de gérer les interruptions de bouton et de minuterie
	
	//--CODE--//
	for (;;){
		// Scrute le bouton et si le bouton est enfoncé le compteur sera incrémenter
		if (controleurInterruption.boutonEstAppuye()){
			emetteurInfrarouge.routineInterruptionBouton();
		}
		// Scrute la minuterie et envoie le message si la minuterie est expiré
		if (controleurInterruption.minuterie1EstExpiree()){
			emetteurInfrarouge.routineInterruptionMinuterie();
		
		}
	}
	return 0;

	}
