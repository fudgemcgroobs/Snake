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
#include <math.h>
#include "grid.h"
#include "snake.h"
#include "pellet.h"
#include "buttonlist.h"
#include "load_and_bind_texture.h"

enum tex { INTER=0, HEADFRONT=1, HEADRIGHT=2, HEADLEFT=3, HEADTOP=4,
		   SEGMENT=5, SEGMENTT=6, TURNL=7, TURNR=8,
		   TAILR=9, TAILL=10, TAILT=11, END=12,
		   GRASS=13, DIRT=14, APPLE=15, APPLETOP=16, TEXNUM=17 };
enum cube_side { L=0, R=1, T=2, B=3, F=4, N=5 };

float arena_size = 600.0f;		// The size, in world units, of the play area
float screen_pad = 5.0f;	// In world coordinates/sizes
float hud_height = 50.0f;		// The height of the HUD in the world
float y_pos = .0f;
float x_pos = .0f;
float z_pos = 300.0f;
float x_ref = .0f;
float y_ref = .0f;
float z_ref = .0f;
float extend = 100.0f;
float text_size = .2f;
float camlerp = .0f;
float plank_pad = 5.0f;
float plank_th = 10.0f;
float plank_size;
float cam_angle;
float screen_height;	// The total height of the viewport and screen
float screen_width;		// The total width of the viewport and screen
float h_limit;			// The horizontal limit at which can draw
float v_limit;			// The vertical limit at which can draw
float grid_left;		// The left edge of the grid (x-coordinate)
float grid_right;		// The right edge of the grid (x-coordinate)
float grid_top;			// The top edge of the grid (y-coordinate)
float grid_bot;			// The bottom edge of the grid (y-coordinate)
float view_rad;			// The radius of the circle followed by the spinning camera

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

bool menu;		// If game is in menu
bool running;	// If game is running at the moment
bool game_over;	// If the game has been lost
bool moved;		// If the snake has moved since the last direction change
bool loop;		// If the snake is allowed to loop at edges of screen
bool display_grid;
bool invisible;
bool fp;
int ticks;			// Ticks that have been counted. Resets depending on difficulty
int menu_screen;	// The current menu screen (check Destination in button.h for options)
unsigned int grid_size = 15;	// The order of the grid/matrix. Must be >5
unsigned int difficulty_step = 2;	// The required score change for difficulty increase
unsigned int max_difficulty = 9;
unsigned int delay_step = 1;		// Tick difference between difficulties
unsigned int max_delay = 23;		// The largest delay (in ticks) between snake steps
unsigned int g_bitmap_text_handle = 0;
unsigned int y_up = 0;
unsigned int z_up = 1;
unsigned int plank_num = 10;
unsigned int difficulty;		// The current difficulty level
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
	textures[DIRT] = load_and_bind_texture("./images/dirt.png");
	textures[APPLE] = load_and_bind_texture("./images/apple.png");
	textures[APPLETOP] = load_and_bind_texture("./images/appletop.png");
}

void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(-h_limit, h_limit, -v_limit, v_limit);
	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();
	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
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

void move_fp() {
	// Move the camera on top of the head and point in right direction
	unsigned int* head = snake->GetHeadPosition();

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

void draw_header(const char* text) {
	glPushMatrix();
		float h_center_offset = -str_width(text) / ( 2 / text_size );
		float v_center_offset = v_limit - screen_pad - ( hud_height / 2 );
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
		float h_center_offset = -h_limit + screen_pad;
		float v_center_offset = v_limit - screen_pad - ( hud_height / 2 );
		glTranslatef(.0f + h_center_offset, .0f + v_center_offset, .0f);
		glScalef(text_size, text_size, 1.0f);
		draw_text(text);
	glPopMatrix();
}

int normalize(int x, int a1, int a2, int b1, int b2) {
	return b1 + ( (x - a1) * (b2 - b1) / (a2 - a1) );
}

/*
* Method that draws a square of side size 1.0
* scaling on x or y by a number N will result in a respective side of size N
*/
void draw_square() {
	static float vertex[4][3] = {
		{.0f, .0f, .0f},
		{1.0f, .0f, .0f},
		{1.0f, -1.0f, .0f},
		{.0f, -1.0f, .0f}
	};

	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 4; i++) {
			glVertex3fv(vertex[i]);
		}
	glEnd();
}

