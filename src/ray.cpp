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
    vec3 pHit, normal, hitTextureCoords;
    vec3 min_pHit, min_normal, min_hitTextureCoords;
    for (int i = 0; i < n_obj; i++) {
        distance = INFINITY;
        if (objects[i]->intersect(this, distance, pHit, normal, hitTextureCoords)) {
            if (distance < min_distance) {
                    min_distance = distance;
                    min_obj_ind = i;
                    min_pHit = pHit;
                    min_normal = normal;
                    min_hitTextureCoords = hitTextureCoords;
            }
        }
    }
    if (min_distance != INFINITY) {
        // Ambient Lighting
        float ambientLevel = 0.05;  // EN FAIRE UN GLOBAL 
        this->addColor(ambientLevel * objects[min_obj_ind]->color());

        // Shadow rays <Lambertian + Specular Lightings>
        for (auto& light: lights) {
            vec3 bias = 0.001 * min_normal;
            vec3 shadowOrigin = min_pHit + bias;
            vec3 L = light->pos() - shadowOrigin;
            float distanceToLight = L.norm();
            float currDist;
            int vis = 1;
            L.normalize();
            Ray shadowRay(shadowOrigin, L, this->color());
            for ( auto& object: objects ) {
                currDist = INFINITY;
                if (object->intersectShadow(shadowRay, currDist)) {
                    if (currDist < distanceToLight) {
                        vis = 0;
                        break; // sort de la boucle sur les objets avec vis = 0;
                    }
                }
            }

            // Lambertian 
            this->addColor(vis * objects[min_obj_ind]->albedo() / M_PI * light->intensity() * std::max(0.0f, (float)min_normal.dot(L)) * objects[min_obj_ind]->colorFromTexture(min_hitTextureCoords));
            
            // Specular
            vec3 R;
            shadowRay.computeReflectedDirection(min_normal,R);
            this->addColor(vis * objects[min_obj_ind]->Ks() * light->intensity() * std::pow(std::max(0.0f, R.dot(this->direction())), objects[min_obj_ind]->spec_n()) * light->color());
        }//(shadow & illumination)


        // Réflexion et refraction
        if (objects[min_obj_ind]->isMirror() && objects[min_obj_ind]->isTransparent()) {
            vec3 refractionColor = vec3(0,0,0);
            float kr;
            objects[min_obj_ind]->fresnel(this->direction(), min_normal, kr);
            bool outside = this->direction().dot(min_normal) < 0;
            vec3 bias = 0.0001 * min_normal;
            vec3 newOriginRef = outside ? min_pHit - bias : min_pHit + bias;
            // vec3 newColorRef = this->color();
            vec3 newColorRef = vec3(0,0,0);
            vec3 newDirectionRef;
            if (kr < 1) {
                if (objects[min_obj_ind]->refract(this->direction(), min_normal, newDirectionRef)) {
                    auto refractionRay = std::make_unique<Ray>(Ray(newOriginRef, newDirectionRef, newColorRef, this->depth() -1));
                    refractionRay->Shoot(objects, lights, n_obj, n_lig);
                    refractionColor = refractionRay->color();
                }
            }
            vec3 newOrigin = outside ? min_pHit + bias : min_pHit - bias;
            // vec3 newColor = this->color();
            vec3 newColor = vec3(0,0,0);
            vec3 newDirection;
            this->computeReflectedDirection(min_normal, newDirection);
            auto mirrorRay = std::make_unique<Ray>(Ray(newOrigin, newDirection, newColor, this->depth() -1));
            mirrorRay->Shoot(objects, lights, n_obj, n_lig);
            
            // Loi de fresnel
            this->addColor(kr * mirrorRay->color() + (1-kr) * refractionColor);
        }//(reflexion + refraction)

        // Réflexion seulement
        else if (objects[min_obj_ind]->isMirror()) {
            vec3 bias = 0.001 * min_normal;
            bool outside = this->direction().dot(min_normal) < 0;
            vec3 newOrigin = outside ? min_pHit + bias : min_pHit - bias;
            vec3 newColor = this->color();
            vec3 newDirection;
            this->computeReflectedDirection(min_normal, newDirection);
            auto mirrorRay = std::make_unique<Ray>(Ray(newOrigin, newDirection, newColor, this->depth() -1));
            mirrorRay->Shoot(objects, lights, n_obj, n_lig);
            this->addColor(objects[min_obj_ind]->k_mirror() * mirrorRay->color());
        }//(reflexion)

        // else if (->isTransparent()) ??
    } else {
        if (_depth == 4) {
            auto t = 0.5*this->direction().y() + 1.0;
            this->addColor((1.0-t)*vec3(255,255,255) + t*vec3(128,179,255)); // Background color?
        } else {
            this->addColor(vec3(0,0,0));
        }
        
    }
}

void Ray::computeReflectedDirection(vec3 normal, vec3& direction)
{
    direction = this->direction() - (2*normal.dot(this->direction())) * normal;
}