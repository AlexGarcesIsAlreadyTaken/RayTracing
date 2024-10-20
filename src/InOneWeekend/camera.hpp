#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "hittable_list.hpp"
#include "material.hpp"

class camera {
public:
    camera(const point3& position, const point3& lookAt, const vec3& up, double image_width = 400.0, double aspect_ratio = 16.0/9.0){
        this->reset(position, lookAt, up, image_width, aspect_ratio);
    }

    void render(const hittable_list& world){
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\r[" << int(100 * j / image_height) << "%]" << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color = vec3(0.0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color / samples_per_pixel);
            }
        }
        std::clog << "\rDone.                         \n";
    }

    void reset(const point3& position, const point3& lookAt, const vec3& up, double image_width, double aspect_ratio) {
        this->position = position;
        this->image_width = image_width;
        this->aspect_ratio = aspect_ratio;
        this->lookAt = lookAt;
        this->vup = up;

        image_height = image_width / aspect_ratio;

        double theta = utilities::degrees_to_radians(vfov);
        double h = std::tan(theta/2.0);

        viewport_height = 2.0 * h * focus_dist;
        viewport_width = viewport_height * (double(this->image_width) / image_height);

        vec3 w = unit_vector(this->position - this->lookAt);
        vec3 u = unit_vector(cross(this->vup, w));
        vec3 v = cross(w, u);
        
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / this->image_width;
        pixel_delta_v = viewport_v / image_height;
        
        // center - vec3(0,0, focal_length) the point of the camera viewport center.
        // in this case we are translating the camera -1 unit into the z-axis 
        point3 viewport_upper_left = this->position - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

        // calculate the camera defocus disk basis vectors
        double defocus_radius = focus_dist * std::tan(utilities::degrees_to_radians(defocus_angle / 2.0));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

private:
    double aspect_ratio;
    double image_width, image_height;

    double viewport_width, viewport_height;
    vec3 pixel_delta_u, pixel_delta_v;
    point3 pixel00_loc;

    double vfov = 20.0;

    double defocus_angle = 0.6; // variation angle of rays through each pixel
    double focus_dist = 10.0; // distance from camera position to plane of perfect focus

    vec3 defocus_disk_u; // defocus disk horizontal radius
    vec3 defocus_disk_v; // defocus disk vertical radius

    point3 position;
    point3 lookAt;
    vec3 vup = vec3(0, 1, 0);

    // for each pixel we will cast a number of rays that "goes" from the center to a point of the square region of the pixel
    uint8_t samples_per_pixel = 20;
    uint8_t max_depth = 6;

    color background_color = color(0.5, 0.7, 1.0);
    
    color ray_color(const ray& r,uint8_t depth, const hittable_list& world) {
        if (depth <= 0)
            return color(0.0);

        hit_record rec;

        if (world.hit(r, interval(0.001, constants::infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) {
        // we gotta generate a ray that goes to the sample position from one point
        // of the defocus disk
        vec3 origin = defocus_disk_sample();
        vec3 offset = sample_square();
        point3 pixel_sample = pixel00_loc + (i + offset.x())*pixel_delta_u + (j + offset.y())*pixel_delta_v;
        return ray(origin, pixel_sample - origin);
    }

    vec3 sample_square() const {
        return vec3(utilities::random_double() - .5, utilities::random_double() - .5, 0.0);
    }

    point3 defocus_disk_sample() const {
        vec3 p = random_in_unit_disk();
        return this->position + (p.x()*defocus_disk_u) + (p.y()*defocus_disk_v);
    }
};

#endif