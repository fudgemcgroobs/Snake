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
#include <time.h>
#include <string>
#include "grid.h"
#include "snake.h"
#include "pallet.h"

float arena_size = 1000.0f;		// The size, in world units, of the play area
float screen_padding = 5.0f;	// In world coordinates/sizes
float hud_height = 50.0f;		// The height of the HUD in the world
float screen_height;	// The total height of the viewport and screen
float screen_width;		// The total width of the viewport and screen
float h_limit;			// The horizontal limit at which can draw
float v_limit;			// The vertical limit at which can draw
float grid_left;		// The left edge of the grid (x-coordinate)
float grid_right;		// The right edge of the grid (x-coordinate)
float grid_top;			// The top edge of the grid (y-coordinate)
float grid_bot;			// The bottom edge of the grid (y-coordinate)
float text_size = .2f;
bool menu = true;
bool running = false;
bool game_over = false;
bool moved = false;
bool loop = true;
int ticks;
int menu_screen;
unsigned int grid_size = 30;	// The order of the grid/matrix. Must be >5
unsigned int difficulty = 0;		// The current difficulty level
unsigned int difficulty_step = 2;	// The required score change for difficulty increase
unsigned int max_difficulty = 9;
unsigned int delay_step = 10;
unsigned int max_delay = 140;
unsigned int g_bitmap_text_handle = 0;
Grid* grid;
Snake* snake;
Pallet* pallet;

unsigned int make_bitmap_text() {
	unsigned int handle_base = glGenLists(256); 
	for (int i = 0; i < 256; i++) {
		// a new list for each character
		glNewList(handle_base+i, GL_COMPILE);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, i);
		glEndList();
	}
	return handle_base;
}

void draw_text(const char* s) {
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
	}
}
void draw_text(std::string s) {
	int len = s.length();
	for (int i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
	}
}

float str_width(const char* s) {
	int len = strlen(s);
	int total_width = 0;
	for(int i = 0; i < len; i++) {	
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, s[i]) ;
	}
	return total_width;
}
float str_width(std::string s) {
	int len = s.length();
	int total_width = 0;
	for(int i = 0; i < len; i++) {	
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, s[i]) ;
	}
	return total_width;
}

/*
* Method that draws a square of side size 1.0
* scaling on x or y by a number N will result in a respective side of size N
*/
void draw_square() {
	static float vertex[4][2] = {
		{.0f, .0f},
		{1.0f, .0f},
		{1.0f, -1.0f},
		{.0f, -1.0f}
	};

	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 4; i++) {
			glVertex2fv(vertex[i]);
		}
	glEnd();
}

/*
* Method that draws a grid using the coordinates stored in the Grid structure
*/
void draw_grid() {
	for(int i = 0; i < grid_size; i++) {
			for(int j = 0; j < grid_size; j++) {
				glPushMatrix();
				Cell* new_cell = grid->GetCellAt(i, j);
				glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
				glScalef(grid->GetCellSize(), grid->GetCellSize(), 1.0f);
				// glRotatei();
				draw_square();
				glPopMatrix();
			}
		}
}

void draw_segment() {
	glRectf(.0f, .0f, 1.0f, -1.0f);
}

void draw_snake() {
	unsigned int** positions = snake->GetSnakePosition();
	for(int i = 0; i < snake->GetLength(); i++) {
		glPushMatrix();
			Cell* new_cell = grid->GetCellAt(positions[i][0], positions[i][1]);
			glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
			glScalef(grid->GetCellSize(), grid->GetCellSize(), 1.0f);
			draw_segment();
		glPopMatrix();
	}

	for ( int i = 0; i < snake->GetLength(); i++) {
		delete [] positions[i];
	}
	delete [] positions;
}

void draw_pallet() {
	Cell* new_cell = grid->GetCellAt(pallet->GetY(), pallet->GetX());
	glPushMatrix();
		glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
		glScalef(grid->GetCellSize() - 0.5f, grid->GetCellSize() - 0.5f, 1.0f);
		glRectf(.0f, .0f, 1.0f, -1.0f);
	glPopMatrix();
}

void draw_header(const char* text) {
	glPushMatrix();
		float h_center_offset = -str_width(text)/(2/text_size);
		float v_center_offset = v_limit - screen_padding - (hud_height/2);
		glTranslatef(.0f + h_center_offset, .0f + v_center_offset, .0f);
		glScalef(text_size, text_size, 1.0f);
		draw_text(text);
	glPopMatrix();
}

void draw_state() {
	const char* text;
	if(game_over) {
		text = "Game Over!";
	} else {
		text = "Eat the Pallets!";
	}
	draw_header(text);
}

void draw_score() {
	std::string text("Score: ");
	text.append(std::to_string(snake->GetScore()));

	glPushMatrix();
		float h_center_offset = -h_limit + screen_padding;
		float v_center_offset = v_limit - screen_padding - (hud_height/2);
		glTranslatef(.0f + h_center_offset, .0f + v_center_offset, .0f);
		glScalef(text_size, text_size, 1.0f);
		draw_text(text);
	glPopMatrix();
}

void check_head_collisions() {
	// If head collide with body
	if(snake->Bite() && running) {
		running = false;
		game_over = true;
		glutPostRedisplay();
	}
	// If head collide w pallet
	unsigned int** positions = snake->GetSnakePosition();
	if(positions[0][0] == pallet->GetY() &&
			positions[0][1] == pallet->GetX()) {
		int palletX;
		int palletY;
		do{
			palletX = (int) ( rand() % ( grid_size - 1 ));
			palletY = (int) ( rand() % ( grid_size - 1 ));
		} while(!pallet->Reposition(palletX, palletY));
		snake->EatPallet();
		if(difficulty < max_difficulty && 
				snake->GetScore() >= difficulty * difficulty_step) {
			difficulty++;
		}
	}
}

