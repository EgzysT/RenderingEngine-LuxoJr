#pragma once

//#include "Graphics.h"
class Graphics;
class Camera;

struct application_window {
	int width = 0;
	int height = 0;
	Camera* camera = nullptr;
	Graphics* graphics = nullptr;
	bool mouse_left = false;
	bool mouse_right = false;
	double lastx = 0.0;
	double lasty = 0.0;

	application_window() {
		width = 0;
		height = 0;
		camera = nullptr;
		graphics = nullptr;
		mouse_left = false;
		mouse_right = false;
		lastx = 0.0;
		lasty = 0.0;
	};
	application_window(int width, int height, Camera* camera, Graphics* graphics) :
		width(width), height(height), camera(camera), graphics(graphics) {
		mouse_left = false;
		mouse_right = false;
		lastx = 0.0;
		lasty = 0.0;
	};
};