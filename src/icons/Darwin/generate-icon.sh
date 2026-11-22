#!/bin/bash
set -euo pipefail

source_png="$1"
output_icns="$2"
iconset="${output_icns%.icns}.iconset"

mkdir -p "$iconset"

for size in 16 32 128 256 512; do
    sips -s format png -z "$size" "$size" "$source_png" \
        --out "$iconset/icon_${size}x${size}.png" >/dev/null
    doubled=$((size * 2))
    sips -s format png -z "$doubled" "$doubled" "$source_png" \
        --out "$iconset/icon_${size}x${size}@2x.png" >/dev/null
done

iconutil -c icns "$iconset" -o "$output_icns"
rm -rf "$iconset"
