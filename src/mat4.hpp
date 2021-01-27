#ifndef MAT4_H 
#define MAT4_H

#include <vector>

class mat4 {
    private:
        std::vector<float> _mat; // remplis par ligne par ligne
    public:
        mat4(): _mat(std::vector<float>(16, 0)) {}
        mat4(const float val): _mat(std::vector<float>(16,val)) {}

};



#endif