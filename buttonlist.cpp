/**
 * Implementation of the ButtonList class.
 */
#include "buttonlist.h"
#include <stdio.h>

/**
 * Initialises the empty list object setting the next_y to the passed top_y
 *  value, and calculating the width based on the screen width and the passed
 *  ratio.
 */
ButtonList::ButtonList(int n_y, int screen_w, int ratio) {
    head = NULL;
    tail = NULL;
    top_y = n_y;
    next_y = n_y;
    width = screen_w * ratio / 100;
    width = width < 100 ? 100 : width;  // Does not llow width less than 100
    pad = BUTTON_HEIGHT / 4;
    count = 0;
}

/**
 * Clears the button list, resetting the count and next_y values.
 */
void ButtonList::Refresh() {
    head = NULL;
    tail = NULL;
    next_y = top_y;
    count = 0;
}

/**
 * Adds a new Button object to the list, to be drawn under the last button.
 */
void ButtonList::AddButton(const char* name, Destination d) {
    Button* b = new Button(next_y, next_y - BUTTON_HEIGHT,
                            0 - (width / 2), 0 + (width / 2),
                            d, name, BUTTON_HEIGHT - 39.8f);
    next_y = next_y - BUTTON_HEIGHT - pad;
    if(head != NULL) {
        tail->SetNext(b);
        tail = b;
    } else {
        // This is the first element added since initilisation of refresh
        head = b;
        tail = b;
    }
    count++;
}

/**
 * Releases memory allocated when creating Button objects.
 */
void ButtonList::Delete() {
    Button* b = head;
    while(b != NULL) {
        Button* n = b->GetNext();
        b->Delete();
        delete b;
        b = n;
    }
}

/**
 * Draw the stored buttons.
 * Each stored button contains the necessary information to draw.
 */
void ButtonList::DrawButtons() {
    Button* b = head;
    while(b != NULL) {
        b->DrawButton();
        b = b->GetNext();
    }
}

/**
 * Returns a bidimensional array contaoining the boundries of each button,
 *  as well as the Destination (effect) of each.
 */
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