#!/usr/bin/env bash
# Generate project files (for IDEs / compile_commands.json)
set -e

UE5_ROOT="${UE5_ROOT:-/opt/unreal-engine}"
PROJECT="$(realpath "$(dirname "$0")/../WhereYouAre.uproject")"

"$UE5_ROOT/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh" \
    "$PROJECT" \
    -Game \
    -Engine \
    "$@"
