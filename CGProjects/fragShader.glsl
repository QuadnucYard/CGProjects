#version 430

in vec4 outColor;
layout(location = 0) out vec4 diffuseColor;

void main()
{
	diffuseColor = vec4(outColor);
//	gl_FragColor = vec4(outColor, 1.0);
}

