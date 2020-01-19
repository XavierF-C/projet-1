////////////////////////////////////////////////////////////////////////
/*
 * Ce fichier regroupe toutes les fonctions de la librairie
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#include "librairie.h"

// Cette fonction ajuste l etat du bouton selon le signal de D3
void miseJourBouton(EtatBouton& bouton)
{
	const uint8_t DELAY_ANTIREBOND = 10;
	
	if ( PIND & PIN_D3) {
		_delay_ms(DELAY_ANTIREBOND); // Mecanisme d antirebond
		if (PIND & PIN_D3) {
			bouton = EtatBouton::Appuye; // Le bouton est stable et appuye
		}
	}
	else {
		_delay_ms(DELAY_ANTIREBOND); // Mecanisme d antirebond
		if (~(PIND) & PIN_D3) {
			bouton = EtatBouton::Relache; // Le bouton est stable et relache
		}
	}
}

// Cette fonction mets la couleur désirée
void miseJourDEL(CouleurDEL couleurDel)
{
	const uint8_t PIN_1 = 1<<0; // Position de la pin 1
	const uint8_t PIN_2 = 1<<1; // Position de la pin 2
	
	DDRB |= (PIN_1) | (PIN_2); // Pour mettre les positions pin 1 et 2 en sortie 
	
	PORTB &= ~(PIN_1) & ~(PIN_2); // Mettre les positions pin 1 et 2 à 0
	
	switch(couleurDel)
	{
		case ROUGE:
			PORTB |= (PIN_1); // Allume en rouge
			break;
			
		case VERT:
			PORTB |= (PIN_2); // Allume en vert
			break;
			
		case ETEINT:
			// Rien à allumer
			break;
	}
}

// IMPORTANT: DEL1 est la DEL de droite vu de face
void miseJour5DELS(AllumeDel del1, AllumeDel del2, AllumeDel del3, AllumeDel del4, AllumeDel del5)
{
	PORTC &= ~(1 << DEL1) & ~(1 << DEL2) & ~(1 << DEL3) & ~(1 << DEL4) & ~(1 << DEL5); // Remise à 0
	PORTC |= (del1 << DEL1) | (del2 << DEL2) | (del3 << DEL3) | (del4 << DEL4) | (del5 << DEL5);
}

// Pour allumer une des DEL
void allumerDELParNumero(uint8_t numeroDEL)
{	
	
	switch (numeroDEL) {
		case 1:
			miseJour5DELS(ON, OFF, OFF, OFF, OFF);
			break;
		case 2:
			miseJour5DELS(OFF, ON, OFF, OFF, OFF);
			break;
		case 3: 
			miseJour5DELS(OFF, OFF, ON, OFF, OFF);
			break;
		case 4: 
			miseJour5DELS(OFF, OFF, OFF, ON, OFF);
			break;
		default: 
			miseJour5DELS(OFF, OFF, OFF, OFF, OFF);
			break;
		}

}

// Initialise les ports pour les DELS du breadboard
void initialisation5DELS(void)
{
	// Initialise les 5 DELS sur le port C aux positions indiqués par les variables globales
	DDRC |= (1 << DEL1) | (1 << DEL2) | (1 << DEL3) | (1 << DEL4) | (1 << DEL5);
	PORTC &= ~(1 << DEL1) & ~(1 << DEL2) & ~(1 << DEL3) & ~(1 << DEL4) & ~(1 << DEL5);
}

// Permet les interruptions avec le bouton au front montant et descendant
void initialisationBouton(void)
{
	// Empêche les interruptions
	cli ();

	// Entrees
	DDRD &= ~(PIN_D3); // PORT D est en mode entree pour l utilisation de D3

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);

	// Un siganl d interruption(ISR (INT0_vect)) est lance au front montant et descendant de la PIN D3, correspondant au bouton-poussoir
	EICRA |= (1 << ISC00);

	// sei permet de recevoir à nouveau des interruptions.
	sei ();
}

// Permet les interruptions avec le bouton au front montant 
void initialisationBoutonFrontMontant(void)
{
	// Empêche les interruptions
	cli ();

	// Entrees
	DDRD &= ~(PIN_D3); // PORT D est en mode entree pour l utilisation de D3

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);

	// Un siganl d interruption(ISR (INT0_vect)) est lance au front montant de la PIN D3, correspondant au bouton-poussoir
	EICRA |= (1 << ISC00) | (1 << ISC01);

	// sei permet de recevoir à nouveau des interruptions.
	sei ();
}

// Permet les interruptions avec la minuterie
void initialisationMinuterie(void)
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

// Configure le signal PWM sur le timer0 pour l'émission infrarouge
void initialisationPWM_timer0(void)
{
	// Empêcher les interruptions
	cli ();

	// Sorties
	DDRB |= 0x0C; // PORT B3 et B4 sont en mode sortie pour le PWM
	
    PORTB &= ~(1 << 2); //La pin 3 du port B est le GND
    
    // Configurer le signal PWM
	TCNT0 = 0x0;
	
	OCR0A = 0;
	OCR0B = 0;
	
	// Mode CTC
	TCCR0A =  (1 << COM0A0) |(1 << COM0B0) |(1 << WGM01);

	// Desactive OC1B et divise la frequence d horloge par 1
	TCCR0B = (1 << CS00);
	// Desactive OC1B et divise la frequence d horloge par 1024
	//TCCR0B = (1 << CS00) || (1 << CS02);
	
	// sei permet de recevoir à nouveau des interruptions.
	sei();

}

// Configure le signal PWM
void initialisationPWM_timer1(void)
{
	// Empêcher les interruptions
	cli ();

	// Sorties
	DDRD |= 0x30; // PORT D est en mode sortie pour l utilisation de D5 et D6
	
	
	// Configurer le signal PWM
	TCNT1 = 0x0;
	
	OCR1A = 0;
	OCR1B = 0;
	
	// Desactive OC1A
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);

	// Desactive OC1B et divise la frequence d horloge par 8
	TCCR1B = (1 << CS11);

	TCCR1C = 0;
	
	// sei permet de recevoir à nouveau des interruptions.
	sei ();

}

//initialisation du timer 2 pour faire des notes mode CTC
void initialisationPWM_Note_timer2(void)
{
	cli ();

	// Sorties
	DDRD |= 0x40; // PORT D est en mode sortie pour l utilisation de D7 et on utilise pa pin GND
	
	// Configurer le signal CTC
	TCNT2 = 0x0;
	
	OCR2A = 0;
	OCR2B = 0;
	
	TCCR2A = (1 << COM2A0) | (1 << COM2B0) | (1 << WGM21);

	// Desactive OC1B et divise la frequence d horloge par 256

	TCCR2B = (1 << CS22);
	
	// sei permet de recevoir à nouveau des interruptions.
	sei ();
}

// Configurer les registres pour le UART
void initialisationUART(void)
{
	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications
	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0B &= ~(1 << UCSZ02);
	
	// Format des trames: 8 bits, 1 stop bits, none parity
	UCSR0C &= ~(1 << UPM01) & ~(1 << UPM00) & ~(1 << USBS0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

// Configurer le port A pour le suiveur de ligne
void initialisationSuiveurDeLigne()
{
	for(int i=0; i<NOMBRE_CAPTEURS_IR; ++i)
	{
		DDRA &= ~(1 << SUIVEUR_DELIGNE[i]); // Mettre une pin du port A en mode d'entrée pour chaque capteur du suiveur de ligne
	}
}

// Cette fonction desactive la minuterie
void fermerMinuterie ()
{
	minuterieActive = false;
	
	// Reinitialise la minuterie a 0
	TCNT1 = 0x0;

	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10); // Arreter la minuterie
	
	// Desactive l interruption
	TIMSK1 &= ~(1 << OCIE1A);

}

// Changer les proportions des signaux PWM des roues
void changerSignalPWM_timer0( uint8_t dureeA, uint8_t dureeB)
{
	OCR0A = dureeA;
	OCR0B = dureeB;
	
}

// Changer les proportions des signaux PWM pour le timer 1
void changerSignalPWM_timer1( uint16_t dureeA, uint16_t dureeB)
{
	OCR1A = dureeA;
	OCR1B = dureeB;
}

//change la fréquence de la minuterie pour jouer une autre note sur le timer 2
void changerNotePWN_timer2(uint8_t note)
{
	// Desactive OC1B et divise la frequence d horloge par 256
	TCCR2B = (1 << CS22);
	
	uint8_t DEBUT_INTERVAL_NOTE = 45;
	uint8_t FIN_INTERVAL_NOTE = 81;
	
	//Tableau des TOP pour chaque note, calculé à l'aide l'équation TOP = F_Clk/(2*N*F_PWM)-1
	const uint8_t tableauNote[FIN_INTERVAL_NOTE - DEBUT_INTERVAL_NOTE + 1] = {141, 133, 126,118, 112, 105, 99, 84, 88, 83, 79, 74, 70, 66, 62, 59, 55, 52, 49, 46, 44, 41, 39, 37, 35, 34, 33, 29, 27, 26, 24,23, 21, 20, 19, 18, 17}; 
	
	//Vérification de la validité de la note
	if(note >= DEBUT_INTERVAL_NOTE && note <= FIN_INTERVAL_NOTE){
		OCR2A = tableauNote[note - 45];
	}
}

//arrête la musique
void eteindreMusique (void)
{
	OCR2A = 0;
	TCCR2B &= ~(1 << CS22) & ~(1 << CS21) & ~(1 << CS20);
}

// Envoyer un caractère
void transmissionUART ( uint8_t donnee )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
		;
	/* Put data into buffer, sends the data */
	UDR0 = donnee;
}

// Envoyer une chaine de caractères
void transmissionChaineUART(uint8_t* chaine)
{
	for(uint8_t i=0; i<strlen((char*)chaine)-1; ++i)
		transmissionUART(chaine[i]);
}
