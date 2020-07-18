package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("//:build/workspace.bzl", "RELEASE_VERSION")

# build package of runtime.
pkg_tar(
    name = "jsr-" + RELEASE_VERSION,
    extension = "tar.gz",
    srcs = [
        ":jsr",
        ":startup_binaries",
        "//:build/data/config.yaml",
    ],
    mode = "0755",
)

filegroup(
    name = "startup_binaries",
    srcs = [
        "//:build/data/natives_blob.bin",
        "//:build/data/snapshot_blob.bin",
    ],
)

# runtime binary.
cc_binary(
    name = "jsr",
    srcs = [
        "src/app.cc",
    ],
    linkstatic = True,
    copts = [
        "-std=c++11",
        "-I."
    ],
    linkopts = ["-lpthread -lrt"],
    deps = [
        ":main",
    ],
)

cc_library(
    name = "main",
    hdrs = [":headers"],
    # includes = [
    #     ".",
    # ],
    copts = [
        "-std=c++11",
        "-I.",
    ],
    srcs = [
        ":base",
        ":config",
        ":instance",
        ":sysfunc",
        ":server",
    ],
    deps = [
        ":runtime_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@jsr_deps//:v8",
        "@jsr_deps//:spdlog",
        "@jsr_deps//:yaml",
    ],
)

filegroup(
    name = "headers",
    srcs = [
        "include/base.h",
        "include/config.h",
        "include/server.h",
        "include/instance.h",
        "include/sysfunc.h",
    ],
)

filegroup(
    name = "base",
    srcs = [
        "src/base/module.cc",
        "src/base/log.cc",
        "src/base/thread_pool.cc",
        "src/base/util.cc",
    ],
)

filegroup(
    name = "config",
    srcs = [
        "src/config/base_config.cc",
        "src/config/config.cc",
        "src/config/server_config.cc",
        "src/config/log_config.cc",
        "src/config/instance_config.cc",
        "src/config/sysfunc_config.cc",
    ],
)

filegroup(
    name = "instance",
    srcs = [
        "src/instance/context.cc",
        "src/instance/request.cc",
        "src/instance/response.cc",
        "src/instance/instance.cc",
        "src/instance/manager.cc",
        "src/instance/util.cc",
    ],
)

filegroup(
    name = "sysfunc",
    srcs = [
        "src/sysfunc/require.cc",
        "src/sysfunc/console.cc",
        "src/sysfunc/http.cc",
        "src/sysfunc/manager.cc",
    ],
)

filegroup(
    name = "server",
    srcs = [
        "src/server/context.cc",
        "src/server/server.cc",
    ],
)

proto_library(
    name = "runtime_proto",
    srcs = ["build/protos/runtime.proto"],
    deps = ["@com_google_protobuf//:any_proto"],
)

cc_proto_library(
    name = "runtime_proto_lib",
    deps = [":runtime_proto"],
)

cc_grpc_library(
    name = "runtime_grpc",
    srcs = [":runtime_proto"],
    grpc_only = True,
    deps = [":runtime_proto_lib"],
)

exports_files(
    [
        "build/tools/build_v8.sh",
        "build/data/natives_blob.bin",
        "build/data/snapshot_blob.bin",
        "build/data/config.yaml",
        "build/protos/runtime.proto",

    ],
    visibility = ["//visibility:public"],
)

# for testing.
cc_library(
    name = "client",
    srcs = [
        "include/test.h",
        "test/client/client.cc",
    ],
    copts = [
        "-std=c++11",
        "-I.",
    ],
    deps = [
        ":runtime_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)