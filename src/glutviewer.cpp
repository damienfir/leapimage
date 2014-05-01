#include "glutviewer.h"

GlutViewer* GlutViewer::self = NULL;

GlutViewer::GlutViewer(): _fullscreen(false), _scale(1), _angle(0)
{
    controller = new Leap::Controller();
    self = this;
}

GlutViewer::~GlutViewer()
{
    delete controller;
}

void GlutViewer::init_gl(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutCreateWindow("leapimage");

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutDisplayFunc(_display);
    glutKeyboardFunc(_keyboard);
    glutIdleFunc(_idle);
    glutReshapeFunc(_reshape);
    glutTimerFunc(0, _timer, 0);

    glewInit();

    glClearColor(0,0,0,0);
}

void GlutViewer::set_image(LeapImage* leapimage)
{
    image = leapimage;
}

void GlutViewer::run()
{
    glutMainLoop();
}


void GlutViewer::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glutSwapBuffers();
}

void GlutViewer::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'f':
            if (!_fullscreen) {
                glutFullScreen();
            } else {
                glutReshapeWindow(_width, _height);
            }
            _fullscreen = !_fullscreen;
            break;
    }
}

void GlutViewer::reshape(int w, int h)
{
    _width = w;
    _height = h;
    glViewport(0,0, w, h);
}

void GlutViewer::idle() {}

void GlutViewer::timer(int value)
{
    if (!_first_frame.isValid()) {
        _first_frame = controller->frame();
        _previous_frame = _first_frame;
    } else {
        _current_frame = controller->frame();

        float s = _current_frame.scaleFactor(_previous_frame);
        float a = _current_frame.rotationAngle(_previous_frame, Leap::Vector::yAxis());
        Leap::Vector t = _current_frame.translation(_previous_frame);
        float p = _current_frame.scaleProbability(_previous_frame);

        /* cout << a << " -- " << s << "(" << p << ")" << endl; */

        _scale *= s;
        _angle += a;
        _trans += t;

        image->scale(_scale);
        image->rotate(_angle);
        image->translate(_trans.x, _trans.y, _trans.z);
        glutPostRedisplay();

        _previous_frame = _current_frame;
    }

    glutTimerFunc(1000.f/60.f, _timer, 0);
}
