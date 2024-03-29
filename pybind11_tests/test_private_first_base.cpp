// THIS ACTUALLY WORKS:
// `py::multiple_inheritance()` was missing before.
// https://github.com/pybind/pybind11/pull/2672#issuecomment-748543648

#include "pybind11_tests.h"

namespace pybind11_tests {
namespace private_first_base {

struct base {
  base() : base_id(100) {}
  virtual ~base() = default;
  virtual int id() const { return base_id; }
  base(const base&) = delete;
  int base_id;
};

struct private_first_base {  // Any class with a virtual function will do.
  virtual void some_other_virtual_function() const {}
  virtual ~private_first_base() = default;
};

struct drvd : private private_first_base, public base {
  int id() const override { return 2 * base_id; }
};

inline drvd* make_drvd() { return new drvd; }
inline base* make_drvd_up_cast() { return new drvd; }

inline int pass_base(const base* b) { return b->id(); }
inline int pass_drvd(const drvd* d) { return d->id(); }

TEST_SUBMODULE(private_first_base, m) {
  py::class_<base>(m, "base");
  py::class_<drvd, base>(m, "drvd", py::multiple_inheritance());

  m.def("make_drvd", make_drvd,
        py::return_value_policy::take_ownership);
  m.def("make_drvd_up_cast", make_drvd_up_cast,
        py::return_value_policy::take_ownership);
  m.def("pass_base", pass_base);
  m.def("pass_drvd", pass_drvd);
}

}  // namespace private_first_base
}  // namespace pybind11_tests
