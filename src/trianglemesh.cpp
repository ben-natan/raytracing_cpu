#include "trianglemesh.hpp"
#include "triangle.hpp"


bool TriangleMesh::intersect(Ray* ray, float& distance, int& meshIndex) const
{
    bool isect = false;
    for (int i=0; i<_numTris; i++) {
        vec3 v0 = _P[_trisIndex[3*i]];
        vec3 v1 = _P[_trisIndex[3*i+1]];
        vec3 v2 = _P[_trisIndex[3*i+2]];
        float t = 100000;
        Triangle triangle(v0,v1,v2);
        if (triangle.intersect(ray, t, meshIndex) && t < distance) {
            meshIndex = i;
            distance = t;
            isect |= true;
        }
    }
    return isect;
};

void TriangleMesh::getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const {
    vec3 tri_uv;
    vec3 v0 = _P[_trisIndex[meshIndex*3]];
    vec3 v1 = _P[_trisIndex[meshIndex*3 + 1]];
    vec3 v2 = _P[_trisIndex[meshIndex*3 + 2]];
    Triangle triangle(v0,v1,v2);
    triangle.getSurfaceProperties(ray, distance, meshIndex, pHit, normal, tri_uv);
    float u = tri_uv.x();
    float v = tri_uv.y();
    vec3 st0 = _texCoordinates[_texIndex[meshIndex * 3]];
    vec3 st1 = _texCoordinates[_texIndex[meshIndex * 3 + 1]];
    vec3 st2 = _texCoordinates[_texIndex[meshIndex * 3 + 2]];
    hitTextureCoords = (1 - u - v) * st0 + u * st1 + v * st2;
    vec3 n0 = _N[_norIndex[meshIndex * 3]];
    vec3 n1 = _N[_norIndex[meshIndex * 3 + 1]];
    vec3 n2 = _N[_norIndex[meshIndex * 3 + 2]];
    normal = ((1 - u - v) * n0 + u * n1 + v * n2).normalize();
}