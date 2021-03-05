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
        std::vector<int> _texIndex;
        std::vector<vec3> _st;
        std::vector<int> _norIndex;
        std::vector<vec3> _normals;

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

        void loadObjModel(const char *filename) {
            // Initialiser l'objet
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
                    _verts.push_back(vert);
                }
                else if (line.substr(0,2) == "vt") {
                    std::istringstream v(line.substr(3));
                    vec3 tex;
                    int U,V;
                    v >> U; v >> V;
                    tex = vec3(U,V,0);
                    _st.push_back(tex);
                }
                else if (line.substr(0,2) == "vn") {
                    std::istringstream v(line.substr(3));
                    vec3 nor;
                    double x,y,z;
                    v >> x; v>> y; v>>z;
                    nor = vec3(x,y,z);
                    _normals.push_back(nor);
                }
                else if (line.substr(0,2)=="f ") {
                    int a,b,c; //mesh index
                    int A,B,C; //texture index
                    int x,y,z; //norm index
                    const char* chh=line.c_str();
                    sscanf(chh, "f -%i/-%i/-%i/-1 -%i/-%i/-%i/-1 -%i/-%i/-%i/-1", &a, &A, &x, &b, &B, &y, &c, &C, &z);
                    a--;b--;c--;
                    A--;B--;C--;
                    x--;y--;z--;
                    _faceIndex.push_back(a);
                    _faceIndex.push_back(b);
                    _faceIndex.push_back(c);
                    _texIndex.push_back(A);
                    _texIndex.push_back(B);
                    _texIndex.push_back(C);
                    _norIndex.push_back(x);
                    _norIndex.push_back(y);
                    _norIndex.push_back(z);
                }


            }
        } 



};