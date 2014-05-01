#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <GL/glew.h>
#include <GL/glut.h>

#ifndef _OPENGL_H
#define _OPENGL_H

class Shader 
{
    GLuint _program;
    GLuint _vs;
    GLuint _fs;
    std::map<const char*, GLint> _uniforms;

    GLuint compile(const char*, GLenum);
    GLuint link();

    GLint get_uniform(const char*);

public:
    Shader(const char*, const char*);
    GLuint program();
    void kill();
    void use();
    void unuse();
    void set_uniform(const char*, int);
    void set_uniform(const char*, float);
    void set_uniform(const char*, int c, float*);
};


class Texture
{
    GLuint _id;

    public:
        Texture(void*, int, int);
        ~Texture();
        void bind();
        void unbind();
};

#endif
