//#pragma once
//#include <string>
//#include <fstream>
//#include <iostream>
//#include <gl\glew.h>
//#include <glm\glm.hpp>
//#include "Lime.h"
//
//namespace Lime
//{
//
//    class ShaderProgram
//    {
//    public:
//        AppDLL_API void LoadShadersToProgram(const std::string vertexShaderFileName, const std::string fragmentShaderNameFileName);
//        AppDLL_API void DeleteProgram();
//        AppDLL_API void Load4vMatrix(const glm::mat4& matrix, const std::string variableName);
//        AppDLL_API void Load3fv(const glm::vec3& vec, const std::string variableName);
//        AppDLL_API void Use();
//    private:
//
//        std::string LoadShaderCode(std::string fileName);
//        bool CheckShaderStatus(GLuint shaderID);
//        bool CheckProgramStatus(GLuint shaderID);
//        bool CheckStatus(GLuint objectID,
//            PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
//            PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
//            GLenum statusType);
//
//
//
//        GLuint programID;
//    };
//
//
//}