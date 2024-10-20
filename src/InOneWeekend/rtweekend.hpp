#ifndef RTWEEKEND_HPP
#define RTWEEKEND_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>


using std::make_shared;
using std::shared_ptr;

namespace constants {
    const double infinity = std::numeric_limits<double>::infinity();
    const double pi = 3.141592653589793;
};

namespace utilities {
    inline double degrees_to_radians(double degrees) {
        return degrees * constants::pi / 180.0;
    }

    // random number in the range [0..1)
    inline double random_double() {
        return std::rand() / (RAND_MAX - 1.0);
    }

    inline double random_double(double min, double max) {
        return min + random_double()*(max - min);
    }

    inline double linear_to_gamma(double linear_component) {
        if (linear_component < 0) return 0.0;
        return std::sqrt(linear_component);
    }
};

#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#endif