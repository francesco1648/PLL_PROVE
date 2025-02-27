#line 1 "C:\\Users\\Titania\\Desktop\\isaac\\picolowlevel_x_vs\\PicoLowLevel\\lib\\TractionEncoder\\src\\Filter.h"
#ifndef FILTER_H
#define FILTER_H

/**
 * Interface for a generic filter, with parameterized type.
 * @tparam T type of the variable to be filtered.
 */
template<typename T>
class Filter {
  public:
    /**
     * Virtual filter function, will be replaced by the one from the chosen filter.
     * @param value to be filtered. 
     * @return T filtered output.
     */
    virtual T filter(T value) = 0;
};

#endif
