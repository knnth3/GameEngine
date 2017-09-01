#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <Windows.h>

#define PI 3.14159f

namespace Lime
{

    void load_obj(const char* filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements)
    {
        std::ifstream in(filename, std::ios::in);
        if (!in)
        {
            std::cerr << "Cannot open " << filename << std::endl; exit(1);
        }

        std::string line;
        while (getline(in, line))
        {
            if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
                vertices.push_back(v);
            }
            else if (line.substr(0, 2) == "f ")
            {
                std::istringstream s(line.substr(2));
                GLushort a, b, c;
                s >> a; s >> b; s >> c;
                a--; b--; c--;
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            }
            else if (line[0] == '#')
            {
                /* ignoring this line */
            }
            else
            {
                /* ignoring this line */
            }
        }

        normals.resize(vertices.size() * 100000, glm::vec3(0.0, 0.0, 0.0));
        for (int i = 0; i < elements.size(); i += 3)
        {
            GLushort ia = elements[i];
            GLushort ib = elements[i + 1];
            GLushort ic = elements[i + 2];
            glm::vec3 normal = glm::normalize(glm::cross(
                glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
            normals[ia] = normals[ib] = normals[ic] = normal;
        }
    }

    std::shared_ptr<ModelData> ModelLoader::LoadModel(std::string filename, unsigned int id)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_ObjectID = AssignID();
        bool entered = false;
        std::vector< glm::vec3 > temp_vertices;
        std::vector< glm::vec2 > temp_uvs;
        std::vector< glm::vec3 > temp_normals;
        float rndmR = (rand() % 101) / 100.0f;
        float rndmG = (rand() % 101) / 100.0f;
        float rndmB = (rand() % 101) / 100.0f;
        std::string lineHeader;
        std::ifstream file(filename);
        while (file)
        {
            file >> lineHeader;


            if (strcmp(lineHeader.c_str(), "v") == 0) {
                glm::vec3 vertex;
                file >> vertex.x >> vertex.y >> vertex.z;
                temp_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader.c_str(), "vt") == 0) {
                glm::vec2 uv;
                file >> uv.x >> uv.y;
                temp_uvs.push_back(uv);
            }
            else if (strcmp(lineHeader.c_str(), "vn") == 0) {
                glm::vec3 normal;
                file >> normal.x >> normal.y >> normal.z;
                temp_normals.push_back(normal);
            }
            else if (strcmp(lineHeader.c_str(), "f") == 0) {
                std::vector<std::string> vertex;
                if (!entered)
                {
                    m_Data->m_Verticies.resize(100000);
                    entered = true;
                }
                vertex.resize(3);
                file >> vertex[0] >> vertex[1] >> vertex[2];
                for (size_t x = 0; x < vertex.size(); x++)
                {
                    if (vertex[0] != "")
                    {
                        size_t beginIdx = vertex[x].rfind('/');
                        std::string base = vertex[x].substr(0, beginIdx);
                        std::string value3 = vertex[x].substr(beginIdx + 1);
                        beginIdx = base.rfind('/');
                        std::string value2 = base.substr(beginIdx + 1);
                        std::string value1 = vertex[x].substr(0, beginIdx);
                        unsigned int result1, result2, result3;
                        try
                        {
                            result1 = std::stoi(value1);
                            result2 = std::stoi(value2);
                            result3 = std::stoi(value3);

                            int currentVertexPointer = result1 - 1;
                            m_Data->m_Indicies.push_back(currentVertexPointer);
                            Vertex outputVert;
                            outputVert.position = temp_vertices[currentVertexPointer];
                            outputVert.color = glm::vec3(rndmR, rndmG, rndmB);
                            outputVert.normal = temp_normals[result3 - 1];
                            m_Data->m_Verticies[currentVertexPointer] = outputVert;

                        }
                        catch (const std::exception&)
                        {
                            std::cout << "error" << std::endl;
                        }
                    }
                }
            }
        }
        m_Data->m_Verticies.shrink_to_fit();
        m_Data->m_Indicies.shrink_to_fit();
        file.close();
        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::LoadModel2(std::string filename, unsigned int id)
    {

        std::vector<glm::vec4> suzanne_vertices;
        std::vector<glm::vec3> suzanne_normals;
        std::vector<GLushort> suzanne_elements;

        return std::shared_ptr<ModelData>();
    }

