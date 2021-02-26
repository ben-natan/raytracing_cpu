#include "trianglemesh.hpp"
#include "triangle.hpp"


bool TriangleMesh::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const
{
    int j = 0;
    bool isect = false;
    for (int i=0; i<_numTris; i++) {
        vec3 v0 = _P[_trisIndex[j]];
        vec3 v1 = _P[_trisIndex[j+1]];
        vec3 v2 = _P[_trisIndex[j+2]];
        vec3 st0 = _texCoordinates[_trisIndex[j]];
        vec3 st1 = _texCoordinates[_trisIndex[j+1]];
        vec3 st2 = _texCoordinates[_trisIndex[j+2]];
        float t = 100000, u, v;
        Triangle triangle = Triangle(v0,v1,v2,st0,st1,st2);
        if (triangle.intersect(ray, t, pHit, normal, hitTextureCoords) && t < distance) {
            distance = t;
            isect |= true;
        }
        j+=3;
    }
    return isect;
}


bool TriangleMesh::intersectShadow(Ray ray, float& distance) const {
    int j = 0;
    bool isect = false;
    for (int i=0; i<_numTris; i++) {
        vec3 v0 = _P[_trisIndex[j]];
        vec3 v1 = _P[_trisIndex[j+1]];
        vec3 v2 = _P[_trisIndex[j+2]];
        float t = 100000, u, v;
        Triangle triangle = Triangle(v0,v1,v2);
        if (triangle.intersectShadow(ray, t) && t < distance) {
            distance = t;
            isect |= true;
        }
        j+=3;
    }
    return isect;
}
