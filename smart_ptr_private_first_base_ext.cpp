// Boost.Python equivalent of test_smart_ptr_private_first_base under
// pybind11 PR #2672:
// https://github.com/pybind/pybind11/pull/2672#issuecomment-748543648

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <memory>

namespace {

struct base {
  base() : base_id(100) {}
  virtual ~base() = default;
  virtual int id() const { return base_id; }
  int base_id;
};

struct private_first_base {  // Any class with a virtual function will do.
  virtual void some_other_virtual_function() const {}
  virtual ~private_first_base() = default;
};

struct drvd : private private_first_base, public base {
  int id() const override { return 2 * base_id; }
};

inline std::shared_ptr<drvd> make_shared_drvd() {
  return std::shared_ptr<drvd>(new drvd);
}

inline int pass_shared_base(std::shared_ptr<base> b) { return b->id(); }

}  // namespace

BOOST_PYTHON_MODULE(rwgk_tbx_smart_ptr_private_first_base_ext)
{
  namespace py = boost::python;

  py::class_<base, std::shared_ptr<base>>("base");
  py::class_<drvd, py::bases<base>, std::shared_ptr<drvd>>("drvd");

  py::def("make_shared_drvd", make_shared_drvd);
  py::def("pass_shared_base", pass_shared_base);
}
