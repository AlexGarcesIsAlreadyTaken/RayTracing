#ifndef COLOR_H
#define COLOR_H


#include "interval.hpp"
#include "vec3.hpp"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	r = utilities::linear_to_gamma(r);
	g = utilities::linear_to_gamma(g);
	b = utilities::linear_to_gamma(b);

	static const interval intensity(0.0, 0.9999);
	int rbyte = int(255.999 * intensity.clamp(r));
	int gbyte = int(255.999 * intensity.clamp(g));
	int bbyte = int(255.999 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
