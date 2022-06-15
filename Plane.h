#ifndef PLANE_H
#define PLANE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

using namespace std;

class Plane : public Mesh {
	public:
		Plane() { buildMesh(this->shapeMesh, this->vertsVector); };

	protected:


	private:
        // PLANE ///////////////////////////////////////////////////////////////////////
        //  v0-----v1
        //  |       | 
        //  |       |
        //  v3-----v2

        // Position and Color data
        vector<GLfloat> vertsVector = {
            -5.0f, -1.3f, -5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // V0
             5.0f, -1.3f, -5.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // V1
            -5.0f, -1.3f,  5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // V3

             5.0f, -1.3f,  5.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // V2
             5.0f, -1.3f, -5.0f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // V1
            -5.0f, -1.3f,  5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f  // V3
        };

        // Creates a buffer object for the indices
        vector<GLshort> verticesVector = {
            0, 1, 3,  // T1
            2, 1, 3   // T2
        };
};

#endif