////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de gérer l'émission de l'infrarouge
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "EmetteurInfrarouge.h"
//Constructeur
EmetteurInfrarouge::EmetteurInfrarouge() : compteur(0) { }


//Pour convertir un nombre en commande et addresse (en 12 bits) qui pourront être envoyés par infrarouge et interprété par le robot récepteur
void EmetteurInfrarouge::convertirNombreEnCommandeAdresse(bool* listeBits, uint8_t nombre) const{ //listeBits est la valeur de retour de la fonction
	
	uint16_t nombreEnCommandeAdresse = 0;
	//L'adresse est 0001 mais les bits sont inversés à 1000 pour être conformes aux 12 bits du protocole SIRC
	const uint16_t ADRESSE_TV = 128; 
	
	// Le nombre est entre 0 et 9 et tient sur 4 bits, il doit être placé aux 8 bits les plus significatifs des 12 bits, soit les bits correspondant à la commande
	nombreEnCommandeAdresse = ADRESSE_TV;
	
	//L'adresse prend les 4 bits moins significatifs sur 12 bits
	nombreEnCommandeAdresse += nombre; 
	
	//Les bits sont placés dans un tableau de booléens
	for(uint8_t i = 0; nombreEnCommandeAdresse > 0; i++){
		listeBits[i] = nombreEnCommandeAdresse % 2;
		nombreEnCommandeAdresse = nombreEnCommandeAdresse / 2;
	}
}

void EmetteurInfrarouge::emissionInfrarougeFrame(bool* listeBits) {
	
	//PWM à 38kHz: 104 a été obtenus en isolant OCRnx de l'équation de la documentation Atmel 2015 focnx = fclk/(2*N*(1+OCRnx) où N = 1 (prescaler) et focnx = 38 000 (fréquence de la DEL infrarouge) et fclk = 8*10⁶
	const uint8_t IR_ON = 104; 
	//L'unité d'interval de temps est 600μS
	const uint16_t INTERVAL = 600;
	//45μS durée de l'emission d'un frame
	const uint16_t DUREE_FRAME = 0.045 * 7812.5;
	
	
	bool estEnTransmission = true;	
	
	minuterie.partirMinuterie1_prescaler_1024(DUREE_FRAME);
	
	//On reste dans la boucle tant que 45μS ne sont pas écoulées, si la transmission finit avant (estEnTransmission = false), la boucle ne fait que gagner du temps pour atteindre 45μS
	while(!minuterie.minuterie1EstExpiree()) {
		
		if (estEnTransmission) {
			//Pour générer PWM en mode CTC à 38kHz 
			changerSignalPWM_timer0(IR_ON, 0); // START: Pour indiquer le commencement de la reception (PWM pour 2400μS et pas de PWM pour 600μS)
			_delay_us(INTERVAL * 4);
			changerSignalPWM_timer0(0, 0) ;
			_delay_us(INTERVAL);
			
			for(uint8_t i = 0; i < LONGEUR_COMMANDE; i++){
				if ( listeBits[i]) { // Émission pour envoyer 1 (PWM pour 1200μS et pas de PWM pour 600μS)
					
					changerSignalPWM_timer0(IR_ON, 0); 
					_delay_us(INTERVAL * 2);
					
					changerSignalPWM_timer0(0, 0);
					_delay_us(INTERVAL);
					
				}	
				else if (!listeBits[i]) {// Émission pour envoyer 0 (PWM pour 600μS et pas de PWM pour 600μS)
			
					changerSignalPWM_timer0(IR_ON, 0); 
					_delay_us(INTERVAL);
					
					changerSignalPWM_timer0(0, 0);
					_delay_us(INTERVAL);
				}
		
			}
			
			estEnTransmission = false;
		}

	}
	
}


void EmetteurInfrarouge::emissionInfrarouge() {
	//L'émission se fait 3 fois pour minimiser le nombre d'erreurs et avoir un plus haut taux de réussite
	uint8_t REPETITION_FRAME = 3;
	bool listeBits[LONGEUR_COMMANDE]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Message d'erreur
	
	//Conversion du compteur en commande et addresse pour l'émission infrarouge
	convertirNombreEnCommandeAdresse(listeBits, compteur);
	
	
	for (uint8_t i = 0; i < REPETITION_FRAME; i++) 
		emissionInfrarougeFrame(listeBits);
	
}

void EmetteurInfrarouge::routineInterruptionBouton() {
	
	const uint16_t DUREE_DEUX_SECONDES = 2 * 7812.5;
				
	minuterie.partirMinuterie1_prescaler_1024(DUREE_DEUX_SECONDES );
	minuterie.partirMinuterie1_prescaler_1024(DUREE_DEUX_SECONDES );
				
	//Incrémenter le compteur tant qu'il n'est pas à 9
	if (compteur < 9) {
		compteur++;
	}		
	//Lorsque le compteur est à 9, fermer la minuterie et repartir le compteur à 1 	
	else {
		compteur = 1;
	}
}	
			
	

void EmetteurInfrarouge::routineInterruptionMinuterie (){
		
	// Si deux secondes sont écoulées, l'émission infrarouge s'effectue, le compteur est remit à zéro et on ferme la minuterie 
	fermerMinuterie();
	//Envoie des données par infrarouge	
	miseJourDEL(ROUGE);
	emissionInfrarouge();
	miseJourDEL(ETEINT);			
	compteur = 0;
	
	}
