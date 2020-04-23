# openFst を利用した分かち書きトランスデューサ

## 必要なものをインストール

インストールの手順についてはこちらのサイトを参考にさせていただきました. \
https://aghriss.github.io/posts/2018/01/01/OpenFSTubuntu.html

1. 以下のサイトから openfst-(version).tar.gz をダウンロード \
   http://www.openfst.org/twiki/bin/view/FST/FstDownload
2. Graphviz のインストール
  ```
  $ sudo apt install graphviz
  ```

3. ダウンロードしたファイルのあるディレクトリに移動し、解凍します
  ```
  $ tar -xzf openfst-1.7.7.tar.gz # この場合は version 1.7.7
  ```

4. 作成されたディレクトリに移動し、インストールします
  ```
  $ cd openfst-1.7.7
  $ ./configure --enable-far=true
  $ make
  $ sudo make install
  ```

5. LD\_LIBRARY\_PATH の設定
  ```
  $ echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib" >> ~/.bashrc
  $ source ~/.bashrc
  ```

6. 正常にインストールされていることを確認
  ```
  $ fstinfo --help
  ```
