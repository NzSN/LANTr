#!/bin/bash
bazel test $(bazel query //...) -c dbg --config=$1 --build_tests_only
