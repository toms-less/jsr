RELEASE_VERSION = "0.0.1"

def jsr_dependencies():
    native.new_local_repository(
        name = "jsr_deps",
        path = "deps",
        build_file = "deps/deps.BUILD",
    )