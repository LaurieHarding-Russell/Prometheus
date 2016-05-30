/*
By Laurie Harding-Russell
Purpose: Unit testing voiceInput.

NOTE: DO NOT USE. Test segment faults for some unknown reason. The library works in prometheus...
I don't know why.
*/

#include "voiceInput.h"
#include <thread>
#include <iostream>

void mainMenuInputTest();

int main() {
	mainMenuInputTest();
}

void mainMenuInputTest() {
	std::thread inputThread = std::thread(&mainMenuInput);
	for (int lp1 =0; lp1 != 1000; lp1++) {
		std::string command;
		do {
			command = getInput();
			std::cout << command << "\n";
		} while (command !="");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	stopInput();
	inputThread.join();
}
