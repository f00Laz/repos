# Rictyフォントを使用したい！

        brew install fontforge
        curl -L -o $HOME/tmp/Inconsolata.otf http://levien.com/type/myfonts/Inconsolata.otf
        curl -L -o $HOME/tmp/3.2.0.zip https://github.com/yascentur/Ricty/zipball/3.2.0
        curl -L -o $HOME/tmp/migu-1m-20120411-2.zip "http://sourceforge.jp/frs/redir.php?m=jaist&f=%2Fmix-mplus-ipa%2F56156%2Fmigu-1m-20120411-2.zip"
        unzip 3.2.0.zip 
        unzip migu-1m-20120411-2.zip 
        cp Inconsolata.otf yascentur-Ricty-b9d8b9c/
        cp migu-1m-20120411-2/*.ttf yascentur-Ricty-b9d8b9c/
        ./yascentur-Ricty-b9d8b9c/ricty_generator.sh Inconsolata.otf migu-1m-20120411-2/migu-1m-regular.ttf  migu-1m-20120411-2/migu-1m-bold.ttf
        cp Ricty*.ttf /Library/Fonts/

## 参考サイト
http://d.hatena.ne.jp/bose999/20120715/1342323176
