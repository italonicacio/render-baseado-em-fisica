#ifndef PINHOLE_CAMERA_H
#define PINHOLE_CAMERA_H

#include "camera.h"
#include "ray.h"

class PinholeCamera : public Camera
{
public:

    PinholeCamera( void );

    PinholeCamera( const float min_x,
                        const float max_x,
                        const float min_y,
                        const float max_y,
                        const float focal_distance,
                        const glm::ivec2 &resolution,
                        const glm::vec3 &position,
                        const glm::vec3 &up_vector,
                        const glm::vec3 &look_at );

    Ray getWorldSpaceRay( const glm::vec2 &pixel_coord ) const;
    

    float min_x_;

    float max_x_;

    float min_y_;

    float max_y_;

    float focal_distance_;

};

#endif //PINHOLE_CAMERA_H

