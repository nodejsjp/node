# Cluster

    Stability: 1 - Experimental

<!--
A single instance of Node runs in a single thread. To take advantage of
multi-core systems the user will sometimes want to launch a cluster of Node
processes to handle the load.
-->

一つの Node インスタンスは一つのスレッドで実行されます。
マルチコアシステムのメリットを生かすために、
ユーザは時々 Node プロセスのクラスを起動して負荷を分散したくなります。

<!--
The cluster module allows you to easily create a network of processes that
all share server ports.
-->

クラスタモジュールは、サーバポートを共有するプロセスのネットワークを
簡単に構築することを可能にします。

    var cluster = require('cluster');
    var http = require('http');
    var numCPUs = require('os').cpus().length;

    if (cluster.isMaster) {
      // Fork workers.
      for (var i = 0; i < numCPUs; i++) {
        cluster.fork();
      }

      cluster.on('exit', function(worker, code, signal) {
        console.log('worker ' + worker.process.pid + ' died');
      });
    } else {
      // Workers can share any TCP connection
      // In this case its a HTTP server
      http.createServer(function(req, res) {
        res.writeHead(200);
        res.end("hello world\n");
      }).listen(8000);
    }

<!--
Running node will now share port 8000 between the workers:
-->

node は 8000 番ポートをワーカ間で共有します。

    % node server.js
    Worker 2438 online
    Worker 2437 online


<!--
This feature was introduced recently, and may change in future versions.
Please try it out and provide feedback.
-->

この機能は最近導入されたばかりであり、
将来のバージョンで変更される可能性があります。
これを試して、フィードバックを行ってください。

<!--
Also note that, on Windows, it is not yet possible to set up a named pipe
server in a worker.
-->

Windows では、ワーカが名前付きパイプによるサーバをセットアップすることは
まだできないことにも注意してください。

## How It Works

<!--type=misc-->

<!--
The worker processes are spawned using the `child_process.fork` method,
so that they can communicate with the parent via IPC and pass server
handles back and forth.
-->

ワーカプロセスは `child_process.fork` メソッドを使って起動されるため、
親プロセスと IPC で通信したり、サーバハンドルをやり取りしたりすることが
できます。

<!--
When you call `server.listen(...)` in a worker, it serializes the
arguments and passes the request to the master process.  If the master
process already has a listening server matching the worker's
requirements, then it passes the handle to the worker.  If it does not
already have a listening server matching that requirement, then it will
create one, and pass the handle to the child.
-->

ワーカが `server.listen(...)` を呼び出すと、引数がシリアライズされて
マスタプロセスにリクエストが渡されます。
マスタプロセスは、ワーカのリクエストにマッチするリスニングサーバが既に
存在すればそのハンドルをワーカに渡します。
リクエストにマッチするリスニングサーバが存在しなければ、それが作成されて
子プロセスに渡されます。

<!--
This causes potentially surprising behavior in three edge cases:
-->

これは三つの極端なケースで驚くような振る舞いを引き起こすかもしれません。

<!--
1. `server.listen({fd: 7})` Because the message is passed to the master,
   file descriptor 7 **in the parent** will be listened on, and the
   handle passed to the worker, rather than listening to the worker's
   idea of what the number 7 file descriptor references.
2. `server.listen(handle)` Listening on handles explicitly will cause
   the worker to use the supplied handle, rather than talk to the master
   process.  If the worker already has the handle, then it's presumed
   that you know what you are doing.
3. `server.listen(0)` Normally, this will cause servers to listen on a
   random port.  However, in a cluster, each worker will receive the
   same "random" port each time they do `listen(0)`.  In essence, the
   port is random the first time, but predictable thereafter.  If you
   want to listen on a unique port, generate a port number based on the
   cluster worker ID.
-->

1. `server.listen({fd: 7})` メッセージはマスタに渡されてるため、
   ワーカのファイル記述子 7 が参照するものではなく、
   **親プロセスの** ファイル記述子 7 がリスニングされてそのハンドルがワーカに
   渡されます。
2. `server.listen(handle)` 明示的なハンドルをリスニングするとマスタプロセスは
   関与することなく、ワーカは与えられたハンドルを使うことになります。
   ワーカがすでにハンドルを持っているなら、何をしようとしているか
   あなたは分かっているでしょう。
