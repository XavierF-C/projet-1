////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe permet l'utilisation de plusieurs minuteries à partir d'une seule.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H

#include <librairie.h>

// Représente la liste des minuteries utilisées
enum Minuterie
{
	MINUTERIE_GENERAL,
	MINUTERIE_CORRECTION,
	MINUTERIE_SON,
	MINUTERIE_IDENTIFIER_DROITE,
	MINUTERIE_SUIVEUR_DE_LIGNE_POSITION_CAPTEUR,
	/////////////////////////////////////////////
	NOMBRE_DE_MINUTERIES // TOUJOURS EN DERNIER
	/////////////////////////////////////////////
};
// Il faut ABSOLUMENT que NOMBRE_DE_MINUTERIES soit en dernier.

class Chronometre
{
public:
	
	Chronometre();
	
	// Quand le chronomètre est démarré, les minuteries actives sont mises à jour
	void demarrerChronometre(ControleurInterruptions& controleurInterruptions);
	// Quand le chronomètre est arrêté, les minuteries sont figées et ne se mettent plus à jour
	void arreterChronometre(ControleurInterruptions& controleurInterruptions);
	
	
	// Ces méthodes permettent l'utilisation d'une minuterie
	void activer(const Minuterie minuterie); // Lorsque le chronomètre est mis à jour, le compteur de cette minuterie s'incrémente 
	bool verifierTempsAtteint(const Minuterie minuterie, uint16_t temps); // Retourne vrai si le compteur de cette minuterie est >= temps, voir les constantes ci-bas pour le temps
	uint16_t obtenirCompteur(const Minuterie minuterie); // Retourne la valeur actuelle du compteur de la minuterie
	void reinitialiser(const Minuterie minuterie); // Mets le compteur de cette minuterie à 0. Ne change rien d'autre
	void eteindre(const Minuterie minuterie); // Empêche le compteur de cette minuterie de s'incrémenter et le remets à 0
	
	// Cette méthode permet de savoir si le temps cyclique de 5 ms s'est écoulé. Si c'est le cas, les compteurs des minuteries actives sont incrémentés
	bool aEteMisAJour(ControleurInterruptions& controleurInterruptions);
	
	// Constantes de temps
	static const uint16_t UNE_SECONDE = 200;
	static const uint16_t DEUX_SECONDES = 400;
	static const uint16_t TROIS_SECONDES = 600;
	
private:
	// Environ 5 ms
	const uint16_t TEMPS_CYCLIQUE = 39;
	
	// Structure permettant de sauvegarder les caractéristiques d'une minuterie
	struct DonneesMinuterie
	{
		uint16_t compteur;
		bool active;
	};
	
	DonneesMinuterie donneesMinuteries[NOMBRE_DE_MINUTERIES];
};

#endif /* CHRONOMETRE_H */
