#include <iostream>
#include <string>

#include <FreeImage.h>

#include "opengl.h"
#include "Leap.h"

using namespace std;


class LeapImage {
    GLuint texture;
    Shader* shader;
    string fname;

public:
    LeapImage(string fname): fname(fname)
    {
        load_image(fname);
        load_shaders();
    }

    ~LeapImage()
    {
        delete shader;
    }

    void rotate(float angle)
    {
        shader->set_uniform("rot", angle);
    }

    void scale(float s)
    {
        shader->set_uniform("scale", s);
    }

private:
    void load_image(string fname)
    {
        FreeImage_Initialise(1);
        FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fname.c_str(), 0);
        FIBITMAP *img = FreeImage_Load(fif, fname.c_str(), JPEG_DEFAULT);
        int width = FreeImage_GetWidth(img);
        int height = FreeImage_GetHeight(img);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, FreeImage_GetBits(img));
        FreeImage_Unload(img);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void load_shaders()
    {
        shader = new Shader("leapimage.v.glsl","leapimage.f.glsl");
        shader->use();

        GLuint vbo_vertices;
        GLfloat vertices[] = {-1.f,-1.f, 1.f,-1.f, -1.f,1.f, 1.f,1.f};
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint vertices_attrib = glGetAttribLocation(shader->program(), "vertices");
        glEnableVertexAttribArray(vertices_attrib);
        glVertexAttribPointer(vertices_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        shader->set_uniform("image", 0);

        rotate(0.0f);
        scale(1.0f);
    }
};


class GlutViewer {

    bool _fullscreen;
    int _width, _height;
    Leap::Controller* controller;
    Leap::Frame _first_frame, current_frame;
    LeapImage* image;

public:
    static GlutViewer* self;

public:
    GlutViewer(): _fullscreen(false)
    {
        controller = new Leap::Controller();
    }

    ~GlutViewer()
    {
        delete controller;
    }

    void init_gl(int *argc, char *argv[])
    {
        glutInit(argc, argv);
        glutCreateWindow("leapimage");

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

        glutDisplayFunc(_display);
        glutKeyboardFunc(_keyboard);
        glutIdleFunc(_idle);
        glutReshapeFunc(_reshape);

        glewInit();

        glClearColor(0,0,0,0);
    }

    void set_image(LeapImage* leapimage)
    {
        image = leapimage;
    }

    void run()
    {
        glutMainLoop();
    }

    static void _display() { self->display(); }
    static void _idle() { self->idle(); }
    static void _keyboard(unsigned char key, int x, int y) { self->keyboard(key, x, y); }
    static void _reshape(int w, int h) { self->reshape(w, h); }

private:

    void display()
    {
        cout << "display..." << endl;
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glutSwapBuffers();
    }

    void keyboard(unsigned char key, int x, int y)
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

    void reshape(int w, int h)
    {
        cout << "reshape..." << endl;
        _width = w;
        _height = h;
        glViewport(0,0, w, h);
    }

    void idle()
    {
        cout << "idle..." << endl;
        if (!_first_frame.isValid()) {
            _first_frame = controller->frame();
        } else {
            current_frame = controller->frame();

            float scale = current_frame.scaleFactor(_first_frame);
            float angle = - current_frame.rotationAngle(_first_frame);

            image->scale(scale);
            image->rotate(angle);
            glutPostRedisplay();
        }
    }
};


GlutViewer* GlutViewer::self = NULL;

int main(int argc, char *argv[])
{
    GlutViewer* viewer = new GlutViewer();
    viewer->init_gl(&argc, argv); 

    LeapImage* image = new LeapImage("boat.jpg");
    viewer->set_image(image);

    viewer->run();

    delete viewer;
    delete image;
    
    return 0;
}
