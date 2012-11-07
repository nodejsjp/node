# Child Process

    Stability: 3 - Stable

<!--
Node provides a tri-directional `popen(3)` facility through the
`child_process` module.
-->

Nodeは `child_process` モジュールを通じて、3 方向の `popen(3)`
機能を提供します。

<!--
It is possible to stream data through a child's `stdin`, `stdout`, and
`stderr` in a fully non-blocking way.
-->

これは完全にノンブロッキングな方法で子プロセスの `stdin`、`stdout`、
そして `stderr` を通じたデータストリームを実現します。

<!--
To create a child process use `require('child_process').spawn()`.
To create a child process use `require('child_process').spawn()` or
`require('child_process').fork()`.  The semantics of each are slightly
different, and explained below.
-->

子プロセスの生成は `require('child_process').spawn()` または
`require('child_process').fork()` を使います。
それぞれの意味論は以下で説明するようにわずかに異なります。

## Class: ChildProcess

<!--
`ChildProcess` is an [EventEmitter][].
-->

`ChildProcess` は [EventEmitter][] です。

<!--
Child processes always have three streams associated with them. `child.stdin`,
`child.stdout`, and `child.stderr`.  These may be shared with the stdio
streams of the parent process, or they may be separate stream objects
which can be piped to and from.

The ChildProcess class is not intended to be used directly.  Use the
`spawn()` or `fork()` methods to create a Child Process instance.
-->

子プロセスは常に 3 本のストリームと関連づけられています。
`child.stdin`、`child.stdout`、そして `child.stderr` です。
それらは親プロセスの標準入出力ストリームを共有するかもしれませんし、
独立したストリームオブジェクトにパイプでつながれているかもしれません。

### Event:  'exit'

<!--
* `code` {Number} the exit code, if it exited normally.
* `signal` {String} the signal passed to kill the child process, if it
  was killed by the parent.
-->

* `code` {Number} 通常に終了した場合は終了コード。
* `signal` {String} 親プロセスによって終了させられた場合は、
子プロセスを終了するために送られたシグナル。

<!--
This event is emitted after the child process ends. If the process terminated
normally, `code` is the final exit code of the process, otherwise `null`. If
the process terminated due to receipt of a signal, `signal` is the string name
of the signal, otherwise `null`.
-->

このイベントは子プロセスが終了した後で生成されます。
プロセスが普通に終了した場合、`code` はプロセスの終了コードです。
それ以外の場合は `null` です。
プロセスがシグナルを受け取って終了した場合、`signal` は文字列によるシグナルの名前です。
それ以外の場合は `null` です。

<!--
Note that the child process stdio streams might still be open.
-->

子プロセスの標準入出力ストリームはオープンしたままになっているかも
しれないことに注意してください。

<!--
See `waitpid(2)`.
-->

`waitpid(2)` を参照してください。

### Event: 'close'

<!--
This event is emitted when the stdio streams of a child process have all
terminated.  This is distinct from 'exit', since multiple processes
might share the same stdio streams.
-->

このイベントは、子プロセスの標準入出力ストリームが全て終了した場合に
生成されます。
複数のプロセスが同じ標準入出力ストリームを共有するかもしれないので、
これは `'exit'` とは明確に異なります。

### Event: 'disconnect'

<!--
This event is emitted after using the `.disconnect()` method in the parent or
in the child. After disconnecting it is no longer possible to send messages.
An alternative way to check if you can send messages is to see if the
`child.connected` property is `true`.
-->

このイベントは、親プロセスまたは子プロセスで `disconnect()` メソッドが
呼び出された場合に生成されます。
この後では、プロセス間でのメッセージ送信はもうできません。
メッセージを送信できるかを調べるには、`child.connected` プロパティが `true`
かチェックしてください。

### Event: 'message'

<!--
* `message` {Object} a parsed JSON object or primitive value
* `sendHandle` {Handle object} a Socket or Server object
-->

* `message` {Object} 解析済みの JSON オブジェクトまたはプリミティブ値
* `sendHandle` {Handle object} ソケットまたはサーバオブジェクト

