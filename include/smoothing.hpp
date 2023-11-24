#ifndef SMOOTHING_HPP
#define SMOOTHING_HPP

template <typename T>
class Smoother {
   public:
    virtual void smooth(const T data_in, T& data_out) = 0;
};

#endif