3. `'server.listen(0)` 通常、これはサーバがランダムなポートをリッスンすることを
   意味します。しかしながらクラスタでは、各ワーカは `listen(0)` によって
   同じ "ランダムな" ポートを受信します。
   すなわち、初回はポートはランダムになりますが、その後はそうではありません。
   もしユニークなポートをリッスンしたければ、クラスタのワーカ ID に基づいて
   ポート番号を生成してください。

<!--
When multiple processes are all `accept()`ing on the same underlying
resource, the operating system load-balances across them very
efficiently.  There is no routing logic in Node.js, or in your program,
and no shared state between the workers.  Therefore, it is important to
design your program such that it does not rely too heavily on in-memory
data objects for things like sessions and login.
-->

複数のプロセスが同じリソースを `accept()` する時、オペレーティングシステムは
とても効率的にそれら全体をロードバランスします。
Node.js にもあなたのプログラムにも、ルーティングのためのロジックや
ワーカ間で共有される状態はありません。
したがって、あなたのプログラムがセッションやログインのためにメモリ内の
データオブジェクトに過度に頼らないように設計することが重要です。

<!--
Because workers are all separate processes, they can be killed or
re-spawned depending on your program's needs, without affecting other
workers.  As long as there are some workers still alive, the server will
continue to accept connections.  Node does not automatically manage the
number of workers for you, however.  It is your responsibility to manage
the worker pool for your application's needs.
-->

全てのワーカは独立したプロセスなので、他のワーカに影響を与えることなく
プログラムのニーズに応じてそれらを殺したり再起動したりすることができます。
いくつかのワーカが生きている限り、サーバは接続を受け付け続けます。
しかしながら、Node はワーカの数を自動的に管理することはありません。
アプリケーションのニーズに応じてワーカのプールを管理することは、
あなたの責務です。

## cluster.settings

<!--
* {Object}
  * `exec` {String} file path to worker file.  (Default=`__filename`)
  * `args` {Array} string arguments passed to worker.
    (Default=`process.argv.slice(2)`)
  * `silent` {Boolean} whether or not to send output to parent's stdio.
    (Default=`false`)
-->

* {Object}
  * `exec` {String} ワーカで実行するファイルへのパス.
    (デフォルトは `__filename`)
  * `args` {Array} ワーカに渡される引数となる文字列。
    (デフォルトは `process.argv.slice(2)`)
  * `silent` {Boolean} 出力を親プロセスに送るかどうか。
    (デフォルトは `false`)

<!--
All settings set by the `.setupMaster` is stored in this settings object.
This object is not supposed to be changed or set manually, by you.
-->

`cluster.setupMaster()` によってセットされた全ての情報は設定オブジェクトに
保存されます。
このオブジェクトはあなたによって変更されることを想定していません。

## cluster.isMaster

* {Boolean}

<!--
True if the process is a master. This is determined
by the `process.env.NODE_UNIQUE_ID`. If `process.env.NODE_UNIQUE_ID` is
undefined, then `isMaster` is `true`.
-->

現在のプロセスがマスタの場合は `true` です。
これは `process.env.NODE_UNIQUE_ID` から決定されます。
`process.env.NODE_UNIQUE_ID` が未定義だと `isMaster` は `true` になります。

## cluster.isWorker

* {Boolean}

<!--
This boolean flag is true if the process is a worker forked from a master.
If the `process.env.NODE_UNIQUE_ID` is set to a value, then
`isWorker` is `true`.
-->

現在のプロセスがマスタからフォークされたワーカの場合、
この論理値型のフラグは `true` です。
`process.env.NODE_UNIQUE_ID` に値が設定されていると、
`isWorker` は `true` になります。

## Event: 'fork'

* `worker` {Worker object}

<!--
When a new worker is forked the cluster module will emit a 'fork' event.
This can be used to log worker activity, and create you own timeout.
-->

