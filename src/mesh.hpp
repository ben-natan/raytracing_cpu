#include <fstream>
#include <sstream>
#include <vector> 
#include <cassert>
#include <iostream>
#include <vector>
#include "vec3.hpp"

class PolygonMesh {
    protected:
        int _nfaces;
        std::vector<int> _faceIndex;
        int _vertsIndexArraySize;
        std::vector<int> _vertsIndex;
        int _vertsArraySize; 
        std::vector<vec3> _verts;
        // std::vector<vec3> _normals; POUR PLUS TARD
        // std::vector<vec3> _st;

    public:

        PolygonMesh() {}

        void loadGeoFile(const char* file) {
            std::ifstream ifs;
            ifs.open(file);
            std::stringstream ss;
            ss << ifs.rdbuf();
            ss >> _nfaces;
            std::cout << "Mesh has " << _nfaces << " faces." << std::endl;

            int tmp; // Lui donner un nom;

            // Reading face index array
            for (int i = 0; i < _nfaces; i++) {
                ss >> tmp;
                _faceIndex.push_back(tmp);
                _vertsIndexArraySize += tmp;
            }
            std::cout << "Verts index array size: " << _vertsIndexArraySize << std::endl;

            int vertsArraySize = 0;
            // Reading vertex index array
            for (int i = 0; i < _vertsIndexArraySize; i++) {
                ss >> tmp;
                _vertsIndex.push_back(tmp);
                if (_vertsIndex[i] > vertsArraySize) vertsArraySize = _vertsIndex[i];
            }
            vertsArraySize += 1;
            _vertsArraySize = vertsArraySize;
            std::cout << "Max verts index " << _vertsArraySize << std::endl;

            // Reading vertices
            _verts.resize(_vertsArraySize);

            for (int i = 0; i < _vertsArraySize; i++) {
                ss >> tmp;
                _verts[i].setx(tmp);
                ss >> tmp;
                _verts[i].sety(tmp);
                ss >> tmp;
                _verts[i].setz(tmp);
            }

            std::cout << "* Done loading vertices" << std::endl;
            

            // Reading normals & st coordinates
            std::cout << "* Done loading normals" << std::endl;
            std::cout << "* Done loading textures" << std::endl;


            ifs.close();
            std::cout << "-* Mesh loaded *-" << std::endl;
            std::cout << "" << std::endl;
        }

        int nfaces() {
            return _nfaces;
        }

        std::vector<int> faceIndex() {
            return _faceIndex;
        }

        std::vector<int> vertsIndex() {
            return _vertsIndex;
        }
        
        std::vector<vec3> verts() {
            return _verts;
        }



};