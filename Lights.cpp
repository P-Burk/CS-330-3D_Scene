#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Lights.h"

void Lights::buildLights(GLMesh& mesh, vector<float>& vertices, Shader& lightingShader) {
    this->vertices = addNormals(vertices);

    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(mesh.vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &this->lightCubeVAO);
    glBindVertexArray(this->lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
}

void Lights::renderLights(vector<glm::vec3>& passedpointLightPositions, glm::mat4& passedProjection, glm::mat4& passedView, glm::mat4& passedModel, bool perspectiveSwitch) {

    // be sure to activate shader when setting uniforms/drawing objects
    this->lightingShader.use();
    this->lightingShader.setVec3("viewPos", this->camera.Position);
    this->lightingShader.setFloat("material.shininess", 32.0f);

    /////////////////////////// LIGHT INFORMATION ///////////////////////////
    // directional light
    this->lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    this->lightingShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    this->lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    this->lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    this->lightingShader.setVec3("pointLights[0].position", passedpointLightPositions[0]);
    this->lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    this->lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    this->lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    this->lightingShader.setFloat("pointLights[0].constant", 1.0f);
    this->lightingShader.setFloat("pointLights[0].linear", 0.09f);
    this->lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    this->lightingShader.setVec3("pointLights[1].position", passedpointLightPositions[1]);
    this->lightingShader.setVec3("pointLights[1].ambient", 1.0f, 1.0f, 1.0f);
    this->lightingShader.setVec3("pointLights[1].diffuse", 0.65f, 0.25f, 0.25f);
    this->lightingShader.setVec3("pointLights[1].specular", 0.4f, 0.05f, 0.05f);
    this->lightingShader.setFloat("pointLights[1].constant", 1.0f);
    this->lightingShader.setFloat("pointLights[1].linear", 0.09f);
    this->lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
    //// point light 3
    //this->lightingShader.setVec3("pointLights[2].position", passedpointLightPositions[2]);
    //this->lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    //this->lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    //this->lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    //this->lightingShader.setFloat("pointLights[2].constant", 1.0f);
    //this->lightingShader.setFloat("pointLights[2].linear", 0.09f);
    //this->lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
    //// point light 4
    //this->lightingShader.setVec3("pointLights[3].position", passedpointLightPositions[3]);
    //this->lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    //this->lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    //this->lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    //this->lightingShader.setFloat("pointLights[3].constant", 1.0f);
    //this->lightingShader.setFloat("pointLights[3].linear", 0.09f);
    //this->lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    //this->lightingShader.setVec3("spotLight.position", this->camera.Position);
    //this->lightingShader.setVec3("spotLight.direction", this->camera.Front);
    //this->lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    //this->lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    //this->lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    //this->lightingShader.setFloat("spotLight.constant", 1.0f);
    //this->lightingShader.setFloat("spotLight.linear", 0.09f);
    //this->lightingShader.setFloat("spotLight.quadratic", 0.032f);
    //this->lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(0.0f)));
    //this->lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(0.0f)));


    //view and projection transformations
    this->lightingShader.setMat4("projection", passedProjection);
    this->lightingShader.setMat4("view", passedView);

    // world transformation
    this->lightingShader.setMat4("model", passedModel);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->specularMap);
 
    // Projection MAtrix
    if (perspectiveSwitch) {
        passedProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    } else {
        passedProjection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // draw the lamps
    this->lightCubeShader.use();
    this->lightCubeShader.setMat4("projection", passedProjection);
    this->lightCubeShader.setMat4("view", passedView);

    //draw light "bulbs" for point lights
    glBindVertexArray(this->lightCubeVAO);
    for (unsigned int i = 0; i < 2; i++) {
        passedModel = glm::mat4(1.0f);
        passedModel = glm::translate(passedModel, passedpointLightPositions[i]);
        passedModel = glm::scale(passedModel, glm::vec3(0.2f));     //sets size of bulbs
        this->lightCubeShader.setMat4("model", passedModel);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}

vector<float> Lights::addNormals(vector<float> inputVec) {
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
