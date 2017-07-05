#pragma once

#include <functional>
#include <unordered_map>
#include <cairo/cairo.h>

#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace mpl_cairo {

namespace py = pybind11;

class PatternCache {
  struct CacheKey {
    py::object path;
    cairo_matrix_t matrix;
    void (*draw_func)(cairo_t*);
  };
  struct Hash {
    size_t operator()(CacheKey const&) const;
  };
  struct EqualTo {
    bool operator()(CacheKey const&, CacheKey const&) const;
  };

  struct CacheEntry {
    double x0, y0, width, height;
    std::unique_ptr<cairo_pattern_t*[]> patterns;
  };

  size_t n_subpix_;
  std::unordered_map<CacheKey, CacheEntry, Hash, EqualTo> cache_;

  public:
  PatternCache(double threshold);
  ~PatternCache();
  void mask(cairo_t* cr, CacheKey key, double x, double y);
};

}