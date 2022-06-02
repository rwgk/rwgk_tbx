import threading
import time

from pybind11_tests import shared_ptr_deleter_gil_acquire as m


def create_threads():
    # Separate function to have all local variables automatically deleted.
    shp_owner = m.shared_ptr_owner()
    threads = []
    for rider_info in (
        (shp_owner, "Ant", 4, 200),
        (shp_owner, "Bug", 12, 100),
        (shp_owner, "Cat", 2, 300),
    ):
        t = threading.Thread(target=m.merry_go_round, args=rider_info)
        threads.append(t)
    return threads


def test_merry_go_round():
    print(flush=True)
    t0 = time.time()
    threads = create_threads()
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    print("All done: %.3f" % (time.time() - t0), flush=True)
