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

<!--
HTTPS is supported if OpenSSL is available on the underlying platform.
-->
プラットフォームで OpenSSL が利用可能であれば HTTPS がサポートされます。

## http.Server

<!--
This is an `EventEmitter` with the following events:
-->
これは以下のイベントを持つ `EventEmitter` です:

### Event: 'request'

`function (request, response) { }`

<!--
 `request` is an instance of `http.ServerRequest` and `response` is
 an instance of `http.ServerResponse`
-->
`request` は `http.ServerRequest` のインスタンス、
`response` は `http.ServerResponse` のインスタンスです。

### Event: 'connection'

`function (stream) { }`

<!--
 When a new TCP stream is established. `stream` is an object of type
 `net.Stream`. Usually users will not want to access this event. The
 `stream` can also be accessed at `request.connection`.
-->
新しい TCP ストリームが確立した時。
`stream` は `net.Stream` 型のオブジェクトです。
通常の利用者がこのイベントにアクセスしたくなることはないでしょう。
`stream` は `request.connection` からアクセスすることもできます。

### Event: 'close'

`function (errno) { }`

<!--
 Emitted when the server closes.
-->
サーバがクローズした時に生成されます。

### Event: 'request'

`function (request, response) {}`

<!--
Emitted each time there is request. Note that there may be multiple requests
per connection (in the case of keep-alive connections).
-->
リクエストの度に生成されます。コネクションごとに複数のリクエストがあることに注意してください
(Keep Alive なコネクションの場合)。

### Event: 'checkContinue'

`function (request, response) {}`

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

`function (request, socket, head)`

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

`function (exception) {}`

<!--
If a client connection emits an 'error' event - it will forwarded here.
-->
クライアントコネクションが 'error' イベントを発した場合 － ここに転送されます。

### http.createServer(requestListener)

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
`request.setBodyEncoding()`.
-->
例: 一つの引数としてボディのチャンクが与えられます。
転送エンコーディングでデコードされます。
ボディのチャンクは文字列です。
ボディのエンコーディングは `request.setBodyEncoding()` で設定されます。

### Event: 'end'

`function () { }`

<!--
Emitted exactly once for each message. No arguments.  After
emitted no other events will be emitted on the request.
-->
メッセージごとに厳密に一回生成されます。引数はありません。
このイベントが生成された後、このリクエストで生成されるイベントはありません。


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
The `net.Stream` object associated with the connection.
-->
コネクションに関連づけられた `net.Stream` オブジェクトです。


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
the request body should be sent. See the the `checkContinue` event on
`Server`.
-->
HTTP/1.1 の 100 Continue メッセージをクライアントに送信し、
リクエストボディを送信してもよいことを示します。
`Server`の`checkContinue`イベントを参照してください。

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
-->
このメソッドはメッセージごとに 1 回だけ呼び出されなくてはならず、
`response.end()` の前に呼び出されなければなりません。

### response.write(chunk, encoding='utf8')

<!--
This method must be called after `writeHead` was
called. It sends a chunk of the response body. This method may
be called multiple times to provide successive parts of the body.
-->
このメソッドは `writeHead` の後に呼び出されなければなりません。
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


## http.Client

<!--
An HTTP client is constructed with a server address as its
argument, the returned handle is then used to issue one or more
requests.  Depending on the server connected to, the client might
pipeline the requests or reestablish the stream after each
stream. _Currently the implementation does not pipeline requests._
-->
HTTP クライアントは引数として渡されるサーバアドレスによって構築され、
戻り値のハンドルは一つまたはそれ以上のリクエストを発行するのに使われます。
接続されたサーバに応じて、クライアントはパイプライン化されたリクエストまたは、
それぞれのストリームの後でストリームを再確立するかもしれません。
_現在の実装はリクエストをパイプライン化しません。_

