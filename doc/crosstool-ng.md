# Mac os xにクロスコンパイル環境を整える

Mac os xのgccはELFではなく、Mach-Oというライブラリ形式を使用する。
それは別にいいんだけどLinuxで実行できるバイナリを作りたいときに困るので、Mac os Xにクロスコンパイル環境を整備する。
クロスコンパイル環境の構築は手動でもできるらしいのだが、僕は何度も失敗してしまったのでcrossTool-NGというビルドツールを使用することにする。
crosstool-ng : http://crosstool-ng.org/

## crosstool-NGのインストール

ソースを持ってくる

   mkdir -p ~/local/src
   cd ~/local/src
   wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.9.3.tar.bz2
   tar jxf crosstool-ng-1.9.3.tar.bz2

gnu-sed,gnu-awk,automake,libtool(libtool,libtoolize),binutils(objcopy,objdump,readelf)が必要なのでhomebrewからインストールする

   brew install gnu-sed
   brew install gnu-awk
   brew install automake
   brew install libtool
   brew install binutils
   
インストール先を作って、コンパイルする

   mkdir $HOME/local/cross
   ./configure --with-sed=/usr/local/Cellar/gnu-sed/4.2.1/bin/gsed --prefix=$HOME/local/cross --with-awk=/usr/local/Cellar/gawk/4.0.1/bin/gawk --with-libtool=/usr/local/Cellar/libtool/2.4.2/bin/glibtool --with-libtoolize=/usr/local/Cellar/libtool/2.4.2/bin/glibtoolize --with-objcopy=/usr/local/Cellar/binutils/2.22/bin/gobjcopy --with-objdump=/usr/local/Cellar/binutils/2.22/bin/gobjdump --with-readelf=/usr/local/Cellar/binutils/2.22/bin/greadelf
   make 
   make install

## クロスコンパイル用の設定ファイルを作成する   

   mkdir -p $HOME/local/build_cross  
   cd $HOME/local/build_cross
   ct-ng menuconfig
 
kernelコンフィグレーションのmenuconfigのような画面が開くので、クロスコンパイルしたい環境に合わせて設定していく。
今回の目的はmac os xでcentos用のバイナリを吐くことなので、次のように設定を変更した。
注意しないといけないのは、C libraryはglibcを使用し、Cloog/pplのバージョンは0.15.10はコンパイルが失敗した。

   target options -> Target Architecture -> x86
                     Bitness -> 32-bits
   Operationg System -> Target OS -> linux
   C-library -> C library -> glibc
   Companion libraries -> Cloog/ppl version -> 0.15.9

## クロスコンパイラをビルドする

コンフィグを作成したらそれに基づいてビルドする

   ct-ng build
   
Mountain Lionで試したらyour file system *not* case-sensitiveって言われて失敗する。。。
別のファイルシステムを作るしかないのか？
