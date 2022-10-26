#version 460

in vec4 outColor;
in vec2 TexCoord;
layout(location = 0) out vec4 diffuseColor;

uniform sampler2D _MainTex;

void main()
{
	diffuseColor = texture(_MainTex, TexCoord) * vec4(outColor);
}
#pragma once
