# process

<!-- type=global -->

<!--
The `process` object is a global object and can be accessed from anywhere.
It is an instance of [EventEmitter][].
-->

`process` はグローバルオブジェクトで、どこからでもアクセスすることができます。
それは [EventEmitter][] のインスタンスです。

## Exit Codes

<!--
Node will normally exit with a `0` status code when no more async
operations are pending.  The following status codes are used in other
cases:
-->

未完了の非同期な操作が無くなると、Node はステータスコード `0` で正常に
終了します。その他のケースでは、以下のステータスコードが使われます。

<!--
* `1` **Uncaught Fatal Exception** - There was an uncaught exception,
  and it was not handled by a domain or an `uncaughtException` event
  handler.
* `2` - Unused (reserved by Bash for builtin misuse)
* `3` **Internal JavaScript Parse Error** - The JavaScript source code
  internal in Node's bootstrapping process caused a parse error.  This
  is extremely rare, and generally can only happen during development
  of Node itself.
* `4` **Internal JavaScript Evaluation Failure** - The JavaScript
  source code internal in Node's bootstrapping process failed to
  return a function value when evaluated.  This is extremely rare, and
  generally can only happen during development of Node itself.
* `5` **Fatal Error** - There was a fatal unrecoverable error in V8.
  Typically a message will be printed to stderr with the prefix `FATAL
  ERROR`.
* `6` **Non-function Internal Exception Handler** - There was an
  uncaught exception, but the internal fatal exception handler
  function was somehow set to a non-function, and could not be called.
* `7` **Internal Exception Handler Run-Time Failure** - There was an
  uncaught exception, and the internal fatal exception handler
  function itself threw an error while attempting to handle it.  This
  can happen, for example, if a `process.on('uncaughtException')` or
  `domain.on('error')` handler throws an error.
* `8` - Unused.  In previous versions of Node, exit code 8 sometimes
  indicated an uncaught exception.
* `9` - **Invalid Argument** - Either an unknown option was specified,
  or an option requiring a value was provided without a value.
* `10` **Internal JavaScript Run-Time Failure** - The JavaScript
  source code internal in Node's bootstrapping process threw an error
  when the bootstrapping function was called.  This is extremely rare,
  and generally can only happen during development of Node itself.
* `12` **Invalid Debug Argument** - The `--debug` and/or `--debug-brk`
  options were set, but an invalid port number was chosen.
* `>128` **Signal Exits** - If Node receives a fatal signal such as
  `SIGKILL` or `SIGHUP`, then its exit code will be `128` plus the
  value of the signal code.  This is a standard Unix practice, since
  exit codes are defined to be 7-bit integers, and signal exits set
  the high-order bit, and then contain the value of the signal code.
-->

* `1` **捕捉されなかった致命的な例外** - 捕捉されなかった例外があり、
  それがドメインや `uncaughtException` イベントハンドラによって
  処理されなかった。
* `2` - 未使用 (Bashに組み込まれた誤用のために予約)。
* `3` **内部的な JavaScript のパースエラー** - Node がプロセスを
  ブートストラップする内部用の JavaScript ソースコードでパースエラーが
  発生した。
  これはきわめてまれで、通常 Node 自身を開発している時にしか起こりません。
* `4` **内部的な JavaScript の評価失敗 ** - Node がプロセスを
  ブートストラップする内部用の JavaScript ソースコードが評価された際、
  関数を返すことに失敗した。
  これはきわめてまれで、通常 Node 自身を開発している時にしか起こりません。
* `5` **致命的なエラー** - 致命的で回復不可能なエラーが V8 で発生した。
  通常、標準エラー出力に接頭辞 `FATAL ERROR` とともにメッセージが出力されます。
* `6` **内部的エラーハンドラが関数ではない** - 捕捉されない例外が発生したが、
  内部的な例外ハンドラにどういうわけか関数ではないものが設定されていて
  呼び出せなかった。
* `7` **内部的なエラーハンドラが実行時に失敗** - 捕捉されない例外が発生し、
  それを処理していた内部的な例外ハンドラ自身も例外をスローした。
  これはたとえば、`process.on('uncaughtException')` または
  `domain.on('error')` のハンドラが例外をスローした場合に発生します。
* `8` - 未使用。以前のバージョンの Node では、終了コード 8 は捕捉されない例外を
  示すことがありました。
* `9` **不正な引数** - 未知のオプションが指定された、あるいは値が必須の
  オプションが値無しで指定された。
* `10` **内部的な JavaScript の実行時失敗** - Node がプロセスを
  ブートストラップする内部用の JavaScript ソースコードが評価された際、
  例外をスローした。
  これはきわめてまれで、通常 Node 自身を開発している時にしか起こりません。
* `12` **不正なデバッグ引数** - `--debug` または `--debug-brk` が指定されたが、
  不正なポート番号が選ばれた。
* `>128` **シグナルによる終了** - Node が `SIGKILL` や `SIGHUP`
  などの致命的なシグナルを受け取ると、終了コードは `128` にシグナルコードを
  加えた値になります。
  これは終了コードが 7bit 整数で定義された時からの Unix の標準的な慣習で、
  シグナルによる終了は最上位のビットを設定し、シグナルコードの値を含みます。

## Event: 'exit'

<!--
Emitted when the process is about to exit. There is no way to prevent the
exiting of the event loop at this point, and once all `exit` listeners have
finished running the process will exit. Therefore you **must** only perform
**synchronous** operations in this handler. This is a good hook to perform
checks on the module's state (like for unit tests). The callback takes one
argument, the code the process is exiting with.
-->

