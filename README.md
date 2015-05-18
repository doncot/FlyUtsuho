FlyUtsuho
=========

2DSTGのエンジン製作習作

## 説明
勉強の為の横2DスクロールSTGエンジンの実験品。自作スクリプトエンジン（Dusk言語という）を内臓。スクリプトからの入力を元にタスク単位で登録、実行を行うようにエンジンを作っている（……作りたかったが、現時点では一部の機能のみタスクエンジンが動かしている）。

## デモ
実行バイナリ - 
### デモ説明
* 現在最低限（未満）の機能しかない。Zで弾を発射できる。敵に弾が当たるとスコアが貰える。一定間隔でやってくる卵（白い奴）を取るとスコアが貰える。
* 敵キャラは全てDusk言語で書かれたスクリプトで動作する。
* スクリプトは"$(projectfile)\Script\TestScene_rev2.script"に記述する（ファイル名は現時点では、ソースに直接書く）。


#### 操作方法
* カーソルキー - 上下左右に移動
* zキー - 火球を発射
* Escキー - プログラムを終了


#### Dusk言語仕様
自作スクリプト言語。
##### 特徴
* pythonのように一行区切りで書く。改行が終端記号。
* エンコードはutf8（BOMありなし両方）とSJISを受け付けれる（専用の命令を書く）。
* C++風のコメント（//以下コメント）。
* （エセ）オブジェクト指向（風）。
* 実装はInterpreter.hファイルに全て書いてある。
 

##### 組み込み命令一覧
###### void encoding(String encodingName)
エンコーディングを指定する。**必ず**一行目に書く。指定がない場合、utf8として解釈する。引数arg1に与えられるのは"utf8"か"sjis"のみ。
###### ResourceVariable loadresource(String resourcePath)
リソースファイルのパス名を与え、**リソース変数(ResourceVariable)**として返す。現時点ではリソースファイルはただの画像ファイルだが、最終的にはxml設定ファイルを読み込む。
###### void ResourceVariable.deploy(int deployTime, [int x,int y])
配置命令。この命令を実行しないと、画面には映らない。deployTimeで配置タイミングをゲームスタート開始時からミリ秒後で指定。x,yで設置希望座標を指定。
###### void ResourceVariable.move(int moveTime,int duration,[int from_x,int from_y],[int to_x,int to_y],TransitType type)
移動命令。moveTimeに移動タイミングを指定（指定方法はdeploy命令と同じ）。durationで移動継続時間を指定。第三引数と第四引数で移動前と移動後の座標を指定。第五引数でアニメーション移動方法を指定する。

*アニメーション移動方法：LINEAR（線形[等速直線]移動）。EASEOUT（段々遅くなる）。EASEIN（ゆっくりから速くなる）。


## システム要件
* DirectX9.0cが動作する環境（SDKはいらない）。

## トラブルシューティング
* "d3dx9_43.dll"がないと言われる -> DirectXエンドユーザーランタイムをインストールする（https://www.microsoft.com/ja-jp/download/details.aspx?id=35）

## ライセンス
* 未完成なので、再配布不許可（No License）。参考にできるならしてみやがれ（滅茶苦茶なコードで本当すみません）。
* 完成できたら、MITライセンスで公開します。
* イーズインアウトアニメーションのコードにBSDライセンスのコードが使われているので注意。
