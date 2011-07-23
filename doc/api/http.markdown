## HTTP

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


## http.Server

<!--

This is an `EventEmitter` with the following events:

-->
これは以下のイベントを持つ `EventEmitter` です:

### Event: 'request'

`function (request, response) { }`

<!--

Emitted each time there is request. Note that there may be multiple requests
per connection (in the case of keep-alive connections).

-->
リクエストの度に生成されます。
コネクションごとに複数のリクエストがあるかもしれないことに注意してください
(Keep Alive なコネクションの場合)。

<!--

 `request` is an instance of `http.ServerRequest` and `response` is
 an instance of `http.ServerResponse`

-->
`request` は `http.ServerRequest` のインスタンス、
`response` は `http.ServerResponse` のインスタンスです。

### Event: 'connection'

`function (socket) { }`

<!--

 When a new TCP stream is established. `socket` is an object of type
 `net.Socket`. Usually users will not want to access this event. The
 `stream` can also be accessed at `request.connection`.

-->
新しい TCP ストリームが確立した時。
`socket` は `net.Socket` 型のオブジェクトです。
通常の利用者がこのイベントにアクセスしたくなることはないでしょう。
`socket` は `request.connection` からアクセスすることもできます。

### Event: 'close'

`function (errno) { }`

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

* `request` is the arguments for the http request, as it is in the request event.
* `socket` is the network socket between the server and client.
* `head` is an instance of Buffer, the first packet of the upgraded stream, this may be empty.

-->
* `request` はリクエストイベントと同様に HTTP リクエストへの引数です。
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

`function (exception) { }`

<!--

If a client connection emits an 'error' event - it will forwarded here.

-->
クライアントコネクションが 'error' イベントを発した場合 － ここに転送されます。

### http.createServer([requestListener])

<!--

Returns a new web server object.

-->
新しい Web サーバオブジェクトを返します。

<!--

The `requestListener` is a function which is automatically
added to the `'request'` event.

-->
`requestListener` は自動的に `'request'` イベントに加えられる関数です。

### server.listen(port, [hostname], [callback])

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

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound to the port.

-->
この関数は非同期です。最後の引数の `callback` はサーバがポートをバインドすると呼び出されます。


### server.listen(path, [callback])

<!--

Start a UNIX socket server listening for connections on the given `path`.

-->
`path` で与えられたコネクションを待ち受ける UNIX ドメインソケットのサーバを開始します。

<!--

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound.

-->
この関数は非同期です。最後の引数の `callback` はサーバがバインドすると呼び出されます。


### server.close()

<!--

Stops the server from accepting new connections.

-->
サーバが新しいコネクションを受け付けるのを終了します。


## http.ServerRequest

<!--

This object is created internally by a HTTP server -- not by
the user -- and passed as the first argument to a `'request'` listener.

-->
このオブジェクトは HTTP サーバ内部 － ユーザではなく － で作成され、
`'request'` リスナーの第1引数として渡されます。

<!--

This is an `EventEmitter` with the following events:

-->
これは以下のイベントを持つ `EventEmitter` です:

### Event: 'data'

`function (chunk) { }`

<!--

Emitted when a piece of the message body is received.

-->
メッセージボディの断片を受信した場合に生成されます。

<!--

Example: A chunk of the body is given as the single
argument. The transfer-encoding has been decoded.  The
body chunk is a string.  The body encoding is set with
`request.setEncoding()`.

-->
例: 一つの引数としてボディのチャンクが与えられます。
転送エンコーディングでデコードされます。
ボディのチャンクは文字列です。
ボディのエンコーディングは `request.setBodyEncoding()` で設定されます。

### Event: 'end'

`function () { }`

<!--

Emitted exactly once for each request. After that, no more `'data'` events
will be emitted on the request.

-->
リクエストごとに厳密に一回生成されます。
その後、このリクエストで `'data'` イベントが生成されることはありません。

### Event: 'close'

`function (err) { }`

<!--

Indicates that the underlaying connection was terminated before
`response.end()` was called or able to flush.

-->
`response.end()` が呼び出されたり、フラッシュされる前に下層の接続が
切断されたことを示します。

