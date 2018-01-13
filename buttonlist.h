/**
 * Linked list class which stores a list of buttons to be displayed.
*/
#ifndef BUTTONLIST_H
#define BUTTONLIST_H

#include "button.h"
#define BUTTON_HEIGHT 40

class ButtonList {
    Button* head;   // The top button
    Button* tail;   // The bottom button
    int top_y;  // The Y coordinate of the top of the list
    int width;  // The desired width of the buttons
    int pad;    // The padding between the buttons
    // The Y coordinate of the next button that will be added to the list
    int next_y;
    unsigned int count; // The number of buttons in the list

    public:
        ButtonList();
        ButtonList(int n_y, int screen_w, int ratio);
        void Refresh();
        void AddButton(const char* name, Destination d);
        void Delete();
        void DrawButtons();
        int** GetButtonBounds();
        unsigned int GetCount();
};
#endif