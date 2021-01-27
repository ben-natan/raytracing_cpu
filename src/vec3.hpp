#ifndef VEC3_H
#define VEC3_H


#include <iostream>

// Retirer le typename parcequ'il sert à rien: le constructeur met les int en float si `float _x, _y, _z`

// template<typename T>
class vec3 {
    private:
        float _x,_y,_z;

    public:
        vec3() : _x(0), _y(0), _z(0) {}

        vec3(float gX, float gY, float gZ): _x(gX), _y(gY), _z(gZ) {};

        // Getter ......------------------&
        float x() {
            return _x;
        }

        float y() {
            return _y;
        }

        float z() {
            return _z;
        }

        // Operators .. ------------------&
        vec3 operator *(vec3 otherVec) {
            return vec3(_x * otherVec._x, _y * otherVec._y, _z * otherVec._z);
        };
        
        vec3 operator +(vec3 const otherVec) {
            return vec3(_x+ otherVec._x, _y + otherVec._y, _z+ otherVec._z);
        };

        vec3 operator -(vec3 otherVec) {
            return vec3(_x- otherVec._x, _y - otherVec._y, _z - otherVec._z);
        };

        vec3 operator *=(vec3 otherVec) {
            _x*=otherVec._x;
            _y*=otherVec._y;
            _z*=otherVec._z;
            return *this;
        };

        vec3 operator +=(vec3 otherVec) {
            _x+=otherVec._x;
            _y+=otherVec._y;
            _z+=otherVec._z;
            return *this;
        };

        vec3 operator -=(vec3 otherVec) {
            _x-=otherVec._x;
            _y-=otherVec._y;
            _z-=otherVec._z;
            return *this;
        };

        friend inline std::ostream& operator <<(std::ostream& os, const vec3 &theVec);

        template<typename T>
        friend vec3 operator *(T k, const vec3 &theVec);

        vec3 normalize() {
            float norm = sqrt(_x*_x + _y*_y + _z*_z);
            _x = _x/norm;
            _y = _y/norm;
            _z = _z/norm;
            return *this;
        }

        float dot(vec3 otherVec) {
            return _x*otherVec._x + _y*otherVec._y + _z*otherVec._z;
        };

        float norm() {
            return sqrt(_x*_x + _y*_y + _z*_z);
        }



};


inline std::ostream& operator <<(std::ostream& os, const vec3 &theVec) {
    os << "[" << theVec._x << "," << theVec._y << "," << theVec._z << "]";
    return os;
};

template<typename T>
vec3 operator *(T k, const vec3 &theVec) {
    return vec3(k*theVec._x, k*theVec._y, k*theVec._z);
}





#endif