<!--
Messages send by `.send(message, [sendHandle])` are obtained using the
`message` event.
-->

`.send(message, [sendHandle])` によって送信されたメッセージは `'message'`
イベントによって取得できます。

### child.stdin

* {Stream object}

<!--
A `Writable Stream` that represents the child process's `stdin`.
Closing this stream via `end()` often causes the child process to terminate.

If the child stdio streams are shared with the parent, then this will
not be set.
-->

子プロセスの `stdin` を表現する `Writable Stream` です。
多くの場合、`end()` を通じてこのストリームを閉じると子プロセスが終了する原因となります。

子プロセスの標準入出力が親プロセスと共有されている場合は設定されません。

### child.stdout

* {Stream object}

<!--
A `Readable Stream` that represents the child process's `stdout`.

If the child stdio streams are shared with the parent, then this will
not be set.
-->

子プロセスの `stdout` を表現する `Readable Stream` です。

子プロセスの標準入出力が親プロセスと共有されている場合は設定されません。

### child.stderr

* {Stream object}

<!--
A `Readable Stream` that represents the child process's `stderr`.

If the child stdio streams are shared with the parent, then this will
not be set.
-->

子プロセスの `stderr` を表現する `Readable Stream` です。

子プロセスの標準入出力が親プロセスと共有されている場合は設定されません。

### child.pid

* {Integer}

<!--
The PID of the child process.
-->

子プロセスの PID です。

<!--
Example:
-->

例:

    var spawn = require('child_process').spawn,
        grep  = spawn('grep', ['ssh']);

    console.log('Spawned child pid: ' + grep.pid);
    grep.stdin.end();

### child.kill([signal])

* `signal` {String}

<!--
Send a signal to the child process. If no argument is given, the process will
be sent `'SIGTERM'`. See `signal(7)` for a list of available signals.
-->

子プロセスにシグナルを送ります。
引数が与えられない場合、子プロセスには `'SIGTERM'` が送られます。
利用可能なシグナルの一覧は `signal(7)` を参照してください。


    var spawn = require('child_process').spawn,
        grep  = spawn('grep', ['ssh']);

    grep.on('exit', function (code, signal) {
      console.log('child process terminated due to receipt of signal '+signal);
    });

    // send SIGHUP to process
    grep.kill('SIGHUP');

<!--
Note that while the function is called `kill`, the signal delivered to the child
process may not actually kill it.  `kill` really just sends a signal to a process.
-->

この関数は `kill` と呼ばれるものの、
子プロセスに届けられるシグナルが実際には子プロセスを殺さないかもしれないことに注意してください。
`kill` はただプロセスにシグナルを送るだけです。

<!--
See `kill(2)`
-->

`kill(2)` を参照してください。

### child.send(message, [sendHandle])

* `message` {Object}
* `sendHandle` {Handle object}

<!--
When using `child_process.fork()` you can write to the child using
`child.send(message, [sendHandle])` and messages are received by
a `'message'` event on the child.
-->

`child_process.fork()` を使うと、`child.send(message, [sendHandle])` を
使って子プロセスにメッセージを送信し、子プロセスではそれを `'message'`
イベントによって受け取ることができます。

<!--
For example:
-->

例:

    var cp = require('child_process');

    var n = cp.fork(__dirname + '/sub.js');

    n.on('message', function(m) {
      console.log('PARENT got message:', m);
    });

    n.send({ hello: 'world' });

<!--
And then the child script, `'sub.js'` might look like this:
-->

子プロセスの `'sub.js'` は次のようになります:

    process.on('message', function(m) {
      console.log('CHILD got message:', m);
    });

    process.send({ foo: 'bar' });

<!--
In the child the `process` object will have a `send()` method, and `process`
will emit objects each time it receives a message on its channel.
-->

子プロセスでは `process` オブジェクトは `send()` メソッドを持ち、
そのチャネル上でメッセージを受信するたびにイベントを生成します。

<!--
There is a special case when sending a `{cmd: 'NODE_foo'}` message. All messages
containing a `NODE_` prefix in its `cmd` property will not be emitted in
the `message` event, since they are internal messages used by node core.
Messages containing the prefix are emitted in the `internalMessage` event, you
should by all means avoid using this feature, it is subject to change without notice.
-->

