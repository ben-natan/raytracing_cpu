#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.hpp"
#include "ray.hpp"
#include "vec3.hpp"


class Triangle: public Object {
    protected:
        vec3 _v0; 
        vec3 _v1;
        vec3 _v2;


    public:
        Triangle(): Object(), _v0(vec3(0,0,0)), _v1(vec3(0,0,0)), _v2(vec3(0,0,0)) {}
        Triangle(vec3 v0, vec3 v1, vec3 v2): Object(), _v0(v0), _v1(v1), _v2(v2) {}

        bool intersect(Ray* ray, float& distance, int& meshIndex) const override;
        void getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        void moveBack(float n) override {
            _v0+=vec3(0,0,-n);
            _v1+=vec3(0,0,-n);
            _v2+=vec3(0,0,-n);
        }


};

#endif