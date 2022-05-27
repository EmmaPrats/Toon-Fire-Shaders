#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
public:
	Shader();
	
	void init(const char* vertexPath, const char* fragmentPath);

	GLuint getID() { return programId; }

	bool wasInitializationSuccessful() { return mAllOK; }

	void use()
	{
		if (mAllOK)
			glUseProgram(programId);
	}

	void deleteProgram()
	{
		if (mAllOK)
			glDeleteProgram(programId);
	}

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }

	void setVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
	}

	void setVec4(unsigned int uniformLocation, float x, float y, float z, float w)
	{
		glUniform4f(uniformLocation, x, y, z, w);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
	}

	void setMat4(unsigned int transformLoc, const GLfloat* value)
	{
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value);
	}

	void setMat4(const std::string& name, const GLfloat* value)
	{
		unsigned int transformLoc = glGetUniformLocation(programId, name.c_str());
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value);
	}

	void setMat4(const std::string& name, glm::mat4 value)
	{
		unsigned int transformLoc = glGetUniformLocation(programId, name.c_str());
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	GLuint programId;
	bool mAllOK;
};

#endif //SHADER_H
