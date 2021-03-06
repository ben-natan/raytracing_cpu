#include "plane.hpp"

bool Plane::intersect(Ray* ray, float& distance, int& meshIndex) const
{
    float denominator = (ray->direction()).dot(_normal);
    if (abs(denominator) > 0.0001) {
        vec3 diff = _position - ray->origin();
        distance = (1/(float)denominator) *diff.dot(_normal);
        vec3 pHit = ray->origin() + distance*ray->direction();
        vec3 diskR = pHit - _position;
        if (diskR.dot(diskR) < 800) {
            return (distance >= 0); 
        }
    }
    return false;
}

void Plane::getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const {
    pHit = ray->origin() + distance * ray->direction();
    normal = _normal;
    // hitTextureCoords
}