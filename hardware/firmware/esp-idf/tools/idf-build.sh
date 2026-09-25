#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v idf.py >/dev/null 2>&1; then
    for export_script in \
        "$HOME/esp/esp-idf/export.sh" \
        "$HOME/esp-idf/export.sh" \
        "/opt/esp/esp-idf/export.sh"; do
        if [ -f "$export_script" ]; then
            # shellcheck disable=SC1090
            . "$export_script"
            break
        fi
    done
fi

if ! command -v idf.py >/dev/null 2>&1; then
    cat >&2 <<'EOF'
idf.py was not found in this shell.

Install ESP-IDF inside WSL/Linux, then rerun this script. A standard install path is:
  $HOME/esp/esp-idf

If ESP-IDF is already installed elsewhere, source its export script first:
  . /path/to/esp-idf/export.sh
EOF
    exit 1
fi

cd "$PROJECT_DIR"
idf.py "${@:-build}"
