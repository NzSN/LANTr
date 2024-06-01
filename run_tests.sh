#!/bin/bash
bazel test $(bazel query //...) -c dbg --config=debug --build_tests_only
