/*
By Laurie Harding-Russell
Purpose: Unit testing voiceInput.

NOTE: DO NOT USE. Test segment faults for some unknown reason. The library works in prometheus...
I don't know why.
*/
/*
#include <iostream>
#include <string>
#include <sstream>
*/
#include "voiceInput.h"

#include <thread>
#include <iostream>

void mainMenuInputTest();

int main() {
	mainMenuInputTest();
	return 0;
}

void mainMenuInputTest() {
	std::thread inputThread = std::thread(&mainMenuInput);
	std::cout << "fixable!\n";
	for (int lp1 =0; lp1 != 1000; lp1++) {
		std::string command;
		std::cout << lp1 << '\n';
		do {
			command = getInput();
			std::cout << command << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		} while (command !="");
	}
	stopInput();
	std::cout << "Bunnies\n";
	inputThread.join();
}
