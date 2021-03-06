#include <iostream>
#include <cmath>
#include "object.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#ifndef PLANE_H
#define PLANE_H

class Texture;

class Plane: public Object {
    private:
        vec3 _position; //p0
        vec3 _normal; //normalisé

    public:

        Plane(vec3 pos, vec3 nor, vec3 color, bool transparent = false, float ior = 1.5): Object(0.3, transparent, color, 0.18, 3, ior), _position(pos), _normal(nor.normalize()) {};
        Plane(vec3 pos, vec3 nor, Texture* texture, bool transparent = false, float ior = 1.5): Object(0.3, transparent, texture, 0.18, 3, ior), _position(pos), _normal(nor.normalize()) {};

        bool intersect(Ray* ray, float& distance, int& meshIndex) const override;
        void getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;
        
        void moveBack(float n) override {
            return;
        }

};


#endif