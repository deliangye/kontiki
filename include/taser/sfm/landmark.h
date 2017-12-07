//
// Created by hannes on 2016-12-01.
//

#ifndef TASER_LANDMARK_H
#define TASER_LANDMARK_H

#include <memory>
#include <vector>

#include <Eigen/Core>
#include "observation.h"

namespace taser {

class Landmark : public std::enable_shared_from_this<Landmark> {
  friend View;
  static size_t new_id() {
    static size_t next_id = -1;
    ++next_id;
    return next_id;
  }

 public:
  Landmark() : inverse_depth_(0), id_(new_id()) {}

  auto id() const {
    return id_;
  }

  void set_reference(std::shared_ptr<Observation> new_ref) {
    if (new_ref->landmark().get() != this)
      throw std::runtime_error("Observation not in observations list");
    else
      reference_observation_ = new_ref;
  }

  auto reference() const {
    auto sp = reference_observation_.lock();
    if (sp)
      return sp;
    else {
      std::stringstream ss;
      ss << "Landmark id=" << id() << ": Failed to get reference observation!";
      throw std::runtime_error(ss.str());
    }
  }

  auto observations() const {
    std::vector<std::shared_ptr<Observation>> obslist;
    for (auto wp : observations_) {
      auto sp = wp.lock();
      if (sp)
        obslist.push_back(sp);
      else
        throw std::runtime_error("Observation removed without Landmark knowing about it!");
    }
    return obslist;
  }

  auto inverse_depth() const { return inverse_depth_; }
  void set_inverse_depth(double x) { inverse_depth_ = x; }
  auto inverse_depth_ptr() { return &inverse_depth_; }

 protected:
  size_t id_;
  double inverse_depth_;
  std::weak_ptr<Observation> reference_observation_;
  std::vector<std::weak_ptr<Observation>> observations_;
};

} // namespace taser
#endif //TASER_LANDMARK_H