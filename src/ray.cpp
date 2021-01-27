#include "ray.hpp"



void Ray::Shoot(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights,
                    int n_obj, int n_lig )
{
    if (_depth < 0) {
            return;
    }
    int min_obj_ind;
    float distance;
    float min_distance = INFINITY;
    vec3 pHit, normal, color;
    for (int i = 0; i < n_obj; i++) {
        if (objects[i]->intersect(this, &distance, pHit, normal, color)) {
            if (distance < min_distance) {
                    min_distance = distance;
                    min_obj_ind = i;
            }
        }
    }

    for (auto& light: lights) {

    }

    if (min_distance != INFINITY ) {
        if (objects[min_obj_ind]->isMirror()) {
            auto mirrorRay = new Ray();   // Ici, bien définir le nouveau ray
            mirrorRay->Shoot(objects, lights, n_obj, n_obj);
            this->addColor(objects[min_obj_ind]->k_mirror() * mirrorRay->color());
        }
        if (objects[min_obj_ind]->isTransparent()) {
            auto transparentRay = new Ray();   // Ici, bien définir le nouveau ray
            transparentRay->Shoot(objects, lights, n_obj, n_obj);
            this->addColor(objects[min_obj_ind]->k_transparent() * transparentRay->color());
        }  
    }
}