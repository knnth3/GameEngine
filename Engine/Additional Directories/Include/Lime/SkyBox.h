//#pragma once
//#include "ShaderProgram.h"
//#include "Primitives.h"
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
//        DLL_EXPORT int Initialize(std::shared_ptr<Camera>& m_camera);
//        DLL_EXPORT void Draw();
//        DLL_EXPORT void Close();
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