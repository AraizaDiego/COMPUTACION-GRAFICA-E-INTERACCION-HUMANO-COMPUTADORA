#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;

	// Inicialización de control ping-pong para articulacion1
	articulacion1Dir = 1.0f;    // empieza bajando
	articulacion1Min = 0.0f;
	articulacion1Max = 30.0f;
	articulacion1Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion2
	articulacion2Dir = -1.0f;    // empieza subiendo
	articulacion2Min = -45.0f;
	articulacion2Max = 45.0f;
	articulacion2Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion3
	articulacion3Dir = -1.0f;    // empieza subiendo
	articulacion3Min = -45.0f;
	articulacion3Max = 45.0f;
	articulacion3Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion4
	articulacion4Dir = -1.0f;    // empieza subiendo
	articulacion4Min = -45.0f;
	articulacion4Max = 45.0f;
	articulacion4Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion5
	articulacion5Dir = -1.0f;    // empieza subiendo
	articulacion5Min = -45.0f;
	articulacion5Max = 45.0f;
	articulacion5Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion6
	articulacion6Dir = 1.0f;    // empieza bajando
	articulacion6Min = 0.0f;
	articulacion6Max = 45.0f;
	articulacion6Step = 5.0f;  // +5° por pulsación

	// Inicialización de control ping-pong para articulacion7
	articulacion7Dir = 1.0f;    // empieza bajando
	articulacion7Min = -840.0f;
	articulacion7Max = 840.0f;
	articulacion7Step = 15.0f;  // +5° por pulsación

	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica XX: Nombre de la práctica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	if (key == GLFW_KEY_F)
	{
		float next = theWindow->articulacion1 + theWindow->articulacion1Dir * theWindow->articulacion1Step;

		if (next > theWindow->articulacion1Max) {
			theWindow->articulacion1 = theWindow->articulacion1Max;
			theWindow->articulacion1Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion1Min) {
			theWindow->articulacion1 = theWindow->articulacion1Min;
			theWindow->articulacion1Dir = 1.0f;
		}
		else {
			theWindow->articulacion1 = next;
		}
	}

	if (key == GLFW_KEY_G)
	{
		float next = theWindow->articulacion2 + theWindow->articulacion2Dir * theWindow->articulacion2Step;

		if (next > theWindow->articulacion2Max) {
			theWindow->articulacion2 = theWindow->articulacion2Max;
			theWindow->articulacion2Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion2Min) {
			theWindow->articulacion2 = theWindow->articulacion2Min;
			theWindow->articulacion2Dir = 1.0f;
		}
		else {
			theWindow->articulacion2 = next;
		}
	}
	if (key == GLFW_KEY_H)
	{
		float next = theWindow->articulacion3 + theWindow->articulacion3Dir * theWindow->articulacion3Step;

		if (next > theWindow->articulacion3Max) {
			theWindow->articulacion3 = theWindow->articulacion3Max;
			theWindow->articulacion3Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion3Min) {
			theWindow->articulacion3 = theWindow->articulacion3Min;
			theWindow->articulacion3Dir = 1.0f;
		}
		else {
			theWindow->articulacion3 = next;
		}
	}
	if (key == GLFW_KEY_J)
	{
		float next = theWindow->articulacion4 + theWindow->articulacion4Dir * theWindow->articulacion4Step;

		if (next > theWindow->articulacion4Max) {
			theWindow->articulacion4 = theWindow->articulacion4Max;
			theWindow->articulacion4Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion4Min) {
			theWindow->articulacion4 = theWindow->articulacion4Min;
			theWindow->articulacion4Dir = 1.0f;
		}
		else {
			theWindow->articulacion4 = next;
		}
	}
	if (key == GLFW_KEY_K)
	{
		float next = theWindow->articulacion5 + theWindow->articulacion5Dir * theWindow->articulacion5Step;

		if (next > theWindow->articulacion5Max) {
			theWindow->articulacion5 = theWindow->articulacion5Max;
			theWindow->articulacion5Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion5Min) {
			theWindow->articulacion5 = theWindow->articulacion5Min;
			theWindow->articulacion5Dir = 1.0f;
		}
		else {
			theWindow->articulacion5 = next;
		}
	}

	if (key == GLFW_KEY_C)
	{
		float next = theWindow->articulacion6 + theWindow->articulacion6Dir * theWindow->articulacion6Step;

		if (next > theWindow->articulacion6Max) {
			theWindow->articulacion6 = theWindow->articulacion6Max;
			theWindow->articulacion6Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion6Min) {
			theWindow->articulacion6 = theWindow->articulacion6Min;
			theWindow->articulacion6Dir = 1.0f;
		}
		else {
			theWindow->articulacion6 = next;
		}
	}
	if (key == GLFW_KEY_V)
	{
		float next = theWindow->articulacion7 + theWindow->articulacion7Dir * theWindow->articulacion7Step;

		if (next > theWindow->articulacion7Max) {
			theWindow->articulacion7 = theWindow->articulacion7Max;
			theWindow->articulacion7Dir = -1.0f; // invertir dirección para la siguiente pulsación
		}
		else if (next < theWindow->articulacion7Min) {
			theWindow->articulacion7 = theWindow->articulacion7Min;
			theWindow->articulacion7Dir = 1.0f;
		}
		else {
			theWindow->articulacion7 = next;
		}
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
