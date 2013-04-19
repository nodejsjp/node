# HTTP

    Stability: 3 - Stable

<!--
To use the HTTP server and client one must `require('http')`.
-->

HTTP サーバおよびクライアントを使用するにはいずれも `require('http')` が必要です。

<!--
The HTTP interfaces in Node are designed to support many features
of the protocol which have been traditionally difficult to use.
In particular, large, possibly chunk-encoded, messages. The interface is
careful to never buffer entire requests or responses--the
user is able to stream data.
-->

Node の HTTP インタフェースは、
伝統的に扱いが難しかったプロトコルの多くの機能をサポートするように設計されています。
とりわけ大きくて、場合によってはチャンク化されたメッセージです。
インタフェースは決してリクエストまたはレスポンス全体をバッファリングしないように気をつけています
－ 利用者はストリームデータを使うことができます。

<!--
HTTP message headers are represented by an object like this:
-->

HTTP メッセージヘッダはこのようなオブジェクトとして表現されます:

    { 'content-length': '123',
      'content-type': 'text/plain',
      'connection': 'keep-alive',
      'accept': '*/*' }

<!--
Keys are lowercased. Values are not modified.
-->

キーは小文字化されます。値は変更されません。

<!--
In order to support the full spectrum of possible HTTP applications, Node's
HTTP API is very low-level. It deals with stream handling and message
parsing only. It parses a message into headers and body but it does not
parse the actual headers or the body.
-->

考えられる HTTP アプリケーションを完全にサポートするために、
Node の HTTP API はとても低水準です。それはストリームのハンドリングとメッセージの解析だけに対処します。
解析はメッセージをヘッダとボディに分けますが、実際のヘッダとボディは解析しません。


## http.STATUS_CODES

* {Object}

<!--
A collection of all the standard HTTP response status codes, and the
short description of each.  For example, `http.STATUS_CODES[404] === 'Not
Found'`.
-->

全ての HTTP 標準ステータスコードと短い説明のコレクションです。
たとえば、`http.STATUS_CODES[404] === 'Not Found'`。

## http.createServer([requestListener])

<!--
Returns a new web server object.
-->

新しい Web サーバオブジェクトを返します。

<!--
The `requestListener` is a function which is automatically
added to the `'request'` event.
-->

`requestListener` は自動的に `'request'` イベントに加えられる関数です。

## http.createClient([port], [host])

<!--
This function is **deprecated**; please use [http.request()][] instead.
Constructs a new HTTP client. `port` and `host` refer to the server to be
connected to.
-->

この関数は **deprecated** です; 代わりに
[http.request()][] を使用してください。
新しい HTTP クライアントを構築します。
`port` と `host` は接続するサーバを示します。

## Class: http.Server

<!--
This is an [EventEmitter][] with the following events:
-->

これは以下のイベントを持つ [EventEmitter][] です:

### Event: 'request'

`function (request, response) { }`

<!--
Emitted each time there is a request. Note that there may be multiple requests
per connection (in the case of keep-alive connections).
-->

リクエストの度に生成されます。
コネクションごとに複数のリクエストがあるかもしれないことに注意してください
(Keep Alive なコネクションの場合)。

<!--
 `request` is an instance of `http.IncomingMessage` and `response` is
 an instance of `http.ServerResponse`
-->

`request` は `http.IncomingMessage` のインスタンス、
`response` は `http.ServerResponse` のインスタンスです。

### Event: 'connection'

`function (socket) { }`

<!--
 When a new TCP stream is established. `socket` is an object of type
 `net.Socket`. Usually users will not want to access this event. The
 `socket` can also be accessed at `request.connection`.
-->

新しい TCP ストリームが確立した時。
`socket` は `net.Socket` 型のオブジェクトです。
通常の利用者がこのイベントにアクセスしたくなることはないでしょう。
`socket` は `request.connection` からアクセスすることもできます。

### Event: 'close'

`function () { }`

<!--
 Emitted when the server closes.
-->

サーバがクローズした時に生成されます。

### Event: 'checkContinue'

`function (request, response) { }`

<!--
Emitted each time a request with an http Expect: 100-continue is received.
If this event isn't listened for, the server will automatically respond
with a 100 Continue as appropriate.
-->

httpの Expect: 100-continue リクエストを受信する度に生成されます。
このイベントが監視されない場合、サーバは自動的に 100 Continue を応答します。

<!--
Handling this event involves calling `response.writeContinue` if the client
should continue to send the request body, or generating an appropriate HTTP
response (e.g., 400 Bad Request) if the client should not continue to send the
request body.
-->

このイベントを処理する場合、クライアントがリクエストボディを送信し続けるべきなら
`response.writeContinue` を呼び出す必要があります。
あるいは、クライアントがリクエストボディを送信し続けるべきでないなら、
適切な HTTP レスポンス (例えば 400 Bad Request) を生成します。

<!--
Note that when this event is emitted and handled, the `request` event will
not be emitted.
-->

このイベントが生成されて処理された場合、`request`イベントは生成されないことに注意してください。

### Event: 'connect'

`function (request, socket, head) { }`

<!--
Emitted each time a client requests a http CONNECT method. If this event isn't
listened for, then clients requesting a CONNECT method will have their
connections closed.
-->

クライアントが HTTP の CONNECT メソッドを要求する度に生成されます。
このイベントが監視されない場合、CONNECT メソッドを要求したクライアントのコネクションはクローズされます。

<!--
* `request` is the arguments for the http request, as it is in the request
  event.
* `socket` is the network socket between the server and client.
* `head` is an instance of Buffer, the first packet of the tunneling stream,
  this may be empty.
-->

* `request` はリクエストイベントの引数と同様に HTTP リクエストです。
* `socket` はサーバとクライアントの間のネットワークソケットです。
* `head` はトンネリングストリームの最初のパケットを持つ Buffer のインスタンスです。
  空の場合もあります。

<!--
After this event is emitted, the request's socket will not have a `data`
event listener, meaning you will need to bind to it in order to handle data
sent to the server on that socket.
-->

このイベントが生成された後、リクエスト元のソケットはもう `data` イベントリスナーを持ちません。
このソケットでサーバへ送られたデータを扱うためにそれをバインドしなければならないことを意味します。

### Event: 'upgrade'

`function (request, socket, head) { }`

