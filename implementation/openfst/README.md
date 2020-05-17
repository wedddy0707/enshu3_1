# openFst, openGrm を利用した俳句生成

## 必要なものをインストール

インストールの手順についてはこちらのサイトを参考にさせていただきました. \
https://aghriss.github.io/posts/2018/01/01/OpenFSTubuntu.html

### Graphviz
```
$ sudo apt install graphviz
```
### openFst
  1. 以下のサイトから openfst-(version).tar.gz をダウンロード \
     http://www.openfst.org/twiki/bin/view/FST/FstDownload
  2. ダウンロードしたファイルのあるディレクトリに移動し、解凍します \
    ```
    $ tar -xzf openfst-1.7.7.tar.gz # この場合は version 1.7.7
    ```
  
  3. 作成されたディレクトリに移動し、インストールします \
    ```
    $ cd openfst-1.7.7
    $ ./configure --enable-far=true
    $ make
    $ sudo make install
    ```
  
  4. LD\_LIBRARY\_PATH の設定 \
    ```
    $ echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib" >> ~/.bashrc
    $ source ~/.bashrc
    ```
  
  5. 正常にインストールされていることを確認 \
    ```
    $ fstinfo --help
    ```

### openGrm
  1. 以下のサイトから ngram-(version).tar.gz をダウンロード \
    http://www.opengrm.org/twiki/bin/view/GRM/NGramDownload
  2. ダウンロードしたファイルのあるディレクトリに移動し、解凍します \
    ```
    $ tar -xzf ngram-1.3.10.tar.gz # この場合は version 1.3.10
    ```
  3. 作成されたディレクトリに移動し、インストールします \
    ```
    $ cd ngram-1.3.10
    $ ./configure
    $ make
    $ sudo make install
    ```
  4. 正常にインストールされていることを確認 \
    ```
    $ ngraminfo --help
    ```

## 使い方

### prefix の用意
prefix は `./data/haiku_prefix.txt` に保存してください.
単語間は半角スペースで空けてください.
#### 例
```
人生 は
```

### context の用意
context は `./data/context.txt` に保存してください.
prefix と同様、単語間は半角スペースで空けてください.
#### 例
```
愛 と 言う
```

### 実行
```
$ make
```
`make` を実行すると `result.png` に結果が出力されます.
初めて実行する場合には、必要なファイルのダウンロードがありますので、
インターネットの接続状況にお気をつけください。

