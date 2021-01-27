#ifndef RAY_H
#define RAY_H


#include "vec3.hpp"
#include "math.hpp"



class Ray {
    private: 
        vec3 _origin;
        vec3 _direction; // Doit être normalisé!
        vec3 _color;

    public:
        Ray() : _origin(vec3(0,0,0)), _direction(vec3(0,1,0)), _color(vec3(100,100,100)) {}

        Ray(int x, int y, int width, int height, float fov) { //Primary ray
            float aspectRatio = width / (float)height; // Si width > height
            float x_camera = (2* ((x + 0.5) / width)- 1) * aspectRatio * tan(fov/2 * M_PI /180); 
            float y_camera = (1 - 2*((y + 0.5) / height)) * tan(fov/2 * M_PI/180);
            _origin = vec3(0,0,0);
            _direction = vec3(x_camera, y_camera, -1).normalize();
        };

        Ray(vec3 origin, vec3 direction, vec3 color): 
            _origin(origin.normalize()), _direction(direction.normalize()), _color(color) { } 

        vec3 origin() {
            return _origin;
        };

        vec3 direction() {
            return _direction;
        };

        vec3 color() {
            return _color;
        }

        void addColor(vec3 color) {
            _color += color;
        }

        void Shoot() {} // Une fois qu'on a trouvé la plus petite distance

};


#endif