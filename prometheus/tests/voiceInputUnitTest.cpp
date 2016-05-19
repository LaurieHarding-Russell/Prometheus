/*
By Laurie Harding-Russell
Purpose: Unit testing voiceInput.
*/

#include "voiceInput.h"
#include <thread>
#include <iostream>

int main() {
	VoiceInput test;
	std::thread inputThread = std::thread(&VoiceInput::getMicInput, test);
	for (int lp1 =0; lp1 != 1000; lp1++){
		test.lock();
		if (!input.empty()) {
			std::cout << input.front() << '\n';
			input.pop();
		}
		test.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	test.lock();
	test.quit = true;
	test.unlock();
	inputThread.join();
}