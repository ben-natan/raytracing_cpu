#include "sphere.hpp"


bool Sphere::intersect(Ray* ray, float& distance, int& meshIndex) const
{
    float t0, t1;
    float a = ray->direction().dot(ray->direction());
    vec3 L = ray->origin() - _center;
    float b = 2*ray->direction().dot(L);
    float c = L.dot(L) - _radius * _radius;
    if (!Tools::solveQuadratic(a,b,c, t0, t1)) {
        return false;
    }
    // Ici on a au moins une intersection et t1 >= t0
    if (t0 > ray->epsilon()) {
        distance = t0;
    } else {
        if (t1 < 0) {
            // Les deux intersections sont negatives donc derriere
            return false;
        }
        distance = t1;
    }
    return true;
}

void Sphere::getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const {
    pHit = ray->origin() + distance * ray->direction();
    normal = (pHit - _center).normalize();
    hitTextureCoords.setx((1 + atan2(normal.z(), normal.x()) / M_PI) * 0.5) ;
    hitTextureCoords.sety(acosf(normal.y())/M_PI);
}