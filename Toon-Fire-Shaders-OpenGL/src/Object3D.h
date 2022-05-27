#pragma once

#include <vector>

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

#include <string>

class Object3D
{
public:
	Object3D();
	~Object3D();

	virtual void init();
	virtual void loadObjFromDisk(std::string file);
	virtual void loadTextureFromDisk(std::string file);
	virtual void generateObj();
	virtual void generateTexture();
	virtual void update();
	virtual void render();
	virtual void prepareVAO();
	virtual void diableVAO();
	virtual void draw();
	virtual void clearGPU();

	virtual void setShader(Shader* p_shader);

	void setPosition(glm::vec3 point) { mPosition = point; };
	glm::vec3 getPosition() { return mPosition; };

	void setRotationAxis(glm::vec3 point) { mRotationAxis = point; };
	glm::vec3 getRotationAxis() { return mRotationAxis; };

	void setRotationDegrees(glm::vec3 point) { mRotationDegrees = point; };
	glm::vec3 getRotationDegrees() { return mRotationDegrees; };

	void setScale(glm::vec3 point) { mScale = point; };
	glm::vec3 getScale() { return mScale; };

	void setTexture(GLuint id) { mTexture = id; };
	GLuint getTexture() { return mTexture; };

protected:
	GLuint mVAO;
	GLuint mVBO;
	GLuint mEBO;
	GLuint mTexture;

	std::vector<GLfloat> mVertexData;
	std::vector<GLuint> mIndexData;

	Shader*		mShaderforDraw;
	int			mUniformModelM;

	glm::vec3	mPosition;
	glm::vec3	mRotationAxis;
	glm::vec3	mRotationDegrees;
	glm::vec3	mScale;

	void prepareObjGL();
};
