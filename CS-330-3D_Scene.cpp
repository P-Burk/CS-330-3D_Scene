/*
 * Class: CS-330-T5621 Computer Graphics and Visualization
 * Instructor: Malcolm Wabara, M.S
 * Assignment: 5-5 Milestone: Texturing Objects in a 3D Scene
 * Student: Preston Burkhardt
 * Date: 05 June 2022
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <vector>

 // GLM imports
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

using namespace std;

// used for storage of the IDs for the VAO and VBO
struct GLMesh {
    GLuint vao;         // Handle for the vertex array object
    GLuint vbo[2];      // Handle for the vertex buffer object
    GLuint nVertices;   // Number of vertices of the mesh
};

// initialize mesh and shader program variables
GLMesh cubeMesh;
GLMesh cylinderMesh;
GLMesh planeMesh;
GLuint gProgramID;
GLuint textureID1;
GLuint textureID2;
GLuint textureID3;

// constants for windown attributes
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char* const WINDOW_TITLE = "5-5 Milestone: Texturing Objects in a 3D Scene";
const char* woodTextureFile = "resources/textures/dark_wood.jpg";
const char* camFrontTextureFile = "resources/textures/Full_camera.png";
const char* camLensTextureFile = "resources/textures/Full_lens.png";
const bool WIREFRAME_MODE = false;
float ROTATE_DEG = 0.0f;
float ROTATE_X = 1.0f;
float ROTATE_Y = 1.0f;
float ROTATE_Z = 1.0f;

// camera and timing
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// perspective switch
bool perspectiveSwitch = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createCubeMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen);
void createCylinderMesh(GLMesh& mesh);
void createPlaneMesh(GLMesh& mesh);
void renderCubeMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID);
void renderCylinderMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID);
void renderPlaneMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID);
void destoryMesh(GLMesh& mesh);
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void destroyShaderProgram(GLuint programID);
void mouseCameraMovement(GLFWwindow* window, double xPos, double yPos);
void scrollCameraMovement(GLFWwindow* window, double xPosOffset, double yPosOffset);
void scrollCameraSpeed(GLFWwindow* window, double xPosOffset, double yPosOffset);
void perspectiveToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
void flipImageVertically(unsigned char* image, int width, int height, int channels);
bool createTexture(const char* filename, GLuint& textureId);
void destroyTexture(GLuint textureId);

void buildMesh(GLMesh& mesh);


// vertex shader source code
const char* vertexShaderSource = "#version 440 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 2) in vec2 textureCoordinate;\n"
"out vec2 vertexTextureCoordinate;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(position, 1.0f);\n"    // transforms vertex data 
"   vertexTextureCoordinate = textureCoordinate;\n"
"}\n\0";

// fragment shader source code
const char* fragmentShaderSource = "#version 440 core\n"
"in vec2 vertexTextureCoordinate;\n"
"out vec4 fragmentColor;\n"
"uniform sampler2D TEXTURE;\n"

"void main()\n"
"{\n"
"   fragmentColor = texture(TEXTURE, vertexTextureCoordinate);\n"
"}\n\0";


int main() {
    cout << WINDOW_TITLE << endl;

    // initialize GLFW and specify versions + profile to be used
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creates window object
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window!!!!" << endl;
        glfwTerminate();
        return -1;
    }

    // sets window object as main context on current thread
    glfwMakeContextCurrent(window);
    // generate window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // camera control calls
    glfwSetCursorPosCallback(window, mouseCameraMovement);
    glfwSetScrollCallback(window, scrollCameraSpeed);


    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }


    // create mesh and shader program
    createCubeMesh(cubeMesh, 0, 0, 0, 1);
    createCylinderMesh(cylinderMesh);
    createPlaneMesh(planeMesh);
    createShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramID);

    // Load texture
    if (!createTexture(woodTextureFile, textureID1)) {
        cout << "Failed to load texture " << woodTextureFile << endl;
        return EXIT_FAILURE;
    }
    // Load texture
    if (!createTexture(camFrontTextureFile, textureID2)) {
        cout << "Failed to load texture " << camFrontTextureFile << endl;
        return EXIT_FAILURE;
    }
    // Load texture
    if (!createTexture(camLensTextureFile, textureID3)) {
        cout << "Failed to load texture " << camLensTextureFile << endl;
        return EXIT_FAILURE;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramID);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramID, "TEXTURE"), 0);

    // while loop to continually render until user closes window
    while (!glfwWindowShouldClose(window)) {

        // enable z-depth buffer
        glEnable(GL_DEPTH_TEST);
        // clear the background frame and z-depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // frame timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);                       // process input

        renderCubeMesh(cubeMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch, textureID2);
        renderCylinderMesh(cylinderMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch, textureID3);
        renderPlaneMesh(planeMesh, gProgramID, window, WIREFRAME_MODE, perspectiveSwitch, textureID1);

        glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
        glfwPollEvents();
    }


    // EXIT CODE /////////////////////////////////////////////
    destroyShaderProgram(gProgramID);
    glfwTerminate();
    return 0;
}

// function used for window resizing
void framebuffer_size_callback(GLFWwindow* window, int passedWidth, int passedHeight) {
    glViewport(0, 0, passedWidth, passedHeight);

}

// processes user input
// closes the OpenGL window when ESC is pressed
// use WASD for camera movement, Q E for up and down
void processInput(GLFWwindow* window) {

    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    
    // perspective switch
    glfwSetKeyCallback(window, perspectiveToggle);
}

void createCubeMesh(GLMesh& mesh, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfixed edgeLen) {
    // cube ///////////////////////////////////////////////////////////////////////
    //    v4----- v5
    //   /|      /|
    //  v0------v1|
    //  | |     | |
    //  | |v7---|-|v6
    //  |/      |/
    //  v3------v2

    // Position and Color data
    GLfloat verts[] = {
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

    // Creates a buffer object for the indices
    GLshort vertices[] = {
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

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

// creates cylinder
void createCylinderMesh(GLMesh& mesh) {

    // Position and Color data
    GLfloat verts[] = {
         // front
         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //0
         0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.0f,    // V1  //1
         0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.033f,  // V2  //2
         
         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //3
         0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.033f,  // V2  //4
         0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.066f,  // V3  //5

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //6
         0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.066f,  // V3  //7
         0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,    0.666f, 0.133f,  // V4  //8

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //9
         0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,    0.666f, 0.133f,  // V4  //10
         0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,    0.666f, 0.2f,    // V5  //11

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //12
         0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,    0.666f, 0.2f,    // V5  //13
         0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.266f,  // V6  //14

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //15
         0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.266f,  // V6  //16
         0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.3f,    // V7  //17

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //18
         0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.3f,    // V7  //19
         0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.333f,  // V8  //20

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //21
         0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.333f,  // V8  //22
        -0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.333f,  // V9  //23

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //24
        -0.1f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.333f,  // V9  //25
        -0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.3f,    // V10 //26

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //27
        -0.3f, -0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.3f,    // V10 //28
        -0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.266f,  // V11 //29

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //30
        -0.4f, -0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.266f,  // V11 //31
        -0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,    0.333f, 0.2f,    // V12 //32

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //33
        -0.5f, -0.1f, 0.5f,     0.0f, 1.0f, 1.0f, 1.0f,    0.333f, 0.2f,    // V12 //34
        -0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,    0.333f, 0.133f,  // V13 //35

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //36
        -0.5f,  0.1f, 0.5f,     1.0f, 1.0f, 0.0f, 1.0f,    0.333f, 0.133f,  // V13 //37
        -0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.066f,  // V14 //38

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //39
        -0.4f,  0.3f, 0.5f,     1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.066f,  // V14 //40
        -0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.033f,  // V15 //41

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //42
        -0.3f,  0.4f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.033f,  // V15 //43
        -0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.0f,    // V16 //44

         0.0f,  0.0f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.166f,  // V0  //45
        -0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.0f,    // V16 //46
         0.1f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.0f,    // V1  //47
         
         //back
         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //48
         0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 1.0f,    // V18 //49
         0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.966f,  // V19 //50

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //51
         0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.966f,  // V19 //52
         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.933f,  // V20 //53

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //54
         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.933f,  // V20 //55
         0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.666f, 0.866f,  // V21 //56

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //57
         0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.666f, 0.866f,  // V21 //58
         0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.666f, 0.8f,    // V22 //59

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //60
         0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.666f, 0.8f,    // V22 //61
         0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.733f,  // V23 //62

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //63
         0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.733f,  // V23 //64
         0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.7f,    // V24 //65

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //66
         0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.7f,    // V24 //67
         0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.666f,  // V25 //68

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //69
         0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.666f,  // V25 //70
        -0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.666f,  // V26 //71

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //72
        -0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.666f,  // V26 //73
        -0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.7f,    // V27 //74

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //75
        -0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.7f,    // V27 //76
        -0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.733f,  // V28 //77

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //78
        -0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.733f,  // V28 //79
        -0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.333f, 0.8f,    // V29 //80

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //81
        -0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.333f, 0.8f,    // V29 //82
        -0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.333f, 0.866f,  // V30 //83

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //84
        -0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.333f, 0.866f,  // V30 //85
        -0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.933f,  // V31 //86

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //87
        -0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.933f,  // V31 //88
        -0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.966f,  // V32 //89

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //90
        -0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.966f,  // V32 //91
        -0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 1.0f,    // V33 //92

         0.0f,  0.0f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.5f,   0.833f,  // V17 //93
        -0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 1.0f,    // V33 //94
         0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 1.0f,    // V18 //95

         // body
         0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.0f,   0.666f,  // V18 //96
        -0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.666f,  // V33 //97
         0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.0f,   0.333f,  // V1  //98
        -0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.333f,  // V16 //99
        -0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.666f,  // V33 //100
         0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.0f,   0.333f,  // V1  //101

        -0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.666f,  // V33 //102
        -0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.666f,  // V32 //103
        -0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.333f,  // V16 //104
        -0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.333f,  // V15 //105
        -0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.666f,  // V32 //106
        -0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.066f, 0.333f,  // V16 //107

        -0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.666f,  // V32 //108
        -0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.666f,  // V31 //109
        -0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.333f,  // V15 //110
        -0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.333f,  // V14 //111
        -0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.666f,  // V31 //112
        -0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.133f, 0.333f,  // V15 //113

        -0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.666f,  // V31 //114
        -0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.233f, 0.666f,  // V30 //115
        -0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.333f,  // V14 //116
        -0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.233f, 0.333f,  // V13 //117
        -0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.233f, 0.666f,  // V30 //118
        -0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.166f, 0.333f,  // V14 //119

        -0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.233f, 0.666f,  // V30 //120
        -0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.3f,   0.666f,  // V29 //121
        -0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.233f, 0.333f,  // V13 //122
        -0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.3f,   0.333f,  // V12 //123
        -0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.3f,   0.666f,  // V29 //124
        -0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.233f, 0.333f,  // V13 //125

        -0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.3f,   0.666f,  // V29 //126
        -0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.666f,  // V28 //127
        -0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.3f,   0.333f,  // V12 //128
        -0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.333f,  // V11 //129
        -0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.666f,  // V28 //130
        -0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.3f,   0.333f,  // V12 //131

        -0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.666f,  // V28 //132
        -0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.666f,  // V27 //133
        -0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.333f,  // V11 //134
        -0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.333f,  // V10 //135
        -0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.666f,  // V27 //136
        -0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.366f, 0.333f,  // V11 //137

        -0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.666f,  // V27 //138
        -0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.666f,  // V26 //139
        -0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.333f,  // V10 //140
        -0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.333f,  // V9  //141
        -0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.666f,  // V26 //142
        -0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.4f,   0.333f,  // V10 //143

        -0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.666f,  // V26 //144
         0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.666f,  // V25 //145
        -0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.333f,  // V9  //146
         0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.333f,  // V8  //147
         0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.666f,  // V25 //148
        -0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.466f, 0.333f,  // V9  //149

         0.1f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.666f,  // V25 //150
         0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.666f,  // V24 //151
         0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.333f,  // V8  //152
         0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.333f,  // V7  //153
         0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.666f,  // V24 //154
         0.1f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.533f, 0.333f,  // V8  //155

         0.3f, -0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.666f,  // V24 //156
         0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.666f,  // V23 //157
         0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.333f,  // V7  //158
         0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.333f,  // V6  //159
         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.666f,  // V23 //160
         0.3f, -0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.6f,   0.333f,  // V7  //161

         0.4f, -0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.666f,  // V23 //162
         0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.7f,   0.666f,  // V22 //163
         0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.333f,  // V6  //164
         0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.7f,   0.333f,  // V5  //165
         0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.7f,   0.666f,  // V22 //166
         0.4f, -0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.633f, 0.333f,  // V6  //167

         0.5f, -0.1f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.7f,   0.666f,  // V22 //168
         0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.766f, 0.666f,  // V21 //169
         0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.7f,   0.333f,  // V5  //170
         0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.766f, 0.333f,  // V4  //171
         0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.766f, 0.666f,  // V21 //172
         0.5f, -0.1f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.7f,   0.333f,  // V5  //173

         0.5f,  0.1f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,    0.766f, 0.666f,  // V21 //174
         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.666f,  // V20 //175
         0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.766f, 0.333f,  // V4  //176
         0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.333f,  // V3  //177
         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.666f,  // V20 //178
         0.5f,  0.1f,  0.5f,    0.0f, 1.0f, 1.0f, 1.0f,    0.766f, 0.333f,  // V4  //179

         0.4f,  0.3f, -0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.666f,  // V20 //180
         0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.666f,  // V19 //181
         0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.333f,  // V3  //182
         0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.333f,  // V2  //183
         0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.666f,  // V19 //184
         0.4f,  0.3f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,    0.833f, 0.333f,  // V3  //185

         0.3f,  0.4f, -0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.666f,  // V19 //186
         0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.933f, 0.666f,  // V18 //187
         0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.333f,  // V2  //188
         0.1f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.933f, 0.333f,  // V1  //189
         0.1f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,    0.933f, 0.666f,  // V18 //190
         0.3f,  0.4f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,    0.866f, 0.333f,  // V2  //191  
    };

    // Creates a buffer object for the indices
    GLshort vertices[] = { 
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
        40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 
        50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 
        70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
        90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 
        100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 
        110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
        120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 
        130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 
        140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 
        150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 
        160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 
        170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 
        180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 
        190, 191
        
    };

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint strideLen = sizeof(float) * (vertexFloats + colorFloats + textureFloats);

    // vertex attribute pointer for position
    glVertexAttribPointer(0, vertexFloats, GL_FLOAT, GL_FALSE, strideLen, 0);
    glEnableVertexAttribArray(0);

    // vertex attribute pointer for color
    glVertexAttribPointer(1, colorFloats, GL_FLOAT, GL_FALSE, strideLen, (char*)(sizeof(float)* vertexFloats));
    glEnableVertexAttribArray(1);

    // vertex attibute pointer for texture
    glVertexAttribPointer(2, textureFloats, GL_FLOAT, GL_FALSE, strideLen, (void*)(sizeof(float)* (vertexFloats + colorFloats)));
    glEnableVertexAttribArray(2);
}

// render the cube
void renderCubeMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID) {
    /*
    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // clear the background frame and z-depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(3.3f, 2.5f, 1.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));
    //rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = rotation * scale * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

// render the cylinder
void renderCylinderMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID) {
    /*
    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // enable z-depth buffer
    glEnable(GL_DEPTH_TEST);

    // clear the background frame and z-depth buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    */
    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.8f, 2.0f, 2.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.23f, -0.1f, 0.5f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);

    //NOTE: put the glClear() and glfwSwapBuffers() function in the main() AROUND the multiple renders() to prevent flashing
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, and so on).
    //glfwSwapBuffers(window);    // Flips the the back buffer with the front buffer every frame
}