<!--
Example of connecting to `google.com`:
-->
`google.com` に接続する例:

    var http = require('http');
    var google = http.createClient(80, 'www.google.com');
    var request = google.request('GET', '/',
      {'host': 'www.google.com'});
    request.end();
    request.on('response', function (response) {
      console.log('STATUS: ' + response.statusCode);
      console.log('HEADERS: ' + JSON.stringify(response.headers));
      response.setEncoding('utf8');
      response.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
      });
    });

<!--
There are a few special headers that should be noted.
-->
少数の特別なヘッダがあることに注意してください。

<!--
* The 'Host' header is not added by Node, and is usually required by
  website.

* Sending a 'Connection: keep-alive' will notify Node that the connection to
  the server should be persisted until the next request.

* Sending a 'Content-length' header will disable the default chunked encoding.

* Sending an 'Expect' header will immediately send the request headers.
  Usually, when sending 'Expect: 100-continue', you should both set a timeout
  and listen for the `continue` event. See RFC2616 Section 8.2.3 for more
  information.
-->
* 'Host' ヘッダは Node によって加えられませんが、通常の Web サイトに必要とされます。

* 'Connection: keep-alive' を送信することで、サーバへの接続を次のリクエストまで維持することを Node に通知します。

* 'Content-length' ヘッダを送信することで、デフォルトのチャンクエンコーディングが無効になります。

* 'Expect' ヘッダを送信することで、リクエストヘッダを直ちに送信します。
通常、'Expect: 100-continue' を送信する場合、タイムアウトと `continue` イベントの両方を設定します。
より詳しくは RFC2616 の 8.2.3 節を参照してください。


### Event: 'upgrade'

`function (request, socket, head)`

<!--
Emitted each time a server responds to a request with an upgrade. If this event
isn't being listened for, clients receiving an upgrade header will have their
connections closed.
-->
サーバがアップグレード要求に応答する度に生成されます。
このイベントが監視されていない場合、クライアントがアップグレードヘッダを受信するとそのコネクションはクローズされます。

<!--
See the description of the `upgrade` event for `http.Server` for further details.
-->
より詳しくは `http.Server` の `upgrade` イベントの説明を参照してください。

### Event: 'continue'

`function ()`

<!--
Emitted when the server sends a '100 Continue' HTTP response, usually because
the request contained 'Expect: 100-continue'. This is an instruction that
the client should send the request body.
-->
通常、リクエストが 'Expect: 100-continue' を含んでいたことにより、
サーバが '100 Continue' HTTP レスポンスを送信することで生成されます。
これはクライアントがリクエストボディを送信すべき事を示します。


### http.createClient(port, host='localhost', secure=false, [credentials])

<!--
Constructs a new HTTP client. `port` and
`host` refer to the server to be connected to. A
stream is not established until a request is issued.
-->
新しい HTTP クライアントを構築します。
`port` と `host` は接続先となるサーバを参照します。
リクエストが発行されるまでストリームは確立されません。

<!--
`secure` is an optional boolean flag to enable https support and `credentials` is an optional
credentials object from the crypto module, which may hold the client's private key,
certificate, and a list of trusted CA certificates.
-->
オプションの`secure` は boolean のフラグで HTTPS サポートを有効にし、
オプションの`credentials` は暗号モジュールの認証情報オブジェクトで、
クライアントの秘密鍵、証明書、そして信頼できる認証局の証明書のリストを含むことができます。

<!--
If the connection is secure, but no explicit CA certificates are passed
in the credentials, then node.js will default to the publicly trusted list
of CA certificates, as given in <http://mxr.mozilla.org/mozilla/source/security/nss/lib/ckfw/builtins/certdata.txt>.
-->
コネクションがセキュアな場合、証明情報で認証局の証明書が明示的に渡されないと、
node.js はデフォルトの信頼できる認証局のリストとして
http://mxr.mozilla.org/mozilla/source/security/nss/lib/ckfw/builtins/certdata.txt を与えます。

### client.request(method='GET', path, [request_headers])

