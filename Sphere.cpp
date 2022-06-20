/****** CODE CITATION **************************************************************
* Title: OpenGL Sphere: Sphere.cpp
* Author: Song Ho Ahn
* Date Accessed: 18 June 2022
* Code Version: N/A
* Availability: https://songho.ca/opengl/gl_sphere.html
************************************************************************************/

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
#include "Sphere.h"

//Constants for sectors and stack count minimums
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

//Constructor
Sphere::Sphere(Shader& litShdr, Shader& ltCUBEshdr, unsigned int difMap, unsigned int specMap, float radius, int sectorCount, int stackCount) {
    set(radius, sectorCount, stackCount);
    this->lightingShader = litShdr;
    this->lightCUBEshader = ltCUBEshdr;
    this->diffuseMap = difMap;
    this->specularMap = specMap;

    buildMesh(this->shapeMesh, this->interleavedVertices);
}

//Setter function
void Sphere::set(float radius, int sectors, int stacks) {
    this->radius = radius;
    this->sectorCount = sectors;
    if (sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if (sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    buildVerticesFlat();
}


//Clears all the vectors used for building
void Sphere::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
}


//Builds the sphere vertices information vector
void Sphere::buildVerticesFlat() {
    const float PI = acos(-1);

    // tmp vertex definition (x,y,z,s,t)
    struct Vertex {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;            // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);        // r * cos(u)
        float z = radius * sinf(stackAngle);         // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;              // starting from 0 to 2pi

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
            vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
            vertex.z = z;                              // z = r * sin(u)
            vertex.s = (float)j / sectorCount;         // s
            vertex.t = (float)i / stackCount;          // t
            tmpVertices.push_back(vertex);
        }
    }

    // clear memory of prev arrays
    clearArrays();

    Vertex v1, v2, v3, v4;                              // 4 vertex positions and tex coords
    std::vector<float> n;                               // 1 face normal

    int i, j, k, vi1, vi2;
    int index = 0;                                
    for (i = 0; i < stackCount; ++i) {
        vi1 = i * (sectorCount + 1);              
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2) {
            // get 4 vertices per sector
            //  v1--v3
            //  |    |
            //  v2--v4
            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];

            // if 1st stack and last stack, store only 1 triangle per sector
            // otherwise, store 2 triangles (quad) per sector
            if (i == 0) // a triangle for first stack ==========================
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                index += 3;     // for next
            } else if (i == (stackCount - 1)) // a triangle for last stack =========
            {
                // put a triangle
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);

                // put tex coords of triangle
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                index += 3;     // for next
            } else // 2 triangles for others ====================================
            {
                // order based on CCW winding
                // put quad vertices: v1 -> v2 -> v4
                //                    v4 -> v3 -> v1
                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);
                addVertex(v4.x, v4.y, v4.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v1.x, v1.y, v1.z);

                // put tex coords of quad
                // follows same order as addition of vertices (1 2 4 4 3 1)
                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);
                addTexCoord(v4.s, v4.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v1.s, v1.t);

                // put normal
                n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                for (k = 0; k < 6; ++k)  // same normals for 4 vertices
                {
                    addNormal(n[0], n[1], n[2]);
                }

                index += 6;     // for next
            }
        }
    }

    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

//builds the final vector of vertex, normals, and texture coordinates
void Sphere::buildInterleavedVertices() {
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2) {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}

// adds vertex to vertices vector
void Sphere::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

// adds normals to normals vector
void Sphere::addNormal(float nx, float ny, float nz) {
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

// adds texture coordinates to texCoords vector
void Sphere::addTexCoord(float s, float t) {
    texCoords.push_back(s);
    texCoords.push_back(t);
}

// calculates normals for a triangel
// if a triangle has no surface (normal length = 0), then return a zero vector
std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,  // v1
    float x2, float y2, float z2,  // v2
    float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON) {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}

// build function to prepare the shape for rendering
void Sphere::buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector) {
    this->lightingShader.use();
    this->lightingShader.setInt("material.diffuse", 0);
    this->lightingShader.setInt("material.specular", 1);

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint normalsFloats = 3;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    mesh.nVertices = vertsVector.size() / (vertexFloats + normalsFloats + textureFloats);

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(1, &mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, vertsVector.size() * sizeof(GLfloat), &vertsVector[0], GL_STATIC_DRAW);    // send vertix coordinates to GPU

    GLint strideLen = sizeof(float) * (vertexFloats + normalsFloats + textureFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, normalsFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float) * vertexFloats));
    glEnableVertexAttribArray(1);

    // vertex attibute pointer for texture
    glVertexAttribPointer(2, textureFloats, GL_FLOAT, GL_FALSE, strideLen, (void*)(sizeof(float) * (vertexFloats + normalsFloats)));
    glEnableVertexAttribArray(2);
}