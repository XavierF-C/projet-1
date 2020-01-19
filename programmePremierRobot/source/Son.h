////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe permet de jouer du son sans utiliser de délais.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef SON_H
#define SON_H

#include <librairie.h>
#include "Chronometre.h"


class Son
{
public:
	
	// Le son utilise le même chronomètre que les autres objets
	Son(Chronometre& chronometre);
	
	// Pour jour une note unique
	void emettre(uint8_t note, uint16_t temps);
	
	// Pour jouer une note répétée
	void emettre(uint8_t note, uint16_t tempsJoue, uint16_t tempsSilence, uint8_t repetition);
	
	// Permet d'arréter automatiquement le son quand la minuterie est expirée
	void miseAJour();
	
	// Permet une interruption brusque d'un son, indépendamment de la mise à jour
	void arreterSon();
	
	// Nos booléens peuvent briser dans private...
	bool sonJoue;
	
private:
	
	uint8_t noteJoue;
	uint16_t tempsNote;
	uint16_t tempsPause;
	uint8_t repetitionRestante;
	
	Chronometre& chronometre;
	
	// Méthode utilisée pour créer le son sur la minuterie2
	void creerSon();
};

#endif /* SON_H */
