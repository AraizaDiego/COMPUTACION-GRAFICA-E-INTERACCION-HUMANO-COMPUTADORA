//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<gtc\quaternion.hpp>
#include<gtx\quaternion.hpp>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshColorList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

glm::quat rotx = glm::angleAxis(glm::radians(0.0f), glm::vec3(1, 0, 0));
glm::quat roty = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0));
glm::quat rotz = glm::angleAxis(glm::radians(00.0f), glm::vec3(0, 0, 1));
glm::quat rot = rotx * roty * rotz;
glm::mat4 localRot = glm::toMat4(rot);


void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

// Pirámide triangular regular2
void CrearPiramideTriangular2()
{
	unsigned int indices_piramide_triangular2[] = {
			0,1,2,
			0,2,3,
			0,3,1,
			1,3,2

	};
	GLfloat vertices_piramide_triangular2[] = {
		0.0f,  1.0f, 0.0f,	//0
		0.0f, 0.0f, 0.707107f,	//1
		-0.612372f, 0.0f, -0.353553f,	//2
		0.612372f, 0.0f, -0.353553f	//3
	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular2, indices_piramide_triangular2, 12, 12);
	meshList.push_back(obj1);

}

// Pirámide triangular regular3
void CrearPiramideTriangular3()
{
	GLfloat vertices_piramide_triangular3[] = {
		// X              Y           Z         R     G     B
		// Cara 1 (rojo) 
		- 0.081650f,  1.033333f,  0.047140f,   1.0f, 0.0f, 0.0f,
		- 0.081650f,  0.033333f,  0.754247f,   1.0f, 0.0f, 0.0f,
		- 0.694022f,  0.033333f, -0.306413f,   1.0f, 0.0f, 0.0f,

		// Cara 2 (verde)
		  0.000000f,  1.033333f, -0.094281f,   0.0f, 1.0f, 0.0f,
		- 0.612372f,  0.033333f, -0.447834f,   0.0f, 1.0f, 0.0f,
		  0.612372f,  0.033333f, -0.447834f,   0.0f, 1.0f, 0.0f,

		// Cara 3 (azul)
		  0.081650f,  1.033333f,  0.047140f,   0.0f, 0.0f, 1.0f,
		  0.694022f,  0.033333f, -0.306413f,   0.0f, 0.0f, 1.0f,
		  0.081650f,  0.033333f,  0.754247f,   0.0f, 0.0f, 1.0f,

		// Cara 4 (amarillo)
		  0.000000f, -0.100000f,  0.707107f,   1.0f, 1.0f, 0.0f,
		  0.612372f, -0.100000f, -0.353553f,   1.0f, 1.0f, 0.0f,
	    - 0.612372f, -0.100000f, -0.353553f,   1.0f, 1.0f, 0.0f,

	};
	MeshColor* obj1 = new MeshColor();
	obj1->CreateMeshColor(vertices_piramide_triangular3, 72);
	meshColorList.push_back(obj1);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(25, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearPiramideTriangular2();//índice 5 en MeshList
	CrearPiramideTriangular3();//índice 0 en MeshColorList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	// vector posicion, vector up (apunta hacia arriba), angulo o velocidad de giro
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		/*
		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[2]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera
		
		*/

	/*
	//CUADRADO ROJO CASA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//PIRAMIDE CUADRANGULAR AZUL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(1.2f,0.5f,1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();

	//CUADRADO VERDE PUERTA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.299f, -3.749f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//FRENTE DE LA CASA VENTANA DERECHA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.2f, -3.749f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//FRENTE DE LA CASA VENTANA IZQUIERDA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, 0.2f, -3.749f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//LADO DERECHO DE LA CASA VENTANA IZQUIERDA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.351f, 0.2f, -3.75));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//LADO DERECHO DE LA CASA VENTANA DERECHA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.351f, 0.2f, -4.25));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		
	//LADO IZQUIERDO DE LA CASA VENTANA DERECHA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.351f, 0.2f, -3.75));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

	//LADO IZQUIERDO DE LA CASA VENTANA IZQUIERDA CUADRO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.351f, 0.2f, -4.25));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		
	//ESFERA AZUL RETAGUARDIA DE LA CASA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.0f, 0.0f, -4.5));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

	//TROCO DEL ARBOL DERECHO CILINDRO CAFE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.399f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

	//TROCO DEL ARBOL IZQUIERDO CILINDRO CAFE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, -0.399f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.478f, 0.255f, 0.067f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

	//HOJAS DEL ARBOL DERECHO CONO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

	//HOJAS DEL ARBOL IZQUIERDO CONO VERDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, -0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[3]->RenderMeshGeometry();

	//SUPERFICIE DEL PISO CUADRADO GRIS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -4.0f));
		model = glm::scale(model, glm::vec3(6.0f, 0.2f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();
		*/
		
		// -------------------- Grupo: rotación alrededor del pivote --------------------
		glm::vec3 pivot = glm::vec3(0.0f, 0.0f, -4.0f);
		glm::mat4 parentModel = glm::mat4(1.0f);

		// Construir parentModel para rotar alrededor de 'pivot':
		// parentModel = T(pivot) * Rz * Ry * Rx * T(-pivot)
		parentModel = glm::translate(parentModel, pivot);
		parentModel = glm::rotate(parentModel, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		parentModel = glm::rotate(parentModel, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		parentModel = glm::rotate(parentModel, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		parentModel = glm::translate(parentModel, -pivot);

		// -------------------- Pirámide base --------------------
		shaderList[0].useShader();
		glm::mat4 model = glm::mat4(1.0f);

		// conserva la posicion local original del objeto principal
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = parentModel * model; // aplicar la rotación/traslación del grupo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[5]->RenderMesh();

		// -------------------- Pirámides pequeñas  --------------------
		shaderList[1].useShader();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		auto renderChild = [&](glm::vec3 pos, glm::vec3 scaleVec, bool hasLocalRot = false, glm::mat4 localRot = glm::mat4(1.0f)) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			model = glm::scale(model, scaleVec);
			if (hasLocalRot) {
				// si localRot no es identidad, aplicarla 
				model = model * localRot;
			}
			glm::mat4 finalModel = parentModel * model; // aplica rotación/traslación del grupo
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(finalModel));
			meshColorList[0]->RenderMeshColor();
			};

		// --- Triangulos rectos (Azul) ---
		renderChild(glm::vec3(0.0f, 0.02662f, -3.556f), glm::vec3(0.26666f));
		renderChild(glm::vec3(0.1925f, 0.02662f, -3.889f), glm::vec3(0.26666f));
		renderChild(glm::vec3(0.385f, 0.02662f, -4.222f), glm::vec3(0.26666f));
		renderChild(glm::vec3(0.0f, 0.340325f, -3.778f), glm::vec3(0.26666f));
		renderChild(glm::vec3(0.1925f, 0.340325f, -4.111f), glm::vec3(0.26666f));
		renderChild(glm::vec3(0.0f, 0.654f, -4.0f), glm::vec3(0.26666f));

		// --- Triangulos rectos (Rojo) ---
		renderChild(glm::vec3(-0.1925f, 0.02662f, -3.889f), glm::vec3(0.26666f));
		renderChild(glm::vec3(-0.385f, 0.02662f, -4.222f), glm::vec3(0.26666f));
		renderChild(glm::vec3(-0.1925f, 0.340325f, -4.111f), glm::vec3(0.26666f));

		// --- Triangulos rectos (Verde) ---
		renderChild(glm::vec3(0.0f, 0.02662f, -4.222f), glm::vec3(0.26666f));

		// -------------------- Triangulos invertidos --------------------

		// Azul (invertido)
		rotx = glm::angleAxis(glm::radians(210.0f), glm::vec3(1, 0, 0));
		roty = glm::angleAxis(glm::radians(50.1f), glm::vec3(0, 1, 0));
		rotz = glm::angleAxis(glm::radians(-58.7f), glm::vec3(0, 0, 1));
		rot = rotx * roty * rotz;
		localRot = glm::toMat4(rot);

		renderChild(glm::vec3(0.0288f, 0.2478f, -3.76f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(0.22f, 0.2478f, -4.092f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(0.031f, 0.56f, -3.98f), glm::vec3(0.26666f), true, localRot);

		// Rojo (invertido)
		rotx = glm::angleAxis(glm::radians(210.0f), glm::vec3(1, 0, 0));
		roty = glm::angleAxis(glm::radians(-50.1f), glm::vec3(0, 1, 0));
		rotz = glm::angleAxis(glm::radians(58.7f), glm::vec3(0, 0, 1));
		rot = rotx * roty * rotz;
		localRot = glm::toMat4(rot);

		renderChild(glm::vec3(-0.0288f, 0.2478f, -3.76f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(-0.22f, 0.2478f, -4.092f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(-0.031f, 0.56f, -3.98f), glm::vec3(0.26666f), true, localRot);

		// Verde (invertido)
		rotx = glm::angleAxis(glm::radians(10.0f), glm::vec3(1, 0, 0));
		roty = glm::angleAxis(glm::radians(-17.0f), glm::vec3(0, 1, 0));
		rotz = glm::angleAxis(glm::radians(58.8f), glm::vec3(0, 0, 1));
		rot = rotx * roty * rotz;
		localRot = glm::toMat4(rot);

		renderChild(glm::vec3(0.245f, 0.159f, -4.18f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(-0.14f, 0.159f, -4.18f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(0.0535f, 0.475f, -4.066f), glm::vec3(0.26666f), true, localRot);

		// Amarillo (invertido con rotacion 180° en Y)
		rotx = glm::angleAxis(glm::radians(0.0f), glm::vec3(1, 0, 0));
		roty = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
		rotz = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 0, 1));
		rot = rotx * roty * rotz;
		localRot = glm::toMat4(rot);

		renderChild(glm::vec3(0.0f, 0.0266f, -3.776f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(-0.1925f, 0.0266f, -4.1085f), glm::vec3(0.26666f), true, localRot);
		renderChild(glm::vec3(0.1925f, 0.0266f, -4.1085f), glm::vec3(0.26666f), true, localRot);

		// -------------------- Finalizar --------------------
		glUseProgram(0);
		mainWindow.swapBuffers();
			}
	return 0;	
}

	
		