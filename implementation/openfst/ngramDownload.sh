#!/bin/bash

DATA_DIR=data

echo ${DATA_DIR}に移ります
cd ${DATA_DIR}

echo 'nwc2010 よりデータをダウンロード. インターネットの接続状況にお気をつけください'
wget -P ${DATA_DIR} -xnH -i nwc_filelist

echo 元のディレクトリに戻ります
cd -
