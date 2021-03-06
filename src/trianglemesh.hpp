#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "vec3.hpp"
#include "object.hpp"
#include <vector>
#include <fstream>
#include <sstream>

class Ray;

class TriangleMesh : public Object {
    protected:
        int _numTris;
        std::vector<vec3> _P;
        std::vector<int> _trisIndex;
        std::vector<vec3> _N;
        std::vector<int> _norIndex;
        std::vector<vec3> _texCoordinates; //utiliser les deux premieres
        std::vector<int> _texIndex;

    public:
        TriangleMesh(int numTris, std::vector<vec3> P, std::vector<int> trisIndex, std::vector<vec3> N, std::vector<int> norIndex,
                     std::vector<vec3> texCoordinates, std::vector<int> texIndex): Object(0.80, false, vec3(0,0,255)),
        _numTris(numTris), _P(P), _trisIndex(trisIndex), _N(N), _norIndex(norIndex), _texCoordinates(texCoordinates), _texIndex(texIndex) {}


        // Transforme un polygon mesh en triangle mesh
        TriangleMesh(int nfaces, std::vector<int> faceIndex, 
                     std::vector<int> vertsIndex, std::vector<vec3> verts,
                     std::vector<int> norIndex, std::vector<vec3> N, 
                     std::vector<int> texIndex, std::vector<vec3> texCoordinates) : Object(0.8,false,vec3(20,0,20)), _numTris(0) 
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

                _P = std::vector<vec3>(maxVertIndex); // de la taille maxVertIndex = le nombre de points
                _N = std::vector<vec3>(maxVertIndex);
                _texCoordinates = std::vector<vec3>(maxVertIndex);
                for (int i = 0; i < maxVertIndex; i++) {
                    _P[i] = verts[i];
                    _N[i] = N[i];
                    _texCoordinates[i] = texCoordinates[i];
                }

                _trisIndex = std::vector<int>(3*_numTris);
                _norIndex = std::vector<int>(3*_numTris);
                _texIndex = std::vector<int>(3*_numTris);
                
                int l = 0;

