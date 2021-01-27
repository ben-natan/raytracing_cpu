#ifndef LIGHT_H
#define LIGHT_H
#include "vec3.hpp"



class Light {
    private:
        vec3 _center;
        vec3 _color;

    public:
        Light(): _center(vec3(10.0,10.0,-2.0)), 
                 _color(vec3(255.0,255.0,255.0)) {}

        ~Light() {
            std::cout << "Light" << std::endl;
        }
};

#endif