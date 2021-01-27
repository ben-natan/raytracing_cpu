#include "vec3.hpp"
#include "ray.hpp"


// Pour l'instant, une sphere
class Light {
    private:
        vec3 _color;
        vec3 _center;
    
    public:
        virtual bool intersect(const std::unique_ptr<Ray>& ray, float* distance, vec3& pHit, vec3& normal, vec3& color) const = 0;
};