#include "ray.hpp"


void Ray::Shoot(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights,
                    int n_obj, int n_lig )
{
    if (_depth < 0) {
            return;
    }
    this->_depth -= 1;
    int min_obj_ind;
    float distance;
    float min_distance = INFINITY;
    vec3 pHit, normal, color;
    vec3 min_pHit, min_normal;
    for (int i = 0; i < n_obj; i++) {
        if (objects[i]->intersect(this, distance, pHit, normal, color)) {
            if (distance < min_distance) {
                    min_distance = distance;
                    min_obj_ind = i;
                    min_pHit = pHit;
                    min_normal = normal;
            }
        }
    }

    if (min_distance != INFINITY ) {
        for (auto& light: lights) {
            vec3 L = light->pos() - pHit;
            float distanceToLight = L.norm();
            float currDist;
            L.normalize();
            for ( auto& object: objects ) {
                Ray shadowRay(pHit, L, this->color());      // Un autre type que les rays serait plus judicieux?
                if (object->intersectShadow(shadowRay, currDist)) {
                    if (currDist < distanceToLight) {
                        break; // sort de la boucle sur les objets
                    }
                }
            }
            // Là, au moins un shadow ray a touché la lumière
            this->addColor(objects[min_obj_ind]->albedo() / M_PI * light->intensity() * std::max(0.0f, (float)normal.dot(L)) * light->color());
        }

        // if (objects[min_obj_ind]->isMirror()) {
        //     auto mirrorRay = new Ray();   // Ici, bien définir le nouveau ray
        //     mirrorRay->Shoot(objects, lights, n_obj, n_obj);
        //     this->addColor(objects[min_obj_ind]->k_mirror() * mirrorRay->color());
        // }
        // if (objects[min_obj_ind]->isTransparent()) {
        //     auto transparentRay = new Ray();   // Ici, bien définir le nouveau ray
        //     transparentRay->Shoot(objects, lights, n_obj, n_obj);
        //     this->addColor(objects[min_obj_ind]->k_transparent() * transparentRay->color());
        // }  
    }
}