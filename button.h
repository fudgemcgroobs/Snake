#ifndef BUTTON_H
#define BUTTON_H

enum Destination { QMAIN, MAIN, OPTIONS, QUIT, PAUSE, GAME, LOOP, GRID, INV, GSIZE };

class Button {
    float text_size;
    int top;
    int bot;
    int left;
    int right;
    Destination dest;
    Button* next;
    const char* name;
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