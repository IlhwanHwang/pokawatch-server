//
// CNH 2016.04.30
// shader management
//

#include <GL/glew.h>

#include "InitShader.h"
#include "shader.h"

void Shader::init() {
	shdSprite = glCreateProgram();
	loadShader(shdSprite, GL_VERTEX_SHADER, "sprite_v.glsl")

}