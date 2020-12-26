from __future__ import absolute_import, division, print_function

import rwgk_tbx.auto_ptr_holder as m
import sys


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
  p0spt = m.autoptr_shared_pass_through(p0pt)
  # Passing to shared_ptr works but is not disowning, it is the same object:
  assert m.autoptr_is_owning(p0pt)
  assert id(p0spt) == id(p0pt)
  p0sptpt = m.autoptr_auto_pass_through(p0spt)
  assert not m.autoptr_is_owning(p0spt)
  assert m.autoptr_is_owning(p0sptpt)


def run(args):
  assert not args
  test_default()
  test_autoptr()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
