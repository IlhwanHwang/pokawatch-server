#version 330

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vTex;
out vec2 fTex;

uniform mat4 modelview;
uniform mat4 projection;

void main()
{
    gl_Position = Projection * ModelView * vPos / vPos.w;
	fTex = vTex;
}
