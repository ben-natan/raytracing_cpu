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
        Sphere() : Object(1.0,0.0,0.50), _center(vec3(0,0,0)), _radius(2) {};
        Sphere(vec3 center, float radius) : Object(1.0,0.0,0.50), _center(center), _radius(radius) {};
        Sphere(float radius): Object(1.0,0.0,0.50), _center(vec3(0,0,0)), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color): Object(1.0, 0.0, 0.50, color), _center(center), _radius(radius) {};

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