/**
 * @file multistream.hpp
 * @author Gedaliah Knizhnik (gedaliah.knizhnik@gmail.com)
 * @brief Filter multi-stream data streams
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MULTISTREAM_FILTER_HPP
#define MULTISTREAM_FILTER_HPP

#include <array>
#include <memory>

#include "filtering/filter.hpp"

/**
 * @brief MultiStreamFilter - filter multi-stream incoming data
 *
 * MultiStreamFiltering is useful for filtering vector data streams. E.g. we
 * have an incoming stream of data as [x, y, z], and we want to filter all the
 * xs, all the ys, and all of the zs.
 *
 * The MultiStreamFilter can handle any filter that inherits from `Filter` using
 * dynamic dispatch and function overloading. It then applies that filter to
 * each incoming datastream.
 *
 * @tparam T - type of each incoming data stream (aka double, int)
 * @tparam N - number of data streams
 */
template <typename T, int N>
class MultiStreamFilter {
 public:
  // CONSTRUCTOR ***************************************************************

  /**
   * @brief Construct a new Multi Stream Filter from any type of Filter object
   *
   * @param filter - an anonymous object of any Filter sub-type.
   */
  MultiStreamFilter(Filter<T> const& filter) {
    for (auto& _filter : _filters) {
      _filter = filter.clone();
    }
  }

  // FILTER FUNCTIONS **********************************************************

  /**
   * @brief Filter the data stream by applying the internal filters
   *
   * @param data_in
   * @param data_out
   */
  void filter(const std::array<T, N> data_in, std::array<T, N>& data_out) {
    for (int ii{0}; ii < N; ++ii) {
      _filters[ii]->filter(data_in[ii], data_out[ii]);
    }
  }

  /**
   * @brief Reset the filters
   *
   */
  void reset() {
    for (auto& _filter : _filters) {
      _filter->reset();
    }
  }

  /**
   * @brief Set the filter sizes.
   *
   * @param size
   */
  virtual void set_filter_size(const int size) {
    for (auto& _filter : _filters) {
      _filter->set_size(size);
    }
  };

 private:
  // VARIABLES *****************************************************************
  std::array<std::unique_ptr<Filter<T>>, N>
      _filters;  ///< Array of pts to the controller filters
};

#endif