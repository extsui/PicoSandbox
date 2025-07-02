cmake_minimum_required(VERSION 3.13)

# SDKのインポート（pico_sdk_import.cmake を読み込む）
include(pico_sdk_import.cmake)

# プロジェクト定義
project(MscSample C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# SDK初期化
pico_sdk_init()

# 実行ファイル定義
add_executable(MscSample
    Main.cpp
    Util.cpp
    Msc.cpp
)

target_include_directories(MscSample PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# ライブラリリンク
target_link_libraries(MscSample
    pico_stdlib
    tinyusb_device  # TinyUSB デバイス機能
    tinyusb_board   # TinyUSB ボード依存層 (board_init())
)

# 出力の有効化
pico_add_extra_outputs(MscSample)

# UARTログ出力の有効化
pico_enable_stdio_uart(MscSample 1)
pico_enable_stdio_usb(MscSample 0)
