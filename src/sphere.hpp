#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "iostream"


class Sphere : public Object {
    private:
        vec3 _center;
        float _radius;

    public:
        Sphere() : Object(0.80,0.20,0.50), _center(vec3(0,0,0)), _radius(2) {};
        Sphere(vec3 center, float radius) : Object(0.80,0.20,0.50), _center(center), _radius(radius) {};
        Sphere(float radius): Object(0.80,0.20,0.50), _center(vec3(0,0,0)), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color): Object(1.0, 0.0, 0.50, 10, 1.5, color), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color, float k_transparent): Object(1.0 - k_transparent, k_transparent, 0.50, 3, 1.5, color), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, int spec_n, float ior, vec3 color): Object(0.80, 0.20, 0.50, spec_n, ior, color), _center(center), _radius(radius) {};

        ~Sphere() {
            std::cout << "Deeee" << std::endl;
        }

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

        bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const override;

        bool intersectShadow(Ray ray, float& distance) const override;

    
};

#endif