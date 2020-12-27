from __future__ import absolute_import, division, print_function

import rwgk_tbx.auto_ptr_holder as m
import gc
import sys
import weakref


def test_default():
  owner = m.owner_pointee_default()
  assert owner.is_owning()
  try:
    owner.get()
  except TypeError as e:
    assert str(e).startswith(
        "No to_python (by-value) converter found for C++ type: ")
  else:
    raise RuntimeError("TypeError not raised.")
  p0 = m.pointee_default()
  assert p0.get_id() == 120
  try:
    m.default_auto_pass_through(p0)
  except Exception as e:
    assert e.__class__.__name__ == "ArgumentError"
    assert str(e).startswith("Python argument types in")
  else:
    raise RuntimeError("ArgumentError not raised.")


def test_autoptr():
  owner = m.owner_pointee_autoptr()
  assert owner.is_owning()
  p1 = owner.get()
  assert not owner.is_owning()
  assert m.autoptr_is_owning(p1)
  assert m.autoptr_is_owning(p1)  # To show that the previous call is not disowning.
  assert p1.get_id() == 130
  p1pt = m.autoptr_auto_pass_through(p1)
  assert not m.autoptr_is_owning(p1)
  assert m.autoptr_is_owning(p1pt)
  p2 = owner.get()
  assert p2 is None
  try:
    p1.get_id()
  except Exception as e:
    # The error message for the disowned object is not very helpful.
    assert e.__class__.__name__ == "ArgumentError"
    assert str(e).startswith("Python argument types in")
  else:
    raise RuntimeError("ArgumentError not raised.")
  p0 = m.pointee_autoptr()
  assert m.autoptr_is_owning(p0)
  p0pt = m.autoptr_auto_pass_through(p0)
  assert not m.autoptr_is_owning(p0)
  assert p0pt.get_id() == 130


def test_autoptr_shared():
  p0 = m.pointee_autoptr()
  # Passing to shared_ptr works but is not disowning, it is the same object:
  p0spt = m.autoptr_shared_pass_through(p0)
  assert id(p0spt) == id(p0)
  assert m.autoptr_is_owning(p0)
  p0sptpt = m.autoptr_auto_pass_through(p0spt)
  assert not m.autoptr_is_owning(p0spt)
  assert m.autoptr_is_owning(p0sptpt)
  owner_shared = m.owner_shared_pointee_autoptr()
  try:
    owner_shared.get()
  except TypeError as e:
    # Note: this failure means the autoptr_shared_pass_through return above
    # only works because the return is identical to the argument.
    assert str(e).startswith(
        "No to_python (by-value) converter found for C++ type: ")
  else:
    raise RuntimeError("TypeError not raised.")
  owner_shared.set(p0sptpt)
  assert m.autoptr_is_owning(p0sptpt)
  assert owner_shared.is_owning()
  # owner_shared is keeping the "actually owning" PyObject alive:
  w = weakref.ref(p0sptpt)
  del p0sptpt
  gc.collect()
  assert w() is not None
  del owner_shared
  gc.collect()
  assert w() is None


def run(args):
  assert not args
  test_default()
  test_autoptr()
  test_autoptr_shared()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
