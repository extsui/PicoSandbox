# PicoSandbox

Raspberry Pi Pico の実験場です。

# セットアップ手順

## ツールチェインのインストール
Ubuntu 22.04+ を前提としています。以下のパッケージをインストールしてください。

```sh
sudo apt update
sudo apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi \
    build-essential pkg-config libusb-1.0-0-dev python3 python3-pip git
```

## リポジトリのクローン
```sh
# 最終的には .bashrc や .zshrc に以下を追記するのが良い
export WORK_DIR=~/work                      # [注意] 環境によって要修正
export PICO_SDK_PATH=${REPO_ROOT}/pico-sdk  # Build に必要

# リポジトリクローン
export REPO_ROOT=${WORK_DIR}/PicoSandbox
cd ${WORK_DIR}
git clone https://github.com/extsui/PicoSandbox.git
cd ${REPO_ROOT}

# pico-sdk 内の TinyUSB など submodule の clone のために必要
# [補足] pico-sdk および pico-examples は git submodule として管理されている
git submodule update --init --recursive
```

## ビルド
```sh
cd ${REPO_ROOT}/Projects/MscSample
rm -rf Build
mkdir Build
cd Build
cmake ..
cmake --build . -j4
```

## 書き込み
```sh
# RaspberryPiPico の BOOTSEL ボタンを押下しながら USB 接続すると
# USB マスストレージデバイス (RPI-RP2) としてマウントされる
# これに .uf2 をコピーすることでフラッシュ書き込みされ、自動リブートされる
cp ${REPO_ROOT}/Projects/MscSample/Build/MscSample.uf2 /media/ext/RPI-RP2
```
