#!/usr/bin/env bash

if [ $# -le 2 ]; then
    echo "_____V8 build tool shell should input more than 2 parameters, 'BUILD_ARCH_TYPE', 'V8_BUILD_OPTIONS', 'BAZEL_OUTPUT_FILES', current parameters count is ''"`echo $#`"'', please check it."
    exit 1
fi

BUILD_ARCH_TYPE=$1
V8_BUILD_OPTIONS=$2

CURRENT_ROOT_HOME=`pwd`
DEPS_HOME=$CURRENT_ROOT_HOME/external/toms_js_deps
V8_HOME=$DEPS_HOME/v8
cd `echo $DEPS_HOME`

# initialize the temporary git for build if not exist.
cd `echo $V8_HOME`
if [ ! -d ".git" ]; then
    git init
    git commit --allow-empty -m init
    echo "_____Initializing v8 temporary git repository '.git'."
fi
cd `echo $DEPS_HOME`

echo "_____Clone 'depot_tools' from 'https://chromium.googlesource.com/chromium/tools/depot_tools.git' to current path "`pwd`

git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
if [ ! -d "depot_tools" ]; then
    echo "_____Clone 'depot_tools' error, 'depot_tools' directory is not found."
    exit 1
fi

# set build tool path
export PATH=$DEPS_HOME/depot_tools:$PATH
echo "_____Set 'depot_tools' path to system 'PATH' variable, current PATH "`echo $PATH`

# sync v8 dependences of current version.
# solutions = [
#   { "name" : "v8",
#     "url" : "https://github.com/v8/v8.git",
#     "deps_file" : "DEPS",
#     "managed" : False,
#     "custom_deps" : {
#       "v8/test/benchmarks/data" : None,
#       "v8/testing/gmock" : None,
#       "v8/test/mozilla/data" : None,
#       "v8/test/test262/data" : None,
#       "v8/test/test262/harness" : None,
#       "v8/third_party/android_ndk" : None,
#       "v8/third_party/android_sdk" : None,
#       "v8/third_party/catapult" : None,
#       "v8/third_party/colorama/src" : None,
#       "v8/third_party/fuchsia-sdk" : None,
#       "v8/third_party/instrumented_libraries" : None,
#       "v8/tools/luci-go" : None,
#       "v8/tools/swarming_client" : None,
#       "v8/third_party/qemu-linux-x64" : None,
#     },
#   },
# ]
gclient sync -v --spec 'solutions = [{ "name" : "v8","url" : "https://github.com/v8/v8.git","deps_file" : "DEPS","managed" : False,"custom_deps" : {"v8/test/benchmarks/data" : None,"v8/testing/gmock" : None,"v8/test/mozilla/data" : None,"v8/test/test262/data" : None,"v8/test/test262/harness" : None,"v8/third_party/android_ndk" : None,"v8/third_party/android_sdk" : None,"v8/third_party/catapult" : None,"v8/third_party/colorama/src" : None,"v8/third_party/fuchsia-sdk" : None,"v8/third_party/instrumented_libraries" : None,"v8/tools/luci-go" : None,"v8/tools/swarming_client" : None,"v8/third_party/qemu-linux-x64" : None,},},]'
echo "_____Synchronize v8 dependences finished."

# compile v8 sources.
cd `echo $V8_HOME`
echo "_____Entering v8 directory '"`pwd`"'."

ARCH="`arch`"
if [[ "$ARCH" == "s390x" ]] || [[ "$ARCH" == "ppc64le" ]]; then
  # set paths manually for now to use locally installed gn
  export BUILD_TOOLS=/home/iojs/build-tools
  export LD_LIBRARY_PATH=$BUILD_TOOLS:$LD_LIBRARY_PATH
  export PATH=$BUILD_TOOLS:$PATH
  if [[ X"$CXX" != X ]]; then
    CXX_PATH=`which $CXX |grep g++`
  fi
  rm -f "$BUILD_TOOLS/g++"
  rm -f "$BUILD_TOOLS/gcc"
fi
if [[ "$ARCH" == "s390x" ]]; then
  ln -s $CXX_PATH "$BUILD_TOOLS/g++"
  ln -s $CXX_PATH "$BUILD_TOOLS/gcc"
  g++ --version
  export PKG_CONFIG_PATH=$BUILD_TOOLS/pkg-config
  gn gen -v $V8_HOME/out/$BUILD_ARCH_TYPE --args='is_component_build=false is_debug=false use_goma=false goma_dir="None" use_custom_libcxx=false v8_target_cpu="s390x" target_cpu="s390x"'
  ninja -v -C $V8_HOME/out/$BUILD_ARCH_TYPE d8 cctest inspector-test
elif [[ "$ARCH" == "ppc64le" ]]; then
  if [[ X"$CXX" != X ]]; then
    ln -s /usr/bin/$CXX "$BUILD_TOOLS/g++"
    ln -s /usr/bin/$CC "$BUILD_TOOLS/gcc"
  fi
  g++ --version
  export PKG_CONFIG_PATH=$BUILD_TOOLS/pkg-config-files
  gn gen out/$BUILD_ARCH_TYPE --args='is_component_build=false is_debug=false use_goma=false goma_dir="None" use_custom_libcxx=false v8_target_cpu="ppc64" target_cpu="ppc64"'
  ninja -C out/$BUILD_ARCH_TYPE d8 cctest inspector-test
else
  echo "_____Compile v8, starting generic build file."
  gn gen out/$BUILD_ARCH_TYPE --args='is_component_build=false is_debug=false target_cpu="x64" use_goma=false goma_dir="None" v8_enable_backtrace=true v8_enable_disassembler=true v8_enable_object_print=true v8_enable_verify_heap=true use_custom_libcxx=false use_sysroot = false'
  BUILD_ARGS_FILE=$V8_HOME/out/$BUILD_ARCH_TYPE/args.gn
  if [ ! -f "$BUILD_ARGS_FILE" ]; then
    echo "_____V8 compiled error, build args file '"`echo $BUILD_ARGS_FILE`"' is not found."
    exit 1
  fi
  echo "_____Build args: "`cat $BUILD_ARGS_FILE`

  echo "_____Compile v8, starting build files."
  ninja -C out/$BUILD_ARCH_TYPE
fi

# redirect to dependence home directory.
cd `echo $DEPS_HOME`

# make v8 static libraries.
OJB_DIR_HOME=$DEPS_HOME/v8/out/$BUILD_ARCH_TYPE/obj/
if [ ! -d "$OJB_DIR_HOME" ]; then
    echo "_____V8 compiled error, target object directory ''"`echo $OJB_DIR_HOME`"'' is not found."
    exit 1
fi

TORQUE=$OJB_DIR_HOME"torque"
TORQUE_BASE=$OJB_DIR_HOME"torque_base"
TORQUE_GENERATED_DEFINITIONS=$OJB_DIR_HOME"torque_generated_definitions"
TORQUE_GENERATED_INITIALIZERS=$OJB_DIR_HOME"torque_generated_initializers"

V8_BASE_WITHOUT_COMPILER=$OJB_DIR_HOME"v8_base_without_compiler"
V8_COMPILER=$OJB_DIR_HOME"v8_compiler"

INSPECTOR=$OJB_DIR_HOME"src/inspector/inspector"
INSPECTOR_STRING=$OJB_DIR_HOME"src/inspector/inspector_string_conversions"

V8_EXTERNAL_SNAPSHOT=$OJB_DIR_HOME"v8_external_snapshot"

V8_INIT=$OJB_DIR_HOME"v8_init"
V8_INITIALIZERS=$OJB_DIR_HOME"v8_initializers"

V8_LIBBASE=$OJB_DIR_HOME"v8_libbase"
V8_LIBPLATFORM=$OJB_DIR_HOME"v8_libplatform"
V8_LIBSAMPLER=$OJB_DIR_HOME"v8_libsampler"

ICUI18N=$OJB_DIR_HOME"third_party/icu/icui18n"
ICUUC=$OJB_DIR_HOME"third_party/icu/icuuc"

BINGINGS=$OJB_DIR_HOME"third_party/inspector_protocol/bindings"
ENCODING=$OJB_DIR_HOME"third_party/inspector_protocol/encoding"

ar cr $DEPS_HOME"/libtorque.a" `echo $TORQUE"/*.o"`
ar cr $DEPS_HOME"/libtorque_base.a" `echo $TORQUE_BASE"/*.o"`
ar cr $DEPS_HOME"/libtorque_generated_definitions.a" `echo $TORQUE_GENERATED_DEFINITIONS"/*.o"`
ar cr $DEPS_HOME"/libtorque_generated_initializers.a" `echo $TORQUE_GENERATED_INITIALIZERS"/*.o"`

ar cr $DEPS_HOME"/libv8_base.a" `echo $V8_BASE_WITHOUT_COMPILER"/*.o"`
ar cr $DEPS_HOME"/libv8_base.a" `echo $V8_COMPILER"/*.o"`

ar cr $DEPS_HOME"/libv8_inspector.a" `echo $INSPECTOR"/*.o"`
ar cr $DEPS_HOME"/libv8_inspector.a" `echo $INSPECTOR_STRING"/*.o"`

ar cr $DEPS_HOME"/libv8_external_snapshot.a" `echo $V8_EXTERNAL_SNAPSHOT"/*.o"`

ar cr $DEPS_HOME"/libv8_init.a" `echo $V8_INIT"/*.o"`
ar cr $DEPS_HOME"/libv8_initializers.a" `echo $V8_INITIALIZERS"/*.o"`

ar cr $DEPS_HOME"/libv8_libbase.a" `echo $V8_LIBBASE"/*.o"`
ar cr $DEPS_HOME"/libv8_libplatform.a" `echo $V8_LIBPLATFORM"/*.o"`
ar cr $DEPS_HOME"/libv8_libsampler.a" `echo $V8_LIBSAMPLER"/*.o"`

ar cr $DEPS_HOME"/libicui18n.a" `echo $ICUI18N"/*.o"`
ar cr $DEPS_HOME"/libicuuc.a" `echo $ICUUC"/*.o"`

ar cr $DEPS_HOME"/libbindings.a" `echo $BINGINGS"/*.o"`
ar cr $DEPS_HOME"/libencoding.a" `echo $ENCODING"/*.o"`

# copy v8 static libraries to be bazel 'outs' files.
index=0
for output_library in $*
do
    if [ $index -ge 2 ]; then
        current_library=`echo $DEPS_HOME`"/"`echo $(basename $output_library)`
        if [ ! -f "$current_library" ]; then
            echo "_____Current bazel 'out' v8 library '"`echo $output_library`"' is not found."
            exit 1
        fi
        cp $current_library $CURRENT_ROOT_HOME"/"$output_library
    fi
    index=`expr $index + 1`
done
