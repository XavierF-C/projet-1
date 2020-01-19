/* ////////////////////////////////////////////////////////////////////////
 * Description:
 * Cette classe permet l'utilisation de plusieurs minuteries à partir d'une seule.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "Chronometre.h"

Chronometre::Chronometre()
{
	for (uint8_t i=0; i < NOMBRE_DE_MINUTERIES; ++i)
	{
		donneesMinuteries[i].compteur = 0;
		donneesMinuteries[i].active = false;
	}
}

// Quand le chronomètre est démarré, les minuteries actives sont mises à jour
void Chronometre::demarrerChronometre(ControleurInterruptions& controleurInterruptions)
{
	controleurInterruptions.partirMinuterie1_prescaler_1024(TEMPS_CYCLIQUE);
}

// Quand le chronomètre est arrêté, les minuteries sont figées et ne se mettent plus à jour
void Chronometre::arreterChronometre(ControleurInterruptions& controleurInterruptions)
{
	controleurInterruptions.fermerMinuterie1();
}

// Lorsque le chronomètre est mis à jour, le compteur de cette minuterie s'incrémente 
void Chronometre::activer(const Minuterie minuterie)
{
	donneesMinuteries[minuterie].active = true;
}

// Retourne vrai si le compteur de cette minuterie est >= temps, plusieurs constantes de temps sont dans le .h du chronomètre
bool Chronometre::verifierTempsAtteint(const Minuterie minuterie, uint16_t temps)
{
	if (donneesMinuteries[minuterie].compteur >= temps)
		return true;
		
	return false;
}

 // Retourne la valeur actuelle du compteur de la minuterie
uint16_t Chronometre::obtenirCompteur(const Minuterie minuterie)
{
	return donneesMinuteries[minuterie].compteur;
}

// Mets le compteur de cette minuterie à 0. Ne change rien d'autre
void Chronometre::reinitialiser(const Minuterie minuterie)
{
	donneesMinuteries[minuterie].compteur = 0;
}

// Empêche le compteur de cette minuterie de s'incrémenter et le remets à 0
void Chronometre::eteindre(const Minuterie minuterie)
{
	donneesMinuteries[minuterie].compteur = 0;
	donneesMinuteries[minuterie].active = false;
}

// Cette méthode permet de savoir si le temps cyclique de 5 ms s'est écoulé. Si c'est le cas, les compteurs des minuteries actives sont incrémentés
bool Chronometre::aEteMisAJour(ControleurInterruptions& controleurInterruptions)
{
	if(controleurInterruptions.minuterie1EstExpiree())
	{
		controleurInterruptions.partirMinuterie1_prescaler_1024(TEMPS_CYCLIQUE);
		
		for (uint8_t i=0; i < NOMBRE_DE_MINUTERIES; ++i)
		{
			if (donneesMinuteries[i].active)
				++donneesMinuteries[i].compteur; // Le compteur de cette minuterie s'incrémente à chaque 5 ms si la minuterie est activée
		}
		
		return true;
	}
	
	return false;
}
