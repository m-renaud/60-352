#include <GL/glut.h>

using GLUTdisplayCB = void (*)();

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Initialize GLUT.
void myGlutInit(
	int* argc, char** argv,
	GLUTdisplayCB displayFunc,
	int width, int height
)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 150);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(displayFunc);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1440.0, 0.0, 900.0);
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Plot a 2D point.
void plot_point(float x, float y)
{
	glVertex2i(
		static_cast<int>(x + 0.5),
		static_cast<int>(y + 0.5)
	);
}
