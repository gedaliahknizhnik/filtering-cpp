#include <iostream>

#include "filter.hpp"
#include "unistd.h"

int main(int argc, char *argv[]) { /* ... */
  int x{4};
  //   Filter<double> s{}; // Not allowed bc abstract class
  MovingAverageFilter<double> s{100};
  ExponentialFilter<double> e{0.1};
  // ExponentialFilter<double> e1{1.1};  // Should throw an exception.

  double out_m{0};
  double out_e{0};
  while (true) {
    s.filter(1, out_m);
    e.filter(1, out_e);
    std::cout << "MovingAverage: " << out_m << " Exponential: " << out_e
              << "\n";
    usleep(100000);
  }
}
