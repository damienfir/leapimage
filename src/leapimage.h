#include <string>
#include "opengl.h"

#ifndef _LEAPIMAGE_H
#define _LEAPIMAGE_H

using namespace std;

class LeapImage
{
    Texture* texture;
    Shader* shader;
    string fname;

public:
    LeapImage(string fname);
    ~LeapImage();
    void rotate(float angle);
    void scale(float s);
    void translate(float x, float y, float z);

private:
    void load_image(string fname);
    void load_shaders();
};

#endif
