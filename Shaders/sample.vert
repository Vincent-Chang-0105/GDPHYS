#version 330 core

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//Create a transfrom variable
uniform mat4 transform;

//Projection Matrix
uniform mat4 projection;

uniform mat4 view;

layout(location = 2) in vec2 aTex;

out vec2 texCoord;

void main()
{
	//gl_Position is predefined
	//this denotes the final position of the vertex / point

	//Multiply the transformation matrix to the vec4  to the vec4 version of aPos
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	texCoord = aTex;
}