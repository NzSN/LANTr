#!/bin/bash

CONFIG=debug

if [[ "$OSTYPE" == "darwin"* ]]; then
    CONFIG=debug_on_mac
fi

bazel test $(bazel query //...) -c dbg --config=$CONFIG --build_tests_only
