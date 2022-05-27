#include "Object3D.h"

#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object3D::Object3D()
{
	mVAO = -1;
	mVBO = -1;;
	mEBO = -1;
	mTexture = -1;

	mVertexData.clear();
	mIndexData.clear();

	mShaderforDraw = nullptr;

	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotationDegrees = glm::vec3(0.0f, 0.0f, 0.0f);
	mScale = glm::vec3(1.0f, 1.0f, 1.0f);
}


Object3D::~Object3D()
{
	mVertexData.clear();
	mIndexData.clear();
}

void Object3D::init()
{
}

void Object3D::loadObjFromDisk(std::string file)
{
	std::string line;
	bool readVBO = false;
	bool readEBO = false;
	FileManager File(file.c_str(), "r");
	if (File.isOpened()) {
		while (!File.isEOF()) {
			line = File.getLine();
			if (line[0] != '#') {		// Ignore comments
				std::string dash = line.substr(0, 3);
				if (dash == "VBO") {
					readVBO = true;
					readEBO = false;
					continue;
				}
				if (dash == "EBO") {
					readVBO = false;
					readEBO = true;
					continue;
				}
				std::vector<std::string> data = splitString(line, ',');
				if (readVBO) {
					for (unsigned int i = 0; i < data.size(); i++) {
						mVertexData.push_back(atof(data[i].c_str()));
					}
				}
				if (readEBO) {
					for (unsigned int i = 0; i < data.size(); i++) {
						mIndexData.push_back(atoi(data[i].c_str()));
					}
				}
			}
		}
		File.close();
	}
	prepareObjGL();
}

void Object3D::loadTextureFromDisk(std::string file)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
	printf("Loaded texture \"%s\" with %d channels.\n", file.c_str(), nrChannels);

	//TODO maybe use nrChannels?
	GLint Mode = GL_RGB;
	//if (tempSurface->format->BytesPerPixel == 4)
	//	Mode = GL_RGBA;

	if (data)
	{
		glGenTextures(1, &mTexture);
		glBindTexture(GL_TEXTURE_2D, mTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, width, height, 0, Mode, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		//Texture wrapping/filtering options:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		printf("Failed to load texture \"%s\".\n", file.c_str());
	}
	stbi_image_free(data);
}

void Object3D::generateObj()
{
	//VBO data
	int	bytesData = 32;
	GLfloat vertexData[32] =
	{
		// x,y,z			u,v(s,t)	color
		-3.5f, 0.0f,  3.5f,	0.0,0.0,	0.0, 0.0, 1.0,
		 3.5f, 0.0f,  3.5f,	1.0,0.0,	0.0, 0.0, 1.0,
		 3.5f, 0.0f, -3.5f,	1.0,1.0,	0.0, 0.0, 1.0,
		-3.5f, 0.0f, -3.5f,	0.0,1.0,	0.0, 0.0, 1.0,
	};

	//EBO data
	GLuint indexData[6] =
	{
		0, 1, 2,   0, 2, 3
	};

	for (int i = 0; i < bytesData; i++) {
		mVertexData.push_back(vertexData[i]);
	}
	for (int i = 0; i < 6; i++) {
		mIndexData.push_back(indexData[i]);
	}
	prepareObjGL();
}

void Object3D::prepareObjGL()
{
	// Create VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	{
		//Create VBO
		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		//Create IBO
		glGenBuffers(1, &mEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexData.size() * sizeof(GLuint), mIndexData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // Texture coor
		glEnableVertexAttribArray(2); // Color Vertex
	}
	glBindVertexArray(0);
}

void Object3D::generateTexture()
{
}

void Object3D::update()
{
}

void Object3D::render()
{
	//Sets texture
	if (mTexture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
	}
	//Set VAO
	prepareVAO();
	//Draw VAO
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, mIndexData.size() , GL_UNSIGNED_INT, NULL);
	//Diable VAO
	diableVAO();
	//Unbind program
	//glUseProgram(NULL);
}

void Object3D::prepareVAO()
{
	glm::mat4 transformMatrix;
	//Set VAO
	glBindVertexArray(mVAO);
	//Do Matrix movements
	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, mPosition);
	glUniformMatrix4fv(mUniformModelM, 1, GL_FALSE, glm::value_ptr(transformMatrix));

}

void Object3D::diableVAO()
{
	//Diable VAO
	glBindVertexArray(0);
}

void Object3D::draw()
{
}

void Object3D::clearGPU()
{
	glDeleteVertexArrays(sizeof(mVAO), &mVAO);
	glDeleteBuffers(sizeof(mVBO), &mVBO);
	glDeleteBuffers(sizeof(mEBO), &mEBO);
}

void Object3D::setShader(Shader * p_shader)
{
	mShaderforDraw = p_shader;
	mUniformModelM = glGetUniformLocation(mShaderforDraw->getID(), "uModel");
}


