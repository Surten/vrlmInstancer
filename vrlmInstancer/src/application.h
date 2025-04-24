#pragma once

#include <string>

#include "sceneManager.h"

class Application {
public:
	Application() {}
	~Application() {}



	void AutomaticMode(std::string fileName);
	void ManualMode();
public:

private:


private:
	SceneManager sm;
};