新しいワーカがフォークされると、クラスタモジュールは `'fork'` イベントを
生成します。
これはワーカの活動をロギングしたり、タイムアウトのために使うことができます。

    var timeouts = [];
    function errorMsg() {
      console.error("Something must be wrong with the connection ...");
    }

    cluster.on('fork', function(worker) {
      timeouts[worker.id] = setTimeout(errorMsg, 2000);
    });
    cluster.on('listening', function(worker, address) {
      clearTimeout(timeouts[worker.id]);
    });
    cluster.on('exit', function(worker, code, signal) {
      clearTimeout(timeouts[worker.id]);
      errorMsg();
    });

## Event: 'online'

* `worker` {Worker object}

<!--
After forking a new worker, the worker should respond with a online message.
When the master receives a online message it will emit such event.
The difference between 'fork' and 'online' is that fork is emitted when the
master tries to fork a worker, and 'online' is emitted when the worker is
being executed.
-->

新しいワーカをフォークした後、ワーカはオンラインメッセージを応答します。
マスタがオンラインメッセージを受信すると、このイベントが生成されます。
`'fork'` と `'online'` の違いは、`'fork'` はマスタがワーカのフォークを
試みた時点で生成されるのに対し、`'online'` はワーカの実行が開始されてから
生成される点です。

    cluster.on('online', function(worker) {
      console.log("Yay, the worker responded after it was forked");
    });

## Event: 'listening'

* `worker` {Worker object}
* `address` {Object}

<!--
When calling `listen()` from a worker, a 'listening' event is automatically assigned
to the server instance. When the server is listening a message is send to the master
where the 'listening' event is emitted.
-->

ワーカが `net.Server.listen()` を呼び出すと、`'listening'` イベントは自動的に
`net.Server` インスタンスに割り当てられます。
`net.Server` が `'listening'` メッセージをマスタに送信すると、
`'listening'` イベントが生成されます。

<!--
The event handler is executed with two arguments, the `worker` contains the worker
object and the `address` object contains the following connection properties:
`address`, `port` and `addressType`. This is very useful if the worker is listening
on more than one address.
-->

イベントハンドラは二つの引数を伴って実行されます。
`worker` はワーカオブジェクトを、`address` オブジェクトは
以下の接続プロパティを含みます:
`address`、`prot`、そして `addressType` です。
これはワーカが複数のアドレスをリッスンしている場合にとても便利です。

    cluster.on('listening', function(worker, address) {
      console.log("A worker is now connected to " + address.address + ":" + address.port);
    });

## Event: 'disconnect'

* `worker` {Worker object}

<!--
When a workers IPC channel has disconnected this event is emitted. This will happen
when the worker dies, usually after calling `.destroy()`.
-->

ワーカの IPC チャネルが切断された場合に生成されます。
それはワーカが終了した場合や、`.destroy()` を呼び出した後に発生します。

<!--
When calling `.disconnect()`, there may be a delay between the
`disconnect` and `exit` events.  This event can be used to detect if
the process is stuck in a cleanup or if there are long-living
connections.
-->

`.disconnect()` を呼び出した後、`'disconnect'` と `'exit'` の間には
遅延があるかもしれません。このイベントはプロセスがクリーンナップで
行き詰まったり、長時間生きている接続がないかを検出することに
使用できます。

    cluster.on('disconnect', function(worker) {
      console.log('The worker #' + worker.id + ' has disconnected');
    });

## Event: 'exit'

<!--
* `worker` {Worker object}
* `code` {Number} the exit code, if it exited normally.
* `signal` {String} the name of the signal (eg. `'SIGHUP'`) that caused
  the process to be killed.
-->

* `worker` {Worker object}
* `code` {Number} 正常に終了した場合は終了コード。
* `signal` {String} プロセスが殺される原因となったシグナルの名前
  (例: `'SIGHUP'`)。

<!--
When any of the workers die the cluster module will emit the 'exit' event.
This can be used to restart the worker by calling `fork()` again.
-->

どのワーカが死んだ場合でも、クラスタモジュールは `'exit'` イベントを
生成します。
これは `fork()` を呼び出してワーカを再開する場合に使用することができます。

    cluster.on('exit', function(worker, code, signal) {
      var exitCode = worker.process.exitCode;
      console.log('worker ' + worker.process.pid + ' died ('+exitCode+'). restarting...');
      cluster.fork();
    });

