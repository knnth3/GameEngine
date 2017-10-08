//#include "SkyBox.h"
//
//namespace Lime
//{
//
//    int SkyBox::CompileVertexData(std::shared_ptr<Camera>& camera)
//    {
//        m_Camera = camera;
//        CreateVertexArray();
//        m_ShaderProgram.LoadShadersToProgram("shaders/skyboxVertexShader.glsl", "shaders/skyBoxfragmentShader.glsl");
//        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//
//        m_Texture = SOIL_load_OGL_cubemap
//        (
//            "textures/envmap_miramar/posx.tga",
//            "textures/envmap_miramar/negx.tga",
//            "textures/envmap_miramar/posy.tga",
//            "textures/envmap_miramar/negy.tga",
//            "textures/envmap_miramar/posz.tga",
//            "textures/envmap_miramar/negz.tga",
//            SOIL_LOAD_RGB,
//            SOIL_CREATE_NEW_ID,
//            SOIL_FLAG_MIPMAPS
//        );
//
//        //unsigned char* img = SOIL_load_image(fileName.c_str(), 0, 0, 0, SOIL_LOAD_RGB);
//		//Might need far plane for this
//		//Using tempVar until implemented
//        m_SkyBox.m_Data = ModelLoader::LoadModel("Cubemap", 0);
//        MoveDataToBuffer();
//        return 0;
//    }
//
//    void SkyBox::Render()
//    {
//        m_ShaderProgram.Use();
//		//Might need to separate rotation matrix from worldMatrix
//        glm::mat4 modelMatrix = m_SkyBox.GetLocalToWorld();
//        m_ShaderProgram.Load4vMatrix(m_Camera->GetViewMatrix(), "view");
//        m_ShaderProgram.Load4vMatrix(m_Camera->Get3DProjectionMatrix(), "projection");
//        m_ShaderProgram.Load4vMatrix(modelMatrix, "model");
//
//        glBindVertexArray(m_VaoID);
//        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IabID);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
//
//        glDrawElements(GL_TRIANGLES, (GLsizei)(m_SkyBox.m_Data->m_Indicies.size()), GL_UNSIGNED_SHORT, 0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    void SkyBox::ClearVertexBuffers()
//    {
//        m_ShaderProgram.DeleteProgram();
//        glDeleteTextures(1, &m_Texture);
//    }
//
//    void SkyBox::CreateVertexArray()
//    {
//        //Creates the Vertex BMBuffer Object ID and the Vertex Array Object ID
//        if (m_VaoID == 0)
//            glGenVertexArrays(1, &m_VaoID);
//        if (m_VboID == 0)
//            glGenBuffers(1, &m_VboID);
//        if (m_IabID == 0)
//            glGenBuffers(1, &m_IabID);
//
//
//        glBindVertexArray(m_VaoID);
//        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
//        glEnableVertexAttribArray(0);
//        glEnableVertexAttribArray(1);
//        glEnableVertexAttribArray(2);
//
//        //Adds attrib for Positions
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::m_position));
//
//        //Adds attrib for Colors
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::m_uv));
//
//        //Adds attrib for Normal Vectors
//        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::m_normal));
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    void SkyBox::MoveDataToBuffer()
//    {
//        glBindVertexArray(m_VaoID);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IabID);
//        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
//        glBufferData(GL_ARRAY_BUFFER, m_SkyBox.m_Data->VertexBufferSize(), m_SkyBox.m_Data->m_Verticies.data(), GL_STATIC_DRAW);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_SkyBox.m_Data->m_Indicies.size() * sizeof(GLushort), m_SkyBox.m_Data->m_Indicies.data(), GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindVertexArray(0);
//    }
//
//    GLuint SkyBox::LoadCubeMap(std::vector<const GLchar*> faces)
//    {
//        GLuint textureID;
//        glGenTextures(1, &textureID);
//        glActiveTexture(GL_TEXTURE0);
//
//        int width, height;
//        unsigned char* image;
//
//        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//        for (GLuint i = 0; i < faces.size(); i++)
//        {
//            image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
//            glTexImage2D(
//                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
//                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
//            );
//        }
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//
//        return textureID;
//    }
//
//}