void draw_cube() {
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
	glPushMatrix();
		float size = grid_right - grid_left + (2*extend);
		glTranslatef(grid_left - extend, grid_top + extend, -size);
		glScalef(size, size, size);

		glEnable(GL_TEXTURE_2D);
			draw_textured_side(DIRT, L);
			draw_textured_side(DIRT, R);
			draw_textured_side(DIRT, T);
			draw_textured_side(DIRT, B);
			draw_textured_top(GRASS, N, RIGHT);
		glDisable(GL_TEXTURE_2D);				
	glPopMatrix();
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

void draw_3D_pellet() {
	Cell* new_cell = grid->GetCellAt(pellet->GetY(), pellet->GetX());
	glPushMatrix();
		float size = grid->GetCellSize();
		glTranslatef(new_cell->GetX(), new_cell->GetY(), .0f);
		glScalef(size, size, size);
		glEnable(GL_TEXTURE_2D);
			draw_textured_side(APPLE, L);
			draw_textured_side(APPLE, R);
			draw_textured_side(APPLE, T);
			draw_textured_side(APPLE, B);
			draw_textured_top(APPLETOP, N, RIGHT);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void draw_plank() {
	glPushMatrix();
		glScalef(plank_th, plank_size, 100.0f);
		glColor3f(0.5f, 0.35f, 0.05f);
		draw_cube();
	glPopMatrix();
}

void draw_outline() {
	glPushMatrix();
		glTranslatef(grid_left - screen_pad - plank_th,
					 grid_bot - (plank_size / 2), .0f);
		for(size_t i = 1; i <= 4; i++) {
			for(size_t j = 1; j <= plank_num; j++) {
				glTranslatef(.0f, plank_size + plank_pad, .0f);
				draw_plank();
			}
			glRotatef(-90.0f, .0f, .0f, 1.0f);
		}
	glPopMatrix();
}

void display_gui() {
	glPushMatrix();
		glTranslatef(.0f, .0f, 1.0f);
		glColor3f(1.0f, 1.0f, 1.0f);

		glMatrixMode(GL_MODELVIEW);
		if(menu_screen == MAIN) {
			draw_header("Main Menu");
		} else if(menu_screen == OPTIONS) {
			draw_header("Options");
		} else if(menu_screen == INSTRUCTIONS) {
			draw_header("Instructions");
		} else if(menu_screen == GAME) {
			draw_state();
			draw_score();
		} else if(menu_screen == QUIT) {
			quit_game();
		}
		buttonList->DrawButtons();
	glPopMatrix();
}

void display_game() {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x_pos, y_pos, z_pos, // eye position
			  x_ref, y_ref, z_ref, // reference point
			  0, y_up, z_up  // up vector
		);
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
	draw_3D_pellet();
	// Draw arena outline
	draw_outline();
	glDisable(GL_DEPTH_TEST);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	display_game();
	
	setOrthographicProjection();
		glPushMatrix();
		glLoadIdentity();
		display_gui();
		glPopMatrix();
	restorePerspectiveProjection();

	glutSwapBuffers();
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
	bool onSnake = false;
	if(positions[0][0] == pellet->GetY() &&
			positions[0][1] == pellet->GetX()) {
		int pelletX;
		int pelletY;
		do{
			onSnake = false;
			pelletX = (int) ( rand() % ( grid_size - 1 ));
			pelletY = (int) ( rand() % ( grid_size - 1 ));
			for(size_t i = 0; i < snake->GetLength(); i++) {
				if(pelletX == positions[i][1] && pelletY == positions[i][0]) {
					onSnake = true;
					break;
				}
			}
		} while(!pellet->Reposition(pelletX, pelletY) || onSnake);
		snake->EatPellet();
		if(difficulty < max_difficulty && 
				snake->GetScore() >= difficulty * difficulty_step) {
			difficulty++;
		}
	}
	for ( int i = 0; i < snake->GetLength() - 1; i++) {
		delete [] positions[i];
	}
	delete [] positions;
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
						x_pos = 0;
						y_pos = 0;
						y_up = 1;
						z_up = 0;
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
	menu = true;		// If game is in menu
	running = false;	// If game is running at the moment
	game_over = false;	// If the game has been lost
	moved = false;		// If the snake has moved since the last direction change
	loop = true;		// If the snake is allowed to loop at edges of screen
	display_grid = false;
	invisible = false;
	fp = false;
	difficulty = 0;
	screen_height = arena_size + screen_pad;
	screen_width = arena_size + screen_pad;
	h_limit = screen_width / 2;
	v_limit = screen_height / 2;
	grid_left = -h_limit + screen_pad;
	grid_top = v_limit - screen_pad;
	view_rad = (grid_right - grid_left) / 64;
	cam_angle = .0f;
	grid = new Grid(arena_size, grid_size, screen_pad,
					grid_left,
					grid_top);
	grid_right = grid_left + (grid_size * grid->GetCellSize());
	grid_bot = grid_top - (grid_size * grid->GetCellSize());
	
	plank_size = (grid_right - grid_left) / plank_num;
	printf("plank num is %d and plank size is %f\n", plank_num, plank_size);
	fflush(stdout);

	snake = new Snake(3, 2, grid_size, loop);

	srand(time(NULL));
	int pelletX = (int) ( rand() % ( grid_size - 1 ));
	int pelletY = (int) ( rand() % ( grid_size - 1 ));
	pellet = new Pellet(pelletX, pelletY);
	ticks = 0;

	menu_screen = 0;
	int v_center_offset = v_limit - hud_height - screen_pad;
	buttonList = new ButtonList(0 + v_center_offset, screen_width, 40);
	buttonList->AddButton("Play", GAME);
	buttonList->AddButton("Options", OPTIONS);
	buttonList->AddButton("Instructions", INSTRUCTIONS);
	buttonList->AddButton("Quit", QUIT);
}

void init_gl(int argc, char* argv[]) {
    // Set viewport size (=scren size) and orthographic viewing
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	gluPerspective(grid_left - grid_right, 1.0f, 200.0f, -100.0f);
	g_bitmap_text_handle = make_bitmap_text();
	
	load_and_bind_textures();
	GLenum error = glGetError();
	if (error!=GL_NO_ERROR) {
		printf("GL error %s\n", gluErrorString(error));
		fflush(stdout);
	}
}

void reshape(int w, int h) {
	screen_width = w;
	screen_height = h;
	h_limit = screen_width / 2;
	v_limit = screen_height / 2;
    // Set viewport size (=scren size) and orthographic viewing
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// Specify a projection with this view volume, centred on origin 
	// Takes LEFT, RIGHT, BOTTOM, TOP, NEAR and FAR
	gluPerspective(grid_left - grid_right, 1.0f, 200.0f, -100.0f);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 'q': 	quit_game(); break;   // Press q to force exit application
		case 'p':	if(!menu) {
						running = !running;
					}
					break;
		case 'f':   fp = !fp;
					if(!fp) {
						x_pos = 0;
						y_pos = 0;
						z_pos = 2;
						x_ref = 0;
						y_ref = 0;
						z_ref = 0;
					} else {
						move_fp();
					}
					break;
		case 'y': 	y_pos -= 10;
					if(y_pos < -100){
						y_pos = -100;
					}
					break;
		case 'Y': 	y_pos += 10;
					if(y_pos > 100) {
						y_pos = 100;
					}
					break;
		case 'x': 	x_pos -= 10;
					if(x_pos < -100) {
						x_pos = -100;
					}
					break;
		case 'X':	x_pos += 10;
					if(x_pos > 100) {
						x_pos = 100;
					}
					break;
		case 'i':   invisible = !invisible; break;
    }
    glutPostRedisplay();
}

