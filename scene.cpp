#include "scene.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"



#include <fstream>

Scene::Scene( void )
{}

Scene::~Scene( void )
{
    if( this->bvh_ )
    {
        delete this->bvh_;
        this->bvh_ = nullptr;
    }

}

bool Scene::intersect( const Ray &ray,
                       IntersectionRecord &intersection_record ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = primitives_.size();
    
    // Loops over the list of primitives, testing the intersection of each primitive against the given ray 
    for ( std::size_t primitive_id = 0; primitive_id < num_primitives; primitive_id++ )
        if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            
            if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0f ) )
            {
                
                intersection_record = tmp_intersection_record;
                intersection_record.primitive_id_ = primitive_id;
                intersection_result = true; // the ray intersects a primitive!
                
            }

    
    return intersection_result;
}

bool Scene::intersect_bvh( const Ray &ray,
                    IntersectionRecord &intersection_record ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;

    intersection_result = bvh_->Intersect(ray, intersection_record);

    return intersection_result;
}

void Scene::BuildBVH()
{
    this->bvh_ = new BVH( this->primitives_);
    std::clog << std::endl;
}


void Scene::load( void ) 
{
    Material light( glm::vec3(0.0f, 0.0f, 0.0f) ,  glm::vec3(13.0f, 13.0f, 13.0f), LIGHT);
    // Material material1( glm::vec3(0.5f, 0.3f, 0.7f)  , glm::vec3(0.0f, 0.0f, 0.0f) , DIFFUSE);
    // Material material2( glm::vec3(0.3f, 0.7f, 0.5f)  , glm::vec3(0.0f, 0.0f, 0.0f) , DIFFUSE);

    // Material material3( glm::vec3(0.1f, 0.2f, 0.0f)  , glm::vec3(0.0f, 0.0f, 0.0f), DIFFUSE);
    // Material material4( glm::vec3(0.5f, 0.3f, 0.0f) , glm::vec3(0.0f, 0.0f, 0.0f), DIFFUSE);

    // Material material5( glm::vec3(1.0f, 1.0f, 1.0f) , glm::vec3(0.0f, 0.0f, 0.0f), SPECULATE);

    // Material material6( glm::vec3(0.9f, 0.3f, 0.0f) , glm::vec3(0.0f, 0.0f, 0.0f), DIFFUSE);

    

    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, -5.0f,  3.0f }, 0.6f, glm::vec3{0.3f, 0.8f, 0.3f}, light } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, -5.0f,  -3.0f }, 0.6f, glm::vec3{0.3f, 0.8f, 0.3f}, light } ) );

    // // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.f, 0.0f,  0.0f }, 0.4f, glm::vec3{0.3f, 0.8f, 0.3f},  material2} ) );

    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  -0.5f, 0.5f,  0.0f }, 0.2f, glm::vec3{0.3f, 0.8f, 0.3f},  material6} ) );
    
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  -0.5f, 0.0f,  0.0f }, 0.2f, glm::vec3{0.3f, 0.8f, 0.3f},  material2} ) );
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.5f, 0.5f,  0.0f }, 0.4f, glm::vec3{0.3f, 0.8f, 0.3f},  material5} ) );

    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, -20.0f,  0.0f }, 10.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material1 } ) );
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f,  20.0f,  0.0f }, 10.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material1 } ) );

    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.0f, -20.0f }, 10.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material4 } ) );
    // // //primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.0f,  10.0f }, 9.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material4 } ) );

    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  20.0f, 0.0f,  0.0f }, 10.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material3 } ) );
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  -20.0f, 0.0f,  0.0f }, 10.0f, glm::vec3{0.3f, 0.8f, 0.3f}, material3 } ) );
   
    


    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.0f,  0.0f }, 0.2f, glm::vec3{0.3f, 0.8f, 0.3f} } ) );
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.5f, 0.0f, -1.0f }, 0.5f, glm::vec3{0.2f, 0.3f, 0.5f} } ) );
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f,-0.5f, -0.4f }, 0.5f, glm::vec3{0.8f, 0.3f, 0.3f} } ) );
    // //primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.5f, -3.0f }, 0.2f, glm::vec3{0.8f, 0.3f, 0.0f} } ) );
    
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  0.0f, 0.0f,  0.5f }, //vertex 0
    //                                                                     glm::vec3{ -0.5f, 0.0f, -1.0f }, //vertex 1
    //                                                                     glm::vec3{  0.0f,-0.5f, -0.5f }, //vertex 2
    //                                                                     glm::vec3{  1.0f, 0.3f,  0.0f } } ) ); //color
    
   
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  0.0f, 0.0f,  0.0f }, //vertex 0
    //                                                                     glm::vec3{  1.0f, 0.0f,  0.0f }, //vertex 1
    //                                                                     glm::vec3{  1.0f,-0.5f,  0.0f }, //vertex 2
    //                                                                     glm::vec3{  0.6f, 0.6f,  0.0f } } ) ); //color
    
    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  0.0f, -0.5f,  0.0f }, //vertex 0
    //                                                                     glm::vec3{  -0.5f,-0.5f,  0.0f }, //vertex 1
    //                                                                     glm::vec3{  -0.5f, 0.0f,  0.0f }, //vertex 2
    //                                                                     glm::vec3{  0.5f, 0.6f,  0.5f } } ) ); //color

    // primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  0.0f, 0.5f,  0.1f }, //vertex 0
    //                                                                     glm::vec3{  0.0f, 0.3f,  0.3f }, //vertex 1
    //                                                                     glm::vec3{ -0.3f, 0.0f,  0.6f }, //vertex 2
    //                                                                     glm::vec3{  0.7f, 0.9f,  0.5f } } ) ); //color
    
    //std::string model = "models_test/cube/cube.obj";

    //std::string model2 = "models_test/box/box.obj";
    //std::string model = "models_test/suz/suz.obj";
    // std::string model = "models_test/suz/suz_scale.obj";

    // std::string model_final = "models/escritorio/escritorio_pc.obj";

    Material material_mesa( glm::vec3(0.5f, 0.5f, 0.8f), glm::vec3(0.0f), DIFFUSE);
    Material material_box( glm::vec3(0.5f, 0.5f, 0.6f), glm::vec3(0.0f), DIFFUSE);
    Material material_cadeira( glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f), DIFFUSE);
    Material material_monitor( glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f), SPECULATE);
    Material material_tela( glm::vec3(0.3f, 0.3f, 0.3f) , glm::vec3(0.0f), SPECULATE);
    Material material_caderno( glm::vec3(1.0f, 1.0f, 1.0f) , glm::vec3(0.0f), DIFFUSE);
    Material material_estatua( glm::vec3(1.0f, 0.7f, 0.5f) , glm::vec3(0.0f), DIFFUSE);
    Material material_computador( glm::vec3(0.1f, 0.3f, 0.1f) , glm::vec3(0.0f), DIFFUSE);
    Material material_teclado( glm::vec3(0.5f, 0.3f, 0.4f) , glm::vec3(0.0f), DIFFUSE);
    Material material_mouse( glm::vec3(0.5f, 0.5f, 0.5f) , glm::vec3(0.0f), DIFFUSE);
    Material material_mousepad( glm::vec3(0.9f, 0.0f, 0.0f) , glm::vec3(0.0f), DIFFUSE);



    std::string mesa = "models/escritorio/mesa.obj";
    std::string box = "models/escritorio/box2.obj";
    std::string cadeira = "models/escritorio/cadeira.obj";
    std::string monitor = "models/escritorio/monitor.obj";
    // std::string tela = "models/escritorio/tela.obj";
    std::string caderno = "models/escritorio/caderno.obj";
    std::string estatua = "models/escritorio/estatua.obj";
    std::string computador = "models/escritorio/computador.obj";
    std::string teclado = "models/escritorio/teclado.obj";
    std::string mouse = "models/escritorio/mouse.obj";
    std::string mousepad = "models/escritorio/mousepad.obj";
    
    LoadMesh(mesa, material_mesa);
    LoadMesh(box, material_box);
    LoadMesh(cadeira, material_cadeira);
    LoadMesh(monitor, material_monitor);
    
    LoadMesh(caderno, material_caderno);
    LoadMesh(estatua, material_estatua);
    LoadMesh(computador, material_computador);
    LoadMesh(teclado, material_teclado);
    LoadMesh(mouse, material_mouse);
    LoadMesh(mousepad, material_mousepad);



    // LoadMesh(tela, material_tela);


    

    //LoadMesh(model2, material1);
    
}