<!--
Emitted each time a client requests a http upgrade. If this event isn't
listened for, then clients requesting an upgrade will have their connections
closed.
-->

クライアントが HTTP のアップグレードを要求する度に生成されます。
このイベントが監視されない場合、アップグレードを要求したクライアントのコネクションはクローズされます。

<!--
* `request` is the arguments for the http request, as it is in the request
  event.
* `socket` is the network socket between the server and client.
* `head` is an instance of Buffer, the first packet of the upgraded stream,
  this may be empty.
-->

* `request` はリクエストイベントの引数と同様に HTTP リクエストです。
* `socket` はサーバとクライアントの間のネットワークソケットです。
* `head` はアップグレードストリームの最初のパケットを持つ Buffer のインスタンスです。
  空の場合もあります。

<!--
After this event is emitted, the request's socket will not have a `data`
event listener, meaning you will need to bind to it in order to handle data
sent to the server on that socket.
-->

このイベントが生成された後、リクエスト元のソケットはもう `data` イベントリスナーを持ちません。
このソケットでサーバへ送られたデータを扱うためにそれをバインドしなければならないことを意味します。

### Event: 'clientError'

`function (exception, socket) { }`

<!--
If a client connection emits an 'error' event - it will forwarded here.
-->

クライアントコネクションが 'error' イベントを発した場合 － ここに転送されます。

<!--
`socket` is the `net.Socket` object that the error originated from.
-->

`socket` はエラーが発生した `net.Socket` オブジェクトです。


### server.listen(port, [hostname], [backlog], [callback])

<!--
Begin accepting connections on the specified port and hostname.  If the
hostname is omitted, the server will accept connections directed to any
IPv4 address (`INADDR_ANY`).
-->

指定されたポートとホスト名でコネクションの受け入れを開始します。
ホスト名が省略されると、サーバはどんな IPv4 アドレスへの接続も受け入れます (`INADDR_ANY`)。

<!--
To listen to a unix socket, supply a filename instead of port and hostname.
-->

UNIX ドメインソケットを待ち受ける場合、ポートとホスト名ではなくファイル名を提供します。

<!--
Backlog is the maximum length of the queue of pending connections.
The actual length will be determined by your OS through sysctl settings such as
`tcp_max_syn_backlog` and `somaxconn` on linux. The default value of this
parameter is 511 (not 512).
-->

バックログは保留された接続のキューの最大長です。
実際の長さは Linux では `tcp_max_syn_backlog` や `somaxconn` など、
sysctl の設定を通じて OS によって決定されます。
このパラメータのデフォルト値は 511 (512 ではありません) です。

<!--
This function is asynchronous. The last parameter `callback` will be added as
a listener for the ['listening'][] event.  See also [net.Server.listen(port)][].
-->

この関数は非同期です。最後の引数の `callback` は
['listening'][] イベントのリスナとして加えられます。
詳細は [net.Server.listen(port)][] を参照してください。


### server.listen(path, [callback])

<!--
Start a UNIX socket server listening for connections on the given `path`.
-->

`path` で与えられたコネクションを待ち受ける UNIX ドメインソケットのサーバを開始します。

<!--
This function is asynchronous. The last parameter `callback` will be added as
a listener for the ['listening'][] event.  See also [net.Server.listen(path)][].
-->

この関数は非同期です。最後の引数の `callback` は
['listening'][] イベントのリスナとして加えられます。
詳細は [net.Server.listen(path)][] を参照してください。


### server.listen(handle, [callback])

* `handle` {Object}
* `callback` {Function}

<!--
The `handle` object can be set to either a server or socket (anything
with an underlying `_handle` member), or a `{fd: <n>}` object.
-->

`handle` オブジェクトには、サーバまたはソケット (下層の `_handle` メンバなら
なんでも) または、 `{fd: <n>}` オブジェクトを設定することができます。

<!--
This will cause the server to accept connections on the specified
handle, but it is presumed that the file descriptor or handle has
already been bound to a port or domain socket.
-->

これによりサーバは指定したハンドルへの接続を受け付けることになりますが、
ファイル記述子またはハンドルは既にポートまたはドメインソケットに
バインドされているものと見なされます。

<!--
Listening on a file descriptor is not supported on Windows.
-->

ファイル記述子へのリスニングは Windows ではサポートされません。

