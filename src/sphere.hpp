#include "vec3.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "iostream"

#ifndef SPHERE_H
#define SPHERE_H

class Texture;

class Sphere : public Object {
    private:
        vec3 _center;
        float _radius;

    public:

        Sphere(vec3 center, float radius, vec3 color, bool transparent = false, float ior = 1.5): Object(0.3, transparent, color, 0.18, 3, ior), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, Texture* texture, bool transparent = false, float ior = 1.5): Object(0.3, transparent, texture, 0.18, ior), _center(center), _radius(radius) {};

        vec3 center() {
            return _center;
        }

        float radius() {
            return _radius;
        }

        void setCenter(vec3 center) {
            _center = center;
        }

        void setRadius(float radius) {
            _radius = radius;
        }

        bool intersect(Ray* ray, float& distance, int& meshIndex) const override;
        void getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        void moveBack(float n) override {
            _center+=vec3(0,0,-n);
        }
    
};

#endif