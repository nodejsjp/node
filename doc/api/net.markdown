## net

<!--

The `net` module provides you with an asynchronous network wrapper. It contains
methods for creating both servers and clients (called streams). You can include
this module with `require("net");`

-->
`net` モジュールは非同期なネットワークのラッパーを提供します。
それはサーバとクライアントの両方 (ストリームと呼ばれます) を作成するための方法を含みます。
このモジュールは`require("net");`によって取り込むことができます。

### net.createServer(connectionListener)

<!--

Creates a new TCP server. The `connectionListener` argument is
automatically set as a listener for the `'connection'` event.

-->
新しい TCP サーバを作成します。
`connectionListener` 引数は `'connection'`
イベントに対するリスナーとして自動的に加えられます。

### net.createConnection(arguments...)

<!--

Construct a new stream object and opens a stream to the given location. When
the stream is established the `'connect'` event will be emitted.

-->
新しいストリームオブジェクトを構築し、与えられたロケーションへのストリームをオープンします。
ストリームが確立されると、`'connect'` イベントが生成されます。

<!--

The arguments for this method change the type of connection:

-->
このメソッドの引数はコネクションの種類によって変わります。

<!--

* `net.createConnection(port, [host])`

  Creates a TCP connection to `port` on `host`. If `host` is omitted, `localhost`
  will be assumed.

* `net.createConnection(path)`

  Creates unix socket connection to `path`

-->
* `net.createConnection(port, [host])`

  `host` 上の `port` に対する TCP コネクションを作成します。
  `host` が省略されると `localhost` が仮定されます。

* `net.createConnection(path)`

  `path` に対する UNIX ドメインソケットを作成します。

---

### net.Server

<!--

This class is used to create a TCP or UNIX server.

-->
このクラスは TCP または UNIX ドメインのサーバを作成するために使われます。

<!--

Here is an example of a echo server which listens for connections
on port 8124:

-->
8124 番のポートへの接続を待ち受けるエコーサーバの例:

    var net = require('net');
    var server = net.createServer(function (c) {
      c.write('hello\r\n');
      c.pipe(c);
    });
    server.listen(8124, 'localhost');

<!--

Test this by using `telnet`:

-->
`telnet` を使ってテストします:

    telnet localhost 8124

<!--

To listen on the socket `/tmp/echo.sock` the last line would just be
changed to

-->
`'/tmp/echo.sock'` へのソケットを待ち受けるには、最後の行をこのように変更します。

    server.listen('/tmp/echo.sock');

<!--

Use `nc` to connect to a UNIX domain socket server:

-->
`nc` を使って UNIX ドメインソケットサーバへ接続します:

    nc -U /tmp/echo.sock

<!--

`net.Server` is an `EventEmitter` with the following events:

-->
`net.Server` は以下のイベントを持つ `EventEmitter` です:

#### server.listen(port, [host], [callback])

<!--

Begin accepting connections on the specified `port` and `host`.  If the
`host` is omitted, the server will accept connections directed to any
IPv4 address (`INADDR_ANY`).

-->
指定された `port` と `host` でコネクションの受け入れを開始します。
`host` が省略されると、サーバはどんな IPv4 アドレスへの接続も受け入れます
(`INADDR_ANY`)。

<!--

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound.

-->
この関数は非同期です。最後の引数の `callback` はサーバがバインドすると呼び出されます。

<!--

One issue some users run into is getting `EADDRINUSE` errors. Meaning
another server is already running on the requested port. One way of handling this
would be to wait a second and the try again. This can be done with

-->
一部のユーザが陥る問題の一つは、`EADDRINUSE` エラーです．
これは、他のサーバが要求されたポートを使っているという意味です。
これに対照する方法の一つは、1秒待機してリトライすることです。
これは次のようになります

    server.on('error', function (e) {
      if (e.errno == require('constants').EADDRINUSE) {
        console.log('Address in use, retrying...');
        setTimeout(function () {
          server.close();
          server.listen(PORT, HOST);
        }, 1000);
      }
    });

<!--

(Note: All sockets in Node are set SO_REUSEADDR already)

-->
注意: Node の全てのソケットは SO_REUSEADDR が設定されます)


#### server.listen(path, [callback])

<!--

