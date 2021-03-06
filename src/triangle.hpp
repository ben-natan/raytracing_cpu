#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.hpp"
#include "ray.hpp"
#include "vec3.hpp"


class Triangle: public Object {
    protected:
        mutable vec3 _v0; 
        mutable vec3 _v1;
        mutable vec3 _v2;
        mutable vec3 _st0;
        mutable vec3 _st1;
        mutable vec3 _st2;
        

    public:
        Triangle(): Object(), _v0(vec3(0,0,0)), _v1(vec3(0,0,0)), _v2(vec3(0,0,0)) {}
        Triangle(vec3 v0, vec3 v1, vec3 v2): Object(), _v0(v0), _v1(v1), _v2(v2) {}
        Triangle(vec3 v0, vec3 v1, vec3 v2, vec3 st0, vec3 st1, vec3 st2): Object(), _v0(v0), _v1(v1), _v2(v2),
                                                                            _st0(st0), _st1(st1), _st2(st2) {}


        bool intersect(Ray* ray, float& distance, int& meshIndex) const override;

        bool intersectInMesh(Ray* ray, float& distance, vec3& uv) const;

        void getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        void moveBack(float n) override {
            _v0+=vec3(0,0,-n);
            _v1+=vec3(0,0,-n);
            _v2+=vec3(0,0,-n);
        }


};

#endif