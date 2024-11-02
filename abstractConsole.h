#pragma once
#ifndef ABSTRACT_CONSOLE_H
#define ABSTRACT_CONSOLE_H
#include <string>

class AConsole
{
public:
	AConsole() = default;
	AConsole(std::string name);
	~AConsole() = default;

	std::string getName();
	virtual void onEnabled() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	std::string name;
	friend class ConsoleManager;
};

#endif