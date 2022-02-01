CC=gcc
CFLAGS += -I/opt/OpenBLAS/include/ -I/home/yixue/diffnn/OpenBLAS -Wall
LDFLAGS= -L/opt/OpenBLAS/lib -L/home/yixue/diffnn/OpenBLAS -lopenblas -lpthread -lm
DEPS= matrix.h nnet.h split.h

all: run_network delta_network_test test_mnist test_HAR
all: CFLAGS += -O3
all: LDFLAGS += -O3 

lineq:
	$(eval CFLAGS += -DLINEQ)
lineqall:
	$(eval CFLAGS += -DLINEQ -DLINEQDELTA)
extravars:
	$(eval CFLAGS += -DEXTRAVARS)
extravarssym: lineqall 
	$(eval CFLAGS += -DEXTRAVARS -DEXTRAVARSSYM)
extravarheuristic:
	$(eval CFLAGS += -DEXTRAVARSHEURISTIC)

allopts: lineqall extravars

testextravarsym: lineqall extravars
	$(eval CFLAGS += -DEXTRAVARSSYM)

bench:
	$(eval CFLAGS += -DMAX_THREAD=10)
bpvm:
	$(eval CFLAGS += -DMAX_THREAD=4)

debug: run_network delta_network_test test_mnist test_HAR
debug: CFLAGS += -g -O0
debug: LDFLAGS += -g -O0

stats: 
	$(eval CFLAGS += -DNEURONSTATE -DNEURONEQ -DNEURONBOUNDS -DFWDINTERVAL -DLOGGING -DBACKPROP)

statetotals: 
	$(eval CFLAGS += -DNEURONSTATETOTALS)

boundstest: 
	$(eval CFLAGS += -fsanitize=address)
	$(eval LDFLAGS += -fsanitize=address -lasan)

network_test: matrix.o nnet.o network_test.o split.o
	$(CC) -o $@ $^ $(LDFLAGS)

run_network: matrix.o interval.o nnet.o run_network.o split.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_mnist: matrix.o nnet.o test_mnist.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_HAR: matrix.o nnet.o test_HAR.o
	$(CC) -o $@ $^ $(LDFLAGS)


bnetwork_test: matrix.o nnet.o bnetwork_test.o split.o
	$(CC) -o $@ $^ $(LDFLAGS)

delta_network_test: matrix.o interval.o nnet.o delta_network_test.o split.o
	$(CC) -o $@ $^ $(LDFLAGS)

c.o: 
	$(CC) $(CFLAGS) $<  -o $@


clean:
	rm -f *.o delta_network_test run_network test_HAR test_mnist

