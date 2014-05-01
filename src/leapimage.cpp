#include <FreeImage.h>
#include "leapimage.h"


LeapImage::LeapImage(string fname): fname(fname)
{
    load_image(fname);
    load_shaders();
}


LeapImage::~LeapImage()
{
    delete shader;
    delete texture;
}


void LeapImage::rotate(float angle)
{
    shader->set_uniform("rot", angle);
}


void LeapImage::scale(float s)
{
    shader->set_uniform("scale", s);
}


void LeapImage::translate(float x, float y, float z)
{
    float s = 1e3;
    float t[] = {x/s, y/s, z/s};
    shader->set_uniform("trans", 3, t);
}


void LeapImage::load_image(string fname)
{
    FreeImage_Initialise(1);
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fname.c_str(), 0);
    FIBITMAP *img = FreeImage_Load(fif, fname.c_str(), JPEG_DEFAULT);
    int width = FreeImage_GetWidth(img);
    int height = FreeImage_GetHeight(img);

    texture = new Texture(FreeImage_GetBits(img), width, height);
    FreeImage_Unload(img);
}


void LeapImage::load_shaders()
{
    shader = new Shader("leapimage.v.glsl","leapimage.f.glsl");

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
    translate(0,0,0);
}
