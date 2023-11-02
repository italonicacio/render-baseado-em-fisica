#include "bvh.h"

BVH::BVH(const std::vector< Primitive::PrimitiveUniquePtr> &primitives) : primitives_{ primitives }
{
    if(this->primitives_.size() > 0)
    {
        std::deque< PrimitiveAABBArea > s(primitives_.size());
        this->primitve_ID_.resize( this->primitives_.size() );

        AABB root_aabb;

        for(std::size_t i = 0; i < primitives_.size(); i++)
        {
            AABB aabb = this->primitives_[i]->GetAABB();

            if(!i)
                root_aabb = aabb;
            else
                root_aabb = root_aabb + aabb;

            s[i].primitve_ID_ = i;
            s[i].centroid_ = aabb.centroid_;
            s[i].aabb_ = aabb;

        }

        SplitNode( &this->root_node, s, 0, s.size() -1, root_aabb.GetArea() );

    }
}

BVH::~BVH()
{
    if( this->root_node )
    {
        delete this->root_node;
        this->root_node = nullptr;
    }
}

bool BVH::Intersect(const Ray &ray, IntersectionRecord &intersection_record) const
{
    return Traverse( this->root_node, ray, intersection_record);
}



float BVH::SAH(  std::size_t s1_size,
                    float s1_area,
                    std::size_t s2_size,
                    float s2_area,
                    float s_area)
{
    return  2.0f * this->const_intersec_aabb_ + ( ( s1_area/s_area ) * s1_size * this->const_intersect_tri_) +
            ( ( s2_area/s_area) * s2_size * this->const_intersect_tri_ );

}                    

void BVH::SplitNode( BVHNode **node, 
                std::deque< PrimitiveAABBArea > &s,
                std::size_t first,
                std::size_t last,
                float s_area)
{
    (*node) = new BVHNode();
    (*node)->first_ = first;
    (*node)->last_ = last;
    (*node)->left = nullptr;
    (*node)->right  = nullptr;

    std::deque < PrimitiveAABBArea > s_aux;

    float best_cost = this->const_intersect_tri_ * (last + 1 - first);
    int best_axis = -1;
    int best_event = -1;


    for(int axis = 1; axis <=3; axis ++)
    {
        switch (axis)
        {
            case 1:
                std::sort( s.begin() +first, s.begin() + last + 1, Comparator::SortInx );
                break;

            case 2:
                std::sort( s.begin() + first, s.begin() + last + 1, Comparator::SortIny );
                break;

            case 3:
                std::sort( s.begin() + first, s.begin() + last + 1, Comparator::SortInz);
                break;

        }

        s_aux = std::deque< PrimitiveAABBArea >(s.begin() + first, s.begin() + last + 1);

        for(std::size_t i = first; i <= last; i++)
        {
            if( i == first)
            {
                s[i].left_area_ = std::numeric_limits<float>::infinity();
                s_aux[0].left_aabb_ = s_aux[0].aabb_;
            }else{
                s[i].left_area_ = s_aux[ i - first - 1 ].left_aabb_.GetArea();
                s_aux[ i - first ].left_aabb_ = s_aux[ i - first ].aabb_ + s_aux[ i - first - 1 ].left_aabb_;

            }
        }

        for( long int i = last; i >= static_cast< long int >(first); i-- )
        {
            if( i == static_cast< long int >(last) )
            {
                s[i].right_area_ = std::numeric_limits<float>::infinity();
                s_aux[ last - first ].right_aabb_ = s_aux[ last - first ].aabb_;
            }else{
                s[i].right_area_ = s_aux[ i - first + 1 ].right_aabb_.GetArea();
                s_aux[ i - first ].right_aabb_ = s_aux[ i - first ].aabb_ + s_aux[ i - first + 1 ].right_aabb_;
            }

            float this_cost = SAH( i - first + 1, s[ (i + 1) % (s.size() + 1) ].left_area_, last - i, s[i].right_area_, s_area );

            if( this_cost < best_cost )
            {
                best_cost = this_cost;
                best_event = i;
                best_axis = axis;
            }
        }
    }

    if(best_axis == -1)
    {
        this->primitives_inserted_ += last - first + 1;
        std::stringstream progress_stream;
        progress_stream << "\r BVH building progress ............: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0 * static_cast< float >( this->primitives_inserted_ ) / this->primitives_.size()
                        << "%";
        std::clog << progress_stream.str();

        for( unsigned long int i = first; i <= last; i++)
        {
            this->primitve_ID_[i] = s[i].primitve_ID_;

            if( i == first )
                (*node)->aabb_ = s[i].aabb_;
            else
                (*node)->aabb_ = (*node)->aabb_ + s[i].aabb_;
        }
        
    }else{
        switch (best_axis)
        {
            case 1:
                std::sort( s.begin() + first, s.begin() + last + 1, Comparator::SortInx );
                break;
        
            case 2:
                std::sort( s.begin() + first, s.begin() + last + 1, Comparator::SortIny );
                break;
            
            case 3:
                std::sort( s.begin() + first, s.begin() + last + 1, Comparator::SortInz );
                break;
        }

        SplitNode(&(*node)->left, s, first, best_event, s_area );
        SplitNode(&(*node)->right, s, best_event + 1, last, s_area );

        (*node)->aabb_ = (*node)->left->aabb_ + (*node)->right->aabb_;    
    }
}

bool BVH::Traverse(  const BVHNode *node,
                const Ray &ray,
                IntersectionRecord &intersect_record) const
{
    bool primitive_intersect = false;

    if( node )
    {
        if( node->aabb_.intersect(ray) )
        {
            if( (!node->left) && (!node->right) )
            {
                IntersectionRecord tmp_intersection_record;

                for( std::size_t primitive_id = node->first_; primitive_id <= node->last_; primitive_id++)
                {
                    if(this->primitives_[ primitve_ID_[primitive_id]]->intersect(ray, tmp_intersection_record) )
                    {
                        if ((tmp_intersection_record.t_ < intersect_record.t_) && (tmp_intersection_record.t_ > 0.0))
                        {
                            intersect_record = tmp_intersection_record;
                            primitive_intersect = true;
                        }
                        
                    }
                }
            }else{
                if( Traverse(node->left, ray, intersect_record) )
                    primitive_intersect = true;
        
                if( Traverse(node->right, ray, intersect_record))
                    primitive_intersect = true;
            }
        }
    }

    return primitive_intersect;
}