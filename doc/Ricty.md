# Rictyフォントを使用したい！

## Mac OS Xのオペレーション

        brew install fontforge
	cd /tmp
        curl -L -o /tmp/Inconsolata.otf http://levien.com/type/myfonts/Inconsolata.otf
        curl -L -o /tmp/3.2.0.zip https://github.com/yascentur/Ricty/zipball/3.2.0
        curl -L -o /tmp/migu-1m-20120411-2.zip "http://sourceforge.jp/frs/redir.php?m=jaist&f=%2Fmix-mplus-ipa%2F56156%2Fmigu-1m-20120411-2.zip"
        unzip 3.2.0.zip 
        unzip migu-1m-20120411-2.zip 
        cp Inconsolata.otf yascentur-Ricty-b9d8b9c/
        cp migu-1m-20120411-2/*.ttf yascentur-Ricty-b9d8b9c/
        ./yascentur-Ricty-b9d8b9c/ricty_generator.sh Inconsolata.otf migu-1m-20120411-2/migu-1m-regular.ttf  migu-1m-20120411-2/migu-1m-bold.ttf
        cp Ricty*.ttf /Library/Fonts/

## Ubuntuのオペレーション
	
	sudo apt-get install fontforge
	cd /tmp
	curl -L -o /tmp/Inconsolata.otf http://levien.com/type/myfonts/Inconsolata.otf
        curl -L -o /tmp/3.2.0.zip https://github.com/yascentur/Ricty/zipball/3.2.0
        curl -L -o /tmp/migu-1m-20120411-2.zip "http://sourceforge.jp/frs/redir.php?m=jaist&f=%2Fmix-mplus-ipa%2F56156%2Fmigu-1m-20120411-2.zip"
        unzip 3.2.0.zip 
        unzip migu-1m-20120411-2.zip 
        cp Inconsolata.otf yascentur-Ricty-b9d8b9c/
        cp migu-1m-20120411-2/*.ttf yascentur-Ricty-b9d8b9c/
        ./yascentur-Ricty-b9d8b9c/ricty_generator.sh Inconsolata.otf migu-1m-20120411-2/migu-1m-regular.ttf  migu-1m-20120411-2/migu-1m-bold.ttf
	mkdir ~/.fonts
	mv Ricty*.ttf ~/.fonts
	fc-cache -vf
	gconftool-2 --set /apps/gnome-terminal/profiles/Default/font --type string "Ricty Regular 12" 

## 参考サイト
http://d.hatena.ne.jp/bose999/20120715/1342323176
