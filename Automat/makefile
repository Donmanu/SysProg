#
#		Targets zum Bauen des Tests
#

# Linken der Object-files, abhaengig von AutomatTarget und TestAutomatTarget
# flag:
# -g --> debug Informationen erzeugen
# -o --> name des output-files

.PHONY: makeTestAutomat  # phony: avoids file name conflicts and improves performance
makeTestAutomat: StateTarget AutomatTarget TestAutomatTarget
	g++ -g objs/State.o objs/Automat.o debug/TestAutomat.o -o debug/AutomatTest
	#g++ -O3 objs/State.o objs/Automat.o debug/TestAutomat.o -o debug/AutomatTest
	

# compilieren des Files Automat.cpp zu Automat.o, 
# das Objekt-File ist abhängig von src/Automat.cpp und src/Automat.h
# flag:
# -c --> nur compilieren
# -g --> debug Informationen erzeugen
# -Wall --> alle meldungen erzeugen (Warning all)

.PHONY: StateTarget  # phony: avoids file name conflicts and improves performance
StateTarget : src/State.cpp includes/State.h
	g++ -g -c -Wall src/State.cpp -o objs/State.o 
	#g++ -O3 -c -Wall src/State.cpp -o objs/State.o 


.PHONY: AutomatTarget  # phony: avoids file name conflicts and improves performance
AutomatTarget :  src/Automat.cpp includes/Automat.h
	g++ -g -c -Wall src/Automat.cpp -o objs/Automat.o  
	#g++ -O3 -c -Wall src/Automat.cpp -o objs/Automat.o  


# TestAutomat.o ist abhängig von src/TestAutomat.cpp und src/Automat.h
.PHONY: TestAutomatTarget  # phony: avoids file name conflicts and improves performance
TestAutomatTarget :  src/TestAutomat.cpp includes/State.h includes/Automat.h
	g++ -g -c -Wall src/TestAutomat.cpp  -o debug/TestAutomat.o
	#g++ -O3 -c -Wall src/TestAutomat.cpp  -o debug/TestAutomat.o



# loeschen aller files im verzeichnis obj und debug, dann neu erzeugen

.PHONY: cleanAutomat  # phony: avoids file name conflicts and improves performance
cleanAutomat:
	rm -f objs/*.o
	rm -f debug/*
	$(MAKE) makeTestAutomat
	
	
# dieses Target wird vom makefile des Gesamtprojekts verwendet
# objs fuer Gesamtprojekt loeschen und dann neu erzeugen
.PHONY: AutomatOBJTarget  # phony: avoids file name conflicts and improves performance
AutomatOBJTarget:
	$(MAKE) StateTarget
	$(MAKE) AutomatTarget

