from __future__ import absolute_import, division, print_function

import rwgk_tbx.cpp_base_py_derived as m
import sys


class drvd(m.base):

  def __init__(self):
    super().__init__()

  def show(self):
    m.to_cout("drvd.show")

  def clone(self):
    m.to_cout("drvd.clone")
    return drvd()


def test_main():
  b = m.base()
  b.show()
  m.show(b)
  #b.clone()
  #m.clone_show(b)

  d = drvd()
  d.show()
  m.show(d)
  d.clone()
  m.clone_show(d)


def run(args):
  assert not args
  test_main()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
