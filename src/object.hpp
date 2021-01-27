#include "vec3.hpp"
#include "ray.hpp"


class Object {
    private:
        float _k_mirror;
        float _k_transparent;
    public: 
        virtual bool intersect(const std::unique_ptr<Ray>& ray, float* distance, vec3& pHit, vec3& normal, vec3& color) const = 0;
        
        bool isMirror() {
            return (_k_mirror!=0.0);
        }

        bool isTransparent() {
            return (_k_transparent!=0.0);
        }

        float k_mirror() {
            return _k_mirror;
        }
        
        float k_transparent() {
            return _k_transparent;
        }
};