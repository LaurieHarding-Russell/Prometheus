/*
By Laurie Harding-Russell
Purpose: This program acts as interface that allows the user to perform basic tasks.
*/
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

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
					clearInput();

                                        inputThread = std::thread(&saveInput);
					computer.displayText("What would you likeme to search?");
					computer.say("what would you like me to search?");
					command = "";
					std::string search = "";
					std::cout << command << '\n';
					//stopInput();
                                        inputThread.join();
					computer.say ("Give me a second to process that.");
					std::string searchType = getInput();
					clearInput();
					processVoiceData();
					stopInput();
					std::string temp;
					do {
						temp = getInput();
						command += temp + " ";
						search += temp + "+";
						std::cout << temp << '\n';
					} while (temp != "");
					command = "Searching for " + command;
					std::cout << search << '\n';
					computer.say(command.c_str());
					if ( search != "") {
						if (searchType == "search") {
							internet->search(search.c_str());
							//inputThread = std::thread(&grammerVInput, "./dictionary/internet.jsgf");
                                        		computer.say(internet->getCurrentResult().c_str());

							/*do {
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
		                                	inputThread.join();*/
						} else {
							computer.say("Feature not programmed yet. ");
						}
					}
                                        inputThread = std::thread(&grammerVInput, "./dictionary/prometheus.jsgf");
				} else if (command == "tv"){
					// command = getInput();
				} else if (command == "talk") {
					std::string say = "";
					stopInput();
					inputThread.join();
					// clear the input
					do { 
                                                command = getInput();
                                        } while (command != "");

					inputThread = std::thread(&generalInput);
					//computer.displayText("What shall I say?");
					computer.say("What shall I say?");
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					do {
						command = getInput();
						//if (command != "")
						//	std::cout << command << " B\n";
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}while (command != "say");
					do {
						command = getInput();
						//if (command != "")
                                                //        std::cout << command << " M\n";
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
						if (command != "please") {
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
	sync();
	reboot(LINUX_REBOOT_CMD_POWER_OFF);
	return 0;
}
