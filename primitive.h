#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>

#include "ray.h"
#include "intersection_record.h"
#include "material.h"
#include "aabb.h"

class Primitive
{
    public:

        glm::vec3 color_ = {1.0f, 1.0f, 1.0f};

        typedef std::unique_ptr< Primitive > PrimitiveUniquePtr;

        Material material_;

        Primitive( void );

        virtual ~Primitive( void );

        virtual bool intersect( const Ray &ray,
                                IntersectionRecord &intersection_record ) const = 0;

        virtual AABB GetAABB() const = 0;

};

#endif //PRIMITIVE_H

