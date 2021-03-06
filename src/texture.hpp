#include "vec3.hpp"
#include "math.h"

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    public:
    virtual vec3 getColor(vec3 hitTextureCoords) {
        return vec3(0,0,0);
    }
};

class Unicolor: public Texture {
    private :
    vec3 _color;

    public:
    Unicolor(vec3 color): _color(color) {}

    vec3 getColor(vec3 hitTextureCoords) override {
        return _color;
    }
};

class CheckersTexture : public Texture {
    public:
    vec3 getColor(vec3 hitTextureCoords) override {
        int M = 10;
        float checker = (fmod(hitTextureCoords.x() * M, 1.0) > 0.5) ^ (fmod(hitTextureCoords.y() * M, 1.0) < 0.5);
        return checker ? vec3(0,0,0) : vec3(255,255,255);
    }
};

#endif