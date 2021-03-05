#include "plane.hpp"

bool Plane::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const
{
    float denominator = (ray->direction()).dot(_normal);
    if (abs(denominator) > 0.0001) {
        vec3 diff = _position - ray->origin(); // Régler pb de const
        distance = (1/(float)denominator) *diff.dot(_normal);
        pHit = ray->origin() + distance*ray->direction();
        normal = _normal;
        // hitTextureCoords à déterminer
        vec3 diskR = pHit - _position;
        if (diskR.dot(diskR) < 400) {
            return (distance >= 0); 
        }
    }
    return false;
}

bool Plane::intersectShadow(Ray ray, float& distance) const {
    float denominator = (ray.direction()).dot(_normal);
    if (abs(denominator) > 0.0001) {
        vec3 diff = _position - ray.origin(); // Régler le problème de const
        distance = (1/(float)denominator) * diff.dot(_normal);
        vec3 pHit = ray.origin() + distance * ray.direction();
        vec3 diskR = pHit - _position;
        if (diskR.dot(diskR) < 400)
            return (distance >= 0);
    }
    return false;
}