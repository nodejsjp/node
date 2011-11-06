## Child Processes

<!--

Node provides a tri-directional `popen(3)` facility through the `ChildProcess`
class.

-->
Nodeは `ChildProcess` クラスを通じて 3 方向の `popen(3)` 機能を提供します。

<!--

It is possible to stream data through the child's `stdin`, `stdout`, and
`stderr` in a fully non-blocking way.

-->
それは完全にノンブロッキングな方法で子プロセスの `stdin`、`stdout`、
そして `stderr` を通じたデータストリームを実現します。

<!--

To create a child process use `require('child_process').spawn()`.

-->
子プロセスの生成は `require('child_process').spawn()` を使います。

<!--

Child processes always have three streams associated with them. `child.stdin`,
`child.stdout`, and `child.stderr`.

-->
子プロセスは常に 3 本のストリームと関連づけられています。
`child.stdin`、`child.stdout`、そして `child.stderr` です。

<!--

`ChildProcess` is an `EventEmitter`.

-->
`ChildProcess` は `EventEmitter` です。

### Event:  'exit'

`function (code, signal) {}`

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

See `waitpid(2)`.

-->
`waitpid(2)` を参照してください。

### child.stdin

<!--

A `Writable Stream` that represents the child process's `stdin`.
Closing this stream via `end()` often causes the child process to terminate.

-->
子プロセスの `stdin` を表現する `Writable Stream` です。
多くの場合、`end()` を通じてこのストリームを閉じると子プロセスが終了する原因となります。

### child.stdout

<!--

A `Readable Stream` that represents the child process's `stdout`.

-->
子プロセスの `stdout` を表現する `Readable Stream` です。

### child.stderr

<!--

A `Readable Stream` that represents the child process's `stderr`.

-->
子プロセスの `stderr` を表現する `Readable Stream` です。

### child.pid

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


### child_process.spawn(command, args=[], [options])

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
      env: process.env,
      setsid: false
    }

<!--

`cwd` allows you to specify the working directory from which the process is spawned.
Use `env` to specify environment variables that will be visible to the new process.

`setsid`, if set true, will cause the subprocess to be run in a new session.

-->
`cwd` で起動されたプロセスのワーキングディレクトリを指定することができます。
`env` は新しいプロセスに見える環境変数を指定するために使います。

`setsid` に true が設定されると、サブプロセスは新しいセッションで実行されます。

<!--

Example of running `ls -lh /usr`, capturing `stdout`, `stderr`, and the exit code:

-->
`ls -lh /usr` を実行して `stdout`、`stderr`、および終了コードを取得する例:

    var util  = require('util'),
        spawn = require('child_process').spawn,
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

    var util  = require('util'),
        spawn = require('child_process').spawn,
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
      console.log(data);
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

There is a deprecated option called `customFds` which allows one to specify
specific file descriptors for the stdio of the child process. This API was
not portable to all platforms and therefore removed.
With `customFds` it was possible to hook up the new process' [stdin, stdout,
stderr] to existing streams; `-1` meant that a new stream should be created.
Use at your own risk.

There are several internal options. In particular `stdinStream`,
`stdoutStream`, `stderrStream`. They are for INTERNAL USE ONLY. As with all
undocumented APIs in Node, they should not be used.

See also: `child_process.exec()`

-->
特定のファイル記述子を子プロセスの標準入出力に指定することを可能にする、
`customFds` と呼ばれる廃止されたオプションがありました。
この API は全てのプラットフォームに移植可能ではないために削除されました。
`customFds` は新しいプロセスの [stdin, stdout, stderr] を既存のストリームに接続することを可能にしました;
`-1` は新しいストリームが作られなければならないことを意味していました。
使用する場合は自己責任で。

よく似た内部的なオプションがあります。
具体的には `stdinStream`、`stdoutStream`、`stderrStream` です。
これらは __内部用途__です。
Node のドキュメント化されていない API と同様に、
これらを使用すべきではありません。

関連項目: `child_process.exec()`

### child_process.exec(command, [options], callback)

<!--

Runs a command in a shell and buffers the output.

-->
コマンドをシェルで実行し、その出力をバッファに格納します。

    var util = require('util'),
        exec = require('child_process').exec,
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

There is a second optional argument to specify several options. The default options are

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


### child_process.execFile(file, args, options, callback)

<!--

This is similar to `child_process.exec()` except it does not execute a
subshell but rather the specified file directly. This makes it slightly
leaner than `child_process.exec`. It has the same options.

-->
子シェルで実行する代わりに指定されたファイルを直接実行することを除いて
`child_process.exec()` と同様です。
これは `child_process.exec` より若干効率的で、同じオプションを持ちます。


### child_process.fork(modulePath, arguments, options)

<!--

This is a special case of the `spawn()` functionality for spawning Node
processes. In addition to having all the methods in a normal ChildProcess
instance, the returned object has a communication channel built-in. The
channel is written to with `child.send(message, [sendHandle])` and messages
are recieved by a `'message'` event on the child.

-->
これは `spawn()` の特別版で、Node プロセスを起動します。
返されるオブジェクトは通常の ChildProcess の全てのメソッドに加えて、
組み込みの通信チャネルを持ちます。
チャネルは `child.send(message, [sendHandle])` によって書き込まれ、
メッセージを受信すると `child` 上で `'message'` イベントが生成されます。

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

And then the child script, `'sub.js'` would might look like this:

-->
そして子スクリプトの `'sub.js'` は次のようになります:

    process.on('message', function(m) {
      console.log('CHILD got message:', m);
    });

    process.send({ foo: 'bar' });

<!--

In the child the `process` object will have a `send()` method, and `process`
will emit objects each time it receives a message on its channel.

-->
子供の `process` オブジェクトは `send()` メソッドを持ち、
`process` はチャネルでメッセージを受信するたびにイベントを生成します。

<!--

By default the spawned Node process will have the stdin, stdout, stderr
associated with the parent's.

-->
デフォルトでは、起動された Node プロセスは親プロセスに関連づけられた標準入力、
標準出力、標準エラー出力を持ちます。

<!--

These child Nodes are still whole new instances of V8. Assume at least 30ms
startup and 10mb memory for each new Node. That is, you cannot create many
thousands of them.

-->
これらの子 Node は、やはり V8 の新しいインスタンスです。
新しい Node ごとに少なくとも 30 ミリ秒の起動時間と 
10MB のメモリを前提としてください。
つまり、数千の子プロセスを作ることは出来ません。

<!--

The `sendHandle` option to `child.send()` is for sending a handle object to
another process. Child will receive the handle as as second argument to the
`message` event. Here is an example of sending a handle:

-->
`child.send()` の `sendHandle` オプションはハンドルオブジェクトを別プロセスに
送ります。
子プロセスはそのハンドルを `message` イベントの第2引数として受け取ります。
これはハンドルを送信するサンプルです。


    var server = require('net').createServer();
    var child = require('child_process').fork(__dirname + '/child.js');
    // Open up the server object and send the handle.
    server.listen(1337, function() {
      child.send({ server: true }, server._handle);
    });

<!--

Here is an example of receiving the server handle and sharing it between
processes:

-->
これはサーバのハンドルを受信してプロセス間で共有するサンプルです。


    process.on('message', function(m, serverHandle) {
      if (serverHandle) {
        var server = require('net').createServer();
        server.listen(serverHandle);
      }
    });


### child.kill(signal='SIGTERM')

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

See `kill(2)`
