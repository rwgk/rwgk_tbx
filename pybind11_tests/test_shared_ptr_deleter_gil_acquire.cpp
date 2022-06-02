#include "pybind11_tests.h"

#include <chrono>
#include <memory>
#include <thread>

struct shared_ptr_owner {
    std::shared_ptr<int> m_shp;

    shared_ptr_owner() : m_shp(new int(42), m_shp_deleter) {}

    static void m_shp_deleter(int *raw_ptr) {
        {
            // Any exceptions here will terminate the process.
            py::gil_scoped_acquire gil_held;
            py::print("About to: delete raw_ptr;", "flush"_a = true);
        }
        delete raw_ptr;
    }
};

TEST_SUBMODULE(shared_ptr_deleter_gil_acquire, m) {
    py::class_<shared_ptr_owner>(m, "shared_ptr_owner").def(py::init());

    m.def("merry_go_round",
          [](const shared_ptr_owner &shp_owner,
             const py::str &rider,
             unsigned num_rounds,
             unsigned cycle_ms) {
              for (unsigned idx_round = 0; idx_round < num_rounds; idx_round++) {
                  py::print(rider, idx_round, shp_owner.m_shp.use_count(), "flush"_a = true);
                  {
                      py::gil_scoped_release gil_released;
                      shared_ptr_owner sp_owner_copy(shp_owner);
                      std::this_thread::sleep_for(std::chrono::milliseconds(cycle_ms));
                  }
              }
              py::print(rider, "done.", "flush"_a = true);
          });
}
