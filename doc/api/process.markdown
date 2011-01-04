## process

<!--

The `process` object is a global object and can be accessed from anywhere.
It is an instance of `EventEmitter`.

-->
`process` はグローバルオブジェクトで、どこからでもアクセスすることができます。
それは `EventEmitter` のインスタンスです。


### Event: 'exit'

`function () {}`

<!--

Emitted when the process is about to exit.  This is a good hook to perform
constant time checks of the module's state (like for unit tests).  The main
event loop will no longer be run after the 'exit' callback finishes, so
timers may not be scheduled.

-->
プロセスが終了しようとしている時に生成されます。
これは (ユニットテストのように) モジュールの状態を一定の時間でチェックするのに適したフックとなります。
メインのイベントループは 'exit' コールバックが終了するともはや動作しないので、
タイマーはスケジュールされないかもしれません。

<!--

Example of listening for `exit`:

-->
`exit` を監視する例:

    process.on('exit', function () {
      process.nextTick(function () {
       console.log('This will not run');
      });
      console.log('About to exit.');
    });

### Event: 'uncaughtException'

`function (err) { }`

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

    process.on('uncaughtException', function (err) {
      console.log('Caught exception: ' + err);
    });

    setTimeout(function () {
      console.log('This will still run.');
    }, 500);

    // Intentionally cause an exception, but don't catch it.
    nonexistentFunc();
    console.log('This will not run.');

<!--

Note that `uncaughtException` is a very crude mechanism for exception
handling.  Using try / catch in your program will give you more control over
your program's flow.  Especially for server programs that are designed to
stay running forever, `uncaughtException` can be a useful safety mechanism.

-->
`uncaughtException` は例外を扱うとても荒削りなメカニズムであることに注意してください。
プログラムの中で try / catch を使えばもっとプログラムの流れをうまく制御できるでしょう。
特にサーバプログラムはいつまでも実行し続けるように設計されるので、
`uncaughtException` は有益で安全なメカニズムになり得ます。


### Signal Events

`function () {}`

<!--

Emitted when the processes receives a signal. See sigaction(2) for a list of
standard POSIX signal names such as SIGINT, SIGUSR1, etc.

-->
プロセスがシグナルを受信た場合に生成されます。
SIGINT、SIGUSR1、その他の POSIX 標準シグナル名の一覧について は sigaction(2) を参照してください。

<!--

Example of listening for `SIGINT`:

-->
`SIGINT`を監視する例:

    // Start reading from stdin so we don't exit.
    process.stdin.resume();

    process.on('SIGINT', function () {
      console.log('Got SIGINT.  Press Control-D to exit.');
    });

<!--

An easy way to send the `SIGINT` signal is with `Control-C` in most terminal
programs.

-->
多くの端末プログラムで簡単に `SIGINT` を送る方法は `Control-C` を押すことです。


### process.stdout

<!--

A `Writable Stream` to `stdout`.

-->
`stdout` に対する `Writable Stream` です。

<!--

Example: the definition of `console.log`

-->
例: `console.log` の定義

    console.log = function (d) {
      process.stdout.write(d + '\n');
    };


### process.stdin

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

    process.stdin.on('data', function (chunk) {
      process.stdout.write('data: ' + chunk);
    });

    process.stdin.on('end', function () {
      process.stdout.write('end');
    });


### process.argv

<!--

An array containing the command line arguments.  The first element will be
'node', the second element will be the name of the JavaScript file.  The
next elements will be any additional command line arguments.

