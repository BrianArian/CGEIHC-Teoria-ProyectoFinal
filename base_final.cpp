/*
Semestre 2022-1
Práctica: Animación Sencilla y animación compleja
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

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
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

//Para importar sonido 
#include <irrklang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")



const float toRadians = 3.14159265f / 180.0f;



float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

float rothel,brazoder,brazoizq,piernader,piernaizq;
float rotheliof;
float rotaheli;
float rotau;
float lampaA;
float lampaB;
float helicoz;
float movheli;
float heli;
bool adelbrazoder, adelbrazoizq, atrasbrazoder, atrasbrazoizq, adelpiernader, adelpiernaizq, atraspiernader, atraspiernaizq;

//Dan
bool direc = true;
float rotalamp;
float rotaluz;
float apaspo;
int pointLightCount = 0;

//arian
float movCoche;
float movcoz;
float movCoche2;
float movcoz2;
float movOffset;
float rotllanta;
float rotllanta2;
float rotllantaOffset;
int avanza;
int avanza2;
//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

//
float movtankx;
float movtankz;
float caso;

float movavix;
float movaviz;
float rotavi;

bool inicio = true;
bool direct = true;
float rotan;
float tiemp;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Puerta1;
Model Puerta2;

Model renault_obj;
Model llanta_obj;
Model helice_m;
Model heli_obj;
Model Brazo_der;
Model Brazo_izq;
Model Pierna_der;
Model Pierna_izq;
Model Torzo;
Model Pista;
Model CrashBox;
Model CrashPyramid;
Model refle;
Texture madera;
Model Tank;
Model avi1;
Model anti;
Model casa;

Skybox skybox;
Skybox skyboxd;
Skybox skyboxn;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;



// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
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


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = -16.5, posYavion = 0.5, posZavion = -40;
float	movAvion_x = 0.0f, movAvion_y = 0.0f,movAvion_z=0.0;
float giroAvion = 0;
float giraItem = 0;

#define MAX_FRAMES 30 //recomendable 100 o 200 para las animaciones, dependiendo de la animación
int i_max_steps = 90;
int i_curr_steps = 3; //debe de tener el mismo valor de Frame index
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para PosicionZ
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 3;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroAvion = giroAvion;

	FrameIndex++;
}

void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



int main(int argc, char* argv[])
{
	

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearCubo();

	//////////////////////////////////////////////////////////////////77///SONIDO////


	//irrklang::ISoundEngine* SoundEngine = createIrrKlangDevice();
	//SoundEngine->play2D("Audio/war.mp3", true);
//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movAvion_x = 0.0;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 20;
	KeyFrame[0].giroAvion = -45;

	KeyFrame[1].movAvion_x = 0.0f;
	KeyFrame[1].movAvion_y = 0.0f;
	KeyFrame[1].movAvion_z = 20.0f;
	KeyFrame[1].giroAvion = 45;

	KeyFrame[2].movAvion_x = 0.0;
	KeyFrame[2].movAvion_y = -0.1;
	KeyFrame[2].movAvion_z = 25.0;
	KeyFrame[2].giroAvion = -45;

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture= Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Brazo_der = Model();
	Brazo_der.LoadModel("Models/soldier/Soldado-brazo-derecho.obj");
	Brazo_izq = Model();
	Brazo_izq.LoadModel("Models/soldier/Soldado-brazo-izquierdo.obj");
	Pierna_der = Model();
	Pierna_der.LoadModel("Models/soldier/Soldado-pierna-derecha.obj");
	Pierna_izq = Model();
	Pierna_izq.LoadModel("Models/soldier/Soldado-pierna-izquierda.obj");
	Torzo = Model();
	Torzo.LoadModel("Models/soldier/Soldado-torzo.obj");
	CrashBox = Model();
	CrashBox.LoadModel("Models/crabox/CrashBox.obj");

	CrashPyramid = Model();
	CrashPyramid.LoadModel("Models/crahpira/CrashPyramid.obj");
	Pista = Model();
	Pista.LoadModel("Models/pista.obj");

	refle = Model();
	refle.LoadModel("Models/refle/ref.obj");

	Tank = Model();
	Tank.LoadModel("Models/trineo.obj");

	avi1 = Model();
	avi1.LoadModel("Models/avion/avi1.obj");

	anti = Model();
	anti.LoadModel("Models/anti/anti1.obj");

	casa = Model();
	casa.LoadModel("Models/house.obj");




	//Pista = Model();
	//Pista.LoadModel("Models/Pista.obj");


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.3f,
								0.0f, 0.0f, -1.0f);
//contador de luces puntuales
	//unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
								0.0f, 1.0f,
								2.0f, 1.5f,1.5f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	unsigned int spotLightCount = 0;
	
	//Luz luminaria 1
	pointLights[0] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 0.5f,
		13.5f, 1.95f, -32.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//Luz luminaria 2
	pointLights[1] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 0.5f,
		13.5f, 1.95f, -8.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	////Luz luminaria 3
	pointLights[2] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 0.5f,
		13.5f, 1.95f, 25.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//luz fija
	spotLights[0] = SpotLight(0.5f, 0.5f, 0.5f,
		0.001f, 0.020f,
		-23.5f, 15.5f, 2.0f,
		1.0f, -0.3f, 0.10f,
		0.5f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(0.5f, 0.5f, 0.5f,
		0.001f, 0.020f,
		15.6f, 20.0f, -15.00f,
		-1.50f, -0.5f, 0.0f,
		0.5f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_right1.png");//RIGHT
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_left2.png");//LEFT
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_bottom4.png");//DOWN
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_top3.png");//UP
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_back6.png");//BACK
	skyboxFaces.push_back("Textures/Skybox/SpaceboxCollection/Spacebox6/SkyBlue2_front5.png"); //FRONT

	std::vector<std::string> skyboxFacesN;
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_right.png");//RIGHT
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_left.png");//LEFT
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_bottom.png");//DOWN
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_top.png");//UP
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_back.png");//BACK
	skyboxFacesN.push_back("Textures/Skybox/SpaceboxCollection/Spacebox1/_front.png"); //FRONT

	//skybox = Skybox(skyboxFaces);
	skyboxd = Skybox(skyboxFaces);
	skyboxn = Skybox(skyboxFacesN);

	skybox = skyboxd;
	rotalamp = 90;
	rotaluz = 1;
	rotan = 90;
	tiemp = 1;
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movcoz = 12.0f;
	movCoche = 30.0f;
	movcoz2 = 11.5f;
	movCoche2 = 29.0f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	rotaheli = 0.0f;
	rotheliof = 100.0f;
	avanza = 0;
	avanza2 = 0;
	movheli = -6.5f;
	heli = 0;
	rotau = 90;
	rotheliof = 10.0;
	rothel = 0.0f;

//Tanque
movtankx =-16.0f;
movtankz=-30.0f;
//avion
movavix = -16.0;
movaviz = -30.0;
rotavi = 0;


 caso=0;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		giraItem += 2.0 * deltaTime;

		//printf("deltaTime:%f\n",&deltaTime);
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[2].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, pointLightCount);
		//Apagar y pender luces spot
		if (mainWindow.getapagarlinterna() < 0)
		{
			spotLightCount = 2;
			shaderList[0].SetSpotLights(spotLights, spotLightCount);

			if (direc == true) {

				if (rotaluz < 1) {
					rotaluz += deltaTime * 2.0f;
					rotalamp += deltaTime * 35.0f;
				}
				else {
					direc = false;
				}
			}
			if (direc == false) {

				if (rotaluz > -3) {
					rotalamp -= deltaTime * 35.0f;
					rotaluz -= deltaTime * 2.0f;
				}
				else {
					direc = true;
				}
			}



		}
		else
		{
			spotLightCount = 0;
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}

		//*****************************avi y tank******************************************
		if (avanza == 0)
		{			
			if (movtankz < 30.0f)
			{
				movtankz += movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
				rotau = 0;
			}
			////arriba
			if (movtankx < 12.0f && movtankz> 30.0f)
			{
				movtankx += movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
				rotau = 90.0f;

			}
			if (movtankx > 12.0f && movtankz >= -30.0f)
			{
				avanza = 1;
			}
		}

		if (avanza == 1)
		{
			if (movtankz > -30.0f)
			{
				movtankz -= movOffset * deltaTime;
				rotau = -180.0f;
			}
			if (movtankx > -16.5f && movtankz < -30.0f)
			{
				movtankx -= movOffset * deltaTime;
				rotau = -90.0f;
			}

			if (movtankx < -16.5f && movtankz < -30.0f)
			{
				avanza = 0;
			}


		}
/******************************************************Avión***********************/
		if (mainWindow.getavanzas() < 0)
		{
		
				movaviz += movOffset * deltaTime;
			

		}
		else {
			movaviz = -30;
		}
	

		if (inicio == true)
		{

			if (direct == true) {

				if (tiemp < 1) {
					tiemp += deltaTime * 2.0f;
					rotan += deltaTime * 35.0f;

				}
				else {
					direct = false;
				}
			}
			if (direct == false) {

				if (tiemp > -3) {
					tiemp -= deltaTime * 2.0f;
					rotan -= deltaTime * 10.0f;
				}
				else {
					direct = true;
				}
			}

		}
		

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxx(1.0);
		glm::mat4 soldado(1.0);
		glm::mat4 modelpuer(1.0);
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pista.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.0f, 1.0f, 42.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casa.RenderModel();
		/*****************************************imple avi trineo*****************************************************/
		model = glm::mat4(1.0);
		modelauxx = model = glm::translate(model, glm::vec3(movtankx, 1.6f, movtankz));
		model = glm::rotate(model, rotau * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tank.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0, 15.5f, movaviz));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.050f, 0.050f, 0.050f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		avi1.RenderModel();
		////////////////Reflectores////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.6f, 12.0f, -12.00f));
		model = glm::rotate(model, rotalamp * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		refle.RenderModel();
		spotLights[0].SetPos(glm::vec3(1.0f, -0.3f, -rotaluz));
		spotLights[1].SetPos(glm::vec3(rotaluz, -0.5f, 1.0f));
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.5f, 12.0f, 2.0f));
		model = glm::rotate(model, rotan * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		anti.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

	

		/**************************************Caja crash*****************************************************/
		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(15.0f, 1.0f, 30.0f));
		model = glm::rotate(model, giraItem * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity,uniformShininess);
		CrashBox.RenderModel();

		model = modelaux;
		model = glm::rotate(model, giraItem * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CrashPyramid.RenderModel();

/*************************************SOLDADO**********************************************************/
			model = glm::mat4(1.0);
			//model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(posXavion,posYavion,posZavion));
			soldado = model = glm::translate(model, glm::vec3(movAvion_x,movAvion_y, movAvion_z));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Torzo.RenderModel();
			
			model = soldado;
			model = glm::translate(model, glm::vec3(0.2f, 2.01f, -0.1f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			model = glm::rotate(model,giroAvion*toRadians,glm::vec3(1.0,0.0,0.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Brazo_der.RenderModel();

			model = soldado;
			model = glm::translate(model, glm::vec3(-0.2f,2.0f, -0.1f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			model=glm::rotate(model,-giroAvion * toRadians, glm::vec3(1.0, 0.0, 0.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Brazo_izq.RenderModel();

			model = soldado;
			model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			model = glm::rotate(model, giroAvion * toRadians, glm::vec3(1.0, 0.0, 0.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Pierna_der.RenderModel();

			model = soldado;
			model = glm::translate(model, glm::vec3(0.3f,1.5f, 0.0f));
			model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
			model = glm::rotate(model, -giroAvion * toRadians, glm::vec3(1.0, 0.0, 0.0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Pierna_izq.RenderModel();




		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
			glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_Y]) {
		skybox = skyboxd;
		pointLightCount = 3;
		
	}

	if (keys[GLFW_KEY_U]) {
		skybox = skyboxn;
		pointLightCount = 0;
	}


	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf(" \npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}

	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

	if (keys[GLFW_KEY_3])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x -= 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}

	if (keys[GLFW_KEY_4])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_y += 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}

	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_y -= 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}

	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			giroAvion += 1.0f;
			printf("\n movAvion_y es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}

}
