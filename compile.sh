#!/bin/bash
set -euo pipefail

BUILD_DIR="$(cd "$(dirname "$0")" && pwd)/build"
SOURCE_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "[BackupEngine] Creating build directory..."
mkdir -p "${BUILD_DIR}"

echo "[BackupEngine] Configuring with CMake..."
cmake -S "${SOURCE_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release

echo "[BackupEngine] Building..."
cmake --build "${BUILD_DIR}" -- -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "[BackupEngine] Build successful. Binary at: ${BUILD_DIR}/backup_engine"
