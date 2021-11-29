#pragma once

#include "Shader.h"
#include "Sprite.h" 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class Manager
{
public:
	Manager();
	~Manager();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void resize(GLFWwindow* window, int width, int height);

	void initialize(GLuint width, GLuint height);
	void run();
	void finish();
	void update();
	void render();
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene(); //antigo setupGeometry
	void setTextureImageOthers(unsigned int& texID);
	void setTextureImageEmojis(unsigned int& texID);
	void setTextureImageMain(unsigned int texID);
	void setSpriteOthers(Sprite*& obj);
	void setSpriteObject(Sprite*& obj);
	void setSpriteEmojis(Sprite*& obj);
	void setSpriteFilters(Sprite*& obj);
	void setSpriteEmoji(Sprite*& obj);
	void setupCamera2D();
	unsigned int loadTexture(string filename);

private:
	GLFWwindow* window;
	Shader* shader;
	glm::vec4 ortho2D;
	glm::mat4 projection;

	vector <Sprite*> objects;
};