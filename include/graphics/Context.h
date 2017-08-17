#pragma once

// Forward declare GLFW's window 
// handling ponter type.
struct GLFWwindow;

class Context
{
private:
	GLFWwindow *handle;

public:
	// Pop up our window in windows OS
	bool init(size_t w = 800, size_t h = 600,
					const char *title = "Graphics");

	// updating the window
		// this is what determines the FPS
	bool step();

	// tell the window to close
	bool term();
};