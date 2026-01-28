CPP	= g++

OBJS =  main.o Neutron.o Material.o

BIN = run

all: $(BIN)

clean:
	rm -f $(OBJS) $(BIN)

$(BIN): $(OBJS)
	$(CPP) $(OBJS) -o $(BIN)

%.o: %.cxx
	$(CPP) -c -g $*.cxx
