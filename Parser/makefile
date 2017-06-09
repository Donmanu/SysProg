## Definition der Variablen

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

## Gesamtprojekt Teil 1: Scanner
SCANNERPROJECTOBJS = $(AUTOMATDIR)/$(OBJDIR)/*.o $(BUFFERDIR)/$(OBJDIR)/*.o $(SYMBOLTABLEDIR)/$(OBJDIR)/*.o $(SCANNERDIR)/$(OBJDIR)/*.o

## hier alle Object Files eintragen, ausser das File mit der Mainmethode
OBJS = $(OBJDIR)/ParseTree.o $(OBJDIR)/Parser.o


#target … : prerequisites …	  |
#			recipe			  | < a 'rule'
#			…				  |




## Variante mit wenig Makefile-Kenntnissen

# linken der Object-files zu ParserTest.elf
.PHONY: makeTestParser  # phony: avoids file name conflicts and improves performance
makeTestParser: ParserOBJTarget
	g++ -g -c -Wall $(SRCDIR)/TestParser.cpp  -o $(BINDIRTEST)/TestParser.o
	g++ -g $(BINDIRTEST)/TestParser.o $(OBJS) $(SCANNERPROJECTOBJS)  -o $(BINDIRTEST)/ParserTest

## compilieren der source-files zu *.o
.PHONY: ParserOBJTarget  # phony: avoids file name conflicts and improves performance
ParserOBJTarget : $(OBJS)

$(OBJDIR)/ParseTree.o :  $(SRCDIR)/ParseTree.cpp $(HEADERDIR)/ParseTree.h
	g++ -g -c -Wall $(SRCDIR)/ParseTree.cpp  -o $(OBJDIR)/ParseTree.o

$(OBJDIR)/Parser.o :  $(SRCDIR)/Parser.cpp $(HEADERDIR)/Parser.h
	g++ -g -c -Wall $(SRCDIR)/Parser.cpp  -o $(OBJDIR)/Parser.o

# loeschen aller files im verzeichnis $(OBJDIR) und $(BINDIRTEST) und neu erstellen
.PHONY: cleanParser  # phony: avoids file name conflicts and improves performance
cleanParser:
	rm -rf $(OBJDIR)/*.o
	rm -rf $(BINDIRTEST)/*
	$(MAKE) makeTestParser