プロセスが終了しようとしている時に生成されます。
この位置からイベントループを抜けることを防ぐ方法はなく、全ての `'exit'`
リスナーの実行が完了すると、プロセスは終了します。
従って、このハンドラでできることは **同期** 操作 **だけ** です。
これは (ユニットテストのように) モジュールの状態をチェックするのに適した
フックとなります。
コールバックはプロセスの終了コードを唯一の引数として呼び出されます。

<!--
Example of listening for `exit`:
-->

`exit` を監視する例:

    process.on('exit', function(code) {
      // do *NOT* do this
      setTimeout(function() {
        console.log('This will not run');
      }, 0);
      console.log('About to exit with code:', code);
    });

## Event: 'uncaughtException'

<!--
Emitted when an exception bubbles all the way back to the event loop. If a
listener is added for this exception, the default action (which is to print
a stack trace and exit) will not occur.
-->

発生した例外がイベントループまでたどり着いた場合に生成されます。
もしこの例外に対するリスナーが加えられていれば、
デフォルトの動作 (それはスタックトレースをプリントして終了します) は起こりません。

<!--
Example of listening for `uncaughtException`:
-->

`uncaughtException` を監視する例:

    process.on('uncaughtException', function(err) {
      console.log('Caught exception: ' + err);
    });

    setTimeout(function() {
      console.log('This will still run.');
    }, 500);

    // Intentionally cause an exception, but don't catch it.
    nonexistentFunc();
    console.log('This will not run.');

<!--
Note that `uncaughtException` is a very crude mechanism for exception
handling.
-->

`uncaughtException` は例外を扱うとても荒削りなメカニズムであることに
注意してください。

<!--
Don't use it, use [domains](domain.html) instead. If you do use it, restart
your application after every unhandled exception!
-->

これを使う代わりに、[ドメイン](domain.html) を使ってください。
それを使えば、捕まえられない例外が発生した後でもアプリケーションを
再開することができます！

<!--
Do *not* use it as the node.js equivalent of `On Error Resume Next`. An
unhandled exception means your application - and by extension node.js itself -
is in an undefined state. Blindly resuming means *anything* could happen.
-->

これを Node.js における `On Error Resume Next` として *使わないで* ください。
捕まえられなかった例外は、アプリケーション
- および Node.js 自身の拡張 - が未定義の状態となることを意味します。
やみくもな再開は *どんなことでも* 起こることを意味します。

<!--
Think of resuming as pulling the power cord when you are upgrading your system.
Nine out of ten times nothing happens - but the 10th time, your system is bust.
-->

電源を引き抜きながらアプリケーションをアップグレードすることを
想像してください。
10 回中 9 回は何も起こりません
- しかし 10 回目にはそのシステムは使えなくなるかもしれません。

<!--
You have been warned.
-->

これは警告です。

## Signal Events

<!--type=event-->
<!--name=SIGINT, SIGHUP, etc.-->

<!--
Emitted when the processes receives a signal. See sigaction(2) for a list of
standard POSIX signal names such as SIGINT, SIGHUP, etc.
-->

プロセスがシグナルを受信した場合に生成されます。
SIGINT、SIGHUP、その他の POSIX 標準シグナル名の一覧について は sigaction(2) を参照してください。

<!--
Example of listening for `SIGINT`:
-->

`SIGINT`を監視する例:

    // Start reading from stdin so we don't exit.
    process.stdin.resume();

    process.on('SIGINT', function() {
      console.log('Got SIGINT.  Press Control-D to exit.');
    });

<!--
An easy way to send the `SIGINT` signal is with `Control-C` in most terminal
programs.
-->

多くの端末プログラムで簡単に `SIGINT` を送る方法は `Control-C` を押すことです。

<!--
Note:
-->

注意:

<!--
- `SIGUSR1` is reserved by node.js to start the debugger.  It's possible to
  install a listener but that won't stop the debugger from starting.
- `SIGTERM` and `SIGINT` have default handlers on non-Windows platforms that resets
  the terminal mode before exiting with code `128 + signal number`. If one of
  these signals has a listener installed, its default behaviour will be removed
  (node will no longer exit).
- `SIGPIPE` is ignored by default, it can have a listener installed.
- `SIGHUP` is generated on Windows when the console window is closed, and on other
  platforms under various similar conditions, see signal(7). It can have a
  listener installed, however node will be unconditionally terminated by Windows
  about 10 seconds later. On non-Windows platforms, the default behaviour of
  `SIGHUP` is to terminate node, but once a listener has been installed its
  default behaviour will be removed.
- `SIGTERM` is not supported on Windows, it can be listened on.
- `SIGINT` is supported on all platforms, and can usually be generated with
  `CTRL+C` (though this may be configurable). It is not generated when terminal
  raw mode is enabled.
- `SIGBREAK` is delivered on Windows when `CTRL+BREAK` is pressed, on non-Windows
  platforms it can be listened on, but there is no way to send or generate it.
- `SIGWINCH` is delivered when the console has been resized. On Windows, this will
  only happen on write to the console when the cursor is being moved, or when a
  readable tty is used in raw mode.
- `SIGKILL` cannot have a listener installed, it will unconditionally terminate
  node on all platforms.
- `SIGSTOP` cannot have a listener installed.
-->

- `SIGUSR1` は Node.js がデバッガを起動するために予約されています。
リスナを登録することは出来ますが、デバッガの起動を止めることは出来ません。
- `SIGTERM` および `SIGINT` は、Windows 以外のプラットフォームでは
  `128` + シグナル番号で終了する前にターミナルのモードをリセットする
  デフォルトのハンドラを持ちます。
  これらのシグナルのどちらかにリスナが登録されると、デフォルトの振る舞いは
  削除されます (node は終了しなくなります)。
