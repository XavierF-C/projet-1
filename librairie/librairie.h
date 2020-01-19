////////////////////////////////////////////////////////////////////////
/*
 * Ce fichier facilite les inclusions des fichiers de la librairie pour les programmes
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019 
 */
////////////////////////////////////////////////////////////////////////

#ifndef LIBRAIRIE_H
#define LIBRAIRIE_H

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <string.h>

#include "can.h"

#include "enumerations.h" // Fichier contenant les énumérations
#include "constantes.h" // Fichier contenant les constantes globales
#include "fonctions.h" // Fichier contentant les signatures des fonctions

#include "ControleurInterruptions.h" // classe qui gère certaines interruptions

#endif /* LIBRAIRIE_H */
