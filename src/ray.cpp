#include "ray.hpp"

#define debug false

void Ray::Shoot(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights,
                    int n_obj, int n_lig )
{   
    #if debug
    std::cout << " ------ Depth: " << _depth << std::endl;
    #endif
    if (_depth < 0) {
            return; //this->addColor(background)
    }
    int min_obj_ind;
    float distance;
    int count = 0; // test
    float min_distance = INFINITY;
    vec3 pHit, normal, color;
    vec3 min_pHit, min_normal;
    for (int i = 0; i < n_obj; i++) {
        if (objects[i]->intersect(this, distance, pHit, normal, color)) {
            count+=1;
            if (distance < min_distance) {
                    min_distance = distance;
                    min_obj_ind = i;
                    min_pHit = pHit;
                    min_normal = normal;
            }
            #if debug
            std::cout << " ** Intersecté: " << objects[i]->color() << "en " << pHit<< std::endl;
            std::cout << " ** Distance: " << distance << std::endl;
            #endif
        }
    }
    #if debug
    // std::cout << "COUNT " << count << std::endl;
    std::cout << "##### Min distance: " << min_distance << " pour pHit: " << min_pHit <<std::endl;
    #endif
    if (min_distance != INFINITY ) {
        // Ambient Lighting
        float ambientLevel = 0.05;
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
            Ray shadowRay(shadowOrigin, L, this->color());      // Un autre type que les rays serait plus judicieux?
            for ( auto& object: objects ) {
                if (object->intersectShadow(shadowRay, currDist)) {
                    if (currDist < distanceToLight) {
                        vis = 0;
                        break; // sort de la boucle sur les objets avec vis = 0;
                    }
                }
            }

            // Lambertian 
            // this->addColor(vis * objects[min_obj_ind]->albedo() / M_PI * light->intensity() * std::max(0.0f, (float)min_normal.dot(L)) * light->color());                // scratchapixel.com & http://graphics.stanford.edu/courses/cs148-10-summer/as3/instructions/as3.pdf
            this->addColor(vis * objects[min_obj_ind]->albedo() / M_PI * light->intensity() * std::max(0.0f, (float)min_normal.dot(L)) * objects[min_obj_ind]->color());

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
            #if debug
            std::cout << "KR: " << kr << std::endl;
            #endif
            this->addColor(kr * mirrorRay->color() + (1-kr) * refractionColor);
        }//(reflexion + refraction)

        // Réflexion seulement
        else if (objects[min_obj_ind]->isMirror()) {
            vec3 bias = 0.001 * min_normal;
            vec3 newOrigin = min_pHit + bias;
            vec3 newColor = this->color();
            vec3 newDirection;
            this->computeReflectedDirection(min_normal, newDirection);
            auto mirrorRay = std::make_unique<Ray>(Ray(newOrigin, newDirection, newColor, this->depth() -1));
            mirrorRay->Shoot(objects, lights, n_obj, n_lig);
            this->addColor(objects[min_obj_ind]->k_mirror() * mirrorRay->color());
        }//(reflexion)
    } else {
        this->addColor(vec3(0,0,0));
    }
}

void Ray::computeReflectedDirection(vec3 normal, vec3& direction)
{
    direction = this->direction() - (2*normal.dot(this->direction())) * normal;
}