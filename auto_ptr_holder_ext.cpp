#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <memory>

namespace {

struct pointee_autoptr {
  pointee_autoptr() : id{130} {}
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
inline std::auto_ptr<T> pass_through(std::auto_ptr<T> aoptr) {
  return aoptr;
}

}  // namespace

BOOST_PYTHON_MODULE(rwgk_tbx_auto_ptr_holder_ext)
{
  namespace py = boost::python;

  py::class_<pointee_autoptr,
             std::auto_ptr<pointee_autoptr>,
             boost::noncopyable>("pointee_autoptr")
    .def(py::init<>())
    .def("get_id", &pointee_autoptr::get_id);
  py::class_<owner<pointee_autoptr>,
             boost::noncopyable>("owner_pointee_autoptr")
    .def(py::init<>())
    .def("get", &owner<pointee_autoptr>::get)
    .def("set", &owner<pointee_autoptr>::set)
    .def("is_owning", &owner<pointee_autoptr>::is_owning);
  ;
  py::def("is_owning", is_owning<pointee_autoptr>);
  py::def("autoptr_pass_through", pass_through<pointee_autoptr>);
}
