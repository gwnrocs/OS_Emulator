#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "abstractConsole.h"
#include "baseScreen.h"

const std::string MAIN_CONSOLE = "MAIN_CONSOLE";
const std::string SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";

class ConsoleManager
{
public:
	typedef std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void registerScreen(std::shared_ptr<AConsole> screenRef);
	void switchToScreen(std::string screenName);
	void unregisterScreen(std::string screenName);

	void switchConsole(std::string consoleName);

	void returnToPreviousConsole();
	void exitApplication();
	bool isRunning() const;

	void clearScreen();

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {};
	static ConsoleManager* sharedInstance;
	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;
	bool running;
};

#pragma once
