////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de gérer l'exécution du défi des neufs points
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "NeufPoints.h"

NeufPoints::NeufPoints(ControleurRoues& unControleurRoues, AutoPilote& unAutopilote, Son& son): son(son), controleurRoues(unControleurRoues), autopilote(unAutopilote) {}

//Identifie sur quelle ligne se trouve le point voulu parmis 3, 6 et 9
SectionPoint NeufPoints::trouverSection(uint8_t point) const {
	
	if (point <= 3) {
		return SECTION3;
	}
	
	else if (point <= 6)
		return SECTION6;
	
	else
		return SECTION9;	
	
}

//Pour avancer pendant un certain délais
void NeufPoints::avancer(uint16_t delais) const {
	controleurRoues.avancerToutDroitRapidement();
	delaisVariableMS(delais);
	controleurRoues.eteindreMoteurs();
	
}

//Pour tourner de 90 degrés vers la gauche
void NeufPoints::tournerGauche90() const {
	
	controleurRoues.tournerGaucheRapidement();
	delaisVariableMS(TEMPS_TOUR_90_DEGRES_G);
	controleurRoues.eteindreMoteurs();	
	
}


//Pour tourner de 90 degrés vers la droite
void NeufPoints::tournerDroite90() const{
	
	controleurRoues.tournerDroiteRapidement();
	delaisVariableMS(TEMPS_TOUR_90_DEGRES_D);
	controleurRoues.eteindreMoteurs();	
	
}

//Permet de faire des délais avec des variables
void NeufPoints::delaisVariableMS(uint16_t delais) const {
	
	for (uint16_t i = 0; i < delais; i++)
		_delay_ms(1);
	
}

//Calcule quel facteur mutilplier la constante TEMPS_ENTRE_POINT, pour diriger le robot à la bonne ligne avant de tourner à gauche et d'avancer sur le point
uint16_t NeufPoints::nEntrePointsParalleles(SectionPoint section) const {
	
	const uint8_t NOMBRE_SECTION = 3;
	return NOMBRE_SECTION - section/NOMBRE_SECTION;
}

//Pour retrouver le parcours
bool NeufPoints::attendreEtVerifierLigneTrouve(uint8_t temps)
{
	autopilote.miseAJourDesCapteurs();
	
	const uint8_t TEMPS_AVANT_DETECTION = 50;
	
	for(int i=0; i<temps; i++)
	{
		autopilote.miseAJourDesCapteurs();
		
		//Retourne vrai si une ligne est détectée par n'importe quel capteur
		if(autopilote.getSegmentCourant() != RIEN)
			return true;
		
		_delay_ms(TEMPS_AVANT_DETECTION);
	}
	
	return false;
}

//Pour retrouver le parcours
void NeufPoints::retrouverParcours() {

	autopilote.miseAJourDesCapteurs();
	
	const uint8_t TEMPS_AVANCER_INITIAL = 20;
	const uint8_t TEMPS_AVANCER = 20;
	const uint8_t TEMPS_BALAYAGE = 10;
	
	controleurRoues.avancerToutDroit();
	if (attendreEtVerifierLigneTrouve(TEMPS_AVANCER_INITIAL))
		return;
			
	// Ici on peut sortir du while avec les conditions à l'intérieur
	// Toute cette série d'instructions sert à balayer pour chercher le segment S3
	while(true) {
		
		controleurRoues.tournerDroite();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
		
		controleurRoues.tournerGauche();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
		
		controleurRoues.eteindreMoteurs();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
			
		controleurRoues.avancerToutDroit();
		if (attendreEtVerifierLigneTrouve(TEMPS_AVANCER))
			return;
		
		controleurRoues.tournerGauche();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
			
		controleurRoues.tournerDroite();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
			
		controleurRoues.eteindreMoteurs();
		if (attendreEtVerifierLigneTrouve(TEMPS_BALAYAGE))
			return;
			
		controleurRoues.avancerToutDroit();
		if (attendreEtVerifierLigneTrouve(TEMPS_AVANCER))
			return;
	}


}

//Emission d'un son grave pour 3 secondes
void NeufPoints::emettreLeSon() {
	
	son.emettre(NOTE_GRAVE, Chronometre::TROIS_SECONDES);
	_delay_ms(TROIS_SECONDES_MS);
	son.arreterSon();
	
}

//Méthodde pour se positioner sur le point et retrouver le parcours par la suite
void NeufPoints::deplacerVersPointEtRetour(uint8_t point){
	
	//Initialisation en fonction du point
	SectionPoint section = trouverSection(point);
	const uint16_t ATTENTE_SECONDE = 1000;
	uint16_t tempsDeplacementVersLAvant = POINT_PARCOURS_PARALLELE + TEMPS_ENTRE_POINT * nEntrePointsParalleles(section);
	uint16_t tempsDeplacementVersCote = POINT_PARCOURS_PERPENDICULAIRE + TEMPS_ENTRE_POINT * (section - point);
	
	
	//Déplacement vers point
	avancer(tempsDeplacementVersLAvant);
	_delay_ms(ATTENTE_SECONDE);
	
	tournerGauche90(); 
	_delay_ms(ATTENTE_SECONDE);
	avancer(tempsDeplacementVersCote);
	_delay_ms(ATTENTE_SECONDE);
			
	tournerDroite90();
	
	//émettre son grave 3 secondes
	emettreLeSon();
	
	tournerDroite90();
	
	//émettre son grave 3 secondes
	emettreLeSon();
	
	//Retour sur le parcours
	avancer(tempsDeplacementVersCote);
	_delay_ms(ATTENTE_SECONDE);
	
	tournerGauche90(); 
	_delay_ms(ATTENTE_SECONDE);
	retrouverParcours();
}	
