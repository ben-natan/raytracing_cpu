#include "ray.hpp"



void Ray::Shoot(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights,
                    int n_obj, int n_lig )
{   
    if (_depth < 0) {
            return;
    }
    int min_obj_ind;
    int meshIndex;
    float distance;
    float min_distance = INFINITY;
   
    for (int i = 0; i < n_obj; i++) {
        distance = INFINITY;
        if (objects[i]->intersect(this, distance, meshIndex)) {
            if (distance < min_distance) {
                    min_distance = distance;
                    min_obj_ind = i;
            }
        }
    }
    
    if (min_distance != INFINITY) {
        vec3 pHit, normal, hitTextureCoords;
        objects[min_obj_ind]->getSurfaceProperties(this, min_distance, meshIndex, pHit, normal, hitTextureCoords);

        // Ambient Lighting
        this->addColor(_ambientLevel * objects[min_obj_ind]->colorFromTexture(hitTextureCoords));

        // Shadow rays <Lambertian + Specular Lightings>
        for (auto& light: lights) {
            bool outside = this->direction().dot(normal) < 0;
            vec3 bias =  _epsilon * pHit;
            vec3 shadowOrigin = outside ? pHit - bias : pHit + bias;
            vec3 L = light->pos() - shadowOrigin;
            float distanceToLight = L.norm();
            float currDist;
            int vis = 1;
            L.normalize();
            Ray *shadowRay = new Ray(shadowOrigin, L, this->color(), _initialDepth, _initialDepth);
            for ( auto& object: objects ) {
                currDist = INFINITY;
                if (object->intersect(shadowRay, currDist, meshIndex)) {
                    if (currDist < distanceToLight) {
                        vis = 0;
                        break; // sort de la boucle sur les objets avec vis = 0;
                    }
                }
            }
            // Lambertian 
            this->addColor(vis * objects[min_obj_ind]->albedo() / M_PI * light->intensity() * std::max(0.0f, (float)normal.dot(L)) * objects[min_obj_ind]->colorFromTexture(hitTextureCoords));
            
            // // Specular
            vec3 R;
            shadowRay->computeReflectedDirection(normal,R);
            this->addColor(vis * objects[min_obj_ind]->Ks() * light->intensity() * std::pow(std::max(0.0f, R.dot(this->direction())), objects[min_obj_ind]->spec_n()) * light->color());
        }//(shadow & illumination)


        // Réflexion et refraction
        if (objects[min_obj_ind]->isMirror() && objects[min_obj_ind]->isTransparent()) {
            vec3 refractionColor = vec3(0,0,0);
            float kr;
            objects[min_obj_ind]->fresnel(this->direction(), normal, kr);
            bool outside = this->direction().dot(normal) < 0;
            vec3 bias = _epsilon * normal;
            vec3 newOriginRef = outside ? pHit - bias : pHit + bias;
            // vec3 newColorRef = this->color();
            vec3 newColorRef = vec3(0,0,0);
            vec3 newDirectionRef;
            if (kr < 1) {
                if (objects[min_obj_ind]->refract(this->direction(), normal, newDirectionRef)) {
                    auto refractionRay = std::make_unique<Ray>(Ray(newOriginRef, newDirectionRef, newColorRef, this->depth() -1, _initialDepth));
                    refractionRay->Shoot(objects, lights, n_obj, n_lig);
                    refractionColor = refractionRay->color();
                }
            }
            vec3 newOrigin = outside ? pHit + bias : pHit - bias;
            // vec3 newColor = this->color();
            vec3 newColor = vec3(0,0,0);
            vec3 newDirection;
            this->computeReflectedDirection(normal, newDirection);
            auto mirrorRay = std::make_unique<Ray>(Ray(newOrigin, newDirection, newColor, this->depth() -1, _initialDepth));
            mirrorRay->Shoot(objects, lights, n_obj, n_lig);
            
            // Loi de fresnel
            this->addColor(kr * mirrorRay->color() + (1-kr) * refractionColor);
        }//(reflexion + refraction)

        // Réflexion seulement
        else if (objects[min_obj_ind]->isMirror()) {
            vec3 bias = _epsilon * normal;
            bool outside = this->direction().dot(normal) < 0;
            vec3 newOrigin = outside ? pHit + bias : pHit - bias;
            vec3 newColor = this->color();
            vec3 newDirection;
            this->computeReflectedDirection(normal, newDirection);
            auto mirrorRay = std::make_unique<Ray>(Ray(newOrigin, newDirection, newColor, this->depth() -1, _initialDepth));
            mirrorRay->Shoot(objects, lights, n_obj, n_lig);
            this->addColor(objects[min_obj_ind]->k_mirror() * mirrorRay->color());
        }//(reflexion)

        // else if (->isTransparent()) ??
    } else {
        if (_depth == _initialDepth) {
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