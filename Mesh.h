#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;

// used for storage of the IDs for the VAO and VBO
struct GLMesh {
    GLuint vao;         // Handle for the vertex array object
    GLuint vbo;      // Handle for the vertex buffer object
    GLuint nVertices;   // Number of vertices of the mesh
};

class Mesh {
public:
    Mesh();
    GLMesh getShapeMesh() { return this->shapeMesh; }
    GLuint getMeshVAO() { return this->shapeMesh.vao; }
    GLuint getMeshVBO(int VBOindex) { return this->shapeMesh.vbo; }
    GLuint getMeshNVertices() { return this->shapeMesh.nVertices; }
    void destoryMesh();


protected:
    GLMesh shapeMesh;
    vector<GLshort> vertsVector;
    vector<GLfloat> verticesVector;
    void buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector, vector<GLshort>& verticesVector);
    void buildMesh(GLMesh& mesh, vector<GLfloat>& vertsVector);
    vector<float> addNormals(vector<float> inputVec);

private:

};
#endif