void special_keys(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_UP:
			if(!fp && moved) {
				if(snake->SetDirection(UP)) {
					moved = false;
				}
			}
			break;
		case GLUT_KEY_RIGHT:
			if(fp) {
				switch(snake->GetDirection()) {
					case UP: snake->SetDirection(RIGHT); break;
					case DOWN: snake->SetDirection(LEFT); break;
					case LEFT: snake->SetDirection(UP); break;
					case RIGHT: snake->SetDirection(DOWN); break;
				}
			} else if(moved) {
				if(snake->SetDirection(RIGHT)) {
					moved = false;
				}
			}
			break;
		case GLUT_KEY_DOWN:
			if(!fp && moved) {
				if(snake->SetDirection(DOWN)) {
					moved = false;
				}
			}	
			break;
		case GLUT_KEY_LEFT:
			if(fp) {
				switch(snake->GetDirection()) {
					case UP: snake->SetDirection(LEFT); break;
					case DOWN: snake->SetDirection(RIGHT); break;
					case LEFT: snake->SetDirection(DOWN); break;
					case RIGHT: snake->SetDirection(UP); break;
				}
			} else if(moved) {
				if(snake->SetDirection(LEFT)) {
					moved = false;
				}
			}
			break;
	}
}

void idle() {
	usleep(1000);	// Microsectonds. 1000 = 1 millisecond
	ticks++;
	if(menu && ticks == 8) {
			cam_angle = cam_angle < 360.0f ? cam_angle + 0.2f : .0f;
			camlerp += (cam_angle - camlerp) * 0.01f;
			x_pos = view_rad*cos(camlerp);
			y_pos = view_rad*sin(camlerp);
			ticks = 0;
	} else if(running) {
		if( (ticks >= max_delay - ( difficulty * delay_step ))) {
			if(snake->Move() != -1) {
				running = false;
				game_over = true;
			} else if(fp) {
				move_fp();
			}
			ticks = 0;
			moved = true;
			glutPostRedisplay();
		}
		check_head_collisions();
	}
	glutPostRedisplay();
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