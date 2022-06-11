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
    GLuint vbo[2];      // Handle for the vertex buffer object
    GLuint nVertices;   // Number of vertices of the mesh
};

class Mesh {
    public:
        Mesh();
        GLMesh getShapeMesh() { return this->shapeMesh; }
        GLuint getMeshVAO() { return this->shapeMesh.vao; }
        GLuint getMeshVBO(int VBOindex) { return this->shapeMesh.vbo[VBOindex]; }
        GLuint getMeshNVertices() { return this->shapeMesh.nVertices; }

    protected:


    private:
        GLMesh shapeMesh;
        vector<GLshort> vertsVector;
        vector<GLfloat> verticesVector;
        void buildMesh();

};


#endif
