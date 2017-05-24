# Definition der Variablen

# enthaelt die Header Files
HEADERDIR = includes

# enthaelt die Source Files
SRCDIR = src

# enthaelt die Obj Files zum Testen
OBJDIR = objs

# enthaelt das ausfuehrbare File zum Testen
BINDIRTEST = debug


AUTOMATDIR = ../Automat

BUFFERDIR = ../Buffer

SYMBOLTABLEDIR = ../Symboltable

SCANNERDIR = ../Scanner

## hier alle Object Files eintragen, ausser das File mit der Mainmethode
## darauf achten, dass letzte Zeile ohne "\" eingetragen wird

OBJS = $(OBJDIR)/Parser.o \
	#$(OBJDIR)/xx1.o \
	#$(OBJDIR)/xxn.o


## Variante mit wenig Makefile-Kenntnissen
# 
## linken der Object-files 
#makeTestParser: $(OBJDIR)/Parser.o $(OBJDIR)/TestParser.o
#	g++ -g $(OBJDIR)/Parser.o $(OBJDIR)/TestParser.o   $(PROJECTOBJDIR)/Automat/*.o $(PROJECTOBJDIR)/Buffer/*.o $(PROJECTOBJDIR)/Symboltable/*.o $(PROJECTOBJDIR)/Scanner/*.o -o $(BINDIRTEST)/ParserTest
##	
##	
##	
### compilieren der source-files
#$(OBJDIR)/Parser.o :  $(SRCDIR)/Parser.cpp $(HEADERDIR)/Parser.h $(HEADERDIR)/Parser.h
#	@echo "g++ $*.cpp"
#	g++ -g  -c -Wall  -o $(OBJDIR)/Parser.o  $(SRCDIR)/Parser.cpp
#	cp $(OBJDIR)/Parser.o $(PROJECTOBJDIR)/Parser
##
#$(OBJDIR)/TestParser.o :  $(SRCDIR)/TestParser.cpp 
#	@echo "g++ $*.cpp"
#	g++ -g  -c -Wall  -o $(OBJDIR)/TestParser.o  $(SRCDIR)/TestParser.cpp
#
	
	
# Variante mit guten Makefile-Kenntnissen
makeTestParser: $(OBJS) ParserTest


$(OBJDIR)/%.o :  $(SRCDIR)/%.cpp $(HEADERDIR)/%.h
	@echo "g++ $*.cpp"
	g++ -g  -c -Wall  -o $@  $<
	#g++ -c -Wall -O0 -o $@  $<  # < put this in, if you suspect debug and/or optimization problems

# 
# $@ name des object files
# $< erste abhaengigkeit


ParserTest: $(SRCDIR)/TestParser.cpp $(HEADERDIR)/Parser.h
	g++ -g  -c -O0 -Wall src/TestParser.cpp -o $(BINDIRTEST)/TestParser.o  
	#g++ -c -O0 -Wall src/TestScanner.cpp -o $(BINDIRTEST)/TestScanner.o  
	g++ -g $(BINDIRTEST)/TestParser.o $(OBJDIR)/*.o $(AUTOMATDIR)/$(OBJDIR)/*.o $(BUFFERDIR)/$(OBJDIR)/*.o $(SYMBOLTABLEDIR)/$(OBJDIR)/*.o -o $(BINDIRTEST)/ParserTest

# loeschen aller files im verzeichnis $(OBJDIR) und $(BINDIRTEST) und neu erstellen

cleanParser:
	rm -rf $(OBJDIR)/*.o
	rm -rf $(BINDIRTEST)/*
	$(MAKE) makeTestParser