void Scene::LoadMesh(std::string &file_name)
{

    std::ifstream fin(file_name.c_str() );

    if(!fin.fail())
    {
        fin.close();
    } else {
        std::cerr << "Couldn't open file: " << file_name << std::endl;
        return;
    }
    
    Assimp::Importer assimp_importer;
    const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
    
    if(!assimp_scene_) 
    {
        std::cerr << assimp_importer.GetErrorString() << std::endl;
        return;
    }

    if(assimp_scene_->HasMeshes()) {

        for(unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++)
        {
            
            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];
            
            for(uint32_t vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3) 
            {
                const aiVector3D *vertex_ptr = &(mesh_ptr->mVertices[vertex_id]);

                glm::vec3 v0(vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z);
                glm::vec3 v1(vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z);
                glm::vec3 v2(vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z);
                
                primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ v0, //vertex 0
                                                                                    v1, //vertex 1
                                                                                    v2, //vertex 2
                                                                                    glm::vec3{  1.0f, 0.0f,  0.0f } } ) ); //color             
                
            }
       
        }
    }
    
}


void Scene::LoadMesh(std::string &file_name, Material &material)
{
    std::ifstream fin(file_name.c_str() );

    if(!fin.fail())
    {
        fin.close();
    } else {
        std::cerr << "Couldn't open file: " << file_name << std::endl;
        return;
    }
    
    Assimp::Importer assimp_importer;
    const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
    
    if(!assimp_scene_) 
    {
        std::cerr << assimp_importer.GetErrorString() << std::endl;
        return;
    }

    if(assimp_scene_->HasMeshes()) {

        for(unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++)
        {
            
            const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];
            
            for(uint32_t vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3) 
            {
                const aiVector3D *vertex_ptr = &(mesh_ptr->mVertices[vertex_id]);

                glm::vec3 v0(vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z);
                glm::vec3 v1(vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z);
                glm::vec3 v2(vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z);

                const aiVector3D* normal_ptr = &mesh_ptr->mNormals[vertex_id];
                glm::vec3 n1(normal_ptr[0].x, normal_ptr[0].y, normal_ptr[0].z);
                glm::vec3 n2(normal_ptr[1].x, normal_ptr[1].y, normal_ptr[1].z);
                glm::vec3 n3(normal_ptr[2].x, normal_ptr[2].y, normal_ptr[2].z);
                
                primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ v0, v1, v2, n1, n2, n3,
                                                                                    glm::vec3{  1.0f, 0.0f,  0.0f }, //color             
                                                                                    material } ) ); //Material
                
            }
       
        }
    }

}