<!--

The `err` parameter is always present and indicates the reason for the timeout:

-->
`err` パラメータは常に与えられ、クローズの理由を示します。

<!--

`err.code === 'timeout'` indicates that the underlaying connection timed out.
This may happen because all incoming connections have a default timeout of 2
minutes.

-->
`err.code === 'timeout'` は下層のコネクションがタイムアウトしたことを示します。
これは、全ての着信側の接続はデフォルト 2 分でタイムアウトするために発生します。

<!--

`err.code === 'aborted'` means that the client has closed the underlaying
connection prematurely.

-->
`err.code === 'aborted'` はクライアントが仮想の接続をいち早く切断したことを
意味します。

<!--

Just like `'end'`, this event occurs only once per request, and no more `'data'`
events will fire afterwards.

-->
`'end'` と同様、このイベントはリクエスト上で一度だけ発生し、その後ではもう
`'data'` イベントが発生することはありません。

<!--

Note: `'close'` can fire after `'end'`, but not vice versa.

-->
注意: `'close'` は `'end'` の後で発生することがあります。
その逆もあります。

### request.method

<!--

The request method as a string. Read only. Example:
`'GET'`, `'DELETE'`.

-->
リクエストメソッドを表す文字列です。参照のみ可能です。
例: `'GET'`、`'DELETE'`


### request.url

<!--

Request URL string. This contains only the URL that is
present in the actual HTTP request. If the request is:

-->
リクエスト URL を表す文字列です。
これは実際の HTTP リクエストに存在する URL だけを含みます。
リクエストがこうなら:

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



### request.headers

<!--

Read only.

-->
参照のみ可能です。

### request.trailers

<!--

Read only; HTTP trailers (if present). Only populated after the 'end' event.

-->
参照のみ可能です; HTTP のトレーラです (もしあれば)。'end' イベントの後にだけ発生します。

### request.httpVersion

<!--

The HTTP protocol version as a string. Read only. Examples:
`'1.1'`, `'1.0'`.
Also `request.httpVersionMajor` is the first integer and
`request.httpVersionMinor` is the second.

-->
HTTP プロトコルのバージョンを表す文字列です。参照のみ可能です。例:
`'1.1'`、`'1.0'`。
同様に `request.httpVersionMajor` は最初の整数、
`request.httpVersionMinor` は 2 番目の整数です。


### request.setEncoding(encoding=null)

<!--

Set the encoding for the request body. Either `'utf8'` or `'binary'`. Defaults
to `null`, which means that the `'data'` event will emit a `Buffer` object..

-->
リクエストボディのエンコーディングを設定します。
`'utf8'` または `'binary'` のいずれかです。
デフォルトは `null` で、`'data'` イベントが
`Buffer` を生成することを意味します。


### request.pause()

<!--

Pauses request from emitting events.  Useful to throttle back an upload.

-->
リクエストによるイベントの生成を中断します。アップロード速度を落とすのに便利です。


### request.resume()

<!--

Resumes a paused request.

-->
中断されたリクエストを再開します。

### request.connection

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



## http.ServerResponse

<!--

This object is created internally by a HTTP server--not by the user. It is
passed as the second parameter to the `'request'` event. It is a `Writable Stream`.

-->
このオブジェクトは HTTP サーバ内部 － ユーザではなく － で作成されます。
`'request'` リスナーの第 2 引数として渡されます。
これは `Writable Stream` です。

### response.writeContinue()

<!--