## Event: 'setup'

* `worker` {Worker object}

<!--
When the `.setupMaster()` function has been executed this event emits.
If `.setupMaster()` was not executed before `fork()` this function will
call `.setupMaster()` with no arguments.
-->

`setupMaster()` が実行された時、このイベントが生成されます。
`fork()` の前に`setupMaster()` が呼ばれなかった場合、
この関数は引数無しで `setupMaster()` を呼び出します。

## cluster.setupMaster([settings])

<!--
* `settings` {Object}
  * `exec` {String} file path to worker file.  (Default=`__filename`)
  * `args` {Array} string arguments passed to worker.
    (Default=`process.argv.slice(2)`)
  * `silent` {Boolean} whether or not to send output to parent's stdio.
    (Default=`false`)
-->

* `settings` {Object}
  * `exec` {String} ワーカで実行するファイルへのパス.
    (デフォルトは `__filename`)
  * `args` {Array} ワーカに渡される引数となる文字列。
    (デフォルトは `process.argv.slice(2)`)
  * `silent` {Boolean} 出力を親プロセスに送るかどうか。
    (デフォルトは `false`)

<!--
`setupMaster` is used to change the default 'fork' behavior. The new settings
are effective immediately and permanently, they cannot be changed later on.
-->

`setupMaster()` は 'fork' のデフォルト動作を変更するために使われます。
新しい設定は即時かつ永続的に有効で、後から変更することはできません。

<!--
Example:
-->

例:

    var cluster = require("cluster");
    cluster.setupMaster({
      exec : "worker.js",
      args : ["--use", "https"],
      silent : true
    });
    cluster.fork();

## cluster.fork([env])

<!--
* `env` {Object} Key/value pairs to add to child process environment.
* return {Worker object}
-->

* `env` {Object} 子プロセスの環境に加えられるキーと値のペア。
* return {Worker object}

<!--
Spawn a new worker process. This can only be called from the master process.
-->

新しいワーカプロセスを起動します。
これはマスタプロセスからのみ呼び出すことができます。

## cluster.disconnect([callback])

<!--
* `callback` {Function} called when all workers are disconnected and handlers are closed
-->

* `callback` {Function} 全てのワーカが切断し、ハンドルがクローズされると
  呼び出されます。

<!--
When calling this method, all workers will commit a graceful suicide. When they are
disconnected all internal handlers will be closed, allowing the master process to
die graceful if no other event is waiting.
-->

このメソッドを呼び出すと、全てのワーカは強制的でない終了に向かいます。
それらの内部的なハンドルが全てクローズされると、
他に待機しているイベントがなければ、
マスタプロセスを非強制的に終了することができます。

<!--
The method takes an optional callback argument which will be called when finished.
-->

このメソッドはオプションの引数としてコールバックを受け取ります。

## cluster.worker

* {Object}

<!--
A reference to the current worker object. Not available in the master process.
-->

現在のワーカオブジェクトへの参照です。
マスタプロセスでは利用できません。

    var cluster = require('cluster');

    if (cluster.isMaster) {
      console.log('I am master');
      cluster.fork();
      cluster.fork();
    } else if (cluster.isWorker) {
      console.log('I am worker #' + cluster.worker.id);
    }

## cluster.workers

* {Object}

<!--
A hash that stores the active worker objects, keyed by `id` field. Makes it
easy to loop through all the workers. It is only available in the master
process.
-->

`id` をキーとしてアクティブなワーカオブジェクトを保存しているハッシュです。
これは全てのワーカに対して繰り返しを行うことを容易にします。
マスタプロセスでのみ利用可能です。

    // Go through all workers
    function eachWorker(callback) {
      for (var id in cluster.workers) {
        callback(cluster.workers[id]);
      }
    }
    eachWorker(function(worker) {
      worker.send('big announcement to all workers');
    });

<!--
Should you wish to reference a worker over a communication channel, using
the worker's unique id is the easiest way to find the worker.
-->

通信チャネルを越えてワーカの参照を渡す場合は、
ワーカのユニークな ID を使ってワーカを探すのが簡単です。

    socket.on('data', function(id) {
      var worker = cluster.workers[id];
    });

