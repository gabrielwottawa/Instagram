#include "Manager.h"
#include <string.h>

static bool keys[1024];
static bool resized;
static GLuint width, height;

int channel = 0;
int emoji = 0;
bool moveObj = false;
bool reset = false;
bool changeImage = false;
double posX, posY = 0;
double posX1, posY1 = 0;
char image[50];

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	initializeGraphics();
}

void Manager::initializeGraphics()
{
	glfwInit();
	window = glfwCreateWindow(width, height, "Instagram", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	cout << "Caso a versão suportada seja menor que 450, devera ser mudada como mostrado no README.md" << endl;
	cout << "" << endl;
	cout << "Aplicador de Filtros!" << endl;
	cout << "Instruções:            R           - Resetar filtro e stickers." << endl;
	cout << "                       I           - Trocar imagem." << endl;
	cout << "             MOUSE BOTAO ESQUERDO  - Selecionar um filtro ou sticker." << endl;
	cout << "             MOUSE BOTAO DIREITO   - Colocar sticker no lugar do clique." << endl;
	cout << "                      ESC          - Sair." << endl;
	cout << "" << endl;
	
	addShader("../shaders/transformations.vs", "../shaders/transformations.fs");
	setupScene();

	resized = true;
}

void Manager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void Manager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		channel = 0;
		reset = true;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		changeImage = true;
	}
}

void Manager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &posX, &posY);
		if (posX >= 90 && posX <= 170 && posY >= 490 && posY <= 570) {
			channel = 1;
		}
		else if (posX >= 180 && posX <= 260 && posY >= 490 && posY <= 570) {
			channel = 2;
		}
		else if (posX >= 270 && posX <= 350 && posY >= 490 && posY <= 570) {
			channel = 3;
		}
		else if (posX >= 360 && posX <= 440 && posY >= 490 && posY <= 570) {
			channel = 4;
		}
		else if (posX >= 450 && posX <= 530 && posY >= 490 && posY <= 570) {
			channel = 5;
		}
		else if (posX >= 540 && posX <= 620 && posY >= 490 && posY <= 570) {
			channel = 6;
		}
		else if (posX >= 630 && posX <= 710 && posY >= 490 && posY <= 570) {
			channel = 7;
		}

		else if (posX >= 195 && posX <= 245 && posY >= 25 && posY <= 75) {
			emoji = 8;
		}
		else if (posX >= 265 && posX <= 315 && posY >= 25 && posY <= 75) {
			emoji = 9;
		}
		else if (posX >= 335 && posX <= 385 && posY >= 25 && posY <= 75) {
			emoji = 10;
		}
		else if (posX >= 405 && posX <= 455 && posY >= 25 && posY <= 75) {
			emoji = 11;
		}
		else if (posX >= 475 && posX <= 525 && posY >= 25 && posY <= 75) {
			emoji = 12;
		}
		else if (posX >= 545 && posX <= 595 && posY >= 25 && posY <= 75) {
			emoji = 13;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && emoji != 0) {
		glfwGetCursorPos(window, &posX1, &posY1);
		moveObj = true;
	}
}

void Manager::resize(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	resized = true;
	glViewport(0, 0, width, height);
}


void Manager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	objects[7]->setChannel(channel);

	if (emoji != 0 && moveObj == true) {
		objects[emoji]->setPosition(glm::vec3(posX1, 600 - posY1, 0.0f));
		moveObj = false;
	}

	if (changeImage) {
		cout << "Coloque uma imagem na pasta '/textures' e digite o nome dela abaixo: (Ex.: imagem.png ou imagem.jpg)" << endl;
		cin >> image;
		char dir[100] = "../textures/";
		strcat_s(dir, 100, image);
		unsigned int texID = loadTexture(dir);
		objects[7]->setTexture(texID);
		changeImage = false;
	}

	if (reset) {
		for (int i = 8; i < 14; i++)
		{
			objects[i]->setPosition(glm::vec3(1000.0f, 1000.0f, 0.0f));
		}
		reset = false;
	}
}

void Manager::render()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized)
	{
		setupCamera2D();
		resized = false;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw(objects[i]->getChannel());
	}
}

void Manager::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		update();

		render();

		glfwSwapBuffers(window);
	}
}

void Manager::finish()
{
	glfwTerminate();
}


