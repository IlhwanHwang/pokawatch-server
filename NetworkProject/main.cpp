//
// CNH 2016.04.30
// CSED353 Project
//

#include <GL/glew.h>
#include <GL/freeglut.h>

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);

	glutCreateWindow("CNH - Network Project");

	glewInit();

	glutMainLoop();
}