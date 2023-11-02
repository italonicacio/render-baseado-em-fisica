#include "pathtracer.h"
#include <iostream>

#include <random>
#include <functional>
#include <chrono>

float PI = (float)M_PI; 

std::uniform_real_distribution<float> dist(0.0f, 1.0f);
auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 rng(seed);

Ray GetNewRay(const IntersectionRecord &intersection_record );
Ray GetReflectedRay(const Ray &ray, IntersectionRecord &intersection_record);

PathTracer::PathTracer( Camera &camera,
                const Scene &scene,
                const glm::vec3 background_color,
                Buffer &buffer,
                unsigned int num_sample) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        buffer_( buffer ),
        num_sample { num_sample }
{}

void PathTracer::integrate()
{

#pragma omp parallel for schedule(dynamic, 1) 

    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
    {
        
        std::stringstream progress_stream;
        progress_stream << "\r  progress .........................: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0 * y / ( buffer_.v_resolution_ - 1 )
                        << "%";

        std::clog << progress_stream.str();

        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
            

            for(std::size_t sample = 1; sample < this->num_sample; sample ++)
            {
                float u = dist(rng);
                float v = dist(rng);

                u = x+u;
                v = y+v;
                Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ u, v } ) };

            
                buffer_.buffer_data_[x][y] += L(ray, 0);
            }

            buffer_.buffer_data_[x][y] /= this->num_sample;
                
            
        }
    }

    std::clog << std::endl;
}

glm::vec3 PathTracer::L(const Ray &ray, int current_depth)
{
    IntersectionRecord intersection_record;
    intersection_record.t_ = std::numeric_limits< double >::max();

    Ray reflect_ray;
    glm::vec3 Lo{0.0f, 0.0f, 0.0f};
    float dot;

    if(current_depth < this->max_depth)
    {
        if(scene_.intersect_bvh(ray, intersection_record))
        {
            switch (intersection_record.material.type_)
            {
                case LIGHT:
                    Lo = intersection_record.material.emission_;
                    break;

                case DIFFUSE:

                    reflect_ray = GetNewRay(intersection_record);

                    dot = glm::dot(intersection_record.normal_, reflect_ray.direction_);
                    if(dot < 0.0f)
                    {
                        reflect_ray.direction_ *= -1.0f;
                        dot *= -1.0f;
                    }

                    Lo = intersection_record.material.emission_ + 2.0f * intersection_record.material.brdf_ * L(reflect_ray, current_depth+1 ) * dot;

                    break;

                case SPECULATE:

                    reflect_ray = GetReflectedRay(ray, intersection_record);
                    Lo = intersection_record.material.brdf_*L(reflect_ray, ++current_depth) ;

                    break;
                
                case COOK_TORRANCE:
                    break;
                    
            
            }

        }

    }
    
    return Lo;
}

Ray GetNewRay(const IntersectionRecord &intersection_record )
{
    float r1 = dist(rng);
    float r2 = dist(rng);
    

    float theta = acos(1 - r1);
    float phi = 2 * ((float)M_PI) * r2;
   
    float x =  sin(theta) * cos(phi);
    float y =  sin(theta) * sin(phi);
    float z =  cos(theta);

    glm::vec3 dir(x, y, z);
    
    
    dir = glm::normalize(dir);

    Ray new_ray(intersection_record.position_, dir);    

    return new_ray;
}

Ray GetReflectedRay(const Ray &ray, IntersectionRecord &intersection_record)
{
    ONB onb_;
    Ray refl_ray;

    onb_.setFromV(intersection_record.normal_);    

    glm::vec3 dir = glm::transpose(onb_.getBasisMatrix()) * ray.direction_;
    dir = { dir.x, -dir.y, dir.z };

    refl_ray = { intersection_record.position_ + (intersection_record.normal_*0.001f), onb_.getBasisMatrix() * dir };
    return refl_ray;
}