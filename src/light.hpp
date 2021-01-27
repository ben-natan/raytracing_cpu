#ifndef LIGHT_H
#define LIGHT_H
#include "mat4.hpp"
#include "vec3.hpp"



class Light {
    protected:
        mat4 _lightToWorld;
        vec3 _color;
        float _intensity;

    public:
        Light() : _lightToWorld(mat4()), _color(vec3(255,255,255)), _intensity(1.0) {}
        Light(const mat4 &l2w): _lightToWorld(l2w) {} 

        virtual ~Light() {
            std::cout << "Light" << std::endl;
        }

        float intensity() {
            return _intensity;
        }

        vec3 color() {
            return _color;
        }

        virtual vec3 pos() const = 0;
        virtual vec3 dir() const = 0;

};



class PointLight: public Light {
    private:
        vec3 _pos;
    public:
        PointLight(const mat4 &l2w, const vec3 &pos, const vec3 &color = vec3(255,255,255), const float &intensity = 1.0):
                Light(l2w),
                _pos(pos)
                {
                    this->_color = color;
                    this->_intensity = intensity;
                }   
        
        vec3 pos() const override{
            return _pos;
        }

        vec3 dir() const override { // Ne doit pas être appelée
            return vec3(0,0,0);
        }

};


class DistantLight: public Light {
    private:
        vec3 _dir;
    public:
        DistantLight(const mat4 &l2w, const vec3 &dir, const vec3 &color = vec3(255,255,255), const float &intensity = 1.0) :
                Light(l2w),
                _dir(dir)
                {
                    this->_color = color;
                    this->_intensity = intensity;
                }
        
        vec3 pos() const override{ // Ne doit pas être appelée
            return vec3(0,0,0);
        }

        vec3 dir() const override { 
            return _dir;
        }
        
    
};





#endif