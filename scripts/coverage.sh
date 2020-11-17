#!/usr/bin/env sh

# Script for uploading coverage report

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info \
    '/usr/*' \
    "$(pwd)"'/third_party/*' \
    "$(pwd)"'/modules/core/third_party/*' \
    "$(pwd)"'/modules/core/test/*' \
    "$(pwd)"'/modules/compression/test/*' \
    "$(pwd)"'/modules/lossless/test/*' \
    --output-file coverage.info
bash <(curl -s https://codecov.io/bash) -f coverage.info || echo "Codecov did not collect coverage reports"
