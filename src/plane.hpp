#include <iostream>
#include <cmath>
#include "object.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#ifndef PLANE_H
#define PLANE_H


class Plane: public Object {
    private:
        vec3 _position; //p0
        vec3 _normal; //normalisé

    public:
        Plane(): Object(0.8,false,0.18), _position(vec3(0,-1,0)), _normal(vec3(0,1,0)) {};
        Plane(vec3 pos): Object(0.8,false,0.18), _position(pos), _normal(vec3(0,1,0)) {};
        Plane(vec3 pos, vec3 nor): Object(0.8,false,0.18), _position(pos), _normal(nor.normalize()) {};
        Plane(vec3 pos, vec3 nor, vec3 color): Object(0.8,false,0.18), _position(pos), _normal(nor.normalize()) {};

        ~Plane() {
            
        }

        bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;
        bool intersectShadow(Ray ray, float& distance) const override;
        void moveBack(float n) override {
            return;
        }

};


#endif