void Manager::setupScene()
{
	Sprite* obj = new Sprite;

	setSpriteFilters(obj);

	setSpriteObject(obj);

	setSpriteEmojis(obj);

	setSpriteOthers(obj);

	setSpriteEmoji(obj);

	unsigned int texID = loadTexture("../textures/lena.png");

	setTextureImageMain(texID);
	setTextureImageEmojis(texID);
	setTextureImageOthers(texID);

	ortho2D[0] = 0.0f;
	ortho2D[1] = 800.0f;
	ortho2D[2] = 0.0f;
	ortho2D[3] = 600.0f; 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Manager::setTextureImageOthers(unsigned int& texID)
{
	texID = loadTexture("../textures/Stickers.png");
	objects[14]->setTexture(texID);
	texID = loadTexture("../textures/Filtros.png");
	objects[15]->setTexture(texID);
}

void Manager::setTextureImageEmojis(unsigned int& texID)
{
	texID = loadTexture("../textures/Emoji1.png");
	objects[8]->setTexture(texID);
	objects[16]->setTexture(texID);
	texID = loadTexture("../textures/Emoji2.png");
	objects[9]->setTexture(texID);
	objects[17]->setTexture(texID);
	texID = loadTexture("../textures/Emoji3.png");
	objects[10]->setTexture(texID);
	objects[18]->setTexture(texID);
	texID = loadTexture("../textures/Emoji4.png");
	objects[11]->setTexture(texID);
	objects[19]->setTexture(texID);
	texID = loadTexture("../textures/Emoji5.png");
	objects[12]->setTexture(texID);
	objects[20]->setTexture(texID);
	texID = loadTexture("../textures/Emoji6.png");
	objects[13]->setTexture(texID);
	objects[21]->setTexture(texID);
}

void Manager::setTextureImageMain(unsigned int texID)
{
	objects[0]->setTexture(texID);
	objects[1]->setTexture(texID);
	objects[2]->setTexture(texID);
	objects[3]->setTexture(texID);
	objects[4]->setTexture(texID);
	objects[5]->setTexture(texID);
	objects[6]->setTexture(texID);
	objects[7]->setTexture(texID);
}

void Manager::setSpriteOthers(Sprite*& obj)
{
	obj = new Sprite;
	obj->setPosition(glm::vec3(110.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(150.0f, 100.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);

	obj = new Sprite;
	obj->setPosition(glm::vec3(43.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(100.0f, 100.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);
}

void Manager::setSpriteObject(Sprite*& obj)
{
	obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 330.0f, 0.0));
	obj->setDimension(glm::vec3(300.0f, 300.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(channel);
	objects.push_back(obj); 
}

void Manager::setSpriteEmojis(Sprite*& obj)
{
	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(290.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(360.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(500.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(570.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);
}

void Manager::setSpriteFilters(Sprite*& obj)
{
	obj->setPosition(glm::vec3(130.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f)); 
	obj->setShader(shader);
	obj->setChannel(1);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(2);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(310.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(3);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(4);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(490.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(5);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(580.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(6);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(670.0f, 70.0f, 0.0));
	obj->setDimension(glm::vec3(80.0f, 80.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(7);
	objects.push_back(obj);
}

void Manager::setSpriteEmoji(Sprite*& obj)
{
	obj = new Sprite;
	obj->setPosition(glm::vec3(220.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);

	obj = new Sprite;
	obj->setPosition(glm::vec3(290.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj); 

	obj = new Sprite;
	obj->setPosition(glm::vec3(360.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);

	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);

	obj = new Sprite;
	obj->setPosition(glm::vec3(500.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);

	obj = new Sprite;
	obj->setPosition(glm::vec3(570.0f, 550.0f, 0.0));
	obj->setDimension(glm::vec3(50.0f, 50.0f, 1.0f));
	obj->setShader(shader);
	obj->setChannel(0);
	objects.push_back(obj);
}

void Manager::setupCamera2D()
{
	float zNear = -1.0, zFar = 1.0;
	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int Manager::loadTexture(string filename)
{
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Falha no carregamento da textura." << std::endl;
		std::cout << "Aperte I para tentar carregar a imagem novamente e certifique-se de que a mesma está na pasta '/textures'." << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	return texture1;
}