Sends a HTTP/1.1 100 Continue message to the client, indicating that
the request body should be sent. See the [checkContinue](#event_checkContinue_) event on
`Server`.

-->
HTTP/1.1 の 100 Continue メッセージをクライアントに送信し、
リクエストボディを送信してもよいことを示します。
`Server`の [checkContinue](#event_checkContinue_) イベントを参照してください。

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

-->
注意: `Content-Length` は文字数ではなくバイト数で与えられます。
上の例が動作するのは `'hello world'` という文字列が単一バイト文字だけを含むためです。
もしボディがより上位にコード化された文字を含む場合は、
指定したエンコーディングによるバイト数を得るために `Buffer.byteLength()` を使うべきです。

### response.statusCode

<!--

When using implicit headers (not calling `response.writeHead()` explicitly), this property
controls the status code that will be send to the client when the headers get
flushed.

-->
(`response.writeHead()` が明示的に呼ばれないために) 暗黙的なヘッダが使われる場合、このプロパティはヘッダがフラッシュされる時にクライアントへ送信されるステータスコードを制御します。

<!--

Example:

-->
例:

    response.statusCode = 404;

### response.setHeader(name, value)

<!--

Sets a single header value for implicit headers.  If this header already exists
in the to-be-sent headers, it's value will be replaced.  Use an array of strings
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

or

    response.setHeader("Set-Cookie", ["type=ninja", "language=javascript"]);


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


### response.write(chunk, encoding='utf8')

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
                              'Trailer': 'TraceInfo' });
    response.write(fileData);
    response.addTrailers({'Content-MD5': "7895bf4b8828b55ceaf47747b4bca667"});
    response.end();


### response.end([data], [encoding])

<!--

This method signals to the server that all of the response headers and body
has been sent; that server should consider this message complete.
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
Node は HTTP リクエストを行うために、サーバごとにいくつかのコネクションを保持します。
この関数はその一つを使って透過的にリクエストを発行できるようにします。

<!--

Options:

-->
オプション:

<!--

- `host`: A domain name or IP address of the server to issue the request to.
- `port`: Port of remote server.
- `socketPath`: Unix Domain Socket (use one of host:port or socketPath)
- `method`: A string specifying the HTTP request method. Possible values:
  `'GET'` (default), `'POST'`, `'PUT'`, and `'DELETE'`.
- `path`: Request path. Should include query string and fragments if any.
   E.G. `'/index.html?page=12'`
- `headers`: An object containing request headers.
- `agent`: Controls `Agent` behavior. Possible values:
 - `undefined` (default): use default `Agent` for this host and port.
 - `Agent` object: explicitly use the passed in `Agent`.
 - `false`: explicitly generate a new `Agent` for this host and port. `Agent` will not be re-used.

-->
- `host`: リクエストを発行するサーバのドメイン名または IP アドレス。
- `port`: リモートサーバのポート。
- `socketPath`: Unix ドメインソケット (host:port または socketPath のどちらか)
- `method`: HTTP リクエストのメソッドを指定する文字列。 可能な値:
  `'GET'` (デフォルト), `'POST'`, `'PUT'`, そして `'DELETE'`。
- `path`: リクエストのパス。問い合わせ文字列やフラグメントがあるなら含めるべきです。
   例. `'/index.html?page=12'`
- `headers`: リクエストヘッダを含むオブジェクト。
- `agent`: `Agent` の振る舞いを制御します。可能な値は:
 - `undefined` (デフォルト): ホストとポートからデフォルトの `Agent` 
を使用します。
 - `Agent`: オブジェクト: 明示的に渡された `Agent` を使用します。
 - `false`: このホストとポートの新しい `Agent` を新たに作成します。
 `Agent` は再利用されません。

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
      host: 'www.google.com',
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

-->
* 'Connection: keep-alive' の送信は、サーバへのコネクションを次のリクエストまで持続することを Node に通知します。

* 'Content-length' ヘッダの送信は、デフォルトのチャンクエンコーディングを無効にします。

* 'Expect' ヘッダの送信は、リクエストヘッダを即時に送信します。
  通常、'Expect: 100-continue' を送信すると、タイムアウトと `continue` イベントを待ち受けます。詳細は RFC2616 の 8.2.3 節を参照してください。

## http.get(options, callback)

<!--

Since most requests are GET requests without bodies, Node provides this
convenience method. The only difference between this method and `http.request()` is
that it sets the method to GET and calls `req.end()` automatically.

-->
ほとんどのリクエストは本文のない GET リクエストであるため、
Node は便利なメソッドを提供します。
このメソッドと `http.request()` の間の違いは、メソッドを GET に設定して `req.end()` を自動的に呼び出すことだけです。

<!--

Example:

