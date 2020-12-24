// Boost.Python equivalent of test_private_first_base under
// pybind11 PR #2672:
// https://github.com/pybind/pybind11/pull/2672

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/module.hpp>
#include <boost/python/return_value_policy.hpp>

namespace {

struct base {
  base() : base_id(100) {}
  virtual ~base() = default;
  virtual int id() const { return base_id; }
  base(const base&) = default;
  int base_id;
};

struct private_first_base {  // Any class with a virtual function will do.
  private_first_base() {}
  virtual void some_other_virtual_function() const {}
  virtual ~private_first_base() = default;
  private_first_base(const private_first_base&) = default;
};

struct drvd : private private_first_base, public base {
  drvd() {}
  int id() const override { return 2 * base_id; }
};

inline drvd* make_drvd() { return new drvd; }
inline base* make_drvd_up_cast() { return new drvd; }

inline int pass_base(const base* b) { return b->id(); }
inline int pass_drvd(const drvd* d) { return d->id(); }

}  // namespace

BOOST_PYTHON_MODULE(rwgk_tbx_private_first_base_ext)
{
  namespace py = boost::python;

  py::class_<base>("base");
  py::class_<drvd, py::bases<base>>("drvd");

  py::def("make_drvd", make_drvd,
          py::return_value_policy<py::manage_new_object>());
  py::def("make_drvd_up_cast", make_drvd_up_cast,
          py::return_value_policy<py::manage_new_object>());
  py::def("pass_base", pass_base);
  py::def("pass_drvd", pass_drvd);
}
