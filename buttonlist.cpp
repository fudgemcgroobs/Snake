#include "buttonlist.h"
#include <stdio.h>

ButtonList::ButtonList(int n_y, int screen_w, int ratio) {
    head = NULL;
    tail = NULL;
    top_y = n_y;
    next_y = n_y;
    width = screen_w * ratio / 100;
    pad = BUTTON_HEIGHT / 4;
    width = width < 100 ? 100 : width;
    count = 0;
}

void ButtonList::Refresh() {
    head = NULL;
    tail = NULL;
    next_y = top_y;
    count = 0;
}

void ButtonList::AddButton(const char* name, Destination d) {
    Button* b = new Button(next_y, next_y - BUTTON_HEIGHT,
                            0 - (width / 2), 0 + (width / 2),
                            d, name, BUTTON_HEIGHT - 39.8f);
    next_y = next_y - BUTTON_HEIGHT - pad;
    if(head != NULL) {
        tail->SetNext(b);
        tail = b;
    } else {
        head = b;
        tail = b;
    }
    count++;
}

void ButtonList::DrawButtons() {
    Button* b = head;
    while(b != NULL) {
        b->DrawButton();
        b = b->GetNext();
    }
}

int** ButtonList::GetButtonBounds() {
    int** button_bounds = 0;
    button_bounds = new int*[count];
    Button* b = head;
    for(int i = 0; i < count && b != NULL; i++) {
        button_bounds[i] = new int[5];
        button_bounds[i][0] = b->GetTop();
        button_bounds[i][1] = b->GetBot();
        button_bounds[i][2] = b->GetLeft();
        button_bounds[i][3] = b->GetRight();
        button_bounds[i][4] = b->GetDestination();
        b = b->GetNext();
    }
    return button_bounds;
}

unsigned int ButtonList::GetCount() {
    return count;
}