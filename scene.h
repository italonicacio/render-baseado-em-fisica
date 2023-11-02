#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "primitive.h"
#include "sphere.h" 
#include "triangle.h"
#include "bvh.h"


class Scene
{
public:

    Scene( void );

    ~Scene( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record ) const;

    bool intersect_bvh( const Ray &ray,
                    IntersectionRecord &intersection_record ) const;

    void BuildBVH();

    void load( void );
    void LoadMesh(std::string &file_name);
    void LoadMesh(std::string &file_name, Material &material);
    
    BVH *bvh_ = nullptr;

    std::vector< Primitive::PrimitiveUniquePtr > primitives_;

};

#endif //SCENE_H

