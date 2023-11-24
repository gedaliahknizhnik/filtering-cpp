#include <cmath>
#include <iostream>
#include <iterator>
#include <random>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include "filter.hpp"

int main() {
  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = 0.1;
  std::default_random_engine generator;
  std::normal_distribution<double> dist(mean, stddev);

  MovingAverageFilter<double> s{50};
  ExponentialFilter<double> e{0.1};

  // Prepare data.
  constexpr int n = 5000;

  std::vector<double> y_m(n);
  std::vector<double> y_e(n);

  std::vector<double> x(n);
  std::vector<double> y(n);

  for (int ii{0}; ii < n; ++ii) {
    x[ii] = ii;
    y[ii] = sin(2 * M_PI * ii / 360.0) + dist(generator);
    s.filter(y[ii], y_m[ii]);
    e.filter(y[ii], y_e[ii]);
  }

  // Set the size of output image to 1200x780 pixels
  plt::figure_size(1200, 780);
  // Plot line from given x and y data. Color is selected automatically.
  plt::named_plot("True", x, y);
  plt::named_plot("Exp.", x, y_e, "--");
  plt::named_plot("MAvg.", x, y_m, "--");

  plt::xlim(0, n);
  // Add graph title
  plt::title("Filtering Example");

  // Enable legend.
  plt::legend();
  // Save the image (file format is determined by the extension)
  plt::save("./basic.png");
}