INPUTS=$(wildcard *.cpp)
OUTDIR=./..

anything: 
	$(CC) -o $(OUTDIR)/graphlang.o $(CFLAGS) $(INPUTS)
	$(AR) rcs $(OUTDIR)/libgraphlang.a $(OUTDIR)/graphlang.o
