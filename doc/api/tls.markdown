## TLS (SSL)

<!--

Use `require('tls')` to access this module.

-->
`require('tls')` でこのモジュールにアクセスします。

<!--

The `tls` module uses OpenSSL to provide Transport Layer Security and/or
Secure Socket Layer: encrypted stream communication.

-->
`tls` モジュールは OpenSSL を使用することで Transport Layer Security および
Secure Socket Layer: 暗号化されたストリーム通信を提供します。

<!--

TLS/SSL is a public/private key infrastructure. Each client and each
server must have a private key. A private key is created like this

-->
TLS/SSL は公開／秘密鍵を基礎とします。
どのクライアントとサーバも秘密鍵が必要です。
秘密鍵は次のように作成します

    openssl genrsa -out ryans-key.pem 1024

<!--

All severs and some clients need to have a certificate. Certificates are public
keys signed by a Certificate Authority or self-signed. The first step to
getting a certificate is to create a "Certificate Signing Request" (CSR)
file. This is done with:

-->
全てのサーバと一部のクライアントは証明書を必要とします。
証明書は認証局の公開鍵または自身によって署名されます。
証明書を作成する最初のステップは「証明書署名要求 (CSR)」ファイルです。
次のようにします:

    openssl req -new -key ryans-key.pem -out ryans-csr.pem

<!--

To create a self-signed certificate with the CSR, do this:

-->
CSR から自己署名証明書を作成するには次のようにします:

    openssl x509 -req -in ryans-csr.pem -signkey ryans-key.pem -out ryans-cert.pem

<!--

Alternatively you can send the CSR to a Certificate Authority for signing.

-->
他に CSR を認証局に送って署名してもらうこともできます。

<!--

(TODO: docs on creating a CA, for now interested users should just look at
`test/fixtures/keys/Makefile` in the Node source code)

-->
(TODO: CA を作るドキュメント、現在は興味あるユーザは Node のソースコードから
`test/fixtures/keys/Makefile` を見る必要がある)


### s = tls.connect(port, [host], [options], callback)

<!--

Creates a new client connection to the given `port` and `host`. (If `host`
defaults to `localhost`.) `options` should be an object which specifies

-->
与えられた `port` と `host` で新しいクライアントコネクションを作成します
(`host` のデフォルトは `localhost` です)。
`options` は以下を指定したオブジェクトです。


<!--

  - `key`: A string or `Buffer` containing the private key of the server in
    PEM format. (Required)

  - `cert`: A string or `Buffer` containing the certificate key of the server in
    PEM format.

  - `ca`: An array of strings or `Buffer`s of trusted certificates. If this is
    omitted several well known "root" CAs will be used, like VeriSign.
    These are used to authorize connections.

-->
  - `key`: PEM フォーマットによるサーバの秘密鍵を持つ文字列または `Buffer` です (必須)。

  - `cert`: PEM フォーマットによる証明書の鍵を持つ文字列または `Buffer` です。

  - `ca`: 信頼できる証明書の文字列または `Buffer` の配列です。
    省略された場合、ベリサインなどのよく知られた「ルート」認証局が使われます。
    これらはコネクションの認証に使われます。

<!--

`tls.connect()` returns a cleartext `CryptoStream` object.

-->
`tls.connect()` は平文の `CryptoStream` オブジェクトを返します。

<!--

After the TLS/SSL handshake the `callback` is called. The `callback` will be
called no matter if the server's certificate was authorized or not. It is up
to the user to test `s.authorized` to see if the server certificate was
signed by one of the specified CAs. If `s.authorized === false` then the error
can be found in `s.authorizationError`.

-->
TLS/SSL ハンドシェークの後で `callback` が呼び出されます。
`callback` は証明書がサーバに認証されたかどうかに関わらず呼び出されます。
サーバ証明書が指定した認証局に承認されたかチェックするために
`s.authorized` を確認するかはユーザ次第です。
`s.authorized === false`の場合、`s.authorizationError` からエラーを見つけることができます。

### tls.Server

<!--

This class is a subclass of `net.Server` and has the same methods on it.
Instead of accepting just raw TCP connections, this accepts encrypted
connections using TLS or SSL.

-->
このクラスは `net.Server` のサブクラスで、同じメソッドを持っています。
生の TCP コネクションを受け入れる代わりに、
TLS または SSL を使った暗号化されたコネクションを受け付けます。

<!--

Here is a simple example echo server:

