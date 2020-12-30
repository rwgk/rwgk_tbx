#include <boost/python/call_method.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/module.hpp>
#include <boost/python/return_value_policy.hpp>

#include <iostream>
#include <memory>
#include <string>

namespace {

namespace py = boost::python;

inline void to_cout(std::string msg) { std::cout << msg << std::endl; }

struct base {
  virtual void show() const {
    to_cout("base::show");
  }

  virtual std::shared_ptr<base> clone() const {
    to_cout("base::clone");
    return std::shared_ptr<base>(new base);
  }

  virtual ~base() = default;
};

struct base_callback : public base {
  PyObject* self;
  base_callback(PyObject* self_) : self(self_) {}
  base_callback(PyObject* self_, const base&) : self(self_) {}

  void show() const override {
    return py::call_method<void>(self, "show");
  }
  static void default_show(std::shared_ptr<base> b) {
    b->base::show();
  }

  std::shared_ptr<base> clone() const override {
    return py::call_method<std::shared_ptr<base>>(self, "clone");
  }
  static std::shared_ptr<base> default_clone(std::shared_ptr<base> b) {
    return b->base::clone();
  }
};

inline void show(std::shared_ptr<base> b) { b->show(); }

inline void clone_show(std::shared_ptr<base> b) {
  to_cout("cpp b->clone()");
  std::shared_ptr<base> c = b->clone();
  to_cout("cpp c->show()");
  c->show();
}

}

BOOST_PYTHON_MODULE(rwgk_tbx_cpp_base_py_derived_ext)
{
  namespace py = boost::python;

  py::def("to_cout", to_cout);

  py::class_<base, std::shared_ptr<base_callback>>("base")
    .def(py::init<>())
    .def("show", &base_callback::default_show)
    .def("clone", &base_callback::default_clone)
  ;

  py::def("show", show);
  py::def("clone_show", clone_show);
}
