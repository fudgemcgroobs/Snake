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
#include "pellet.h"
#include "buttonlist.h"
#include "load_and_bind_texture.h"

enum tex { INTER=0, HEADFRONT=1, HEADRIGHT=2, HEADLEFT=3, HEADTOP=4,
		   SEGMENT=5, SEGMENTT=6, TURNL=7, TURNR=8,
		   TAILR=9, TAILL=10, TAILT=11, END=12,
		   GRASS=13, TEXNUM=14 };
enum cube_side { L=0, R=1, T=2, B=3, F=4, N=5 };

float arena_size = 600.0f;		// The size, in world units, of the play area
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
float extend;	// Size by which a 3D snake segment should be elongated	

// Cube edges: Right, Left etc.
float c_r = 1.0f;
float c_l = .0f;
float c_t = .0f;
float c_b = -1.0f;
float c_f = .0f;
float c_n = 1.0f;

// Coordinates of cube corners divided into sides
static float cube[6][4][3] = {
	{{c_l, c_t, c_n}, {c_l, c_b, c_n}, {c_l, c_b, c_f}, {c_l, c_t, c_f}}, // Left
	{{c_r, c_b, c_n}, {c_r, c_t, c_n}, {c_r, c_t, c_f}, {c_r, c_b, c_f}}, // Right
	{{c_r, c_t, c_n}, {c_l, c_t, c_n}, {c_l, c_t, c_f}, {c_r, c_t, c_f}}, // Top
	{{c_l, c_b, c_n}, {c_r, c_b, c_n}, {c_r, c_b, c_f}, {c_l, c_b, c_f}}, // Bottom
	{{c_l, c_b, c_f}, {c_l, c_t, c_f}, {c_r, c_t, c_f}, {c_r, c_b, c_f}}, // Far
	{{c_l, c_t, c_n}, {c_r, c_t, c_n}, {c_r, c_b, c_n}, {c_l, c_b, c_n}}  // Near
};
static int tex_source_coords[4][2] {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

bool menu = true;		// If game is in menu
bool running = false;	// If game is running at the moment
bool game_over = false;	// If the game has been lost
bool moved = false;		// If the snake has moved since the last direction change
bool loop = true;		// If the snake is allowed to loop at edges of screen
bool display_grid = false;
bool invisible = false;
int ticks;			// Ticks that have been counted. Resets depending on difficulty
int menu_screen;	// The current menu screen (check Destination in button.h for options)
int y_tilt = 0;
int x_tilt = 0;
unsigned int grid_size = 15;	// The order of the grid/matrix. Must be >5
unsigned int difficulty = 0;		// The current difficulty level
unsigned int difficulty_step = 2;	// The required score change for difficulty increase
unsigned int max_difficulty = 9;
unsigned int delay_step = 10;		// Tick difference between difficulties
unsigned int max_delay = 140;		// The largest delay (in ticks) between snake steps
unsigned int g_bitmap_text_handle = 0;
unsigned int textures[TEXNUM];
Grid* grid;				// Stores grid cell coordinates
Snake* snake;			// Stores snake information and allows snake movement
Pellet* pellet;			// Stores food pellet info and provides pellet functionality
ButtonList* buttonList;	// Stores GUI buttons and their information/effects

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

void load_and_bind_textures()
{
	textures[INTER] = load_and_bind_texture("./images/inter.png");
	textures[HEADFRONT] = load_and_bind_texture("./images/headfront.png");
	textures[HEADRIGHT] = load_and_bind_texture("./images/headright.png");
	textures[HEADLEFT] = load_and_bind_texture("./images/headleft.png");
	textures[HEADTOP] = load_and_bind_texture("./images/headtop.png");
	textures[SEGMENT] = load_and_bind_texture("./images/segment.png");
	textures[SEGMENTT] = load_and_bind_texture("./images/segmentt.png");
	textures[TURNL] = load_and_bind_texture("./images/turnl.png");
	textures[TURNR] = load_and_bind_texture("./images/turnr.png");
	textures[TAILR] = load_and_bind_texture("./images/tailr.png");
	textures[TAILL] = load_and_bind_texture("./images/taill.png");
	textures[TAILT] = load_and_bind_texture("./images/tailt.png");
	textures[END] = load_and_bind_texture("./images/end.png");

	textures[GRASS] = load_and_bind_texture("./images/grass.png");
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

int normalize(int x, int a1, int a2, int b1, int b2) {
	return b1 + ( (x - a1) * (b2 - b1) / (a2 - a1) );
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

void draw_cube() {
	glColor3f(1.0f, 1.0f, 1.0f);
	for(size_t i = 0; i < 6; i++) {
		glBegin(GL_QUADS);
			for(size_t j = 0; j < 4; j++) {
				glVertex3fv(cube[i][j]);
			}
		glEnd();
	}
}

void draw_textured_side(tex source, cube_side side) {
	glBindTexture(GL_TEXTURE_2D, textures[source]);
	glBegin(GL_QUADS);
		for(size_t i = 0; i < 4; i++) {
			glTexCoord2f(tex_source_coords[i][0],
						 tex_source_coords[i][1]);
			glVertex3fv(cube[side][i]);
		}
	glEnd();
}

void draw_textured_top(tex source, cube_side side, unsigned int dir) {
	glBindTexture(GL_TEXTURE_2D, textures[source]);
	glBegin(GL_QUADS);
		unsigned int add = 1;
		switch(dir) {
			case LEFT: add = 3; break;
			case UP: add = 0; break;
			case DOWN: add = 2; break;						
		}
		for(size_t i = 0; i < 4; i++) {
			glTexCoord2f(tex_source_coords[i][0],
						 tex_source_coords[i][1]);
			glVertex3fv(cube[side][(i+add)%4]);
		}
	glEnd();
}

void draw_grass() {
	glBindTexture(GL_TEXTURE_2D, textures[GRASS]);
	glBegin(GL_QUADS);
		glTexCoord2f(.0f, .0f);
		glVertex3f(-h_limit, -h_limit, .2f);
		glTexCoord2f(1.0f, .0f);
		glVertex3f(h_limit, -h_limit, .2f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(h_limit, h_limit, .2f);
		glTexCoord2f(.0f, 1.0f);
		glVertex3f(-h_limit, h_limit, .2f);
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
					draw_square();
				glPopMatrix();
			}
		}
}

void draw_head(unsigned int dir) {
	cube_side order[5] = {L, R, B, T, N};
	switch(dir) {
		case LEFT: order[0] = R; order[1] = L;
				   order[2] = T; order[3] = B; break;
		case UP: order[0] = B; order[1] = T;
				 order[2] = R; order[3] = L; break;
		case DOWN: order[0] = T; order[1] = B;
				   order[2] = L; order[3] = R; break;
	}
	glEnable(GL_TEXTURE_2D);
		for(size_t i = 0; i < 4; i++) {
			draw_textured_side((tex) i, order[i]);
		}
		draw_textured_top(HEADTOP, order[4], dir);
	glDisable(GL_TEXTURE_2D);
}

void draw_tail(unsigned int dir) {
	glEnable(GL_TEXTURE_2D);
		cube_side order[5] = {R, L, B, T, N};
		switch(dir) {
			case LEFT: order[0] = L; order[1] = R;
					   order[2] = T; order[3] = B; break;
			case UP: order[0] = T; order[1] = B;
					 order[2] = R; order[3] = L; break;
			case DOWN: order[0] = B; order[1] = T;
					   order[2] = L; order[3] = R; break;
		}
		draw_textured_side(INTER, order[0]);
		draw_textured_side(END, order[1]);
		draw_textured_side(TAILR, order[2]);
		draw_textured_side(TAILL, order[3]);
		draw_textured_top(TAILT, order[4], dir);
	glDisable(GL_TEXTURE_2D);
}

void draw_turn(unsigned int ahead, unsigned int behind) {
	glEnable(GL_TEXTURE_2D);
	cube_side order[5] = {L, R, B, T, N};
	switch(ahead) {
		case LEFT: order[0] = R; order[1] = L; break;
		case UP: order[0] = B; order[1] = T; break;
		case DOWN: order[0] = T; order[1] = B; break;
	}
	switch(behind) {
		case LEFT: order[2] = L; order[3] = R; break;
		case RIGHT: order[2] = R; order[3] = L; break;
		case UP: order[2] = T; order[3] = B; break;
	}
	draw_textured_side(SEGMENT, order[0]);
	draw_textured_side(INTER, order[1]);
	draw_textured_side(SEGMENT, order[2]);
	draw_textured_side(INTER, order[3]);
	if((ahead > behind || (ahead == UP && behind == LEFT)) &&
		!(ahead == LEFT && behind == UP)) {
		draw_textured_top(TURNL, order[4], ahead);
	} else {
		draw_textured_top(TURNR, order[4], ahead);
	}
	glDisable(GL_TEXTURE_2D);
}

void draw_segment(unsigned int dir) {
	glEnable(GL_TEXTURE_2D);
	cube_side order[5] = {L, R, B, T, N};
	switch(dir) {
		case LEFT: order[0] = R; order[1] = L;
				   order[2] = T; order[3] = B; break;
		case UP: order[0] = B; order[1] = T;
				 order[2] = R; order[3] = L; break;
		case DOWN: order[0] = T; order[1] = B;
				   order[2] = L; order[3] = R; break;
	}
	draw_textured_side(INTER, order[0]);
	draw_textured_side(INTER, order[1]);
	draw_textured_side(SEGMENT, order[2]);
	draw_textured_side(SEGMENT, order[3]);		
	draw_textured_top(SEGMENTT, order[4], dir);
	glDisable(GL_TEXTURE_2D);
}

void draw_3D_snake() {
	unsigned int** positions = snake->GetSnakePosition();
	for(int i = 0; i < snake->GetLength(); i++) {
		glPushMatrix();
			Cell* new_cell = grid->GetCellAt(positions[i][0], positions[i][1]);
			glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
			glScalef(grid->GetCellSize(), grid->GetCellSize(), grid->GetCellSize());
			if(i == 0) {
				draw_head(positions[i][2]);
			} else if(i == snake->GetLength() - 1) {
				draw_tail(positions[i][2]);
			} else if(!invisible) {
				if(positions[i][2] != positions[i][3]) {
					draw_turn(positions[i][2], positions[i][3]);
				} else {
					draw_segment(positions[i][2]);
				}
			}
		glPopMatrix();
	}

	for ( int i = 0; i < snake->GetLength(); i++) {
		delete [] positions[i];
	}
	delete [] positions;
}

void draw_pellet() {
	Cell* new_cell = grid->GetCellAt(pellet->GetY(), pellet->GetX());
	glPushMatrix();
		glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
		glScalef(grid->GetCellSize() - 0.5f, grid->GetCellSize() - 0.5f, 1.0f);
		glRectf(.0f, .0f, 1.0f, -1.0f);
	glPopMatrix();
}

void draw_header(const char* text) {
	glPushMatrix();
		float h_center_offset = -str_width(text) / ( 2 / text_size );
		float v_center_offset = v_limit - screen_padding - ( hud_height / 2 );
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
		text = "Eat the pellets!";
	}
	draw_header(text);
}

void draw_score() {
	std::string text("Score: ");
	text.append(std::to_string(snake->GetScore()));

	glPushMatrix();
		float h_center_offset = -h_limit + screen_padding;
		float v_center_offset = v_limit - screen_padding - ( hud_height / 2 );
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
	// If head collide w pellet
	unsigned int** positions = snake->GetSnakePosition();
	if(positions[0][0] == pellet->GetY() &&
			positions[0][1] == pellet->GetX()) {
		int pelletX;
		int pelletY;
		do{
			pelletX = (int) ( rand() % ( grid_size - 1 ));
			pelletY = (int) ( rand() % ( grid_size - 1 ));
		} while(!pellet->Reposition(pelletX, pelletY));
		snake->EatPellet();
		if(difficulty < max_difficulty && 
				snake->GetScore() >= difficulty * difficulty_step) {
			difficulty++;
		}
	}
}

void quit_game() {
	grid->Delete();
	snake->Delete();
	buttonList->Delete();
	delete grid;
	delete snake;
	delete buttonList;
	exit(0);
}

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 'q': 	quit_game(); break;   // Press q to force exit application
		case 'p':	if(!menu) {
						running = !running;
					}
					break;
		case 'y': 	y_tilt--; break;
		case 'Y': 	y_tilt++; break;
		case 'x': 	x_tilt--; break;
		case 'X':	x_tilt++; break;
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

void display_gui() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);
	// Draw the separator between the HUD and play area
	glBegin(GL_LINES);
		glVertex2f(-h_limit, grid_top + screen_padding);
		glVertex2f(h_limit, grid_top + screen_padding);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	if(menu_screen == MAIN) {
		draw_header("Main Menu");
	} else if(menu_screen == OPTIONS) {
		draw_header("Options");
	} else if(menu_screen == INSTRUCTIONS) {
		draw_header("Instructions");
	} else if(menu_screen == GAME) {
		menu = false;
		running = true;
	} else if(menu_screen == QUIT) {
		quit_game();
	}
	buttonList->DrawButtons();
	glutSwapBuffers();
}