## Class: Worker

<!--
A Worker object contains all public information and method about a worker.
In the master it can be obtained using `cluster.workers`. In a worker
it can be obtained using `cluster.worker`.
-->

ワーカに関する全ての公開された情報やメソッドを持つオブジェクトです。
マスタでは `cluster.wrokers` から取得することができます。
ワーカでは `cluster.worker` から取得することができます。

### worker.id

* {String}

<!--
Each new worker is given its own unique id, this id is stored in the
`id`.

While a worker is alive, this is the key that indexes it in
cluster.workers
-->

新しいワーカはいずれもユニークな ID を与えられます。
この ID は `id` に保存されます。

ワーカが生きている間、これは `cluseter.workers` のキーとなります。

### worker.process

* {ChildProcess object}

<!--
All workers are created using `child_process.fork()`, the returned object
from this function is stored in process.

See: [Child Process module](child_process.html)
-->

全てのワーカは `child_process.fork()` によって作成されます。
その戻り値が `process` に設定されます。

参照: [Child Process module](child_process.html)

### worker.suicide

* {Boolean}

<!--
This property is a boolean. It is set when a worker dies after calling `.destroy()`
or immediately after calling the `.disconnect()` method. Until then it is `undefined`.
-->

このプロパティは論理値型です。
これはワーカが `.destroy()` を呼び出して終了するか、
`.disconnect()` メソッドを呼び出した後に設定されます。
それまでは `undefined` です。

### worker.send(message, [sendHandle])

* `message` {Object}
* `sendHandle` {Handle object}

<!--
This function is equal to the send methods provided by
`child_process.fork()`.  In the master you should use this function to
send a message to a specific worker.  However in a worker you can also use
`process.send(message)`, since this is the same function.

This example will echo back all messages from the master:
-->

この関数は `child_process.fork()` が返すオブジェクトの `send()`
メソッドと同じです．
マスタは特定のワーカにメッセージを送信するためにこの関数を
使用することができます。
しかし、ワーカでは `process.send(message)` を使うこともできます。

この例はマスタからのメッセージをエコーバックします。

    if (cluster.isMaster) {
      var worker = cluster.fork();
      worker.send('hi there');

    } else if (cluster.isWorker) {
      process.on('message', function(msg) {
        process.send(msg);
      });
    }

### worker.destroy()

<!--
This function will kill the worker, and inform the master to not spawn a
new worker.  The boolean `suicide` lets you distinguish between voluntary
and accidental exit.
-->

この関数はワーカを終了し、マスタに新しいワーカを起動しないように伝えます。
boolean の `suicide` により、自発的かアクシデントによる終了かを識別できます。

    cluster.on('exit', function(worker, code, signal) {
      if (worker.suicide === true) {
        console.log('Oh, it was just suicide\' – no need to worry').
      }
    });

    // destroy worker
    worker.destroy();


### worker.disconnect()

<!--
When calling this function the worker will no longer accept new connections, but
they will be handled by any other listening worker. Existing connection will be
allowed to exit as usual. When no more connections exist, the IPC channel to the worker
will close allowing it to die graceful. When the IPC channel is closed the `disconnect`
event will emit, this is then followed by the `exit` event, there is emitted when
the worker finally die.
-->

この関数を呼び出すと、そのワーカはそれ以上新しい接続
(それらは他のワーカによって扱われます) を受け付けなくなります。
既存の接続は通常通りに
コネクションが無くなると、ワーカを正常に終了するために IPC チャネルは
閉じられます。
IPC チャネルが閉じられると `'disconnect'` イベントが生成され、
その後ワーカが終了すると `'exit'` イベントが生成されます。

<!--
Because there might be long living connections, it is useful to implement a timeout.
This example ask the worker to disconnect and after 2 seconds it will destroy the
server. An alternative would be to execute `worker.destroy()` after 2 seconds, but
that would normally not allow the worker to do any cleanup if needed.
-->

