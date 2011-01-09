## Debugger

<!--
V8 comes with an extensive debugger which is accessable out-of-process via a
simple [TCP protocol](http://code.google.com/p/v8/wiki/DebuggerProtocol).
Node has a built-in client for this debugger. To use this, start Node with the
`debug` argument; a prompt will appear:
-->
V8は外部プロセスから [TCP プロトコル](http://code.google.com/p/v8/wiki/DebuggerProtocol)経由で接続可能なデバッガを備えています。
Node にはこのデバッガへのクライアントが組み込まれています。
これを使うには、 `debug` 引数を指定して Node を起動します。
次のようになります:

    % node debug myscript.js
    debug>

<!--
At this point `myscript.js` is not yet running. To start the script, enter
the command `run`. If everything works okay, the output should look like
this:
-->
ポイントは，`myscript.js` はまだ実行されないことです。
スクリプトを開始するには、`run` コマンドを入力します。
全てがうまくいけば、出力はこのようになります:

    % node debug myscript.js
    debug> run
    debugger listening on port 5858
    connecting...ok

<!--
Node's debugger client doesn't support the full range of commands, but
simple step and inspection is possible. By putting the statement `debugger;`
into the source code of your script, you will enable a breakpoint.
-->
Node のデバッガクライアントはあらゆるコマンドを完全にサポートしているわけではありませんが、
単純なステップ実行やインスペクションが可能です。
スクリプトのソースコードに `debugger;` 文を挿入すると、
ブレークポイントが有効になります。

<!--
For example, suppose `myscript.js` looked like this:
-->
例えば、`myscript.js` が次のようだとします:

    // myscript.js
    x = 5;
    setTimeout(function () {
      debugger;
      console.log("world");
    }, 1000);
    console.log("hello");

<!--
Then once the debugger is run, it will break on line 4.
-->
ひとたびデバッガを実行すると、4行目で中断します。

    % ./node debug myscript.js
    debug> run
    debugger listening on port 5858
    connecting...ok
    hello
    break in #<an Object>._onTimeout(), myscript.js:4
      debugger;
      ^
    debug> next
    break in #<an Object>._onTimeout(), myscript.js:5
      console.log("world");
      ^
    debug> print x
    5
    debug> print 2+2
    4
    debug> next
    world
    break in #<an Object>._onTimeout() returning undefined, myscript.js:6
    }, 1000);
    ^
    debug> quit
    A debugging session is active. Quit anyway? (y or n) y
    %


<!--
The `print` command allows you to evaluate variables. The `next` command steps
over to the next line. There are a few other commands available and more to
come type `help` to see others.
-->
``print`` コマンドは変数を評価します。
``next`` コマンドは次の行にステップオーバーします。
他にもいくつかのコマンドを利用することができます。
それらについては ``help`` をタイプしてください。


### Advanced Usage

<!--
The V8 debugger can be enabled and accessed either by starting Node with
the `--debug` command-line flag or by signaling an existing Node process
with `SIGUSR1`.
-->
V8 デバッガは Node をコマンドラインの `--debug` フラグで起動したり、起動済みの Node プロセスに `SIGUSR1` シグナルを送ることでも有効にできます。

