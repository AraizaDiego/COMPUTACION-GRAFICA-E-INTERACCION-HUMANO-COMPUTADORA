/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere 
heredar entre diversas instancias para que est�n unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
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
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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

// Pir�mide triangular regular
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
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
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

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los v�rtices de la circunferencia del cono
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

//funci�n para crear pir�mide cuadrangular unitaria
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



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(10, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CreateShaders();
	
	

	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
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
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a
	glm::mat4 modelauxC(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de la cola
	glm::mat4 modelauxPiv(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a del pivote para las articulaciones
	glm::mat4 modelauxP(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de las patas
	glm::mat4 modelauxPD1(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de la pata delantera izquierda
	glm::mat4 modelauxPD2(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de la pata delantera derecha
	glm::mat4 modelauxPT1(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de la pata trasera izquierda
	glm::mat4 modelauxPT2(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarqu�a de la pata trasera derecha

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		/*
		//CREANDO LA CABINA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -4.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 2.0f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.8f, 0.8f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		//ARTICULACION1 CABINA-BRAZO
		//model = glm::mat4(1.0);//NO EXISTE, SUSTITUIRLA POR:
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();

		// Creando el brazo de una gr�a
		//articulacion1 hasta articulaci�n5 s�lo son puntos de rotaci�n o articulaci�n, en este caso no dibujaremos esferas que los representen

		//primer brazo que conecta con la cabina
		//para reiniciar la matriz de modelo con valor de la matriz identidad
		//model = glm::mat4(1.0);
		//rotaci�n alrededor de la articulaci�n que une con la cabina
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//Traslaci�n inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
		//se programe cambio entre proyecci�n ortogonal y perspectiva
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro, cono, pir�mide base cuadrangular
		//sp.render(); //dibuja esfera

		//SEGUNDA ARTICULACION 
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		sp.render(); //dibuja esfera

		//segundo brazo

		//para reiniciar la matriz de modelo con valor de la matriz identidad
		//Comentar y se modifica para agregar la jerarquia:
		//usar una matriz temporal o auxiliar

		//model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		//Traslaci�n inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::translate(model, glm::vec3(-0.30f, 5.6f, 0.0f));
		//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
		//se programe cambio entre proyecci�n ortogonal y perspectiva
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pir�mide triangular

		//TERCER ARTICULACION
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//tercer brazo
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		meshList[0]->RenderMesh(); 

		//CUARTA ARTICULACION 
		model = modelaux;;
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.76f, 1.76f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();
		
		//CANASTA
		model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 2.2f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CUERPO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, 4.5f, -4.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(7.5f, 4.0f, 1.8f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.7f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//BASE
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(8.5f, 2.5f, 2.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();

		//LLANTA 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.25f, -1.25f, 1.9f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		//LLANTA 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -3.8, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//LLANTA 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//LLANTA 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.25f, -3.8f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();*/

		//CREANDO EL CUERPO SUPERIOR
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, -4.0f));
		modelaux = model;
		modelauxP = model;
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		//CREANDO PANZA SUPERIOR
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.199f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.6f, 1.5f, 2.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9294f, 0.8078f, 0.6941f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO CUERPO MEDIO
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.65f, 0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(4.6f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO PANZA MEDIO
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.65f, -0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.8f, 1.5f, 2.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9294f, 0.8078f, 0.6941f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO CUERPO INFERIOR
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.95f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(2.3f, 2.37f, 2.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION COLA1
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.75f, 0.6f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxPiv= model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO COLA1
		model = glm::translate(model, glm::vec3(1.15f, 0.3f, 0.0f));
		modelauxC = model;
		model = glm::rotate(model, glm::radians(16.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.9f, 1.25f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION COLA2
		model = modelauxC;
		model = glm::translate(model, glm::vec3(0.7f, 0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO COLA2
		model = glm::translate(model, glm::vec3(1.20f, 0.6f, 0.0f));
		modelauxC = model;
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.06f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION COLA3
		model = modelauxC;
		model = glm::translate(model, glm::vec3(0.4f, 0.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO COLA3
		model = glm::translate(model, glm::vec3(0.7f, 0.6f, 0.0f));
		modelauxC = model;
		model = glm::rotate(model, glm::radians(47.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.83f, 0.65f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION COLA4
		model = modelauxC;
		model = glm::translate(model, glm::vec3(0.2f, 0.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO COLA4
		model = glm::translate(model, glm::vec3(0.3f, 0.7f, 0.0f));
		model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.6f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//-------------------------------------------------------------

		//CREANDO CUELLO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, 1.2f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.8f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO CUELLO2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.8f, 1.5f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9294f, 0.8078f, 0.6941f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO COLLAR
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, 0.0f, 0.0f));;
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 2.1f, 1.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO EBILLA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -1.1f, 0.0f));;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.15f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5960f, 0.1607f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//CREANDO CRANEO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.875f, 1.875f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO HOCICO
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.8f, -0.52f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 1.12f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO HOCICO2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.3f, -1.05f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8039f, 0.5607f, 0.3764f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO NARIZ
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.9f, -0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.366f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.14f, 0.03f, 0.011f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OREJA1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 2.15f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.8f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2580f, 0.1240f, 0.0837f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OREJA2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -2.15f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.8f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2580f, 0.1240f, 0.0837f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO1_1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.85f, 0.37f, 0.75f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.665f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.00f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO1_2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.87f, 0.37f, 0.79f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.62f, 0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.36f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO1_3
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.88f, 0.36f, 0.81f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.58f, 0.42f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO2_1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.85f, 0.37f, -0.75f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.665f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.00f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO2_2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.87f, 0.37f, -0.79f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.62f, 0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.36f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//CREANDO OJO2_3
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.88f, 0.36f, -0.81f));
		model = glm::rotate(model, glm::radians(-6.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.58f, 0.42f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render(); //dibuja esfera

		//-------------------------------------------------------------

		//ARTICULACION PATA DELANTERA IZQUIERDA1
		model = modelauxP;
		model = glm::translate(model, glm::vec3(-1.35f, 0.25f, 1.5f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA DELANTERA IZQUIERDA1
		model = glm::translate(model, glm::vec3(0.53f, -1.38f, 0.25f));
		modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA DELANTERA IZQUIERDA2
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(0.53f, -1.4f, 0.0f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA DELANTERA IZQUIERDA2
		model = glm::translate(model, glm::vec3(0.0f, -1.38f, 0.0f));
		modelauxPD1 = model;
		model = glm::scale(model, glm::vec3(1.5f, 2.8f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO PIE PATA DELANTERA IZUQUIERDA
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(-0.8f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2280f, 0.0790f, 0.0437f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA DELANTERA DERECHA1
		model = modelauxP;
		model = glm::translate(model, glm::vec3(-1.35f, 0.25f, -1.5f));
		//modelauxPD2= model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA DELANTERA DERECHA1
		model = glm::translate(model, glm::vec3(0.53f, -1.38f, -0.25f));
		modelauxPD2 = model;
		model = glm::rotate(model, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA DELANTERA DERECHA2
		model = modelauxPD2;
		model = glm::translate(model, glm::vec3(0.53f, -1.4f, 0.0f));
		//modelauxPD2 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA DELANTERA DERECHA2
		model = glm::translate(model, glm::vec3(0.0f, -1.38f, 0.0f));
		modelauxPD2 = model;
		model = glm::scale(model, glm::vec3(1.5f, 2.8f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO PIE PATA DELANTERA DERECHA
		model = modelauxPD2;
		model = glm::translate(model, glm::vec3(-0.8f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2280f, 0.0790f, 0.0437f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//-------------------------------------------------------------
		
		//ARTICULACION PATA TRASERA IZQUIERDA1
		model = modelauxP;
		model = glm::translate(model, glm::vec3(4.9f, 0.25f, 1.5f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA TRASERA IZQUIERDA1
		model = glm::translate(model, glm::vec3(0.53f, -1.38f, 0.25f));
		modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA TRASERA IZQUIERDA2
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(0.53f, -1.4f, 0.0f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA TRASERA IZQUIERDA2
		model = glm::translate(model, glm::vec3(0.0f, -1.38f, 0.0f));
		modelauxPD1 = model;
		model = glm::scale(model, glm::vec3(1.5f, 2.8f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO PIE PATA TRASERA IZQUIERDA
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(-0.8f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2280f, 0.0790f, 0.0437f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA TRASERA DERECHA1
		model = modelauxP;
		model = glm::translate(model, glm::vec3(4.9f, 0.25f, -1.5f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA TRASERA DERECHA1
		model = glm::translate(model, glm::vec3(0.53f, -1.38f, -0.25f));
		modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(21.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION PATA TRASERA DERECHA2
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(0.53f, -1.4f, 0.0f));
		//modelauxPD1 = model;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, -1.0f));
		modelauxPiv = model;
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.66f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2880f, 0.1390f, 0.1037f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		sp.render();
		model = modelauxPiv;

		//CREANDO PATA TRASERA DERECHA2
		model = glm::translate(model, glm::vec3(0.0f, -1.38f, 0.0f));
		modelauxPD1 = model;
		model = glm::scale(model, glm::vec3(1.5f, 2.8f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2980f, 0.1490f, 0.1137f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//CREANDO PIE PATA TRASERA DERECHA
		model = modelauxPD1;
		model = glm::translate(model, glm::vec3(-0.8f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.2280f, 0.0790f, 0.0437f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		