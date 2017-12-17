#ifdef __APPLE__
#include <GLUT/glut.h> 
#else

#include <GL/glew.h>
#include <GL/glut.h> 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include "arena.h"

unsigned int arena_size = 512;
unsigned int grid_size = 10;
unsigned int screen_padding = 5;
unsigned int hud_height = 15;
unsigned float cell_size;

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 'q': exit(0); break;   // Press q to force exit application
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
    //gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, -10,    // Eye position
              0, 0, 0,      // Ref point
              0, 1, 0       // Up vector
    );

    glutSwapBuffers();
}

void idle() {

}

void init(int argc, char* argv[])
{	
	cell_size = arena_size / grid_size;
	// if (argc>3)
	// 	g_program_obj = create_and_compile_shaders(argv[1], argv[2], argv[3]);

    // if (g_program_obj)
    // {
        
    // }
    // Set orthographic viewing
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	glOrtho(-2.0, 2.0, -2.0, 2.0, -4.0, 4.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.1f, 0.44f, 1.0f);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(arena_size + screen_padding + hud_height, arena_size + screen_padding);
	glutInitWindowPosition(50, 50);
    glutCreateWindow("Snake");

#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK!=err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutIdleFunc(idle); 

	fprintf(stderr, "Open GL version %s\n", glGetString(GL_VERSION));
	init(argc, argv); 


	glutMainLoop(); 

	return 0; 
}