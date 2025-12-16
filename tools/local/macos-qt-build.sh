#!/usr/bin/env bash
# Build and optionally package/run the macOS Qt version of Workrave.
# This script assumes all prerequisites (Qt, Xcode command-line tools) are already installed.

set -euo pipefail

script_name=$(basename "$0")

usage() {
  cat <<USAGE
${script_name} [options]

Options:
  --build-dir DIR        Build directory (default: build-macos-qt)
  --build-type TYPE      CMake build type (default: Release)
  --arch LIST            macOS architectures, semicolon-separated (default: arm64;x86_64)
  --generator NAME       CMake generator (default: Ninja if available, otherwise Unix Makefiles)
  --package              Run "cmake --build --target package" after compiling
  --run                  Open the generated Workrave.app after building
  --skip-configure       Skip the CMake configure step (use existing cache)
  -h, --help             Show this help message

Examples:
  # Configure, build, and package a universal binary using Ninja
  ${script_name} --package

  # Build only for Apple Silicon with an existing configuration
  ${script_name} --arch arm64 --skip-configure
USAGE
}

ensure_macos() {
  if [[ $(uname -s) != "Darwin" ]]; then
    echo "[error] This script is intended for macOS." >&2
    exit 1
  fi
}

default_generator() {
  if command -v ninja >/dev/null 2>&1; then
    echo "Ninja"
  else
    echo "Unix Makefiles"
  fi
}

configure() {
  local generator=$1
  local build_dir=$2
  local build_type=$3
  local archs=$4

  echo "[info] Configuring CMake (generator: ${generator}, build: ${build_dir}, type: ${build_type}, archs: ${archs})"
  cmake -G "${generator}" \
    -S . \
    -B "${build_dir}" \
    -DWITH_UI=Qt \
    -DCMAKE_BUILD_TYPE="${build_type}" \
    -DCMAKE_OSX_ARCHITECTURES="${archs}"
}

build() {
  local build_dir=$1
  local build_type=$2

  echo "[info] Building Workrave"
  cmake --build "${build_dir}" --config "${build_type}"
}

package() {
  local build_dir=$1
  local build_type=$2

  echo "[info] Packaging DMG and app bundle"
  cmake --build "${build_dir}" --config "${build_type}" --target package
}

run_app() {
  local build_dir=$1

  local app_path="${build_dir}/workrave.app"
  if [[ ! -d "${app_path}" ]]; then
    echo "[error] ${app_path} not found. Did the build succeed?" >&2
    exit 1
  fi

  echo "[info] Launching ${app_path}"
  open "${app_path}"
}

main() {
  ensure_macos
  # Attempt to configure Qt environment if not already set
  if [[ -z "${CMAKE_PREFIX_PATH:-}" ]] && command -v brew >/dev/null 2>&1; then
    local qt_prefix
    qt_prefix=$(brew --prefix qt@6 2>/dev/null)
    if [[ -n "${qt_prefix}" && -d "${qt_prefix}" ]]; then
      echo "[info] Automatically setting Qt environment from Homebrew"
      export CMAKE_PREFIX_PATH="${qt_prefix}"
      export PATH="${qt_prefix}/bin:$PATH"
    fi
  fi

  local build_dir="build-macos-qt"
  local build_type="Release"
  local archs="arm64;x86_64"
  local generator=""
  local do_package=false
  local do_run=false
  local skip_configure=false

  while [[ $# -gt 0 ]]; do
    case "$1" in
      --build-dir)
        build_dir=$2; shift 2;;
      --build-type)
        build_type=$2; shift 2;;
      --arch)
        archs=$2; shift 2;;
      --generator)
        generator=$2; shift 2;;
      --package)
        do_package=true; shift;;
      --run)
        do_run=true; shift;;
      --skip-configure)
        skip_configure=true; shift;;
      -h|--help)
        usage; exit 0;;
      *)
        echo "[error] Unknown option: $1" >&2
        usage
        exit 1
        ;;
    esac
  done

  generator=${generator:-$(default_generator)}

  if [[ ${skip_configure} == false ]]; then
    configure "${generator}" "${build_dir}" "${build_type}" "${archs}"
  else
    echo "[info] Skipping configure step"
  fi

  build "${build_dir}" "${build_type}"

  if [[ ${do_package} == true ]]; then
    package "${build_dir}" "${build_type}"
  fi

  if [[ ${do_run} == true ]]; then
    run_app "${build_dir}"
  fi
}

main "$@"
