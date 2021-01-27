#ifndef VEC3_H
#define VEC3_H


#include <iostream>

// Retirer le typename parcequ'il sert à rien: le constructeur met les int en float si `float x, y, z`

// template<typename T>
class vec3 {
    private:
        float x,y,z;

    public:
        vec3() : x(0), y(0), z(0) {}

        vec3(float gX, float gY, float gZ): x(gX), y(gY), z(gZ) {};

        // Operators .. ------------------&
        vec3 operator *(vec3 otherVec) {
            return vec3(x * otherVec.x, y * otherVec.y, z * otherVec.z);
        };
        
        vec3 operator +(vec3 const otherVec) {
            return vec3(x+ otherVec.x, y + otherVec.y, z+ otherVec.z);
        };

        vec3 operator -(vec3 otherVec) {
            return vec3(x- otherVec.x, y - otherVec.y, z - otherVec.z);
        };

        vec3 operator *=(vec3 otherVec) {
            x*=otherVec.x;
            y*=otherVec.y;
            z*=otherVec.z;
            return *this;
        };

        vec3 operator +=(vec3 otherVec) {
            x+=otherVec.x;
            y+=otherVec.y;
            z+=otherVec.z;
            return *this;
        };

        vec3 operator -=(vec3 otherVec) {
            x-=otherVec.x;
            y-=otherVec.y;
            z-=otherVec.z;
            return *this;
        };

        friend std::ostream& operator <<(std::ostream& os, const vec3 &theVec);

        template<typename T>
        friend vec3 operator *(T k, const vec3 &theVec);

        vec3 normalize() {
            float norm = sqrt(x*x + y*y + z*z);
            x = x/norm;
            y = y/norm;
            z = z/norm;
            return *this;
        }

        float dot(vec3 otherVec) {
            return x*otherVec.x + y*otherVec.y + z*otherVec.z;
        };



};


std::ostream& operator <<(std::ostream& os, const vec3 &theVec) {
    os << "[" << theVec.x << "," << theVec.y << "," << theVec.z << "]";
    return os;
};

template<typename T>
vec3 operator *(T k, const vec3 &theVec) {
    return vec3(k*theVec.x, k*theVec.y, k*theVec.z);
}





#endif