特別なケースとして、`{cmd: 'NODE_foo'}` のようなメッセージを
送信する場合があります。
`cmd` プロパティが接頭辞 `NODE_` を含む全てのメッセージは node のコアで
使われる内部的なメッセージであるため、`'message'` イベントを生成しません。
この接頭辞を含むメッセージは `'internalMessage'` イベントを生成しますが、
それを使用すべきではありません。それは保証なしに変更される可能性があります。

<!--
The `sendHandle` option to `child.send()` is for sending a TCP server or
socket object to another process. The child will receive the object as its
second argument to the `message` event.
-->

`child.send()` の `sendHandle` オプションは TCP サーバまたは
ソケットオブジェクトを他のプロセスに送信するためのものです。
子プロセスはそれを `'message'` イベントの第 2 引数として受信します。

<!--
**send server object**
-->

**サーバオブジェクトを送信する**

<!--
Here is an example of sending a server:
-->

サーバを送信する例:

    var child = require('child_process').fork('child.js');

    // Open up the server object and send the handle.
    var server = require('net').createServer();
    server.on('connection', function (socket) {
      socket.end('handled by parent');
    });
    server.listen(1337, function() {
      child.send('server', server);
    });

<!--
And the child would the receive the server object as:
-->

サーバオブジェクトを受信する子プロセス:

    process.on('message', function(m, server) {
      if (m === 'server') {
        server.on('connection', function (socket) {
          socket.end('handled by child');
        });
      }
    });

<!--
Note that the server is now shared between the parent and child, this means
that some connections will be handled by the parent and some by the child.
-->

サーバは親プロセスと子プロセスで共有されることに注意してください。
これはコネクションが時には親あるいは子で処理されることを意味します。

<!--
**send socket object**
-->

**ソケットオブジェクトを送信する**

<!--
Here is an example of sending a socket. It will spawn two children and handle
connections with the remote address `74.125.127.100` as VIP by sending the
socket to a "special" child process. Other sockets will go to a "normal" process.
-->

これはソケットを送信する例です。
これは二つの子プロセスを起動し、コネクションのリモートアドレスが VIP
(`74.125.127.100`) ならソケットを "special" 子プロセスに送信します。
その他のソケットは "normal" プロセスに送られます。

    var normal = require('child_process').fork('child.js', ['normal']);
    var special = require('child_process').fork('child.js', ['special']);

    // Open up the server and send sockets to child
    var server = require('net').createServer();
    server.on('connection', function (socket) {

      // if this is a VIP
      if (socket.remoteAddress === '74.125.127.100') {
        special.send('socket', socket);
        return;
      }
      // just the usual dudes
      normal.send('socket', socket);
    });
    server.listen(1337);

<!--
The `child.js` could look like this:
-->

`chold.js` は次のようになります:

    process.on('message', function(m, socket) {
      if (m === 'socket') {
        socket.end('You were handled as a ' + process.argv[2] + ' person');
      }
    });

<!--
Note that once a single socket has been sent to a child the parent can no
longer keep track of when the socket is destroyed. To indicate this condition
the `.connections` property becomes `null`.
It is also recommended not to use `.maxConnections` in this condition.
-->

一度ソケットが子プロセスに送信されると、親プロセスはもうソケットがいつ
破棄されるか知ることができないことに注意してください。
この状態を示すために，`.connections` プロパティは `null` になります。
この状態では、`.maxConnections` も使わないことを推奨します。

### child.disconnect()

<!--
To close the IPC connection between parent and child use the
`child.disconnect()` method. This allows the child to exit gracefully since
there is no IPC channel keeping it alive. When calling this method the
`disconnect` event will be emitted in both parent and child, and the
`connected` flag will be set to `false`. Please note that you can also call
`process.disconnect()` in the child process.
-->