Start a UNIX socket server listening for connections on the given `path`.

-->
与えられた `path` へのコネクションを待ち受けるする UNIX ドメインソケットのサーバを開始します。

<!--

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound.

-->
この関数は非同期です。
最後の引数の `callback` はサーバがバインドすると呼び出されます。

#### server.listenFD(fd)

<!--

Start a server listening for connections on the given file descriptor.

-->
与えられたファイル記述子上のコネクションを待ち受けるサーバを開始します。

<!--

This file descriptor must have already had the `bind(2)` and `listen(2)` system
calls invoked on it.

-->
このファイル記述子は既に `bind(2)` および
`listen(2)` システムコールが呼び出されていなければなりません。

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

Returns the bound address of the server as seen by the operating system.
Useful to find which port was assigned when giving getting an OS-assigned address

-->
オペレーティングシステムによってサーバにバインドされたアドレスを返します。
OSによって割り当てられたアドレスが渡された時に、どのポートに割り当てられたものかを調べるのに便利です。

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


#### server.maxConnections

<!--

Set this property to reject connections when the server's connection count gets high.

-->
サーバの接続数が大きくなった時に接続を拒否するためにこのプロパティを設定します。

#### server.connections

<!--

The number of concurrent connections on the server.

-->
このサーバ上の並行コネクションの数です。

#### Event: 'connection'

`function (stream) {}`

<!--

Emitted when a new connection is made. `stream` is an instance of
`net.Stream`.

-->
新しいコネクションが作成されると生成されます。
`stream` は `net.Stream` のインスタンスです。

#### Event: 'close'

`function () {}`

<!--

Emitted when the server closes.

-->
サーバがクローズした時に生成されます。

---

### net.Stream

<!--

This object is an abstraction of of a TCP or UNIX socket.  `net.Stream`
instance implement a duplex stream interface.  They can be created by the
user and used as a client (with `connect()`) or they can be created by Node
and passed to the user through the `'connection'` event of a server.

-->
このオブジェクトは TCP または UNIX ドメインのソケットを抽象化したものです。
`net.Stream` のインスタンスは双方向のストリームインタフェースを実装します。
それらはユーザによって (`connect()` によって) 作成されてクライアントとして使われるか、
Node によって作成されてサーバの `'connection'` イベントを通じてユーザに渡されます。

<!--

`net.Stream` instances are EventEmitters with the following events:

-->
`net.Stream` のインスタンスは以下のイベントを持つ EventEmitter です:

#### stream.connect(port, [host], [callback])
#### stream.connect(path, [callback])

<!--

Opens the connection for a given stream. If `port` and `host` are given,
then the stream will be opened as a TCP stream, if `host` is omitted,
`localhost` will be assumed. If a `path` is given, the stream will be
opened as a unix socket to that path.

-->
与えられたストリームでコネクションをオープンします。
`port` と `host` が与えられた場合、
ストリームは TCP ストリームとしてオープンされます。
`host` が省略された場合は `localhost` が仮定されます。
`path` が与えられた場合は、
ストリームはそのパスへの UNIX ドメインソケットとしてオープンされます。

<!--

Normally this method is not needed, as `net.createConnection` opens the
stream. Use this only if you are implementing a custom Stream or if a
Stream is closed and you want to reuse it to connect to another server.

-->
通常このメソッドは必要なく、`net.createConnection` でストリームをオープンします。
これを使うのは、カスタマイズされたストリームを実装している場合や、
ストリームがクローズされた後にコネクションを再利用して別のサーバに接続する場合だけです。

<!--

This function is asynchronous. When the `'connect'` event is emitted the
stream is established. If there is a problem connecting, the `'connect'`
event will not be emitted, the `'error'` event will be emitted with
the exception.

-->
この関数は非同期です。ストリームが確立されると `'connect'` イベントが生成されます。
接続で問題があった場合は `'connect'` イベントは生成されず、
例外とともに `'error'` イベントが生成されます。

<!--

The `callback` paramenter will be added as an listener for the 'connect'
event.

-->
`callback` 引数は 'connect' イベントのリスナに加えられます。


#### stream.setEncoding(encoding=null)

<!--

Sets the encoding (either `'ascii'`, `'utf8'`, or `'base64'`) for data that is
received.

