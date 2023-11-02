#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <sstream>
#include <iomanip>
#include <cmath>

#include "camera.h"
#include "scene.h"
#include "buffer.h"

class PathTracer
{
    public:

        PathTracer( Camera &camera,
                const Scene &scene,
                const glm::vec3 background_color,
                Buffer &buffer,
                unsigned int num_sample);

        void integrate();
        glm::vec3 L(const Ray &ray, int current_depth);

    private:

        const Camera &camera_;

        const Scene &scene_;

        glm::dvec3 background_color_;

        Buffer &buffer_;

        unsigned int num_sample;

        int max_depth = 5;

};

#endif //PATHTRACER_H

