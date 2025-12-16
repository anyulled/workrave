# Building and running Workrave for macOS (Qt)

These instructions describe how to build the macOS bundle using the Qt UI. The CMake project already configures macOS app bundle layout and Qt packaging hooks, so the steps below focus on preparing dependencies, configuring CMake, and producing a runnable `.app` or DMG.

## Prerequisites

1. Install Xcode Command Line Tools (required for clang and standard SDK headers):
   ```sh
   xcode-select --install
   ```
2. Install Homebrew from https://brew.sh and then install build tools and Qt 6:
   ```sh
   brew install cmake ninja pkg-config qt@6 gettext
   ```
3. Add Qt to your `PATH` so `qmake`, `macdeployqt`, and Qt CMake packages can be found:
   ```sh
   export PATH="$(brew --prefix qt@6)/bin:$PATH"
   export CMAKE_PREFIX_PATH="$(brew --prefix qt@6)"
   ```

## Configure the build

1. Clone the repository and create a build directory:
   ```sh
   git clone https://github.com/rcaelers/workrave.git
   cd workrave
   mkdir -p build && cd build
   ```
2. Configure CMake for the Qt UI (Core Next is enabled automatically for Qt builds). Use Ninja for faster builds and set a universal binary if you want both Apple Silicon and Intel:
   ```sh
   cmake -G Ninja \
     -DCMAKE_BUILD_TYPE=Release \
     -DWITH_UI=Qt \
     -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
     ..
   ```
   CMake will set the macOS bundle directories and register `workrave` as a `MACOSX_BUNDLE`, so the resulting target is an `.app` inside the build tree.

## Build and run locally

If you prefer an automated flow, use the helper script from the repo root:

```sh
tools/local/macos-qt-build.sh --package
```

The script configures a Release universal binary with the Qt UI, builds it, and runs CPack to produce an `.app` bundle and DMG. Pass `--run` to open the bundle automatically or `--arch arm64` to target Apple Silicon only. Use `tools/local/macos-qt-build.sh --help` for the full option list.

1. Build the application:
   ```sh
   cmake --build .
   ```
2. Run the app directly from the build tree:
   ```sh
   open ./workrave.app
   ```
   The bundle contains the Qt-based UI and macOS integration layers. If you prefer running the binary without Finder, the executable is located in `workrave.app/Contents/MacOS/`.

## Create a distributable DMG

The project ships a CPack configuration for macOS that drives `macdeployqt` to stage Qt frameworks and generate a drag-and-drop installer. After building:
```sh
cmake --build . --target package
```
This produces both the finalized `workrave.app` bundle and a DMG in the build directory that includes the appropriate Qt runtime libraries. Signing for notarization can be added by setting `WORKRAVE_SIGN_IDENTITY` before packaging.

## Troubleshooting

- If CMake cannot locate Qt, ensure `CMAKE_PREFIX_PATH` points to your Qt installation (`$(brew --prefix qt@6)`).
- If you only need an Apple Silicon binary, drop `-DCMAKE_OSX_ARCHITECTURES` or set it to `arm64`.
- To speed up incremental builds, keep the `build` directory and re-run `cmake --build .` after source changes.
