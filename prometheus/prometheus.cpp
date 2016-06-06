
#include <iostream>
#include <string>
#include <sstream>

#include "voiceInput.h"

#include "tvControl.h"
#include "computer.h"
#include "internet.h"


int main() {
	tvControl tv;
	Computer computer;
	Internet* internet = new Internet();
	computer.displayText("Morning. Try talking to me!");
	computer.say("Morning. You look well today.");

	// setup pocket sphinx and listen for input
	std::thread inputThread = std::thread(&mainMenuInput);
	std::string command;
	bool running = true;
	while (running) {
		command = getInput();

		if (command != "") {
			if (command == "computer") {
				std::cout << "Computer: ";
				int num = 0;
				//waitForSpeaker();
				command = getInput();
				if (command == "search") {
					std::string search = "";
					stopInput();
                                        inputThread.join();
                                        inputThread = std::thread(&generalInput);
					computer.displayText("What would you likeme to search?");
					computer.say("what would you like me to search?");
					command = "";
					do {
						command = getInput();
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					} while (command == "");

					while (command != "") {
                                                command = getInput();
                                                search += command;
                                        }
                                        computer.say(search.c_str());

					std::string result = internet->search(search.c_str());

					stopInput();
                                        inputThread.join();
                                        inputThread = std::thread(&mainMenuInput);

				} else if (command == "tv"){
					// command = getInput();
				} else if (command == "talk") {
					std::string say = "";
				//	waitForSpeaker();
					stopInput();
					inputThread.join();
					// Maybe clear the input just in case?
					inputThread = std::thread(&generalInput);
					//computer.displayText("What shall I say?");
					computer.say("What shall I say?");
					std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					do {
						command = getInput();
						say += command;
						std::cout << say << '\n';
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					} while (command != "");
					computer.say(say.c_str());
					std::cout << "The computer said" << say << '\n';

					stopInput();
                                        inputThread.join();
                                        inputThread = std::thread(&mainMenuInput);


				} else if (command == "shutdown") {
					stopInput();
					running = false;
					computer.say("I'm going to sleep now");
				} else {
					std::cout << command + "\n";
				}
			} else {
				std::cout << command + "\n";
			}
		}

		// let other threads do work.
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	inputThread.join();
	return 0;
}