    std::shared_ptr<ModelData> ModelLoader::CreateTriangle(int size)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_Verticies =
        {
            { glm::vec3(-size,0,size),glm::vec3(1,1,1), glm::vec3(0,0,0) },
            { glm::vec3(-size,0,-size),glm::vec3(1,1,1), glm::vec3(0,0,0) },
            { glm::vec3(size,0,-size),glm::vec3(1,1,1), glm::vec3(0,0,0) },
            { glm::vec3(size,0,size),glm::vec3(1,1,1), glm::vec3(0,0,0) }
        };
        m_Data->m_Indicies =
        {
            0,1,2,3,0,2
        };
        m_Data->m_ObjectID = AssignID();
        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::CreateTriangle2()
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_Verticies =
        {
            { glm::vec3(-1,-1,0),glm::vec3(1,0,0), glm::vec3(0,0,0) },
            { glm::vec3(0,-1,0),glm::vec3(0,1,0), glm::vec3(0,0,0) },
            { glm::vec3(-0.5f,0,0),glm::vec3(0,0,1), glm::vec3(0,0,0) }
        };
        m_Data->m_Indicies =
        {
            0,1,2
        };
        m_Data->m_ObjectID = AssignID();
        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::CreateCircle(float radius, float width, unsigned int tesselation, glm::vec3 color)
    {
        //makes the tesselation divisible by 4 & keeps max of 56 total triangles (there is no visual difference afterwards);
        if (tesselation > 56 || tesselation == 0)
            tesselation = 56;
        else if(tesselation < 4)
            tesselation = 4;
        else
            tesselation = 4 * (tesselation / 4);

        float difference = (radius - width);
        if ( difference <= 0.0f)
        {
            difference = 0.0f;
        }
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_ObjectID = AssignID();
        std::vector<Vertex> outsideRing;
        if (difference == 0)
        {
            outsideRing = CreateCircleVertexArray(radius, tesselation, color);
            m_Data->m_Verticies = outsideRing;

            m_Data->m_Verticies.push_back(
            {
                glm::vec3(0.0f,0.0f,0.0f),
                color,
                glm::vec3(0.0f,1.0f,0.0f)
            });

            for (unsigned int it = 0; it < tesselation; it++)
            {
                unsigned int prevVertex = 0;
                if (it == 0)
                {
                    prevVertex = tesselation - 1;
                }
                else
                {
                    prevVertex = it - 1;
                }

                m_Data->m_Indicies.push_back(it);
                m_Data->m_Indicies.push_back(prevVertex);
                m_Data->m_Indicies.push_back(tesselation);
            }
        }
        else
        {
            //vertex creation
            outsideRing = CreateCircleVertexArray(radius, tesselation, color);
            std::vector<Vertex> outsideRingFinal;
            std::vector<Vertex> insideRing;
            outsideRingFinal.reserve(outsideRing.size() * 2);
            for (size_t vert = 0; vert < outsideRing.size(); vert++)
            {
                glm::vec3 prevVec;
                glm::vec3 currentVec;
                glm::vec3 c;
                if (vert == 0)
                {
                    prevVec = outsideRing[outsideRing.size() - 1].position;
                    currentVec = outsideRing[vert].position;
                    c = prevVec - currentVec;

                }
                else
                {
                    prevVec = outsideRing[vert-1].position;
                    currentVec = outsideRing[vert].position;
                    c = prevVec - currentVec;
                }

                //grabbing the middle position of the two vectors
                c = c * 0.5f;
                //creating the middle vector with the middle position
                glm::vec3 middleVec = currentVec + c;
                Vertex middle;
                middle.color = outsideRing[vert].color;
                middle.normal = outsideRing[vert].normal;
                middle.position = middleVec;
                outsideRingFinal.push_back(middle);
                outsideRingFinal.push_back(outsideRing[vert]);
                //outsideRingFinal[vert+1] = outsideRing[vert];
            }
            glm::vec3 col(0.0f, 0.0f, 0.0f);
            insideRing = CreateCircleVertexArray(radius - width, tesselation, col);
            m_Data->m_Verticies.insert(m_Data->m_Verticies.end(), outsideRingFinal.begin(), outsideRingFinal.end());
            m_Data->m_Verticies.insert(m_Data->m_Verticies.end(), insideRing.begin(), insideRing.end());

            //index creation
            GLshort smallIndex = (GLshort) outsideRingFinal.size();
            GLshort bigIndex = 0;
            for (GLshort ind = 0; ind < insideRing.size(); ind++)
            {
                if (ind == 0)
                {
                    m_Data->m_Indicies.push_back(smallIndex);
                    m_Data->m_Indicies.push_back(ind);
                    m_Data->m_Indicies.push_back(smallIndex + ((GLshort) insideRing.size() - 1));

                    m_Data->m_Indicies.push_back(smallIndex);
                    m_Data->m_Indicies.push_back(ind + 1);
                    m_Data->m_Indicies.push_back(ind);

                    m_Data->m_Indicies.push_back(smallIndex);
                    m_Data->m_Indicies.push_back(ind + 2);
                    m_Data->m_Indicies.push_back(ind + 1);
                }
                else if (ind == (insideRing.size() - 1))
                {
                    bigIndex += 2;
                    m_Data->m_Indicies.push_back(smallIndex + ind);
                    m_Data->m_Indicies.push_back(smallIndex - 2);
                    m_Data->m_Indicies.push_back(smallIndex + (ind - 1));

                    m_Data->m_Indicies.push_back(smallIndex + ind);
                    m_Data->m_Indicies.push_back(smallIndex - 1);
                    m_Data->m_Indicies.push_back(smallIndex - 2);

                    m_Data->m_Indicies.push_back(smallIndex + ind);
                    m_Data->m_Indicies.push_back(0);
                    m_Data->m_Indicies.push_back(smallIndex - 1);
                }
                else
                {
                    bigIndex += 2;
                    m_Data->m_Indicies.push_back(smallIndex + ind);
                    m_Data->m_Indicies.push_back(bigIndex);
                    m_Data->m_Indicies.push_back(smallIndex + (ind - 1));
                    for (GLshort it = 0; it < 2; it++)
                    {
                        m_Data->m_Indicies.push_back(smallIndex + ind);
                        m_Data->m_Indicies.push_back(bigIndex + it + 1);
                        m_Data->m_Indicies.push_back(bigIndex + it);
                    }
                }
            }
        }

        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::CreatePlane(unsigned int tesselation, const unsigned int length, const unsigned int width)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        float tessRatio = (float)(tesselation*tesselation) / (length*width);
        if (tessRatio > 16.0f)
        {
            tessRatio = 16.0f;
            tesselation = (unsigned int)(sqrt((length*width) * tessRatio));
        }
        m_Data->m_ObjectID = AssignID();
        m_Data->m_Verticies.resize((tesselation + 1) * (tesselation + 1));
        m_Data->m_Indicies.resize((tesselation) * (tesselation) * 6);
        long double lDivs = (long double)length / tesselation;
        long double wDivs = (long double)width / tesselation;

        long double lHalf = length / 2.0f;
        long double wHalf = width / 2.0f;

        unsigned int vertOffset = 0;
        unsigned int indOffset = 0;
        srand((unsigned int)time(NULL));
        float rndmR = 1;//(rand() % 101) / 100.0f;
        float rndmG = 1;//(rand() % 101) / 100.0f;
        float rndmB = 1;//(rand() % 101) / 100.0f;
        for (long double x = 0; x <= length; x += lDivs)
        {
            unsigned int square = 0;
            for (long double y = 0; y <= width; y += wDivs)
            {
                Vertex& thisVert = m_Data->m_Verticies[vertOffset];
                thisVert.position.x = (float)(x - lHalf);
                thisVert.position.z = (float)(y - wHalf);
                thisVert.position.y = 0;
                thisVert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                if (y < width && x < length)
                {
                    m_Data->m_Indicies[indOffset++] = vertOffset + (tesselation + 1);
                    m_Data->m_Verticies[vertOffset + (tesselation + 1)].color = glm::vec3(rndmR, rndmG, rndmB);
                    m_Data->m_Indicies[indOffset++] = vertOffset + 0;
                    m_Data->m_Verticies[vertOffset + 0].color = glm::vec3(rndmR, rndmG, rndmB);
                    m_Data->m_Indicies[indOffset++] = vertOffset + 1;
                    m_Data->m_Verticies[vertOffset + 1].color = glm::vec3(rndmR, rndmG, rndmB);
                    m_Data->m_Indicies[indOffset++] = vertOffset + (tesselation + 2);
                    m_Data->m_Verticies[vertOffset + (tesselation + 2)].color = glm::vec3(rndmR, rndmG, rndmB);
                    m_Data->m_Indicies[indOffset++] = vertOffset + (tesselation + 1);
                    m_Data->m_Verticies[vertOffset + (tesselation + 1)].color = glm::vec3(rndmR, rndmG, rndmB);
                    m_Data->m_Indicies[indOffset++] = vertOffset + 1;
                    m_Data->m_Verticies[vertOffset + 1].color = glm::vec3(rndmR, rndmG, rndmB);
                    square++;
                }
                vertOffset++;
            }
        }
        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::CreateSimplePlane(const unsigned int width, const unsigned int length, glm::vec3 color)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_ObjectID = AssignID();
        m_Data->m_Verticies = 
        {
            //position->color->normal
            { glm::vec3(-(width / 2.0f),0.0f,(length / 2.0f)),color,glm::vec3(0.0f,1.0f,0.0f) },
            { glm::vec3((width / 2.0f),0.0f,(length / 2.0f)),color,glm::vec3(0.0f,1.0f,0.0f) },
            { glm::vec3((width / 2.0f),0.0f,-(length / 2.0f)),color,glm::vec3(0.0f,1.0f,0.0f) },
            { glm::vec3(-(width / 2.0f),0.0f,-(length / 2.0f)),color,glm::vec3(0.0f,1.0f,0.0f) },
        };
        m_Data->m_Indicies = 
        {
            0,1,2,
            0,2,3
        };
        return m_Data;
    }