- `SIGPIPE` はデフォルトでは無視され、リスナを登録することが出来ます。
- `SIGHUP` は Windows ではコンソールウィンドウが閉じられると発生します。
  他のプラットフォームでも同様の条件で発生します。詳細は signal(7)
  を参照してください。
  リスナを登録することは出来ますが、Windows では約 10 秒後に node は無条件に
  Windows によって終了されます。
  Windows 以外のプラットフォームでは、`SIGHUP` のデフォルトの振る舞いは
  nodeを終了することですが、リスナを登録するとデフォルトの振る舞いは
  削除されます。
- `SIGTERM` は Windows ではサポートされません。
  しかし、リスナを登録することは可能です。
- `SIGINT` は全てのプラットフォームでサポートされ、通常 `CTRL+C`
  (おそらく設定可能でしょう) によって生成されます。
  ターミナルが raw モードの場合は生成されません。
- `SIGBREAK` は Windows において `CTRL+BREAK` が推された時に送られます。
  Windows 以外のプラットフォームでもリスナを登録することは出来ますが、
  それを生成したり送信する方法はありません。
- `SIGWINCH` はコンソールのサイズが変更された場合に送られます。
  Windows では、カーソルが移動するか、tty が raw モードの場合に、
  コンソールへ書き込むと発生します。
- `SIGKILL` のリスナを組み込むことは出来ません。
  それは全てのプラットフォームで node を無条件に終了します。
- `SIGSTOP` のリスナを組み込むことは出来ません。

## process.stdout

<!--
A `Writable Stream` to `stdout`.
-->

`stdout` に対する `Writable Stream` です。

<!--
Example: the definition of `console.log`
-->

例: `console.log` の定義

    console.log = function(d) {
      process.stdout.write(d + '\n');
    };

<!--
`process.stderr` and `process.stdout` are unlike other streams in Node in
that writes to them are usually blocking.  They are blocking in the case
that they refer to regular files or TTY file descriptors. In the case they
refer to pipes, they are non-blocking like other streams.
-->

`process.stderr` と `process.stdout` は他のストリームと異なり、
書き込みは通常ブロックします。
それらが通常ファイルや TTY のファイル記述子を参照しているケースでは、
それらはブロックします。
パイプを参照しているケースでは、他のストリームと同様にブロックしません。

<!--
To check if Node is being run in a TTY context, read the `isTTY` property
on `process.stderr`, `process.stdout`, or `process.stdin`:
-->

Node が TTY のコンテキストで実行されているかチェックするには、
`process.stderr`, `process.stdout`, または `process.stdin` の
`isTTY` プロパティを参照します。

    $ node -p "Boolean(process.stdin.isTTY)"
    true
    $ echo "foo" | node -p "Boolean(process.stdin.isTTY)"
    false

    $ node -p "Boolean(process.stdout.isTTY)"
    true
    $ node -p "Boolean(process.stdout.isTTY)" | cat
    false

