#include "ConsoleManager.h"
#include <string>
#include <iostream>
#include "MainConsole.h"
#include "BaseScreen.h"
#include <mutex>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;
ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	sharedInstance = new ConsoleManager();

	if (sharedInstance == NULL)
	{
		std::cout << "ConsoleManager initialization failed." << std::endl;
	}
	else
	{
		std::cout << "ConsoleManager initialized." << std::endl;
	}

	/*
	* For Testing only
		Process process1("Process_1");
		Command command1(CommandEnum::PRINT);
		process1.addCommand(&command1);
		process1.executeCommand();
	*/
}

void ConsoleManager::destroy()
{
	delete sharedInstance;
}

void ConsoleManager::registerScreen(std::shared_ptr<AConsole> screenRef)
{
	std::string screenName = screenRef->getName();
	std::cout << "Registering screen with name " << screenName << std::endl;

	if (this->consoleTable.find(screenName) != this->consoleTable.end())
	{
		std::cout << "Screen with name " << screenName << " already exists." << std::endl;
		return;
	}

	this->consoleTable[screenName] = screenRef;
	std::cout << "Screen with name " << screenRef->getName() << " registered." << std::endl;
}

void ConsoleManager::switchToScreen(std::string screenName)
{
	if (this->consoleTable.find(screenName) != this->consoleTable.end())
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
}

void ConsoleManager::switchConsole(std::string consoleName)
{
	if (this->consoleTable.find(consoleName) != this->consoleTable.end())
	{
		system("cls");
		std::cin.clear();
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else {
		std::cout << "Console with name " << consoleName << " does not exist." << std::endl;
	}
}


void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		system("cls");
		this->currentConsole = this->previousConsole;
		this->currentConsole->onEnabled();
	}
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	// this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();

	std::cout << "Main Console created" << std::endl;

	this->consoleTable[MAIN_CONSOLE] = mainConsole;
}

void ConsoleManager::clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}