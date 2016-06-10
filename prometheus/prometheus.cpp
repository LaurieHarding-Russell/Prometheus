
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
	std::thread inputThread = std::thread(&grammerVInput, "./dictionary/prometheus.jsgf");
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
					stopInput();
                                        inputThread.join();
                                        inputThread = std::thread(&generalInput);
					computer.displayText("What would you likeme to search?");
					computer.say("what would you like me to search?");
					command = "";
					std::string search = "";
					do {
						command = getInput();
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					} while (command == "search");
					do {
						command += getInput() + " ";
                                        	search += command + "+";
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					} while (command == "please");

					stopInput();
                                        inputThread.join();

					command = "Searching for " + command;
					computer.say (command.c_str());
					internet->search(search.c_str());
					inputThread = std::thread(&grammerVInput, "./dictionary/internet.jsgf");
                                        computer.say(internet->getCurrentResult().c_str());

					do {
						command = getInput();
						if (command == "next") {
							computer.say(internet->getNextResult().c_str());
						} else if (command == "current") {
							computer.say(internet->getCurrentResult().c_str());
						} else if (command == "previous") {
							computer.say(internet->getPreviousResult().c_str());
						} else if (command == "select") {

						}
					} while (command != "exit");

                                        stopInput();
                                        inputThread.join();

                                        inputThread = std::thread(&grammerVInput, "./dictionary/prometheus.jsgf");
				} else if (command == "tv"){
					// command = getInput();
				} else if (command == "talk") {
					std::string say = "";
					stopInput();
					inputThread.join();
					// Maybe clear the input just in case?
					inputThread = std::thread(&generalInput);
					//computer.displayText("What shall I say?");
					computer.say("What shall I say?");
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					do {
						command = getInput();
					}while (command != "say");
					do {
						command = getInput();
						if (command != "please") {
							say+=command + " ";
							std::this_thread::sleep_for(std::chrono::milliseconds(10));
						}
					} while (command != "please");
					computer.say(say.c_str());
					std::cout << "The computer said" << say << '\n';

					stopInput();
                                        inputThread.join();
                                        inputThread = std::thread(&grammerVInput, "./dictionary/prometheus.jsgf");


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
