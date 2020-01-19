////////////////////////////////////////////////////////////////////////
/*
 * Ce fichier regroupe les signatures des fonctions de la librairie
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019 
 */
////////////////////////////////////////////////////////////////////////

#ifndef FONCTIONS_H
#define FONCTIONS_H

// Cette fonction ajuste l etat du bouton selon le signal de D3
void miseJourBouton(EtatBouton& bouton);

// Cette fonction mets la couleur désirée
void miseJourDEL(CouleurDEL couleurDel);

// Allumer les dels sur le breadboard, aller voir dans le .cpp pour les détails
void miseJour5DELS(AllumeDel del1, AllumeDel del2, AllumeDel del3, AllumeDel del4, AllumeDel del5);

// Pour allumer une des DEL
void allumerDELParNumero(uint8_t numeroDEL);

// Initialise les ports pour les DELS du breadboard
void initialisation5DELS(void);

// Permet les interruptions avec le bouton au front montant et descendant
void initialisationBouton(void);

// Permet les interruptions avec le bouton au front montant 
void initialisationBoutonFrontMontant(void);

// Permet les interruptions avec la minuterie
void initialisationMinuterie(void);

// Configure le signal PWM pour l'infrarouge
void initialisationPWM_timer0(void);

// Configure le signal PWM
void initialisationPWM_timer1(void);

//initialisation du timer 2 pour faire des notes
void initialisationPWM_Note_timer2(void);

// Configurer les registres pour le UART
void initialisationUART(void);

// Configurer le port A pour le suiveur de ligne
void initialisationSuiveurDeLigne();

// Cette fonction desactive la minuterie
void fermerMinuterie(void);

// Changer les proportions des signaux PWM pour le timer 0
void changerSignalPWM_timer0(uint8_t dureeA, uint8_t dureeB);

// Changer les proportions des signaux PWM pour le timer 1
void changerSignalPWM_timer1(uint16_t dureeA, uint16_t dureeB);

//change la fréquence de la minuterie pour jouer une autre note sur le timer 2 mode CTC
void changerNotePWN_timer2(uint8_t note);

//arrête la musique
void eteindreMusique (void);

// Envoyer un caractère
void transmissionUART(uint8_t donnee);

// Envoyer une chaine de caractères
void transmissionChaineUART(uint8_t* chaine);

#endif /* FONCTIONS_H */
