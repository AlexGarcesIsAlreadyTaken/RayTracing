#ifndef INTERVAL_HPP
#define INTERVAL_HPP

struct interval {
    double min, max;

    interval() : min(+constants::infinity), max(-constants::infinity) {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    double size() const { return max - min; }

    bool contains(double x) const { return min <= x && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }
    double clamp(double x) const { if (x < min) return min; if (x > max) return max; return x; }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+constants::infinity, -constants::infinity);
const interval interval::universe = interval(-constants::infinity, +constants::infinity);

#endif
