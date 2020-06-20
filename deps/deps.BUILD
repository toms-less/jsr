package(default_visibility = ["//visibility:public"])

# As it is a long time(download, compile, etc.) to build v8,
# current build file support to choose a way to load v8 static libraries.
# 1. compile with v8 sources by default;
# 2. use a local compiled libraries with a bazel option '--define use_local_v8_library=true'.
#
# If using local v8 libraries, you should put them into 'path/to/deps/libs/v8' directory.
config_setting(
    name = "use_local_v8_library",
    values = {
        "define":"use_local_v8_library=true"
    },
    visibility = ["//visibility:public"],
)

cc_library(
    name = "v8",
    hdrs = [":v8_include"],
    includes = [
        "v8/include",
        "v8/include/libplatform",
    ],
    srcs = [] + select({
        ":use_local_v8_library" : [":local_v8_libraries"],
        "//conditions:default" : [":build_v8_libraries"],
    }),
    linkstatic = 1,
    visibility = ["//visibility:public"],
)

filegroup(
    name = "v8_include",
    srcs = glob([
        "v8/include/*",
        "v8/include/**/*",
    ]),
)

# local v8 libraries.
filegroup(
    name ="local_v8_libraries",
    srcs = [
        ":libs/v8/libv8_base.a",
        ":libs/v8/libv8_libplatform.a",
        ":libs/v8/libv8_inspector.a",
        ":libs/v8/libv8_external_snapshot.a",
        ":libs/v8/libv8_libsampler.a",
        ":libs/v8/libv8_libbase.a",
        ":libs/v8/libtorque_generated_definitions.a",
        ":libs/v8/libicui18n.a",
        ":libs/v8/libicuuc.a",
        ":libs/v8/libencoding.a",
        ":libs/v8/libbindings.a",
    ],
)

# build v8 libraries. 'local' attribute is necessary
# as the 'build_v8.sh' will modify home direcory for a lock 
# when sync v8 dependences. If 'local' is not 1,
# this rule will occur error 'read-only file system'
# since the sandbox using os namespace. 
genrule(
    name = "build_v8_libraries",
    message = "build v8.",
    srcs = glob(["**/v8/*"]),
    outs = [
        "libv8_base.a",
        "libv8_libplatform.a",
        "libv8_inspector.a",
        "libv8_external_snapshot.a",
        "libv8_libsampler.a",
        "libv8_libbase.a",
        "libtorque_generated_definitions.a",
        "libicui18n.a",
        "libicuuc.a",
        "libencoding.a",
        "libbindings.a",
    ],
    cmd = "$(location :build_v8_sh) x64.release i18nsupport=on $(OUTS)",
    tools = [
        ":build_v8_sh",
    ],
    local = 1,
)

sh_binary(
    name = "build_v8_sh",
    srcs = ["@jsr//:build/tools/build_v8.sh"],
)

# spdlog.
cc_library(
    name = "spdlog",
    includes = [
        "spdlog/include",
    ],
    defines = ["SPDLOG_COMPILED_LIB"],
    hdrs = glob([
        "spdlog/include/spdlog/*.h",
        "spdlog/include/spdlog/**/*.h",
    ]),
    srcs = glob([
        "spdlog/include/spdlog/*.h",
        "spdlog/include/spdlog/**/*.h",
        "spdlog/src/*.cpp",
    ]),
    visibility = ["//visibility:public"],
)

# yaml.
cc_library(
    name = "yaml",
    includes = [
        "yaml/include",
    ],
    hdrs = glob([
        "yaml/include/yaml-cpp/*.h",
        "yaml/include/yaml-cpp/**/*.h",
        "yaml/src/*.h",
        "yaml/src/**/*.h",
    ]),
    srcs = glob([
        "yaml/include/yaml-cpp/*.h",
        "yaml/include/yaml-cpp/**/*.h",
        "yaml/src/*.h",
        "yaml/src/**/*.h",
        "yaml/src/*.cpp", 
        "yaml/src/**/*.cpp", 
    ]),
    visibility = ["//visibility:public"],
)