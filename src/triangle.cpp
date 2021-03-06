#include "triangle.hpp"

bool Triangle::intersect(Ray* ray, float& distance, int& meshIndex) const {
    vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = _v1 - _v0;
    edge2 = _v2 - _v0;
    h = ray->direction().crossProduct(edge2);
    a = edge1.dot(h);
    if (a > -ray->epsilon() && a < ray->epsilon())
        return false; // rayon parallèle
    f = 1.0/a;
    s = ray->origin() - _v0;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) 
        return false;
    q = s.crossProduct(edge1);
    v = f * ray->direction().dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    float t = f * edge2.dot(q);
    if (t > ray->epsilon())
    {
        distance = t;
        return true;
    }
    else
        return false;
}

bool Triangle::intersectInMesh(Ray* ray, float& distance, vec3& uv) const {
    vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = _v1 - _v0;
    edge2 = _v2 - _v0;
    h = ray->direction().crossProduct(edge2);
    a = edge1.dot(h);
    if (a > -ray->epsilon() && a < ray->epsilon())
        return false; // rayon parallèle
    f = 1.0/a;
    s = ray->origin() - _v0;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0) 
        return false;
    q = s.crossProduct(edge1);
    v = f * ray->direction().dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    float t = f * edge2.dot(q);
    if (t > ray->epsilon())
    {
        distance = t;
        uv.setx(u);
        uv.sety(v);
        return true;
    }
    else
        return false;

}

void Triangle::getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const {
    float a,f,u,v;
    vec3 h,s,q;
    vec3 edge1 = _v1 - _v0;
    vec3 edge2 = _v2 - _v0;
    pHit = ray->origin() + distance * ray->direction();
    normal = edge1.crossProduct(edge2).normalize();
    h = ray->direction().crossProduct(edge2);
    a = edge1.dot(h);
    f = 1.0/a;
    s = ray->origin() - _v0;
    u = f * s.dot(h);
    q = s.crossProduct(edge1);
    v = f * ray->direction().dot(q);
    hitTextureCoords.setx(u);
    hitTextureCoords.sety(v);
}