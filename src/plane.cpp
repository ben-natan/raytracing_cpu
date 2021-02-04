#include "plane.hpp"

bool Plane::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const
{
    float denominator = (ray->direction()).dot(_normal);
    if (abs(denominator) > 0.0001) {
        vec3 diff = -(ray->origin() - _position); // Régler pb de const
        distance = (1/(float)denominator) *diff.dot(_normal);
        pHit = ray->origin() + distance*ray->direction();
        normal = _normal;
        return (distance >= 0); 
    }
    return false;
}

bool Plane::intersectShadow(Ray ray, float& distance) const {
    float denominator = (ray.direction()).dot(_normal);
    if (abs(denominator) > 0.0001) {
        vec3 diff = -(ray.origin() - _position); // Régler le problème de const
        distance = (1/(float)denominator) * diff.dot(_normal);
        return (distance >= 0);
    }
    return false;
}