void display_game() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x_tilt, y_tilt, 2, // eye position
			  0, 0, 0, // reference point
			  0, 1, 0  // up vector
		);

		glColor3f(1.0f, 1.0f, 1.0f);
		// Draw the separator between the HUD and play area
		glBegin(GL_LINES);
			glVertex2f(-h_limit, grid_top + screen_padding);
			glVertex2f(h_limit, grid_top + screen_padding);
		glEnd();

		draw_grass();
		// Draw the grid on which the snake and pellets will be displayed
		if(display_grid) {
			draw_grid();
		} else {
			// Draw edge
		}
		// Draw the snake
		draw_3D_snake();
		// Draw the pellet
		glColor3f(.3f, .6f, .3f);
		draw_pellet();
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
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		int** bounds = buttonList->GetButtonBounds();
		x = normalize(x, 0, screen_width, -h_limit, h_limit);
		y = normalize(y, 0, screen_height, v_limit, -v_limit);
		int checks = buttonList->GetCount();

		for(int i = 0; i < checks; i++) {
			if(y < bounds[i][0] && y > bounds[i][1] &&
			   		x > bounds[i][2] && x < bounds[i][3]) {
				menu_screen = bounds[i][4];
				switch(menu_screen) {
					case MAIN:
						buttonList->Refresh();
						buttonList->AddButton("Play", GAME);
						buttonList->AddButton("Options", OPTIONS);
						buttonList->AddButton("Instructions", INSTRUCTIONS);
						buttonList->AddButton("Quit", QUIT);
						break;
					case GAME:
						buttonList->Refresh();
						menu = false;
						running = true;
						break;
					case GRID:
						display_grid = !display_grid;
						menu_screen = OPTIONS;
						goto options;
						break;
					case LOOP:
						loop = !loop;
						snake->SetLoop(loop);
						menu_screen = OPTIONS;
						goto options;
						break;
					case OPTIONS:
						options:
						buttonList->Refresh();
						buttonList->AddButton("Back", MAIN);
						if(loop) {
							buttonList->AddButton("Loop: ON", LOOP);
						} else {
							buttonList->AddButton("Loop: OFF", LOOP);
						}
						if(display_grid) {
							buttonList->AddButton("Grid: ON", GRID);
						} else {
							buttonList->AddButton("Grid: OFF", GRID);							
						}
						break;
					case INSTRUCTIONS:
						break;
					case QUIT:
						quit_game();
						break;
					case PAUSE:
						break;
				}
			}
		}
		for ( int i = 0; i < checks; i++) {
			delete [] bounds[i];
		}
		delete [] bounds;
		glutPostRedisplay();
	}
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
	extend = grid->GetCellSize() / 2;
	snake = new Snake(3, 2, grid_size, loop);

	srand(time(NULL));
	int pelletX = (int) ( rand() % ( grid_size - 1 ));
	int pelletY = (int) ( rand() % ( grid_size - 1 ));
	pellet = new Pellet(pelletX, pelletY);
	ticks = 0;

	menu_screen = 0;
	int v_center_offset = v_limit - hud_height - screen_padding;
	buttonList = new ButtonList(0 + v_center_offset, screen_width, 40);
	buttonList->AddButton("Play", GAME);
	buttonList->AddButton("Options", OPTIONS);
	buttonList->AddButton("Instructions", INSTRUCTIONS);
	buttonList->AddButton("Quit", QUIT);
}

void reshape(int w, int h) {
	// Set viewport size (=scren size) and orthographic viewing
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	glOrtho(-h_limit, h_limit, -v_limit, v_limit, -10000, 10000);
	glutPostRedisplay();
}

void init_gl(int argc, char* argv[]) {
    // Set viewport size (=scren size) and orthographic viewing
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	glOrtho(-h_limit, h_limit, -v_limit, v_limit, -10000, 10000);
	g_bitmap_text_handle = make_bitmap_text();
	
	load_and_bind_textures();
	GLenum error = glGetError();
	if (error!=GL_NO_ERROR) {
		printf("GL error %s\n", gluErrorString(error));
		fflush(stdout);
	}
	glEnable(GL_DEPTH_TEST);
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
		glutReshapeFunc(reshape); 
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