-->
例:

    var options = {
      host: 'www.google.com',
      port: 80,
      path: '/index.html'
    };

    http.get(options, function(res) {
      console.log("Got response: " + res.statusCode);
    }).on('error', function(e) {
      console.log("Got error: " + e.message);
    });


## http.Agent
## http.getAgent(options)

<!--

`http.request()` uses a special `Agent` for managing multiple connections to
an HTTP server. Normally `Agent` instances should not be exposed to user
code, however in certain situations it's useful to check the status of the
agent. The `http.getAgent()` function allows you to access the agents.

-->
`http.request()` は HTTP サーバへの複数のコネクションを管理する特別な `Agent` を使用します。
通常 `Agent` インスタンスはユーザコードに出てきませんが、特定の状況ではエージェントの状態をチェックすることが役に立ちます。
`http.getAgent()` 関数はエージェントへのアクセスを可能にします。

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

A client server pair that show you how to listen for the `upgrade` event using `http.getAgent`:
-->
`http.getAget` を使ってどのように `upgrade` イベントを監視するかを示す、
クライアントとサーバのペア:

    var http = require('http');
    var net = require('net');

    // Create an HTTP server
    var srv = http.createServer(function (req, res) {
      res.writeHead(200, {'Content-Type': 'text/plain'});
      res.end('okay');
    });
    srv.on('upgrade', function(req, socket, upgradeHead) {
      socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' +
                   'Upgrade: WebSocket\r\n' +
                   'Connection: Upgrade\r\n' +
                   '\r\n\r\n');

      socket.ondata = function(data, start, end) {
        socket.write(data.toString('utf8', start, end), 'utf8'); // echo back
      };
    });

    // now that server is running
    srv.listen(1337, '127.0.0.1', function() {

      // make a request
      var agent = http.getAgent('127.0.0.1', 1337);

      var options = {
        agent: agent,
        port: 1337,
        host: '127.0.0.1',
        headers: {
          'Connection': 'Upgrade',
          'Upgrade': 'websocket'
        }
      };

      var req = http.request(options);
      req.end();

      agent.on('upgrade', function(res, socket, upgradeHead) {
        console.log('got upgraded!');
        socket.end();
        process.exit(0);
      });
    });

### agent.maxSockets

<!--

By default set to 5. Determines how many concurrent sockets the agent can have open.

-->
デフォルトでは 5 に設定されます。
エージェントがいくつのソケットを並行にオープンするかを決定します。

### agent.sockets

<!--

An array of sockets currently in use by the Agent. Do not modify.

-->
エージェントが現在使っているソケットの配列です。
変更しないでください。

### agent.queue

<!--

A queue of requests waiting to be sent to sockets.

-->
ソケットへの送信を待機しているリクエストのキューです。

## http.ClientRequest

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
argument which is an instance of `http.ClientResponse`.

-->
レスポンスを取得するには、`'response'` 用のリスナーをリクエストオブジェクトに加えます。
`'response'` イベントはレスポンスヘッダを受信するとリクエストオブジェクトによって生成されます。
`'response'` イベントは `http.ClientResponse` のインスタンスを唯一の引数として実行されます。

<!--

During the `'response'` event, one can add listeners to the
response object; particularly to listen for the `'data'` event. Note that
the `'response'` event is called before any part of the response body is received,
so there is no need to worry about racing to catch the first part of the
body. As long as a listener for `'data'` is added during the `'response'`
event, the entire body will be caught.

-->
`'response'` イベントの間、レスポンスオブジェクトにリスナーを加えることができます;
とりわけ `'data'` イベントのリスナーです。
`'response'` イベントはレスポンスボディのどの部分を受信するよりも前に呼び出されることに注意してください。
そのため、ボディの最初の部分の受信と競合することを心配する必要はありません。
`'response'` イベントの間に `'data'` イベントのリスナーが加えられる限り、
ボディ全体を受信することができます。


    // Good
    request.on('response', function (response) {
      response.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
      });
    });

    // Bad - misses all or part of the body
    request.on('response', function (response) {
      setTimeout(function () {
        response.on('data', function (chunk) {
          console.log('BODY: ' + chunk);
        });
      }, 10);
    });

<!--

This is a `Writable Stream`.

