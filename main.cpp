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
#include "grid.h"

float arena_size = 513.0f;
float screen_padding = 5.0f;
float hud_height = 15.0f;
float screen_height;
float screen_width;
float h_limit;
float v_limit;
float grid_left;
float grid_right;
float grid_top;
float grid_bot;
unsigned int grid_size = 10;
Grid* grid;

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 'q': exit(0); break;   // Press q to force exit application
    }
    glutPostRedisplay();
}

// void reshape(int w, int h)
// {
// 	glViewport(0, 0, w, h);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
    
//     //gluPerspective(40.0, 1.0f, 1.0, 5.0);

// 	glutPostRedisplay();
// }

void display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1,    // Eye position
              0, 0, 0,      // Ref point
              0, 1, 0       // Up vector
    );

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2f(-h_limit, grid_top + screen_padding);
		glVertex2f(h_limit, grid_top + screen_padding);
	glEnd();
	
    glutSwapBuffers();
}

// void idle() {

// }

void init(int argc, char* argv[])
{	
	// if (argc>3)
	// 	g_program_obj = create_and_compile_shaders(argv[1], argv[2], argv[3]);

    // if (g_program_obj)
    // {
        
    // }
	
    // Set orthographic viewing
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	gluOrtho2D(-h_limit, h_limit, -v_limit, v_limit);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(.0f, 0.2f, .0f, 1.0f);
}

int main(int argc, char* argv[]) {
	screen_height = arena_size + screen_padding + hud_height;
	screen_width = arena_size + screen_padding;
	h_limit = screen_width / 2;
	v_limit = screen_height / 2;
	grid_left = -h_limit + screen_padding;
	grid_right = h_limit - screen_padding;
	grid_top = v_limit - hud_height - screen_padding;
	grid_bot = -v_limit + screen_padding; 
	grid = new Grid(arena_size, grid_size, screen_padding,
					grid_right,
					grid_top);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
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
	// glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	// glutIdleFunc(idle); 

	fprintf(stderr, "Open GL version %s\n", glGetString(GL_VERSION));
	init(argc, argv); 


	glutMainLoop(); 

	return 0; 
}