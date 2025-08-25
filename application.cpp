#include "application.hpp"
#include <windows.h>
#include <stdexcept>
#include "command_system/command_interpreter.hpp"
#include <iostream>

using Core::Application;

Application::Application() {
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		throw std::runtime_error("Failed to initialize COM library.");
	}
}

Application::~Application() {
	CoUninitialize();
}

void Application::Run() {
	CommandSystem::CommandInterpreter interpreter;
	// コマンドグループの作成
	while (!interpreter.ShouldExit()) {
		std::string command;
		std::getline(std::cin, command);
		interpreter.AddCommand(command);
	}
}