#!/bin/bash

cd "$(realpath "$(dirname "${BASH_SOURCE[0]}")")" || exit 1

hooks="$(git rev-parse --git-dir)/hooks"
mkdir -p "${hooks}"

ln -fs "$(realpath --relative-to="${hooks}" .)/pre-commit.sh"  "${hooks}/pre-commit"

