#include "SpriteBatch.h"
#include <iostream>
#include <glm\gtx\transform.hpp>
#include <iostream>


namespace Lime
{

    void SpriteBatch::Initialize(std::shared_ptr<Camera> camera, std::shared_ptr<ShaderProgram> shaders)
    {
        m_Camera = camera;
        m_ShaderProgram = shaders;
        m_ShaderProgram->LoadShadersToProgram("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
        CreateVertexArray();
    }

    void SpriteBatch::CreateVertexArray()
    {
        //Creates the Vertex Buffer Object ID and the Vertex Array Object ID
        if (m_VaoID == 0)
            glGenVertexArrays(1, &m_VaoID);
        if (m_VboID == 0)
            glGenBuffers(1, &m_VboID);
        if (m_IabID == 0)
            glGenBuffers(1, &m_IabID);


        glBindVertexArray(m_VaoID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        //Adds attrib for Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        //Adds attrib for Colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        //Adds attrib for Normal Vectors
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void SpriteBatch::LoadModelUniforms(const unsigned int nOfObject)
    {
        glm::mat4 modelMatrix = m_MatrixBank[nOfObject];
        m_ShaderProgram->Load4vMatrix(modelMatrix, "modelMatrix");

        glm::mat4 fullTransformMatrix = m_Camera->GetProjectiomMatrix() * m_Camera->GetViewMatrix() * modelMatrix;
        m_ShaderProgram->Load4vMatrix(fullTransformMatrix, "fullTransformMatrix");

        m_ShaderProgram->Load3fv(m_Camera->GetPosition(), "cameraPos");

        glm::vec3 lightPosition1(0, 5, 0);
        glm::vec3 lightPosition2(1.f, 5.f, 1.f);
        glm::vec3 lightPosition3(-1.f, 5.f, 1.f);
        glm::vec3 lightPosition4(-1.f, 5.f, -1.f);
        glm::vec3 lightPosition5(1.f, 5.f, -1.f);
        m_ShaderProgram->Load3fv(lightPosition1, "lightPosition[0]");
        m_ShaderProgram->Load3fv(lightPosition2, "lightPosition[1]");
        m_ShaderProgram->Load3fv(lightPosition3, "lightPosition[2]");
        m_ShaderProgram->Load3fv(lightPosition4, "lightPosition[3]");
        //m_ShaderProgram->Load3fv(lightPosition5, "lightPosition[4]");


        float atten1 = 1.0f;
        float atten2 = 0.01f;
        float atten3 = 0.002f;
        glm::vec3 lightAttenuation(atten1, atten2, atten3);
        m_ShaderProgram->Load3fv(lightAttenuation, "lightAttenuation[0]");
        m_ShaderProgram->Load3fv(lightAttenuation, "lightAttenuation[1]");
        m_ShaderProgram->Load3fv(lightAttenuation, "lightAttenuation[2]");
        m_ShaderProgram->Load3fv(lightAttenuation, "lightAttenuation[3]");
        //m_ShaderProgram->Load3fv(lightAttenuation, "lightAttenuation[4]");
    }

    void SpriteBatch::RecordOffsetof(unsigned int keyval, const GLsizeiptr offset)
    {
        if (m_OffsetofBank.count(keyval) == 0)
        {
            m_OffsetofBank.insert(std::make_pair(keyval, offset));
        }
    }

    void SpriteBatch::Begin()
    {
        //Resets all memory banks to 0 for next render;
        m_ModelDataBank.clear();
        m_MatrixBank.clear();
        m_OffsetofBank.clear();
        m_ObjectsToPrint.clear();
        m_VertexBatchSize = 0;
        m_IndexBatchSize = 0;
        m_VertexSize = 0;
    }

    void SpriteBatch::Draw(Model3D& model)
    {
        //Checks to see if the model pointers arent set to 0
        //If not, pointer adress is copied to memory bank;
        if (model.m_Data != nullptr)
        {
            m_MatrixBank.push_back(model.GetLocalToWorld());
            m_ObjectsToPrint.push_back(model.m_Data->m_ObjectID);
            m_VertexBatchSize += model.m_Data->VertexBufferSize();
            m_VertexSize += model.m_Data->m_Verticies.size();
            m_IndexBatchSize += model.m_Data->IndexBufferSize();
            m_ModelDataBank.insert(std::make_pair(model.m_Data->m_ObjectID, model.m_Data));
        }
    }

    void SpriteBatch::End()
    {
        //Begins render process
        m_ShaderProgram->Use();
        CreateRenderBatch();
        RenderBatch();
    }

    void SpriteBatch::Close()
    {
        m_ModelDataBank.clear();
        m_MatrixBank.clear();

        m_OffsetofBank.clear();
        m_ObjectsToPrint.clear();
        m_VertexBatchSize = 0;
        m_IndexBatchSize = 0;
        glDeleteBuffers(1, &m_VboID);
        glDeleteVertexArrays(1, &m_VaoID);
    }

    void SpriteBatch::CreateRenderBatch()
    {
        glBindVertexArray(m_VaoID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IabID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glBufferData(GL_ARRAY_BUFFER, m_VertexBatchSize, 0, GL_DYNAMIC_DRAW);
        std::vector<GLushort> indices;
        indices.reserve(m_IndexBatchSize);
        GLsizeiptr IndexNumOffset = 0;
        GLsizeiptr VertNumOffset = 0;
        GLushort offset = 0;
        std::vector<Vertex> verts;
        verts.reserve(m_VertexSize);
        for (auto currentModel : m_ModelDataBank)
        {

            unsigned int indiceSize = (unsigned int)currentModel.second->m_Indicies.size();
            for (size_t it = 0; it < indiceSize; it++)
            {
                RecordOffsetof(currentModel.second->m_ObjectID, IndexNumOffset);
                unsigned int indici = (unsigned int)currentModel.second->m_Indicies.at(it);
                indices.push_back((unsigned int)VertNumOffset + indici);
            }
            VertNumOffset += currentModel.second->m_Verticies.size();
            IndexNumOffset += currentModel.second->m_Indicies.size();

            int sizeOfData = (int)currentModel.second->VertexBufferSize();
            verts.insert(verts.end(), currentModel.second->m_Verticies.begin(), currentModel.second->m_Verticies.end());
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexBatchSize, verts.data());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SpriteBatch::RenderBatch()
    {
        //Tells GFX to render data;
        glBindVertexArray(m_VaoID);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IabID);

        for (size_t object = 0; object < m_ObjectsToPrint.size(); object++)
        {
            unsigned int id = m_ObjectsToPrint[object];
            unsigned int nOfindices = (unsigned int)m_ModelDataBank[id]->m_Indicies.size();
            GLsizeiptr currentElementOffset = m_OffsetofBank[id];
            LoadModelUniforms((const unsigned int)object);
            glDrawElements(GL_TRIANGLES, nOfindices, GL_UNSIGNED_SHORT, (void *)(sizeof(GLushort) * currentElementOffset));
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}