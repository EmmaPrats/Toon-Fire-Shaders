#pragma once

#include "glm/glm.hpp"

class Camera3D
{
public:
	Camera3D();
	~Camera3D();

	void init();
	void update();

	// forward-backward-left-right-up-down
	void update(bool *moveBoolArray);

	//Setters
	void setCameraPos(float X, float Y, float Z);
	void setCameraFront(float X, float Y, float Z);
	void setCameraUp(float X, float Y, float Z);
	void setUniformProjectionMatrix(int screenSizeW, int screenSizeH, int uniformID);
	void setUniformViewMatrix(int uniformID);
	void setHeadTurn(int xOffset, int yOffset);
	void setAddZoom(int value);
	void setCameraFly(bool value) { mCameraFly = value; };
	void setGround(float value) { mGround = value; };

	// Getters
	glm::vec3 getCameraPos();
	void invertPitch();


private:
	// Matrix
	glm::mat4	mViewMatrix;
	glm::mat4	mProjectionMatrix;

	//Camera vectors and varbs
	glm::vec3	mCameraPos;
	glm::vec3	mCameraFront;
	glm::vec3	mCameraUp;
	float		mCameraSpeed;
	float		mYaw;
	float		mPitch;
	float		mFov;
	bool        mCameraFly;
	float		mGround;

	//Head
	float		mSensitivityTurn;
};

