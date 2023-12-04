#include <cmath>
#include <random>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include "filter.hpp"

int main() {
  const double freq{10};
  // Define random generator with Gaussian distribution
  const double mean = 0.0;
  const double stddev = 0.1;
  std::default_random_engine generator;
  std::normal_distribution<double> dist(mean, stddev);

  LowPassFilter<double> lp{0.1};
  HighPassFilter<double> hp{0.1};

  // Prepare data.
  constexpr int n = 5000;

  std::vector<double> y_h(n);
  std::vector<double> y_l(n);

  std::vector<double> x(n);
  std::vector<double> y(n);

  for (int ii{0}; ii < n; ++ii) {
    x[ii] = ii;
    y[ii] = sin(freq * M_PI * ii / 360.0) + dist(generator);
    lp.filter(y[ii], y_l[ii]);
    hp.filter(y[ii], y_h[ii]);
  }

  // Set the size of output image to 1200x780 pixels
  plt::figure_size(1200, 780);
  // Plot line from given x and y data. Color is selected automatically.
  plt::named_plot("True", x, y);
  plt::named_plot("HighPass.", x, y_h, "--");
  plt::named_plot("LowPass.", x, y_l, "--");

  plt::xlim(0, n);
  // Add graph title
  plt::title("Filtering Example");

  // Enable legend.
  plt::legend();
  // Save the image (file format is determined by the extension)
  plt::save("./highlowpass.png");
}
