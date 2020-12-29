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

template <typename T, template <typename> class SmartPointer = std::auto_ptr>
struct owner {
  SmartPointer<T> smptr;
  owner() : smptr(new T) {}
  SmartPointer<T> get() { return smptr; }
  void set(SmartPointer<T> other) { smptr = other; }
  bool is_owning() const { return smptr.get() != nullptr; }
};

template <typename T>
inline bool is_owning(const std::auto_ptr<T>& smptr) {
  return smptr.get() != nullptr;
}

template <typename T>
inline std::auto_ptr<T> auto_pass_through(std::auto_ptr<T> smptr) {
  return smptr;
}

template <typename T>
inline std::shared_ptr<T> shared_pass_through(std::shared_ptr<T> shptr) {
  return shptr;
}

struct base {};
struct drvd : base {};

inline std::auto_ptr<drvd> make_drvd() { return std::auto_ptr<drvd>(new drvd); }
inline bool pass_base_raw_ptr(const base* b) { return b != nullptr; }
inline bool pass_drvd_raw_ptr(const drvd* d) { return d != nullptr; }
inline bool pass_base_auto_ptr(std::auto_ptr<base> b) { return b.get() != nullptr; }
inline bool pass_drvd_auto_ptr(std::auto_ptr<drvd> d) { return d.get() != nullptr; }

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
  py::class_<owner<pointee<130>,
                   std::shared_ptr>>("owner_shared_pointee_autoptr")
    .def(py::init<>())
    .def("get", &owner<pointee<130>, std::shared_ptr>::get)
    .def("set", &owner<pointee<130>, std::shared_ptr>::set)
    .def("is_owning", &owner<pointee<130>, std::shared_ptr>::is_owning);
  ;

  py::class_<base, std::auto_ptr<base>, boost::noncopyable>("base");
  py::class_<drvd, std::auto_ptr<drvd>, py::bases<base>, boost::noncopyable>("drvd");
  py::def("make_drvd", make_drvd);
  py::def("pass_base_raw_ptr", pass_base_raw_ptr);
  py::def("pass_drvd_raw_ptr", pass_drvd_raw_ptr);
  py::def("pass_base_auto_ptr", pass_base_auto_ptr);
  py::def("pass_drvd_auto_ptr", pass_drvd_auto_ptr);
}
