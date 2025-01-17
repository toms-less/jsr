package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")
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
    linkopts = ["-lpthread -lrt -lcrypto -lssl"],
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
        ":module",
        ":binding",
        ":server",
    ],
    deps = [
        ":curl",
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
        "include/module.h",
        "include/binding.h",
    ],
)

filegroup(
    name = "base",
    srcs = [
        "src/base/http/client.cc",
        "src/base/http/cookie.cc",
        "src/base/http/entry.cc",
        "src/base/http/header.cc",
        "src/base/log.cc",
        "src/base/thread_pool.cc",
        "src/base/util.cc",
    ],
)

filegroup(
    name = "config",
    srcs = [
        "src/config/config.cc",
        "src/config/server_config.cc",
        "src/config/log_config.cc",
        "src/config/instance_config.cc",
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
    name = "module",
    srcs = [
        "src/module/module.cc",
        "src/module/context.cc",
        "src/module/parser.cc",
    ],
)

filegroup(
    name = "binding",
    srcs = [
        "src/binding/binding.cc",
        "src/binding/console.cc",
        "src/binding/http.cc",
        "src/binding/manager.cc",
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
cc_binary(
    name = "tests",
    srcs = [
        "test/common/tests.cc",
    ],
    linkstatic = True,
    copts = [
        "-std=c++11",
        "-I."
    ],
    deps = [
        ":client",
        ":main",
    ],
)

cc_library(
    name = "client",
    hdrs = ["include/test.h"],
    srcs = [
        "test/common/client.cc",
        "test/common/context.cc",
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

go_binary(
    name = "module-server",
    embed = [":libcode"],
)

go_library(
    name = "libcode",
    srcs = [
        "test/common/modules/server.go",
    ],
    importpath = "modules",
    deps = [
        ":handlers"
    ],
)

go_library(
    name = "handlers",
    srcs = [
        "test/common/modules/handler/dispatcher.go",
        "test/common/modules/handler/simple.go",
    ],
    importpath = "modules/handler",
)

#-------curl dependency-------
# curl version 'curl-7_70_0'.
cc_library(
    name = "curl",
    hdrs = glob([
        "deps/curl/include/curl/*h",
    ]),
    includes = [
        "deps/curl/include/curl",
    ],
    srcs = [
        ":build_curl",
    ],
    linkstatic = 1,
)

genrule(
    name = "build_curl",
    srcs = glob([
        "deps/curl/*",
        "deps/curl/**/**",
    ]),
    outs = [
        "libcurl.a",
    ],
    cmd = "$(location :curl_build_sh) $(OUTS)",
    tools = [":curl_build_sh"],
    output_to_bindir = 1,
)

sh_binary(
    name = "curl_build_sh",
    srcs = [
        "build/tools/build_curl.sh",
    ],
)