////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe dicte le comportement du robot selon la méthode appelée.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "AutoPilote.h"

//Constructeur
AutoPilote::AutoPilote() : pileEtatsCapteur(capteurDeLigne) {
		
	initialisationSuiveurDeLigne();
	initialisation5DELS();

	chronometre.demarrerChronometre(controleurInterruptions);
	
	//Attributs pour identifier la droite
	debutEnregistrement = false;
	premierCoinDroit = false;
	premierCoinGauche = false;
	
	}
	
	
// Getters
SegmentDetecte AutoPilote::getSegmentCourant() {
		
	return 	pileEtatsCapteur.obtenirSommet().segment;
	
}
	
Chronometre& AutoPilote::getChronometre(){
	return chronometre;
}

ControleurInterruptions AutoPilote::getControleurInterruptions() {
	
		return controleurInterruptions;
	
}

ControleurRoues& AutoPilote::getControleurRoues() {
	
		return controleurRoues;
	
}

		
		
//Mise à jour des capteurs et de la pile	
void AutoPilote::miseAJourDesCapteurs() {
	
	capteurDeLigne.miseAJour();	
	pileEtatsCapteur.miseAJour(capteurDeLigne);
	pileEtatsCapteur.allumerDELSiNoirDetecte();
		
}

//Pour suivre la ligne lors du parcours en général 
void AutoPilote::suivreLaLigne() {
		
	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();

	//Suivre la ligne en fonction des informations reçues par les capteurs
	suivreLeTrajet();
	
	//Le chronomètre est mis à jour à chaque 5 ms
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
	
}

//Pour suivre la ligne en ralentissant sur les courbes (section 2)
void AutoPilote::suivreLaLigneC2C3() {
		
	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();

	//Suivre la ligne en fonction des informations reçues par les capteurs
	suivreLeTrajetC2C3();
	
	//Le chronomètre est mis à jour à chaque 5 ms	
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
	
}

//Pour suivre la ligne, mais en entrant dans les rectangles
void AutoPilote::suivreLaLigneAvecRectangles(bool& estDansRectangle) {
	
	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();

	//Suivre la ligne en fonction des informations reçues par les capteurs
	suivreLeTrajetAvecRectangles(estDansRectangle);
	
	//Le chronomètre est mis à jour à chaque 5 ms	
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
	
}

//Pour identifier la droite parmi les 4
uint8_t AutoPilote::suivreDroiteADectecter() {
			
	uint8_t droite = 0;

	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();	
	//Suivre la ligne en fonction des informations reçues par les capteurs
	droite = detecterDroite(); 
	
	//Le chronomètre est mis à jour à chaque 5 ms	
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
	
	//On retourne 0 tant que la droite n'est pas détectée, le programme main sortira de cette méthode lorsqu'elle ne retournera plus 0 (la droite est détectée)
	return droite;
	
	
}

// Permet au robot d'avoir une bonne orientation par rapport à un «T», avant de se placer sur un des 9 points
bool AutoPilote::alignerSelonUnT()
{
	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();
	
	SegmentDetecte segmentCourant = getSegmentCourant();
	
	const uint16_t TEMPS_REQUIS_ENTRE_T_COMPLET_ET_LIGNE = 12; // Représente 60 ms
	const uint16_t TEMPS_A_RECULONS = 200; // Représente 1000 ms
	
	//Le robot recule et réavance jusqu'à ce qu'il ait un bon angle par rapport au T
	if (segmentCourant == T_COMPLET)
	{
		// Pour vérifier si le robot est bien aligné par rapport au T, le segmentCourant LIGNE doit être détecté avant le T
		if (chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_A_RECULONS) &&
		!chronometre.verifierTempsAtteint(MINUTERIE_CORRECTION, TEMPS_REQUIS_ENTRE_T_COMPLET_ET_LIGNE))
		{
			chronometre.eteindre(MINUTERIE_GENERAL);
			chronometre.eteindre(MINUTERIE_CORRECTION);
			
			controleurRoues.eteindreMoteurs();
			_delay_ms(UNE_SECONDE_MS);
			
			return true;
		}
		
		chronometre.activer(MINUTERIE_GENERAL);
		chronometre.reinitialiser(MINUTERIE_GENERAL);
	}
	if (segmentCourant == LIGNE)
	{
		chronometre.activer(MINUTERIE_CORRECTION);
		chronometre.reinitialiser(MINUTERIE_CORRECTION);
	}
	
	//Pour suivre la ligne
	if (chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_A_RECULONS))
	{
		switch(segmentCourant)
		{
			case GAUCHE:
			case LIGNE_GAUCHE:
				controleurRoues.devierGauche();
			break;
			case EXTREMITE_GAUCHE:
			case EXTREMITE_GAUCHE_GAUCHE:
			case COIN_GAUCHE:
			case GAUCHE_ERRONEE:
				controleurRoues.tournerGauche();
			break;
				
			case DROITE:
			case LIGNE_DROITE:
				controleurRoues.devierDroite();
			break;
			case EXTREMITE_DROITE:
			case EXTREMITE_DROITE_DROITE:
			case COIN_DROIT:
			case DROITE_ERRONEE:
				controleurRoues.tournerDroite();
			break;
			
			default:
				controleurRoues.avancerToutDroit();
			break;
		}
	}
	else
	{	//Le robot recule s'il n'a pas l'angle voulu
		if (segmentCourant != RIEN)
			controleurRoues.reculer();
		else
			controleurRoues.avancerToutDroit();
	}
	
	//Le chronomètre est mis à jour à chaque 5 ms	
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
	
	return false;
}

