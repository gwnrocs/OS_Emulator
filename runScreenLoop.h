#pragma once
#ifndef RUN_SCREEN_LOOP_H
#define RUN_SCREEN_LOOP_H

#include <iostream>
#include <string>
#include <functional>
#include "utils.h"

using namespace std;

void runScreenLoop(function<void()> action);

#endif
