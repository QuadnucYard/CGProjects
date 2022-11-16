#version 460

in VS_OUT {
	vec4 Color;
	vec2 TexCoord;
} v2f;

out vec4 fragColor;

uniform sampler2D _MainTex;

void main()
{
	fragColor = texture(_MainTex, v2f.TexCoord) * vec4(v2f.Color);
}
