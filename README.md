# Experimental stuff using cctbx_project/libtbx build tools.

## Example bootstrap
```
cd clone  # Any name.
git clone https://github.com/SCons/scons.git
git clone https://github.com/cctbx/boost.git
git clone https://github.com/cctbx/cctbx_project.git
git clone https://github.com/rwgk/rwgk_tbx.git
mkdir bintbx  # Any other name.
cd bintbx
python3 ../cctbx_project/libtbx/configure.py --compiler=clang rwgk_tbx
source setpaths.sh
make
libtbx.python ../rwgk_tbx/tst_smart_ptr_private_first_base.py
```
