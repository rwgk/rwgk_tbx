from __future__ import absolute_import, division, print_function

import rwgk_tbx.cpp_base_py_derived as m
import sys


class drvd(m.base):

  def __init__(self, _num = 200):
    super().__init__()
    self._drvd_num = _num

  def get_num(self):
    return self._drvd_num

  def clone(self):
    return drvd(250)


def expect_type_error(callable):
  try:
    callable()
  except TypeError as e:
    assert str(e).startswith(
        "No to_python (by-value) converter found for C++ type: ")
    #   "std::shared_ptr<(anonymous namespace)::base>"
    # may be name-mangled, therefore only checking for pieces:
    assert "shared" in str(e)
    assert "base" in str(e)
  else:
    raise RuntimeError("TypeError not raised.")


def test_base():
  b = m.base()
  assert b.get_num() == 100
  m.get_num(b) == 100
  # bc = b.clone()
  # assert bc.get_num() == 150
  # assert m.clone_get_num(b) == 103157
  expect_type_error(b.clone)
  expect_type_error(lambda b=b: m.clone_get_num(b))


def test_drvd():
  d = drvd()
  assert d.get_num() == 200
  assert m.get_num(d) == 200
  dc = d.clone()
  assert dc.get_num() == 250
  assert m.clone_get_num(d) == 203257


def run(args):
  assert not args
  test_base()
  test_drvd()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
