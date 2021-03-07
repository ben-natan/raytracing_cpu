#ifndef LIGHT_H
#define LIGHT_H
#include "vec3.hpp"



class Light {
    protected:
        vec3 _color;
        float _intensity;

    public:
        Light() : _color(vec3(255,255,255)), _intensity(3.0) {}
        Light(vec3 color, float intensity = 3.0) : _color(color) {} 

        virtual ~Light() = default;

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
        PointLight(const vec3 &pos, const vec3 &color = vec3(255,255,255), const float &intensity = 1.0):
                Light(),
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


class DirectionalLight: public Light {
    private:
        vec3 _dir;
    public:
        DirectionalLight(const vec3 &dir, const vec3 &color = vec3(255,255,255), const float &intensity = 1.0) :
                Light(),
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