/****** CODE CITATION **************************************************************
* Title: Learn OpenGL: 6.multiple_lights.vs
* Author: Joey de Vries
* Date Accessed: 11 June 2022
* Code Version: N/A
* Availability: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.vs
************************************************************************************/

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
