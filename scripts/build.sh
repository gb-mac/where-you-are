#!/usr/bin/env bash
# Build WhereYouAre for Linux
set -e

UE5_ROOT="${UE5_ROOT:-/opt/unreal-engine}"
PROJECT="$(realpath "$(dirname "$0")/../WhereYouAre.uproject")"

"$UE5_ROOT/Engine/Build/BatchFiles/Linux/Build.sh" \
    WhereYouAre \
    Linux \
    Development \
    "$PROJECT" \
    "$@"
