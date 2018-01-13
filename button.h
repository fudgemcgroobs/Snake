/**
 * Class to store and display buttons to be used in a ButtonList
 *  to display GUI menus.
 */
#ifndef BUTTON_H
#define BUTTON_H
/**
 * Destinations describe button effects. A handler must be implemented externally
 *  to receive the button destination and take action (e.g. reinitialise the menu)
 */
enum Destination { QMAIN, MAIN, OPTIONS, QUIT, PAUSE, GAME, LOOP, GRID, INV, GSIZE };

class Button {
    float text_size;
    int top;    // The top limit of the button
    int bot;    // The bottom limit of the button
    int left;   // The left limit of the button
    int right;  // The right limit of the button
    Destination dest;   // The effectg of the button
    Button* next;       // The button to be drawn below this button
    const char* name;   // The description of the button
    float str_width(const char* s);
    void draw_text(const char* s);

    public:
        Button();
        Button(int t, int b, int l, int r, Destination d,
                const char* n, float t_s);
        void Delete();
        void SetNext(Button* b);
        void DrawButton();
        int GetTop();
        int GetBot();
        int GetLeft();
        int GetRight();
        Button* GetNext();
        Destination GetDestination();
};
#endif