親プロセスと子プロセス間の IPC コネクションをクローズするには
`child.disconnect()` メソッドを使用します。
これは、子プロセスが IPC チャネルを保持しないことにより、強制的ではない
終了を可能にします。
このメソッドを呼び出すと親プロセスと子プロセスの両方で `'disconnect'`
イベントが生成され、`connected` フラグが `false` に設定されます。
子プロセスで `process.disconnect()` を呼び出すことも可能であることに
注意してください。

## child_process.spawn(command, [args], [options])

<!--
* `command` {String} The command to run
* `args` {Array} List of string arguments
* `options` {Object}
  * `cwd` {String} Current working directory of the child process
  * `stdio` {Array|String} Child's stdio configuration. (See below)
  * `customFds` {Array} **Deprecated** File descriptors for the child to use
    for stdio.  (See below)
  * `env` {Object} Environment key-value pairs
  * `detached` {Boolean} The child will be a process group leader.  (See below)
* return: {ChildProcess object}
-->

* `command` {String} 実行するコマンド
* `args` {Array} 文字列による引数の配列
* `options` {Object}
  * `cwd` {String} 子プロセスのカレントワーキングディレクトリ
  * `stdio` {Array|String} 子プロセスの標準入出力の設定 (後述)。
  * `customFds` {Array} **Deprecated** 子プロセスが標準入出力として使用する
    ファイル記述子の配列 (後述)
  * `env` {Object} 環境変数として与えるキー・値のペア
  * `detached` {Boolean} 子プロセスがプロセスグループのリーダになるかどうか
    (後述)。
* return: {ChildProcess object}

<!--
Launches a new process with the given `command`, with  command line arguments in `args`.
If omitted, `args` defaults to an empty Array.
-->

`args` をコマンドライン引数として、与えられた `command` で新しいプロセスを起動します。
`args` が省略された場合、空の配列がデフォルトとなります。

<!--
The third argument is used to specify additional options, which defaults to:
-->

第 3 引数は追加のオプションを指定するために使われ、そのデフォルトは:

    { cwd: undefined,
      env: process.env
    }

<!--
`cwd` allows you to specify the working directory from which the process is spawned.
Use `env` to specify environment variables that will be visible to the new process.
-->

`cwd` で起動されたプロセスのワーキングディレクトリを指定することができます。
`env` は新しいプロセスに見える環境変数を指定するために使います。

<!--
Example of running `ls -lh /usr`, capturing `stdout`, `stderr`, and the exit code:
-->

