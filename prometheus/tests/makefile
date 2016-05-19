CC = g++
CFLAGS = -g -std=c++11 -lpthread
PS = -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase` 
FLITEDIR = ./../../flite-2.0.0-release
FLITE = -I${FLITEDIR}/include -L${FLITEDIR}/build/armv7l-linux-gnueabihf/lib -lflite_cmu_us_kal -lflite_usenglish -lflite_cmulex -lflite -lm
SSL = -lssl -lcrypto
ALL = ${PS} ${FLITE} -llirc_client ${SSL}

Test: InternetUnitTest VoiceUnitTest

InternetUnitTest: internetUnitTest.cpp ./../internet.o
	${CC} ${CFLAGS} internetUnitTest.cpp ./../internet.o -o InternetUnitTest -I./../ ${SSL}
	./InternetUnitTest

#VoiceUnitTest: voiceInputUnitTest.cpp ./../voiceInput.o
#	${CC} ${CFLAGS} internetUnitTest.cpp ./../internet.o -o VoiceUnitTest-I./../ ${SSL}
#	./VoiceUnitTest

clean:
	rm InternetUnitTest *.o
