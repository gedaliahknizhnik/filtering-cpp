/**
 * @file filter.hpp
 * @author Gedaliah Knizhnik (gedaliah.knizhnik@gmail.com)
 * @brief Filters for incoming data streams
 * @version 0.2
 * @date 2023-11-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILTER_HPP
#define FILTER_HPP

#include <memory>
#include <stdexcept>
#include <vector>

// ABSTRACT FILTER CLASS *******************************************************

/**
 * @brief Abstract base class from which all filters are derived
 *
 * @tparam T - data type that the filter will use.
 */
template <typename T>
class Filter {
 public:
  /**
   * @brief Destroy the Filter object
   *
   */
  virtual ~Filter() {}

  /**
   * @brief Primary filter function for each filter
   *
   * @param data_in - incoming data as it arrives
   * @param data_out - reference to output of filtered data
   */
  virtual void filter(const T data_in, T& data_out) = 0;
  /**
   * @brief Reset the filter to an un-initialized state
   *
   */
  virtual void reset() = 0;
  /**
   * @brief Set the filter size
   *
   * @param size - how many data points does the filter consider?
   */
  virtual void set_filter_size(const int size) = 0;

  /**
   * @brief Return a unique pointer to allow creating multi-filter objects
   *
   * @return std::unique_ptr<Filter<T>>
   */
  virtual std::unique_ptr<Filter<T>> clone() const = 0;
};

// EXPONENTIAL FILTER **********************************************************

/**
 * @brief Exponential Filter
 *
 * The most basic type of filter, in which (for x in and y out):
 *    y[k] = (1-a)*y[k-1] + a*x[k]
 *
 * @tparam T - data type used by the filter
 */
template <typename T>
class ExponentialFilter : public Filter<T> {
 public:
  // CONSTRUCTORS **************************************************************

  /**
   * @brief Construct a new Exponential Filter object using a filter constant
   *
   * @param filter_constant - constant used in the filter
   */
  ExponentialFilter(const T filter_constant)
      : _filter_constant{filter_constant} {
    if ((_filter_constant <= 0) || (_filter_constant > 1)) {
      throw std::domain_error("Filter constant must be in the range (0, 1]");
    }
  }

  /**
   * @brief Apply the filter to a new input data point
   *
   * @param data_in - newest arrived data point
   * @param data_out - reference to the output data
   */
  virtual void filter(const T data_in, T& data_out) override {
    _filtered_data =
        _filter_constant * data_in + (1 - _filter_constant) * _filtered_data;
    data_out = _filtered_data;
  }

  /**
   * @brief Reset the filter by setting the filtered data to ZERO
   *
   */
  virtual void reset() override { _filtered_data = 0; }
  /**
   * @brief Set the filter size - NO EFFECT
   *
   * This function has no effect because an exponential filter does not store
   * more than one prior point.
   *
   * @param size - the size of the filter
   */
  virtual void set_filter_size(const int size) override { return; };
  /**
   * @brief Return a cast pointer to Filter<T> for creating multi-filter objects
   *
   * @return std::unique_ptr<Filter<T>>
   */
  virtual std::unique_ptr<Filter<T>> clone() const override {
    return std::make_unique<ExponentialFilter<T>>(*this);
  }

 protected:
  // VARIABLES *****************************************************************
  T _filter_constant;
  T _filtered_data{0};
};

/**
 * @brief Moving Average filter
 *
 * Note that the behavior of this filter before the data set is *full* is to
 * average with a bunch of zeros.
 *
 * @tparam T - data type used by the filter
 */
template <typename T>
class MovingAverageFilter : public Filter<T> {
 public:
  // CONSTRUCTORS **************************************************************

  /**
   * @brief Construct a new Moving Average Filter object using the filter size
   *
   * @param filter_size - the number of data points considered by the filter
   */
  MovingAverageFilter(const int filter_size) : _filter_size{filter_size} {
    _data.resize(_filter_size);
  }

  /**
   * @brief Construct a new Moving Average Filter object using the sample
   * frequency and filter period
   *
   * @param call_frequency - frequency at which new data arrives [Hz]
   * @param filter_period - period at which the data is filtered [s]
   */
  MovingAverageFilter(const int call_frequency, const T filter_period)
      : MovingAverageFilter{filter_period * call_frequency} {}

  // FILTERING FUNCTIONS *******************************************************

