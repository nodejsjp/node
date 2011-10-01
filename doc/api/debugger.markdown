## Debugger

<!--
V8 comes with an extensive debugger which is accessible out-of-process via a
simple [TCP protocol](http://code.google.com/p/v8/wiki/DebuggerProtocol).
Node has a built-in client for this debugger. To use this, start Node with the
`debug` argument; a prompt will appear:
-->
V8は外部プロセスから [TCP プロトコル](http://code.google.com/p/v8/wiki/DebuggerProtocol)経由で接続可能なデバッガを備えています。
Node にはこのデバッガへのクライアントが組み込まれています。
これを使うには、 `debug` 引数を指定して Node を起動します。
次のようになります:

    % node debug myscript.js
    < debugger listening on port 5858
    connecting... ok
    break in /home/indutny/Code/git/indutny/myscript.js:1
      1 x = 5;
      2 setTimeout(function () {
      3   debugger;
    debug>

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

    % node debug myscript.js
    < debugger listening on port 5858
    connecting... ok
    break in /home/indutny/Code/git/indutny/myscript.js:1
      1 x = 5;
      2 setTimeout(function () {
      3   debugger;
    debug> cont
    < hello
    break in /home/indutny/Code/git/indutny/myscript.js:3
      1 x = 5;
      2 setTimeout(function () {
      3   debugger;
      4   console.log("world");
      5 }, 1000);
    debug> next
    break in /home/indutny/Code/git/indutny/myscript.js:4
      2 setTimeout(function () {
      3   debugger;
      4   console.log("world");
      5 }, 1000);
      6 console.log("hello");
    debug> repl
    Press Ctrl + C to leave debug repl
    > x
    5
    > 2+2
    4
    debug> next
    < world
    break in /home/indutny/Code/git/indutny/myscript.js:5
      3   debugger;
      4   console.log("world");
      5 }, 1000);
      6 console.log("hello");
      7
    debug> quit
    %


<!--
The `repl` command allows you to evaluate code remotely. The `next` command
steps over to the next line. There are a few other commands available and more
to come type `help` to see others.
-->
``repl`` コマンドはコードをリモートで評価します。
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

