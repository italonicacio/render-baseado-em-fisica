#ifndef BVH_H
#define BVH_H

#include <sstream>
#include <iostream>
#include <iomanip>
#include <deque>
#include <queue>
#include <algorithm>

#include "primitive.h"
#include "aabb.h"


struct BVHNode
{
    ~BVHNode()
    {
        if( this->left )
        {
            delete this->left;
            this->left = nullptr;
        }

        if( this->right )
        {
            delete this->right;
            this->right = nullptr;
        }
    }

    std::size_t first_;
    std::size_t last_;
    AABB aabb_;
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;

};

struct PrimitiveAABBArea
{
    std::size_t primitve_ID_;
    glm::vec3 centroid_;
    AABB aabb_;
    float left_area_;
    float right_area_;

    AABB left_aabb_;
    AABB right_aabb_;

};

struct Comparator
{
    static bool SortInx(const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs)
    { return lhs.centroid_.x < rhs.centroid_.x; }

    static bool SortIny(const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs)
    { return lhs.centroid_.y < rhs.centroid_.y; }

    static bool SortInz(const PrimitiveAABBArea &lhs, const PrimitiveAABBArea &rhs)
    { return lhs.centroid_.z < rhs.centroid_.z; }


};

class BVH
{
    public:
        BVH(const std::vector< Primitive::PrimitiveUniquePtr> &primitives);
        ~BVH();

        bool Intersect(const Ray &ray, IntersectionRecord &intersection_record) const;
     
    private:
        float SAH(  std::size_t s1_size,
                    float s1_area,
                    std::size_t s2_size,
                    float s2_area,
                    float s_area);

        void SplitNode( BVHNode **node, 
                        std::deque< PrimitiveAABBArea > &s,
                        std::size_t first,
                        std::size_t last,
                        float s_area);

        bool Traverse(  const BVHNode *node,
                        const Ray &ray,
                        IntersectionRecord &intersect_record) const;
        
        BVHNode *root_node = nullptr;
    
        float const_intersect_tri_ = 0.8f;
        float const_intersec_aabb_ = 0.2f;

        std::deque< unsigned long int > primitve_ID_;
        
        const std::vector< Primitive::PrimitiveUniquePtr > &primitives_;
        std::size_t primitives_inserted_ = 0;



};

#endif // BVH_H