<!--
See [the tty docs](tty.html#tty_tty) for more information.
-->

より詳細は [the tty docs](tty.html#tty_tty) を参照してください。

## process.stderr

<!--
A writable stream to stderr.

`process.stderr` and `process.stdout` are unlike other streams in Node in
that writes to them are usually blocking.  They are blocking in the case
that they refer to regular files or TTY file descriptors. In the case they
refer to pipes, they are non-blocking like other streams.
-->

`stderr` に対する `Writable Stream` です。

`process.stderr` と `process.stdout` は他のストリームと異なり、
書き込みは通常ブロックします。
それらが通常ファイルや TTY のファイル記述子を参照しているケースでは、
それらはブロックします。
パイプを参照しているケースでは、他のストリームと同様にブロックしません。


## process.stdin

<!--
A `Readable Stream` for stdin. The stdin stream is paused by default, so one
must call `process.stdin.resume()` to read from it.
-->

標準入力に対する `Readable Stream` です。
デフォルトでは、標準入力に対するストリームは中断されているため、
読み込みのためには `process.stdin.resume()` を呼び出さなければなりません。

<!--
Example of opening standard input and listening for both events:
-->

標準入力をオープンして二つのイベントを監視する例:

    process.stdin.resume();
    process.stdin.setEncoding('utf8');

    process.stdin.on('data', function(chunk) {
      process.stdout.write('data: ' + chunk);
    });

    process.stdin.on('end', function() {
      process.stdout.write('end');
    });


## process.argv

<!--
An array containing the command line arguments.  The first element will be
'node', the second element will be the name of the JavaScript file.  The
next elements will be any additional command line arguments.
-->

コマンドライン引数を含む配列です。
最初の要素は 'node'、2 番目の要素は JavaScript ファイルの名前になります。
その後の要素はコマンドラインの追加の引数になります。

    // print process.argv
    process.argv.forEach(function(val, index, array) {
      console.log(index + ': ' + val);
    });

<!--
This will generate:
-->

このように出力されます:

    $ node process-2.js one two=three four
    0: node
    1: /Users/mjr/work/node/process-2.js
    2: one
    3: two=three
    4: four


## process.execPath

<!--
This is the absolute pathname of the executable that started the process.
-->

プロセスによって開始された実行可能ファイルの絶対パスです。

<!--
Example:
-->

例:

    /usr/local/bin/node


## process.execArgv

<!--
This is the set of node-specific command line options from the
executable that started the process.  These options do not show up in
`process.argv`, and do not include the node executable, the name of
the script, or any options following the script name. These options
are useful in order to spawn child processes with the same execution
environment as the parent.
-->

これはプロセス起動時に実行可能ファイルに与えられた node 固有の
コマンドライン・オプション群です。
それらのオプションは `process.argv` には現れず、node の実行可能ファイルや
スクリプト名、スクリプト名に続くどんなオプションも含まれません。
これらのオプションは親プロセスと同じ実行環境を持つ子プロセスを起動するために
役に立ちます。

<!--
Example:
-->

例:

    $ node --harmony script.js --version

<!--
results in process.execArgv:
-->

`process.execArgv` の結果:

    ['--harmony']

<!--
and process.argv:
-->

そして `process.argv` の結果:

    ['/usr/local/bin/node', 'script.js', '--version']


## process.abort()

<!--
This causes node to emit an abort. This will cause node to exit and
generate a core file.
-->

これは node をアボートさせます。
これは node が終了してコアファイルを生成する原因となります。

## process.chdir(directory)

<!--
Changes the current working directory of the process or throws an exception if that fails.
-->

プロセスのカレントワーキングディレクトリを変更します。
もし失敗した場合は例外をスローします。

    console.log('Starting directory: ' + process.cwd());
    try {
      process.chdir('/tmp');
      console.log('New directory: ' + process.cwd());
    }
    catch (err) {
      console.log('chdir: ' + err);
    }



## process.cwd()

<!--
Returns the current working directory of the process.
-->

プロセスのカレントワーキングディレクトリを返します。

    console.log('Current directory: ' + process.cwd());


## process.env

<!--
An object containing the user environment. See environ(7).
-->

ユーザの環境を含むオブジェクトです。environ(7) を参照してください。


## process.exit([code])

<!--
Ends the process with the specified `code`.  If omitted, exit uses the
'success' code `0`.
-->

指定の `code` でプロセスを終了します。
もし省略されると、「成功」を示すコード `0` を使って終了します。

<!--
To exit with a 'failure' code:
-->

「失敗」を示すコードで終了する例:

    process.exit(1);

<!--
The shell that executed node should see the exit code as 1.
-->

node を実行したシェルで終了コードが 1 であることを見ることができるでしょう。


## process.exitCode

<!--
A number which will be the process exit code, when the process either
exits gracefully, or is exited via `process.exit()` without specifying
a code.
-->

プロセスが正常終了する場合や、`process.exit()` でコードが指定されなかった
場合に、終了コードとなる数値。

<!--
Specifying a code to `process.exit(code)` will override any previous
setting of `process.exitCode`.
-->

`process.exit(code)` でコードが指定されると、以前 `process.exitCode` に
設定された値は上書きされます。


## process.getgid()

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Gets the group identity of the process. (See getgid(2).)
This is the numerical group id, not the group name.
-->

プロセスのグループ識別子を取得します (getgid(2) 参照)。
これは数値によるグループ ID で、グループ名ではありません。

    if (process.getgid) {
      console.log('Current gid: ' + process.getgid());
    }


## process.setgid(id)

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Sets the group identity of the process. (See setgid(2).)  This accepts either
a numerical ID or a groupname string. If a groupname is specified, this method
blocks while resolving it to a numerical ID.
-->

プロセスのグループ識別子を設定します (setgid(2) 参照)。
これは数値による ID もグループ名の文字列のどちらも受け入れます。
もしグループ名が指定されると、数値による ID が解決できるまでこのメソッドはブロックします。

    if (process.getgid && process.setgid) {
      console.log('Current gid: ' + process.getgid());
      try {
        process.setgid(501);
        console.log('New gid: ' + process.getgid());
      }
      catch (err) {
        console.log('Failed to set gid: ' + err);
      }
    }


## process.getuid()

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Gets the user identity of the process. (See getuid(2).)
This is the numerical userid, not the username.
-->

プロセスのユーザ識別子を取得します (getuid(2) 参照)。
これは数値によるユーザ ID で、ユーザ名ではありません。

    if (process.getuid) {
      console.log('Current uid: ' + process.getuid());
    }


## process.setuid(id)

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Sets the user identity of the process. (See setuid(2).)  This accepts either
a numerical ID or a username string.  If a username is specified, this method
blocks while resolving it to a numerical ID.
-->

プロセスのユーザ識別子を設定します (setuid(2) 参照)。
これは数値による ID もユーザ名の文字列のどちらも受け入れます。
もしユーザ名が指定されると、数値による ID が解決できるまでこのメソッドはブロックします。

    if (process.getuid && process.setuid) {
      console.log('Current uid: ' + process.getuid());
      try {
        process.setuid(501);
        console.log('New uid: ' + process.getuid());
      }
      catch (err) {
        console.log('Failed to set uid: ' + err);
      }
    }


## process.getgroups()

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Returns an array with the supplementary group IDs. POSIX leaves it unspecified
if the effective group ID is included but node.js ensures it always is.
-->

補助グループ ID の配列を返します。
POSIX は実効グループ ID が含まれることを明示していませんが、
Node.js では常にそれが含まれることを保証します。


## process.setgroups(groups)

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Sets the supplementary group IDs. This is a privileged operation, meaning you
need to be root or have the CAP_SETGID capability.
-->

補助グループ ID を設定します。
これは特権オペレーションであり、ルートであるか、または CAP_SETGID ケーパビリティを持つ必要があります。

<!--
The list can contain group IDs, group names or both.
-->

リストはグループ ID、グループ名、または両方を含むことができます。


## process.initgroups(user, extra_group)

<!--
Note: this function is only available on POSIX platforms (i.e. not Windows,
Android)
-->

注意: この関数は POSIX プラットフォーム (すなわち、Windows と Android 以外)
でのみ利用可能です。

<!--
Reads /etc/group and initializes the group access list, using all groups of
which the user is a member. This is a privileged operation, meaning you need
to be root or have the CAP_SETGID capability.
-->

`/etc/group` を読み込んでグループアクセスリストを初期化し、
user がメンバーである全てのグループを使用します。
これは特権オペレーションであり、ルートであるか、または CAP_SETGID ケーパビリティを持つ必要があります。

<!--
`user` is a user name or user ID. `extra_group` is a group name or group ID.
-->

`user` はユーザ名またはユーザ ID、
`extra_group` はグループ名またはグループ ID です。

<!--
Some care needs to be taken when dropping privileges. Example:
-->

特権を落とす際は、いくつか注意すべき事があります。例:

    console.log(process.getgroups());         // [ 0 ]
    process.initgroups('bnoordhuis', 1000);   // switch user
    console.log(process.getgroups());         // [ 27, 30, 46, 1000, 0 ]
    process.setgid(1000);                     // drop root gid
    console.log(process.getgroups());         // [ 27, 30, 46, 1000 ]


## process.version

<!--
A compiled-in property that exposes `NODE_VERSION`.
-->

`NODE_VERSION` を提示するコンパイル済みプロパティです。

    console.log('Version: ' + process.version);

## process.versions

<!--
A property exposing version strings of node and its dependencies.
-->

node と依存ライブラリのバージョン文字列を提示します。

    console.log(process.versions);

<!--
Will print something like:
-->

は以下のように出力します。

    { http_parser: '1.0',
      node: '0.10.4',
      v8: '3.14.5.8',
      ares: '1.9.0-DEV',
      uv: '0.10.3',
      zlib: '1.2.3',
      modules: '11',
      openssl: '1.0.1e' }

## process.config

<!--
An Object containing the JavaScript representation of the configure options
that were used to compile the current node executable. This is the same as
the "config.gypi" file that was produced when running the `./configure` script.

An example of the possible output looks like:
-->

現在のnode実行ファイルをコンパイルした際に使われた configure のオプションを
JavaScript で表現したオブジェクトを保持します。
これは `./configure` スクリプトを実行した際に生成された "cofnig.gypi"
ファイルと同じです。

実際の出力の例です:

    { target_defaults:
       { cflags: [],
         default_configuration: 'Release',
         defines: [],
         include_dirs: [],
         libraries: [] },
      variables:
       { host_arch: 'x64',
         node_install_npm: 'true',
         node_prefix: '',
         node_shared_cares: 'false',
         node_shared_http_parser: 'false',
         node_shared_libuv: 'false',
         node_shared_v8: 'false',
         node_shared_zlib: 'false',
         node_use_dtrace: 'false',
         node_use_openssl: 'true',
         node_shared_openssl: 'false',
         strict_aliasing: 'true',
         target_arch: 'x64',
         v8_use_snapshot: 'true' } }

## process.kill(pid, [signal])

<!--
Send a signal to a process. `pid` is the process id and `signal` is the
string describing the signal to send.  Signal names are strings like
'SIGINT' or 'SIGHUP'.  If omitted, the signal will be 'SIGTERM'.
See kill(2) for more information.
-->

プロセスにシグナルを送ります。
`pid` はプロセス ID で `signal` は送信されるシグナルを文字列で記述したものです。
シグナルの名前は `'SIGINT'` や `'SIGHUP'` のような文字列です。
省略すると、シグナルは `'SIGTERM'` となります。
詳細は kill(2) を参照してください。

<!--
Will throw an error if target does not exist, and as a special case, a signal of
`0` can be used to test for the existence of a process.
-->

対象が存在しない場合はエラーがスローされます。
特殊なケースとして、プロセスが存在することを確認するためにシグナル
`0` を使うことが出来ます。

<!--
Note that just because the name of this function is `process.kill`, it is
really just a signal sender, like the `kill` system call.  The signal sent
may do something other than kill the target process.
-->

この関数の名前が `process.kill` であるとおり、これは `kill`
システムコールのように単にシグナルを送信することに注意してください。
対象のプロセスを殺すためだけでなく、他のシグナルも送信できます。

<!--
Example of sending a signal to yourself:
-->

自身にシグナルを送信する例:

    process.on('SIGHUP', function() {
      console.log('Got SIGHUP signal.');
    });

    setTimeout(function() {
      console.log('Exiting.');
      process.exit(0);
    }, 100);

    process.kill(process.pid, 'SIGHUP');

<!--
Note: When SIGUSR1 is received by Node.js it starts the debugger, see
[Signal Events](#process_signal_events).
-->

注意: SIGUSR1 が Node.js によって受け取られると、それはデバッガを起動します。
[Signal Events](#process_signal_events) を参照してください。

## process.pid

<!--
The PID of the process.
-->

このプロセスの PID です。

    console.log('This process is pid ' + process.pid);


## process.title

<!--
Getter/setter to set what is displayed in 'ps'.
-->

'ps' でどのよう表示されるかを設定するための getter/setter です。

<!--
When used as a setter, the maximum length is platform-specific and probably
short.
-->

setter が使われる場合、その最大長はプラットフォーム依存で、おそらく短いです。

<!--
On Linux and OS X, it's limited to the size of the binary name plus the
length of the command line arguments because it overwrites the argv memory.
-->

Linux と OS X では、それは argv のメモリを上書きするため、
バイナリ名にコマンドライン引数を加えたものに制限されます。

<!--
v0.8 allowed for longer process title strings by also overwriting the environ
memory but that was potentially insecure/confusing in some (rather obscure)
cases.
-->

v0.8 はより長いプロセスタイトル文字列で環境を上書きしていましたが、
それはいくつかの (はっきりしない) ケースにおいて、
潜在的に危険で混乱していました。


## process.arch

<!--
What processor architecture you're running on: `'arm'`, `'ia32'`, or `'x64'`.
-->

実行しているプロセッサのアーキテクチャ: `'arm'`、`'ia32'`、または
`'x64'`。

    console.log('This processor architecture is ' + process.arch);


## process.platform

<!--
What platform you're running on:
`'darwin'`, `'freebsd'`, `'linux'`, `'sunos'` or `'win32'`
-->

どのプラットフォームで動いているかを示します:
`'darwin'`、`'freebsd'`、`'linux'`、`'sunos'`、または `'win32'`

    console.log('This platform is ' + process.platform);


## process.memoryUsage()

<!--
Returns an object describing the memory usage of the Node process
measured in bytes.
-->

Node プロセスのメモリ使用状況をバイト単位で記述したオブジェクトを返します。

    var util = require('util');

    console.log(util.inspect(process.memoryUsage()));

<!--
This will generate:
-->

このように生成されます:

    { rss: 4935680,
      heapTotal: 1826816,
      heapUsed: 650472 }

<!--
`heapTotal` and `heapUsed` refer to V8's memory usage.
-->

`heapTotal` と `heapUsed` は V8 のメモリ使用状況を参照します。


## process.nextTick(callback)

* `callback` {Function}

<!--
Once the current event loop turn runs to completion, call the callback
function.
-->

現在のイベントループが完了した後、コールバック関数を呼び出します。

<!--
This is *not* a simple alias to `setTimeout(fn, 0)`, it's much more
efficient.  It runs before any additional I/O events (including
timers) fire in subsequent ticks of the event loop.
-->

これは `setTimeout(fn, 0)` の単純なエイリアス*ではなく*、
はるかに効率的です。
これは他のどの I/O イベント (タイマを含みます) が発生するよりも前に
実行されます。

    console.log('start');
    process.nextTick(function() {
      console.log('nextTick callback');
    });
    console.log('scheduled');
    // Output:
    // start
    // scheduled
    // nextTick callback

<!--
This is important in developing APIs where you want to give the user the
chance to assign event handlers after an object has been constructed,
but before any I/O has occurred.
-->

これは API の開発において、オブジェクトが構築された後で
どんな I/O イベントが発生するよりも前に、
イベントハンドラを割り当てるチャンスをユーザに与えたい場合に重要になります。

    function MyThing(options) {
      this.setupOptions(options);

      process.nextTick(function() {
        this.startDoingStuff();
      }.bind(this));
    }

    var thing = new MyThing();
    thing.getReadyForStuff();

    // thing.startDoingStuff() gets called now, not before.

<!--
It is very important for APIs to be either 100% synchronous or 100%
asynchronous.  Consider this example:
-->

API は 100% 同期的か、100% 非同期的かのどちらかであることがとても重要です。
この例を考えてみてください:

    // WARNING!  DO NOT USE!  BAD UNSAFE HAZARD!
    function maybeSync(arg, cb) {
      if (arg) {
        cb();
        return;
      }

      fs.stat('file', cb);
    }

<!--
This API is hazardous.  If you do this:
-->

この API は危険です。こうすると:

    maybeSync(true, function() {
      foo();
    });
    bar();

<!--
then it's not clear whether `foo()` or `bar()` will be called first.
-->

`foo()` と `bar()` のどちらが先に呼び出されるか不明瞭になります。

<!--
This approach is much better:
-->

以下のアプローチはずっと優れています:

    function definitelyAsync(arg, cb) {
      if (arg) {
        process.nextTick(cb);
        return;
      }

      fs.stat('file', cb);
    }

<!--
Note: the nextTick queue is completely drained on each pass of the
event loop **before** additional I/O is processed.  As a result,
recursively setting nextTick callbacks will block any I/O from
happening, just like a `while(true);` loop.
-->

注意: イベントループの各繰り返しでは、I/O が処理される **前** に
nextTick のキューが完全に空にされます。
この結果、再帰的に設定された nextTick のコールバックは、
`while (true);` ループのように全てのI/O をブロックします

## process.umask([mask])

<!--
Sets or reads the process's file mode creation mask. Child processes inherit
the mask from the parent process. Returns the old mask if `mask` argument is
given, otherwise returns the current mask.
-->

プロセスのファイルモード作成マスクを設定または読み込みます。
子プロセスは親プロセスからマスクを継承します。
`mask` 引数が与えられると元のマスクが返され、そうでなければ現在のマスクが返されます。

    var oldmask, newmask = 0644;

    oldmask = process.umask(newmask);
    console.log('Changed umask from: ' + oldmask.toString(8) +
                ' to ' + newmask.toString(8));


## process.uptime()

<!--
Number of seconds Node has been running.
-->

Node が実行されてからの秒数です。

## process.hrtime()

<!--
Returns the current high-resolution real time in a `[seconds, nanoseconds]`
tuple Array. It is relative to an arbitrary time in the past. It is not
related to the time of day and therefore not subject to clock drift. The
primary use is for measuring performance between intervals.

You may pass in the result of a previous call to `process.hrtime()` to get
a diff reading, useful for benchmarks and measuring intervals:
-->

高分解能な現在時刻を `[seconds, nanoseconds]` の配列で返します。
過去の任意の時間との比較することができます。
それは一日における時刻には関連が無いため、クロックドリフトに影響されません。
主な用途はベンチマークやインターバルの測定です。

以前に `process.hrtime()` を呼び出した結果を渡すことにより、
差分を得ることができます。これはベンチマークやインターバルの測定に便利です。

    var time = process.hrtime();
    // [ 1800216, 25 ]

    setTimeout(function() {
      var diff = process.hrtime(time);
      // [ 1, 552 ]

      console.log('benchmark took %d nanoseconds', diff[0] * 1e9 + diff[1]);
      // benchmark took 1000000527 nanoseconds
    }, 1000);


## Async Listeners

<!-- type=misc -->

    Stability: 1 - Experimental

<!--
The `AsyncListener` API is the JavaScript interface for the `AsyncWrap`
class which allows developers to be notified about key events in the
lifetime of an asynchronous event. Node performs a lot of asynchronous
events internally, and significant use of this API will have a **dramatic
performance impact** on your application.
-->

`AsyncListener` API は、開発者が非同期イベントの有効期間中に
キーとなるイベントの通知を受け取ることを可能にする、
`AsyncWrap` クラスへの JavaScript インターフェースです。
Node は内部的に多くの非同期イベントを扱うため、この API を使用することは
アプリケーションのパフォーマンスに **重大な影響** を与えます。


## process.createAsyncListener(asyncListener[, callbacksObj[, storageValue]])

<!--
* `asyncListener` {Function} callback fired when an asynchronous event is
instantiated.
* `callbacksObj` {Object} optional callbacks that will fire at specific
times in the lifetime of the asynchronous event.
* `storageValue` {Value} a value that will be passed as the first argument
when the `asyncListener` callback is run, and to all subsequent callback.
-->

* `asyncListener` {Function} 非同期イベントが生成されると呼び出される
  コールバック。
* `callbacksObj` {Object} 非同期イベントの有効期間中に、指定された回数だけ
  呼び出されるオプションのコールバック。
* `storageValue` {Value} `asyncListener` とそれに続くコールバックが
  実行される時、最初の引数として渡される値。

<!--
Returns a constructed `AsyncListener` object.
-->

構築された `AsyncListener` オブジェクトを返します。

<!--
To begin capturing asynchronous events pass the object to
[`process.addAsyncListener()`][]. The same `AsyncListener` instance can
only be added once to the active queue, and subsequent attempts to add the
instance will be ignored.
-->

非同期イベントを捉えるには、[`process.addAsyncListener()`][] に
このオブジェクトを渡してください。
同じ `AsyncListener` インスタンスは一度だけアクティブなキューに加えられます。
それ以降、インスタンスの追加を試みても無視されます。

<!--
To stop capturing pass the object to [`process.removeAsyncListener()`][].
This does _not_ mean the `AsyncListener` previously added will stop
triggering callbacks. Once attached to an asynchronous event it will
persist with the lifetime of the asynchronous call stack.
-->

イベントの捕捉を止めるには、このオブジェクトを
[`process.removeAsyncListener()`][] に渡してください。
それは以前に加えられた `AsyncListener` がコールバックされなくなることを
意味 _しません_。
一度非同期イベントに加えられると、それは非同期処理のコールスタックが有効な間、
持続します。

<!--
Explanation of function parameters:
-->

関数のパラメータの説明:

<!--
`asyncListener(storageValue)`: A `Function` called when an asynchronous
event is instantiated. If a `Value` is returned then it will be attached
to the event and overwrite any value that had been passed to
`process.createAsyncListener()`'s `storageValue` argument. If an initial
`storageValue` was passed when created, then `asyncListener()` will
receive that as a function argument.
-->

`asyncListener(storageValue)`: 非同期イベントが生成された時に呼び出される関数。
関数がもし `Value` を返すなら、それはイベントに割り当てられ、
`process.createAsyncListener()` の `storageValue` 引数に渡された値を
上書きします。
作成時に `storageValue` の初期値が渡されていた場合、`asyncListener()` は
関数の引数としてそれを受け取ります。

<!--
`callbacksObj`: An `Object` which may contain three optional fields:
-->

`callbacksObj`: オプションのフィールドを3つ持つことができる `Object` です。

<!--
* `before(context, storageValue)`: A `Function` that is called immediately
before the asynchronous callback is about to run. It will be passed both
the `context` (i.e. `this`) of the calling function and the `storageValue`
either returned from `asyncListener` or passed during construction (if
either occurred).
-->

* `before(context, storageValue)`: 非同期コールバックが実行される前に
呼び出される関数です。
それは呼び出される関数の `context` (すなわち `this`) と、
`asyncListener` の戻り値または構築時に渡された値 (両方の場合) のどちらかが
`storageValue` として渡されます。

<!--
* `after(context, storageValue)`: A `Function` called immediately after
the asynchronous event's callback has run. Note this will not be called
if the callback throws and the error is not handled.
-->

* `after(context, storageValue)`: 非同期イベントのコールバックが実行された後で
呼び出される関数です。
コールバックがエラーをスローしてそれが処理されなければ、
これは呼び出されないことに注意してください。

<!--
* `error(storageValue, error)`: A `Function` called if the event's
callback threw. If `error` returns `true` then Node will assume the error
has been properly handled and resume execution normally. When multiple
`error()` callbacks have been registered, only **one** of those callbacks
needs to return `true` for `AsyncListener` to accept that the error has
been handled.
-->

* `error(storageValue, error)`: イベントのコールバックがスローすると
呼び出されます。
もし `error()` が `true` を返すと、Node はエラーが正しく処理されたと見なし、
正常に実行を再開します。
複数の `error()` が登録されている場合、エラーが処理されたと `AsyncListener` に
認められるには、**一つ** だけでもコールバックが `true` を返す必要があります。

<!--
`storageValue`: A `Value` (i.e. anything) that will be, by default,
attached to all new event instances. This will be overwritten if a `Value`
is returned by `asyncListener()`.
-->

* `storageValue`: デフォルトで、新しいイベントのインスタンスに割り当てられる値
(つまり何でも)。`asyncListener()` が返す値によって上書きされるかもしれません。

<!--
Here is an example of overwriting the `storageValue`:
-->

`storageValue` が上書きされる例です:

    process.createAsyncListener(function listener(value) {
      // value === true
      return false;
    }, {
      before: function before(context, value) {
        // value === false
      }
    }, true);

<!--
**Note:** The [EventEmitter][], while used to emit status of an asynchronous
event, is not itself asynchronous. So `asyncListener()` will not fire when
an event is added, and `before`/`after` will not fire when emitted
callbacks are called.
-->

**注意:** [EventEmitter][] は、非同期イベントを生成するために使われますが、
それ自身は非同期ではありません。
そのため、イベントが加えられても `asyncListener()` は呼び出されず、
コールバックが呼び出されても `before`/`after` は呼び出されません。


## process.addAsyncListener(asyncListener[, callbacksObj[, storageValue]])
## process.addAsyncListener(asyncListener)

<!--
Returns a constructed `AsyncListener` object and immediately adds it to
the listening queue to begin capturing asynchronous events.
-->

`AsyncListener` オブジェクトを構築し、それを非同期イベントを捕捉するキューに
加えて返します。

<!--
Function parameters can either be the same as
[`process.createAsyncListener()`][], or a constructed `AsyncListener`
object.
-->

関数の引数は [`process.createAsyncListener()`][] と同じか、
構築された `AsyncListener` オブジェクトです。

<!--
Example usage for capturing errors:
-->

エラーを捕捉する例:

    var cntr = 0;
    var key = process.addAsyncListener(function() {
      return { uid: cntr++ };
    }, {
      before: function onBefore(context, storage) {
        // Need to remove the listener while logging or will end up
        // with an infinite call loop.
        process.removeAsyncListener(key);
        console.log('uid: %s is about to run', storage.uid);
        process.addAsyncListener(key);
      },
      after: function onAfter(context, storage) {
        process.removeAsyncListener(key);
        console.log('uid: %s is about to run', storage.uid);
        process.addAsyncListener(key);
      },
      error: function onError(storage, err) {
        // Handle known errors
        if (err.message === 'really, it\'s ok') {
          process.removeAsyncListener(key);
          console.log('handled error just threw:');
          console.log(err.stack);
          process.addAsyncListener(key);
          return true;
        }
      }
    });

    process.nextTick(function() {
      throw new Error('really, it\'s ok');
    });

    // Output:
    // uid: 0 is about to run
    // handled error just threw:
    // Error: really, it's ok
    //     at /tmp/test2.js:27:9
    //     at process._tickCallback (node.js:583:11)
    //     at Function.Module.runMain (module.js:492:11)
    //     at startup (node.js:123:16)
    //     at node.js:1012:3

## process.removeAsyncListener(asyncListener)

<!--
Removes the `AsyncListener` from the listening queue.
-->

`AsyncListener` を監視キューから削除します。

<!--
Removing the `AsyncListener` from the queue does _not_ mean asynchronous
events called during its execution scope will stop firing callbacks. Once
attached to an event it will persist for the entire asynchronous call
stack. For example:
-->

キューから `AsyncLister` を削除することは、その実行スコープの間で
呼ばれる非同期イベントによるコールバック呼び出しの中止を意味しません。

    var key = process.createAsyncListener(function asyncListener() {
      // To log we must stop listening or we'll enter infinite recursion.
      process.removeAsyncListener(key);
      console.log('You summoned me?');
      process.addAsyncListener(key);
    });

    // We want to begin capturing async events some time in the future.
    setTimeout(function() {
      process.addAsyncListener(key);

      // Perform a few additional async events.
      setTimeout(function() {
        setImmediate(function() {
          process.nextTick(function() { });
        });
      });

      // Removing the listener doesn't mean to stop capturing events that
      // have already been added.
      process.removeAsyncListener(key);
    }, 100);

    // Output:
    // You summoned me?
    // You summoned me?
    // You summoned me?
    // You summoned me?

<!--
The fact that we logged 4 asynchronous events is an implementation detail
of Node's [Timers][].
-->

4つの非同期イベントを記録したという事実は、Node の [Timers][] 実装の詳細を
表しています。

<!--
To stop capturing from a specific asynchronous event stack
`process.removeAsyncListener()` must be called from within the call
stack itself. For example:
-->

非同期イベントの捕捉を特定のスタック上で止めたければ、
`process.removeAsyncListener()` はそのコールスタック自身で呼び出すべきです。
たとえば:

    var key = process.createAsyncListener(function asyncListener() {
      // To log we must stop listening or we'll enter infinite recursion.
      process.removeAsyncListener(key);
      console.log('You summoned me?');
      process.addAsyncListener(key);
    });

    // We want to begin capturing async events some time in the future.
    setTimeout(function() {
      process.addAsyncListener(key);

      // Perform a few additional async events.
      setImmediate(function() {
        // Stop capturing from this call stack.
        process.removeAsyncListener(key);

        process.nextTick(function() { });
      });
    }, 100);

    // Output:
    // You summoned me?

<!--
The user must be explicit and always pass the `AsyncListener` they wish
to remove. It is not possible to simply remove all listeners at once.
-->

ユーザは削除したい `AsyncListener` をいつでも明示的に渡すべきです。
すべてのリスナを一度に削除することは出来ません。


[EventEmitter]: events.html#events_class_events_eventemitter
[Timers]: timers.html
[`process.createAsyncListener()`]: #process_process_createasynclistener_asynclistener_callbacksobj_storagevalue
[`process.addAsyncListener()`]: #process_process_addasynclistener_asynclistener
[`process.removeAsyncListener()`]: #process_process_removeasynclistener_asynclistener
