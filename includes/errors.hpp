#pragma once

#define FAILURE_GLFW		1,	"Failed to initialize GLFW."
#define FAILURE_WINDOW		2,	"Failed to initialize window."
#define FAILURE_GLEW		3,	"Failed to initialize GLEW."
#define FAILURE_OBJECT		4,	"Object failed to be parsed. Check your object files."
#define FAILURE_ATLAS		5,	"Failed to create texture atlas. Please check if your images are in the right size and format."
#define FAILURE_ATLAS_UV	6,	"Unknown material in texture atlas."
#define FAILURE_SKYBOX		7,	"Failure to load skybox. Please check if your images are in the right size and format."
#define FAILURE_ATLAS_LOAD	8,	"Failure to load atlas. Either the dev's code is garbage or your block assets are invalid."
#define FAILURE_WORLD		9,	"MDRRRRRRR"
#define FAILURE_THREAD		10,	"Not enough threads to start the program. Go buy a better computer."
