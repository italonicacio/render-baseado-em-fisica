#include "sphere.h"

Sphere::Sphere( void )
{}

Sphere::Sphere( const glm::vec3 &center,
                float radius ) :
        center_{ center },
        radius_{ radius }
{}

Sphere::Sphere( const glm::vec3 &center,
                float radius,
                const glm::vec3 &color) :
        center_{ center },
        radius_{ radius }
{ this->color_ = color; }

Sphere::Sphere( const glm::vec3 &center,
                float radius,
                const glm::vec3 &color,
                const Material &material) :
        center_{ center },
        radius_{ radius }
{ this->color_ = color; this->material_ = material; }

bool Sphere::intersect( const Ray &ray,
                        IntersectionRecord &intersection_record ) const
{
    /* Ray-sphere intersection test adapted from the very efficient algorithm presented in the article:
     *
     *     "Intersection of a Ray with a Sphere".
     *     Jeff Hultquist.
     *     Graphics Gems.
     *     Academic Press.
     *     1990.
     */

    float t1;
    float t2;

    glm::vec3 eo = center_ - ray.origin_;
    float v = glm::dot( eo, ray.direction_ );
    float disc = ( radius_ * radius_ ) - ( glm::dot( eo, eo ) - ( v * v ) );

    if ( disc < 0.0f )
        return false;                           // no intersection
    else
    {
        float d = sqrt( disc );
        t1 = v - d;                             // first intersection point
        t2 = v + d;                             // second intersection point
    }

    // Set the intersection record
    intersection_record.t_ =  ( t1 > 0.00001f ) ? t1 : t2;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    intersection_record.normal_ = glm::normalize( intersection_record.position_ - center_ );
    intersection_record.color_ = color_;
    intersection_record.material.brdf_ = this->material_.brdf_;
    intersection_record.material.emission_ = this->material_.emission_;
    intersection_record.material.type_ = this->material_.type_;


    return true;
}


AABB Sphere::GetAABB() const
{
    AABB aabb;

    aabb.min_= this->center_ - this->radius_;
    aabb.max_ = this->center_ + this->radius_;
    aabb.centroid_ = this->center_;
    return aabb;
}
