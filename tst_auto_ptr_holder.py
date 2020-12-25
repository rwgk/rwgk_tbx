from __future__ import absolute_import, division, print_function

import rwgk_tbx.auto_ptr_holder as m
import sys


def test_autoptr():
  owner = m.owner_pointee_autoptr()
  assert owner.is_owning()
  p1 = owner.get()
  assert not owner.is_owning()
  assert m.is_owning(p1)
  assert m.is_owning(p1)  # To show that the previous call is not disowning.
  assert p1.get_id() == 130
  p1pt = m.autoptr_pass_through(p1)
  assert not m.is_owning(p1)
  assert m.is_owning(p1pt)
  p2 = owner.get()
  assert p2 is None
  try:
    p1.get_id()
  except Exception as e:
    # The error message for the disowned object is not very helpful.
    assert e.__class__.__name__ == "ArgumentError"
    assert str(e).startswith("Python argument types in")
  else:
    raise RuntimeError()
  p0 = m.pointee_autoptr()
  assert m.is_owning(p0)
  p0pt = m.autoptr_pass_through(p0)
  assert not m.is_owning(p0)
  assert p0pt.get_id() == 130


def run(args):
  assert not args
  test_autoptr()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
