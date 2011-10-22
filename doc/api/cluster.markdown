## Cluster

<!--
A single instance of Node runs in a single thread. To take advantage of
multi-core systems the user will sometimes want to launch a cluster of Node
processes to handle the load.
-->
一つの Node インスタンスは一つのスレッドで実行されます。
マルチコアシステムのメリットを生かすために、
ユーザは時々 Node プロセスのクラスを起動して負荷を分散したくなります。

<!--
By starting node with the `cluster` argument, Node will detect the number of
CPUs on the machine and start that many processes. For example suppose we
had a simple HTTP server in server.js:
-->
node を `cluster` オプションと共に開始すると、Node はマシン上の
CPU 数を検出して、多くのプロセスを開始します:

    require('http').createServer(function(req, res) {
      res.writeHead(200);
      res.end('hello world\n');
    }).listen(8000);

<!--
If we start it like this
-->
次のように開始すると

    % node cluster server.js 
    Detected 2 cpus
    Worker 2438 online
    Worker 2437 online

<!--
Node will automatically share port 8000 between the multiple instances.
-->
Node は複数のインスタンス間で 8000 番ポートを自動的に共有します。
