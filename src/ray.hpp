#ifndef RAY_H
#define RAY_H


#include "vec3.hpp"
#include "math.hpp"
#include "object.hpp"
#include "light.hpp"
#include <vector>
#include <memory>



class Ray {
    private: 
        vec3 _origin;
        vec3 _direction; // Doit être normalisé!
        vec3 _color;
        int _depth;

    public:
        Ray() : _origin(vec3(0,0,0)), _direction(vec3(0,1,0)), _color(vec3(255,255,255)), _depth(2) {}

        Ray(int x, int y, int width, int height, float fov, int depth = 2) { //Primary ray
            float aspectRatio = width / (float)height; // Si width > height
            float x_camera = (2* ((x + 0.5) / width)- 1) * aspectRatio * tan(fov/2 * M_PI /180); 
            float y_camera = (1 - 2*((y + 0.5) / height)) * tan(fov/2 * M_PI/180);
            _origin = vec3(0,0,0);
            _direction = vec3(x_camera, y_camera, -1.0).normalize();
            _depth = depth;
            _color = vec3(0,0,0);
        };

        Ray(vec3 origin, vec3 direction, vec3 color, int depth = 2): 
            _origin(origin.normalize()), _direction(direction.normalize()), _color(color), _depth(depth) { } 
        
        // Setters ......------------------&
        void setOrigin(vec3 origin) {
            _origin = origin;
        }

        void setDirection(vec3 direction) {
            _direction = direction;
        }

        void setColor(vec3 color) {
            _color = color;
        }


        // Getters ......------------------&
        vec3 origin() {
            return _origin;
        };

        vec3 direction() {
            return _direction;
        };

        vec3 color() {
            return _color;
        }

        int depth() {
            return _depth;
        }

        void addColor(vec3 color) {
            _color += color;
            if (_color.x() > 255) _color.setx(255);
            if (_color.y() > 255) _color.sety(255);
            if (_color.z() > 255) _color.setz(255);
        }

        void multiplyColor(vec3 color) {
            _color *= ((1/255)*color); 
        }
        
        void Shoot(std::vector<std::unique_ptr<Object>>& objects, std::vector<std::unique_ptr<Light>>& lights,
                    int n_obj, int n_lig);

        void computeReflectedDirection(vec3 normal, vec3& direction);
};


#endif