長時間にわたる接続があるかもしれないため、これはタイムアウトを実装するために
有益です。
この例はワーカが IPC チャネルを閉じた後、2 秒でサーバから終了されます。
他の方法として `worker.destroy()` を呼び出してから
2 秒後とすることもできますが、その場合はワーカが必要なクリーンナップを
行えないかもしれません。

    if (cluster.isMaster) {
      var worker = cluster.fork();
      var timeout;

      worker.on('listening', function(address) {
        worker.disconnect();
        timeout = setTimeout(function() {
          worker.send('force kill');
        }, 2000);
      });

      worker.on('disconnect', function() {
        clearTimeout(timeout);
      });

    } else if (cluster.isWorker) {
      var net = require('net');
      var server = net.createServer(function(socket) {
        // connection never end
      });

      server.listen(8000);

      server.on('close', function() {
        // cleanup
      });

      process.on('message', function(msg) {
        if (msg === 'force kill') {
          server.destroy();
        }
      });
    }

### Event: 'message'

* `message` {Object}

<!--
This event is the same as the one provided by `child_process.fork()`.
In the master you should use this event, however in a worker you can also use
`process.on('message')`

As an example, here is a cluster that keeps count of the number of requests
in the master process using the message system:
-->

このイベントは `child_process.fork()` が提供するものと同じです。
マスタではこのイベントを使うべきですが、ワーカでは `process.on('message')`
を使うこともできます。

メッセージシステムを使用してクラスタ全体のリクエスト数を
マスタプロセスで保持する例です:

    var cluster = require('cluster');
    var http = require('http');

    if (cluster.isMaster) {

      // Keep track of http requests
      var numReqs = 0;
      setInterval(function() {
        console.log("numReqs =", numReqs);
      }, 1000);

      // Count requestes
      function messageHandler(msg) {
        if (msg.cmd && msg.cmd == 'notifyRequest') {
          numReqs += 1;
        }
      }

      // Start workers and listen for messages containing notifyRequest
      var numCPUs = require('os').cpus().length;
      for (var i = 0; i < numCPUs; i++) {
        cluster.fork();
      }

      Object.keys(cluster.workers).forEach(function(id) {
        cluster.workers[id].on('message', messageHandler);
      });

    } else {

      // Worker processes have a http server.
      http.Server(function(req, res) {
        res.writeHead(200);
        res.end("hello world\n");

        // notify master about the request
        process.send({ cmd: 'notifyRequest' });
      }).listen(8000);
    }

### Event: 'online'

<!--
Same as the `cluster.on('online')` event, but emits only when the state change
on the specified worker.
-->

`cluster.on('online')` と同様ですが，特定のワーカの状態が変化した場合のみ
イベントを生成します。

    cluster.fork().on('online', function() {
      // Worker is online
    };

### Event: 'listening'

* `address` {Object}

<!--
Same as the `cluster.on('listening')` event, but emits only when the state change
on the specified worker.
-->

`cluster.on('listening')` と同様ですが、特定のワーカの状態が変化した場合のみ
イベントを生成します。

    cluster.fork().on('listening', function(address) {
      // Worker is listening
    };

### Event: 'disconnect'

<!--
Same as the `cluster.on('disconnect')` event, but emits only when the state change
on the specified worker.
-->

`cluster.on('disconnect')` と同じですが、指定されたワーカの状態が
変更された場合のみ生成されます。

    cluster.fork().on('disconnect', function() {
      // Worker has disconnected
    };

### Event: 'exit'

<!--
* `code` {Number} the exit code, if it exited normally.
* `signal` {String} the name of the signal (eg. `'SIGHUP'`) that caused
  the process to be killed.
-->

* `code` {Number} 正常に終了した場合は終了コード。
* `signal` {String} プロセスが殺される原因となったシグナルの名前
  (例: `'SIGHUP'`)。

<!--
Emitted by the individual worker instance, when the underlying child process
is terminated.  See [child_process event: 'exit'](child_process.html#child_process_event_exit).
-->

子プロセスが終了すると、ワーカのインスタンスによって生成されます。
[child_process event: 'exit'](child_process.html#child_process_event_exit)
を参照してください。

    var worker = cluster.fork();
    worker.on('exit', function(code, signal) {
      if( signal ) {
        console.log("worker was killed by signal: "+signal);
      } else if( code !== 0 ) {
        console.log("worker exited with error code: "+code);
      } else {
        console.log("worker success!");
      }
    };