-->
これは `Writable Stream` です。

<!--

This is an `EventEmitter` with the following events:

-->
これは以下のイベントを持つ `EventEmitter` です。

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

### Event 'response'

`function (response) { }`

<!--

Emitted when a response is received to this request. This event is emitted only once. The
`response` argument will be an instance of `http.ClientResponse`.

-->
このリクエストに対するレスポンスを受信した時に生成されます。
このイベントは一回だけ生成されます。
`response` 引数は `http.ClientResponse` のインスタンスです。


### request.write(chunk, encoding='utf8')

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

The `chunk` argument should be an array of integers
or a string.

-->
`chunk` 引数は整数の配列か文字列になります。

<!--

The `encoding` argument is optional and only
applies when `chunk` is a string.

-->
`encoding` 引数はオプションで、`chunk` が文字列の場合だけ適用されます。


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

If `data` is specified, it is equivalent to calling `request.write(data, encoding)`
followed by `request.end()`.

-->
`data` が指定された場合は、
`request.write(data, encoding)` に続けて `request.end()` を呼び出すのと等価です。

### request.abort()

<!--

Aborts a request.  (New since v0.3.8.)

-->
リクエストをアボートします (v0.3.8 からの新機能)

## http.ClientResponse

<!--

This object is created when making a request with `http.request()`. It is
passed to the `'response'` event of the request object.

-->
このオブジェクトは `http.request()` によってリクエストと一緒に作成されます。
これはリクエストオブジェクトの `'response'` イベントに渡されます。

<!--

The response implements the `Readable Stream` interface.

-->
レスポンスは `Readable Stream` インタフェースを実装します。

### Event: 'data'

`function (chunk) { }`

<!--

Emitted when a piece of the message body is received.

-->
メッセージボディの断片を受信した場合に生成されます。


### Event: 'end'

`function () { }`

<!--

Emitted exactly once for each message. No arguments. After
emitted no other events will be emitted on the response.

-->
メッセージごとに厳密に一回だけ生成されます。
このイベントが生成された後、このレスポンスはどんなイベントも生成しません。

### Event: 'close'

`function (err) { }`

<!--

Indicates that the underlaying connection was terminated before
`end` event was emitted.
See [http.ServerRequest](#http.ServerRequest)'s `'close'` event for more
information.

-->
`'end'` イベントが生成される前に下層の接続が切断されたことを示します。
[http.ServerRequest](#http.ServerRequest) の `'close'`
イベントにより多くの情報があります。

### response.statusCode

<!--

The 3-digit HTTP response status code. E.G. `404`.

-->
3 桁の数字によるレスポンスのステータスコードです。例えば `404`。

### response.httpVersion

<!--

The HTTP version of the connected-to server. Probably either
`'1.1'` or `'1.0'`.
Also `response.httpVersionMajor` is the first integer and
`response.httpVersionMinor` is the second.

-->
接続しているサーバとの HTTP のバージョンです。
おそらく `'1.1'` または `'1.0'` のどちらかです。
同様に `response.httpVersionMajor` は最初の整数、
`response.httpVersionMinor` は 2 番目の整数です。

### response.headers

<!--

The response headers object.

-->
レスポンスヘッダオブジェクトです。

### response.trailers

<!--

The response trailers object. Only populated after the 'end' event.

-->
レスポンスのトレーラオブジェクトです。
'end' イベントの後にだけ発生します。

### response.setEncoding(encoding=null)

<!--

Set the encoding for the response body. Either `'utf8'`, `'ascii'`, or `'base64'`.
Defaults to `null`, which means that the `'data'` event will emit a `Buffer` object..

-->
レスポンスボディのエンコーディングを設定します。
`'utf8'`、`'ascii'`、あるいは `'base64'` のいずれかです。
デフォルトは `null` で、
`'data'` イベントが `Buffer` を生成することを意味します。

### response.pause()

<!--

Pauses response from emitting events.  Useful to throttle back a download.

-->
イベントの生成によるレスポンスを中断します。ダウンロード速度を落とすのに便利です。

### response.resume()

<!--

Resumes a paused response.

-->
中断されていたレスポンスを再開します。