    std::shared_ptr<ModelData> ModelLoader::CreateCube(float width, float height, glm::vec3 color)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_ObjectID = AssignID();
        srand((unsigned int)time(NULL));
        float rndmR = (rand() % 101) / 100.0f;
        float rndmG = (rand() % 101) / 100.0f;
        float rndmB = (rand() % 101) / 100.0f;
        color = glm::vec3(rndmR, rndmG, rndmB);

        float halfWidth = width / 2;
        int vertexCount = 0;
        for (size_t side = 0; side < 6; side++)
        {
            std::vector<Vertex> lib;
            switch (side)
            {
            case 0:
                lib = CreateCubeSide(width, height, glm::vec3(1, 0, 0), color);
                break;
            case 1:
                lib = CreateCubeSide(width, height, glm::vec3(-1, 0, 0), color);
                break;
            case 2:
                lib = CreateCubeSide(width, height, glm::vec3(0, 1, 0), color);
                break;
            case 3:
                lib = CreateCubeSide(width, height, glm::vec3(0, -1, 0), color);
                break;
            case 4:
                lib = CreateCubeSide(width, height, glm::vec3(0, 0, 1), color);
                break;
            case 5:
                lib = CreateCubeSide(width, height, glm::vec3(0, 0, -1), color);
                break;
            default:
                break;
            }
            if (lib.size() != 0)
            {
                m_Data->m_Indicies.push_back(vertexCount + 0);
                m_Data->m_Indicies.push_back(vertexCount + 1);
                m_Data->m_Indicies.push_back(vertexCount + 2);
                m_Data->m_Indicies.push_back(vertexCount + 3);
                m_Data->m_Indicies.push_back(vertexCount + 0);
                m_Data->m_Indicies.push_back(vertexCount + 2);
                m_Data->m_Verticies.insert(m_Data->m_Verticies.end(), lib.begin(), lib.end());
                vertexCount += 4;
            }
        }

