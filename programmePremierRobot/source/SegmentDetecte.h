////////////////////////////////////////////////////////////////////////
/* Description :
 * Ce fichier permet la manipulation de données brutes du capteur frontal du robot.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef SEGMENT_DETECTE_H
#define SEGMENT_DETECTE_H

#include <librairie.h>

// 
enum SegmentDetecte
{
	// En faisant face au robot
	
	RIEN, // B B B B B
	LIGNE, // B B N B B
	
	GAUCHE, // B N B B B
	EXTREMITE_GAUCHE, // N B B B B
	LIGNE_GAUCHE, // B N N B B
	EXTREMITE_GAUCHE_GAUCHE, // N N B B B
	COIN_GAUCHE, // N N N B B
		
	DROITE, // B B B N B
	EXTREMITE_DROITE, // B B B B N
	LIGNE_DROITE, // B B N N B
	EXTREMITE_DROITE_DROITE, // B B B N N
	COIN_DROIT, // B B N N N
	
	GAUCHE_ERRONEE, // N X X N B
	DROITE_ERRONEE, // B N X X N
	
	T_COMPLET, // N N N N N
	T_INCOMPLET, // Au moins 1 Noir sur la gauche et au moins 1 Noir sur la droite
};

// Vérifie si le bit à la x dans donnees correspond à la couleur
static bool estMemeCouleur(CouleurCapteur couleur, uint8_t positionIRx, uint8_t donnees)
{
	// Comparer le bon bit de donnees avec la couleur
	return (couleur == ( (donnees) & (1 << positionIRx) ) >> positionIRx );
}

// Vérifie si les 5 bits dans donnees correspondend aux couleurs
static bool sontMemeCouleur(CouleurCapteur ir1, CouleurCapteur ir2, CouleurCapteur ir3 , CouleurCapteur ir4, CouleurCapteur ir5, uint8_t donnees)
{
	// Comparer le bon bit de donnees avec la couleur du capteur correspondante
		
	if (estMemeCouleur(ir1, IR1, donnees) &&
		estMemeCouleur(ir2, IR2, donnees) &&
		estMemeCouleur(ir3, IR3, donnees) &&
		estMemeCouleur(ir4, IR4, donnees) &&
		estMemeCouleur(ir5, IR5, donnees) )
		return true;
		
	return false;
}

// Ici, le mot clef inline permet la suppression du warning «defined but not used» puisqu'on utilise cette fonction!
//Identifier le segment selon la donnée brut fournis
static inline SegmentDetecte identifierSegment(uint8_t donnees)
{
	SegmentDetecte segment;
	
	if (sontMemeCouleur(BLANC, BLANC, NOIR, BLANC, BLANC, donnees))
		segment = LIGNE;
	else if (sontMemeCouleur(BLANC, BLANC, BLANC, BLANC, BLANC, donnees))
		segment = RIEN;
	// Partie gauche, vue de dos
	else if (sontMemeCouleur(BLANC, NOIR, BLANC, BLANC, BLANC, donnees))
		segment = GAUCHE;
	else if (sontMemeCouleur(NOIR, BLANC, BLANC, BLANC, BLANC, donnees))
		segment = EXTREMITE_GAUCHE;
	else if (sontMemeCouleur(BLANC, NOIR, NOIR, BLANC, BLANC, donnees))
		segment = LIGNE_GAUCHE;
	else if (sontMemeCouleur(NOIR, NOIR, BLANC, BLANC, BLANC, donnees))
		segment = EXTREMITE_GAUCHE_GAUCHE;
	else if (sontMemeCouleur(NOIR, NOIR, NOIR, BLANC, BLANC, donnees))
		segment = COIN_GAUCHE;
	// Partie droite, vue de dos
	else if (sontMemeCouleur(BLANC, BLANC, BLANC, NOIR, BLANC, donnees))
		segment = DROITE;
	else if (sontMemeCouleur(BLANC, BLANC, BLANC, BLANC, NOIR, donnees))
		segment = EXTREMITE_DROITE;
	else if (sontMemeCouleur(BLANC, BLANC, NOIR, NOIR, BLANC, donnees))
		segment = LIGNE_DROITE;
	else if (sontMemeCouleur(BLANC, BLANC, BLANC, NOIR, NOIR, donnees))
		segment = EXTREMITE_DROITE_DROITE;
	else if (sontMemeCouleur(BLANC, BLANC, NOIR, NOIR, NOIR, donnees))
		segment = COIN_DROIT;
	
	// Combinaisons particulières pouvant survenir proche des coins	
	else if (sontMemeCouleur(NOIR, BLANC, BLANC, NOIR, BLANC, donnees) ||
			sontMemeCouleur(NOIR, NOIR, BLANC, NOIR, BLANC, donnees) ||
			sontMemeCouleur(NOIR, BLANC, NOIR, NOIR, BLANC, donnees) ||
			sontMemeCouleur(NOIR, NOIR, NOIR, NOIR, BLANC, donnees)
		)
		segment = GAUCHE_ERRONEE;
	else if (sontMemeCouleur(BLANC, NOIR, BLANC, BLANC, NOIR, donnees) ||
			sontMemeCouleur(BLANC, NOIR, NOIR, BLANC, NOIR, donnees) ||
			sontMemeCouleur(BLANC, NOIR, BLANC, NOIR, NOIR, donnees) ||
			sontMemeCouleur(BLANC, NOIR, NOIR, NOIR, NOIR, donnees)
		)
		segment = DROITE_ERRONEE;
	
	// Ruban perpendiculaire aka un «T»
	else if (sontMemeCouleur(NOIR, NOIR, NOIR, NOIR, NOIR, donnees))
		segment = T_COMPLET;
	else
		segment = T_INCOMPLET; // Devrait presque toujours être un «T» avec des valeurs manquantes
	
	return segment;
}

#endif /* SEGMENT_DETECTE_H */
