#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <algorithm>



class math {
    public:

        static bool solveQuadratic(float a, float b, float c, float &x0, float &x1) {
            float discr = b*b - 4*a*c;
            if (discr < 0) {
                return false;
            } else if ( discr == 0 ) {
                x0 = x1 = -0.5 * b / a;
                return true;
            } else {
                float q = sqrt(discr);
                x0 = (-b + q)/(2*a);
                x1 = (-b - q)/(2*a);
                if (x0 > x1) {
                    std::swap(x0,x1); // x1 plus grand que x0
                }
                return true;
            };
        }
};

#endif