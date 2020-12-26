#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <memory>

namespace {

template <int Id>
struct pointee {
  pointee() : id{Id} {}
  int get_id() const { return id; }
  int id;
};

template <typename T>
struct owner {
  std::auto_ptr<T> aoptr;
  owner() : aoptr(new T) {}
  std::auto_ptr<T> get() { return aoptr; }
  void set(std::auto_ptr<T> other) { aoptr = other; }
  bool is_owning() const { return aoptr.get() != nullptr; }
};

template <typename T>
inline bool is_owning(const std::auto_ptr<T>& aoptr) {
  return aoptr.get() != nullptr;
}

template <typename T>
inline std::auto_ptr<T> auto_pass_through(std::auto_ptr<T> aoptr) {
  return aoptr;
}

template <typename T>
inline std::shared_ptr<T> shared_pass_through(std::shared_ptr<T> shptr) {
  return shptr;
}

}  // namespace

BOOST_PYTHON_MODULE(rwgk_tbx_auto_ptr_holder_ext)
{
  namespace py = boost::python;

  py::class_<pointee<120>>("pointee_default")
    .def(py::init<>())
    .def("get_id", &pointee<120>::get_id);
  py::class_<owner<pointee<120>>,
             boost::noncopyable>("owner_pointee_default")
    .def(py::init<>())
    .def("get", &owner<pointee<120>>::get)
    .def("is_owning", &owner<pointee<120>>::is_owning);
  ;
  py::def("default_auto_pass_through", auto_pass_through<pointee<120>>);

  py::class_<pointee<130>,
             std::auto_ptr<pointee<130>>,
             boost::noncopyable>("pointee_autoptr")
    .def(py::init<>())
    .def("get_id", &pointee<130>::get_id);
  py::class_<owner<pointee<130>>,
             boost::noncopyable>("owner_pointee_autoptr")
    .def(py::init<>())
    .def("get", &owner<pointee<130>>::get)
    .def("set", &owner<pointee<130>>::set)
    .def("is_owning", &owner<pointee<130>>::is_owning);
  ;
  py::def("autoptr_is_owning", is_owning<pointee<130>>);
  py::def("autoptr_auto_pass_through", auto_pass_through<pointee<130>>);
  py::def("autoptr_shared_pass_through", shared_pass_through<pointee<130>>);
}
