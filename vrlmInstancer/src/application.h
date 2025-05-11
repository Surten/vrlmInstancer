#pragma once

#include <string>

#include "sceneManager.h"

/// <summary>
/// Class providing entry point different functions of the application
/// </summary>
class Application {
public:
	Application() {}
	~Application() {}


	/// <summary>
	/// Takes a .txt file and processes the commands inside it line by line, see the function definition or the manual in Appendix D for avaliable commands
	/// </summary>
	/// <param name="fileName"></param>
	void AutomaticMode(std::string fileName);

	/// <summary>
	/// Currently unused, used for debugging during application building
	/// </summary>
	void ManualMode();
public:

private:


private:
	SceneManager sm;
};
