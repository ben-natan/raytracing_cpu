#include "trianglemesh.hpp"
#include "triangle.hpp"


bool TriangleMesh::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const
{
    bool isect = false;
    int min_tri = -1;
    vec3 uv;
    float u,v;
    for (int i=0; i<_numTris; i++) {
        vec3 v0 = _P[_trisIndex[3*i]];
        vec3 v1 = _P[_trisIndex[3*i+1]];
        vec3 v2 = _P[_trisIndex[3*i+2]];
        float t = 100000;
        Triangle triangle(v0,v1,v2);
        if (triangle.intersectInMesh(ray, t, uv) && t < distance) {
            min_tri = i;
            distance = t;
            isect |= true;
        }
    }
    if (isect) {
        pHit = ray->origin() + distance * ray->direction();
        u = uv.x();
        v = uv.y();
        vec3 st0 = _texCoordinates[_texIndex[min_tri * 3]];
        vec3 st1 = _texCoordinates[_texIndex[min_tri * 3 + 1]];
        vec3 st2 = _texCoordinates[_texIndex[min_tri * 3 + 2]];
        hitTextureCoords = (1 - u - v) * st0 + u * st1 + v * st2;
        vec3 n0 = _N[_norIndex[min_tri * 3]];
        vec3 n1 = _N[_norIndex[min_tri * 3 + 1]];
        vec3 n2 = _N[_norIndex[min_tri * 3 + 2]];
        normal = ((1 - u - v) * n0 + u * n1 + v * n2).normalize();
    }
    return isect;
};


bool TriangleMesh::intersectShadow(Ray ray, float& distance) const {

    bool isect = false;
    for (int i=0; i<_numTris; i++) {
        vec3 v0 = _P[_trisIndex[3*i]];
        vec3 v1 = _P[_trisIndex[3*i+1]];
        vec3 v2 = _P[_trisIndex[3*i+2]];
        float t = 100000, u, v;
        Triangle triangle(v0,v1,v2);
        if ((triangle.intersectShadow(ray, t)) && t < distance) {
            distance = t;
            isect |= true;
        }
    }
    return isect;
}