#include <boost/python/call_method.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <memory>

namespace {

namespace py = boost::python;

struct base {
  base() : base_num(100) {}

  virtual int get_num() const { return base_num; }

  virtual std::shared_ptr<base> clone() const {
    return std::shared_ptr<base>(new base(150));
  }

  virtual ~base() = default;

 private:
  explicit base(int num) : base_num(num) {}
  int base_num;
};

inline int get_num(std::shared_ptr<base> b) { return b->get_num(); }

inline int clone_get_num(std::shared_ptr<base> b) {
  std::shared_ptr<base> c = b->clone();
  return (b->get_num() + 3) * 1000 + (c->get_num() + 7);
}

struct base_callback : public base {
  PyObject* self;
  base_callback(PyObject* self_) : self(self_) {}
  base_callback(PyObject* self_, const base&) : self(self_) {}

  int get_num() const override {
    return py::call_method<int>(self, "get_num");
  }
  static int default_get_num(std::shared_ptr<base> b) {
    return b->base::get_num();
  }

  std::shared_ptr<base> clone() const override {
    return py::call_method<std::shared_ptr<base>>(self, "clone");
  }
  static std::shared_ptr<base> default_clone(std::shared_ptr<base> b) {
    return b->base::clone();
  }
};

}

BOOST_PYTHON_MODULE(rwgk_tbx_cpp_base_py_derived_ext)
{
  namespace py = boost::python;

  py::class_<base, std::shared_ptr<base_callback>>("base")
    .def(py::init<>())
    .def("get_num", &base_callback::default_get_num)
    .def("clone", &base_callback::default_clone)
  ;

  py::def("get_num", get_num);
  py::def("clone_get_num", clone_get_num);
}
