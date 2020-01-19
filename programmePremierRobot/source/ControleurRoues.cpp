////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe permet d'automatiser plusieurs actions liées aux moteurs du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "ControleurRoues.h"

ControleurRoues::ControleurRoues()
{
	//initialise les registre ainsi que les ports nécessaire pour utiliser le PMW du timer0
	initialisationPWM_timer0();
	
	//Les roues sont initialement arrêté
	pwmRoueGauche = ARRET;
	pwmRoueDroite = ARRET;
	
	ajustementRoueGauche = 0; // Pas d'ajustements au début
	ajustementRoueDroite = 0; // Pas d'ajustements au début
	
	//Les roues sont initialement vers l'avant
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	
	facteurDynamique = 0;
}
//Permet au robot d'avancer avec des ajustements utiliser pour le segment du parcours entre C2 et C3
void ControleurRoues::avancerC2C3()
{
	//La vitesse est initialement à normal vers l'avant
	pwmRoueGauche = VITESSE_NORMALE;
	pwmRoueDroite = VITESSE_NORMALE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//si nous nous trouvons dans l'état AU_CENTRE notre vitesse est plus élevé 
	//Cela permet de ralentir dans les courbes
	if (positionCapteur == AU_CENTRE)
	{
		pwmRoueGauche = VITESSE_ELEVE;
		pwmRoueDroite = VITESSE_ELEVE;
	}
	
	//Notre vitesse va subir des ajustements dynamique particulier au segment entre C2 et C3
	etat = AVANCER_AVEC_AJUSTEMENTS_C2_C3;
	mettreMarche();
}

//Permet au robot d'avancer avec des ajustement
void ControleurRoues::avancer()
{
	//La vitesse est initialement à normal vers l'avant
	pwmRoueGauche = VITESSE_NORMALE;
	pwmRoueDroite = VITESSE_NORMALE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse va subir des ajustements dynamique
	etat = AVANCER_AVEC_AJUSTEMENTS;
	mettreMarche();
}

