# Definition der Variablen

# enthaelt die Header Files
HEADERDIR = includes

# enthaelt die Source Files
SRCDIR = src

# enthaelt die Obj Files zum Testen
OBJDIR = objs

# enthaelt das ausfuehrbare File zum Testen
BINDIRTEST = debug


## hier alle Object Files eintragen, ausser das File mit der Mainmethode
## darauf achten, dass letzte Zeile ohne "\" eingetragen wird

OBJS = $(OBJDIR)/Symboltable.o \
	$(OBJDIR)/StringTab.o \
	$(OBJDIR)/Information.o

#
#		Targets zum Bauen des Tests
#

# Variante mit geringen Makefile-Kenntnissen

## Linken der Object-Files
makeTestSymboltable: $(OBJDIR)/Information.o $(OBJDIR)/StringTab.o $(OBJDIR)/Symboltable.o $(BINDIRTEST)/TestSymboltable.o
	g++ -g $(OBJDIR)/Information.o $(OBJDIR)/StringTab.o $(OBJDIR)/Symboltable.o $(BINDIRTEST)/TestSymboltable.o -o $(BINDIRTEST)/SymboltableTest
#	cp $(OBJDIR)/Symboltable.o ../projectOBJs
#
#	
## compilieren der Source-files
#

$(OBJDIR)/Information.o : $(SRCDIR)/Information.cpp $(HEADERDIR)/Information.h
	@echo "g++ $*.cpp"
	g++ -g -c -Wall $(SRCDIR)/Information.cpp -o $(OBJDIR)/Information.o
	#g++ -O3 -c -Wall $(SRCDIR)/Information.cpp -o $(OBJDIR)/Information.o

$(OBJDIR)/StringTab.o : $(SRCDIR)/StringTab.cpp $(HEADERDIR)/StringTab.h
	@echo "g++ $*.cpp"
	g++ -g -c -Wall $(SRCDIR)/StringTab.cpp -o $(OBJDIR)/StringTab.o
	#g++ -O3 -c -Wall $(SRCDIR)/StringTab.cpp -o $(OBJDIR)/StringTab.o

$(OBJDIR)/Symboltable.o : $(SRCDIR)/Symboltable.cpp $(HEADERDIR)/Symboltable.h
	@echo "g++ $*.cpp"
	g++ -g  -c -Wall $(SRCDIR)/Symboltable.cpp -o $(OBJDIR)/Symboltable.o
	#g++ -O3  -c -Wall $(SRCDIR)/Symboltable.cpp -o $(OBJDIR)/Symboltable.o
#
#
$(BINDIRTEST)/TestSymboltable.o :  $(SRCDIR)/TestSymboltable.cpp $(HEADERDIR)/Information.h $(HEADERDIR)/StringTab.h $(HEADERDIR)/Symboltable.h
	@echo "g++ $*.cpp"
	g++ -g  -c -Wall $(SRCDIR)/TestSymboltable.cpp -o $(BINDIRTEST)/TestSymboltable.o
	#g++ -O3  -c -Wall $(SRCDIR)/TestSymboltable.cpp -o $(BINDIRTEST)/TestSymboltable.o
#


# Variante mit guten Makefile-Kenntnissen

#makeTestSymboltable: SymboltableTarget TestSymboltableTarget
#	g++ -g $(OBJDIR)/*.o $(BINDIRTEST)/TestSymboltable.o  -o $(BINDIRTEST)/SymboltableTest

SymboltableTarget: $(OBJS)/

# $@ name des object files = name auf der linken Seite vom Doppelpunkt des Targets
# $< erste abhaengigkeit, $< das erste Element in der Liste der Abhaengigkeiten
# $@ --> linke Seite vom Doppelpunkt
# $^ --> rechte Seite vom Doppelpunkt

#$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(HEADERDIR)/%.h
#	g++ -g  -c -Wall  $<  -o $@



#TestSymboltableTarget: $(SRCDIR)/TestSymboltable.cpp $(HEADERDIR)/Symboltable.h
#	g++ -g  -c -Wall $(SRCDIR)/TestSymboltable.cpp -o $(BINDIRTEST)/TestSymboltable.o  
	


# loeschen aller files im verzeichnis $(OBJDIR) und $(BINDIRTEST) und neu compilieren

cleanSymboltable:
	rm -rf $(OBJDIR)/*.o
	rm -rf $(BINDIRTEST)/*
	$(MAKE) makeTestSymboltable
	
	
	
## objs fuer GesamtProjekt neu erzeugen
SymboltableOBJTarget:
	$(MAKE) SymboltableTarget

	