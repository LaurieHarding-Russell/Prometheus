CC = g++
CFLAGS = -g -std=c++11 -lpthread
GRAPHICS = -lX11
PS = -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase` 
FLITEDIR = ./../../flite-2.0.0-release
FLITE = -I${FLITEDIR}/include -L${FLITEDIR}/build/armv7l-linux-gnueabihf/lib -lflite_cmu_us_slt -lflite_usenglish -lflite_cmulex -lflite -lm
SSL = -lssl -lcrypto
#CEC = -lcec -ldl
ALL = ${PS} ${FLITE} ${GRAPHICS} -llirc_client ${SSL}

Prometheus: prometheus.cpp tvControl.o computer.o internet.o 
	${CC} ${CFLAGS} prometheus.cpp tvControl.o computer.o internet.o -o Prometheus ${ALL}

computer.o: computer.cpp computer.h
	${CC} ${CFLAGS} -c computer.cpp ${FLITE} ${GRAPHICS}

tvControl.o: tvControl.cpp tvControl.h
	${CC} ${CFLAGS} -c tvControl.cpp -llirc_client

internet.o: internet.cpp internet.h
	${CC} ${CFLAGS} -c internet.cpp

clean:
	rm Prometheus *.o

test:
	./tests/make
