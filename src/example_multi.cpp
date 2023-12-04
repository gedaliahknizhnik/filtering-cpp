#include <cmath>
#include <random>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include "filtering/multistream.hpp"

int main() {
  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = 0.1;
  std::default_random_engine generator;
  std::normal_distribution<double> dist(mean, stddev);

  MovingAverageFilter<double> s{50};
  ExponentialFilter<double> e{0.1};

  MultiStreamFilter<double, 2> m{MovingAverageFilter<double>(20)};
  // MultiStreamFilter<double, 2> m{ExponentialFilter<double>(0.1)};

  // Prepare data.
  constexpr int n = 5000;

  std::vector<std::array<double, 2>> y_m(n);
  std::vector<double> y_m1(n);  // For plotting only
  std::vector<double> y_m2(n);  // For plotting only

  std::vector<double> x(n);
  std::vector<double> y(n);
  std::vector<double> z(n);

  for (int ii{0}; ii < n; ++ii) {
    x[ii] = ii;
    y[ii] = sin(2 * M_PI * ii / 360.0) + dist(generator);
    z[ii] = cos(2 * M_PI * ii / 360.0) + dist(generator);
    m.filter({y[ii], z[ii]}, y_m[ii]);
    y_m1[ii] = y_m[ii][0];
    y_m2[ii] = y_m[ii][1];
  }

  // PLOTTING
  // Set the size of output image to 1200x780 pixels
  plt::figure_size(1200, 780);
  // Plot line from given x and y data. Color is selected automatically.
  plt::named_plot("True y", x, y);
  plt::named_plot("True z", x, z);
  plt::named_plot("MAvg. y", x, y_m1, "--");
  plt::named_plot("MAvg. z", x, y_m2, "--");

  plt::xlim(0, n);
  // Add graph title
  plt::title("Multi-Filtering Example");

  // Enable legend.
  plt::legend();
  // Save the image (file format is determined by the extension)
  plt::save("./multi.png");
}
