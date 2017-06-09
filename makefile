#
#	baut das komplette Scanner+Parser Projekt
#
OBJDIR = objs

AUTOMATDIR = Automat

BUFFERDIR = Buffer

SYMBOLTABLEDIR = Symboltable

SCANNERDIR = Scanner

PARSERDIR = Parser



.PHONY: all  # phony: avoids file name conflicts and improves performance
all:	automatOBJs bufferOBJs symboltableOBJs scanner parser
	@echo "target 'all' finished"

	
# rm 	remove
# -f	force, ohne nachfragen
.PHONY: clean  # phony: avoids file name conflicts and improves performance
clean:
	rm -f $(AUTOMATDIR)/$(OBJDIR)/*.o
	rm -f $(BUFFERDIR)/$(OBJDIR)/*.o
	rm -f $(SYMBOLTABLEDIR)/$(OBJDIR)/*.o
	rm -f $(SCANNERDIR)/$(OBJDIR)/*.o
	rm -f $(SCANNERDIR)/debug/*
	rm -f $(PARSERDIR)/$(OBJDIR)/*.o
	rm -f $(PARSERDIR)/debug/*
	

.PHONY: automatOBJs  # phony: avoids file name conflicts and improves performance
automatOBJs:
	$(MAKE) -C $(AUTOMATDIR) AutomatOBJTarget
	
	
.PHONY: bufferOBJs  # phony: avoids file name conflicts and improves performance
bufferOBJs:
	$(MAKE) -C $(BUFFERDIR) BufferOBJTarget

	
.PHONY: symboltableOBJs  # phony: avoids file name conflicts and improves performance
symboltableOBJs:
	$(MAKE) -C $(SYMBOLTABLEDIR) SymboltableOBJTarget
	

.PHONY: scanner  # phony: avoids file name conflicts and improves performance
scanner: 
	$(MAKE) -C $(SCANNERDIR) makeTestScanner

#parserOBJs:
#	$(MAKE) -C $(PARSERDIR) ParserOBJTarget

.PHONY: parser  # phony: avoids file name conflicts and improves performance
parser:
	$(MAKE) -C $(PARSERDIR) makeTestParser
	