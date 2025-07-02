cmake_minimum_required(VERSION 3.13)

# SDKのインポート（pico_sdk_import.cmake を読み込む）
include(pico_sdk_import.cmake)

# プロジェクト定義
project(HelloWorld C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# SDK初期化
pico_sdk_init()

# 実行ファイル定義
add_executable(HelloWorld Main.cpp)

# ライブラリリンク
target_link_libraries(HelloWorld pico_stdlib)

# 出力の有効化
pico_add_extra_outputs(HelloWorld)

# UARTログ出力を有効に（必要に応じて）
pico_enable_stdio_uart(HelloWorld 1)
pico_enable_stdio_usb(HelloWorld 0)