-->
受信したデータのエンコーディングを設定します (`'ascii'`、`'utf8'`、
あるいは `'base64'` のいずれかです)。

#### stream.setSecure([credentials])

<!--

Enables SSL support for the stream, with the crypto module credentials specifying
the private key and certificate of the stream, and optionally the CA certificates
for use in peer authentication.

-->
秘密鍵とサーバ証明書を指定した暗号モジュールの認証情報で、
ストリームに対して SSL サポートを有効にします。
オプションで認証局で証明された相手側の認証を使うこともできます。

<!--

If the credentials hold one ore more CA certificates, then the stream will request
for the peer to submit a client certificate as part of the SSL connection handshake.
The validity and content of this can be accessed via `verifyPeer()` and `getPeerCertificate()`.

-->
認証情報が一つ以上の認証局の証明書を持っている場合、
ストリームは SSL コネクションにおけるハンドシェークの一部としてクライアント証明書を送るよう相手に要求します。
その有効性と内容は、`verifyPeer()` と `getPeerCertificate()` を通じてアクセスできます。

#### stream.verifyPeer()

<!--

Returns true or false depending on the validity of the peers's certificate in the
context of the defined or default list of trusted CA certificates.

-->
指定された、あるいはデフォルトの信頼された認証局の証明書において、
相手の証明書の妥当性に応じて true または false を返します。

#### stream.getPeerCertificate()

<!--

Returns a JSON structure detailing the peer's certificate, containing a dictionary
with keys for the certificate `'subject'`, `'issuer'`, `'valid_from'` and `'valid_to'`.

-->
相手の証明書の詳細を、`'subject'`、`'issuer'`、`'valid_from'`
そして `'valid_to'` をキーとする証明書の辞書を含む JSON 形式で返します。

#### stream.write(data, [encoding], [callback])

<!--

Sends data on the stream. The second parameter specifies the encoding in the
case of a string--it defaults to UTF8 encoding.

-->ストリームにデータを送信します。
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

#### stream.write(data, [encoding], [fileDescriptor], [callback])

<!--

For UNIX sockets, it is possible to send a file descriptor through the
stream. Simply add the `fileDescriptor` argument and listen for the `'fd'`
event on the other end.

-->
UNIX ソケットの場合、ファイル記述子をストリームに送信することができます。
単純に `fileDescriptor` 引数を加えることで、相手側には `'fd'` イベントが生成されます。


#### stream.end([data], [encoding])

<!--

Half-closes the stream. I.E., it sends a FIN packet. It is possible the
server will still send some data.

-->
ストリームをハーフクローズします。例えば FIN パケットを送信します。
サーバはまだデータを送り続けてくることができます。

<!--

If `data` is specified, it is equivalent to calling `stream.write(data, encoding)`
followed by `stream.end()`.

-->
`data` が指定された場合は、
`stream.write(data, encoding)` に続けて `stream.end()` を呼び出すのと等価です。

#### stream.destroy()

<!--

Ensures that no more I/O activity happens on this stream. Only necessary in
case of errors (parse error or so).

-->
このストリーム上でどんな I/O も起こらないことを保証します。
(パースエラーなどの) エラーの場合にだけ必要です。

#### stream.pause()

<!--

Pauses the reading of data. That is, `'data'` events will not be emitted.
Useful to throttle back an upload.

-->
データの読み込みを中断します。つまり、`'data'` イベントは生成されません。
アップロード速度を落とすために便利です。

#### stream.resume()

<!--

Resumes reading after a call to `pause()`.

-->
`pause()` を呼び出した後で読み込みを再開します。

#### stream.setTimeout(timeout)

<!--

Sets the stream to timeout after `timeout` milliseconds of inactivity on
the stream. By default `net.Stream` do not have a timeout.

-->

<!--

When an idle timeout is triggered the stream will receive a `'timeout'`
event but the connection will not be severed. The user must manually `end()`
or `destroy()` the stream.

-->
アイドルタイムアウトが引き起こされると、ストリームは `'timeout'` イベントを受信しますが、
コネクションは切断されません。
ユーザは手動で `end()` または `destroy()` を呼び出す必要があります。

<!--

If `timeout` is 0, then the existing idle timeout is disabled.