<!--
Issues a request; if necessary establishes stream. Returns a `http.ClientRequest` instance.
-->
リクエストを発行します; 必要であればストリームを確立します。
`http.ClientRequest` のインスタンスを返します。

<!--
`method` is optional and defaults to 'GET' if omitted.
-->
`method` はオプションで、省略された場合のデフォルトは 'GET'です。

<!--
`request_headers` is optional.
Additional request headers might be added internally
by Node. Returns a `ClientRequest` object.
-->
`request_headers` はオプションです。
Node 内部で付加的なリクエストヘッダが加えられることがあります。
`ClientRequest` オブジェクトを返します。

<!--
Do remember to include the `Content-Length` header if you
plan on sending a body. If you plan on streaming the body, perhaps
set `Transfer-Encoding: chunked`.
-->
ボディを送信しようとしている場合は、`Content-Length` ヘッダを含めることを忘れないでください。
ボディをストリーム化する場合は、おそらく `Transfer-Encoding: chunked` をセットしてください。

<!--
*NOTE*: the request is not complete. This method only sends the header of
the request. One needs to call `request.end()` to finalize the request and
retrieve the response.  (This sounds convoluted but it provides a chance for
the user to stream a body to the server with `request.write()`.)
-->
*注意*: リクエストは完了していません。このメソッドはリクエストのヘッダを送信するだけです。
リクエストを完了してレスポンスを読み出すには `request.end()` を呼ぶ必要があります。
(複雑に感じるかもしれませんが、
これは `request.write()` でボディをストリーム化するチャンスをユーザに提供します))。

### client.verifyPeer()

<!--
Returns true or false depending on the validity of the server's certificate
in the context of the defined or default list of trusted CA certificates.
-->
指定された、あるいはデフォルトの信頼された認証局の証明書において、
サーバの証明書の妥当性に応じて true または false を返します。

### client.getPeerCertificate()

<!--
Returns a JSON structure detailing the server's certificate, containing a dictionary
with keys for the certificate `'subject'`, `'issuer'`, `'valid_from'` and `'valid_to'`.
-->
サーバ証明書の詳細を、'subject'、'issuer'、'valid_from' そして
'valid_to' をキーとする証明書の辞書を含む JSON 形式で返します。


## http.ClientRequest

<!--
This object is created internally and returned from the `request()` method
of a `http.Client`. It represents an _in-progress_ request whose header has
already been sent.
-->
このオブジェクトは HTTP サーバ内部で作成され、`http.Client` の
`request()` メソッドから返されます。
それはヘッダが送信された _進行中_ のリクエストを表現します。

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


## http.ClientResponse

<!--
This object is created when making a request with `http.Client`. It is
passed to the `'response'` event of the request object.
-->
このオブジェクトは `http.Client` によってリクエストと一緒に作成されます。
これはリクエストオブジェクトの `'response'` イベントに渡されます。

<!--
The response implements the `Readable Stream` interface.
-->
レスポンスは `Readable Stream` インタフェースを実装します。

### Event: 'data'

`function (chunk) {}`

<!--
Emitted when a piece of the message body is received.
-->
メッセージボディの断片を受信した場合に生成されます。

<!--
    Example: A chunk of the body is given as the single
    argument. The transfer-encoding has been decoded.  The
    body chunk a String.  The body encoding is set with
    `response.setBodyEncoding()`.
-->
    例: ボディのチャンクは一つの引数として与えられます。
    転送エンコーディングでデコードされます。
    ボディのチャンクは文字列です。
    ボディエンコーディングは `response.setBodyEncoding()` によって設定されます。

### Event: 'end'

`function () {}`

<!--
Emitted exactly once for each message. No arguments. After
emitted no other events will be emitted on the response.
-->
メッセージごとに厳密に一回だけ生成されます。
このイベントが生成された後、このレスポンスはどんなイベントも生成しません。

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

### response.client

<!--
A reference to the `http.Client` that this response belongs to.
-->
このレスポンスを所有する `http.Client` への参照です。
