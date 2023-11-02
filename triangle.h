#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <vector>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"

class Triangle: public Primitive
{

    public:
        Triangle();
        
        Triangle(   const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2);

        Triangle(   const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2,
                    const glm::vec3 &color);

        Triangle(   const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2,
                    const glm::vec3 &normal_0, 
                    const glm::vec3 &normal_1, 
                    const glm::vec3 &normal_2,
                    const glm::vec3 &color,
                    const Material material);            
      

        bool intersect (const Ray &ray, 
                        IntersectionRecord &intersection_record) const;

        glm::vec3 vertices_[3];
        glm::vec3 normal_[3];
        glm::vec3 vertices_colors_[3];

        AABB GetAABB() const;

    private:
        float kIntersectionTestEpsilon_ = 0.000001f;

        
};



#endif //TRIANGLE_H