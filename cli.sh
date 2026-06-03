#!/usr/bin/env bash
set -euo pipefail

# ── 1. Папка для плагина ChatGPT ─────────────────────────────
TARGET=$HOME/.local/share/qtqml/ChatGPT
mkdir -p "$TARGET"
if [ "$(stat -c %U "$TARGET")" != "$(whoami)" ]; then
    echo "⚠️  $TARGET принадлежит другому пользователю – меняем владельца"
    sudo chown -R "$(whoami)":"$(whoami)" "$TARGET"
fi

# ── 2. Копируем .so, qmldir, QML‑файлы ────────────────────────
BUILD=$HOME/Documents/NeuroClient/build/Desktop-Debug
cp "$BUILD/libChatGPT_QML.so"   "$TARGET/"
cp "$BUILD/qmldir"              "$TARGET/"
cp -r "$BUILD/qml"              "$TARGET/"

# ── 3. Убираем строку «prefer» из qmldir ─────────────────────
sed -i '/prefer/d' "$TARGET/qmldir"

# ── 4. Добавляем depends Theme (можно опустить, если делаем вариант 2) ─────
if ! grep -q '^depends Theme' "$TARGET/qmldir"; then
    echo 'depends Theme' >> "$TARGET/qmldir"
fi

# ── 5. Устанавливаем Theme как отдельный модуль ───────────────────────
THEME_SRC=$HOME/Documents/NeuroClient/qml/theme
THEME_DST=$HOME/.local/share/qtqml/Theme
mkdir -p "$THEME_DST"
cp "$THEME_SRC/qmldir"   "$THEME_DST/"
cp "$THEME_SRC/Theme.qml" "$THEME_DST/"

# ── 6. Добавляем пути к пользовательским модулям ───────────────────────
export QT_QML_IMPORT_PATH="$HOME/.local/share/qtqml:/etc/nixos/Ambxst:${QT_QML_IMPORT_PATH:-}"
export QML2_IMPORT_PATH="$QT_QML_IMPORT_PATH"

# ── 7. Запускаем приложение ───────────────────────────────────────
#qs -p $HOME/Documents/NeuroClient/qml/Main.qml