void AutoPilote::allerToutDroit()
{
	//Mise à jour des capteurs et de la pile	
	miseAJourDesCapteurs();

	//Suivre la ligne en fonction des informations reçues par les capteurs
	controleurRoues.avancerToutDroit();
	
	//Le chronomètre est mis à jour chaque 5ms	
	if (chronometre.aEteMisAJour(controleurInterruptions))
	{
		controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	}
}

void AutoPilote::eteindreMoteurs() {
	
controleurRoues.eteindreMoteurs();	
	
}

//Pour tourner à gauche de façon à ce que le robot soit droit sur la ligne ensuite
void AutoPilote::tournerCoinGauche() {
	
	controleurRoues.avancerToutDroit();
	controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	_delay_ms(TEMPS_CENTRE_SUR_COIN_MS);
	
	bool aFiniDeTourner = false;
	
	while(!aFiniDeTourner)
	{
		miseAJourDesCapteurs();
		controleurRoues.tournerGauche();
		if (chronometre.aEteMisAJour(controleurInterruptions))
		{
			controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
		}
	
		switch(getSegmentCourant())
		{
			case LIGNE:
			case DROITE:
			case EXTREMITE_DROITE:
			case LIGNE_DROITE:
			case EXTREMITE_DROITE_DROITE:
			case COIN_DROIT:
			case DROITE_ERRONEE:
				aFiniDeTourner = true;
			break;
			
			default:
				// Pas d'autres cas à considérer
			break;
		}
	}
}

//Permet d'ignorer la ligne S1 pour se rendre à la section C1
void AutoPilote::tournerCoinGaucheC1() {
	
	//Pour ignorer S1: avancer et commencer à tourner
	controleurRoues.avancerToutDroit();
	controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	_delay_ms(TEMPS_CENTRE_SUR_COIN_MS);
	
	miseAJourDesCapteurs();
	controleurRoues.tournerGauche();
	controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
	_delay_ms(TEMPS_IGNORER_S1);
	
	bool aFiniDeTourner = false;
	
	//Tourner tant que l'on ne détecte pas la ligne du parcours
	while(!aFiniDeTourner)
	{
		miseAJourDesCapteurs();
		controleurRoues.tournerGauche();
		if (chronometre.aEteMisAJour(controleurInterruptions))
		{
			controleurRoues.miseAJour(pileEtatsCapteur, chronometre);
		}
	
		switch(getSegmentCourant())
		{
			case LIGNE_GAUCHE:
			case LIGNE:
			case LIGNE_DROITE:
				aFiniDeTourner = true;
			break;
			
			default:
				// Pas d'autres cas à considérer
			break;
		}
	}
}

// Pour compter les rectangles détectés et déterminer quand ils sont tous été passés
void AutoPilote::unRectangleComplete()
{
	++nombreRectangleDetecte;
}

bool AutoPilote::tousRectanglesCompletes()
{
	const uint8_t NOMBRE_RECTANGLE = 3;
	if (nombreRectangleDetecte >= NOMBRE_RECTANGLE)
		return true;
		
	return false;
}

//Affiche le contenu de la pile pour déboguer
void AutoPilote::getPileEtatCapteur(){
	pileEtatsCapteur.afficherPile();
}

