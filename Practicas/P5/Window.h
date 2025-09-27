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
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	// --- variables para el control (ping-pong) de articulacion1 ---
	GLfloat articulacion1Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion1Min;   // l�mite m�nimo
	GLfloat articulacion1Max;   // l�mite m�ximo
	GLfloat articulacion1Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion2 ---
	GLfloat articulacion2Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion2Min;   // l�mite m�nimo
	GLfloat articulacion2Max;   // l�mite m�ximo
	GLfloat articulacion2Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion3 ---
	GLfloat articulacion3Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion3Min;   // l�mite m�nimo
	GLfloat articulacion3Max;   // l�mite m�ximo
	GLfloat articulacion3Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion4 ---
	GLfloat articulacion4Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion4Min;   // l�mite m�nimo
	GLfloat articulacion4Max;   // l�mite m�ximo
	GLfloat articulacion4Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion5 ---
	GLfloat articulacion5Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion5Min;   // l�mite m�nimo
	GLfloat articulacion5Max;   // l�mite m�ximo
	GLfloat articulacion5Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion6 ---
	GLfloat articulacion6Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion6Min;   // l�mite m�nimo
	GLfloat articulacion6Max;   // l�mite m�ximo
	GLfloat articulacion6Step;  // paso por pulsaci�n

	// --- variables para el control (ping-pong) de articulacion7 ---
	GLfloat articulacion7Dir;   // 1.0f = incrementar, -1.0f = decrementar
	GLfloat articulacion7Min;   // l�mite m�nimo
	GLfloat articulacion7Max;   // l�mite m�ximo
	GLfloat articulacion7Step;  // paso por pulsaci�n
};

