#include <iostream>
#include <cmath>
#include "object.hpp"
#include "vec3.hpp"

#ifndef PLANE_H
#define PLANE_H


class Plane: public Object {
    private:
        vec3 _position; //p0
        vec3 _normal; //normalisé

    public:
        Plane(): Object(1.0,0.0,0.18), _position(vec3(0,-1,0)), _normal(vec3(0,1,0)) {};
        Plane(vec3 pos): Object(1.0,0.0,0.18), _position(pos), _normal(vec3(0,1,0)) {};
        Plane(vec3 pos, vec3 nor): Object(1.0,0.0,0.18), _position(pos), _normal(nor.normalize()) {};

        ~Plane() {
            std::cout << "Plane" << std::endl;
        }

        bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const override;

        bool intersectShadow(Ray ray, float& distance) const override;
};


#endif