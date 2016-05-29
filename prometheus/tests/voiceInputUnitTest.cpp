/*
By Laurie Harding-Russell
Purpose: Unit testing voiceInput.
*/

#include "voiceInput.h"
#include <thread>
#include <iostream>

int main() {
	std::thread inputThread = std::thread(&startGeneralInput);
	for (int lp1 =0; lp1 != 1000; lp1++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	inputThread.join();
}
