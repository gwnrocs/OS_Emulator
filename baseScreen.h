#pragma once
#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

#include "abstractConsole.h"
#include <memory>
#include "Process.h"
#include <string>

class BaseScreen : public AConsole
{
public:
	BaseScreen(std::shared_ptr<Process>, std::string processName);
	void onEnabled() override;
	void display() override;
	void process() override;

private:
	void printProcessInfo() const;
	std::shared_ptr<Process> attachedProcess;
};

#endif
