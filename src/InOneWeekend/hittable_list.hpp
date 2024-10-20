#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP


#include "hittable.hpp"

#include <vector>

class hittable_list : public hittable {
    private:
        std::vector<shared_ptr<hittable>> hittables;

    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> hittable) {
            add(hittable);
        }

        void add(shared_ptr<hittable> hittable) {
            hittables.push_back(hittable);
        }

        bool hit(const ray& r, interval i, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;

            for (const auto& hittable : this->hittables) {
                if (hittable->hit(r, i, temp_rec)) {
                    hit_anything = true;
                    i.max = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
};

#endif