////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet la gestion des interruptions du bouton poussoir et de la minuterie 1
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "constantes.h"
#include "ControleurInterruptions.h"

// Variables volatiles pour l'interruption
static volatile uint8_t minuterieExpiree;
static volatile uint8_t boutonPoussoir;

// Interruption de la minuterie 1
ISR (TIMER1_COMPA_vect) {
	cli();
	minuterieExpiree = 1;
	ControleurInterruptions::fermerMinuterie1();
	sei();
}

// Interruption du bouton poussoir
ISR (INT0_vect) {
	cli();
	// anti-rebond
	_delay_ms(10);

	if ( PIND & PIN_D3 ) {
		boutonPoussoir = 1;
	}

	EIFR |= (1 << INTF0) ;
	sei();
}

ControleurInterruptions::ControleurInterruptions()
{
	minuterieExpiree = 0;
	boutonPoussoir = 0;
	
	initialisationInterruptions();
}

// Environ 1 seconde pour une duree de 7812.5
void ControleurInterruptions::partirMinuterie1_prescaler_1024(uint16_t duree)
{
	minuterieExpiree = 0;

	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée

	TCNT1 = 0x0; // Mettre le compteur à 0

	// Environ 1 seconde avec l horloge ralentie
	OCR1A = duree;

	// Desactive OC1A
	TCCR1A = (0 << COM1A1) | (0 << COM1A0);

	// Desactive OC1B et divise la frequence d horloge par 1024
	TCCR1B = (0 << COM1B1) | (0 << COM1B0) | (1 << CS12) | (0 << CS11) | (1 << CS10);

	TCCR1C = 0;

	TIMSK1 |= 1 << OCIE1A;
}

// Environ 1 seconde pour une duree de 8 * 10⁶
void ControleurInterruptions::partirMinuterie1_prescaler_1(uint16_t duree)
{
	minuterieExpiree = 0;

	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée

	TCNT1 = 0x0; // Mettre le compteur à 0

	// Environ 1 seconde avec l horloge ralentie
	OCR1A = duree;

	// Desactive OC1A
	TCCR1A = (0 << COM1A1) | (0 << COM1A0);

	// Desactive OC1B et divise la frequence d horloge par 1
	TCCR1B = (0 << COM1B1) | (0 << COM1B0) | (0 << CS12) | (0 << CS11) | (1 << CS10);

	TCCR1C = 0;

	TIMSK1 |= 1 << OCIE1A;
}

// Désactive la minuterie 1
void ControleurInterruptions::fermerMinuterie1(void)
{
	TCNT1 = 0x0; // Mettre le compteur à 0
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10); // Arreter la minuterie
}

// Vérifie si la minuterie est expirée
bool ControleurInterruptions::minuterie1EstExpiree()
{
	if (minuterieExpiree)
	{
		minuterieExpiree = 0;
		return true;
	}
	
	return false;
}

// Vérifie si le bouton est appuyé
bool ControleurInterruptions::boutonEstAppuye()
{
	if (boutonPoussoir)
	{
		boutonPoussoir = 0;
		return true;
	}
	
	return false;
}

// Permet de configurer l'interruption de la minuterie1 et du bouton poussoir
void ControleurInterruptions::initialisationInterruptions(void)
{
	initialisationMinuterie1();
	initialisationBoutonPoussoir();
}

// Méthode appelée par le constructeur pour bien configuer la minuterie1
void ControleurInterruptions::initialisationMinuterie1(void)
{

	// cli est une routine qui bloque toutes les interruptions.
	cli ();
	
	const uint8_t PIN_D5 = (1 << 4);
	const uint8_t PIN_D6 = (1 << 5);
	DDRD &= ~PIN_D5 & ~PIN_D6; // Mettre ces pins en mode d'entrée
	
	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);
	
	EICRA |= (1 << ISC00) | (1 << ISC01); // Interruption au front montant
	
	// sei permet de recevoir à nouveau des interruptions
	sei ();

}

// Méthode appelée par le constructeur pour bien configuer le bouton poussoir
void ControleurInterruptions::initialisationBoutonPoussoir(void)
{
	// cli est une routine qui bloque toutes les interruptions.
	cli ();
	
	DDRD &= ~PIN_D3; // Mettre cette pins en mode d'entrée
	
	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);
	
	EICRA |= (1 << ISC00) | (1 << ISC01); // Interruption au front montant
	
	// sei permet de recevoir à nouveau des interruptions
	sei ();
}
