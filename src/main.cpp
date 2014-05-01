#include "glutviewer.h"
#include "leapimage.h"


int main(int argc, char *argv[])
{
    GlutViewer* viewer = new GlutViewer();
    viewer->init_gl(&argc, argv); 

    LeapImage* image = new LeapImage("res/boat.jpg");
    viewer->set_image(image);

    viewer->run();

    delete image;
    delete viewer;
    
    return 0;
}
