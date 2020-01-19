include ../makefile.commun

# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECTNAME=libstatique

# Fichiers sources
# Utilisez le suffixe .cpp pour les fichiers C++
# Listez tous les fichiers a compiler, separes par
# un espace. exemple: 'tp1.c tp2.cpp':
PRJSRC= fonctions.cpp can.cpp ControleurInterruptions.cpp

####### variables #######

#compilateur pour la librairie
CL=avr-ar

# Flags pour la creation de la librairie.a
CLFLAGS=crs


####### Cible (Target) #######

#Nom des cibles par defaut
TRG=$(PROJECTNAME).a


####### Creation des commandes du Makefile ####### 

# Creation des cibles Phony (Phony Target)
# En plus de la commande make qui permet de compiler
# votre projet, vous pouvez utilisez les commandes
# make all, make install et make clean
.PHONY: all clean 

# Make all permet simplement de compiler le projet
#
all: $(TRG)

# Créer la libairie.a
$(TRG): $(OBJDEPS)
	$(CL) $(CLFLAGS) -o $(TRG) $^

# Production des fichiers object
# De C a objet
%.o: %.c
	$(CC) $(CFLAGS) -c $<
# De C++ a objet
%.o: %.cpp 
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

# Verification des dependances (header dependencies)
-include *.d

# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(OBJDEPS) $(HEXTRG) *.d *.a *.out.map

#####                    EOF                   #####
