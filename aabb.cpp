#include "aabb.h"

AABB::AABB() { }

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) :
            min_{ min },
            max_{ max },
            centroid_{ 0.5f * (min_ + max_) }
{ }

AABB::~AABB() { }

float AABB::GetArea()
{
    float a = this->max_.x - this->min_.x;
    float b = this->max_.y - this->min_.y;
    float c = this->max_.z - this->min_.z;

    return 2.0f * ( a * (b + c) + b * c );

}

 bool AABB::intersect( const Ray &ray) const
{

    float tmin;
    float tmax;
    float tymin;
    float tymax;
    float tzmin;
    float tzmax;
    float divx;
    float divy;
    float divz;

    divx = 1.0f/ray.direction_.x;
    if( divx >= 0.0f )
    {
        tmin = (this->min_.x - ray.origin_.x) * divx;
        tmax = (this->max_.x - ray.origin_.x) * divx;
    }else {
        tmin = (this->max_.x - ray.origin_.x) * divx;
        tmax = (this->min_.x - ray.origin_.x) * divx;
    }

    divy = 1.0f/ray.direction_.y;
    if( divy >= 0.0f )
    {
        tymin = (this->min_.y - ray.origin_.y) * divy;
        tymax = (this->max_.y - ray.origin_.y) * divy;
    }else{
        tymin = (this->max_.y - ray.origin_.y) * divy;
        tymax = (this->min_.y - ray.origin_.y) * divy;
    }

    if( tmin > tymax || tymin > tmax )
        return false;

    if( tymax < tmax )
        tmax = tymax;

    divz = 1.0f/ray.direction_.z;
    if(divz >= 0.0f)
    {
        tzmin = (this->min_.z - ray.origin_.z) * divz;
        tzmax = (this->max_.z - ray.origin_.z) * divz;
    }else{
        tzmin = (this->max_.z - ray.origin_.z) * divz;
        tzmax = (this->min_.z - ray.origin_.z) * divz;
    }

    if( tmin > tzmax || tzmin > tmax)
        return false;

    if( tzmin > tmin )
        tmin = tzmin;
    
    if( tzmax < tmax )
        tmax = tzmax;
    
    return true;
    
}