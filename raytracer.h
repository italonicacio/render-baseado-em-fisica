#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <sstream>
#include <iomanip>
#include <cmath>

#include "camera.h"
#include "scene.h"
#include "buffer.h"

class RayTracer
{
    public:

        RayTracer( Camera &camera,
                const Scene &scene,
                const glm::vec3 background_color,
                Buffer &buffer );

        void integrate( void );

    private:

        const Camera &camera_;

        const Scene &scene_;

        glm::dvec3 background_color_;

        Buffer &buffer_;

};

#endif //RAYTRACER_H

