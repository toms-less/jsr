## Introduction

'jsr' is a condensed name of JavaScript runtime. In this runtime, we use v8 to make it.
It is different from NodeJS. NodeJS is a standalone JavaScript language. Jsr is a distribudted
runtime of JavaScript. In the jsr, we can fragment user function code and execute them on the
distribudted runtimes. And also users do not need to care about where the code was executed.
Jsr can hot-reload the user functions in a very short time.

It is mainly developed by C++ and use the bazel tool to build it. In this runtime, we use gRPC
to exchange message between services. For the JavaScript performance, we choose the v8 engine
for JavaScript runtime.

## Build
As the bazel support multiple languages, all the subjects of toms are organized by bazel.
If you are first time to build jsr, you can build it with the below command.
```shell
bazel build //...
```

In the progress of building, v8 will cost a very long time. So if you want to shorten the compile time,
you can do as follow:

1.Compile v8 libraries standalone with the following command;
```shell
bazel build //:v8
```
2.Save the libraries to your other local folder to reuse next time;

3.Add the v8 libraries to the 'deps/libs/v8' folder. If 'deps/libs/v8' does not exist,
   create it manually;

4.Run the following command and build it.
```shell
bazel build //... --define use_local_v8_library=true
```

Bazel is a popular tool, if you want to learn more about it, go to [Bazel Home](https://www.mdeditor.com/).

## Community
Now toms is developing and has not be released, there are a lot of things to do. If you are interested in toms,
please contact us by email [jingbing.csu@gmail.com](jingbing.csu@gmail.com). We hope you can join us to make it.