#version 330 core

out vec4 FragColor; // Returns a color

//Simple shader that colors the model 
void main()
{
	//				  R   G   B  a  Ranges from 0->1
	FragColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); //Sets the color of the fragment
}

