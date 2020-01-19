////////////////////////////////////////////////////////////////////////
/*
 * Description:
 * Ceci est le programme principal du robot devant suivre le trajet
 * Principalement, on y retrouve une machine à état pour le fonctionnement du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 18 Mars 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "source/inclusionsPremierRobot.h"

enum Section
{
	C1 = 1,
	C2 = 2,
	C3 = 3,
	C4 = 4,
	EXECUTION_NEUF_POINTS,
	DETECTION_DROITE,
	RETOUR_DETECTION_DROITE,
	RECTANGLE
};

uint8_t reception() {
	
	DDRD &= ~(0x01);
	
	RecepteurInfrarouge recepteur;
	
	return recepteur.receptionInfrarouge();
			
}

int main()
{	
	//--Initialisations--//
	initialisationSuiveurDeLigne();
	initialisationUART();
	initialisation5DELS();
	
	// Objets
	AutoPilote autopilote;
	ControleurRoues controleurRoues;
	Chronometre& chronometre = autopilote.getChronometre();
	Son son(chronometre);
	NeufPoints neufPoints = NeufPoints(autopilote.getControleurRoues(), autopilote, son);
	
	// Variables pour naviguer dans la machine
	const uint8_t NOMBRE_SECTIONS = 4;	
	uint8_t compteurSection = 0;
	Section sectionCourante;
	
	// Variables utiles à certains défis
	bool estDansRectangle = false;
	bool neufPointsFini = false;
	
	//--Reception État--//
	
	uint8_t sectionInitiale = reception();
	
	while (sectionInitiale > NOMBRE_SECTIONS ) //Réception recommencée tant que la valeur recue n'est pas entre 1 et 4
		sectionInitiale = reception();
	
	sectionCourante = (Section)sectionInitiale; //
	allumerDELParNumero(sectionInitiale);
	
	_delay_ms(DEUX_SECONDES_MS);
	
	//--Machine à état du parcours--//
	while (compteurSection < NOMBRE_SECTIONS) {
		
		///////////////////////////////
		// Mises à jour fréquentes du système de son (voir Classe Son)
		son.miseAJour();
		///////////////////////////////
		
		switch (sectionCourante) {
			
			case C1:
				autopilote.suivreLaLigne();	
				
				//Souvent le robot arrive au «T» avec un certain angle, il détecte donc parfois un coin coin gauche, on s'assure ici qu'il interprete le T correctement
				if ((autopilote.getSegmentCourant() == T_COMPLET || autopilote.getSegmentCourant() == COIN_GAUCHE) && !neufPointsFini) {
					
					neufPointsFini = true;
					chronometre.activer(MINUTERIE_GENERAL);
					chronometre.reinitialiser(MINUTERIE_GENERAL); // Nécessaires pour le prochain état
					sectionCourante = EXECUTION_NEUF_POINTS;
				}
				else if (autopilote.getSegmentCourant() == COIN_GAUCHE) {
					
					sectionCourante = C2;
					compteurSection++;
					
					if (compteurSection < NOMBRE_SECTIONS)
						autopilote.tournerCoinGauche();
				}
				break;
			
			case EXECUTION_NEUF_POINTS:
				if (autopilote.alignerSelonUnT()) // enligne le robot sur le T (S2)
				{
						//RECEPTION INFRAROUGE
						uint8_t numeroPoint = reception();
						neufPoints.deplacerVersPointEtRetour(numeroPoint);
						sectionCourante = C1;
				}
				break;
				
			case C2:
				autopilote.suivreLaLigneC2C3();	
				
				if (autopilote.getSegmentCourant() == COIN_GAUCHE) {
					
					sectionCourante = C3;
					compteurSection++;
					
					if (compteurSection < NOMBRE_SECTIONS)
						autopilote.tournerCoinGauche();
				}
				break;	
								
			case C3:
				autopilote.suivreLaLigne();	
				
				//Souvent le robot arrive au «T» avec un certain angle, il détecte donc parfois un coin coin gauche, on s'assur ici qu'il interprete le T correctement
				if (autopilote.getSegmentCourant() == T_COMPLET || autopilote.getSegmentCourant() == COIN_GAUCHE) {
					
					autopilote.eteindreMoteurs();
					
					//Attente de l'appuis du bouton pour avancer et détecter la droite
					while (!autopilote.getControleurInterruptions().boutonEstAppuye()) {} 
					
					_delay_ms(UNE_SECONDE_MS);
					sectionCourante = DETECTION_DROITE;
					chronometre.eteindre(MINUTERIE_GENERAL);				
				}
				break;
				
			case DETECTION_DROITE:			
			{	
				uint8_t droite = autopilote.suivreDroiteADectecter();
				
				if (droite != 0) {
					
					autopilote.eteindreMoteurs();
					
					allumerDELParNumero(droite);
					
					//Attente de l'appuis du bouton pour continuer à suivre la droite
					while (!autopilote.getControleurInterruptions().boutonEstAppuye()) {}
					
					_delay_ms(UNE_SECONDE_MS);
					miseJourDEL(ETEINT);
					sectionCourante = RETOUR_DETECTION_DROITE;
				}
			}
				break;
				
			case RETOUR_DETECTION_DROITE:
				autopilote.suivreLaLigne();				
				
				if (autopilote.getSegmentCourant() == COIN_GAUCHE) {
					
					miseJourDEL(ETEINT);
					sectionCourante = C4;
					compteurSection++;
					chronometre.eteindre(MINUTERIE_GENERAL);
					chronometre.activer(MINUTERIE_GENERAL); // Utile pour l'état C4
					
					if (compteurSection < NOMBRE_SECTIONS)
						autopilote.tournerCoinGauche();
				}
				break;
						
			case C4:
				miseJourDEL(VERT);
				chronometre.activer(MINUTERIE_GENERAL);
				
				if (chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_LARGEUR_RUBAN))
					autopilote.suivreLaLigne();
				else
					autopilote.allerToutDroit(); // Éviter des problèmes de transition entre suivreLaLigne et les rectangles
				
				// On peut entrer seulement dans les rectangles si un certain temps s'est écoulé
				if (autopilote.getSegmentCourant() == T_COMPLET && chronometre.verifierTempsAtteint(MINUTERIE_GENERAL, TEMPS_LARGEUR_RUBAN) ) {	
					
					estDansRectangle = true;
					chronometre.reinitialiser(MINUTERIE_GENERAL);
					
					son.emettre(NOTE_AIGU, DUREE_SON_RECTANGLE, DUREE_SON_RECTANGLE_SILENCE, REPETITIONS_SON_RECTANGLE);
					
					sectionCourante = RECTANGLE;	
				}
				
				// Si tous les rectangles ont été détectés, on vérifie si on doit aller en C1
				if (autopilote.getSegmentCourant() == COIN_GAUCHE && autopilote.tousRectanglesCompletes()) {
					
					sectionCourante = C1;
					compteurSection++;
					chronometre.eteindre(MINUTERIE_GENERAL);
					
					if (compteurSection < NOMBRE_SECTIONS)
						autopilote.tournerCoinGaucheC1();
				}
				break;
				
			case RECTANGLE:
				
				miseJourDEL(ROUGE);
				autopilote.suivreLaLigneAvecRectangles(estDansRectangle);
				
				// On doit sortir de l'état rectangle
				if (!estDansRectangle)
				{
					son.emettre(NOTE_AIGU, DUREE_SON_RECTANGLE, DUREE_SON_RECTANGLE_SILENCE, REPETITIONS_SON_RECTANGLE);
					chronometre.reinitialiser(MINUTERIE_GENERAL);
					autopilote.unRectangleComplete();
					sectionCourante = C4;
				}
				break;
						
		}
	
	}
	
	// Dernière action du programme du robot
	autopilote.eteindreMoteurs();
	son.emettre(NOTE_AIGU, DEUX_SECONDES_MS);
	_delay_ms(DEUX_SECONDES_MS);
	son.arreterSon();
	
	return 0;
}
