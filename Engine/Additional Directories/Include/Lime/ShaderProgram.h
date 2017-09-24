#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <gl\glew.h>
#include <glm\glm.hpp>
#include "Primitives.h"

namespace Lime
{

    class ShaderProgram
    {
    public:
        DLL_EXPORT void LoadShadersToProgram(const std::string vertexShaderFileName, const std::string fragmentShaderNameFileName);
        DLL_EXPORT void DeleteProgram();
        DLL_EXPORT void Load4vMatrix(const glm::mat4& matrix, const std::string variableName);
        DLL_EXPORT void Load3fv(const glm::vec3& vec, const std::string variableName);
        DLL_EXPORT void Use();
    private:

        std::string LoadShaderCode(std::string fileName);
        bool CheckShaderStatus(GLuint shaderID);
        bool CheckProgramStatus(GLuint shaderID);
        bool CheckStatus(GLuint objectID,
            PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
            PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
            GLenum statusType);



        GLuint programID;
    };


}