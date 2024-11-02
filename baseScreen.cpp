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
	std::cout << "Displaying " << this->name << std::endl;
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
		else if (command == "screen") {
			break;
		}
	}
}

void BaseScreen::printProcessInfo() const {
	std::cout << "Process Name: " << this->attachedProcess->getName() << std::endl;
	std::cout << "Time Created: " << this->attachedProcess->getCreationTime() << std::endl;
	std::cout << "Status: " << this->attachedProcess->getStatus() << std::endl;
}