void quit_game() {
	grid->Delete();
	snake->Delete();
	delete grid;
	delete snake;
	exit(0);
}

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 'q': 	quit_game(); break;   // Press q to force exit application
		case 'p':	if(!menu) {
						running = !running;
					}
					break;
    }
    glutPostRedisplay();
}

void special_keys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_UP:
			if(moved) {
				if(snake->SetDirection(UP)) {
					moved = false;
				}
			}
			break;
		case GLUT_KEY_RIGHT:
			if(moved) {
				if(snake->SetDirection(RIGHT)) {
					moved = false;
				}
			}
			break;
		case GLUT_KEY_DOWN:
			if(moved) {
				if(snake->SetDirection(DOWN)) {
					moved = false;
				}
			}	
			break;
		case GLUT_KEY_LEFT:
			if(moved) {
				if(snake->SetDirection(LEFT)) {
					moved = false;
				}
			}
			break;
	}
}

// void reshape(int w, int h)
// {
// 	glViewport(0, 0, w, h);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
    
//     //gluPerspective(40.0, 1.0f, 1.0, 5.0);

// 	glutPostRedisplay();
// }

void display_main_menu() {
	draw_header("Main Menu");
}

void display_options() {
	draw_header("Options");
}

void display_instructions() {
	draw_header("Instructions");
}

void display_gui() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Draw the separator between the HUD and play area
	glBegin(GL_LINES);
		glVertex2f(-h_limit, grid_top + screen_padding);
		glVertex2f(h_limit, grid_top + screen_padding);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	if(menu_screen == 0) {
		display_main_menu();
	} else if(menu_screen == 1) {
		display_options();
	} else if(menu_screen == 2) {
		display_instructions();
	} else if(menu_screen == 3) {
		menu = false;
		running = true;
	} else if(menu_screen == 4) {
		quit_game();
	}
	glutSwapBuffers();
}

void display_game() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Draw the separator between the HUD and play area
	glBegin(GL_LINES);
		glVertex2f(-h_limit, grid_top + screen_padding);
		glVertex2f(h_limit, grid_top + screen_padding);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
		// Draw the grid on which the snake and pallets will be displayed
		draw_grid();
		// Draw the snake
		draw_snake();
		// Draw the pallet
		glColor3f(.3f, .6f, .3f);
		draw_pallet();
		//draw HUD text
		glColor3f(1.0f, 1.0f, 1.0f);
		draw_state();
		draw_score();
    glutSwapBuffers();
}

void display() {
	if(menu) {
		display_gui();
	} else {
		display_game();
	}
}

void idle() {
	usleep(1000);	// Microsectonds. 1000 = 1 millisecond
	if(running) {
		ticks++;
		if( (ticks == max_delay - ( difficulty * delay_step ))) {
			if(snake->Move() != -1) {
				running = false;
				game_over = true;
			}
			ticks = 0;
			moved = true;
			glutPostRedisplay();
		}
		check_head_collisions();
	}
}

void mouse_action(int button, int state, int x, int y) {
	
}

void init_structs() {
	screen_height = arena_size + screen_padding + hud_height;
	screen_width = arena_size + screen_padding;
	h_limit = screen_width / 2;
	v_limit = screen_height / 2;
	grid_left = -h_limit + screen_padding;
	grid_right = h_limit - screen_padding;
	grid_top = v_limit - hud_height - screen_padding;
	grid_bot = -v_limit + screen_padding;
	grid = new Grid(arena_size, grid_size, screen_padding,
					grid_left,
					grid_top);
	snake = new Snake(3, 2, grid_size, loop);
	srand(time(NULL));
	int palletX = (int) ( rand() % ( grid_size - 1 ));
	int palletY = (int) ( rand() % ( grid_size - 1 ));
	pallet = new Pallet(palletX, palletY);
	ticks = 0;
	menu_screen = 0;
	float v_center_offset = v_limit - hud_height - screen_padding;
	buttonList->Refresh(.0f - v_center_offset);
	buttonList->AddButton("Play", 3);
	buttonList->AddButton("Options", 1);
	buttonList->AddButton("Instructions", 2)
	buttonList->AddButton("Quit", 4);
}

void init_gl(int argc, char* argv[]) {
	// if (argc>3)
	// 	g_program_obj = create_and_compile_shaders(argv[1], argv[2], argv[3]);

    // if (g_program_obj) {
        
    // }
	
    // Set viewport size (=scren size) and orthographic viewing
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	gluOrtho2D(-h_limit, h_limit, -v_limit, v_limit);

	glClearColor(.0f, 0.2f, .0f, 1.0f);
	g_bitmap_text_handle = make_bitmap_text();
}

int main(int argc, char* argv[]) {
	// For a properly working game, grid order must be greater than 5
	if(grid_size > 5 && arena_size > 200) {
		init_structs();
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
		glutInitWindowSize(screen_width, screen_height);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("Snake");

	#ifndef __APPLE__
		GLenum err = glewInit();
		if(GLEW_OK != err) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(1);
		}
		fprintf(stderr, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
	#endif

		glutKeyboardFunc(keyboard); 
		glutSpecialFunc(special_keys);
		glutMouseFunc(mouse_action);
		// glutReshapeFunc(reshape); 
		glutDisplayFunc(display);
		glutIdleFunc(idle);

		fprintf(stderr, "Open GL version %s\n", glGetString(GL_VERSION));
		init_gl(argc, argv); 


		glutMainLoop();
	} else {
		printf("Selected grid size (%d) or arena size (%d) are too small!",
				grid_size, arena_size);
	} 

	return 0; 
}