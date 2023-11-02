#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#define LIGHT 0
#define DIFFUSE 1
#define SPECULATE 2
#define COOK_TORRANCE 3

class Material
{
    public:

        Material();
        Material(glm::vec3 brdf, glm::vec3 emission, int type);

        glm::vec3 brdf_;

        glm::vec3 emission_; 

        int type_;

};

#endif // MATERIAL_H