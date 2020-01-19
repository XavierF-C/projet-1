////////////////////////////////////////////////////////////////////////
/* Description :
 * Cette classe permet la gestion des interruptions du bouton poussoir et de la minuterie 1
 * Auteurs : Xavier Fateux-Chapleau, -------, -------, -------
 * Matricules : 1956824, -------, -------, -------
 * Date : 14 Avril 2019
 */
////////////////////////////////////////////////////////////////////////

#ifndef CONTROLEUR_INTERRUPTIONS_H
#define CONTROLEUR_INTERRUPTIONS_H

class ControleurInterruptions
{
public:
	
	ControleurInterruptions(); // Se charge d'initialiser les interruptions
	
	void partirMinuterie1_prescaler_1024(uint16_t duree); // Partir la minuterie avec une durée = duree / 7812.5 secondes
	void partirMinuterie1_prescaler_1(uint16_t duree); // durée = durée / 8 * 10⁶ secondes
	static void fermerMinuterie1(void); // static pour permettre à l'interruption d'appeler cette méthode
	bool minuterie1EstExpiree(); // On utilise cette méthode pour vérifier si la minuterie est expirée
	
	bool boutonEstAppuye(); // On utilise cette méthode pour vérifier si le bouton est appuye
	
private:
	
	void initialisationInterruptions(void);// Permet de configurer l'interruption de la minuterie1 et du bouton poussoir
	
	void initialisationMinuterie1(void); // Méthode appelée par le constructeur pour bien configuer la minuterie1
	void initialisationBoutonPoussoir(void); // Méthode appelée par le constructeur pour bien configuer le bouton poussoir
};

#endif /* CONTROLEUR_INTERRUPTIONS_H */