// used for clearing the mesh
void destoryMesh(GLMesh& mesh) {
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(2, mesh.vbo);
}

// creates the shader program
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programID) {

    // compilation and linkage error reporting
    int successNum = 0;
    char infoLog[512];

    // create shader program object
    programID = glCreateProgram();


    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);       // create vertex shader object
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);   // create fragment shader object

    glShaderSource(vertexShaderID, 1, &vtxShaderSource, NULL);      // get the shader source for vertex fragment
    glShaderSource(fragmentShaderID, 1, &fragShaderSource, NULL);   // get the shader source for fragment shader

    glCompileShader(vertexShaderID);                                // compile vertex shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &successNum);  // check for and print errors
    if (!successNum) {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    glCompileShader(fragmentShaderID);                              // compile fragment shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &successNum);// check for and print errors
    if (!successNum) {
        glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return false;
    }

    // attach compiled shaders to their respective shader programs
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);


    glLinkProgram(programID);                                       // links the shader program
    glGetProgramiv(programID, GL_LINK_STATUS, &successNum);         // check for and print errors
    if (!successNum) {
        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
        return false;
    }

    glUseProgram(programID);

    return true;

}

// clears the shader program
void destroyShaderProgram(GLuint programID) {
    glDeleteProgram(programID);
}