//Permet au robot de dévier vers la gauche
void ControleurRoues::devierGauche()
{
	//La vitesse est plus élevé sur la roue droite pour que le robot se dirige vers la gauche
	pwmRoueGauche = VITESSE_NORMALE;
	pwmRoueDroite = VITESSE_ELEVE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de dévier vers la droite
void ControleurRoues::devierDroite()
{
	//La vitesse est plus élevé sur la roue gauche pour que le robot se dirige vers la droite
	pwmRoueGauche = VITESSE_ELEVE;
	pwmRoueDroite = VITESSE_NORMALE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot d'avancer sans ajustement dynamique
void ControleurRoues::avancerToutDroit()
{
	//La vitesse est initialement à normal vers l'avant
	pwmRoueGauche = VITESSE_NORMALE;
	pwmRoueDroite = VITESSE_NORMALE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot d'avancer sans ajustement dynamique plus rapidement
void ControleurRoues::avancerToutDroitRapidement() 
{
	
	//La vitesse est initialement élevée vers l'avant
	pwmRoueGauche = VITESSE_ELEVE;
	pwmRoueDroite = VITESSE_ELEVE;
	directionRoueGauche = AVANCER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de tourner sur lui même vers la gauche à vitesse élevée
void ControleurRoues::tournerGaucheRapidement()
{
	//La vitesse est initialement élevée
	pwmRoueGauche = VITESSE_ELEVE;
	pwmRoueDroite = VITESSE_ELEVE;
	//La roue gauche se dirige vers l'arrière et la droite vers l'avant à vitesse constante
	//pour que le robot pivote sur lui même vers la gauche
	directionRoueGauche = RECULER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de tourner sur lui même vers la gauche
void ControleurRoues::tournerGauche()
{
	//La vitesse est initialement à une vitesse différente pour favoriser la rotation
	pwmRoueGauche = VITESSE_ROTATION;
	pwmRoueDroite = VITESSE_ROTATION;
	//La roue gauche se dirige vers l'arrière et la droite vers l'avant à vitesse constante
	//pour que le robot pivote sur lui même vers la gauche
	directionRoueGauche = RECULER;
	directionRoueDroite = AVANCER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de tourner sur lui même vers la droite à vitesse élevée
void ControleurRoues::tournerDroiteRapidement()
{
	//La vitesse est initialement élevée
	pwmRoueGauche = VITESSE_ELEVE;
	pwmRoueDroite = VITESSE_ELEVE;
	//La roue droite se dirige vers l'arrière et la gauche vers l'avant à vitesse constante
	//pour que le robot pivote sur lui même vers la droite
	directionRoueGauche = AVANCER;
	directionRoueDroite = RECULER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de tourner sur lui même vers la droite
void ControleurRoues::tournerDroite()
{
	//La vitesse est initialement à une vitesse différente pour favoriser la rotation
	pwmRoueGauche = VITESSE_ROTATION;
	pwmRoueDroite = VITESSE_ROTATION;
	//La roue droite se dirige vers l'arrière et la gauche vers l'avant à vitesse constante
	//pour que le robot pivote sur lui même vers la droite
	directionRoueGauche = AVANCER;
	directionRoueDroite = RECULER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Permet au robot de reculer
void ControleurRoues::reculer()
{
	//La vitesse est initialement basse vers l'arrière
	pwmRoueGauche = VITESSE_RALENTIE;
	pwmRoueDroite = VITESSE_RALENTIE;
	directionRoueGauche = RECULER;
	directionRoueDroite = RECULER;
	
	//Notre vitesse ne subira aucun ajustement dynamique
	etat = AUCUN_AJUSTEMENT;
	mettreMarche();
}

//Met à jour le segment, l'ajustement et le facteur dynamique
void ControleurRoues::miseAJour(PileEtatsCapteur& pileEtatsCapteur, Chronometre& chronometre)
{
	//Vérifie sur quel segment nous nous trouvons et appplique les changements 
	//nécessaire au parcours du robot dépendamment de ce dernier
	miseAJourPositionCapteur(pileEtatsCapteur, chronometre);
	
	//Met à jour l'ajustement du robot selon les changements exécuté plus haut
	miseAJourEtat(pileEtatsCapteur);
	
	//Met à jour le facteur dynamique du robot
	miseAJourFacteurDynamique(pileEtatsCapteur);
}

//Donne la vitesse au robot selon les différents ajustement
void ControleurRoues::mettreMarche()
{
	//Variable locale de la fonction
	uint8_t ralentissement = (ajustementRoueDroite + ajustementRoueGauche); // variable pour ralentir le robot
	int8_t differenceVitesseRoueGauche = ajustementRoueGauche;				//variable locale pour l'ajustement de la roue gauche	
	int8_t differenceVitesseRoueDroite = ajustementRoueDroite;				//variable locale pour l'ajustement de la roue droite
	
	//Si le robot n'a aucun ajustement nous mettons l'ajustement local à 0
	if (etat == AUCUN_AJUSTEMENT)
	{
		differenceVitesseRoueGauche = 0;
		differenceVitesseRoueDroite = 0;
	}
	
	//Si nous sommes au centre ou sans ajustement, nous ne ralentissons pas le robot
	//Aide à ralentir dans les courbes
	if (positionCapteur == AU_CENTRE || etat == AUCUN_AJUSTEMENT)
		ralentissement = 0;
	
	//Donne une vitesse à chacune des roues de notre robot
	changerSignalPWM_timer0(
		pwmRoueDroite + differenceVitesseRoueDroite - ralentissement,
		pwmRoueGauche + differenceVitesseRoueGauche - ralentissement);
	
	//applique la direction calculé précedemment sur les roues
	ajusterSens_roues(directionRoueGauche, directionRoueDroite);
}

// Pour arrêter les moteurs
void ControleurRoues::eteindreMoteurs()
{
	changerSignalPWM_timer0(ARRET, ARRET);
}

// Configure le signal PWM sur le timer0 pour les roues
void ControleurRoues::initialisationPWM_timer0(void)
{
	// Empêcher les interruptions
	cli ();

	// Sorties
	DDRB |= 0x3C; // PORT B est en mode sortie pour l utilisation de B4 et B5 pour la vitesse et B3 et B6 pour le sens de rotation
	
	
	// Configurer le signal PWM
	TCNT0 = 0x0;
	
	OCR0A = ARRET;
	OCR0B = ARRET;
	
	// Desactive OC1A
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);

	// Desactive OC1B et divise la frequence d horloge par 8
	TCCR0B = (1 << CS01);
	
	// sei permet de recevoir à nouveau des interruptions.
	sei();

}

// Ajuster le sens de rotation des roues, vue de dos
void ControleurRoues::ajusterSens_roues( bool directionGauche, bool directionDroite)
{	
	PORTB &= ~( (1 << DECALAGE_B3) | (1 << DECALAGE_B6) );// Mettre B3 et B6 à 0 pour rénitialiser
	PORTB |= (directionDroite << DECALAGE_B3) | (directionGauche << DECALAGE_B6);// Ajuster B3 et B6 pour avancer ou reculer
}

//Met des poids sur les différents segment de notre roue gauche
int8_t ControleurRoues::poidsGaucheDuSegment(SegmentDetecte& segment)
{
	switch (segment)
	{
		case LIGNE_GAUCHE:
			return AJUSTEMENT_LEGER;
		break;
		case GAUCHE:
			return AJUSTEMENT_MODERE;
		break;
		case EXTREMITE_GAUCHE_GAUCHE:
			return AJUSTEMENT_ELEVE;
		break;
		case EXTREMITE_GAUCHE:
			return AJUSTEMENT_MAXIMAL;
		break;
		case COIN_GAUCHE:
			return AJUSTEMENT_MAXIMAL;
		break;
		
		case LIGNE:
			return AJUSTEMENT_AUCUN;
		break;
		
		default:
			return AJUSTEMENT_AUCUN;
		break;
	}
}

//Met des poids sur les différents segment de notre roue droite
int8_t ControleurRoues::poidsDroitDuSegment(SegmentDetecte& segment)
{
	switch (segment)
	{
		case LIGNE_DROITE:
			return AJUSTEMENT_LEGER;
		break;
		case DROITE:
			return AJUSTEMENT_MODERE;
		break;
		case EXTREMITE_DROITE_DROITE:
			return AJUSTEMENT_ELEVE;
		break;
		case EXTREMITE_DROITE:
			return AJUSTEMENT_MAXIMAL;
		break;
		case COIN_DROIT:
			return AJUSTEMENT_MAXIMAL;
		break;
		
		case LIGNE:
			return AJUSTEMENT_AUCUN;
		break;
		
		default:
			return AJUSTEMENT_AUCUN;
		break;
	}
}

//Retourne notre ajustement maximal que le robot tentera d'atteindre dynamiquement
int8_t ControleurRoues::calculerCible(int8_t poidsSegmentCourant, int8_t poidsSegmentPrecedent)
{
	//variable locale 
	int8_t cible = poidsSegmentCourant;
	const int8_t FACTEUR_DYNAMIQUE = 25 + facteurDynamique / 10; // Permet de favoriser la correction de la trajectoire
	
	//indique que le robot ne se dirige pas vers le bon endroit
	if (poidsSegmentCourant > poidsSegmentPrecedent)
		//Nous augmentons la cible pour que le robot tende plus vite vers le bon endroit
		cible += FACTEUR_DYNAMIQUE;
	//indique que le robot était sur un état stable
	else if (poidsSegmentPrecedent == 0)
		//Nous réduisons la cible pour qu'il ne s'ajuste pas trop sec
		cible -= FACTEUR_DYNAMIQUE / 2;
	
	return cible;
}

//Fait tendre l'ajustement vers la cible
void ControleurRoues::tendreAjustementVers(int8_t& ajustement, int8_t cible)
{
	// Accélère l'ajustement
	ajustement += (cible - ajustement) / 6; // Valeur expérimentale pour effectuer les changements plus rapidement
	
	//Augmenter l'ajustement pour qu'il se rapproche de la cible
	if (ajustement < cible)
	{
		++ajustement;
	}
	//Diminuer l'ajustement pour qu'il se rapproche de la cible
	else if (ajustement > cible)
	{
		--ajustement;
	}
}

//Met à jour l'état PositionCapteur du robot selon le segment détecté
void ControleurRoues::miseAJourPositionCapteur(PileEtatsCapteur& pileEtatsCapteur, Chronometre& chronometre)
{
	chronometre.activer(MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR); // Cette minuterie doit être active pour le bon ajustement de positionCapteur
	
	PositionCapteur anciennePositionCapteur = positionCapteur;
	
	const uint16_t COMPTEUR_MEME_POSTION_MAX = 600; // Correspond à 3 secondes. On ne veut pas que le compteur augmente plus pour les formules en dessous
	
	//Plus le compteur reste dans un état, plus il est dur de quitter celui ci
	const int16_t TEMPS_POUR_DEVENIR_CENTRE = 100+compteurMemePositionCapteur/2;
	const int16_t TEMPS_POUR_DEVENIR_GAUCHE_OU_DROITE = 30+compteurMemePositionCapteur/9;
	
	
	switch(pileEtatsCapteur.obtenirSommet().segment)
	{
		//Si le robot reste sur la ligne pendant TEMPS_POUR_DEVENIR_CENTRE l'état du robot changera pour AU_CENTRE
		case LIGNE:
			if (chronometre.verifierTempsAtteint(MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR, TEMPS_POUR_DEVENIR_CENTRE))
				positionCapteur = AU_CENTRE;
		break;
		
		//Si le robot reste sur la ligne pendant TEMPS_POUR_DEVENIR_GAUCHE_OU_DROITE l'état du robot changera pour A_GAUCHE
		case LIGNE_GAUCHE:
		case GAUCHE:
			if (chronometre.verifierTempsAtteint(MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR, TEMPS_POUR_DEVENIR_GAUCHE_OU_DROITE))
				positionCapteur = A_GAUCHE;
		break;
		//Si le robot est sur une des extrémités l'etat du robot changera pour A_GAUCHE
		case EXTREMITE_GAUCHE:
		case EXTREMITE_GAUCHE_GAUCHE:
			positionCapteur = A_GAUCHE;
		break;
		//Si le robot reste sur la ligne pendant TEMPS_POUR_DEVENIR_GAUCHE_OU_DROITE l'état du robot changera pour A_DROITE
		case LIGNE_DROITE:
		case DROITE:
			if (chronometre.verifierTempsAtteint(MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR, TEMPS_POUR_DEVENIR_GAUCHE_OU_DROITE))
				positionCapteur = A_DROITE;
		break;
		//Si le robot est sur une des extrémités l'etat du robot changera pour A_DROITE
		case EXTREMITE_DROITE:
		case EXTREMITE_DROITE_DROITE:
			positionCapteur = A_DROITE;
		break;
		
		default:
			// Pas d'autres cas à considérer
		break;
	}
	
	//Le chronometre est reinitialiser a chaque changement de sommet sur la pile
	if (pileEtatsCapteur.sommetAChange())
	{
		chronometre.reinitialiser(MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR);
	}
	
	//Nous réinitialisons notre compteur si la position n'est pas la même que la précédente
	if (anciennePositionCapteur != positionCapteur)
	{
		compteurMemePositionCapteur = 0;
	}
	//Si la position est la même nous incrémentons notre compteur
	if (compteurMemePositionCapteur < COMPTEUR_MEME_POSTION_MAX)
	{
		++compteurMemePositionCapteur;
	}
}

//Met à jour les ajustements selon le segment où se situe le robot
void ControleurRoues::miseAJourEtat(PileEtatsCapteur& pileEtatsCapteur)
{
	switch (etat)
	{
	case AUCUN_AJUSTEMENT:
		ajustementRoueGauche = AJUSTEMENT_AUCUN;
		ajustementRoueDroite = AJUSTEMENT_AUCUN;
	break;
	case AVANCER_AVEC_AJUSTEMENTS:
	case AVANCER_AVEC_AJUSTEMENTS_C2_C3:
		miseAJourAvancer(pileEtatsCapteur);
	break;
	default:
		// Pas d'autres cas à considérer
	break;
	}
}

//ajoute un facteur dynamique maximal lors d'un changement de sommet et ralentie le robot s'il a déja un facteur dynamique
void ControleurRoues::miseAJourFacteurDynamique(PileEtatsCapteur& pileEtatsCapteur)
{
	// Permet au prochain changement d'état de faire un virage plus sec, s'il survient rapidement.
	if (facteurDynamique > 0)
		--facteurDynamique;
	if (pileEtatsCapteur.sommetAChange())
	{
		facteurDynamique = 255;
	}
}

//Met à jour la cible que l'ajustement veut atteindre et fait tendre l'ajustement vers cette cible
void ControleurRoues::miseAJourAvancer(PileEtatsCapteur& pileEtatsCapteur)
{
	
	SegmentDetecte segmentActuel = pileEtatsCapteur.obtenirSommet().segment;
	SegmentDetecte segmentPrecedent = pileEtatsCapteur.obtenirDessousSommet().segment;
	
	// Pour dévier à gauche, la roue droite doit aller plus vite
	int8_t cibleGauche = calculerCible(
		poidsDroitDuSegment(segmentActuel),
		poidsDroitDuSegment(segmentPrecedent)
	);
	
	// Pour dévier à droite, la roue gauche doit aller plus vite
	int8_t cibleDroite = calculerCible(
		poidsGaucheDuSegment(segmentActuel),
		poidsGaucheDuSegment(segmentPrecedent)
	);
	
	tendreAjustementVers(ajustementRoueGauche, cibleGauche);
	tendreAjustementVers(ajustementRoueDroite, cibleDroite);
}
