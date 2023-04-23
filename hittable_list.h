//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_HITTABLE_LIST_H
#define RAYTRACER_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:

    hittable_list() {}

    hittable_list(shared_ptr<hittable> object) {
        add(object);
    }

    void clear() {
        objects.clear();
    }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

public:
    std::vector<shared_ptr<hittable>> objects;

};

bool hittable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_any = false;
    auto closest_so_far = t_max;

    for (const auto &object: objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_any = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_any;
}

#endif //RAYTRACER_HITTABLE_LIST_H
