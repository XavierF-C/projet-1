////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe gère le capteur du premier robot
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef CAPTEUR_DE_LIGNE_H
#define CAPTEUR_DE_LIGNE_H

// Permettre de recueillir les données du capteur de lignes
class CapteurDeLigne
{
public:
	
	// Recueillir les données des 5 capteurs
	void miseAJour();
	
	// Obtenir les données des 5 capteurs compressées en un uint8_t
	uint8_t etatCapteur(uint8_t donneesPrecedentes);

private:

	uint8_t donneesImmediates[NOMBRE_CAPTEURS_IR];
	can convertisseur;
	//Seuil calculé expérimentalement
	const uint8_t SEUIL_BLANC = 110;
	const uint8_t SEUIL_NOIR = 70;
};

#endif /* CAPTEUR_DE_LIGNE_H */
