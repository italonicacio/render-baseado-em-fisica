#include "triangle.h"

#define NTEST_CULLING

Triangle::Triangle()
{}
        
Triangle::Triangle( const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2) :
            vertices_{ vertex_0, vertex_1, vertex_2 }
{}

Triangle::Triangle( const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2,
                    const glm::vec3 &color) :
            vertices_{ vertex_0, vertex_1, vertex_2 }
{ color_ = color; }

Triangle::Triangle( const glm::vec3 &vertex_0, 
                    const glm::vec3 &vertex_1, 
                    const glm::vec3 &vertex_2,
                    const glm::vec3 &normal_0, 
                    const glm::vec3 &normal_1, 
                    const glm::vec3 &normal_2,
                    const glm::vec3 &color,
                    const Material material):          
            vertices_{ vertex_0, vertex_1, vertex_2 },
            normal_{ normal_0, normal_1, normal_2 }
{ color_ = color; this->material_ = material;}



bool Triangle::intersect (  const Ray &ray, 
                            IntersectionRecord &intersection_record) const
{
    
    
    glm::vec3 edge_1 = vertices_[1] - vertices_[0];
    glm::vec3 edge_2 = vertices_[2] - vertices_[0];

    glm::vec3 p_vec = glm::cross(ray.direction_, edge_2);

    float det = glm::dot(edge_1, p_vec);
    
#ifdef TEST_CULLING
    if(det < this->this->kIntersectionTestEpsilon_ ) 
        return false;
#else
    if( (det > -this->kIntersectionTestEpsilon_) && (det < this->kIntersectionTestEpsilon_) )
        return false;
#endif

    float inv_det= 1.0f/det;

    glm::vec3 t_vec = ray.origin_ - vertices_[0];
    float u = glm::dot(t_vec, p_vec)* inv_det;

    if(u < 0.0f || u > 1.0f)
        return false;

    glm::vec3 q_vec = glm::cross(t_vec, edge_1);
    float v = glm::dot(ray.direction_, q_vec) * inv_det;

     if(v < 0.0f || (u + v) > 1.0f)
        return false;
    
    intersection_record.t_ = glm::dot(edge_2, q_vec) * inv_det;
    intersection_record.position_ = ray.origin_ + intersection_record.t_*ray.direction_;
    intersection_record.normal_ = (u*this->normal_[0]) + (v*this->normal_[1]) + ((1.0f-u-v)*this->normal_[2]); // glm::normalize(glm::cross(edge_1, edge_2));
    intersection_record.color_ = color_;
    // intersection_record.material = this->material_;
    intersection_record.material.brdf_ = this->material_.brdf_;
    intersection_record.material.emission_ = this->material_.emission_;
    intersection_record.material.type_ = this->material_.type_;
    
    return true;
}


AABB Triangle::GetAABB() const
{
    AABB aabb;

    aabb.min_ = glm::min( glm::min( this->vertices_[0], this->vertices_[1] ), this->vertices_[2]);
    aabb.max_ = glm::max( glm::max( this->vertices_[0], this->vertices_[1] ), this->vertices_[2]);
    aabb.centroid_ = (1.0f/3.0f) * (this->vertices_[0] + this->vertices_[1] + this->vertices_[2]);

    return aabb;
} 