#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Mesh.h"

using namespace std;


Mesh::Mesh() {
    vector<GLfloat> vertsVector = {
        // CUBE ////////////////////////////////////////////////////////////////////////////////////
         // front 1
         -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,     0.2f, 0.5f,  // V0  // 0
          0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 1.0f,     0.8f, 0.5f,  // V1  // 1
         -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,     0.2f, 0.2f,  // V3  // 2

         // front 2
          0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.2f,  // V2  // 3
          0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 1.0f,     0.8f, 0.5f,  // V1  // 4
         -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,     0.2f, 0.2f,  // V3  // 5

         // back 1
         0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f, 1.0f,     0.8f, 1.0f,  // V6   // 6
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,     0.8f, 0.7f,  // V5   // 7
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.2f, 1.0f,  // V7   // 8

         // back 2
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,     0.2f, 0.7f,  // V4   // 9
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,     0.8f, 0.7f,  // V5   // 10
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.2f, 1.0f,  // V7   // 11

         // left 1
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,     0.0f, 0.5f,  // V4   // 12
        -0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,     0.2f, 0.5f,  // V0   // 13
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.0f, 0.2f,  // V7   // 14

         // left 2
        -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f, 1.0f,     0.2f, 0.2f,  // V3   // 15
        -0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,     0.2f, 0.5f,  // V0   // 16
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.0f, 0.2f,  // V7   // 17

         // right 1
         0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f, 1.0f,     0.8f, 0.5f,  // V1   // 18
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.5f,  // V5   // 19
         0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.2f,  // V2   // 20

         // right 2
         0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f, 1.0f,     1.0f, 0.2f,  // V6   // 21
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,     1.0f, 0.5f,  // V5   // 22
         0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.2f,  // V2   // 23

         // top 1
        -0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,     0.2f, 0.5f,  // V0   // 24
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,     0.2f, 0.7f,  // V4   // 25
         0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f, 1.0f,     0.8f, 0.5f,  // V1   // 26

         // top 2
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f, 1.0f,     0.8f, 0.7f,  // V5   // 27
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f, 1.0f,     0.2f, 0.7f,  // V4   // 28
         0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f, 1.0f,     0.8f, 0.5f,  // V1   // 29

         // bottom 1
        -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f, 1.0f,     0.2f, 0.2f,  // V3   // 30
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.2f, 0.0f,  // V7   // 31
         0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.2f,  // V2   // 32

         // bottom 2
         0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.0f,  // V6   // 33
        -0.5f, -0.5f, -0.5f,     0.5f, 1.0f, 1.0f, 1.0f,     0.2f, 0.0f,  // V7   // 34
         0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f, 1.0f,     0.8f, 0.2f,  // V2   // 35
    };

    vector<GLshort> verticesVector = {
        // CUBE //////////////////////////////
        0, 1, 2,       // FT1
        3, 4, 5,       // FT2
        6, 7, 8,       // BaT1
        9, 10, 11,     // BaT2
        12, 13, 14,    // LT1
        15, 16, 17,    // LT2
        18, 19, 20,    // RT1
        21, 22, 23,    // RT2
        24, 25, 26,    // TT1
        27, 28, 29,    // TT2
        30, 31, 32,    // BoT1
        33, 34, 35     // BoT2
    };

    buildMesh(shapeMesh, vertsVector, verticesVector);
}

void Mesh::buildMesh(GLMesh &mesh, vector<GLfloat> &vertsVector, vector<GLshort>& verticesVector) {
    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, vertsVector.size() * sizeof(GLfloat), &vertsVector[0], GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = verticesVector.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesVector.size() * sizeof(GLshort), &verticesVector[0], GL_STATIC_DRAW);

    GLint strideLen = sizeof(float) * (vertexFloats + colorFloats + textureFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, colorFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float) * vertexFloats));
    glEnableVertexAttribArray(1);

    // vertex attibute pointer for texture
    glVertexAttribPointer(2, textureFloats, GL_FLOAT, GL_FALSE, strideLen, (void*)(sizeof(float) * (vertexFloats + colorFloats)));
    glEnableVertexAttribArray(2);
}
