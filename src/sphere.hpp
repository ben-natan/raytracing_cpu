#include "vec3.hpp"
#include "ray.hpp"
#include "object.hpp"

#ifndef SPHERE_H
#define SPHERE_H


class Sphere : public Object {
    private:
        vec3 _center;
        float _radius;

    public:
        ~Sphere() {
            std::cout << "ra" << std::endl;
        }

        Sphere() : _center(vec3(0,0,0)), _radius(2) {};
        Sphere(vec3 center, float radius) : _center(center), _radius(radius) {};
        Sphere(float radius): _center(vec3(0,0,0)), _radius(radius) {};

        // Mettre ici la fonction pour intersecter
        bool intersect(const std::unique_ptr<Ray>& ray, float* distance, vec3& pHit, vec3& normal, vec3& color) const override {
            // a = 1 , b = 2D(O-C) c = |O-C|^2 - R^2
            float t0, t1;
            float a = ray->direction().dot(ray->direction());
            vec3 L = ray->origin() - _center;
            float b = 2*ray->direction().dot(L);
            float c = L.dot(L) - _radius * _radius;  // PERF: stocker radius au carré
            if (!math::solveQuadratic(a,b,c, t0, t1)) {
                return false;
            }
            // Ici on a au moins une intersection et t1 >= t0
            if (t0 > 0) {
                *distance = t0;
            } else {
                if (t1 < 0) {
                    // Les deux intersections sont negatives donc derriere
                    return false;
                }
                *distance = t1;
            }
            return true;
        };

        void Shoot() {};

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
    
};

#endif