-->
コマンドライン引数を含む配列です。
最初の要素は 'node'、2 番目の要素は JavaScript ファイルの名前になります。
その後の要素はコマンドラインの追加の引数になります。

    // print process.argv
    process.argv.forEach(function (val, index, array) {
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


### process.execPath

<!--

This is the absolute pathname of the executable that started the process.

-->
プロセスによって開始された実行可能ファイルの絶対パスです。

<!--

Example:

-->
例:

    /usr/local/bin/node


### process.chdir(directory)

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



### process.cwd()

<!--

Returns the current working directory of the process.

-->
プロセスのカレントワーキングディレクトリを返します。

    console.log('Current directory: ' + process.cwd());


### process.env

<!--

An object containing the user environment. See environ(7).

-->
ユーザの環境を含むオブジェクトです。environ(7) を参照してください。


### process.exit(code=0)

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


### process.getgid()

<!--

Gets the group identity of the process. (See getgid(2).)
This is the numerical group id, not the group name.

-->
プロセスのグループ識別子を取得します (getgid(2) 参照)。
これは数値によるグループ ID で、グループ名ではありません。

    console.log('Current gid: ' + process.getgid());


### process.setgid(id)

<!--

Sets the group identity of the process. (See setgid(2).)  This accepts either
a numerical ID or a groupname string. If a groupname is specified, this method
blocks while resolving it to a numerical ID.

-->
プロセスのグループ識別子を設定します (setgid(2) 参照)。
これは数値による ID もグループ名の文字列のどちらも受け入れます。
もしグループ名が指定されると、数値による ID が解決できるまでこのメソッドはブロックします。

    console.log('Current gid: ' + process.getgid());
    try {
      process.setgid(501);
      console.log('New gid: ' + process.getgid());
    }
    catch (err) {
      console.log('Failed to set gid: ' + err);
    }


### process.getuid()

<!--

Gets the user identity of the process. (See getuid(2).)
This is the numerical userid, not the username.

-->
プロセスのユーザ識別子を取得します (getuid(2) 参照)。
これは数値によるユーザ ID で、ユーザ名ではありません。

    console.log('Current uid: ' + process.getuid());


### process.setuid(id)

<!--

Sets the user identity of the process. (See setuid(2).)  This accepts either
a numerical ID or a username string.  If a username is specified, this method
blocks while resolving it to a numerical ID.

-->
プロセスのユーザ識別子を設定します (setuid(2) 参照)。
これは数値による ID もユーザ名の文字列のどちらも受け入れます。
もしユーザ名が指定されると、数値による ID が解決できるまでこのメソッドはブロックします。

    console.log('Current uid: ' + process.getuid());
    try {
      process.setuid(501);
      console.log('New uid: ' + process.getuid());
    }
    catch (err) {
      console.log('Failed to set uid: ' + err);
    }


### process.version

<!--

A compiled-in property that exposes `NODE_VERSION`.

-->
`NODE_VERSION` を提示するコンパイル済みプロパティです。

    console.log('Version: ' + process.version);

### process.installPrefix

<--
A compiled-in property that exposes `NODE_PREFIX`.

-->
`NODE_PREFIX` を提示するコンパイル済みプロパティです。

    console.log('Prefix: ' + process.installPrefix);


### process.kill(pid, signal='SIGINT')

<!--

Send a signal to a process. `pid` is the process id and `signal` is the
string describing the signal to send.  Signal names are strings like
'SIGINT' or 'SIGUSR1'.  If omitted, the signal will be 'SIGINT'.
See kill(2) for more information.

-->
プロセスにシグナルを送ります。
`pid` はプロセス ID で `signal` は送信されるシグナルを文字列で記述したものです。
シグナルの名前は 'SIGINT' や 'SIGUSR1' のような文字列です。
省略すると、シグナルは 'SIGINT' となります。
詳細は kill(2) を参照してください。

<!--

Note that just because the name of this function is `process.kill`, it is
really just a signal sender, like the `kill` system call.  The signal sent
may do something other than kill the target process.

-->
この関数の名前が `process.kill` であるとおり、
これは `kill` システムコールのように本当にシグナルを送信することに注意してください。
対象のプロセスを殺すだけでなく、他のシグナルも送信されます。

<!--

Example of sending a signal to yourself:

-->
自身にシグナルを送信する例:

    process.on('SIGHUP', function () {
      console.log('Got SIGHUP signal.');
    });

    setTimeout(function () {
      console.log('Exiting.');
      process.exit(0);
    }, 100);

    process.kill(process.pid, 'SIGHUP');


### process.pid

<!--

The PID of the process.

-->

    console.log('This process is pid ' + process.pid);

### process.title

<!--

Getter/setter to set what is displayed in 'ps'.

-->
'ps' でどのよう表示されるかを設定するための getter/setter です。


### process.platform

<!--

What platform you're running on. `'linux2'`, `'darwin'`, etc.

-->
どのプラットフォームで動いているかです。`'linux2'`、`'darwin'`、など。

    console.log('This platform is ' + process.platform);


### process.memoryUsage()

<!--

Returns an object describing the memory usage of the Node process.

-->
Node プロセスのメモリ使用状況を記述したオブジェクトを返します。

    var util = require('util');

    console.log(util.inspect(process.memoryUsage()));

<!--

This will generate:

-->
このように生成されます:

    { rss: 4935680,
      vsize: 41893888,
      heapTotal: 1826816,
      heapUsed: 650472 }

<!--

`heapTotal` and `heapUsed` refer to V8's memory usage.

-->
`heapTotal` と `heapUsed` は V8 のメモリ使用状況を参照します。


### process.nextTick(callback)

<!--

On the next loop around the event loop call this callback.
This is *not* a simple alias to `setTimeout(fn, 0)`, it's much more
efficient.

-->
イベントループの次以降のループでコールバックを呼び出します。
これは `setTimeout(fn, 0)` の単純なエイリアス*ではなく*、
はるかに効率的です。

    process.nextTick(function () {
      console.log('nextTick callback');
    });


### process.umask([mask])

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

