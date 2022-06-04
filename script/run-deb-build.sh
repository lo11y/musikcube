#!/bin/bash

set -e

can_sudo() {
  groups | awk -F: \
    '{
      split($2, listGroupsAsVals, " ");

      for (gr in listGroupsAsVals)
        listGroupsAsKeys[listGroupsAsVals[i]] = "";

      if ("sudo" in listGroupsAsKeys)
        exit 0;

      exit 1;
    }' | return
}

JOB_COUNT=${MUSIKCUBE_BUILD_JOB_COUNT}
OUTPUT_DIR=$(pwd)/deb

if [[ -z "${JOB_COUNT}" ]]; then
  JOB_COUNT=2
fi

if can_sudo; then
  sudo apt-get build-dep $(pwd)
fi

mkdir -p $(pwd)/deb

dpkg-buildpackage  \
  -uc \
  -us \
  -b \
  -j${JOB_COUNT} \
  --buildinfo-option=-u"${OUTPUT_DIR}" \
  --changes-option=-u"${OUTPUT_DIR}" \
  --no-sign \
  --post-clean

