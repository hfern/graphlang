INPUTS=$(wildcard *.cpp)
OUTDIR=./..

anything: 
	@echo "heyllio :)"
	@echo "Hey $(CC)"
	$(CC) -o $(OUTDIR)/graphlang.o $(CFLAGS) $(INPUTS)
	$(AR) rcs $(OUTDIR)/libgraphlang.a $(OUTDIR)/graphlang.o