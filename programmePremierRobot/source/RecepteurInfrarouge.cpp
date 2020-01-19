////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet la réception infrarouge du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "RecepteurInfrarouge.h"

RecepteurInfrarouge::RecepteurInfrarouge() : etat_courant(SCRUTATION), compteurBits(0),  compteurPlanB(0) {};

//La fonction est appelée quand la minuterie est expiré et décide du prochain état de la réception
void RecepteurInfrarouge::routineInterruptionMinuterie(){
	const uint16_t TEMPS_SYNCHRONISATION = 0.0012* 8000000; //correspond à 1200μS pour la minuterie
	
	switch (etat_courant){
		case START:
				//Si aucun deuxième front descendant n'a été détecté après 2500μS, on commence la réception 
				etat_courant = SYNCHRONISATION;
				minuterie.partirMinuterie1_prescaler_1(TEMPS_SYNCHRONISATION);
				break;
		case SYNCHRONISATION:
				//Si aucun front descendant n'est detecté 1200μS après le start, c'est une erreur
				etat_courant = ERREUR;
				break;
		case LECTURE:
				//Si 900μS sont écoulé sans deuxième front descendant, on effectue la lecture du bit
				message[compteurBits] = ~(PIND) & (0x01); //masque pour seulement voir la pin 1 et l'inverser
				compteurBits++;
				etat_courant = SYNCHRONISATION;
				minuterie.partirMinuterie1_prescaler_1(TEMPS_SYNCHRONISATION);
				break;
		default: 
				etat_courant = ERREUR;
	}
	
}
//Prend le tableau du message, le transforme en entier, vérifie l'addresse et retourne le message ou une valeur erronée si l'addresse n'est pas bonne
uint8_t RecepteurInfrarouge::convertirCommandeAdresseEnNombre () const {
	uint16_t nombre =0;
	for(int i=0; i < LONGEUR_COMMANDE; i++){
		nombre |= message[i] << i;
	}
	//Masque pour isoler les bits de l'addresse et ignorer le reste
	if ((nombre & (0xFF80)) == 128){	
		//Crée un masque pour ignorer l'addresse
		return (nombre & (0x007F)); 
	}
	return VALEUR_ERRONEE;//Afin de traiter le cas ou l'adresse recue n'est pas 1, nous retournerons 10;
	
}

//Fonction pour détecter un front bas du capteur
bool RecepteurInfrarouge::frontBasDetecte(bool& estFrontHaut) const {
		//Afin de détecter un front haut et attendre un front descendant
		if (PIND & 0x01) 
			estFrontHaut = true;
		//Si un front descendant détecté après le front haut précédent	
		else if (!(PIND & 0x01) && estFrontHaut) {
			_delay_us(LONGEUR_ANTIREBOND); //mécanisme du type anti rebond
			if(!(PIND & 0x01)){
				estFrontHaut = false;
				return true;
			}			
		}
	return false;		
}

//Commande appeller lorsque le bouton est enfoncé pour augmenter le compteurPlanB
void RecepteurInfrarouge::routineInterruptionBouton() {
	
	const uint16_t DUREE_DEUX_SECONDES = 2 * 7812.5;
				
	minuterie.partirMinuterie1_prescaler_1024(DUREE_DEUX_SECONDES );
				
	//Incrémenter le compteur tant qu'il n'est pas à 9
	if (compteurPlanB < 9) {
		compteurPlanB++;
	}		
	//Lorsque le compteur est à 9, fermer la minuterie et repartir à zéro	
	else {
		compteurPlanB = 1;
	}
}	

//Plan B si la récéption ne fonctionne pas, utilisation du bouton poussoir
uint8_t RecepteurInfrarouge::infrarougePlanB() {
	//La DEL rouge indique qu'il faut appuyer sur le bouton pour envoyer la valeur
	miseJourDEL(ROUGE);

	while (!minuterie.minuterie1EstExpiree()) {
		if (minuterie.boutonEstAppuye())
			routineInterruptionBouton();
		}
		
	miseJourDEL(ETEINT);
	return compteurPlanB;
}

