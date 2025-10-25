/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

#include "OrderLights.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dado8Texture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model F1Cuerpo_M;
Model F1Cofre_M;
Model LlantaDI_M;
Model LlantaDD_M;
Model LlantaTI_M;
Model LlantaTD_M;
Model Torch_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado2()
{
	unsigned int cubo_indices2[] = {
		// cara 0
		0, 1, 2,
		// cara 1
		3, 4, 5,
		// cara 2
		6, 7, 8,
		// cara 3
		9, 10, 11,
		// cara 4
		12, 13, 14,
		// cara 5
		15, 16, 17,
		// cara 6
		18, 19, 20,
		// cara 7
		21, 22, 23

	};
// average normals
	GLfloat cubo_vertices2[] = {
	// face 0 (top, front, right)
	// x      y      z      S     T        NX        NY        NZ
	 0.0f,  0.5f,  0.0f,   0.25f, 0.50f,  -0.577350f, -0.577350f, -0.577350f, //0
	 0.0f,  0.0f,  0.5f,   0.50f, 0.75f,  -0.577350f, -0.577350f, -0.577350f, //1
	 0.5f,  0.0f,  0.0f,   0.00f, 0.75f,  -0.577350f, -0.577350f, -0.577350f, //2

	// face 1 (top, right, back)
	 0.0f,  0.5f,  0.0f,   0.75f, 0.50f,  -0.577350f, -0.577350f,  0.577350f, //3
	 0.5f,  0.0f,  0.0f,   0.50f, 0.25f,  -0.577350f, -0.577350f,  0.577350f, //4
	 0.0f,  0.0f, -0.5f,   1.00f, 0.25f,  -0.577350f, -0.577350f,  0.577350f, //5

	// face 2 (top, back, left)
	 0.0f,  0.5f,  0.0f,   0.75f, 1.00f,   0.577350f, -0.577350f,  0.577350f, //6
	 0.0f,  0.0f, -0.5f,   0.50f, 0.75f,   0.577350f, -0.577350f,  0.577350f, //7
    -0.5f,  0.0f,  0.0f,   1.00f, 0.75f,   0.577350f, -0.577350f,  0.577350f, //8

	// face 3 (top, left, front)
	 0.0f,  0.5f,  0.0f,   0.50f, 0.25f,   0.577350f, -0.577350f, -0.577350f, //9
	-0.5f,  0.0f,  0.0f,   0.75f, 0.50f,   0.577350f, -0.577350f, -0.577350f, //10
	 0.0f,  0.0f,  0.5f,   0.25f, 0.50f,   0.577350f, -0.577350f, -0.577350f, //11

	// face 4 (bottom, right, front)
	 0.0f, -0.5f,  0.0f,   0.75f, 0.50f,  -0.577350f,  0.577350f, -0.577350f, //12
	 0.5f,  0.0f,  0.0f,   1.00f, 0.75f,  -0.577350f,  0.577350f, -0.577350f, //13
	 0.0f,  0.0f,  0.5f,   0.50f, 0.75f,  -0.577350f,  0.577350f, -0.577350f, //14

	// face 5 (bottom, back, right)
	 0.0f, -0.5f,  0.0f,   0.25f, 0.50f,  -0.577350f,  0.577350f,  0.577350f, //15
	 0.0f,  0.0f, -0.5f,   0.00f, 0.25f,  -0.577350f,  0.577350f,  0.577350f, //16
	 0.5f,  0.0f,  0.0f,   0.50f, 0.25f,  -0.577350f,  0.577350f,  0.577350f, //17

	// face 6 (bottom, left, back)
	 0.0f, -0.5f,  0.0f,   0.25f, 0.00f,   0.577350f,  0.577350f,  0.577350f, //18
	-0.5f,  0.0f,  0.0f,   0.50f, 0.25f,   0.577350f,  0.577350f,  0.577350f, //19
	 0.0f,  0.0f, -0.5f,   0.00f, 0.25f,   0.577350f,  0.577350f,  0.577350f, //20

	// face 7 (bottom, front, left)
	 0.0f, -0.5f,  0.0f,   0.50f, 0.75f,   0.577350f,  0.577350f, -0.577350f, //21
	 0.0f,  0.0f,  0.5f,   0.25f, 0.50f,   0.577350f,  0.577350f, -0.577350f, //22
	-0.5f,  0.0f,  0.0f,   0.75f, 0.50f,   0.577350f,  0.577350f, -0.577350f, //23
	};

	Mesh* dado2 = new Mesh();
	dado2->CreateMesh(cubo_vertices2, cubo_indices2, 192, 24);
	meshList.push_back(dado2);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado2();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dado8Texture = Texture("Textures/Dado8O.png");
	dado8Texture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	F1Cuerpo_M = Model();
	F1Cuerpo_M.LoadModel("Models/F1Cuerpo.obj");
	F1Cofre_M = Model();
	F1Cofre_M.LoadModel("Models/F1Cofre.obj");
	LlantaDI_M = Model();
	LlantaDI_M.LoadModel("Models/LlantaDI.obj");
	LlantaDD_M = Model();
	LlantaDD_M.LoadModel("Models/LlantaDD.obj");
	LlantaTI_M = Model();
	LlantaTI_M.LoadModel("Models/LlantaTI.obj");
	LlantaTD_M = Model();
	LlantaTD_M.LoadModel("Models/LlantaTD.obj");
	Torch_M = Model();
	Torch_M.LoadModel("Models/Torch.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		//Radación de la luz, intensidad del color (un valor grande se come el color)
		0.3f, 0.3f,			//Intensidad ambiental y difusa
		0.0f, 0.0f, -1.0f);	//Dirección de la luz
	//contador de luces puntuales

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		//Posición de la luz
		-6.0f, 1.5f, 1.5f,
		//Que tan grande es la luz
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		3.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 
	//FARO DEL AUTO 1
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		10.0f, 10.0f,
		16.8f, 1.0f, -5.28f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.05f,
		40.0f);
	spotLightCount++; 

	//FARO DEL AUTO 2
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f, //Color
		10.0f, 10.0f,							//Intensidad ambiental y difusa
		16.8f, 1.0f, 5.28f,						//Posición
		1.0f, 0.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		40.0f);									//Edge
	spotLightCount++;
	glm::vec3 Fdir1(1.0f, 0.0f, 0.0f);

	//LUZ HELICÓPTERO
	spotLights[4] = SpotLight(1.0f, 0.96f, 0.176f, //Color
		1.0f, 1.0f,							//Intensidad ambiental y difusa
		0.0f, 4.8f, 8.0f,						//Posición
		0.0f, -1.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		15.0f);									//Edge
	spotLightCount++;
	glm::vec3 Fdir2(0.0f, -1.0f, 0.0f);

	//LUZ TORCH
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,	//Color
		0.2f, 0.9f,									//Intensidad ambiental y difusa
		0.0f, 3.1f, -10.0f,							//Posición
		0.45f, 0.1f, 0.1f);							//Atenuación Constante, lineal y exponente
	pointLightCount++;

	//LUCES PREVIO 5 
	/*
	spotLights[0] = SpotLight(0.0f, 1.0f, 0.0f, //Color
		1.0f, 1.0f,								//Intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,						//Posición
		-1.0f, 0.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		10.0f);									//Edge
	spotLightCount++;

	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //Color
		1.0f, 1.0f,								//Intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,						//Posición
		-1.0f, 0.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		10.0f);									//Edge
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f, //Color
		1.0f, 1.0f,								//Intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,						//Posición
		-1.0f, 0.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		10.0f);									//Edge
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f, //Color
		1.0f, 1.0f,								//Intensidad ambiental y difusa
		0.0f, 0.0f, 0.0f,						//Posición
		-1.0f, 0.0f, 0.0f,						//Dirección
		0.0f, 0.0f, 0.05f,						//Atenuación Constante, lineal y exponente
		10.0f);									//Edge
	spotLightCount++;

	OrderLights OrderLights(spotLightCount, 2.0f); //Crear instancia de OrderLights
	OrderLights.AddPattern({ 0,1,2,3 });
	OrderLights.AddPattern({ 3,0,1,2 }); */

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//OrderLights.SetIndexPattern(mainWindow.getsetPattern());
		//OrderLights.UsePattern(spotLights);

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		glm::vec3 Fpos1 (16.9f + mainWindow.getmuevex(), 1.0f, -5.28f);
		spotLights[2].SetFlash(Fpos1, Fdir1);

		glm::vec3 Fpos2 (16.9f + mainWindow.getmuevex(), 1.0f, 5.28f);
		spotLights[3].SetFlash(Fpos2, Fdir1);

		glm::vec3 Fpos3 (0 + mainWindow.getmuevex2(), 4.8f, 8.0f);
		spotLights[4].SetFlash(Fpos3, Fdir2);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		if (mainWindow.getOnLigth())
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{
			pointLightCount--;
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			pointLightCount++;
		}
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//DADO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 4.5f, -12.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[4]->RenderMesh();

		//CUERPO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 2.08f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		F1Cuerpo_M.RenderModel();

		//COFRE
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.5846875f, 1.8315f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		F1Cofre_M.RenderModel();

		//LLANTA DELANTERA IZQUIERDA
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.175f, -0.6375f, -5.7375f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDI_M.RenderModel();

		//LLANTA DELANTERA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.175f, -0.7175f, 5.7375f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDD_M.RenderModel();

		//LLANTA DELANTERA IZQUIERDA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-13.65, -0.6795f, -5.325f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTI_M.RenderModel();

		//LLANTA DELANTERA DERECHA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-13.65f, -0.7005f, 5.4375f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTD_M.RenderModel();
	
		//HELICÓPTERO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex2(), 5.0f, 8.0));
		modelaux = model;
		//spotLights[4].SetPos(glm::vec3(modelaux[3].x, -0.2f + modelaux[3].y, modelaux[3].z));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//TORCH
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -10.0f));
		modelaux = model;
		pointLights[1].SetPos(glm::vec3(modelaux[3].x, 4.1f + modelaux[3].y, modelaux[3].z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Torch_M.RenderModel();
		glDisable(GL_BLEND);

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 1.0f, -8.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
