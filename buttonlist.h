#ifndef BUTTONLIST_H
#define BUTTONLIST_H

#include "button.h"
#define BUTTON_HEIGHT 40

class ButtonList {
    Button* head;
    Button* tail;
    int top_y;
    int width;
    int pad;
    int next_y;
    unsigned int count;

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