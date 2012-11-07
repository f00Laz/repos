 # How to use weechat on mac os x 

公式サイト：http://www.weechat.org/

weechatはCUIのIRCクライアントです。
次のような特徴を持っています。
 - マルチプロトコル( IRC, Jabber )
 - 複数サーバへの接続( SSL, IPv6, プロキシのサポート ) 
 - コンパクト、高速、軽量
 - プラグインとスクリプトでカスタマイズや拡張が可能
 - IRCのRFCに準拠
 - マルチプラットフォーム( Gnu/Linux, *BSD, MacOS X, Windowsなど )
 - 完全なGPL、フリーソフトウェア

 ## インストール

perl、ruby、pythonのプラグインを使いたいので以下のようにインストールします。

	brew install weechat --perl --ruby --python

 ## weechatの起動

	weechat-curses

 ## 一般的な設定

weechatはホームディレクトリ以下に.weechatというディレクトリを作成し、その中に様々な設定ファイルを持ちますが、設定ファイルを直接書き換える必要なく、weechatにコマンドを入力することで設定を変更することができます。
以下の記述は私の社内のIRCサーバに接続した時の経験を元に書いていますので、もしかしたらあまり一般的な設定ではないかもしれません。

 ### サーバの追加 

irc.hogehoge.co.jpのポート6668番にSSL証明とパスワードを有効にして、hogehogeという名前で登録した時の例を示します。

 	/server add hogehoge irc.hogehoge.co.jp/6668 -ssl -password=********

 ### 証明書でエラーが出るので無視 

社内だけで使っているIRCサーバですので証明書が正規のものではないようで、SSL証明が正しくないと怒られてしまいました。
そこで、以下のように入力し、SSL証明をOFFにします。

	 /set irc.server.hogehoge.ssl_verify off 

 ### サーバごとに本名とニックネームを設定する  

	 /set irc.server.hogehoge.nicks "simofuji,simo,simomo"
	 /set irc.server.hogehoge.username "Simofuji Kousuke"

 ### サーバの自動接続 

weechat起動時に自動的に接続したいサーバには以下の設定を行います。

 	/set irc.server.hogehoge.autoconnect on

 ### 自動入室するチャンネル
 
weechat起動時に自動的に接続したいチャンネルを以下のように設定します。

	 /set irc.server.hogehoge.autojoin "#test,#test2"

 ### スクロールのキーバインドを変更する 
 
スクロールのキーバインドはデフォルトではPageUP、PageDownなのですが、Mac Bookには該当のキーが見当たらなかったのでCtrl-k、Ctrl-jでスクロールできるようにキーバインドを変更しました。

 	/key bind ctrl-K /window page_up
 	/key bind ctrl-J /winodw page_down

 ## プラグイン

http://www.weechat.org/scripts/には多くのプラグインやスクリプトが公開されています。
これらのプラグインを入れて自分好みにカスタマイズしていきます！
 
 ## weeget : プラグインを管理する
 weegetは
 weegetをインストールする。（weechat用のプラグインマネージャ)
 cd ~/.weechat/python/autoload/
 wget http://www.weechat.org/files/scripts/weeget.py

 weechatから/python autoload

 * buffersを入れる(バッファ一覧) * 
 /weeget install buffers

