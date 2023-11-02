#ifndef INTERSECTION_RECORD_H
#define INTERSECTION_RECORD_H

#include <glm/glm.hpp>
#include "material.h"

struct IntersectionRecord
{
    float t_;
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec3 color_;
    int primitive_id_;
    Material material;

};

#endif //INTERSECTION_RECORD_H_