ls -lh /usr` を実行して `stdout`、`stderr`、および終了コードを取得する例:

    var spawn = require('child_process').spawn,
        ls    = spawn('ls', ['-lh', '/usr']);

    ls.stdout.on('data', function (data) {
      console.log('stdout: ' + data);
    });

    ls.stderr.on('data', function (data) {
      console.log('stderr: ' + data);
    });

    ls.on('exit', function (code) {
      console.log('child process exited with code ' + code);
    });


<!--
Example: A very elaborate way to run 'ps ax | grep ssh'
-->

とても手の込んだ方法で実行する 'ps ax | grep ssh' の例:

    var spawn = require('child_process').spawn,
        ps    = spawn('ps', ['ax']),
        grep  = spawn('grep', ['ssh']);

    ps.stdout.on('data', function (data) {
      grep.stdin.write(data);
    });

    ps.stderr.on('data', function (data) {
      console.log('ps stderr: ' + data);
    });

    ps.on('exit', function (code) {
      if (code !== 0) {
        console.log('ps process exited with code ' + code);
      }
      grep.stdin.end();
    });

    grep.stdout.on('data', function (data) {
      console.log('' + data);
    });

    grep.stderr.on('data', function (data) {
      console.log('grep stderr: ' + data);
    });

    grep.on('exit', function (code) {
      if (code !== 0) {
        console.log('grep process exited with code ' + code);
      }
    });


<!--
Example of checking for failed exec:
-->

exec の失敗をチェックする例:

    var spawn = require('child_process').spawn,
        child = spawn('bad_command');

    child.stderr.setEncoding('utf8');
    child.stderr.on('data', function (data) {
      if (/^execvp\(\)/.test(data)) {
        console.log('Failed to start child process.');
      }
    });

<!--
Note that if spawn receives an empty options object, it will result in
spawning the process with an empty environment rather than using
`process.env`. This due to backwards compatibility issues with a deprecated
API.
-->

`spawn()` は空の `options` オブジェクトを受け取ると、
`process.env` を使うのではなく，空の環境変数で子プロセスを起動します。
これは廃止された API との互換性のためです。

<!--
The 'stdio' option to `child_process.spawn()` is an array where each
index corresponds to a fd in the child.  The value is one of the following:
-->

`child_process.spawn()` の `stdio` オプションは配列で、
それぞれのインデックスは子プロセスの fd に対応します。
要素の値は以下のいずれかです:

<!--
1. `'pipe'` - Create a pipe between the child process and the parent process.
   The parent end of the pipe is exposed to the parent as a property on the
   `child_process` object as `ChildProcess.stdio[fd]`. Pipes created for
   fds 0 - 2 are also available as ChildProcess.stdin, ChildProcess.stdout
   and ChildProcess.stderr, respectively.
2. `'ipc'` - Create an IPC channel for passing messages/file descriptors
   between parent and child. A ChildProcess may have at most *one* IPC stdio
   file descriptor. Setting this option enables the ChildProcess.send() method.
   If the child writes JSON messages to this file descriptor, then this will
   trigger ChildProcess.on('message').  If the child is a Node.js program, then
   the presence of an IPC channel will enable process.send() and
   process.on('message').
3. `'ignore'` - Do not set this file descriptor in the child. Note that Node
   will always open fd 0 - 2 for the processes it spawns. When any of these is
   ignored node will open `/dev/null` and attach it to the child's fd.
4. `Stream` object - Share a readable or writable stream that refers to a tty,
   file, socket, or a pipe with the child process. The stream's underlying
   file descriptor is duplicated in the child process to the fd that 
   corresponds to the index in the `stdio` array.
5. Positive integer - The integer value is interpreted as a file descriptor 
   that is is currently open in the parent process. It is shared with the child
   process, similar to how `Stream` objects can be shared.
6. `null`, `undefined` - Use default value. For stdio fds 0, 1 and 2 (in other
   words, stdin, stdout, and stderr) a pipe is created. For fd 3 and up, the
   default is `'ignore'`.
-->

1. `'pipe'` - 子プロセスと親プロセスの間でパイプを作成します。
   パイプの親側の端点は `child_process` オブジェクトのプロパティ
   `ChildProcess.stdio[fd]` として親プロセスに公開されます。
   fd 0～2 はそれぞれ、`ChildProcess.stdin`、`ChildProcess.stdout`、
   `ChildProcess.stderr` としても参照可能です。
2. `'ipc'` - 親プロセスと子プロセスの間でメッセージパッシングのための
   IPC チャネル／ファイル記述子を作成します。
   `ChildProcess` は標準入出力に高々一つの IPC ファイル記述子を持ちます。
   このオプションを設定すると、`ChildProcess.send()` メソッドが有効になります。
   子プロセスがこのファイル記述子に JSON メッセージを書き込むと、
   それは `ChildProcess.on('message')` を引き起こします。
   子プロセスが Node.js プログラムなら、IPC チャネルの存在は `process.send()`
   および `process.on('message')` を有効にします。
3. `'ignore'` - 子プロセスにファイル記述子を設定しません。
   Node は子プロセスを起動する際、常に fd 0～2 をオープンすることに
   注意してください。これらのうちのどれかが `'ignore'` の場合、node は
   `/dev/null` をオープンして、それを子プロセスの fd に割り当てます。
4. `Stream` オブジェクト - tty、ファイル、ソケット、またはパイプを参照する
   読み込みまたは書き込み可能なストリームを子プロセスと共有します。
   ストリームの下層にあるファイル記述子は、子プロセスの `stdio` 配列の
   対応する位置にコピーされます。
5. 非負整数 - 整数の値を親プロセスが現在オープンしているファイル記述子として
   解釈されます。
   それは `Stream` オブジェクトの場合と同様に子プロセスに共有されます。
6. `null`、`undefined` - デフォルト値を使用します。
   `stdio` の `fd` が 0、1、または 2 (言い換えると stdin、stdout、または
   stderr) の場合はパイプが作成されます。fd が 3 以上の場合、デフォルトは
   `'ignore'` です。

<!--
As a shorthand, the `stdio` argument may also be one of the following
strings, rather than an array:
-->

簡易な記法として、`stdio` に配列ではなく以下の文字列の一つを指定することも
できます。

<!--
* `ignore` - `['ignore', 'ignore', 'ignore']`
* `pipe` - `['pipe', 'pipe', 'pipe']`
* `inherit` - `[process.stdin, process.stdout, process.stderr]` or `[0,1,2]`
-->

* `ignore` - `['ignore', 'ignore', 'ignore']`
* `pipe` - `['pipe', 'pipe', 'pipe']`
* `inherit` - `[process.stdin, process.stdout, process.stderr]` または `[0,1,2]`

<!--
Example:
-->

例:

    var spawn = require('child_process').spawn;

    // Child will use parent's stdios
    spawn('prg', [], { stdio: 'inherit' });

    // Spawn child sharing only stderr
    spawn('prg', [], { stdio: ['pipe', 'pipe', process.stderr] });

    // Open an extra fd=4, to interact with programs present a
    // startd-style interface.
    spawn('prg', [], { stdio: ['pipe', null, null, null, 'pipe'] });

<!--
If the `detached` option is set, the child process will be made the leader of a
new process group.  This makes it possible for the child to continue running 
after the parent exits.
-->

`detached` オプションが設定されると、子プロセスは新しいプロセスグループの
リーダになります。
これは親プロセスが終了しても子プロセスの実行が継続することを可能にします。

<!--
By default, the parent will wait for the detached child to exit.  To prevent
the parent from waiting for a given `child`, use the `child.unref()` method,
and the parent's event loop will not include the child in its reference count.
-->

デフォルトでは、親プロセスは切り離された子プロセスの終了を待機します。
親プロセスが `child` を待機することを防ぐには、`child.unref()` メソッドを
使用し、親のイベントループに子のリファレンスカウントが含まれないようにします。

<!--
Example of detaching a long-running process and redirecting its output to a
file:
-->

長時間実行する子プロセスを切り離し、出力をファイルにリダイレクトする例:

     var fs = require('fs'),
         spawn = require('child_process').spawn,
         out = fs.openSync('./out.log', 'a'),
         err = fs.openSync('./out.log', 'a');

     var child = spawn('prg', [], {
       detached: true,
       stdio: [ 'ignore', out, err ]
     });

     child.unref();

<!--
When using the `detached` option to start a long-running process, the process
will not stay running in the background unless it is provided with a `stdio`
configuration that is not connected to the parent.  If the parent's `stdio` is
inherited, the child will remain attached to the controlling terminal.
-->

長時間実行されるプロセスを開始するために `detached` オプションを使用する場合、
その `stdio` が親と接続するような構成を与えられない限り、そのプロセスは
バックグラウンドにとどまりません。
親の `stdio` が継承されるなら、子プロセスは制御しているターミナルに
接続されたままです。

<!--
There is a deprecated option called `customFds` which allows one to specify
specific file descriptors for the stdio of the child process. This API was
not portable to all platforms and therefore removed.
With `customFds` it was possible to hook up the new process' `[stdin, stdout,
stderr]` to existing streams; `-1` meant that a new stream should be created.
Use at your own risk.

There are several internal options. In particular `stdinStream`,
`stdoutStream`, `stderrStream`. They are for INTERNAL USE ONLY. As with all
undocumented APIs in Node, they should not be used.

See also: `child_process.exec()` and `child_process.fork()`
-->

指定のファイル記述子を子プロセスの標準入出力に指定することを可能にする、
`customFds` と呼ばれる廃止されたオプションがありました。
この API は全てのプラットフォームに移植可能ではないために削除されました。
`customFds` は新しいプロセスの `[stdin, stdout, stderr]` を既存のストリームに接続することを可能にしました;
`-1` は新しいストリームが作られなければならないことを意味していました。
使用する場合は自己責任で。

よく似た内部的なオプションがあります。
具体的には `stdinStream`、`stdoutStream`、`stderrStream` です。
これらは __内部用途__です。
Node のドキュメント化されていない API と同様に、
これらを使用すべきではありません。

関連項目: `child_process.exec()` および `child_process.fork()`

## child_process.exec(command, [options], callback)

<!--
* `command` {String} The command to run, with space-separated arguments
* `options` {Object}
  * `cwd` {String} Current working directory of the child process
  * `stdio` {Array|String} Child's stdio configuration. (See above)
  * `customFds` {Array} **Deprecated** File descriptors for the child to use
    for stdio.  (See above)
  * `env` {Object} Environment key-value pairs
  * `encoding` {String} (Default: 'utf8')
  * `timeout` {Number} (Default: 0)
  * `maxBuffer` {Number} (Default: 200*1024)
  * `killSignal` {String} (Default: 'SIGTERM')
* `callback` {Function} called with the output when process terminates
  * `error` {Error}
  * `stdout` {Buffer}
  * `stderr` {Buffer}
* Return: ChildProcess object
-->

* `command` {String} 実行するコマンド、空白で区切られた引数を持ちます
* `options` {Object}
  * `cwd` {String} 子プロセスのカレントワーキングディレクトリ
  * `stdio` {Array|String} 子プロセスの標準入出力の設定 (前述)。
  * `customFds` {Array} **Deprecated** 子プロセスが標準入出力として使用する
   ファイル記述子の配列 (前述)
  * `env` {Object} 環境変数として与えるキー・値のペア
  * `encoding` {String} (Default: 'utf8')
  * `timeout` {Number} (Default: 0)
  * `maxBuffer` {Number} (Default: 200*1024)
  * `killSignal` {String} (Default: 'SIGTERM')
* `callback` {Function} プロセスが終了するとその出力を伴って呼び出されます
  * `error` {Error}
  * `stdout` {Buffer}
  * `stderr` {Buffer}
* Return: ChildProcess object

<!--
Runs a command in a shell and buffers the output.
-->

コマンドをシェルで実行し、その出力をバッファに格納します。

    var exec = require('child_process').exec,
        child;

    child = exec('cat *.js bad_file | wc -l',
      function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
          console.log('exec error: ' + error);
        }
    });

<!--
The callback gets the arguments `(error, stdout, stderr)`. On success, `error`
will be `null`.  On error, `error` will be an instance of `Error` and `err.code`
will be the exit code of the child process, and `err.signal` will be set to the
signal that terminated the process.
-->

コールバックは引数 `(error, stdout, stderr)` を得ます。
成功すると、`error` は `null` になります。
エラーだと、`error` は `Error` のインスタンスとなり、
`err.code` は子プロセスの終了コード、
`err.signal` はプロセスを終了させたシグナルとなります。

<!--
There is a second optional argument to specify several options. The
default options are
-->

任意の第 2 引数でいくつかのオプションを指定することができます。
オプションのデフォルトは

    { encoding: 'utf8',
      timeout: 0,
      maxBuffer: 200*1024,
      killSignal: 'SIGTERM',
      cwd: null,
      env: null }

<!--
If `timeout` is greater than 0, then it will kill the child process
if it runs longer than `timeout` milliseconds. The child process is killed with
`killSignal` (default: `'SIGTERM'`). `maxBuffer` specifies the largest
amount of data allowed on stdout or stderr - if this value is exceeded then
the child process is killed.
-->

もし `timeout` が 0 より大きいと、
子プロセスは実行時間が `timeout` ミリ秒よりも長くなると kill されます。
子プロセスは `killSignal` で kill されます (デフォルト: `'SIGTERM'`)。
`maxBuffer` は標準出力と標準エラーの最大のデータ量を指定します － この値を超えると子プロセスは kill されます。


## child_process.execFile(file, args, options, callback)

<!--
* `file` {String} The filename of the program to run
* `args` {Array} List of string arguments
* `options` {Object}
  * `cwd` {String} Current working directory of the child process
  * `stdio` {Array|String} Child's stdio configuration. (See above)
  * `customFds` {Array} **Deprecated** File descriptors for the child to use
    for stdio.  (See above)
  * `env` {Object} Environment key-value pairs
  * `encoding` {String} (Default: 'utf8')
  * `timeout` {Number} (Default: 0)
  * `maxBuffer` {Number} (Default: 200\*1024)
  * `killSignal` {String} (Default: 'SIGTERM')
* `callback` {Function} called with the output when process terminates
  * `error` {Error}
  * `stdout` {Buffer}
  * `stderr` {Buffer}
* Return: ChildProcess object
-->

* `file` {String} 実行するプログラムのファイル名
* `args` {Array} 文字列による引数の配列
* `options` {Object}
  * `cwd` {String} 子プロセスのカレントワーキングディレクトリ
  * `stdio` {Array|String} 子プロセスの標準入出力の設定 (前述)。
  * `customFds` {Array} **Deprecated** 子プロセスが標準入出力として使用する
   ファイル記述子の配列 (前述)
  * `env` {Object} 環境変数として与えるキー・値のペア
  * `encoding` {String} (Default: 'utf8')
  * `timeout` {Number} (Default: 0)
  * `maxBuffer` {Number} (Default: 200\*1024)
  * `killSignal` {String} (Default: 'SIGTERM')
* `callback` {Function} プロセスが終了するとその出力を伴って呼び出されます
  * `error` {Error}
  * `stdout` {Buffer}
  * `stderr` {Buffer}
* Return: ChildProcess object

<!--
This is similar to `child_process.exec()` except it does not execute a
subshell but rather the specified file directly. This makes it slightly
leaner than `child_process.exec`. It has the same options.
-->

子シェルで実行する代わりに指定されたファイルを直接実行することを除いて
`child_process.exec()` と同様です。
これは `child_process.exec` より若干効率的で、同じオプションを持ちます。


## child\_process.fork(modulePath, [args], [options])

<!--
* `modulePath` {String} The module to run in the child
* `args` {Array} List of string arguments
* `options` {Object}
  * `cwd` {String} Current working directory of the child process
  * `env` {Object} Environment key-value pairs
  * `encoding` {String} (Default: 'utf8')
* Return: ChildProcess object
-->

* `modulePath` {String} 子プロセスで実行するモジュール
* `args` {Array} 文字列による引数の配列
* `options` {Object}
  * `cwd` {String} 子プロセスのカレントワーキングディレクトリ
  * `env` {Object} 環境変数として与えるキー・値のペア
  * `encoding` {String} (Default: 'utf8')
* Return: ChildProcess object

<!--
This is a special case of the `spawn()` functionality for spawning Node
processes. In addition to having all the methods in a normal ChildProcess
instance, the returned object has a communication channel built-in. See
`child.send(message, [sendHandle])` for details.
-->

これは `spawn()` の特別版で、Node プロセスを起動します。
返されるオブジェクトは通常の ChildProcess の全てのメソッドに加えて、
組み込みの通信チャネルを持ちます。
詳細は `child.send(message, [sendHandle])` を参照してください。

<!--
By default the spawned Node process will have the stdout, stderr associated
with the parent's. To change this behavior set the `silent` property in the
`options` object to `true`.
-->

デフォルトでは、起動された Node プロセスは親プロセスに関連づけられた標準出力と
標準エラー出力を持ちます。これを変更するには `options` オブジェクトの
`silent` プロパティを `true` に設定します。

<!--
The child process does not automatically exit once it's done, you need to call
`process.exit()` explicitly. This limitation may be lifted in the future.
-->

子プロセスは自動的に終了するわけではないので、
明示的に `process.exit()` を呼び出す必要があります。
この制限は将来取り除かれるかもしれません。

<!--
These child Nodes are still whole new instances of V8. Assume at least 30ms
startup and 10mb memory for each new Node. That is, you cannot create many
thousands of them.
-->

これらの子 Node は、やはり V8 の新しいインスタンスです。
新しい Node ごとに少なくとも 30 ミリ秒の起動時間と 
10MB のメモリを前提としてください。
つまり、数千の子プロセスを作ることは出来ません。

[EventEmitter]: events.html#events_class_events_eventemitter
