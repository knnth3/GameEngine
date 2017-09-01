#include "ShaderProgram.h"

namespace Lime
{

    std::string ShaderProgram::LoadShaderCode(std::string fileName)
    {
        std::ifstream input(fileName);
        if (!input.good())
        {
            std::cout << "File failed to load..." << fileName << std::endl;
            exit(1);
        }
        return std::string(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>());
    }

    void ShaderProgram::LoadShadersToProgram(const std::string vertexShaderFileName, const std::string fragmentShaderNameFileName)
    {
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        const GLchar* adapter[1];
        std::string temp = LoadShaderCode(vertexShaderFileName);
        adapter[0] = temp.c_str();
        glShaderSource(vertexShaderID, 1, adapter, 0);
        temp = LoadShaderCode(fragmentShaderNameFileName);
        adapter[0] = temp.c_str();
        glShaderSource(fragmentShaderID, 1, adapter, 0);

        glCompileShader(vertexShaderID);
        glCompileShader(fragmentShaderID);

        if (!CheckShaderStatus(vertexShaderID) || !CheckShaderStatus(fragmentShaderID))
            return;

        programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);

        glLinkProgram(programID);

        if (!CheckProgramStatus(programID))
            return;

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        glUseProgram(programID);
    }

    bool ShaderProgram::CheckShaderStatus(GLuint shaderID)
    {
        return CheckStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
    }

    bool ShaderProgram::CheckProgramStatus(GLuint programID)
    {
        return CheckStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
    }

    bool ShaderProgram::CheckStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
    {
        GLint status;
        objectPropertyGetterFunc(objectID, statusType, &status);
        if (status != GL_TRUE)
        {
            GLint infoLogLength;
            objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* buffer = new GLchar[infoLogLength];

            GLsizei bufferSize;
            getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
            std::cout << buffer << std::endl;
            delete[] buffer;
            return false;
        }
        return true;
    }

    void ShaderProgram::DeleteProgram()
    {
        glUseProgram(0);
        glDeleteProgram(programID);
    }

    void ShaderProgram::Load4vMatrix(const glm::mat4 & matrix, const std::string variableName)
    {
        GLuint loc = glGetUniformLocation(programID, variableName.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
    }

    void ShaderProgram::Load3fv(const glm::vec3 & vec, const std::string variableName)
    {
        GLuint loc = glGetUniformLocation(programID, variableName.c_str());
        glUniform3fv(loc, 1, &vec[0]);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(programID);
    }

}