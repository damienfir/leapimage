#include <iostream>
#include <string>

#include <FreeImage.h>

#include "opengl.h"
#include "Leap.h"

using namespace Leap;
using namespace std;


Frame _first_frame;
Controller* controller;
Shader* shader;

class Viewer {
    GLuint texture;

public:
    void scale_image(float s)
    {
        /* shader->set_uniform("scale", s); */
        glutPostRedisplay();
    }

    void init_gl(int *argc, char *argv[])
    {
        glutInit(argc, argv);
        glutCreateWindow("leapimage");

        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

        glutDisplayFunc(glut_display);
        glutKeyboardFunc(glut_keyboard);
        glutIdleFunc(glut_idle);

        glewInit();

        load_image();
        load_shaders();

        glClearColor(0,0,0,0);
    }

    ~Viewer()
    {
        delete shader;
    }

private:
    void load_image()
    {
        string fname = "boat.jpg";
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
        shader->set_uniform("scale", 1.0f);
        shader->set_uniform("rot", 0.0f);

    }

    static void glut_display()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glutSwapBuffers();
    }

    static void glut_keyboard(unsigned char key, int x, int y)
    {
        /* shader->set_uniform("scale", 2.0f); */
    }

    static void glut_idle()
    {
        if (!_first_frame.isValid()) {
            _first_frame = controller->frame();
        } else {
            Frame f = controller->frame();

            float scale = f.scaleFactor(_first_frame);
            float rotation = - f.rotationAngle(_first_frame);

            cout << "[leap] " << f.toString() << " -> " << scale << endl;

            shader->set_uniform("scale", scale);
            shader->set_uniform("rot", rotation);
            /* viewer->scale_image(scale); */
            glutPostRedisplay();
        }
    }
};

class ImageListener: public Listener {

    Frame _first_frame;
    Viewer* viewer;

public:
    ImageListener(Viewer* v): viewer(v) {}

    /* virtual void onInit(const Controller &c) {} */
    /* virtual void onConnect(const Controller &c) {} */
    /* virtual void onDisconnect(const Controller &c) {} */
    /* virtual void onExit(const Controller &c) {} */

    virtual void onFrame(const Controller &c)
    {
        if (!_first_frame.isValid()) {
            _first_frame = c.frame();
        } else {
            Frame f = c.frame();

            float scale = f.scaleFactor(_first_frame);

            /* cout << "[leap] " << f.toString() << " -> " << scale<< endl; */

            viewer->scale_image(scale);
        }
    }
};

int main(int argc, char *argv[])
{
    controller = new Controller();

    Viewer* v = new Viewer();
    v->init_gl(&argc, argv); 

    /* ImageListener listener(v); */

    /* c.addListener(listener); */

    glutMainLoop();

    /* c.removeListener(listener); */
    delete v;
    
    return 0;
}