  /**
   * @brief Primary filter function. Apply a moving average filter to the input
   * data and output the filtered result
   *
   * @param data_in - input data point
   * @param data_out - reference to output data point
   */
  virtual void filter(const T data_in, T& data_out) override {
    int ind{circular_ind(_filter_ind++)};

    _filter_sum = _filter_sum - _data[ind] + data_in;
    _data[ind] = data_in;

    data_out = _filter_sum / _filter_size;
  }

  /**
   * @brief Reset the filter by resetting all the data points to zero.
   *
   */
  virtual void reset() override {
    std::fill(_data.begin(), _data.end(), 0);
    _filter_ind = 0;
  }

  /**
   * @brief Set the filter size. Note that this resets the filter automatically
   *
   * @param size
   */
  virtual void set_filter_size(const int size) override {
    _filter_size = size;
    _data.resize(_filter_size);

    reset();
  };

  /**
   * @brief Return a cast pointer to Filter<T> for creating multi-filter objects
   *
   * @return std::unique_ptr<Filter<T>>
   */
  virtual std::unique_ptr<Filter<T>> clone() const override {
    return std::make_unique<MovingAverageFilter<T>>(*this);
  }

 private:
  // PRIVATE SUPPORT FUNCTIONS *************************************************

  /**
   * @brief Allow an arbitrarily large index to safely index into the circular
   * data buffer.
   *
   * @param ind - regular index
   * @return int - correct index to use when indexing into _data
   */
  inline int circular_ind(const int ind) { return ind % _filter_size; }

  // VARIABLES *****************************************************************

  std::vector<T> _data{};  ///< Internal data vector
  int _filter_size{};      ///< Size of the internal data vector

  T _filter_sum{0};    ///< Running sum of the entries in the data vector
  int _filter_ind{0};  ///< Index at which we're entering the
};

/**
 * @brief A discrete implementation of a low-pass filter.
 *
 * This is exactly the same as the basic ExponentialFilter defined above. We
 * simply define an RC constructor for it.
 *
 * @tparam T - the data type used by the filter
 */
template <typename T>
class LowPassFilter : public ExponentialFilter<T> {
 public:
  // CONSTRUCTORS **************************************************************

  /**
   * @brief Create a LowPassFilter visualized as an RC circuit.
   *
   * @param RC - the product of the resistance and capacitance
   * @param dt - the sampling interval
   */
  LowPassFilter(T RC, T dt) : ExponentialFilter<T>{dt / (RC + dt)} {};
  /**
   * @brief Create a LowPassFilter visualized as an RC circuit. @overload
   *
   * @param R - the resistance value
   * @param C - the capacitance value
   * @param dt - the sampling interval
   */
  LowPassFilter(T R, T C, T dt) : LowPassFilter{R * C, dt} {};
};

/**
 * @brief A discrete implementation of a high-pass filter.
 *
 * This is a modified ExponentialFilter as defined above. We define an RC
 * constructor and modify the filter method.
 *
 * A high pass filter is implemented as:
 *  y[k] = alpha*y[k-1] + alpha*(x[k] - x[k-1])
 *
 * @tparam T - the data type used by the filter
 */
template <typename T>
class HighPassFilter : public ExponentialFilter<T> {
  using ExponentialFilter<T>::_filter_constant;  ///< Gives access to protected
                                                 ///< member
  using ExponentialFilter<T>::_filtered_data;    ///< Gives access to protected
                                                 ///< member

 public:
  // CONSTRUCTORS **************************************************************

  /**
   * @brief Create a HighPassFilter visualized as an RC circuit.
   *
   * @param RC - the product of the resistance and capacticance
   * @param dt - the sampling interval
   */
  HighPassFilter(T RC, T dt) : ExponentialFilter<T>{RC / (RC + dt)} {};
  /**
   * @brief Create a HighPassFilter visualized as an RC circuit.
   *
   * @param R - the resistance value
   * @param C - the capacitance value
   * @param dt - the sampling interval
   */
  HighPassFilter(T R, T C, T dt) : HighPassFilter(R * C, dt){};

  /**
   * @brief Filter the data coming in
   *
   * @param data_in - the newest data point input to the filter
   * @param data_out - reference to where to put the output filtered data.
   */
  virtual void filter(const T data_in, T& data_out) override {
    _filtered_data = _filter_constant * _filtered_data +
                     _filter_constant * (data_in - _last_data);
    _last_data = data_in;
    data_out = _filtered_data;
  }

 private:
  T _last_data;
};

#endif