<!--
This function is asynchronous. The last parameter `callback` will be added as
a listener for the ['listening'](net.html#event_listening_) event.
See also [net.Server.listen()](net.html#net_server_listen_handle_callback).
-->

この関数は非同期です。最後の引数の `callback` は
['listening'](net.html#event_listening_) イベントのリスナとして加えられます。
詳細は [net.Server.listen()](net.html#net_server_listen_handle_callback)
を参照してください。

### server.close([callback])

<!--
Stops the server from accepting new connections.  See [net.Server.close()][].
-->

サーバが新しいコネクションを受け付けるのを終了します。
[net.Server.close()][] を参照してください。

### server.maxHeadersCount

<!--
Limits maximum incoming headers count, equal to 1000 by default. If set to 0 -
no limit will be applied.
-->

受け付けるヘッダ数の上限で、デフォルトは 1000 です。
0 に設定されると、制限しないことになります。

### server.setTimeout(msecs, callback)

* `msecs` {Number}
* `callback` {Function}

<!--
Sets the timeout value for sockets, and emits a `'timeout'` event on
the Server object, passing the socket as an argument, if a timeout
occurs.
-->

ソケットにタイムアウト値を設定し、サーバオブジェクト上で `'timeout'`
イベントを生成します。
タイムアウトが発生すると、ソケットが引数として渡されます。

<!--
If there is a `'timeout'` event listener on the Server object, then it
will be called with the timed-out socket as an argument.
-->

サーバオブジェクトに `'timeout'` イベントのリスナが存在すると、
それはタイムアウトしたソケットを引数として呼び出されます。

<!--
By default, the Server's timeout value is 2 minutes, and sockets are
destroyed automatically if they time out.  However, if you assign a
callback to the Server's `'timeout'` event, then you are responsible
for handling socket timeouts.
-->

デフォルトでは、サーバのタイムアウト値は 2 分で、
タイムアウトしたソケットは自動的に破棄されます。
しかしながら、`'timeout'` イベントのコールバックをサーバに割り当てた場合、
タイムアウトしたソケットのハンドリングはあなたの責務となります。

### server.timeout

<!--
* {Number} Default = 120000 (2 minutes)
-->

* {Number} デフォルト = 120000 (2 分)

<!--
The number of milliseconds of inactivity before a socket is presumed
to have timed out.
-->

不活性なソケットがタイムアウトしたと推定されるまでのミリ秒を表す数値。

<!--
Note that the socket timeout logic is set up on connection, so
changing this value only affects *new* connections to the server, not
any existing connections.
-->

ソケットのタイムアウト処理は接続のセットアップ時に行われるため、
この値の変更は既存の接続ではなく、サーバへの *新しい* 接続にだけ
影響することに注意してください。

<!--
Set to 0 to disable any kind of automatic timeout behavior on incoming
connections.
-->

0 を設定すると、到着する接続に対する自動的なタイムアウトの振る舞いは
無効になります。

## Class: http.ServerResponse

<!--
This object is created internally by a HTTP server--not by the user. It is
passed as the second parameter to the `'request'` event.

The response implements the [Writable Stream][] interface. This is an
[EventEmitter][] with the following events:
-->

このオブジェクトは HTTP サーバ内部 － ユーザではなく － で作成されます。
`'request'` リスナーの第 2 引数として渡されます。

レスポンスは [Writable  Stream][] インタフェースを実装します。
これは以下のイベントを持つ [EventEmitter][] です:

### Event: 'close'

`function () { }`

<!--
Indicates that the underlying connection was terminated before
`response.end()` was called or able to flush.
-->

`response.end()` が呼び出されたりフラッシュされる前に、
下層の接続が切断されたことを示します。

### response.writeContinue()

<!--
Sends a HTTP/1.1 100 Continue message to the client, indicating that
the request body should be sent. See the ['checkContinue'][] event on `Server`.
-->

HTTP/1.1 の 100 Continue メッセージをクライアントに送信し、
リクエストボディを送信してもよいことを示します。
`Server`の ['checkContinue'][] イベントを参照してください。

### response.writeHead(statusCode, [reasonPhrase], [headers])

<!--
Sends a response header to the request. The status code is a 3-digit HTTP
status code, like `404`. The last argument, `headers`, are the response headers.
Optionally one can give a human-readable `reasonPhrase` as the second
argument.
-->

レスポンスヘッダを送信します。
ステータスコードは `404` のような 3 桁の数字による HTTP ステータスコードです。
最後の引数 `headers` は、レスポンスヘッダです。
オプションとして人に読める形式の `reasonPhrase` を第 2 引数で与えることができます。

<!--
Example:
-->

例:

    var body = 'hello world';
    response.writeHead(200, {
      'Content-Length': body.length,
      'Content-Type': 'text/plain' });

<!--
This method must only be called once on a message and it must
be called before `response.end()` is called.

If you call `response.write()` or `response.end()` before calling this, the
implicit/mutable headers will be calculated and call this function for you.
-->

このメソッドはメッセージごとに 1 回だけ呼び出されなくてはならず、
`response.end()` の前に呼び出されなければなりません。

もしこのメソッドが呼び出される前に `response.write()` または `response.end()` が呼ばれると、暗黙的で可変のヘッダが算出されてこの関数が呼び出されます。

<!--
Note: that Content-Length is given in bytes not characters. The above example
works because the string `'hello world'` contains only single byte characters.
If the body contains higher coded characters then `Buffer.byteLength()`
should be used to determine the number of bytes in a given encoding.
And Node does not check whether Content-Length and the length of the body
which has been transmitted are equal or not.
-->

注意: `Content-Length` は文字数ではなくバイト数で与えられます。
上の例が動作するのは `'hello world'` という文字列が単一バイト文字だけを含むためです。
もしボディがより上位にコード化された文字を含む場合は、
指定したエンコーディングによるバイト数を得るために `Buffer.byteLength()` を使うべきです。
Node は、Content-Length と実際に送信されたレスポンスボディの長さが等しいかどうかチェックしません。

### response.setTimeout(msecs, callback)

* `msecs` {Number}
* `callback` {Function}

<!--
Sets the Socket's timeout value to `msecs`.  If a callback is
provided, then it is added as a listener on the `'timeout'` event on
the response object.
-->

ソケットのタイムアウト値を `msec` に設定します。
コールバックが与えられると、それはレスポンスオブジェクトの `'timeout'`
イベントのリスナとして加えられます。

<!--
If no `'timeout'` listener is added to the request, the response, or
the server, then sockets are destroyed when they time out.  If you
assign a handler on the request, the response, or the server's
`'timeout'` events, then it is your responsibility to handle timed out
sockets.
-->

リクエスト、レスポンス、そしてサーバのいずれにも `'timeout'`
リスナが存在しない場合、タイムアウトしたソケットは破棄されます。
もしリクエスト、レスポンス、サーバのいずれかに `'timeout'` イベントを
設定した場合、タイムアウトしたソケットのハンドリングはあなたの責務となります。

### response.statusCode

<!--
When using implicit headers (not calling `response.writeHead()` explicitly), this property
controls the status code that will be sent to the client when the headers get
flushed.
-->

(`response.writeHead()` が明示的に呼ばれないために) 暗黙的なヘッダが使われる場合、このプロパティはヘッダがフラッシュされる時にクライアントへ送信されるステータスコードを制御します。

<!--
Example:
-->

例:

    response.statusCode = 404;

<!--
After response header was sent to the client, this property indicates the
status code which was sent out.
-->

レスポンスヘッダがクライアントに送信された後、
このプロパティは送信されたステータスコードを示します。

### response.setHeader(name, value)

<!--
Sets a single header value for implicit headers.  If this header already exists
in the to-be-sent headers, its value will be replaced.  Use an array of strings
here if you need to send multiple headers with the same name.
-->

暗黙的ヘッダのヘッダ値を設定します。
送信されようとしているレスポンスヘッダにこのヘッダが既に含まれている場合、
その値は置き換えられます。
同じ名前で複数のヘッダを送信したい場合は文字列の配列を使ってください。

<!--
Example:
-->

例:

    response.setHeader("Content-Type", "text/html");

<!--
or
-->

または

    response.setHeader("Set-Cookie", ["type=ninja", "language=javascript"]);

### response.headersSent

<!--
Boolean (read-only). True if headers were sent, false otherwise.
-->

(読み込み専用の) Boolean。
ヘッダが送信済みなら true、それ以外は false です。

### response.sendDate

<!--
When true, the Date header will be automatically generated and sent in 
the response if it is not already present in the headers. Defaults to true.

This should only be disabled for testing; HTTP requires the Date header
in responses.
-->

`true` の場合、Date ヘッダが自動的に生成され、レスポンスとして送信されます
(`headers` にすでに与えられていない場合)。
デフォルトは `true` です。

これを無効にするのはテストでのみにすべきです。
HTTP はレスポンスに Date ヘッダを要求します。

### response.getHeader(name)

<!--
Reads out a header that's already been queued but not sent to the client.  Note
that the name is case insensitive.  This can only be called before headers get
implicitly flushed.
-->

すでにキューに入れられているが未送信のヘッダを読み上げます．
名前は大文字小文字を区別しないことに注意してください。
これはヘッダが暗黙的にフラッシュされる前だけ呼び出すことができます。

<!--
Example:
-->

例:

    var contentType = response.getHeader('content-type');

### response.removeHeader(name)

<!--
Removes a header that's queued for implicit sending.
-->

暗黙的に送信するためキューに入れられたヘッダを削除します。

<!--
Example:
-->

例:

    response.removeHeader("Content-Encoding");


### response.write(chunk, [encoding])

<!--
If this method is called and `response.writeHead()` has not been called, it will
switch to implicit header mode and flush the implicit headers.

This sends a chunk of the response body. This method may
be called multiple times to provide successive parts of the body.
-->

このメソッドが呼び出され、`response.writeHead()` が呼び出されなければ、
暗黙的ヘッダモードに切り替わり、暗黙的ヘッダはフラッシュされます。

これはレスポンスボディのチャンクを送信します。
このメソッドはボディの連続した部分を提供するために複数回呼び出されるかもしれません。

<!--
`chunk` can be a string or a buffer. If `chunk` is a string,
the second parameter specifies how to encode it into a byte stream.
By default the `encoding` is `'utf8'`.
-->

`chunk` は文字列またはバッファにすることができます。
`chunk` が文字列の場合、どのエンコードでバイトストリームにするかを第 2 引数で指定します。
デフォルトの `encoding` は `'utf8'` です。

<!--
**Note**: This is the raw HTTP body and has nothing to do with
higher-level multi-part body encodings that may be used.
-->

**注意**: これは生の HTTP ボディで、
高水準のマルチパートボディエンコーディングで使われるものとは無関係です。

<!--
The first time `response.write()` is called, it will send the buffered
header information and the first body to the client. The second time
`response.write()` is called, Node assumes you're going to be streaming
data, and sends that separately. That is, the response is buffered up to the
first chunk of body.
-->

初めて `response.write()` が呼び出されると、
バッファリングされていたヘッダ情報と最初のボディがクライアントに送信されます。
2 回目に `response.write()` が呼ばれると、
Node はストリーミングデータを分割して送信しようとしていると仮定します。
すなわち、レスポンスはボディの最初のチャンクまでバッファリングされます。

<!--
Returns `true` if the entire data was flushed successfully to the kernel
buffer. Returns `false` if all or part of the data was queued in user memory.
`'drain'` will be emitted when the buffer is again free.
-->

データ全体のカーネルバッファへのフラッシュが成功すると `true` を返します。
データ全体または一部がユーザメモリ内のキューに入れられた場合は
`false` を返します。
再びバッファが空いた場合は `'drain'` イベントが生成されます。

### response.addTrailers(headers)

<!--
This method adds HTTP trailing headers (a header but at the end of the
message) to the response.
-->

このメソッドは HTTP トレーラヘッダ (メッセージの最後に置かれるヘッダ) をレスポンスに追加します。

<!--
Trailers will **only** be emitted if chunked encoding is used for the
response; if it is not (e.g., if the request was HTTP/1.0), they will
be silently discarded.
-->

トレーラはレスポンスがチャンク化されたエンコーディングで**のみ**生成されます;
そうでなければ (例えばリクエストが HTTP/1.0)、黙って破棄されます。

<!--
Note that HTTP requires the `Trailer` header to be sent if you intend to
emit trailers, with a list of the header fields in its value. E.g.,
-->

HTTP は、トレーラを生成するならそのヘッダフィールドのリストを値として
`Trailer` ヘッダを送信することを要求していることに注意してください。

    response.writeHead(200, { 'Content-Type': 'text/plain',
                              'Trailer': 'Content-MD5' });
    response.write(fileData);
    response.addTrailers({'Content-MD5': "7895bf4b8828b55ceaf47747b4bca667"});
    response.end();


### response.end([data], [encoding])

<!--
This method signals to the server that all of the response headers and body
have been sent; that server should consider this message complete.
The method, `response.end()`, MUST be called on each
response.
-->

このメソッドはレスポンスの全てのヘッダとボディを送信したことをサーバに伝えます;
サーバはメッセージが終了したと考えるべきです。
この `response.end()` メソッドは各レスポンスごとに呼び出さなければ*なりません*。

<!--
If `data` is specified, it is equivalent to calling `response.write(data, encoding)`
followed by `response.end()`.
-->

`data` が指定された場合、
`response.write(data, encoding)` に続けて `response.end()` を呼び出すのと等価です。


## http.request(options, callback)

<!--
Node maintains several connections per server to make HTTP requests.
This function allows one to transparently issue requests.
-->

Node は HTTP リクエストを行うために、サーバごとにいくつかのコネクションを
保持します。
この関数はその一つを使って透過的にリクエストを発行できるようにします。

<!--
`options` can be an object or a string. If `options` is a string, it is
automatically parsed with [url.parse()][].
-->

`options` はオブジェクトまたは文字列です。
もし `options` が文字列なら、それは [url.parse()][] によって自動的に
解析されます。

<!--
Options:
-->

オプション:

<!--
- `host`: A domain name or IP address of the server to issue the request to.
  Defaults to `'localhost'`.
- `hostname`: To support `url.parse()` `hostname` is preferred over `host`
- `port`: Port of remote server. Defaults to 80.
- `localAddress`: Local interface to bind for network connections.
- `socketPath`: Unix Domain Socket (use one of host:port or socketPath)
- `method`: A string specifying the HTTP request method. Defaults to `'GET'`.
- `path`: Request path. Defaults to `'/'`. Should include query string if any.
  E.G. `'/index.html?page=12'`
- `headers`: An object containing request headers.
- `auth`: Basic authentication i.e. `'user:password'` to compute an
  Authorization header.
- `agent`: Controls [Agent][] behavior. When an Agent is used request will
  default to `Connection: keep-alive`. Possible values:
 - `undefined` (default): use [global Agent][] for this host and port.
 - `Agent` object: explicitly use the passed in `Agent`.
 - `false`: opts out of connection pooling with an Agent, defaults request to
   `Connection: close`.
-->

- `host`: リクエストを発行するサーバのドメイン名または IP アドレス。
- `hostname`: `url.parse()` サポート。`hostname` は `host` を上書きします。
- `port`: リモートサーバのポート。デフォルトは 80 です。
- `localAddress`: ネットワーク接続をバインドするローカルインタフェースです。
- `socketPath`: Unix ドメインソケット (host:port または socketPath のどちらか)
- `method`: HTTP リクエストのメソッドの文字列。デフォルトは `'GET'` です。
- `path`: リクエストのパス。デフォルトは `'/'` です。
  必要なら問い合わせ文字列を含めるべきです．
  例 `'/index.html?page=12'`
- `headers`: リクエストヘッダを含むオブジェクト。
- `auth`: ベーシック認証すなわち Authorization ヘッダのための
  `'user:password'`。
- `agent`: [Agent][] の振る舞いを制御します。
  エージェントが使われる場合、Connection:keep-alive がデフォルトになります。
  可能な値は:
  - `undefined` (デフォルト): ホストとポートで [global Agent][] を使用します。
  - `Agent` オブジェクト: 明示的に渡された `Agent` を使用します。
  - `false`: Agent によるコネクションプーリングを使用しません。
     Connection:close の場合のデフォルトです。

<!--
`http.request()` returns an instance of the `http.ClientRequest`
class. The `ClientRequest` instance is a writable stream. If one needs to
upload a file with a POST request, then write to the `ClientRequest` object.
-->

`http.request()` は `http.ClientRequest` クラスのインスタンスを返します。
`http.ClientRequest` のインスタンスは書き込み可能なストリームです。
もし POST リクエストでファイルのアップロードがしたければ、
`http.ClientRequest` オブジェクトに出力してください。

<!--
Example:
-->

例:

    var options = {
      hostname: 'www.google.com',
      port: 80,
      path: '/upload',
      method: 'POST'
    };

    var req = http.request(options, function(res) {
      console.log('STATUS: ' + res.statusCode);
      console.log('HEADERS: ' + JSON.stringify(res.headers));
      res.setEncoding('utf8');
      res.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
      });
    });

    req.on('error', function(e) {
      console.log('problem with request: ' + e.message);
    });

    // write data to request body
    req.write('data\n');
    req.write('data\n');
    req.end();

<!--
Note that in the example `req.end()` was called. With `http.request()` one
must always call `req.end()` to signify that you're done with the request -
even if there is no data being written to the request body.
-->

この例で `req.end()` が呼ばれていることに注意してください。
`http.request()` では、リクエストが終了したことを示すために、
常に `req.end()` を呼び出さなければなりません
- リクエストのボディに出力するデータがなかったとしても。

<!--
If any error is encountered during the request (be that with DNS resolution,
TCP level errors, or actual HTTP parse errors) an `'error'` event is emitted
on the returned request object.
-->

リクエスト中に何らかのエラー (DNS 解決、TCP レベルのエラー、HTTP パースエラーなど) が発生すると、戻り値のリクエストオブジェクトで `'error'` イベントが生成されます。


<!--
There are a few special headers that should be noted.
-->

いくつかの特別なヘッダに注意が必要です。

<!--
* Sending a 'Connection: keep-alive' will notify Node that the connection to
  the server should be persisted until the next request.

* Sending a 'Content-length' header will disable the default chunked encoding.

* Sending an 'Expect' header will immediately send the request headers.
  Usually, when sending 'Expect: 100-continue', you should both set a timeout
  and listen for the `continue` event. See RFC2616 Section 8.2.3 for more
  information.

* Sending an Authorization header will override using the `auth` option
  to compute basic authentication.
-->

* 'Connection: keep-alive' の送信は、サーバへのコネクションを次のリクエストまで持続することを Node に通知します。

* 'Content-length' ヘッダの送信は、デフォルトのチャンクエンコーディングを無効にします。

* 'Expect' ヘッダの送信は、リクエストヘッダを即時に送信します。
  通常、'Expect: 100-continue' を送信すると、タイムアウトと `continue` イベントを待ち受けます。詳細は RFC2616 の 8.2.3 節を参照してください。

* Authorization ヘッダの送信は、`auth` オプションによるベーシック認証を
  上書きします。

## http.get(options, callback)

<!--
Since most requests are GET requests without bodies, Node provides this
convenience method. The only difference between this method and `http.request()`
is that it sets the method to GET and calls `req.end()` automatically.
-->

ほとんどのリクエストは本文のない GET リクエストであるため、
Node は便利なメソッドを提供します。
このメソッドと `http.request()` の間の違いは、メソッドを GET に設定して
`req.end()` を自動的に呼び出すことだけです。

<!--
Example:
-->

例:

    http.get("http://www.google.com/index.html", function(res) {
      console.log("Got response: " + res.statusCode);
    }).on('error', function(e) {
      console.log("Got error: " + e.message);
    });


## Class: http.Agent

<!--
In node 0.5.3+ there is a new implementation of the HTTP Agent which is used
for pooling sockets used in HTTP client requests.

Previously, a single agent instance helped pool for a single host+port. The
current implementation now holds sockets for any number of hosts.

The current HTTP Agent also defaults client requests to using
Connection:keep-alive. If no pending HTTP requests are waiting on a socket
to become free the socket is closed. This means that node's pool has the
benefit of keep-alive when under load but still does not require developers
to manually close the HTTP clients using keep-alive.

Sockets are removed from the agent's pool when the socket emits either a
"close" event or a special "agentRemove" event. This means that if you intend
to keep one HTTP request open for a long time and don't want it to stay in the
pool you can do something along the lines of:
-->

Node 0.5.3 以降には、HTTP クライアントリクエストのソケットを
プーリングするために新しい HTTP Agent の実装が存在します。

以前は、エージェントの一つのインスタンスが一つのホスト + ポートのプールを
助けていましたが、現在の実装では任意の数のホストに対するソケットを
保持できるようになりました。

現在の HTTP Agent では、クライアントリクエストはデフォルトで
Connection:keep-alive を使うようにもなりました。
ソケットを待ってペンディングになっている HTTP リクエストがなければ、
ソケットはクローズされます。
これは、node のプールは高負荷時に keep-alive のメリットを持ちながら、
keep-alive を使用する HTTP クライアントを開発者が手動でクローズする
必要がないことを意味します。

ソケットは `'close'` イベントまたは特別な `'agentRemove'` イベントが
生成された時にエージェントのプールから削除されます。
これは、一つの HTTP リクエストを長時間オープンしたままにするために、
プールにソケットがとどまらないことを意図するなら、
以下のようにすることができることを意味します:

    http.get(options, function(res) {
      // Do stuff
    }).on("socket", function (socket) {
      socket.emit("agentRemove");
    });

<!--
Alternatively, you could just opt out of pooling entirely using `agent:false`:
-->

別の方法として、 `agent: false` を指定することで、
プーリングを使用しないこともできます:

    http.get({hostname:'localhost', port:80, path:'/', agent:false}, function (res) {
      // Do stuff
    })

### agent.maxSockets

<!--
By default set to 5. Determines how many concurrent sockets the agent can have 
open per host.
-->

デフォルトでは 5 に設定されます。
エージェントがいくつのソケットを並行にオープンするかを決定します。

### agent.sockets

<!--
An object which contains arrays of sockets currently in use by the Agent. Do not 
modify.
-->

エージェントが現在使っているソケットの配列です。
変更しないでください。

### agent.requests

<!--
An object which contains queues of requests that have not yet been assigned to 
sockets. Do not modify.
-->

まだソケットが割り当てられていないリクエストのキューを含むオブジェクトです。
変更しないでください。

## http.globalAgent

<!--
Global instance of Agent which is used as the default for all http client
requests.
-->

全ての HTTP クライアントリクエストで使用される、デフォルトの Agent のインスタンスです。


## Class: http.ClientRequest

<!--
This object is created internally and returned from `http.request()`.  It
represents an _in-progress_ request whose header has already been queued.  The
header is still mutable using the `setHeader(name, value)`, `getHeader(name)`,
`removeHeader(name)` API.  The actual header will be sent along with the first
data chunk or when closing the connection.
-->

このオブジェクトは HTTP サーバ内部で作成され、`http.request()` から返されます。
それはヘッダがキューに入れられた _進行中_ のリクエストを表現します。
ヘッダは `setHeader(name, value)`, `getHeader(name)`, `removeHeader(name)` API によってまだ可変のままです。
実際にヘッダが送信されるのは、最初のデータチャンクが送信される時またはコネクションがクローズされる時です。

<!--
To get the response, add a listener for `'response'` to the request object.
`'response'` will be emitted from the request object when the response
headers have been received.  The `'response'` event is executed with one
argument which is an instance of `http.IncomingMessage`.
-->

レスポンスを取得するには、`'response'` 用のリスナーをリクエストオブジェクトに加えます。
`'response'` イベントはレスポンスヘッダを受信するとリクエストオブジェクトによって生成されます。
`'response'` イベントは `http.IncomingMessage` のインスタンスを唯一の引数として
実行されます。

<!--
During the `'response'` event, one can add listeners to the
response object; particularly to listen for the `'data'` event.
-->

`'response'` イベントの間、レスポンスオブジェクトにリスナーを加えることができます;
とりわけ `'data'` イベントのリスナーです。

<!--
If no `'response'` handler is added, then the response will be
entirely discarded.  However, if you add a `'response'` event handler,
then you **must** consume the data from the response object, either by
calling `response.read()` whenever there is a `'readable'` event, or
by adding a `'data'` handler, or by calling the `.resume()` method.
Until the data is consumed, the `'end'` event will not fire.
-->

`'response'` ハンドラが加えられない場合、レスポンスは完全に捨てられます。
しかし、`'response'` イベントハンドラを加えた場合は、
`'readable'` イベントが発生した時に `response.read()` を呼ぶか、
`'data'` ハンドラを加えるか、`.resume()` メソッドを呼び出すかのいずれかにより、
レスポンスオブジェクトからのデータを消費しなければ *なりません* 。

<!--
This is a `Writable Stream`.
Note: Node does not check whether Content-Length and the length of the body
which has been transmitted are equal or not.

Note: Node does not check whether Content-Length and the length of the body
which has been transmitted are equal or not.
-->

これは `Writable Stream` です。
注意: Node は Content-Length と実際に送信されたリクエストボディの長さが等しいかどうかチェックしません。

`ServerRequest` が `'data'` イベントを生成した時にリスナが存在しなければ、
__データは失われる__ことに注意してください。

<!--
The request implements the [Writable Stream][] interface. This is an
[EventEmitter][] with the following events:
-->

リクエストは [Writable  Stream][] インタフェースを実装します。
これは以下のイベントを持つ [EventEmitter][] です。

### Event 'response'

`function (response) { }`

<!--
Emitted when a response is received to this request. This event is emitted only
once. The `response` argument will be an instance of `http.IncomingMessage`.
-->

このリクエストに対するレスポンスを受信した時に生成されます。
このイベントは一回だけ生成されます。
`response` 引数は `http.IncomingMessage` のインスタンスです。

<!--
Options:
-->

オプション:

<!--
- `host`: A domain name or IP address of the server to issue the request to.
- `port`: Port of remote server.
- `socketPath`: Unix Domain Socket (use one of host:port or socketPath)
-->

- `host`: リクエストを発行するサーバのドメイン名または IP アドレス。
- `port`: リモートサーバのポート。
- `soocketPath`: Unix ドメインソケット (host:port または socketPath のどちらか)

### Event: 'socket'

`function (socket) { }`

<!--
Emitted after a socket is assigned to this request.
-->

このリクエストにソケットが割り当てられた後に生成されます。

### Event: 'connect'

`function (response, socket, head) { }`

<!--
Emitted each time a server responds to a request with a CONNECT method. If this
event isn't being listened for, clients receiving a CONNECT method will have
their connections closed.
-->

サーバが CONNECT メソッドの要求に応答する度に生成されます。
このイベントが監視されていない場合、クライアントが CONNECT メソッドへの
レスポンスを受信すると、そのコネクションはクローズされます。

<!--
A client server pair that show you how to listen for the `connect` event.
-->

どのように `connect` イベントを監視するかを示すクライアントとサーバのペア:


    var http = require('http');
    var net = require('net');
    var url = require('url');

    // Create an HTTP tunneling proxy
    var proxy = http.createServer(function (req, res) {
      res.writeHead(200, {'Content-Type': 'text/plain'});
      res.end('okay');
    });
    proxy.on('connect', function(req, cltSocket, head) {
      // connect to an origin server
      var srvUrl = url.parse('http://' + req.url);
      var srvSocket = net.connect(srvUrl.port, srvUrl.hostname, function() {
        cltSocket.write('HTTP/1.1 200 Connection Established\r\n' +
                        'Proxy-agent: Node-Proxy\r\n' +
                        '\r\n');
        srvSocket.write(head);
        srvSocket.pipe(cltSocket);
        cltSocket.pipe(srvSocket);
      });
    });

    // now that proxy is running
    proxy.listen(1337, '127.0.0.1', function() {

      // make a request to a tunneling proxy
      var options = {
        port: 1337,
        hostname: '127.0.0.1',
        method: 'CONNECT',
        path: 'www.google.com:80'
      };

      var req = http.request(options);
      req.end();

      req.on('connect', function(res, socket, head) {
        console.log('got connected!');

        // make a request over an HTTP tunnel
        socket.write('GET / HTTP/1.1\r\n' +
                     'Host: www.google.com:80\r\n' +
                     'Connection: close\r\n' +
                     '\r\n');
        socket.on('data', function(chunk) {
          console.log(chunk.toString());
        });
        socket.on('end', function() {
          proxy.close();
        });
      });
    });

### Event: 'upgrade'

`function (response, socket, head) { }`

<!--
Emitted each time a server responds to a request with an upgrade. If this
event isn't being listened for, clients receiving an upgrade header will have
their connections closed.
-->

サーバがアップグレード要求に応答する度に生成されます。
このイベントが監視されていない場合、クライアントがアップグレードヘッダを受信するとそのコネクションはクローズされます。

<!--
A client server pair that show you how to listen for the `upgrade` event.
-->

どのように `upgrade` イベントを監視するかを示すクライアントとサーバのペア:

    var http = require('http');

    // Create an HTTP server
    var srv = http.createServer(function (req, res) {
      res.writeHead(200, {'Content-Type': 'text/plain'});
      res.end('okay');
    });
    srv.on('upgrade', function(req, socket, head) {
      socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' +
                   'Upgrade: WebSocket\r\n' +
                   'Connection: Upgrade\r\n' +
                   '\r\n');

      socket.pipe(socket); // echo back
    });

    // now that server is running
    srv.listen(1337, '127.0.0.1', function() {

      // make a request
      var options = {
        port: 1337,
        hostname: '127.0.0.1',
        headers: {
          'Connection': 'Upgrade',
          'Upgrade': 'websocket'
        }
      };

      var req = http.request(options);
      req.end();

      req.on('upgrade', function(res, socket, upgradeHead) {
        console.log('got upgraded!');
        socket.end();
        process.exit(0);
      });
    });


### Event: 'continue'

`function () { }`

<!--
Emitted when the server sends a '100 Continue' HTTP response, usually because
the request contained 'Expect: 100-continue'. This is an instruction that
the client should send the request body.
-->

通常、リクエストが 'Expect: 100-continue' を含んでいたことにより、
サーバが '100 Continue' HTTP レスポンスを送信することで生成されます。
これはクライアントがリクエストボディを送信すべき事を示します。

### request.write(chunk, [encoding])

<!--
Sends a chunk of the body.  By calling this method
many times, the user can stream a request body to a
server--in that case it is suggested to use the
`['Transfer-Encoding', 'chunked']` header line when
creating the request.
-->

ボディのチャンクを送信します。
このメソッドを何回も呼び出すと、サーバへのリクエストボディをストリーム化できます － 
このケースは `['Transfer-Encoding', 'chunked']` ヘッダでリクエストを生成したことを意味します。

<!--
The `chunk` argument should be a [Buffer][] or a string.
-->

`chunk` 引数は [Buffer][] または文字列です。

<!--
The `encoding` argument is optional and only applies when `chunk` is a string.
Defaults to `'utf8'`.
-->

`encoding` 引数はオプションで、`chunk` が文字列の場合だけ適用されます。
デフォルトは `'utf8'` です。


### request.end([data], [encoding])

<!--
Finishes sending the request. If any parts of the body are
unsent, it will flush them to the stream. If the request is
chunked, this will send the terminating `'0\r\n\r\n'`.
-->

リクエストの送信を終了します。
ボディのいくつかの部分がまだ送信されていない場合、それはストリームにフラッシュされます。
リクエストがチャンク化されている場合、これは終端の `'0\r\n\r\n'` を送信します。

<!--
If `data` is specified, it is equivalent to calling
`request.write(data, encoding)` followed by `request.end()`.
-->

`data` が指定された場合は、
`request.write(data, encoding)` に続けて `request.end()` を呼び出すのと等価です。

### request.abort()

<!--
Aborts a request.  (New since v0.3.8.)
-->

リクエストをアボートします (v0.3.8 からの新機能)

### request.setTimeout(timeout, [callback])

<!--
Once a socket is assigned to this request and is connected
[socket.setTimeout()][] will be called.
-->

このリクエストにソケットが割り当てられて接続した際に、
[socket.setTimeout()][] が呼び出されます。

### request.setNoDelay([noDelay])

<!--
Once a socket is assigned to this request and is connected
[socket.setNoDelay()][] will be called.
-->

このリクエストにソケットが割り当てられて接続した際に、
[socket.setNoDelay()][] が呼び出されます。


### request.setSocketKeepAlive([enable], [initialDelay])

<!--
Once a socket is assigned to this request and is connected
[socket.setKeepAlive()][] will be called.
-->

このリクエストにソケットが割り当てられて接続した際に、
[socket.setKeepAlive()][] が呼び出されます。


## http.IncomingMessage

<!--
An `IncomingMessage` object is created by `http.Server` or `http.ClientRequest`
and passed as the first argument to the `'request'` and `'response'` event
respectively. It may be used to access response status, headers and data.
-->

`IncomingMessage` オブジェクトは `http.Server` または `http.ClientRequest()`
によって作成され、`'request'` および `'response'` イベントそれぞれの
最初の引数として渡されます。
それはステータス、ヘッダ、およびデータにアクセスするために使われます。

<!--
It implements the [Readable Stream][] interface, as well as the
following additional events, methods, and properties.
-->

これは [Readable Stream][] インタフェースの実装で、
以下のイベント、メソッド、およびプロパティを追加で持ちます。

### Event: 'close'

`function () { }`

<!--
Indicates that the underlaying connection was terminated before
`response.end()` was called or able to flush.
-->

`response.end()` が呼ばれたりフラッシュ可能になる前に、
下層の接続が切断されたことを示します。

<!--
Just like `'end'`, this event occurs only once per response. See
[http.ServerResponse][]'s `'close'` event for more information.
-->

`'end'` のように、このイベントはレスポンス毎に一回生成されます。
[http.ServerResponse][] の `'close'` イベントにより多くの情報があります。

### message.httpVersion

<!--
In case of server request, the HTTP version sent by the client. In the case of
client response, the HTTP version of the connected-to server.
Probably either `'1.1'` or `'1.0'`.
-->

サーバリクエストの場合、クライアントが送信した HTTP バージョンです。
クライアントレスポンスの場合、接続したサーバの HTTP バージョンです。
いずれの場合も `'1.1'` または `'1.0'` です。

<!--
Also `response.httpVersionMajor` is the first integer and
`response.httpVersionMinor` is the second.
-->

同様に `response.httpVersionMajor` は最初の整数、
`response.httpVersionMinor` は 2 番目の整数です。


### message.headers

<!--
The request/response headers object.
-->

リクエスト／レスポンスヘッダオブジェクトです。

<!--
Read only map of header names and values. Header names are lower-cased.
Example:
-->

ヘッダ名と値のリードオンリーなマップです。ヘッダ名は小文字です。
例:

    // Prints something like:
    //
    // { 'user-agent': 'curl/7.22.0',
    //   host: '127.0.0.1:8000',
    //   accept: '*/*' }
    console.log(request.headers);

### message.trailers

<!--
The request/response trailers object. Only populated after the 'end' event.
-->

リクエスト／レスポンスのトレーラオブジェクトです。
`'end'` イベントの後にだけ発生します。

### message.setTimeout(msecs, callback)

* `msecs` {Number}
* `callback` {Function}

<!--
Calls `message.connection.setTimeout(msecs, callback)`.
-->

`message.connection.setTimeout(msecs, callback)` を呼びます。

### message.method

<!--
**Only valid for request obtained from `http.Server`.**
-->

** `http.Server` から得たリクエストでのみ有効です **

<!--
The request method as a string. Read only. Example:
`'GET'`, `'DELETE'`.
-->

リクエストメソッドを表す文字列です。参照のみ可能です。
例: `'GET'`、`'DELETE'`

### message.url

<!--
**Only valid for request obtained from `http.Server`.**
-->

** `http.Server` から得たリクエストでのみ有効です **

<!--
Request URL string. This contains only the URL that is
present in the actual HTTP request. If the request is:
-->

リクエスト URL を表す文字列です。
これは実際の HTTP リクエストに存在する URL だけを含みます。
もしリクエストが:

    GET /status?name=ryan HTTP/1.1\r\n
    Accept: text/plain\r\n
    \r\n

<!--
Then `request.url` will be:
-->

この場合の `request.url` はこうなります:

    '/status?name=ryan'

<!--
If you would like to parse the URL into its parts, you can use
`require('url').parse(request.url)`.  Example:
-->

URL の要素を解析したい場合は、
`require('url').parse(request.url)` を参照してください。例:

    node> require('url').parse('/status?name=ryan')
    { href: '/status?name=ryan',
      search: '?name=ryan',
      query: 'name=ryan',
      pathname: '/status' }

<!--
If you would like to extract the params from the query string,
you can use the `require('querystring').parse` function, or pass
`true` as the second argument to `require('url').parse`.  Example:
-->

問い合わせ文字列からパラメータを取り出したい場合は、
`require('querystring').parse` 関数を参照するか、
`require('url').parse` の第 2 引数に `true` を渡してください。例:

    node> require('url').parse('/status?name=ryan', true)
    { href: '/status?name=ryan',
      search: '?name=ryan',
      query: { name: 'ryan' },
      pathname: '/status' }

### message.statusCode

<!--
**Only valid for response obtained from `http.ClientRequest`.**
-->

** `http.ClientRequest` から得たレスポンスでのみ有効です **

<!--
The 3-digit HTTP response status code. E.G. `404`.
-->

3 桁の数字によるレスポンスのステータスコードです。例えば `404`。

### message.socket

<!--
The `net.Socket` object associated with the connection.
-->

コネクションに関連づけられた `net.Socket` オブジェクトです。

<!--
With HTTPS support, use request.connection.verifyPeer() and
request.connection.getPeerCertificate() to obtain the client's
authentication details.
-->

HTTPS では `request.connection.verifyPeer()` と
`request.connection.getPeerCertificate()` で
クライアントの認証の詳細を取得できます。

[Agent]: #http_class_http_agent
['checkContinue']: #http_event_checkcontinue
[Buffer]: buffer.html#buffer_buffer
[EventEmitter]: events.html#events_class_events_eventemitter
[global Agent]: #http_http_globalagent
[http.request()]: #http_http_request_options_callback
[http.IncomingMessage]: #http_class_http_incomingmessage
['listening']: net.html#net_event_listening
[net.Server.close()]: net.html#net_server_close_callback
[net.Server.listen(path)]: net.html#net_server_listen_path_callback
[net.Server.listen(port)]: net.html#net_server_listen_port_host_backlog_callback
[Readable Stream]: stream.html#stream_readable_stream
[socket.setKeepAlive()]: net.html#net_socket_setkeepalive_enable_initialdelay
[socket.setNoDelay()]: net.html#net_socket_setnodelay_nodelay
[socket.setTimeout()]: net.html#net_socket_settimeout_timeout_callback
[stream.setEncoding()]: stream.html#stream_stream_setencoding_encoding
[url.parse()]: url.html#url_url_parse_urlstr_parsequerystring_slashesdenotehost
[Writable Stream]: stream.html#stream_writable_stream
