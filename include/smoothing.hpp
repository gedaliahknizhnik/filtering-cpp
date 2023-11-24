#ifndef SMOOTHING_HPP
#define SMOOTHING_HPP

#include <vector>

template <typename T>
class Smoother {
 public:
  virtual void smooth(const T data_in, T& data_out) = 0;
  virtual void reset() = 0;
  virtual void set_filter_size(const int size) = 0;
};

template <typename T>
class ExponentialSmoother : public Smoother<T> {
 public:
  ExponentialSmoother(const T filter_constant)
      : _filter_constant{filter_constant} {}

  virtual void smooth(const T data_in, T& data_out) override {
    _filtered_data =
        _filter_constant * (_filtered_data) + (1 - _filter_constant) * data_in;
    data_out = _filtered_data;
  }

  virtual void reset() override { _filtered_data = 0; }
  virtual void set_filter_size(const int size) override { return; };

 private:
  T _filter_constant;
  T _filtered_data{0};
};

template <typename T>
class MovingAverageSmoother : public Smoother<T> {
 public:
  // CONSTRUCTORS **************************************************************
  MovingAverageSmoother(const int filter_size) : _filter_size{filter_size} {
    _data.resize(_filter_size);
  }

  MovingAverageSmoother(const int call_frequency, const int filter_period)
      : MovingAverageSmoother{filter_period * call_frequency} {}

  // FILTERING FUNCTIONS *******************************************************
  virtual void smooth(const T data_in, T& data_out) override {
    int ind{circular_ind(_filter_ind++)};

    _filter_sum = _filter_sum - _data[ind] + data_in;
    _data[ind] = data_in;

    data_out = _filter_sum / _filter_size;
  }

  virtual void reset() override {
    std::fill(_data.begin(), _data.end(), 0);
    _filter_ind = 0;
  }

  virtual void set_filter_size(const int size) {
    _filter_size = size;
    _data.resize(_filter_size);

    reset();
  };

 private:
  // PRIVATE SUPPORT FUNCTIONS *************************************************

  inline int circular_ind(const int ind) { return ind % _filter_size; }

  // VARIABLES *****************************************************************

  std::vector<T> _data{};
  int _filter_size{};

  T _filter_sum{0};
  int _filter_ind{0};
};

#endif