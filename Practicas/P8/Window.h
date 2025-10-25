#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLint getDirection() { return direction; }
	GLfloat getmuevex2() { return muevex2; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getsetPattern() { return setpattern; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLboolean getOnLigth() { return onlight;  }
	GLboolean getOnLigth2() { return onlight2; }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat setpattern, articulacion6;
	GLboolean onlight, onlight2;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevex2;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	// --- variables para el control (ping-pong) de articulacion6 ---
	GLfloat articulacion6Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion6Min;   // límite mínimo
	GLfloat articulacion6Max;   // límite máximo
	GLfloat articulacion6Step;  // paso por pulsación

	//Bandera muevex
	GLint direction;

};

