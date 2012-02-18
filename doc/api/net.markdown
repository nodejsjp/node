## net

<!--

The `net` module provides you with an asynchronous network wrapper. It contains
methods for creating both servers and clients (called streams). You can include
this module with `require('net');`

-->
`net` モジュールは非同期なネットワークのラッパーを提供します。
それはサーバとクライアントの両方 (ストリームと呼ばれます) を作成するための方法を含みます。
このモジュールは`require("net");`によって取り込むことができます。

### net.createServer([options], [connectionListener])

<!--

Creates a new TCP server. The `connectionListener` argument is
automatically set as a listener for the ['connection'](#event_connection_)
event.

`options` is an object with the following defaults:

-->
新しい TCP サーバを作成します。
`connectionListener` 引数は ['connection'](#event_connection_)
イベントに対するリスナーとして自動的に加えられます。

`options` は以下のデフォルト値を持つオブジェクトです:

    { allowHalfOpen: false
    }

<!--

If `allowHalfOpen` is `true`, then the socket won't automatically send FIN
packet when the other end of the socket sends a FIN packet. The socket becomes
non-readable, but still writable. You should call the `end()` method explicitly.
See ['end'](#event_end_) event for more information.

-->
`allowHalfOpen` が `true` だと、反対側のソケットが FIN パケットを送信してきても自動的に FIN を送信しなくなります。
ソケットは読み込み可能ではなくなりますが、書き込み可能のままです。
明示的に `end()` を呼び出す必要があります。
['end'](#event_end_) イベントにより多くの情報があります。

<!--

Here is an example of a echo server which listens for connections
on port 8124:

-->
8124 番のポートへの接続を待ち受けるエコーサーバの例:

    var net = require('net');
    var server = net.createServer(function(c) { //'connection' listener
      console.log('server connected');
      c.on('end', function() {
        console.log('server disconnected');
      });
      c.write('hello\r\n');
      c.pipe(c);
    });
    server.listen(8124, function() { //'listening' listener
      console.log('server bound');
    });

<!--

Test this by using `telnet`:

-->
`telnet` を使ってテストします:

    telnet localhost 8124

<!--

To listen on the socket `/tmp/echo.sock` the third line from the last would
just be changed to

-->
`'/tmp/echo.sock'` へのソケットを待ち受けるには、最後から三行目をこのように変更します。

    server.listen('/tmp/echo.sock', function() { //'listening' listener

<!--

Use `nc` to connect to a UNIX domain socket server:

-->
`nc` を使って UNIX ドメインソケットサーバへ接続します:

    nc -U /tmp/echo.sock

### net.connect(arguments...)
### net.createConnection(arguments...)

<!--

Construct a new socket object and opens a socket to the given location. When
the socket is established the ['connect'](#event_connect_) event will be
emitted.

-->
新しいソケットオブジェクトを構築し、与えられたロケーションへのソケットをオープンします。
ソケットが確立されると、['connect'](#event_connect_) イベントが生成されます。

<!--

The arguments for these methods change the type of connection:

-->
このメソッドの引数はコネクションの種類によって変わります。

* `net.connect(port, [host], [connectListener])`
* `net.createConnection(port, [host], [connectListener])`

<!--

  Creates a TCP connection to `port` on `host`. If `host` is omitted,
  `'localhost'` will be assumed.

-->
  `host` 上の `port` に対する TCP コネクションを作成します。
  `host` が省略されると `localhost` が仮定されます。

* `net.connect(path, [connectListener])`
* `net.createConnection(path, [connectListener])`

<!--

  Creates unix socket connection to `path`.

-->
  `path` に対する UNIX ドメインソケットを作成します。

<!--

The `connectListener` parameter will be added as an listener for the
['connect'](#event_connect_) event.

-->
`connectListener` 引数は ['connect'](#event_connect_)
イベントのリスナとして追加されます。

<!--

Here is an example of a client of echo server as described previously:

-->
前述のエコーサーバに接続するクライアントの例:

    var net = require('net');
    var client = net.connect(8124, function() { //'connect' listener
      console.log('client connected');
      client.write('world!\r\n');
    });
    client.on('data', function(data) {
      console.log(data.toString());
      client.end();
    });
    client.on('end', function() {
      console.log('client disconnected');
    });

<!--

To connect on the socket `/tmp/echo.sock` the second line would just be
changed to

-->
`'/tmp/echo.sock'` へのソケットに接続するには、2 行目をこのように変更します。

    var client = net.connect('/tmp/echo.sock', function() { //'connect' listener

---

### net.Server

<!--

This class is used to create a TCP or UNIX server.
A server is a `net.Socket` that can listen for new incoming connections.

-->
このクラスは TCP または UNIX ドメインのサーバを作成するために使われます。
サーバは `net.Scoket` であり、新たに到着する接続を待ち受けることができます。

#### server.listen(port, [host], [listeningListener])

<!--

Begin accepting connections on the specified `port` and `host`.  If the
`host` is omitted, the server will accept connections directed to any
IPv4 address (`INADDR_ANY`). A port value of zero will assign a random port.

-->
指定された `port` と `host` でコネクションの受け入れを開始します。
`host` が省略されると、サーバはどんな IPv4 アドレスへの接続も受け入れます
(`INADDR_ANY`)。
ポート番号に 0 を指定すると、ランダムなポートが割り当てられます。

<!--

This function is asynchronous.  When the server has been bound,
['listening'](#event_listening_) event will be emitted.
the last parameter `listeningListener` will be added as an listener for the
['listening'](#event_listening_) event.

-->
この関数は非同期です。
サーバがバインドされると、['listening'](#event_listening_)
イベントが生成されます。
最後の引数 `listeningListener` は ['listening'](#event_listening_)
のリスナとして加えられます。

<!--

One issue some users run into is getting `EADDRINUSE` errors. This means that
another server is already running on the requested port. One way of handling this
would be to wait a second and then try again. This can be done with

-->
一部のユーザが陥る問題の一つは、`EADDRINUSE` エラーです。
これは、他のサーバが要求されたポートを使っていることを意味します。
これに対照する方法の一つは、1秒待機してからリトライすることです。
これは次のようになります

    server.on('error', function (e) {
      if (e.code == 'EADDRINUSE') {
        console.log('Address in use, retrying...');
        setTimeout(function () {
          server.close();
          server.listen(PORT, HOST);
        }, 1000);
      }
    });

<!--

(Note: All sockets in Node set `SO_REUSEADDR` already)

-->
注意: Node の全てのソケットは `SO_REUSEADDR` が設定されます)


#### server.listen(path, [listeningListener])

<!--

Start a UNIX socket server listening for connections on the given `path`.

-->
与えられた `path` へのコネクションを待ち受けるする UNIX ドメインソケットのサーバを開始します。

<!--

This function is asynchronous.  When the server has been bound,
['listening'](#event_listening_) event will be emitted.
the last parameter `listeningListener` will be added as an listener for the
['listening'](#event_listening_) event.

-->
この関数は非同期です。
サーバがバインドされると、`'listening'` イベントが生成されます。
最後の引数 `listeningListener` は ['listening'](#event_listening_)
のリスナとして加えられます。


#### server.pause(msecs)

<!--

Stop accepting connections for the given number of milliseconds (default is
one second).  This could be useful for throttling new connections against
DoS attacks or other oversubscription.

-->
接続の待ち受けをミリ秒で与えられた時間だけ中断します (デフォルトは 1 秒です)。
これは、新しい接続を抑えることで DoS 攻撃やその他の加入超過に対抗するために
役立ちます。

#### server.close()

<!--

Stops the server from accepting new connections. This function is
asynchronous, the server is finally closed when the server emits a `'close'`
event.

-->
サーバが新しいコネクションを受け付けるのを終了します。
この関数は非同期で、サーバは最終的に `'close'` イベントを生成した時にクローズされます。


#### server.address()

<!--

Returns the bound address and port of the server as reported by the operating system.
Useful to find which port was assigned when giving getting an OS-assigned address.
Returns an object with two properties, e.g. `{"address":"127.0.0.1", "port":2121}`

-->
オペレーティングシステムから報告された、サーバにバインドされたアドレスとポートを返します。
OSによって割り当てられたアドレスが渡された時に、どのポートに割り当てられたものかを調べるのに便利です。
返されるオブジェクトは二つのプロパティを持ちます。
例えば `{"address":"127.0.0.1", "port":2121}`

<!--

Example:

-->
例:

    var server = net.createServer(function (socket) {
      socket.end("goodbye\n");
    });

    // grab a random port.
    server.listen(function() {
      address = server.address();
      console.log("opened server on %j", address);
    });

<!--

Don't call `server.address()` until the `'listening'` event has been emitted.

-->
`'listening'` イベントが生成される前に `server.address()` を呼び出してはいけません。

#### server.maxConnections

<!--

Set this property to reject connections when the server's connection count gets
high.

-->
サーバの接続数が大きくなった時に接続を拒否するためにこのプロパティを設定します。

#### server.connections

<!--

The number of concurrent connections on the server.

-->
このサーバ上の並行コネクションの数です。

<!--

`net.Server` is an `EventEmitter` with the following events:

-->
`net.Server` は以下のイベントを持つ `EventEmitter` です:

#### Event: 'listening'

`function () {}`

<!--

Emitted when the server has been bound after calling `server.listen`.

-->
`server.listen()` が呼ばれた後、サーバがバインドされると生成されます。

#### Event: 'connection'

`function (socket) {}`

<!--

Emitted when a new connection is made. `socket` is an instance of
`net.Socket`.

-->
新しいコネクションが作成されると生成されます。
`socket` は `net.Socket` のインスタンスです。

#### Event: 'close'

`function () {}`

<!--

Emitted when the server closes.

-->
サーバがクローズした時に生成されます。

#### Event: 'error'

`function (exception) {}`

<!--

Emitted when an error occurs.  The `'close'` event will be called directly
following this event.  See example in discussion of `server.listen`.

-->
エラーが発生すると生成されます。
このイベントに続いて `'close'` イベントが直接生成される場合があります。
`server.listen()` の例を参照してください。

---

### net.Socket

<!--

This object is an abstraction of a TCP or UNIX socket.  `net.Socket`
instances implement a duplex Stream interface.  They can be created by the
user and used as a client (with `connect()`) or they can be created by Node
and passed to the user through the `'connection'` event of a server.

-->
このオブジェクトは TCP または UNIX ドメインのソケットを抽象化したものです。
`net.Socket` のインスタンスは双方向のストリームインタフェースを実装します。
それらはユーザによって (`connect()` によって) 作成されてクライアントとして使われるか、
Node によって作成されてサーバの `'connection'` イベントを通じてユーザに渡されます。

#### new net.Socket([options])

<!--

Construct a new socket object.

-->
新しいソケットオブジェクトを構築します。

<!--

`options` is an object with the following defaults:

-->
`options` は以下のデフォルト値を持つオブジェクトです。

    { fd: null
      type: null
      allowHalfOpen: false
    }

<!--


`fd` allows you to specify the existing file descriptor of socket. `type`
specified underlying protocol. It can be `'tcp4'`, `'tcp6'`, or `'unix'`.
About `allowHalfOpen`, refer to `createServer()` and `'end'` event.

-->
`fd` に既存のソケットのファイル記述子を指定することができます。
`type` にはプロトコルを指定することができます。
指定できるのは `'tcp4'`、`'tcp6'` または `'unix'` のいずれかです。
`allowHalfOpen` については `createServer()` および `'end'` イベントを参照してください。

#### socket.connect(port, [host], [connectListener])
#### socket.connect(path, [connectListener])

<!--

Opens the connection for a given socket. If `port` and `host` are given,
then the socket will be opened as a TCP socket, if `host` is omitted,
`localhost` will be assumed. If a `path` is given, the socket will be
opened as a unix socket to that path.

-->
与えられたソケットでコネクションをオープンします。
`port` と `host` が与えられた場合、
ソケットは TCP ソケットとしてオープンされます。
`host` が省略された場合は `localhost` が仮定されます。
`path` が与えられた場合は、
ソケットはそのパスへの UNIX ドメインソケットとしてオープンされます。

<!--

Normally this method is not needed, as `net.createConnection` opens the
socket. Use this only if you are implementing a custom Socket or if a
Socket is closed and you want to reuse it to connect to another server.

-->
通常このメソッドは必要なく、`net.createConnection` でソケットをオープンします。
これを使うのは、カスタマイズされたソケットを実装している場合や、
ソケットがクローズされた後にコネクションを再利用して別のサーバに接続する場合だけです。

<!--

This function is asynchronous. When the ['connect'](#event_connect_) event is
emitted the socket is established. If there is a problem connecting, the
`'connect'` event will not be emitted, the `'error'` event will be emitted with
the exception.

-->
この関数は非同期です。ソケットが確立されると ['connect'](#event_connect_)
イベントが生成されます。
接続で問題があった場合は `'connect'` イベントは生成されず、
例外とともに `'error'` イベントが生成されます。

<!--

The `connectListener` parameter will be added as an listener for the
['connect'](#event_connect_) event.

-->
`connectListener` 引数は ['connect'](#event_connect_)
イベントのリスナに加えられます。


#### socket.bufferSize

<!--

`net.Socket` has the property that `socket.write()` always works. This is to
help users get up and running quickly. The computer cannot always keep up
with the amount of data that is written to a socket - the network connection
simply might be too slow. Node will internally queue up the data written to a
socket and send it out over the wire when it is possible. (Internally it is
polling on the socket's file descriptor for being writable).

The consequence of this internal buffering is that memory may grow. This
property shows the number of characters currently buffered to be written.
(Number of characters is approximately equal to the number of bytes to be
written, but the buffer may contain strings, and the strings are lazily
encoded, so the exact number of bytes is not known.)

Users who experience large or growing `bufferSize` should attempt to
"throttle" the data flows in their program with `pause()` and `resume()`.

-->
`net.Socket` には、`socket.write()` と常に協調するプロパティがあります。
これはユーザが実行速度を向上させる手助けになります。
コンピュータは、ソケットに書き込まれるデータ量についていくことはできません。
- ネットワーク接続は、単純に遅すぎます。
Node は、ソケットに書き込まれるデータを内部のキューに入れ、可能になった時にワイヤ上に送信します (内部ではソケットのファイル記述子が書き込み可能になるのをポーリングします)。

内部的なバッファリングの結果、メモリ消費が増大するかもしれません。
このプロパティは、現在書き込みのためにバッファリングされている文字数を示します。
(文字数は書き込まれるバイト数とほぼ同じですが、バッファが文字列を含んでいる場合、文字列は遅延的にエンコードされるため、正確なバイト数は分かっていません)

大きな、あるいは増大する `bufferSize` を体験したユーザは、そのプログラムで `pause()` および `resume()` を使ってデータフローを「抑えよう」としなければなりません。

#### socket.setEncoding([encoding])

<!--

Sets the encoding (either `'ascii'`, `'utf8'`, or `'base64'`) for data that is
received. Defaults to `null`.

-->
受信したデータのエンコーディングを設定します (`'ascii'`、`'utf8'`、
あるいは `'base64'` のいずれかです)。デフォルトは `null` です。

#### socket.setSecure()

<!--

This function has been removed in v0.3. It used to upgrade the connection to
SSL/TLS. See the [TLS section](tls.html#tLS_) for the new API.

-->
この関数は v0.3 で削除されました。
これはコネクションを SSL/TLS にアップグレードするために使われていました。
新しい API である [TLS の章](tls.html#tLS_) を参照してください。

#### socket.write(data, [encoding], [callback])

<!--

Sends data on the socket. The second parameter specifies the encoding in the
case of a string--it defaults to UTF8 encoding.

-->
ソケットにデータを送信します。
文字列の場合、第 2 引数はエンコーディングを指定します － デフォルトは UTF-8 です。

<!--

Returns `true` if the entire data was flushed successfully to the kernel
buffer. Returns `false` if all or part of the data was queued in user memory.
`'drain'` will be emitted when the buffer is again free.

-->
データ全体のカーネルバッファへのフラッシュが成功すると `true` を返します。
データ全体または一部がユーザメモリ内のキューに入れられた場合は `false` を返します。
再びバッファが空いた場合は `'drain'` イベントが生成されます。

<!--

The optional `callback` parameter will be executed when the data is finally
written out - this may not be immediately.

-->
オプションの `callback` 引数はデータが最終的に出力された時に実行されます
－ これはすぐには起きないでしょう。

#### socket.write(data, [encoding], [callback])

<!--

Write data with the optional encoding. The callback will be made when the
data is flushed to the kernel.

-->
データを出力します。オプションのエンコーディングを指定することができます。
コールバックはデータがカーネルにフラッシュされると呼び出されます。

#### socket.end([data], [encoding])

<!--

Half-closes the socket. i.e., it sends a FIN packet. It is possible the
server will still send some data.

-->
ソケットをハーフクローズします。例えば FIN パケットを送信します。
サーバはまだデータを送り続けてくることができます。

<!--

If `data` is specified, it is equivalent to calling
`socket.write(data, encoding)` followed by `socket.end()`.

-->
`data` が指定された場合は、
`socket.write(data, encoding)` に続けて `socket.end()` を呼び出すのと等価です。

#### socket.destroy()

<!--

Ensures that no more I/O activity happens on this socket. Only necessary in
case of errors (parse error or so).

-->
このソケット上でどんな I/O も起こらないことを保証します。
(パースエラーなどの) エラーの場合にだけ必要です。

#### socket.pause()

<!--

Pauses the reading of data. That is, `'data'` events will not be emitted.
Useful to throttle back an upload.

-->
データの読み込みを中断します。つまり、`'data'` イベントは生成されません。
アップロード速度を落とすために便利です。

#### socket.resume()

<!--

Resumes reading after a call to `pause()`.

-->
`pause()` を呼び出した後で読み込みを再開します。

#### socket.setTimeout(timeout, [callback])

<!--

Sets the socket to timeout after `timeout` milliseconds of inactivity on
the socket. By default `net.Socket` do not have a timeout.

-->

<!--

When an idle timeout is triggered the socket will receive a `'timeout'`
event but the connection will not be severed. The user must manually `end()`
or `destroy()` the socket.

-->
アイドルタイムアウトが引き起こされると、ソケットは `'timeout'` イベントを受信しますが、
コネクションは切断されません。
ユーザは手動で `end()` または `destroy()` を呼び出す必要があります。

<!--

If `timeout` is 0, then the existing idle timeout is disabled.

-->
`timeout` が 0 の場合、アイドルタイムアウトは無効にされます。

<!--

The optional `callback` parameter will be added as a one time listener for the
`'timeout'` event.

-->
オプションの `callback` 引数は、`timeouot` イベントの一回限りのリスナを追加します。

#### socket.setNoDelay([noDelay])

<!--

Disables the Nagle algorithm. By default TCP connections use the Nagle
algorithm, they buffer data before sending it off. Setting `true` for
`noDelay` will immediately fire off data each time `socket.write()` is called.
`noDelay` defaults to `true`.

-->
Nagle アルゴリズムを無効にします。
デフォルトでは TCP コネクションは Nagle アルゴリズムを使用し、データを送信する前にバッファリングします。
`noDelay` に `true` を設定すると、データは `socket.write()`
を呼び出す度に即座に送信されます。デフォルトは `true` です。

#### socket.setKeepAlive([enable], [initialDelay])

<!--

Enable/disable keep-alive functionality, and optionally set the initial
delay before the first keepalive probe is sent on an idle socket.
`enable` defaults to `false`.

Set `initialDelay` (in milliseconds) to set the delay between the last
data packet received and the first keepalive probe. Setting 0 for
initialDelay will leave the value unchanged from the default
(or previous) setting. Defaults to `0`.

-->
キープアライブ機能を有効/無効にします。
オプションで最初の keepalive probe がアイドルソケットに送信されるまでの初期遅延を設定します。
`enable` のデフォルトは `false` です。

`initialDelay` (ミリ秒) が設定されると、
最後にデータパケットを受信してから最初の keepalive probe までの遅延が設定されます。
初期遅延に 0 が設定されると、デフォルト設定から値を変更されないようにします。
デフォルトは `0` です。

#### socket.address()

<!--

Returns the bound address and port of the socket as reported by the operating
system. Returns an object with two properties, e.g.
`{"address":"192.168.57.1", "port":62053}`

-->
オペレーティングシステムから報告された、ソケットにバインドされたアドレスとポートを返します。
返されるオブジェクトは二つのプロパティを持ちます。
例えば `{"address":"192.168.57.1", "port":62053}`

#### socket.remoteAddress

<!--

The string representation of the remote IP address. For example,
`'74.125.127.100'` or `'2001:4860:a005::68'`.

-->
リモートの IP アドレスを表現する文字列です。
例えば、`'74.125.127.100'` あるいは `'2001:4860:a005::68'`。

<!--

<!--

This member is only present in server-side connections.

-->
このメンバはサーバサイドのコネクションにのみ与えられます。

#### socket.remotePort

<!--

The numeric representation of the remote port. For example,
`80` or `21`.

-->

リモートポートの数値表現です。
たとえば、`80` や `21`。

#### socket.bytesRead

<!--

The amount of received bytes.

-->
受信したバイトの合計です。

#### socket.bytesWritten

<!--

The amount of bytes sent.

-->
送信したバイトの合計です。

<!--

`net.Socket` instances are EventEmitters with the following events:

-->
`net.Socket` のインスタンスは以下のイベントを持つ EventEmitter です:

#### Event: 'connect'

`function () { }`

<!--

Emitted when a socket connection is successfully established.
See `connect()`.

-->
ソケットコネクションの確立が成功した場合に生成されます。
`connect()` を参照してください。

#### Event: 'data'

`function (data) { }`

<!--

Emitted when data is received.  The argument `data` will be a `Buffer` or
`String`.  Encoding of data is set by `socket.setEncoding()`.
(See the [Readable Stream](streams.html#readable_Stream) section for more
information.)

Note that the __data will be lost__ if there is no listener when a `Socket`
emits a `'data'` event.

-->
データを受信した場合に生成されます。
`data` 引数は `Buffer` または `String` です。
データのエンコーディングは `socket.setEncoding()` で設定されます。
(より詳しい情報は [Readable Stream](streams.html#readable_Stream) を参照してください)。

`Socket` が `'data'` イベントを生成した時にリスナが存在しなければ、
__データは失われる__ことに注意してください。

#### Event: 'end'

`function () { }`

<!--

Emitted when the other end of the socket sends a FIN packet.

-->
ソケットの相手側が FIN パケットを送信した場合に生成されます。

<!--

By default (`allowHalfOpen == false`) the socket will destroy its file
descriptor  once it has written out its pending write queue.  However, by
setting `allowHalfOpen == true` the socket will not automatically `end()`
its side allowing the user to write arbitrary amounts of data, with the
caveat that the user is required to `end()` their side now.

-->
デフォルト (`allowHalfOpen == false`) では、
保留されていた書き込みキューが出力されるとソケットはファイル識別子を破棄します。
しかし、`allowHalfOpen == true` が設定されていると、
ユーザがデータを書き込めるようにしておくために、ソケットは自動的に `end()` を呼び出さないので、
ユーザが `end()` を呼び出す必要があります。


#### Event: 'timeout'

`function () { }`

<!--

Emitted if the socket times out from inactivity. This is only to notify that
the socket has been idle. The user must manually close the connection.

See also: `socket.setTimeout()`

-->
ソケットがタイムアウトして非アクティブになった場合に生成されます。
これはソケットがアイドルになったことを通知するだけです。
利用者は手動でコネクションをクローズする必要があります。

`socket.setTimeout()` を参照してください。


#### Event: 'drain'

`function () { }`

<!--

Emitted when the write buffer becomes empty. Can be used to throttle uploads.

See also: the return values of `socket.write()`

-->
書き込みバッファが空になった場合に生成されます。アップロード速度を落とすために使うことができます。

`socket.write()` の戻り値を参照してください。

#### Event: 'error'

`function (exception) { }`

<!--

Emitted when an error occurs.  The `'close'` event will be called directly
following this event.

-->
エラーが発生した場合に生成されます。`'close'` イベントはこのイベントの後に直接呼び出されます。

#### Event: 'close'

`function (had_error) { }`

<!--

Emitted once the socket is fully closed. The argument `had_error` is a boolean
which says if the socket was closed due to a transmission error.

-->
ソケットが完全にクローズした場合に生成されます。
引数 `had_error` は boolean で、ソケットが転送エラーでクローズされたのかどうかを示します。

---

### net.isIP

#### net.isIP(input)

<!--

Tests if input is an IP address. Returns 0 for invalid strings,
returns 4 for IP version 4 addresses, and returns 6 for IP version 6 addresses.

-->


#### net.isIPv4(input)

<!--

Returns true if input is a version 4 IP address, otherwise returns false.

-->
input が バージョン 4 の IP アドレスなら true、そうでなければ false を返します。


#### net.isIPv6(input)

<!--

Returns true if input is a version 6 IP address, otherwise returns false.

-->
input が バージョン 6 の IP アドレスなら true、そうでなければ false を返します。

