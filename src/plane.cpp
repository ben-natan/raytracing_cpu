#include "plane.hpp"
#include <algorithm>

bool Plane::intersect(Ray* ray, float& distance, int& meshIndex) const
{
    float denominator = (ray->direction()).dot(_normal);
    if (abs(denominator) > ray->epsilon()) {
        vec3 diff = _position - ray->origin();
        distance = (1/(float)denominator) *diff.dot(_normal);
        vec3 pHit = ray->origin() + distance*ray->direction();
        vec3 diskR = pHit - _position;
        return (distance >= 0); 
    }
    return false;
}

void Plane::getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const {
    pHit = ray->origin() + distance * ray->direction();
    normal = _normal;
    vec3 u = vec3(_normal.y(), - _normal.x(), 0).normalize();
    vec3 v = u.crossProduct(_normal);
    float hitX = u.dot(pHit);
    float hitY = v.dot(pHit);
    hitX > 0 ? hitTextureCoords.setx(hitX) : hitTextureCoords.setx(-hitX);
    hitY > 0 ? hitTextureCoords.sety(hitY) : hitTextureCoords.sety(-hitY);
}