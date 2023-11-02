#include "material.h"

Material::Material() {}

Material::Material(glm::vec3 brdf, glm::vec3 emission, int type) : brdf_{ brdf }, emission_{ emission }, type_{ type } {}