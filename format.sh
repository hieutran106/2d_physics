#!/bin/bash
# Format all .cpp and .h files in src/ using clang-format.
# Works on macOS/Linux (clang-format on PATH) and Windows Git-Bash (VS 2022 bundled clang-format).

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Windows Visual Studio 2022 bundled clang-format (used as a fallback on Windows).
VS_CLANG_FORMAT="/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/clang-format.exe"

# Resolve clang-format in priority order:
#   1. CLANG_FORMAT environment variable (explicit override)
#   2. clang-format found on PATH (typical macOS/Linux install)
#   3. Visual Studio 2022 bundled clang-format (Windows)
if [ -n "$CLANG_FORMAT" ] && [ -x "$CLANG_FORMAT" ]; then
    : # use provided override
elif command -v clang-format >/dev/null 2>&1; then
    CLANG_FORMAT="$(command -v clang-format)"
elif [ -f "$VS_CLANG_FORMAT" ]; then
    CLANG_FORMAT="$VS_CLANG_FORMAT"
else
    echo "Error: clang-format not found."
    echo "  - Install it (macOS: 'brew install clang-format'), or"
    echo "  - set the CLANG_FORMAT environment variable to its full path, or"
    echo "  - install Visual Studio 2022 with the LLVM toolset (Windows)."
    exit 1
fi

echo "Using clang-format: $CLANG_FORMAT"

find "$SCRIPT_DIR/src" \
    -type f \( -name "*.cpp" -o -name "*.h" \) \
    -print0 | while IFS= read -r -d '' file; do
    echo "Formatting: $file"
    "$CLANG_FORMAT" -i "$file"
done

echo "Done."
