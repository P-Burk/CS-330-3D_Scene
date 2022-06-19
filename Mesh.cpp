#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Mesh.h"

using namespace std;


Mesh::Mesh() {}

void Mesh::buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector, vector<GLshort>& verticesVector) {

    vertsVector = addNormals(vertsVector);

    this->lightingShader.use();
    this->lightingShader.setInt("material.diffuse", 0);
    this->lightingShader.setInt("material.specular", 1);

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint normalsFloats = 3;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    mesh.nVertices = verticesVector.size() * (vertexFloats + normalsFloats + textureFloats);

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(1, &mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, vertsVector.size() * sizeof(GLfloat), &vertsVector[0], GL_STATIC_DRAW);    // send vertix coordinates to GPU

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesVector.size() * sizeof(GLshort), &verticesVector[0], GL_STATIC_DRAW);*/

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

void Mesh::buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector) {

    vertsVector = addNormals(vertsVector);

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

void Mesh::destoryMesh() {
    glDeleteVertexArrays(1, &this->shapeMesh.vao);
    glDeleteBuffers(1, &this->shapeMesh.vbo);
}

vector<float> Mesh::addNormals(vector<float> inputVec) {
    const float EPSILON = 0.000001f;
    vector<float> outputVec;
    vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    unsigned int totalTriangles = inputVec.size() / 15;
    float nx, ny, nz;

    for (int i = 0; i < totalTriangles; i++) {
        float x1 = inputVec[0];
        float y1 = inputVec[1];
        float z1 = inputVec[2];
        float x2 = inputVec[5];
        float y2 = inputVec[6];
        float z2 = inputVec[7];
        float x3 = inputVec[10];
        float y3 = inputVec[11];
        float z3 = inputVec[12];

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
        inputVec.insert(inputVec.begin() + 13, normal.begin(), normal.end());
        inputVec.insert(inputVec.begin() + 8, normal.begin(), normal.end());
        inputVec.insert(inputVec.begin() + 3, normal.begin(), normal.end());
        outputVec.insert(outputVec.end(), inputVec.begin(), inputVec.begin() + 24);
        inputVec.erase(inputVec.begin(), inputVec.begin() + 24);
    }

    //debuging code
    //std::cout << "Normal: (" << normal[0] << "f, " << normal[1] << "f, " << normal[2] << "f)" << std::endl;
    //std::cout << length << endl;

    return outputVec;
}