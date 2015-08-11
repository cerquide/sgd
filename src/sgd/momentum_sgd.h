#ifndef SGD_MOMENTUM_SGD_H
#define SGD_MOMENTUM_SGD_H

#include "basedef.h"
#include "data/data_point.h"
#include "data/data_set.h"
#include "learn-rate/learn_rate_value.h"
#include "sgd/base_sgd.h"
#include <stdlib.h>

class momentum_sgd : public base_sgd {
  /**
   * Stochastic gradient descent using classical momentum
   *
   * @param sgd       attributes affiliated with sgd as R type
   * @param n_samples number of data samples
   * @param ti        timer for benchmarking how long to get each estimate
   */
public:
  momentum_sgd(Rcpp::List sgd, unsigned n_samples, const boost::timer& ti) :
    base_sgd(sgd, n_samples, ti) {
    mu_ = 0.9;
    v_ = last_estimate_;
  }

  template<typename MODEL>
  mat update(unsigned t, const mat& theta_old, const data_set& data,
    MODEL& model, bool& good_gradient) {
    data_point data_pt = data.get_data_point(t);
    mat grad_t = model.gradient(theta_old, data_pt, data);
    learn_rate_value at = learning_rate(grad_t, t);
    if (!is_finite(grad_t)) {
      good_gradient = false;
    }
    v_ = mu_ * v_ + (at * grad_t);
    return theta_old + v_;
  }

  momentum_sgd& operator=(const mat& theta_new) {
    base_sgd::operator=(theta_new);
    return *this;
  }
private:
  double mu_; // factor to weigh previous "velocity"
  mat v_;     // "velocity"
};

#endif
