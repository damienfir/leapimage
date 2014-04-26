#include "opengl.h"

#include <iostream>
#include <exception>

using namespace std;

/* VBO vbo_init(GLfloat* vertices) { */
/*     VBO out; */

/*     glGenBuffers(1, &(out.vbo)); */
/*     glBindBuffer(GL_ARRAY_BUFFER, out.vbo); */
/*     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); */

/*     vbo_unbind(); */

/*     return out; */
/* } */

/* void vbo_bind(VBO vbo) */
/* { */
/*     glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo); */
/* } */

/* void vbo_unbind() */
/* { */
/*     glBindBuffer(GL_ARRAY_BUFFER, 0); */
/*     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
/* } */

/* void vbo_kill(VBO vbo) */
/* { */
/*     glDeleteBuffers(1,vbo.vbo); */
/* } */


char* read_file(const char* filename){
    long n, r;
    FILE *fp = fopen(filename, "r");
    if (NULL == fp) {
        std::cout << "[shader] file " << filename << " doesn't exist..." << std::endl;
        throw std::exception();
    }

    fseek(fp, 0L, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    char * out = (char*)malloc(n);
    r = fread(out, 1, n, fp);
    fclose(fp);
    out[r-1] = '\0';

    return out;
}

Shader::Shader(const char *vs, const char *fs)
{
    _vs = compile(vs, GL_VERTEX_SHADER);
    _fs = compile(fs, GL_FRAGMENT_SHADER);
    _program = link(); 
}

GLuint Shader::compile(const char *filename, GLenum type)
{
    GLint compile_ok = GL_FALSE;
    GLuint id = glCreateShader(type);

    const char *source = read_file(filename);
    glShaderSource(id, 1, &source, NULL);
    free((void*)source);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        GLint log_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        char* log = (char*)malloc(log_length);
        glGetShaderInfoLog(id, log_length, NULL, log);
        std::cerr << "[shaders] cannot compile: " << log << std::endl;
        free(log);

        throw std::exception();
    }

    return id;
}

GLuint Shader::link()
{
    GLint link_ok = GL_FALSE;

    GLuint id = glCreateProgram();
    glAttachShader(id, _vs);
    glAttachShader(id, _fs);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        GLint log_length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
        char* log = (char*)malloc(log_length);
        glGetProgramInfoLog(id, log_length, NULL, log);
        std::cerr << "[shaders] cannot link: " << log << std::endl;
        free(log);

        throw std::exception();
    }

    return id;
}


GLuint Shader::program()
{
    return _program;
}

void Shader::kill()
{
    glDeleteProgram(_program);
}

GLint Shader::get_uniform(const char* name)
{
    if (_uniforms.find(name) == _uniforms.end())
    {
        GLint uniform = glGetUniformLocation(_program, name);
        _uniforms[name] = uniform;
    }
    return _uniforms[name];
}

void Shader::set_uniform(const char* name, int value)
{
    glUniform1i(get_uniform(name), value);
}

void Shader::set_uniform(const char* name, float value)
{
    glUniform1f(get_uniform(name), value);
}

/* void get_attribute(const char *name) */
/* { */
/*  attributes[name] = glGetAttribLocation(program, name); */
/*  if (attributes[name] == -1) { */
/*      fprintf(stderr, "Could not get attribute %s\n", name); */
/*  } */
/* } */

/* void get_uniform(const char *name) */
/* { */
/*  uniforms[name] = glGetUniformLocation(program, name); */
/*  if (uniforms[name] == -1) { */
/*      fprintf(stderr, "Could not get uniform %s\n", name); */
/*  } */
/* } */

/* void bind_attribute(Shader shader, GLuint location, const GLchar* name) */
/* { */
/*  shader.attrib[location] = location; */
/*  glBindAttribLocation(shader.program, location, name); */
/* } */

void Shader::use()
{
    glUseProgram(_program);
    /* map<string, GLint>::iterator p; */
    /* for (p = attributes.begin(); p != attributes.end(); ++p) { */
    /*  glEnableVertexAttribArray(p->second); */
    /* } */
}
