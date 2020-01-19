////////////////////////////////////////////////////////////////////////
/* Description :
 * Ce fichier regroupe plusieurs constantes utilisées par les programmes
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef CONSTANTES_H
#define CONSTANTES_H

// Constantes globales pour les DEL
// IMPORTANT: DEL1 est la DEL de droite vu de face
static const uint8_t DEL1 = 0;
static const uint8_t DEL2 = 1;
static const uint8_t DEL3 = 2;
static const uint8_t DEL4 = 3;
static const uint8_t DEL5 = 4; // DEL5 est donc la DEL à gauche en vu de face

// Constantes globales
static const uint8_t PIN_D3 = 0x04;

//Constantes globales pour les délais lors de l'écriture et la lecture de la mémoire
static const uint8_t DELAI_LECTURE_MEM = 5;
static const uint8_t DELAI_ECRITURE_MEM = 5;

//Constantes globales pour brancher chaque capteur du suiveur de ligne
static const uint8_t IR1 = 0;
static const uint8_t IR2 = 1;
static const uint8_t IR3 = 2;
static const uint8_t IR4 = 3;
static const uint8_t IR5 = 4;
static const uint8_t NOMBRE_CAPTEURS_IR = 5;
static const uint8_t SUIVEUR_DELIGNE[NOMBRE_CAPTEURS_IR]={
	IR1,
	IR2,
	IR3,
	IR4,
	IR5
};

// Variables globales
static volatile bool minuterieActive = false;

//constante de temps
static const uint16_t TEMPS_LARGEUR_RUBAN = 200; // temps necessaire pour parcourir une largeur de ruban (environ 1sec)
static const uint16_t TROIS_SECONDES_MS = 3000;
static const uint16_t DEUX_SECONDES_MS = 2000;
static const uint16_t UNE_SECONDE_MS = 1000;

// Constantes pour jouer du son
static const uint8_t NOTE_GRAVE = 55;
static const uint8_t NOTE_AIGU = 70;
static const uint8_t DUREE_SON_RECTANGLE = 10; // Représente 50 ms
static const uint8_t DUREE_SON_RECTANGLE_SILENCE = 2; // Représente 10 ms
static const uint8_t REPETITIONS_SON_RECTANGLE = 2; 

#endif /* CONSTANTES_H */
