////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe gère le capteur du premier robot
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <librairie.h>
#include "CapteurDeLigne.h"

// Recueillir les données des 5 capteurs
void CapteurDeLigne::miseAJour()
{
	// La classe can lit 10 bits, et on élimine donc les 2 bits les moins significatifs car ils ne sont pas utiles
	const uint8_t ELIMINER_2BITS = 2;
	
	donneesImmediates[IR1] = convertisseur.lecture(SUIVEUR_DELIGNE[IR1]) >> ELIMINER_2BITS;
	donneesImmediates[IR2] = convertisseur.lecture(SUIVEUR_DELIGNE[IR2]) >> ELIMINER_2BITS;
	donneesImmediates[IR3] = convertisseur.lecture(SUIVEUR_DELIGNE[IR3]) >> ELIMINER_2BITS;
	donneesImmediates[IR4] = convertisseur.lecture(SUIVEUR_DELIGNE[IR4]) >> ELIMINER_2BITS;
	donneesImmediates[IR5] = convertisseur.lecture(SUIVEUR_DELIGNE[IR5]) >> ELIMINER_2BITS;
}

// Obtenir les données des 5 capteurs compressées en un uint8_t
uint8_t CapteurDeLigne::etatCapteur(uint8_t donneesPrecedentes)
{
	// 1 représente blanc
	// 0 représente noir
	uint8_t etat = donneesPrecedentes;
	
	for(uint8_t i=0; i<NOMBRE_CAPTEURS_IR; ++i)
	{
		if(donneesImmediates[i] <= SEUIL_NOIR) // Le bit correspondant est mis NOIR;
		{
			etat &= ~(1 << i);
		}
		
		if(donneesImmediates[i] >= SEUIL_BLANC) // Le bit correspondant est mis BLANC;
		{
			etat |= (1 << i);
		}
	}
	
	return etat;
}
