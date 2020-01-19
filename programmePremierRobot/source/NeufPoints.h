////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet de gérer l'exécution du défi des neufs points
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef NEUF_POINTS_H
#define NEUF_POINTS_H

#include "AutoPilote.h"
#include "ControleurRoues.h"

//Pour différencier les lignes 3, 6 et 9 du carré de points
enum SectionPoint {SECTION3 = 3, SECTION6 = 6, SECTION9 = 9};

class NeufPoints
{
public:
	NeufPoints(ControleurRoues& unControleurRoues, AutoPilote& unAutopilote, Son& son);
	
	//Méthodde pour se positioner sur le point et retrouver le parcours par la suite
	void deplacerVersPointEtRetour(uint8_t point) ;
	
	//Pour avancer pendant un certain délais
	void avancer(uint16_t delais) const ;
	
	//Pour tourner de 90 degrés
	void tournerDroite90() const;
	void tournerGauche90() const;
	
	//Pour retrouver le parcours
	bool attendreEtVerifierLigneTrouve(uint8_t temps);
	void retrouverParcours();
	
	//Emission d'un son grave pour 3 secondes
	void emettreLeSon();

private:	

	//Permet de faire des délais avec des variables
	void delaisVariableMS(uint16_t delais) const;
	
	//Identifie sur quelle ligne se trouve le point voulu parmis 3, 6 et 9
	SectionPoint trouverSection(uint8_t point) const;
	
	//Calcule quel facteur mutilplier la constante TEMPS_ENTRE_POINT, pour diriger le robot à la bonne ligne avant de tourner à gauche et d'avancer sur le point
	uint16_t nEntrePointsParalleles(SectionPoint section) const; 
	Son& son;

	//Ces constantes correspondent au temps que prend le robot pour parcourir certaines distances clés, elles ont été obtenues expérimentalement
	const uint16_t TEMPS_ENTRE_POINT = 400; //Entre 2 points
	const uint16_t POINT_PARCOURS_PERPENDICULAIRE = 1200; //Entre le parcours et la première colonne de points (perpendiculaire au parcours)
	const uint16_t POINT_PARCOURS_PARALLELE = 1750; //Entre le parcours et la première ligne de points (parallèle au parcours)
	//Pour tourner de 90 degrés (Valeurs différentes pour gauche et droite)
	const uint16_t TEMPS_TOUR_90_DEGRES_D = 720;
	const uint16_t TEMPS_TOUR_90_DEGRES_G = 710;
	
	//Pour controler les roues
	ControleurRoues& controleurRoues;
	AutoPilote& autopilote;
	
	
};

#endif /* NEUF_POINTS_H */	
