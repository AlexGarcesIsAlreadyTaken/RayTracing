#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {

	point3 position = point3(13, 2,3);
	point3 lookAt = point3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);
	camera cam(position, lookAt, up, 800);

	hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = utilities::random_double();
            point3 center(a + 0.9*utilities::random_double(), 0.2, b + 0.9*utilities::random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = utilities::random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1, material1));

	auto material2 = make_shared<lambertian>(color(.4, .2, .1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1, material2));

	auto material3 = make_shared<metal>(color(0.7, .6, .5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1, material3));

	cam.render(world);
}
