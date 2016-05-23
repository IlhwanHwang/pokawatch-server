#version 330

in vec2 fTex;

layout (location = 0) out vec4 pCol;

uniform sampler2D tex;
uniform vec4 blend;

void main()
{
	pCol = blend * texture2D(tex, fTex);
}
