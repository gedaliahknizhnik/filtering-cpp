#include <iostream>

#include "smoothing.hpp"
#include "unistd.h"

int main(int argc, char *argv[]) { /* ... */
  int x{4};
  //   Smoother<double> s{}; // Not allowed bc abstract class
  MovingAverageSmoother<double> s{100};
  ExponentialSmoother<double> e{0.1};
  double out_m{0};
  double out_e{0};
  while (true) {
    s.smooth(1, out_m);
    e.smooth(1, out_e);
    std::cout << "MovingAverage: " << out_m << " Exponential: " << out_e
              << "\n";
    usleep(100000);
  }
}