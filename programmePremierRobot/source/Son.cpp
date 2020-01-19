////////////////////////////////////////////////////////////////////////
/* Description:
 * Cette classe permet de jouer du son sans utiliser de délais.
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "Son.h"

// Le son utilise le même chronomètre que les autres objets
Son::Son(Chronometre& chronometre) : chronometre(chronometre)
{
	initialisationPWM_Note_timer2();
	sonJoue = false;
	noteJoue = 50; // arbitraire mais valide
	tempsNote = 0;
	tempsPause = 0;
	repetitionRestante = 0;
}

// Pour jour une note unique
void Son::emettre(uint8_t note, uint16_t temps)
{
	sonJoue = true;
	noteJoue = note;
	tempsNote = temps;
	tempsPause = 0;
	repetitionRestante = 1;
	
	creerSon();
}

// Pour jouer une note répétée
void Son::emettre(uint8_t note, uint16_t tempsJoue, uint16_t tempsSilence, uint8_t repetition)
{
	sonJoue = true;
	noteJoue = note;
	tempsNote = tempsJoue;
	tempsPause = tempsSilence;
	repetitionRestante = repetition;
	
	creerSon();
}

// Permet d'arréter automatiquement le son quand la minuterie est expirée
void Son::miseAJour()
{
	if (sonJoue)
	{
		if (chronometre.verifierTempsAtteint(MINUTERIE_SON, tempsNote))
		{
			chronometre.eteindre(MINUTERIE_SON);
			eteindreMusique();
			sonJoue = false;
			
			if (repetitionRestante > 0)
			{
				--repetitionRestante;
				chronometre.activer(MINUTERIE_SON);
			}
		}
	}
	else
	{
		if (repetitionRestante > 0 && chronometre.verifierTempsAtteint(MINUTERIE_SON, tempsPause))
		{
			emettre(noteJoue, tempsNote, tempsPause, repetitionRestante);
		}
	}
}

// Permet une interruption brusque d'un son, indépendamment de la mise à jour
void Son::arreterSon()
{
	chronometre.eteindre(MINUTERIE_SON);
	eteindreMusique();
	
	sonJoue = false;
	tempsNote = 0;
	tempsPause = 0;
	repetitionRestante = 0;
}

// Méthode utilisée pour créer le son sur la minuterie2
void Son::creerSon()
{
	changerNotePWN_timer2(noteJoue);
	
	chronometre.activer(MINUTERIE_SON);
	chronometre.reinitialiser(MINUTERIE_SON);
}