// Permet de suivre la ligne du parcours selon le segment détecté 
void AutoPilote::suivreLeTrajet()
{
	switch(pileEtatsCapteur.obtenirSommet().segment)
	{
		case RIEN:// Devrait dépendre des états précédents, si rien de nouveau n'est detecté par les capteur
		
			switch(pileEtatsCapteur.obtenirDessousSommet().segment)
			{
				case GAUCHE:
				case EXTREMITE_GAUCHE:
				case LIGNE_GAUCHE:
				case EXTREMITE_GAUCHE_GAUCHE:
				case COIN_GAUCHE:
				case GAUCHE_ERRONEE:
					controleurRoues.tournerGauche();
				break;
				
				case DROITE:
				case EXTREMITE_DROITE:
				case LIGNE_DROITE:
				case EXTREMITE_DROITE_DROITE:
				case COIN_DROIT:
				case DROITE_ERRONEE:
					controleurRoues.tournerDroite();
				break;
				
				default:
					controleurRoues.reculer();
				break;
			}
		break;
		
		case LIGNE:
		
		case LIGNE_GAUCHE:
		case GAUCHE:
		case EXTREMITE_GAUCHE:
		case EXTREMITE_GAUCHE_GAUCHE:
				
		case LIGNE_DROITE:
		case DROITE:
		case EXTREMITE_DROITE:
		case EXTREMITE_DROITE_DROITE:
			controleurRoues.avancer();
		break;
		
		case COIN_GAUCHE:
		case GAUCHE_ERRONEE:
			controleurRoues.tournerGauche();
		break;
		
		case COIN_DROIT:
		case DROITE_ERRONEE:
			controleurRoues.tournerDroite();
		break;
		
		case T_COMPLET:
			controleurRoues.eteindreMoteurs();
		break;
		case T_INCOMPLET:
			// Le robot ne réagit pas à cet état
		break;
		
		default:
			// Pas d'autres cas à considérer
		break;
	}
	
}

//Pour suivre la ligne en ralentissant sur les courbes
void AutoPilote::suivreLeTrajetC2C3()
{
	switch(pileEtatsCapteur.obtenirSommet().segment)
	{
		case RIEN:// Devrait dépendre des états précédents
		
			switch(pileEtatsCapteur.obtenirDessousSommet().segment)
			{
				case GAUCHE:
				case EXTREMITE_GAUCHE:
				case LIGNE_GAUCHE:
				case EXTREMITE_GAUCHE_GAUCHE:
				case COIN_GAUCHE:
				case GAUCHE_ERRONEE:
					controleurRoues.tournerGauche();
				break;
				
				case DROITE:
				case EXTREMITE_DROITE:
				case LIGNE_DROITE:
				case EXTREMITE_DROITE_DROITE:
				case COIN_DROIT:
				case DROITE_ERRONEE:
					controleurRoues.tournerDroite();
				break;
				
				default:
					controleurRoues.reculer();
				break;
			}
		break;
		
		case LIGNE:
		
		case LIGNE_GAUCHE:
		case GAUCHE:
		case EXTREMITE_GAUCHE:
		case EXTREMITE_GAUCHE_GAUCHE:
				
		case LIGNE_DROITE:
		case DROITE:
		case EXTREMITE_DROITE:
		case EXTREMITE_DROITE_DROITE:
			controleurRoues.avancerC2C3();
		break;
		
		case COIN_GAUCHE:
		case GAUCHE_ERRONEE:
			controleurRoues.tournerGauche();
		break;
		
		case COIN_DROIT:
		case DROITE_ERRONEE:
			controleurRoues.tournerDroite();
		break;
		
		case T_COMPLET:
			controleurRoues.eteindreMoteurs();
		break;
		case T_INCOMPLET:
			// Le robot ne réagit pas à cet état
		break;
		default:
			// Pas d'autres cas à considérer
		break;
	}
	
} 


//Pour entrer dans les rectangles lorsqu'un T est détecté
void AutoPilote::suivreLeTrajetAvecRectangles(bool& estDansRectangle)
{
	switch(pileEtatsCapteur.obtenirSommet().segment)
	{
		case RIEN:
			controleurRoues.avancerToutDroit();	
		break;
		
		case LIGNE:
			controleurRoues.avancerToutDroit();
		break;
		//Le robot évite la ligne pour rester dans le rectange
		case LIGNE_GAUCHE:
		case GAUCHE:
		case EXTREMITE_GAUCHE:
		case EXTREMITE_GAUCHE_GAUCHE:
			controleurRoues.devierDroite(); //Déviation à droite si détection d'une ligne noir à gauche
		break;
				
		case LIGNE_DROITE:
		case DROITE:
		case EXTREMITE_DROITE:
		case EXTREMITE_DROITE_DROITE:
			controleurRoues.devierGauche(); //Déviation à gauche si détection d'une ligne noir à droite
		break;
		
		case COIN_GAUCHE:
		case GAUCHE_ERRONEE:
			controleurRoues.tournerDroite();
		break;
		
		case COIN_DROIT:
		case DROITE_ERRONEE:
			controleurRoues.tournerGauche();
		break;
		
		case T_COMPLET:
		//Pour distinguer le T du rectangle et assurer que le robot ne suive pas les lignes
		if(chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_LARGEUR_RUBAN))
		{	
			estDansRectangle = false;
			controleurRoues.avancerToutDroit();
		}
		else
			controleurRoues.avancerToutDroit();
		break;
		
		case T_INCOMPLET:
		default:
			controleurRoues.avancerToutDroit();
		break;
	}
	
}

