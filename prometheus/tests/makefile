CC = g++
CFLAGS = -g -std=c++11 -lpthread
GRAPHICS = -lX11
PS = -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase`
FLITEDIR = ./../../../flite-2.0.0-release
FLITE = -I${FLITEDIR}/include -L${FLITEDIR}/build/armv7l-linux-gnueabihf/lib -lflite_cmu_us_kal -lflite_usenglish -lflite_cmulex -lflite -lm
SSL = -lssl -lcrypto
#CEC = -lcec -ldl
ALL = ${PS} ${FLITE} ${GRAPHICS} -llirc_client ${SSL}

Test: InternetUnitTest TvControlUnitTest ComputerUnitTest

InternetUnitTest: internetUnitTest.cpp ./../internet.o
	${CC} ${CFLAGS} internetUnitTest.cpp ./../internet.o -o InternetUnitTest -I./../ ${SSL}
	./InternetUnitTest

TvControlUnitTest: tvControlUnitTest.cpp ./../tvControl.o
	${CC} ${CFLAGS} tvControlUnitTest.cpp ./../tvControl.o -o TvControlUnitTest -I./../ ${CEC}
	./TvControlUnitTest

# Test not working.
VoiceUnitTest: voiceInputUnitTest.cpp ./../voiceInput.h
	${CC} ${CFLAGS} voiceInputUnitTest.cpp -o VoiceUnitTest -I ./../ ${PS}
	./VoiceUnitTest

ComputerUnitTest: computerUnitTest.cpp ./../computer.o
	${CC} ${CFLAGS} computerUnitTest.cpp ./../computer.o -o ComputerUnitTest -I./../ ${ALL}
	./ComputerUnitTest

clean:
	rm InternetUnitTest VoiceUnitTest ComputerUnitTest
