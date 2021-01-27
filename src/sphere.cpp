#include "sphere.hpp"


bool Sphere::intersect(Ray* ray, float* distance, vec3& pHit, vec3& normal, vec3& color) const
{
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
}