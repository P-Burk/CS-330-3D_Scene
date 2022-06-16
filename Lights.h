/****** CODE CITATION **************************************************************
* Title: Learn OpenGL: multiple_lights_exercise1.cpp
* Author: Joey de Vries
* Date Accessed: 14 June 2022
* Code Version: N/A
* Availability: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
************************************************************************************/

#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

//Global variables
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

class Lights {
public:
    //CONSTRUCTOR
    Lights(Shader& passedLightingShader, Shader& passedLightCubeShader, Camera& passedCamera, unsigned int& DM1,
        unsigned int& SM1, unsigned int& DM2, unsigned int& SM2, unsigned int& DM3, unsigned int& SM3, vector<glm::vec3>& pointLightPositions) 
    {

        //set member variables
        this->lightingShader = passedLightingShader;
        this->lightCubeShader = passedLightCubeShader;
        this->camera = passedCamera;
        this->diffuseMap1 = DM1;
        this->specularMap1 = SM1;
        this->diffuseMap2 = DM2;
        this->specularMap2 = SM2;        
        this->diffuseMap3 = DM3;
        this->specularMap3 = SM3;
        this->pointLightPositions = pointLightPositions;

        //build light meshes
        buildLights(this->shapeMesh, this->vertices, this->lightingShader);
    }

    //FUNCTIONS
    void renderLights(vector<glm::vec3>& passedpointLightPositions, glm::mat4& passedProjection, glm::mat4& passedView, glm::mat4& passedModel);
    unsigned int getLightCubeVAO() { return this->lightCubeVAO; }


protected:
    GLMesh shapeMesh;


private:
    Shader lightingShader;
    Shader lightCubeShader;
    Camera camera;
    vector<glm::vec3> pointLightPositions;
    unsigned int diffuseMap1 = NULL;
    unsigned int specularMap1 = NULL;
    unsigned int diffuseMap2 = NULL;
    unsigned int specularMap2 = NULL;
    unsigned int diffuseMap3 = NULL;
    unsigned int specularMap3 = NULL;
    unsigned int lightCubeVAO;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    void buildLights(GLMesh& mesh, vector<float>& vertices, Shader& lightingShader);
    vector<float> addNormals(vector<float> inputVec);

    vector<float> vertices = {
        // front 
         0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
        -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
        -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
        -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
         0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
         0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 

         // back 
        -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 
         0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 
         0.5f, -0.5f, -0.5f,      0.8f, 1.0f,  // V6 
         0.5f, -0.5f, -0.5f,      0.8f, 1.0f,  // V6 
        -0.5f, -0.5f, -0.5f,      0.2f, 1.0f,  // V7 
        -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 

         // left 
        -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
        -0.5f,  0.5f, -0.5f,      0.0f, 0.5f,  // V4 
        -0.5f, -0.5f, -0.5f,      0.0f, 0.2f,  // V7 
        -0.5f, -0.5f, -0.5f,      0.0f, 0.2f,  // V7 
        -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
        -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 

         // right 
         0.5f,  0.5f, -0.5f,      1.0f, 0.5f,  // V5 
         0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
         0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
         0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
         0.5f, -0.5f, -0.5f,      1.0f, 0.2f,  // V6 
         0.5f,  0.5f, -0.5f,      1.0f, 0.5f,  // V5 

         // top 
         0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 
        -0.5f,  0.5f, -0.5f,      0.2f, 0.7f,  // V4 
        -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
        -0.5f,  0.5f,  0.5f,      0.2f, 0.5f,  // V0 
         0.5f,  0.5f,  0.5f,      0.8f, 0.5f,  // V1 
         0.5f,  0.5f, -0.5f,      0.8f, 0.7f,  // V5 

         // bottom 
         0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
        -0.5f, -0.5f,  0.5f,      0.2f, 0.2f,  // V3 
        -0.5f, -0.5f, -0.5f,      0.2f, 0.0f,  // V7 
        -0.5f, -0.5f, -0.5f,      0.2f, 0.0f,  // V7 
         0.5f, -0.5f, -0.5f,      0.8f, 0.0f,  // V6 
         0.5f, -0.5f,  0.5f,      0.8f, 0.2f,  // V2 
    };

    //moved to main.cpp file for easier modification
    //vector<glm::vec3> pointLightPositions = {
    //    glm::vec3(-1.0f, -1.5f,  -0.1f),
    //    glm::vec3(0.0f,  1.8f,  2.0f),
    //    glm::vec3(-1.0f,  0.8f,  -3.0f),
    //    glm::vec3(2.0f,  0.5f,  0.0f)
    //};

};

#endif