-->
これはシンプルなエコーサーバの例です。

    var tls = require('tls');
    var fs = require('fs');

    var options = {
      key: fs.readFileSync('server-key.pem'),
      cert: fs.readFileSync('server-cert.pem')
    };

    tls.createServer(options, function (s) {
      s.write("welcome!\n");
      s.pipe(s);
    }).listen(8000);


<!--

You can test this server by connecting to it with `openssl s_client`:

-->
このサーバをテストするために `openssl s_client` で接続することができます。


    openssl s_client -connect 127.0.0.1:8000


#### tls.createServer(options, secureConnectionListener)

<!--

This is a constructor for the `tls.Server` class. The options object
has these possibilities:

-->
`tls.Server` クラスのコンストラクタです。
オプションのオブジェクトは以下を持つことができます。

<!--

  - `key`: A string or `Buffer` containing the private key of the server in
    PEM format. (Required)

  - `cert`: A string or `Buffer` containing the certificate key of the server in
    PEM format. (Required)

  - `ca`: An array of strings or `Buffer`s of trusted certificates. If this is
    omitted several well known "root" CAs will be used, like VeriSign.
    These are used to authorize connections.

  - `requestCert`: If `true` the server will request a certificate from
    clients that connect and attempt to verify that certificate. Default:
    `false`.

  - `rejectUnauthorized`: If `true` the server will reject any connection
    which is not authorized with the list of supplied CAs. This option only
    has an effect if `requestCert` is `true`. Default: `false`.

-->
  - `key`: PEM フォーマットによるサーバの秘密鍵を持つ文字列または `Buffer` です (必須)。

  - `cert`: PEM フォーマットによる証明書の鍵を持つ文字列または `Buffer` です (必須)。

  - `ca`: 信頼できる証明書の文字列または `Buffer` の配列です。
    省略された場合、ベリサインなどのよく知られた「ルート」認証局が使われます。
    これらはコネクションの認証に使われます。

  - `requestCert`: `true` の場合、サーバは接続しようとするクライアントからの証明書を要求します
    デフォルトは `false` です。

  - `rejectUnauthorized`: `true` の場合、
    サーバは提供された認証局のリストによって認証されていないコネクションを破棄します．
    このオプションは `requestCert` が `true` の場合だけ効果があります。
    デフォルトは `false` です。


#### Event: 'secureConnection'

`function (cleartextStream) {}`

<!--

This event is emitted after a new connection has been successfully
handshaked. The argument is a duplex instance of `stream.Stream`. It has all
the common stream methods and events.

-->
このイベントは、新しい接続のハンドシェークが成功した場合に生成されます。
引数は `stream.Stream` のインスタンスです。
これはストリームに共通する全てのメソッドとイベントを持っています。

<!--

`cleartextStream.authorized` is a boolean value which indicates if the
client has verified by one of the supplied cerificate authorities for the
server. If `cleartextStream.authorized` is false, then
`cleartextStream.authorizationError` is set to describe how authorization
failed. Implied but worth mentioning: depending on the settings of the TLS
server, you unauthorized connections may be accepted.

-->
`cleartextStream.authorized` は提供された認証局のいずれかによって認証されたかを示す boolean 値です。
`cleartextStream.authorized` が false の場合、
`cleartextStream.authorizationError` にはどのように認証が失敗したのかが設定されます。
暗黙的ですが言及する価値のあること:
TLS サーバの設定に依存しますが、認証されていないコネクションも受け入れられることがあります。


#### server.listen(port, [host], [callback])

<!--

Begin accepting connections on the specified `port` and `host`.  If the
`host` is omitted, the server will accept connections directed to any
IPv4 address (`INADDR_ANY`).

-->
指定の `port` と `host` で接続の受け入れを開始します。
`host` が省略されると、サーバはどんな IPv4 アドレスからのコネクションも受け入れます (`INADDR_ANY`)。

<!--

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound.

-->
この関数は非同期です。
最後の引数 `callback` はサーバがバインドされると呼び出されます。

<!--

See `net.Server` for more information.

-->
より詳細は `net.Server` を参照してください。


#### server.close()

<!--

Stops the server from accepting new connections. This function is
asynchronous, the server is finally closed when the server emits a `'close'`
event.

-->
サーバが新しい接続を受け入れることを終了します。
この関数は非同期で、サーバが最終的にクローズされるとサーバは `'close'` イベントを生成します。


#### server.maxConnections

<!--

Set this property to reject connections when the server's connection count gets high.

-->
このプロパティを設定すると、サーバの接続数がこれを越えた場合に接続を破棄します。

#### server.connections

<!--

The number of concurrent connections on the server.

-->
サーバの並行コネクションの数です。
