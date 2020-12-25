#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <memory>

namespace {

struct pointee_default {};
struct pointee_sharedp {};

template <typename T>
struct owner {
  std::shared_ptr<T> shptr;
  owner() : shptr(new T) {}
  std::shared_ptr<T> get() const { return shptr; }
  void set(std::shared_ptr<T> other) { shptr = other; }
  std::size_t use_count() const { return shptr.use_count(); }
};

template <typename T>
inline std::size_t use_count(std::shared_ptr<T> shptr) {
  return shptr.use_count();
}

template <typename T>
inline std::shared_ptr<T> pass_through(std::shared_ptr<T> shptr) {
  return shptr;
}

}  // namespace

BOOST_PYTHON_MODULE(rwgk_tbx_shared_ptr_use_count_ext)
{
  namespace py = boost::python;

  py::class_<pointee_default>("pointee_default")
    .def(py::init<>());
  py::class_<owner<pointee_default>>("owner_pointee_default")
    .def("get", &owner<pointee_default>::get)
    .def("set", &owner<pointee_default>::set)
    .def("use_count", &owner<pointee_default>::use_count);
  py::def("default_use_count", use_count<pointee_default>);

  py::class_<pointee_sharedp,
             std::shared_ptr<pointee_sharedp>>("pointee_sharedp")
    .def(py::init<>());
  py::class_<owner<pointee_sharedp>>("owner_pointee_sharedp")
    .def(py::init<>())
    .def("get", &owner<pointee_sharedp>::get)
    .def("set", &owner<pointee_sharedp>::set)
    .def("use_count", &owner<pointee_sharedp>::use_count);
  py::def("sharedp_use_count", use_count<pointee_sharedp>);
  py::def("sharedp_pass_through", pass_through<pointee_sharedp>);
}
