#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.hpp"
class Object;
#include "ray.hpp"
#include <iostream>
#include <algorithm>

class Ray;

class Object {
    protected:
        float _k_mirror;
        float _k_transparent;
        float _albedo;
        float _Ks;
        int _spec_n;
        float _ior; // index of refraction
        vec3 _color;
    public: 
        virtual ~Object() {
            
        }
        Object(): _k_mirror(1.0), _k_transparent(0.0), _albedo(0.18), _spec_n(3), _ior(1.5), _color(vec3(255,255,255)), _Ks(0.1) {} // albedo 0.18
        Object(float k_mir, float k_trans, float albedo = 0.18, int spec_n = 3, float ior = 1.5, vec3 color = vec3(255,255,255), float Ks = 0.1): _k_mirror(k_mir), _k_transparent(k_trans), _albedo(albedo), _spec_n(spec_n), _ior(ior), _color(color), _Ks(Ks) {}
        Object(float k_mir, float k_trans, vec3 color = vec3(255,255,255), float albedo = 0.18, int spec_n = 3, float ior = 1.5, float Ks = 0.1): _k_mirror(k_mir), _k_transparent(k_trans), _albedo(albedo), _spec_n(spec_n), _ior(ior), _color(color), _Ks(Ks) {}

        virtual bool intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const = 0;
        virtual bool intersectShadow(Ray ray, float& distance) const = 0;

        
        bool isMirror() {
            return (_k_mirror!=0.0);
        }

        bool isTransparent() {
            return (_k_transparent!=0.0);
        }

        vec3 color() {
            return _color;
        }

        float k_mirror() {
            return _k_mirror;
        }
        
        float k_transparent() {
            return _k_transparent;
        }

        float albedo() {
            return _albedo;
        }

        int spec_n() {
            return _spec_n;
        }

        float Ks() {
            return _Ks;
        }

        bool refract(vec3 incident, vec3 normal, vec3& newDir) {
            float cosi = std::clamp(-1.0f, 1.0f, normal.dot(incident));
            float n_i = 1, n_t = _ior;
            vec3 n = normal;
            if (cosi < 0) { cosi = -cosi; } else { std::swap(n_i, n_t); n = -n;}
            float eta = n_i / n_t;
            float k = 1 - eta * eta * ( 1 - cosi * cosi);
            if (k < 0) {
                return false;
            } else {
                newDir = (eta*incident + (eta*cosi - sqrtf(k))*n).normalize();
                return true;
            }
        }

        void fresnel(vec3 incident, vec3 normal, float &kr) {
            float cosi = std::clamp(-1.0f, 1.0f, normal.dot(incident));
            float n_i = 1, n_t = _ior;
            if (cosi > 0) { std::swap(n_i,n_t); }
            float sint = n_i / n_t *sqrtf(std::max(0.f, 1 - cosi * cosi));
            if (sint >= 1) {
                kr = 1;
            } else {
                float cost = sqrtf(std::max(0.f, 1 - sint * sint));
                cosi = fabs(cosi);
                float Rs = ((n_t * cosi) - (n_i * cost)) / ((n_t * cosi) + (n_i * cost));
                float Rp = ((n_i * cosi) - (n_t * cost)) / ((n_i * cosi) + (n_t * cost)); 
                kr = (Rs * Rs + Rp * Rp) / 2;  // kt = 1 - kr
            }
        }
};

#endif