from __future__ import absolute_import, division, print_function

import rwgk_tbx.smart_ptr_private_first_base as m
import sys

def run(args):
  assert not args
  d = m.make_shared_drvd()
  i = m.pass_shared_base(d)
  assert i == 200
  print("Done.")

if (__name__ == "__main__"):
  run(sys.argv[1:])
