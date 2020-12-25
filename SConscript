import libtbx.load_env
Import("env_etc", "env_no_includes_boost_python_ext")
env = env_no_includes_boost_python_ext.Clone()
env_etc.enable_more_warnings(env=env)
env_etc.include_registry.append(
  env=env,
  paths=[
    env_etc.libtbx_include,
    env_etc.boost_adaptbx_include,
    env_etc.boost_include,
    env_etc.python_include])
env.SharedLibrary(
  target="#lib/rwgk_tbx_private_first_base_ext",
  source="private_first_base_ext.cpp")
env.SharedLibrary(
  target="#lib/rwgk_tbx_shared_ptr_use_count_ext",
  source="shared_ptr_use_count_ext.cpp")
env.SharedLibrary(
  target="#lib/rwgk_tbx_smart_ptr_private_first_base_ext",
  source="smart_ptr_private_first_base_ext.cpp")
