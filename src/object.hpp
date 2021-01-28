#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.hpp"
#include "ray.hpp"
#include <iostream>

class Ray;

class Object {
    protected:
        float _k_mirror;
        float _k_transparent;
        float _albedo;
    public: 
        virtual ~Object() {
            std::cout << "ho" <<std::endl;
        }
        Object(): _k_mirror(1.0), _k_transparent(0.0), _albedo(0.18) {}
        Object(float k_mir, float k_trans, float albedo): _k_mirror(k_mir), _k_transparent(k_trans), _albedo(albedo) {}

        virtual bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const = 0;
        virtual bool intersectShadow(Ray ray, float& distance) const = 0;
        
        bool isMirror() {
            return (_k_mirror!=0.0);
        }

        bool isTransparent() {
            return (_k_transparent!=0.0);
        }

        float k_mirror() {
            return _k_mirror;
        }
        
        float k_transparent() {
            return _k_transparent;
        }

        float albedo() {
            return _albedo;
        }
};

#endif