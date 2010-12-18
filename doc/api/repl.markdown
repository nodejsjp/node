## REPL

<!--
A Read-Eval-Print-Loop (REPL) is available both as a standalone program and easily
includable in other programs.  REPL provides a way to interactively run
JavaScript and see the results.  It can be used for debugging, testing, or
just trying things out.
-->
Read-Eval-Print-Loop (REPL) は単独のプログラムとしても他のプログラムに手軽に取り込む形でも利用することができます。
REPL は対話的に JavaScript を実行して結果を確認する手段を提供します。 
デバッグやテストやその他の様々なことを試す用途で利用されます。

<!--
By executing `node` without any arguments from the command-line you will be
dropped into the REPL. It has simplistic emacs line-editing.
-->
コマンドラインから `node` を引数無しで実行することで、REPL プログラムに入ります。
REPL は Emacs 風の簡易な行編集機能を備えています。

    mjr:~$ node
    Type '.help' for options.
    > a = [ 1, 2, 3];
    [ 1, 2, 3 ]
    > a.forEach(function (v) {
    ...   console.log(v);
    ...   });
    1
    2
    3

<!--
For advanced line-editors, start node with the environmental variable `NODE_NO_READLINE=1`.
This will start the REPL in canonical terminal settings which will allow you to use with `rlwrap`.
-->
より進んだ行編集を行うには、環境変数に `NODE_NO_READLINE=1` を設定してnodeを起動してください。
これによって正規の端末設定で REPL を起動し、`rlwrap` を有効にした状態でREPLを利用することができます。

<!--
For example, you could add this to your bashrc file:
-->
例として、bashrc ファイルに以下のように設定を追加します:

    alias node="env NODE_NO_READLINE=1 rlwrap node"


### repl.start(prompt='> ', stream=process.openStdin())

<!--
Starts a REPL with `prompt` as the prompt and `stream` for all I/O.  `prompt`
is optional and defaults to `> `.  `stream` is optional and defaults to
`process.openStdin()`.
-->
`prompt` でプロンプト記号を、 `stream` で I/O を引数に取って REPL を起動します。
`prompt` は省略可能で、 デフォルトは `node> ` です。
`stream` は省略可能で、 デフォルトは `process.openStdin()`です。

<!--
Multiple REPLs may be started against the same running instance of node.  Each
will share the same global object but will have unique I/O.
-->
複数の REPL を起動した場合、同一の node インスタンスが実行されないことがあります。
それぞれの REPL はグローバルオブジェクトを共有しますが、I/O は固有のものを持ちます。

<!--
Here is an example that starts a REPL on stdin, a Unix socket, and a TCP socket:
-->
REPL を標準入力、Unix ドメインソケット、TCP ソケットのもとで起動する例を示します:

    var net = require("net"),
        repl = require("repl");

    connections = 0;

    repl.start("node via stdin> ");

    net.createServer(function (socket) {
      connections += 1;
      repl.start("node via Unix socket> ", socket);
    }).listen("/tmp/node-repl-sock");

    net.createServer(function (socket) {
      connections += 1;
      repl.start("node via TCP socket> ", socket);
    }).listen(5001);

<!--
Running this program from the command line will start a REPL on stdin.  Other
REPL clients may connect through the Unix socket or TCP socket. `telnet` is useful
for connecting to TCP sockets, and `socat` can be used to connect to both Unix and
TCP sockets.
-->
このプログラムをコマンドラインから実行すると、標準入力のもとで REPL が起動します。
他の REPL クライアントは Unix ドメインソケットか TCP ソケットを介して接続することができます。
`telnet` が TCP ソケットへの接続に便利です。
`socat` は Unix ドメイン /TCP 両方のソケットへの接続に利用できます。

<!--
By starting a REPL from a Unix socket-based server instead of stdin, you can
connect to a long-running node process without restarting it.
-->
標準入力の代わりに Unix ドメインソケットをベースとしたサーバから REPL を起動することによって、
再起動することなく node の常駐プロセスへ接続することができます。


### REPL Features

<!--
Inside the REPL, Control+D will exit.  Multi-line expressions can be input.
-->
REPL の中で Control+D を実行すると終了します。複数行に渡る式を入力とすることができます。

<!--
The special variable `_` (underscore) contains the result of the last expression.
-->
特別な変数である `_` (アンダースコア) は一番最後の式の結果を保持します。

    > [ "a", "b", "c" ]
    [ 'a', 'b', 'c' ]
    > _.length
    3
    > _ += 1
    4

<!--
The REPL provides access to any variables in the global scope. You can expose a variable
to the REPL explicitly by assigning it to the `context` object associated with each
`REPLServer`.  For example:
-->
REPL はグローバルスコープに存在する全ての変数にアクセス可能です。
それぞれの `REPLServer` に紐づく `context` オブジェクトに変数を付与することで、
明示的に変数を公開させることが可能です。 例:

    // repl_test.js
    var repl = require("repl"),
        msg = "message";

    repl.start().context.m = msg;

<!--
Things in the `context` object appear as local within the REPL:
-->
`context` オブジェクトに設定された変数は、REPL の中ではローカルな変数として現れます:

    mjr:~$ node repl_test.js
    > m
    'message'

<!--
There are a few special REPL commands:
-->

<!--
  - `.break` - While inputting a multi-line expression, sometimes you get lost
    or just don't care about completing it. `.break` will start over.
  - `.clear` - Resets the `context` object to an empty object and clears any multi-line expression.
  - `.exit` - Close the I/O stream, which will cause the REPL to exit.
  - `.help` - Show this list of special commands.
-->
* `.break` - 複数行に渡って式を入力している間に、途中で分からなくなったり完了させなくても良くなることがあります。`.break` で最初からやり直します。
* `.clear` - `context` オブジェクトを空の状態にリセットし、複数行に入力している式をクリアします。
* `.exit` - I/Oストリームを閉じ、REPLを終了させます。
* `.help` - このコマンドの一覧を表示します。
