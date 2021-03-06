#include "vec3.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "iostream"

#ifndef SPHERE_H
#define SPHERE_H


class Sphere : public Object {
    private:
        vec3 _center;
        float _radius;

    public:
        Sphere() : Object(0.80,false,0.50), _center(vec3(0,0,0)), _radius(2) {};
        Sphere(vec3 center, float radius) : Object(0.80,false,0.50), _center(center), _radius(radius) {};
        Sphere(float radius): Object(0.80,false,0.50), _center(vec3(0,0,0)), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color): Object(0.3, false, 0.50, 10, 1.5, color), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color, float k_mirror): Object(k_mirror, false, 0.18, 3, 1.5, color), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, vec3 color, float k_mirror, bool transparent): Object(k_mirror, transparent, 0.18, 3, 1.5, color), _center(center), _radius(radius) {};
        Sphere(vec3 center, float radius, int spec_n, float ior, vec3 color): Object(0.3, false, 0.18, spec_n, ior, color), _center(center), _radius(radius) {};

        Sphere(vec3 center, float radius, vec3 color, bool transparent, float ior): Object(0.3, transparent, 0.18, 3, ior, color), _center(center), _radius(radius) {};


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

        // vec3 colorFromTexture(vec3 hitTextureCoords) override {
        //     // Ici on fait un damier
        //     int M = 10;
        //     float checker = (fmod(hitTextureCoords.x() * M, 1.0) > 0.5) ^ (fmod(hitTextureCoords.y() * M, 1.0) < 0.5);
        //     return checker ? vec3(0,0,0) : vec3(255,255,255);
        // }
    
};

#endif