#version 330 core

out vec4 FragColor; // Returns a color

uniform sampler2D tex0;

in vec2 texCoord;

uniform vec4 color;
//Simple shader that colors the model 
void main()
{
	//				  R   G   B  a  Ranges from 0->1
	FragColor = color; //Sets the color of the fragment

	//FragColor = texture(tex0, texCoord);
}

