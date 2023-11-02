#include "main.h"

int main( void )
{
    // unsigned int x_resolution = 512;
    // unsigned int y_resolution = 512;

    unsigned int x_resolution = 1024;
    unsigned int y_resolution = 1024;

    // unsigned int x_resolution = 2000;
    // unsigned int y_resolution = 2000;

    // unsigned int x_resolution = 4000;
    // unsigned int y_resolution = 4000;

    // OrthographicCamera camera{ -1.25f, 
    //                             1.25f, 
    //                            -1.25f, 
    //                             1.25f,
    //                             glm::ivec2{ x_resolution, y_resolution }, 
    //                             glm::vec3{ 0.0f, 0.0f,  1.0f },     // position
    //                             glm::vec3{ 0.0f, 1.0f,  0.0f },     // up
    //                             glm::vec3{ 0.0f, 0.0f, -1.0f } };   // look at

    PinholeCamera camera{      -1.25f, 
                                1.25f, 
                               -1.25f, 
                                1.25f,
                                2.5f,
                                glm::ivec2{ x_resolution, y_resolution }, 
                                glm::vec3{ 2.40649f, -4.01016f,  -4.11434f },     // position
                                glm::vec3{ 0.0f, 1.0f,  0.0f },     // up
                                glm::vec3{ -1.0f, -2.0f, 0.0f } };   // look at
                                
    Scene scene{};
    
    scene.load();

    Buffer rendering_buffer{ x_resolution, y_resolution };
    //glm::vec3 background_color{ 0.4f, 0.6f, 0.3f };
    glm::vec3 background_color{ 0.0f, 0.0f, 0.0f };

    // //Set up the renderer.
    // RayTracer rt( camera,
    //               scene,
    //               background_color,
    //               rendering_buffer );

    // rt.integrate(); // Renders the final image.
    
    scene.BuildBVH();

    PathTracer pt( camera,
                  scene,
                  background_color,
                  rendering_buffer,
                  100);

    pt.integrate();

    // Save the rendered image to a .ppm file.
    rendering_buffer.save( "output_image.ppm" );

    return EXIT_SUCCESS;
}

