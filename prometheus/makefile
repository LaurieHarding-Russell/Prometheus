CC = g++
CFLAGS = -g -std=c++11 -lpthread
PS = -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase` 
FLITEDIR = ./../../flite-2.0.0-release
FLITE = -I${FLITEDIR}/include -L${FLITEDIR}/build/armv7l-linux-gnueabihf/lib -lflite_cmu_us_kal -lflite_usenglish -lflite_cmulex -lflite -lm
SSL = -lssl -lcrypto
ALL = ${PS} ${FLITE} -llirc_client ${SSL}

Prometheus: prometheus.cpp tvControl.o computer.o internet.o 
	${CC} ${CFLAGS} prometheus.cpp tvControl.o computer.o internet.o -o Prometheus ${ALL}

#voiceInput.o: voiceInput.cpp voiceInput.h
#	${CC} ${CFLAGS} -c voiceInput.cpp ${PS}

computer.o: computer.cpp computer.h
	${CC} ${CFLAGS} -c computer.cpp ${FLITE}

tvControl.o: tvControl.cpp tvControl.h
	${CC} ${CFLAGS} -c tvControl.cpp -llirc_client

internet.o: internet.cpp internet.h
	${CC} ${CFLAGS} -c internet.cpp

clean:
	rm Prometheus *.o

test:
	./tests/make
