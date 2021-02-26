#include "object.hpp"
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
        Triangle(): _v0(vec3(0,0,0)), _v1(vec3(0,0,0)), _v2(vec3(0,0,0)) {}
        Triangle(vec3 v0, vec3 v1, vec3 v2): _v0(v0), _v1(v1), _v2(v2) {}
        Triangle(vec3 v0, vec3 v1, vec3 v2, vec3 st0, vec3 st1, vec3 st2): _v0(v0), _v1(v1), _v2(v2),
                                                                           _st0(st0), _st1(st1), _st2(st2) {}


        bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        bool intersectShadow(Ray ray, float& distance) const override;


};