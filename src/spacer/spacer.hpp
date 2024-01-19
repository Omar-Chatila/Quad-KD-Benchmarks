#pragma once

#include "malloc_count.h"
#include "stack_count.h"

namespace util {

/**
 * @brief Calculating space overhead
 * 
 * Calculating space overhead
 */
class spacer {
public:
    spacer() {
        space_ = malloc_count_current();
    }

    void reset() {
        space_ = malloc_count_current();
    }

    int64_t space_used() {
        return malloc_count_current() - space_;
    }

    int64_t space_peak() {
        return malloc_count_peak() - space_;
    }

private:
    double space_;
};

}