        return m_Data;
    }


    std::shared_ptr<ModelData> ModelLoader::LoadCubeMapCube(const float SIZE)
    {
        std::shared_ptr<ModelData> m_Data = std::make_shared<ModelData>();
        m_Data->m_ObjectID = AssignID();

        glm::vec3 color(1, 0, 0);
        m_Data->m_Verticies =
        {
            { glm::vec3(-SIZE, +SIZE, +SIZE), // 0
            color, // Color
            glm::vec3(+0.0f, +1.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, +SIZE, +SIZE), // 1
            color, // Color
            glm::vec3(+0.0f, +1.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, +SIZE, -SIZE), // 2
            color, // Color
            glm::vec3(+0.0f, +1.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, +SIZE, -SIZE), // 3
            color, // Color
            glm::vec3(+0.0f, +1.0f, +0.0f) }, // Normal

            { glm::vec3(-SIZE, +SIZE, -SIZE), // 4
            color, // Color
            glm::vec3(+0.0f, +0.0f, -1.0f) }, // Normal
            { glm::vec3(+SIZE, +SIZE, -SIZE), // 5
            color, // Color
            glm::vec3(+0.0f, +0.0f, -1.0f) }, // Normal
            { glm::vec3(+SIZE, -SIZE, -SIZE), // 6
            color, // Color
            glm::vec3(+0.0f, +0.0f, -1.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, -SIZE), // 7
            color, // Color
            glm::vec3(+0.0f, +0.0f, -1.0f) }, // Normal

            { glm::vec3(+SIZE, +SIZE, -SIZE), // 8
            color, // Color
            glm::vec3(+1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, +SIZE, +SIZE), // 9
            color, // Color
            glm::vec3(+1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, -SIZE, +SIZE), // 10
            color, // Color
            glm::vec3(+1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, -SIZE, -SIZE), // 11
            color, // Color
            glm::vec3(+1.0f, +0.0f, +0.0f) }, // Normal

            { glm::vec3(-SIZE, +SIZE, +SIZE), // 12
            color, // Color
            glm::vec3(-1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, +SIZE, -SIZE), // 13
            color, // Color
            glm::vec3(-1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, -SIZE), // 14
            color, // Color
            glm::vec3(-1.0f, +0.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, +SIZE), // 15
            color, // Color
            glm::vec3(-1.0f, +0.0f, +0.0f) }, // Normal

            { glm::vec3(+SIZE, +SIZE, +SIZE), // 16
            color, // Color
            glm::vec3(+0.0f, +0.0f, +1.0f) }, // Normal
            { glm::vec3(-SIZE, +SIZE, +SIZE), // 17
            color, // Color
            glm::vec3(+0.0f, +0.0f, +1.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, +SIZE), // 18
            color, // Color
            glm::vec3(+0.0f, +0.0f, +1.0f) }, // Normal
            { glm::vec3(+SIZE, -SIZE, +SIZE), // 19
            color, // Color
            glm::vec3(+0.0f, +0.0f, +1.0f) }, // Normal

            { glm::vec3(+SIZE, -SIZE, -SIZE), // 20
            color, // Color
            glm::vec3(+0.0f, -1.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, -SIZE), // 21
            color, // Color
            glm::vec3(+0.0f, -1.0f, +0.0f) }, // Normal
            { glm::vec3(-SIZE, -SIZE, +SIZE), // 22
            color, // Color
            glm::vec3(+0.0f, -1.0f, +0.0f) }, // Normal
            { glm::vec3(+SIZE, -SIZE, +SIZE), // 23
            color, // Color
            glm::vec3(+0.0f, -1.0f, +0.0f) } // Normal
        };
        m_Data->m_Indicies =
        {
            2,   1,  0,  3,  2,  0, // Top
            6,   5,  4,  7,  6,  4, // Front
            10,   9, 8,  11, 10, 8, // Right
            14, 13, 12, 15, 14, 12, // Left
            18, 17, 16, 19, 18, 16, // Back
            21, 22, 20, 22, 23, 20 // Bottom
        };

        return m_Data;
    }

    unsigned int ModelLoader::AssignID()
    {
        static unsigned int ObjectID = 0;
        return ObjectID++;
    }

    std::vector<Vertex> ModelLoader::CreateCircleVertexArray(float radius, unsigned int tesselation, glm::vec3 color)
    {
        std::vector<Vertex> verticies;
;        verticies.resize(tesselation);
        for (unsigned int it = 0; it <= (tesselation / 4); it++)
        {
            float thetaRad = (PI * (float)it) / (tesselation / 2);
            float x1, x2, x3, x4;
            float y1, y2, y3, y4;
            if (it == 0)
            {
                x1 = radius;
                x3 = -x1;

                y1 = 0;
                y3 = y1;


                verticies[it] =
                {
                    glm::vec3(x1,0.0f,y1),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
                verticies[(tesselation / 2)] =
                {
                    glm::vec3(x3,0.0f,y3),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };

            }
            else if (it == (tesselation / 4))
            {
                x1 = 0;
                x4 = x1;

                y1 = radius;
                y4 = -y1;



                verticies[it] =
                {
                    glm::vec3(x1,0.0f,y1),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };

                verticies[(tesselation / 2) + it] =
                {
                    glm::vec3(x4,0.0f,y4),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
            }
            else
            {
                x1 = radius * glm::cos(thetaRad);
                x2 = x1;
                x3 = -x1;
                x4 = -x1;

                y1 = radius * glm::sin(thetaRad);
                y2 = -y1;
                y3 = y1;
                y4 = -y1;



                verticies[it] =
                {
                    glm::vec3(x1,0.0f,y1),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
                verticies[(tesselation)-it] =
                {
                    glm::vec3(x2,0.0f,y2),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
                verticies[(tesselation / 2) - it] =
                {
                    glm::vec3(x3,0.0f,y3),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
                verticies[(tesselation / 2) + it] =
                {
                    glm::vec3(x4,0.0f,y4),
                    color,
                    glm::vec3(0.0f,1.0f,0.0f)
                };
            }
        }
        return verticies;
    }

    std::vector<Vertex> ModelLoader::CreateCubeSide(float width, float height, glm::vec3 normal, glm::vec3 color)
    {
        std::vector<Vertex> lib;
        lib.reserve(4);

        Vertex v1;
        Vertex v2;
        Vertex v3;
        Vertex v4;

        float halfWidth = width / 2;
        float halfHeight = height / 2;
        if (abs(normal.x) == 1.0f)
        {
            if (normal.x == 1.0f)
            {
                v1.color = color;
                v1.normal = glm::vec3(1.0f, 0.0f, 0.0f);
                v1.position = glm::vec3(halfWidth, 0, -halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(1.0f, 0.0f, 0.0f);
                v2.position = glm::vec3(halfWidth, height, -halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(1.0f, 0.0f, 0.0f);
                v3.position = glm::vec3(halfWidth, height, halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(1.0f, 0.0f, 0.0f);
                v4.position = glm::vec3(halfWidth, 0, halfHeight);
            }
            else
            {
                v1.color = color;
                v1.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                v1.position = glm::vec3(-halfWidth, 0, halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                v2.position = glm::vec3(-halfWidth, height, halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                v3.position = glm::vec3(-halfWidth, height, -halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                v4.position = glm::vec3(-halfWidth, 0, -halfHeight);
            }
        }
        else if (abs(normal.y) == 1.0f)
        {
            if (normal.y == 1.0f)
            {
                v1.color = color;
                v1.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v1.position = glm::vec3(-halfWidth, height, halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v2.position = glm::vec3(halfWidth, height, halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v3.position = glm::vec3(halfWidth, height, -halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v4.position = glm::vec3(-halfWidth, height, -halfHeight);
            }
            else
            {
                v1.color = color;
                v1.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                v1.position = glm::vec3(-halfWidth, 0, halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                v2.position = glm::vec3(-halfWidth, 0, -halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                v3.position = glm::vec3(halfWidth, 0, -halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(0.0f, -1.0f, 0.0f);
                v4.position = glm::vec3(halfWidth, 0, halfHeight);
            }
        }
        else if (abs(normal.z) == 1.0f)
        {
            if (normal.z == 1.0f)
            {
                v1.color = color;
                v1.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                v1.position = glm::vec3(halfWidth, 0, halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                v2.position = glm::vec3(halfWidth, height, halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                v3.position = glm::vec3(-halfWidth, height, halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                v4.position = glm::vec3(-halfWidth, 0, halfHeight);
            }
            else
            {
                v1.color = color;
                v1.normal = glm::vec3(0.0f, 0.0f, -1.0f);
                v1.position = glm::vec3(-halfWidth, 0, -halfHeight);

                v2.color = color;
                v2.normal = glm::vec3(0.0f, 0.0f, -1.0f);
                v2.position = glm::vec3(-halfWidth, height, -halfHeight);

                v3.color = color;
                v3.normal = glm::vec3(0.0f, 0.0f, -1.0f);
                v3.position = glm::vec3(halfWidth, height, -halfHeight);

                v4.color = color;
                v4.normal = glm::vec3(0.0f, 0.0f, -1.0f);
                v4.position = glm::vec3(halfWidth, 0, -halfHeight);
            }
        }

        lib.push_back(v1);
        lib.push_back(v2);
        lib.push_back(v3);
        lib.push_back(v4);
        return lib;
    }

}