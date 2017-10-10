//#pragma once
//#include "ShaderProgram.h"
//#include "Shared.h"
//#include "ModelLoader.h"
//#include "Camera.h"
//#include <SOIL\SOIL.h>
//
//namespace Lime
//{
//
//    class SkyBox
//    {
//    public:
//        AppDLL_API int CompileVertexData(std::shared_ptr<Camera>& m_camera);
//        AppDLL_API void Render();
//        AppDLL_API void ClearVertexBuffers();
//
//    private:
//        void CreateVertexArray();
//        void MoveDataToBuffer();
//        GLuint LoadCubeMap(std::vector<const GLchar*> faces);
//
//        GLuint m_VaoID;
//        GLuint m_VboID;
//        GLuint m_IabID;
//        GLuint m_Texture;
//        Model3D m_SkyBox;
//        std::shared_ptr<Camera> m_Camera;
//        ShaderProgram m_ShaderProgram;
//    };
//
//
//}