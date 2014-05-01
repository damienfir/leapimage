#include <Leap.h>

#include "leapimage.h"

#ifndef _GLUTVIEWER_H
#define _GLUTVIEWER_H

class GlutViewer
{
    bool _fullscreen;
    int _width, _height;
    float _scale, _angle;
    Leap::Vector _trans;
    Leap::Controller* controller;
    Leap::Frame _first_frame, _previous_frame, _current_frame;
    LeapImage* image;

public:
    static GlutViewer* self;

public:
    GlutViewer();
    ~GlutViewer();

    void init_gl(int *argc, char *argv[]);
    void set_image(LeapImage* leapimage);
    void run();

    static void _display() { self->display(); }
    static void _idle() { self->idle(); }
    static void _keyboard(unsigned char key, int x, int y) { self->keyboard(key, x, y); }
    static void _reshape(int w, int h) { self->reshape(w, h); }
    static void _timer(int value) { self->timer(value); }

private:
    void display();
    void keyboard(unsigned char key, int x, int y);
    void reshape(int w, int h);
    void idle();
    void timer(int value);
};

#endif
