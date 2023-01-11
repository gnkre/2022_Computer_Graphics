#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 crntPos;
out vec3 Normal;

uniform mat4 camera_matrix;

uniform mat4 pre_compute_matrix;


void main()
{
	crntPos = vec3(pre_compute_matrix * vec4(aPos, 1.0f));
	Normal = aNormal;
	gl_Position = camera_matrix * vec4(crntPos, 1.0);
}