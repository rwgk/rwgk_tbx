from __future__ import absolute_import, division, print_function

import rwgk_tbx.shared_ptr_use_count as m
import sys


def test_default():
  owner = m.owner_pointee_default()
  assert owner.use_count() == 1
  # Cannot return shared_ptr:
  try:
    owner.get()
  except TypeError:
    pass
  else:
    raise RuntimeError("TypeError not raised.")
  p0 = m.pointee_default()
  # But passing via shared_ptr does work:
  assert m.default_use_count(p0) == 2
  owner.set(p0)
  assert owner.use_count() == 1  # Not 2.
  assert m.default_use_count(p0) == 2  # Not 3.


def test_sharedp():
  owner = m.owner_pointee_sharedp()
  assert owner.use_count() == 1
  p1 = owner.get()
  assert owner.use_count() == 2
  assert m.sharedp_use_count(p1) == 2
  p2 = owner.get()
  assert owner.use_count() == 3
  assert m.sharedp_use_count(p1) == 2  # Not 4.
  assert m.sharedp_use_count(p2) == 2  # Not 4.
  assert id(p1) != id(p2)  # Not the same PyObject.
  p1pt = m.sharedp_pass_through(p1)
  assert id(p1pt) == id(p1)  # Same PyObject.
  p0 = m.pointee_sharedp()
  assert m.sharedp_use_count(p0) == 2
  owner.set(p0)
  assert owner.use_count() == 1  # Not 2.
  assert m.sharedp_use_count(p0) == 2  # Not 3.


def run(args):
  assert not args
  test_default()
  test_sharedp()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
