/*
By Laurie Harding-Russell
Purpose: To unit test internet.cpp
*/
#include "internet.h"

int main(){
	// Test Search
	std::cout << "starting test\n";
	Internet internet;
	std::cout << "Initialized internet object \n";
	std::cout << internet.search("soccer") <<"\n\n";
	//std::cout << internet.search("wiki+who+is+Peter+Singer")<<'\n';
	return 0;
}
