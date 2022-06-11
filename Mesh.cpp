#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Mesh.h"

using namespace std;


Mesh::Mesh() {}

void Mesh::buildMesh(GLMesh &mesh, vector<GLfloat>& vertsVector, vector<GLshort>& verticesVector) {
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
