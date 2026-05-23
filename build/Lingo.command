#!/bin/zsh
set -euo pipefail

PROJECT_DIR="/Users/insany/Documents/osnova/proga/cpp/Lingo"
BUILD_DIR="$PROJECT_DIR/build"
BINARY="$BUILD_DIR/lingo"

if [ ! -x "$BINARY" ]; then
    echo "Не найден исполняемый файл: $BINARY"
    echo "Собери проект перед запуском."
    read "?Нажми Enter, чтобы закрыть окно..."
    exit 1
fi

cd "$PROJECT_DIR"
exec "$BINARY"
