#include "abstractConsole.h"

AConsole::AConsole(std::string name) : name(name) {}

std::string AConsole::getName() {
    return name;
}