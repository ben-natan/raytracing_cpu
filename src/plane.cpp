// #include "plane.hpp"

// bool Plane::intersect(Ray* ray, float& distance, vec3& pHit, vec3& normal, vec3& color) const
// {
//     float denominator = (ray->direction()).dot(_normal);
//     if (abs(denominator) > 0.0001) {
//         vec3 diff = vec3(0,0,0)-(ray->origin() - _position); // Régler le problème de const
//         float t = diff.dot(_normal)/denominator;
//         // if (t > 0.0001) {
//             // Pour le shadow acne!
//         // }
//         distance = t;
//         pHit = ray->origin() + t*ray->direction();
//         normal = _normal;
//         return true;
//     }
//     return false;
// }

// bool Plane::intersectShadow(Ray ray, float& distance) const {
//     float denominator = (ray.direction()).dot(_normal);
//     if (abs(denominator) > 0.0001) {
//         vec3 diff = vec3(0,0,0)-(ray.origin() - _position); // Régler le problème de const
//         float t = diff.dot(_normal)/denominator;
//         // if (t > 0.0001) {
//             // Pour le shadow acne!
//         // }
//         distance = t;
//         return true;
//     }
//     return false;
// }