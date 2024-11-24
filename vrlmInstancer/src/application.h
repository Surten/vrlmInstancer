#pragma once

#include <string>

#include "sceneManager.h"

class Application {
public:
	Application() {}



	void AutomaticMode(std::string fileName);
	void ManualMode();
public:

private:


private:
	SceneManager sm;
};