void mouseCameraMovement(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse == true) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xPosOffest = xPos - lastX;
    float yPosOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xPosOffest, yPosOffset);
}

// Use the scroll wheel for camera ZOOM //////////////////////////////////////////
void scrollCameraMovement(GLFWwindow* window, double xPosOffset, double yPosOffset) {
    camera.ProcessMouseScroll(yPosOffset);
}

// Use the scroll wheel for adjusting camera SPEED //////////////////////////////
void scrollCameraSpeed(GLFWwindow* window, double xPosOffset, double yPosOffset) {
    camera.ProcessMouseScroll_Speed(yPosOffset);
}

void perspectiveToggle(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        perspectiveSwitch = !perspectiveSwitch;
}

void createPlaneMesh(GLMesh& mesh) {
    // PLANE ///////////////////////////////////////////////////////////////////////
    //  v0-----v1
    //  |       | 
    //  |       |
    //  v3-----v2

    // Position and Color data
    GLfloat verts[] = {
        -5.0f, -1.3f,  5.0f,     1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 0
         5.0f, -1.3f,  5.0f,     1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 1
         5.0f, -1.3f, -5.0f,     1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 2
        -5.0f, -1.3f, -5.0f,     1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f // 3
    };

    // Creates a buffer object for the indices
    GLshort vertices[] = {
        0, 1, 3,  // T1
        2, 1, 3   // T2
    };

    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

void renderPlaneMesh(const GLMesh& mesh, GLuint programID, GLFWwindow* window, const bool WIREFRAME_MODE, bool perspective, GLuint textureID) {

    // 1. scales object
    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

    // 2. rotates object 
    glm::mat4 rotation = glm::mat4(1.0f);

    // for no rotation, set radians to 0 and X, Y, and Z values to 1
    rotation = glm::rotate(rotation, glm::radians(ROTATE_DEG), glm::vec3(ROTATE_X, ROTATE_Y, ROTATE_Z));

    // 3. places object at origin
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.5f));

    // Transformations are applied in right-to-left order.
    glm::mat4 model = scale * rotation * translation;

    // Transforms the camera: move the camera back (Z axis)
    //glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = camera.GetViewMatrix();

    // Projection MAtrix
    glm::mat4 projection;
    if (perspective) {
        projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    }
    else {
        projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    }

    // retrieves and passes transformation matrices to shader program
    GLint modelLoc = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint viewLoc = glGetUniformLocation(programID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(programID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used.
    glUseProgram(programID);

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(programID, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VAO.
    glBindVertexArray(mesh.vao);

    // wireframe mode
    if (WIREFRAME_MODE == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID1);

    // Draw the triangle.
    glDrawElements(GL_TRIANGLES, mesh.nVertices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object.
    glBindVertexArray(0);
}


// flips image vertically because images are loaded opposite of how OpenGL interprets Y-axis
void flipImageVertically(unsigned char* image, int width, int height, int channels) {
    for (int j = 0; j < height / 2; ++j) {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i) {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

// creates the texture
bool createTexture(const char* filename, GLuint& textureId) {

    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image) {

        // flip the image on y-axis
        flipImageVertically(image, width, height, channels);

        // generate and bind texture
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // logic for handling 3 or 4 channel images
        if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        } else if (channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        } else {
            cout << "No support for images with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    return false;
}

// destroys texture
void destroyTexture(GLuint textureId) {
    glGenTextures(1, &textureId);
}

void buildMesh(GLMesh& mesh) {
    // creates vertex attribute pointer
    const GLuint vertexFloats = 3;      // number of coordinates per vertex
    const GLuint colorFloats = 4;       // floats that represent color (r, g, b, a)
    const GLuint textureFloats = 2;     // floats for texture mapping

    glGenVertexArrays(1, &mesh.vao);            // generate VAO
    glBindVertexArray(mesh.vao);                // binds VAO

    glGenBuffers(2, mesh.vbo);                  // generates two buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]); // binds VBOs
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);    // send vertix coordinates to GPU

    mesh.nVertices = sizeof(vertices) / sizeof(vertices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