-->
`timeout` が 0 の場合、アイドルタイムアウトは無効にされます。

#### stream.setNoDelay(noDelay=true)

<!--

Disables the Nagle algorithm. By default TCP connections use the Nagle
algorithm, they buffer data before sending it off. Setting `noDelay` will
immediately fire off data each time `stream.write()` is called.

-->
Nagle アルゴリズムを無効にします。
デフォルトでは TCP コネクションは Nagle アルゴリズムを使用し、データを送信する前にバッファリングします。
`noDelay` に設定すると、データは `stream.write()` を呼び出す度に即座に送信されます。

#### stream.setKeepAlive(enable=false, [initialDelay])

<!--

Enable/disable keep-alive functionality, and optionally set the initial
delay before the first keepalive probe is sent on an idle stream.
Set `initialDelay` (in milliseconds) to set the delay between the last
data packet received and the first keepalive probe. Setting 0 for
initialDelay will leave the value unchanged from the default
(or previous) setting.

-->
キープアライブ機能を有効/無効にします。
オプションで最初の keepalive probe がアイドルストリームに送信されるまでの初期遅延を設定します。
`initialDelay` (ミリ秒) が設定されると、
最後にデータパケットを受信してから最初の keepalive probe までの遅延が設定されます。
初期遅延に 0 が設定されると、デフォルト設定から値を変更されないようにします。

#### stream.remoteAddress

<!--

The string representation of the remote IP address. For example,
`'74.125.127.100'` or `'2001:4860:a005::68'`.

-->
リモートの IP アドレスを表現する文字列です。
例えば、`'74.125.127.100'` あるいは `'2001:4860:a005::68'`。

<!--

This member is only present in server-side connections.

-->
このメンバはサーバサイドのコネクションでのみ与えられます。


#### Event: 'connect'

`function () { }`

<!--

Emitted when a stream connection successfully is established.
See `connect()`.

-->
ストリームコネクションの確立が成功した場合に生成されます。
`connect()` を参照してください。

#### Event: 'data'

`function (data) { }`

<!--

Emitted when data is received.  The argument `data` will be a `Buffer` or
`String`.  Encoding of data is set by `stream.setEncoding()`.
(See the section on `Readable Stream` for more information.)

-->
データを受信した場合に生成されます。
`data` 引数は `Buffer` または `String` です。
データのエンコーディングは `stream.setEncoding()` で設定されます。
(より詳しい情報は `Readable Stream` を参照してください)。

#### Event: 'end'

`function () { }`

<!--

Emitted when the other end of the stream sends a FIN packet.

-->
ストリームの相手側が FIN パケットを送信した場合に生成されます。

<!--

By default (`allowHalfOpen == false`) the stream will destroy its file
descriptor  once it has written out its pending write queue.  However, by
setting `allowHalfOpen == true` the stream will not automatically `end()`
its side allowing the user to write arbitrary amounts of data, with the
caveat that the user is required to `end()` their side now.

-->
デフォルト (`allowHalfOpen == false`) では、
保留されていた書き込みキューが出力されるとストリームはファイル識別子を破棄します。
しかし、`allowHalfOpen == true` が設定されていると、
ユーザがデータを書き込めるようにしておくために、ストリームは自動的に `end()` を呼び出さないので、
ユーザが `end()` を呼び出す必要があります。


#### Event: 'timeout'

`function () { }`

<!--

Emitted if the stream times out from inactivity. This is only to notify that
the stream has been idle. The user must manually close the connection.

-->
ストリームがタイムアウトして非アクティブになった場合に生成されます。
これはストリームがアイドルになったことを通知するだけです。
利用者は手動でコネクションをクローズする必要があります。

See also: `stream.setTimeout()`


#### Event: 'drain'

`function () { }`

<!--

Emitted when the write buffer becomes empty. Can be used to throttle uploads.

-->
書き込みバッファが空になった場合に生成されます。アップロード速度を落とすために使うことができます。

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

Emitted once the stream is fully closed. The argument `had_error` is a boolean
which says if the stream was closed due to a transmission error.

-->
ストリームが完全にクローズした場合に生成されます。
引数 `had_error` は boolean で、ストリームが転送エラーでクローズされたのかどうかを示します。

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

