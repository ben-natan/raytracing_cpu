#include "sphere.hpp"


bool Sphere::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const
{
// a = 1 , b = 2D(O-C) c = |O-C|^2 - R^2
    float t0, t1;
    float a = ray->direction().dot(ray->direction());
    vec3 L = ray->origin() - _center;
    float b = 2*ray->direction().dot(L);
    float c = L.dot(L) - _radius * _radius;  // PERF: stocker radius au carré
    if (!Tools::solveQuadratic(a,b,c, t0, t1)) {
        return false;
    }
    // Ici on a au moins une intersection et t1 >= t0
    if (t0 > 0.001) {
        distance = t0;
    } else {
        if (t1 < 0) {
            // Les deux intersections sont negatives donc derriere
            return false;
        }
        distance = t1;
    }
    // std::cout << "                &dans intersect:" << 
    // "Ray Origin: " << ray->origin() << " Distance: " << distance << std::endl;
    pHit = ray->origin() + distance * ray->direction();
    normal = (pHit - _center).normalize();
    return true;
}

bool Sphere::intersectShadow(Ray ray, float& distance) const
{
    float t0, t1;
    float a = ray.direction().dot(ray.direction());
    vec3 L = ray.origin() - _center;
    float b = 2*ray.direction().dot(L);
    float c = L.dot(L) - _radius * _radius;
    if (!Tools::solveQuadratic(a,b,c,t0,t1)) {
        return false;
    }

    if (t0 > 0.001) { //pour eviter l'acne
        distance = t0;
    } else {
        if (t1 < 0) {
            return false;
        }
        distance = t1;
    }
    return true;
}
