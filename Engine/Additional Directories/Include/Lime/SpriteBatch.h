//#pragma once
//#include <gl\glew.h>
//#include <memory>
//#include <map>
//#include "Primitives.h"
//#include "ShaderProgram.h"
//#include "Camera.h"
//
//namespace Lime
//{
//
//    class SpriteBatch
//    {
//    public:
//        DLL_EXPORT void Initialize(std::shared_ptr<Camera> camera, std::shared_ptr<ShaderProgram> shaders);
//        DLL_EXPORT void Begin();
//        DLL_EXPORT void Draw(Model3D& model);
//        DLL_EXPORT void End();
//        DLL_EXPORT void Close();
//    private:
//
//        void CreateRenderBatch();
//        void RenderBatch();
//        void CreateVertexArray();
//        void LoadModelUniforms(const unsigned int nOfObject);
//        void RecordOffsetof(unsigned int keyval, const GLsizeiptr offset);
//
//
//        int m_HasNewInfo = 0;
//        GLuint m_VboID = 0;
//        GLuint m_VaoID = 0;
//        GLuint m_IabID = 0;
//        GLsizeiptr m_VertexBatchSize = 0;
//        GLsizeiptr m_VertexSize = 0;
//        GLsizeiptr m_IndexBatchSize = 0;
//        std::shared_ptr<ShaderProgram> m_ShaderProgram = nullptr;
//        std::map<unsigned int, std::shared_ptr<ModelData>> m_ModelDataBank;
//        std::vector<glm::mat4> m_MatrixBank;
//        std::map<unsigned int, GLsizeiptr> m_OffsetofBank;
//        std::vector<unsigned int> m_ObjectsToPrint;
//        std::shared_ptr<Camera> m_Camera;
//
//    };
//
//
//}