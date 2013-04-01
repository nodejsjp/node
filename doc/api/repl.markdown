# REPL

<!--
A Read-Eval-Print-Loop (REPL) is available both as a standalone program and
easily includable in other programs. The REPL provides a way to interactively
run JavaScript and see the results.  It can be used for debugging, testing, or
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
For advanced line-editors, start node with the environmental variable
`NODE_NO_READLINE=1`. This will start the main and debugger REPL in canonical
terminal settings which will allow you to use with `rlwrap`.
-->

より進んだ行編集を行うには、環境変数に `NODE_NO_READLINE=1`
を設定してnodeを起動してください。
これによって main とデバッガ REPL を正規の端末設定で起動し、
`rlwrap` を利用することができます。

<!--
For example, you could add this to your bashrc file:
-->

例として、bashrc ファイルに以下のように設定を追加します:

    alias node="env NODE_NO_READLINE=1 rlwrap node"


## repl.start(options)

<!--
Returns and starts a `REPLServer` instance. Accepts an "options" Object that
takes the following values:
-->
`REPLServer` インスタンスを作成して返します。
以下の値を含む "options" オブジェクトを受け取ります。

<!--
 - `prompt` - the prompt and `stream` for all I/O. Defaults to `> `.

 - `input` - the readable stream to listen to. Defaults to `process.stdin`.

 - `output` - the writable stream to write readline data to. Defaults to
   `process.stdout`.

 - `terminal` - pass `true` if the `stream` should be treated like a TTY, and
   have ANSI/VT100 escape codes written to it. Defaults to checking `isTTY`
   on the `output` stream upon instantiation.

 - `eval` - function that will be used to eval each given line. Defaults to
   an async wrapper for `eval()`. See below for an example of a custom `eval`.

 - `useColors` - a boolean which specifies whether or not the `writer` function
   should output colors. If a different `writer` function is set then this does
   nothing. Defaults to the repl's `terminal` value.

 - `useGlobal` - if set to `true`, then the repl will use the `global` object,
   instead of running scripts in a separate context. Defaults to `false`.

 - `ignoreUndefined` - if set to `true`, then the repl will not output the
   return value of command if it's `undefined`. Defaults to `false`.

 - `writer` - the function to invoke for each command that gets evaluated which
   returns the formatting (including coloring) to display. Defaults to
   `util.inspect`.
-->

 - `prompt` - プロンプト。デフォルトは `> ` です。

 - `input` - 監視する入力ストリーム。デフォルトは `process.stdin` です。

 - `output` - 読み込んだデータを書き込む出力ストリーム。
   デフォルトは `process.stdout` です。

 - `terminal` - もし `stream` が TTY で、ANSI/VT100 エスケープコードを
   出力するなら `true`。デフォルトはインスタンス作成時に `output`
   ストリームを `isTTY` でチェックします。

 - `eval` - 各行を評価するために使われる関数。デフォルトは `eval()` を
   非同期にラップした関数です。
   `eval` をカスタマイズする例は下記を参照してください。

 - `useColors` - `write` 関数が色を付けるかどうかを指定するブーリアン値。
   `writer` に異なる関数が設定された場合、これは何もしません。
   デフォルトは repl の `terminal` の値です。

 - `useGlobal` - もし `true` に設定されると、repl は独立したコンテキストを
   使う代わりに `global` オブジェクトを使用します。デフォルトは `false` です。

 - `ignoreUndefined` - もし `true` に設定されると、repl はコマンドの戻り値が
   `undefined` だった場合にそれを出力しません。デフォルトは `false` です。

 - `writer` - コマンドが評価されるごとに実行される関数で、表示するために
   フォーマット (色づけも含みます) して返します。
   デフォルトは `util.inspect` です。

<!--
You can use your own `eval` function if it has following signature:
-->

