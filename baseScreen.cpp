#include "baseScreen.h"
#include <iostream>
#include "ConsoleManager.h"

BaseScreen::BaseScreen(std::shared_ptr<Process> process, std::string processName)
	: attachedProcess(process)
{
	this->name = processName;
}

void BaseScreen::onEnabled() {
	display();
	process();
}

void BaseScreen::display() {
	system("cls");
	std::cout << "Displaying Process \"" << this->name << "\"\n-----------------------\n";
	printProcessInfo();
}

void BaseScreen::process() {
	while (true) {
		std::string command;
		std::cout << "Enter command: ";
		std::cin >> command;
		if (command == "exit") {
			std::cout << "Exiting " << this->name << std::endl;
			ConsoleManager::getInstance()->returnToPreviousConsole();
		}
		//TODO: FIX
		else if (command == "process-smi") {
			printProcessInfo();
		}
		else {
			std::cout << "Command not recognized.\n\n";
		}
	}
}

void BaseScreen::printProcessInfo() const {
	std::cout << "\nProcess Name: " << this->attachedProcess->getName() << "\n"; 
	std::cout << "\Time Created: " << this->attachedProcess->getCreationTime() << "\n";

	// Convert Status enum to string
	int status = this->attachedProcess->getStatus();
	std::string strStatus = "";
	switch (status) {
		case 0: strStatus = "Created"; break;
		case 1: strStatus="Waiting"; break;
		case 2: strStatus="Running"; break;
		case 3: strStatus="Done";
	}

	std::cout << "Status: " << strStatus << "\n"; 

	//TODO: Change to actual process ID
	std::cout << "ID: temp\n";

	std::cout << "Current Instruction Line: " << this->attachedProcess->getCurrentLine() << "\n";
	std::cout << "Lines of Code: " << this->attachedProcess->getTotalLines() << "\n\n";
}