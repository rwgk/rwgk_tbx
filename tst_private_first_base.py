from __future__ import absolute_import, division, print_function

import rwgk_tbx.private_first_base as m
import sys


def test_make_drvd_pass_base():
    d = m.make_drvd()
    i = m.pass_base(d)
    assert i == 200


def test_make_drvd_up_cast_pass_drvd():
    b = m.make_drvd_up_cast()
    # the base return is down-cast immediately.
    assert b.__class__.__name__ == "drvd"
    i = m.pass_drvd(b)
    assert i == 200


def run(args):
  assert not args
  test_make_drvd_pass_base()
  test_make_drvd_up_cast_pass_drvd()
  print("Done.")


if (__name__ == "__main__"):
  run(sys.argv[1:])