以下のシグネチャを持つ独自の `eval()` 関数を使うことができます。

    function eval(cmd, context, filename, callback) {
      callback(null, result);
    }

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

    repl.start({
      prompt: "node via stdin> ",
      input: process.stdin,
      output: process.stdout
    });

    net.createServer(function (socket) {
      connections += 1;
      repl.start({
        prompt: "node via Unix socket> ",
        input: socket,
        output: socket
      }).on('exit', function() {
        socket.end();
      })
    }).listen("/tmp/node-repl-sock");

    net.createServer(function (socket) {
      connections += 1;
      repl.start({
        prompt: "node via TCP socket> ",
        input: socket,
        output: socket
      }).on('exit', function() {
        socket.end();
      });
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

For an example of running a "full-featured" (`terminal`) REPL over
a `net.Server` and `net.Socket` instance, see: https://gist.github.com/2209310

For an example of running a REPL instance over `curl(1)`,
see: https://gist.github.com/2053342
-->

標準入力の代わりに Unix ドメインソケットをベースとしたサーバから REPL を起動することによって、
再起動することなく node の常駐プロセスへ接続することができます。

`net.Server` および `net.Socket` インスタンス上の "フル機能の" (`terminal`)
REPL を実行する例は、https://gist.github.com/2209310 を参照してください。

`curl(1)` 上で REPL インスタンスを実行する例は、
https://gist.github.com/2053342 を参照してください。

### Event: 'exit'

`function () {}`

<!--
Emitted when the user exits the REPL in any of the defined ways. Namely, typing
`.exit` at the repl, pressing Ctrl+C twice to signal SIGINT, or pressing Ctrl+D
to signal "end" on the `input` stream.
-->

何らかの方法でユーザが REPL を終了した場合に生成されます。
すなわち、repl で `.exit` をタイプする、Ctrl+C を 2 回推して
SIGINT を生成する、あるいは Ctrl+D を推して `input` ストリームで `'end'` を
知らせるなどです。

<!--
Example of listening for `exit`:
-->

`'exit'` を監視する例:

    r.on('exit', function () {
      console.log('Got "exit" event from repl!');
      process.exit();
    });


### Event: 'reset'

`function (context) {}`

<!--
Emitted when the REPL's context is reset. This happens when you type `.clear`.
If you start the repl with `{ useGlobal: true }` then this event will never
be emitted.
-->

REPL のコンテキストがリセットされた場合に生成されます。
これは `.clear` をタイプした時に発生します。
もし `{ useGlobal: true }` を指定して repl を開始した場合、
このイベントは決して生成されません。

<!--
Example of listening for `reset`:
-->

`reset` を監視する例:

    // Extend the initial repl context.
    r = repl.start({ options ... });
    someExtension.extend(r.context);

    // When a new context is created extend it as well.
    r.on('reset', function (context) {
      console.log('repl has a new context');
      someExtension.extend(context);
    });


## REPL Features

<!-- type=misc -->

<!--
Inside the REPL, Control+D will exit.  Multi-line expressions can be input.
Tab completion is supported for both global and local variables.
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
The REPL provides access to any variables in the global scope. You can expose
a variable to the REPL explicitly by assigning it to the `context` object
associated with each `REPLServer`.  For example:
-->

REPL はグローバルスコープに存在する全ての変数にアクセス可能です。
それぞれの `REPLServer` に紐づく `context` オブジェクトに変数を付与することで、
明示的に変数を公開させることが可能です。 例:

    // repl_test.js
    var repl = require("repl"),
        msg = "message";

    repl.start("> ").context.m = msg;

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

特別な REPL コマンドがいくつかあります:

<!--
  - `.break` - While inputting a multi-line expression, sometimes you get lost
    or just don't care about completing it. `.break` will start over.
  - `.clear` - Resets the `context` object to an empty object and clears any
    multi-line expression.
  - `.exit` - Close the I/O stream, which will cause the REPL to exit.
  - `.help` - Show this list of special commands.
  - `.save` - Save the current REPL session to a file
    >.save ./file/to/save.js
  - `.load` - Load a file into the current REPL session.
    >.load ./file/to/load.js
-->

  - `.break` - 複数行に渡って式を入力している間に、途中で分からなくなったり完了させなくても良くなることがあります。`.break` で最初からやり直します。
  - `.clear` - `context` オブジェクトを空の状態にリセットし、複数行に入力している式をクリアします。
  - `.exit` - I/Oストリームを閉じ、REPLを終了させます。
  - `.help` - このコマンドの一覧を表示します。
  - `.save` - 現在の REPL セッションをファイルに保存します。
    >.save ./file/to/save.js
  - `.load` - 現在の REPL セッションにファイルをロードします。
    >.load ./file/to/load.js

<!--
The following key combinations in the REPL have these special effects:
-->

REPL では、以下のキーコンビネーションは特別な効果を持ちます

<!--
  - `<ctrl>C` - Similar to the `.break` keyword.  Terminates the current
    command.  Press twice on a blank line to forcibly exit.
  - `<ctrl>D` - Similar to the `.exit` keyword.
-->

  - `<ctrl>C` - `.break` キーワードと同様です。
  現在のコマンドを終了します。
  強制的に終了したければ空の行で 2 回押してください。
  - `<ctrl>D` - `.exit` キーワードと同様です。