// Prend un frame par infrarouge et retourne la commande
uint8_t RecepteurInfrarouge::receptionInfrarouge_frame() { // Prend en paramètre un objet ControlleurInterruptions, mais n'utilise que les méthodes en lien avec la minuterie

	etat_courant = SCRUTATION;
	const uint16_t TEMPS_START = 0.0025 * 8000000; // correspond à 2500μS secondes pour la minuterie
	const uint16_t TEMPS_LECTURE = 0.0009 * 8000000; // correspond à 900μS secondes pour la minuterie
	bool estFrontHaut = false;
	DDRD &= ~(0x01); // La PIN 1 de D est entrée
	
	while(compteurBits < LONGEUR_COMMANDE)
	{
		switch (etat_courant) {
		
			case SCRUTATION:
					if (frontBasDetecte(estFrontHaut)){
						
						_delay_us(LONGEUR_ANTIREBOND_SCRUTATION); // Mécanisme d'antirebond
						if (!(PIND & 0x01))
						{
							minuterie.partirMinuterie1_prescaler_1(TEMPS_START - LONGEUR_ANTIREBOND_SCRUTATION );
							etat_courant = START;
						}
						
						/*minuterie.partirMinuterie1_prescaler_1(TEMPS_START );
						etat_courant = START;*/
					}					
					break;
			case START: 
					if (frontBasDetecte(estFrontHaut)) {
						etat_courant = ERREUR;
						minuterie.fermerMinuterie1();
					}
					
					if  (minuterie.minuterie1EstExpiree()) 
						routineInterruptionMinuterie();
					
					break;
		
			case SYNCHRONISATION:
					if (frontBasDetecte(estFrontHaut)) {
						etat_courant = LECTURE;
						//Il faut attendre 900μS avant de lire la donnée
						minuterie.partirMinuterie1_prescaler_1(TEMPS_LECTURE); 
					}
					if  (minuterie.minuterie1EstExpiree()) 
						routineInterruptionMinuterie();
					break;	
			case LECTURE:					
					if (frontBasDetecte(estFrontHaut)) {
						etat_courant = ERREUR;
						minuterie.fermerMinuterie1();
					}
					if  (minuterie.minuterie1EstExpiree()) 
						routineInterruptionMinuterie();
					break;
			case ERREUR: //Quitte la machine à état avec une valeur erronée
					return VALEUR_ERRONEE;
		}
	}
	uint8_t valeurRecue = convertirCommandeAdresseEnNombre();
	
	//Le plan B est appelé si la réception échoue (mauvaise addresse), il est alors obligatoire d'utiliser le bouton
	/*if (valeurRecue == 10){
		return infrarougePlanB();
	}*/
	return valeurRecue;
}

//Cette méthode compare les trois valeurs obtenues des frames et retourne la valeur qui est présente au moins deux fois dans le tableau
uint8_t RecepteurInfrarouge::comparerFrames(uint8_t* valeurs) {
	
	uint8_t valeurRetour = VALEUR_ERRONEE;
	
	//Cherche pour une valeur majoritaire
	if (valeurs[0] == valeurs[1])
		valeurRetour = valeurs[0];
	else if ( valeurs[1]==valeurs[2])
		valeurRetour = valeurs[1];
	else if (valeurs[2]==valeurs[0])
		valeurRetour = valeurs[0];
		
	//Si la valeur majoritaire est erroné il faut utiliser le plan B
	if (valeurRetour == VALEUR_ERRONEE){
		/*
		if (valeurs[0] != VALEUR_ERRONEE)
			return valeurRetour;
		else if ( valeurs[1] != VALEUR_ERRONEE)
			return valeurRetour;
		else if (valeurs[2] != VALEUR_ERRONEE)
			return valeurRetour;
		*/
		return infrarougePlanB();
	}
	miseJourDEL(ETEINT);	
	return valeurRetour;
	
}
	
//Lit trois frame, compare ces trois frames et retourne la commande
uint8_t RecepteurInfrarouge::receptionInfrarouge() {
	
	miseJourDEL(VERT);
	const uint8_t REPETITION_FRAME = 3;
	uint8_t valeurs[REPETITION_FRAME];
	
	//Reception des trois valeurs
	for (uint8_t i = 0; i < REPETITION_FRAME; i++){	
		valeurs[i] = receptionInfrarouge_frame(); 

		_delay_ms(LONGEUR_ENTRE_RECEPTION);

	}
	return comparerFrames(valeurs); //Retourne la valeur 
}
	
	
	