                for (int i = 0, k = 0; i < nfaces; i++) {
                    for (int j = 0; j < faceIndex[i] - 2; j++) {
                        _trisIndex[l] = vertsIndex[k];
                        _trisIndex[l + 1] = vertsIndex[k + j + 1];
                        _trisIndex[l + 2] = vertsIndex[k + j + 2];

                        _norIndex[l] = norIndex[k];
                        _norIndex[l + 1] = norIndex[k + j + 1];
                        _norIndex[l + 2] = norIndex[k + j + 2];

                        _texIndex[l] = texIndex[k]; 
                        _texIndex[l + 1] = texIndex[k + j + 1]; 
                        _texIndex[l + 2] = texIndex[k + j + 2]; 
                        l+=3;
                    }
                    k+=faceIndex[i];
                }

            }

        TriangleMesh(const char* filename): Object(0.80,false,vec3(20,0,20)), _numTris(0) {
            std::ifstream in(filename, std::ios::in);
            if (!in) {
                std::cerr << "Failed to open file" << std::endl;
            }

            std::string line;
            while (std::getline(in, line)) {
                if (line.substr(0,2) == "v ") {
                    std::istringstream v(line.substr(2));
                    vec3 vert;
                    double x,y,z;
                    v>>x;v>>y;v>>z;
                    vert = vec3(x,y,z);
                    _P.push_back(vert);
                }
                else if (line.substr(0,2) == "vt") {
                    std::istringstream v(line.substr(3));
                    vec3 tex;
                    double U,V;
                    v >> U; v >> V;
                    tex = vec3(U,V,0);
                    _texCoordinates.push_back(tex);
                }
                else if (line.substr(0,2) == "vn") {
                    std::istringstream v(line.substr(3));
                    vec3 nor;
                    double x,y,z;
                    v >> x; v>> y; v>>z;
                    nor = vec3(x,y,z);
                    _N.push_back(nor);
                }
                else if (line.substr(0,2)=="f ") {
                    _numTris += 1;
                    int a,b,c; //mesh index
                    int p,q,r; //texture index
                    int x,y,z; //norm index
                    const char* chh=line.c_str();
                    sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &a, &p, &x, &b, &q, &y, &c, &r, &z);
                    a--;b--;c--;
                    p--;q--;r--;
                    x--;y--;z--;
                    _trisIndex.push_back(a);
                    _trisIndex.push_back(b);
                    _trisIndex.push_back(c);
                    _texIndex.push_back(p);
                    _texIndex.push_back(q);
                    _texIndex.push_back(r);
                    _norIndex.push_back(x);
                    _norIndex.push_back(y);
                    _norIndex.push_back(z);
                }
            }
        }

        bool intersect(Ray* ray, float& distance, int& meshIndex) const override;
        void getSurfaceProperties(Ray *ray, float distance, int meshIndex, vec3& pHit, vec3& normal, vec3& hitTextureCoords) const override;

        void moveBack(float n) override {
            for (auto& point: _P) {
                point += vec3(0,0,-n);
            }
        }

        static TriangleMesh *generatePolySphere(vec3 center, float rad, int divs) {
            // generate points                                                                                                                                                                                      
            uint32_t numVertices = (divs - 1) * divs + 2; 
            std::vector<vec3> P(numVertices);
            std::vector<vec3> N(numVertices);
            std::vector<vec3> texCoordinates(numVertices);
            float u = -M_PI_2;
            float v = -M_PI;
            float du = M_PI/divs;
            float dv = 2 * M_PI/divs;
            P[0] = vec3(0, -rad, 0) + center; 
            N[0] = vec3(0, -rad, 0);
            uint32_t k = 1; 
            for (uint32_t i = 0; i < divs - 1; i++) { 
                u += du; 
                v = -M_PI; 
                for (uint32_t j = 0; j < divs; j++) { 
                    float x = rad * cos(u) * cos(v); 
                    float y = rad * sin(u); 
                    float z = rad * cos(u) * sin(v) ; 
                    P[k] = vec3(x, y, z) + center; 
                    N[k] = vec3(x,y,z);
                    texCoordinates[k].setx(u / M_PI + 0.5);
                    texCoordinates[k].sety(v * 0.5 / M_PI + 0.5); 
                    v += dv, k++; 
                } 
            } 
            P[k] = vec3(0, rad, 0) + center; 
            N[k] = vec3(0, rad, 0);
        
            uint32_t npolys = divs * divs; 

            std::vector<int> faceIndex(npolys);
            std::vector<int> vertsIndex((6+(divs-1)*4)*divs);
            std::vector<int> norIndex((6+(divs-1)*4)*divs);
            std::vector<int> texIndex((6+(divs-1)*4)*divs);
        
            // create the connectivity lists                                                                                                                                                                        
            uint32_t vid = 1, numV = 0, l = 0; 
            k = 0; 
            for (uint32_t i = 0; i < divs; i++) { 
                for (uint32_t j = 0; j < divs; j++) { 
                    if (i == 0) { 
                        faceIndex[k++] = 3; 
                        vertsIndex[l] = 0; 
                        vertsIndex[l + 1] = j + vid; 
                        vertsIndex[l + 2] = (j == (divs - 1)) ? vid : j + vid + 1; 

                        norIndex[l] = 0; 
                        norIndex[l + 1] = j + vid; 
                        norIndex[l + 2] = (j == (divs - 1)) ? vid : j + vid + 1; 

                        texIndex[l] = 0; 
                        texIndex[l + 1] = j + vid; 
                        texIndex[l + 2] = (j == (divs - 1)) ? vid : j + vid + 1; 

                        l += 3; 
                    } 
                    else if (i == (divs - 1)) { 
                        faceIndex[k++] = 3; 
                        vertsIndex[l] = j + vid + 1 - divs; 
                        vertsIndex[l + 1] = vid + 1; 
                        vertsIndex[l + 2] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 

                        norIndex[l] = j + vid + 1 - divs; 
                        norIndex[l + 1] = vid + 1; 
                        norIndex[l + 2] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 

                        texIndex[l] = j + vid + 1 - divs; 
                        texIndex[l + 1] = vid + 1; 
                        texIndex[l + 2] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 

                        l += 3; 
                    } 
                    else { 
                        faceIndex[k++] = 4; 
                        vertsIndex[l] = j + vid + 1 - divs; 
                        vertsIndex[l + 1] = j + vid + 1; 
                        vertsIndex[l + 2] = (j == (divs - 1)) ? vid + 1 : j + vid + 2; 
                        vertsIndex[l + 3] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 

                        norIndex[l] = j + vid + 1 - divs; 
                        norIndex[l + 1] = j + vid + 1; 
                        norIndex[l + 2] = (j == (divs - 1)) ? vid + 1 : j + vid + 2; 
                        norIndex[l + 3] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 

                        texIndex[l] = j + vid + 1 - divs; 
                        texIndex[l + 1] = j + vid + 1; 
                        texIndex[l + 2] = (j == (divs - 1)) ? vid + 1 : j + vid + 2; 
                        texIndex[l + 3] = (j == (divs - 1)) ? vid + 1 - divs : j + vid + 2 - divs; 
                        
                        l += 4; 
                    } 
                    numV++; 
                } 
                vid = numV; 
            } 
        
            return new TriangleMesh(npolys, faceIndex, vertsIndex, P, norIndex, N, texIndex, texCoordinates);
        }
};


#endif