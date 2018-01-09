#ifdef __APPLE__
#include <GLUT/glut.h> 
#else

#include <GL/glut.h> 
#endif

#include "button.h"
#include <stdio.h>
#include <cstring>

float Button::str_width(const char* s) {
	int len = strlen(s);
	int total_width = 0;
	for(int i = 0; i < len; i++) {	
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, s[i]) ;
	}
	return total_width;
}

void Button::draw_text(const char* s) {
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
	}
}

Button::Button(int t, int b, int l, int r, Destination d, const char* n, float t_s) {
    top = t;
    bot = b;
    left = l;
    right = r;
    dest = d;
    next = NULL;
    name = n;
    text_size = t_s;
}

void Button::Delete() {}

void Button::SetNext(Button* b) {
    next = b;
}

void Button::DrawButton() {
    glBegin(GL_LINE_LOOP);
        glVertex2f(left, top);
        glVertex2f(right, top);
        glVertex2f(right, bot);
        glVertex2f(left, bot);
    glEnd();

    glPushMatrix();
		float h_center_offset = -str_width(name)/(2/text_size);
		glTranslatef(.0f + h_center_offset,
            (top + bot) / 2 - (2/text_size),.0f);
		glScalef(text_size, text_size, 1.0f);
		draw_text(name);
	glPopMatrix();
}

int Button::GetTop() {
    return top;
}

int Button::GetBot() {
    return bot;
}

int Button::GetLeft() {
    return left;
}

int Button::GetRight() {
    return right;
}

Button* Button::GetNext() {
    return next;
}

Destination Button::GetDestination() {
    return dest;
}