// Pour identifier la droite 2 ou 1
void AutoPilote::detecterDroite1ou2() {
	
	if(debutEnregistrement)
	{
		premierCoinGauche=true;
		miseJourDEL(ROUGE);
		if (premierCoinDroit && !chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, SEUIL_DIFFERENCE_DROITE)) {
			miseJourDEL(ETEINT);
			droiteDetectee = 2;
		}
		else if (premierCoinDroit && chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, SEUIL_DIFFERENCE_DROITE)){
			miseJourDEL(ETEINT);
			droiteDetectee = 1;
		}		
	}	
}

//Pour identifier la droite 3 ou 4
void AutoPilote::detecterDroite3ou4() {
	if(debutEnregistrement)
	{
		premierCoinDroit = true; 
		miseJourDEL(ROUGE);
		if (premierCoinGauche && !chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, SEUIL_DIFFERENCE_DROITE)){
			miseJourDEL(ETEINT);
			droiteDetectee = 4;
		}
		else if (premierCoinGauche && chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, SEUIL_DIFFERENCE_DROITE)){
			miseJourDEL(ETEINT);
			droiteDetectee = 3;	
		}
	}
}	

//Un enregistrement des coins détectés et du temps entre leur detection débute dès qu'un «T» est détecté
uint8_t AutoPilote::detecterDroite() {
	
	const uint16_t TEMPS_SUR_T = 50; //Pour éviter que le robot interprête le «T» comme un coin, en y sortant
	
	switch(pileEtatsCapteur.obtenirSommet().segment)
	{
		case RIEN:
			controleurRoues.eteindreMoteurs();
		break;
		//Tant que le robot détecte une ligne droite il va aller tout droit et s'ajuster selon ce qu'il capte
		case LIGNE:
		case LIGNE_GAUCHE:
		case LIGNE_DROITE:
			controleurRoues.avancer(); // Avancer avec ajustement
			break;
		//Si le robot détecte un coin, il doit avancer sans s'ajuster à celui-ci
		case GAUCHE:
		case EXTREMITE_GAUCHE:
		case DROITE:
		case EXTREMITE_DROITE:
			controleurRoues.avancer(); //Avancer sans ajustement
			
		break;
		case COIN_GAUCHE:
		case GAUCHE_ERRONEE:
			switch(pileEtatsCapteur.obtenirDessousSommet().segment)
			{	//Pour éviter que le robot interprête le «T» comme un coin, en y sortant
				case T_COMPLET:
				case T_INCOMPLET:
					// Le coin gauche n'est pas immédiatement après le «T».
				break;
				default:
					if (chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_SUR_T))
						detecterDroite3ou4();
				break;
			}
		break;
		case COIN_DROIT:
		case DROITE_ERRONEE:
			switch(pileEtatsCapteur.obtenirDessousSommet().segment)
			{	//Pour éviter que le robot interprête le «T» comme un coin, en y sortant
				case T_COMPLET:
				case T_INCOMPLET:
					// Le coin droit n'est pas immédiatement après le «T».
				break;
				default:
					if (chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_SUR_T))
						detecterDroite1ou2();
				break;
			}
		break;
			
		case T_COMPLET: // La détection du T indique le début de la détection des coins pour identifier la droite
			chronometre.reinitialiser(MINUTERIE_GENERAL);
			chronometre.activer(MINUTERIE_GENERAL);
			//compteur = 0;// Ce compteur permet de distinguer les droite 1 et 2 ainsi que 3 et 4 en fonction du temps de détection du dernier coin //A VOIR! Pas défini
			debutEnregistrement = true;
			miseJourDEL(VERT);
			controleurRoues.avancerToutDroit(); 
		break;
		case T_INCOMPLET:
			controleurRoues.avancerToutDroit(); 
		break;
		default:
			// Pas d'autres cas à considérer
		break;
	}
	
	//Tant que la droite n'est pas identifiée la fonction retourne 0
	return droiteDetectee;
	
}
 

