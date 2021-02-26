#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "vec3.hpp"
#include "ray.hpp"
#include "object.hpp"
#include <vector>


class TriangleMesh : public Object {
    protected:
        int _numTris;
        std::vector<vec3> _P;
        std::vector<int> _trisIndex;
        std::vector<vec3> _N;
        std::vector<vec3> _texCoordinates; //utiliser les deux premieres

    public:
        TriangleMesh(int numTris, std::vector<vec3> P, std::vector<int> trisIndex, std::vector<vec3> N): Object(0.80,0.20,vec3(0,0,255)),
                _numTris(numTris), _P(P), _trisIndex(trisIndex), _N(N) {}


        // Transforme un polygon mesh en triangle mesh
        TriangleMesh(int nfaces, std::vector<int> faceIndex, std::vector<int> vertsIndex,
                std::vector<vec3> verts, std::vector<vec3> st) : Object(0.80,0.20,vec3(20,0,20)), _numTris(0) 
            {
                    
                int k=0, maxVertIndex=0;

                // find out how many triangles for this mesh
                for (int i=0; i < nfaces; i++) {
                    _numTris += faceIndex[i] - 2;
                    for (int j = 0; j < faceIndex[i]; j++) {
                        if (vertsIndex[k+j] > maxVertIndex)
                            maxVertIndex = vertsIndex[k+j];
                        }
                    k += faceIndex[i];
                }
                maxVertIndex += 1;

                _P = std::vector<vec3>(maxVertIndex); // de la taille maxVertIndex
                for (int i = 0; i < maxVertIndex; i++) {
                    _P[i] = verts[i];
                }

                _trisIndex = std::vector<int>(3*_numTris);
                _texCoordinates = std::vector<vec3>(3*_numTris);
                int l = 0;

                for (int i = 0, k = 0; i < nfaces; i++) {
                    for (int j = 0; j < faceIndex[i] - 2; j++) {
                        _trisIndex[l] = vertsIndex[k];
                        _trisIndex[l+1] = vertsIndex[k + j + 1];
                        _trisIndex[l+2] = vertsIndex[k + j + 2];
                        _texCoordinates[l] = st[k]; 
                        _texCoordinates[l + 1] = st[k + j + 1]; 
                        _texCoordinates[l + 2] = st[k + j + 2]; 
                        l+=3;
                    }
                    k+=faceIndex[i];
                }

            }

        bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        bool intersectShadow(Ray ray, float& distance) const override;

        vec3 colorFromTexture(vec3 hitTextureCoords) const override {
            // Ici on fait un damier
            int M = 10;
            float checker = (fmod(hitTextureCoords.x() * M, 1.0) > 0.5) ^ (fmod(hitTextureCoords.y() * M, 1.0) < 0.5);
            return checker ? vec3(0,0,0) : vec3(255,255,255);
        }
};


#endif