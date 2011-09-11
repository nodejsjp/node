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

  - `NPNProtocols`: An array of string or `Buffer` containing supported NPN
    protocols. `Buffer` should have following format: `0x05hello0x05world`, where
    first byte is next protocol name's length. (Passing array should usually be
    much simplier: `['hello', 'world']`.)

  - `servername`: Servername for SNI (Server Name Indication) TLS extension.

-->
  - `key`: PEM フォーマットによるサーバの秘密鍵を持つ文字列または `Buffer` です (必須)。

  - `cert`: PEM フォーマットによる証明書の鍵を持つ文字列または `Buffer` です。

  - `ca`: 信頼できる証明書の文字列または `Buffer` の配列です。
    省略された場合、ベリサインなどのよく知られた「ルート」認証局が使われます。
    これらはコネクションの認証に使われます。

  - `NPNProtocols`: サポートする NPN プロトコルの文字列または `Buffer` 
    の配列です。
    `Buffer` は次のような形式です: `0x05hello0x5world`
    最初のバイトは次のプロトコル名の長さです
    (通常、配列を渡す方がシンプルです: `['hello', 'world']`)。

  - `servername`: TLS 拡張である SNI (Server Name Indication) のサーバ名です。

<!--

`tls.connect()` returns a [CleartextStream](#tls.CleartextStream) object.

-->
`tls.connect()` は [CleartextStream](#tls.CleartextStream) 
オブジェクトを返します。

<!--

After the TLS/SSL handshake the `callback` is called. The `callback` will be
called no matter if the server's certificate was authorized or not. It is up
to the user to test `s.authorized` to see if the server certificate was signed
by one of the specified CAs. If `s.authorized === false` then the error
can be found in `s.authorizationError`. Also if NPN was used - you can check
`s.npnProtocol` for negotiated protocol.

-->
TLS/SSL ハンドシェークの後で `callback` が呼び出されます。
`callback` は証明書がサーバに認証されたかどうかに関わらず呼び出されます。
サーバ証明書が指定した認証局に承認されたかチェックするために
`s.authorized` を確認するかはユーザ次第です。
`s.authorized === false`の場合、`s.authorizationError` からエラーを見つけることができます。
同様に NPN が使われている場合は `s.npnProtocol` から合意されたプロトコルを
チェックすることが出来ます。

### STARTTLS

<!--

In the v0.4 branch no function exists for starting a TLS session on an
already existing TCP connection.  This is possible it just requires a bit of
work. The technique is to use `tls.createSecurePair()` which returns two
streams: an encrypted stream and a cleartext stream. The encrypted stream is
then piped to the socket, the cleartext stream is what the user interacts with
thereafter.

[Here is some code that does it.](http://gist.github.com/848444)

-->
v0.4 ブランチでは、既に存在する TCP コネクション上で TLS セッションを開始する機能はありません。
それには少しの作業が必要となります。
そのテクニックは `tls.createSecurePair()` が返す二つのストリーム:
暗号化されたストリームと平文のストリームを使います。
暗号化されたストリームは既存のソケットにつながれ、
平文のストリームはその後ユーザとのインタラクションで使われます。

[ここにそのコードがあります。](http://gist.github.com/848444)

### NPN and SNI

<!--
NPN (Next Protocol Negotitation) and SNI (Server Name Indication) are TLS
handshake extensions allowing you:
-->
NPN (Next Protocol Negotitation) と SNI (Server Name Indication) は
TLS の拡張で、以下を可能にします。

<!--
  * NPN - to use one TLS server for multiple protocols (HTTP, SPDY)
  * SNI - to use one TLS server for multiple hostnames with different SSL
    certificates.
-->
  * NPN - 一つの TLS サーバで複数のプロトコル (HTTP、SPDY) を使用。
  * SNI - 一つの TLS サーバでホスト名の異なる複数の証明書を使用。

### pair = tls.createSecurePair([credentials], [isServer], [requestCert], [rejectUnauthorized])

<!--
Creates a new secure pair object with two streams, one of which reads/writes
encrypted data, and one reads/writes cleartext data.
Generally the encrypted one is piped to/from an incoming encrypted data stream,
and the cleartext one is used as a replacement for the initial encrypted stream.
 - `credentials`: A credentials object from crypto.createCredentials( ... )

 - `isServer`: A boolean indicating whether this tls connection should be
   opened as a server or a client.

 - `requestCert`: A boolean indicating whether a server should request a
   certificate from a connecting client. Only applies to server connections.

 - `rejectUnauthorized`: A boolean indicating whether a server should
   automatically reject clients with invalid certificates. Only applies to
   servers with `requestCert` enabled.

`tls.createSecurePair()` returns a SecurePair object with
[cleartext](#tls.CleartextStream) and `encrypted` stream properties.
-->
二つのストリームを持つセキュアペアオブジェクトを作成します。
一つは暗号化されたデータを読み書きし、もう一つは平文のデータを読み書きします。
通常、暗号化されたストリームに外部からの暗号化されたデータが連結され、
暗号化されたストリームの代わりに平文のストリームが使われます。

 - `credentials`: `crypto.createCredentials( ... )` で作成された
   証明書オブジェクト。

 - `isServer`: この TLS コネクションをサーバとしてオープンするかどうかを示す
   ブーリアン値。

 - `requestCert`: クライアントからの接続に対して、サーバがクライアントに
   証明書を要求するかどうかを示すブーリアン値。
   サーバコネクションにのみ適用されます。

 - `rejectUnauthorized`: クライアント認証が不正だった場合に、
   自動的にクライアントを破棄するかどうかを示すブーリアン値。
   `requestCert` が有効なサーバにのみ適用されます。

`tls.createSequrePair()` は、[cleartext](#tls.CleartextStream) と `encrypted`
をプロパティとして持つ `SecurePair` オブジェクトを返します。

#### Event: 'secure'

<!--
The event is emitted from the SecurePair once the pair has successfully
established a secure connection.

Similarly to the checking for the server 'secureConnection' event,
pair.cleartext.authorized should be checked to confirm whether the certificate
used properly authorized.
-->
SecurePair オブジェクトのペアが安全な接続を確立した場合に発生します。

サーバの `'secureConnection'` イベントと同様に、
`pari.cleartext.authorized` によって接続相手の証明書を承認できたかどうかを
チェックすることができます。

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

  - `NPNProtocols`: An array or `Buffer` of possible NPN protocols. (Protocols
    should be ordered by their priority).

  - `SNICallback`: A function that will be called if client supports SNI TLS
    extension. Only one argument will be passed to it: `servername`. And
    `SNICallback` should return SecureContext instance.
    (You can use `crypto.createCredentials(...).context` to get proper
    SecureContext). If `SNICallback` wasn't provided - default callback with
    high-level API will be used (see below).

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

  - `NPNProtocols`: NPN プロトコルで使用可能な文字列または `Buffer` の配列
    (プロトコルはその優先度に応じて並んでいる必要があります)。

  - `SNICallback`: クライアントが TLS 拡張の SNI をサポートしている場合に
    呼び出される関数です。
    `servername` が唯一の引数として渡されます。
    `SNICallback` は SecureContext のインスタンスを返す必要があります
    (SecureContext を取得するために `crypto.createCredentials(...).context`
    を使用することができます)。
    `SNICallback` が渡されなかった場合は、デフォルトのコールバックとして
    後述する高水準 API が使用されます。

#### Event: 'secureConnection'

`function (cleartextStream) {}`

<!--

This event is emitted after a new connection has been successfully
handshaked. The argument is a instance of
[CleartextStream](#tls.CleartextStream). It has all the common stream methods
and events.

-->
このイベントは、新しい接続のハンドシェークが成功した場合に生成されます。
引数は [CleartextStream](#tls.CleartextStream) のインスタンスです。
これはストリームに共通する全てのメソッドとイベントを持っています。

<!--

`cleartextStream.authorized` is a boolean value which indicates if the
client has verified by one of the supplied certificate authorities for the
server. If `cleartextStream.authorized` is false, then
`cleartextStream.authorizationError` is set to describe how authorization
failed. Implied but worth mentioning: depending on the settings of the TLS
server, you unauthorized connections may be accepted.
`cleartextStream.npnProtocol` is a string containing selected NPN protocol.
`cleartextStream.servername` is a string containing servername requested with
SNI.

-->
`cleartextStream.authorized` は提供された認証局のいずれかによって認証されたかを示す boolean 値です。
`cleartextStream.authorized` が false の場合、
`cleartextStream.authorizationError` にはどのように認証が失敗したのかが設定されます。
暗黙的ですが言及する価値のあること:
TLS サーバの設定に依存しますが、認証されていないコネクションも受け入れられることがあります。
`cleartextStream.npnProtocol` は、選択された NPN プロトコルを持つ文字列です。
`cleartextStream.servername` は、SNI でリクエストされたサーバ名を持つ
文字列です。

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

#### server.addContext(hostname, credentials)

<!--

Add secure context that will be used if client request's SNI hostname is
matching passed `hostname` (wildcards can be used). `credentials` can contain
`key`, `cert` and `ca`.

-->
クライアントが要求してきた SNI ホスト名と `hostname` (ワイルドカードを使用可能)
がマッチした場合のセキュリティコンテキストを追加します。
`credentials` は `key`、`cert`、そして `ca` を含むことができます。

#### server.maxConnections

<!--

Set this property to reject connections when the server's connection count
gets high.

-->
このプロパティを設定すると、サーバの接続数がこれを越えた場合に接続を破棄します。

#### server.connections

<!--

The number of concurrent connections on the server.

-->
サーバの並行コネクションの数です。

### tls.CleartextStream

<!--
This is a stream on top of the *Encrypted* stream that makes it possible to
read/write an encrypted data as a cleartext data.

This instance implements a duplex [Stream](streams.html#streams) interfaces.
It has all the common stream methods and events.
-->
暗号化されたストリーム上で、暗号化されたデータを平文のデータとして
読み書きすることができるストリームです。

このインスタンスは双方向の [Stream](streams.html#streams) インタフェースを
実装します。
ストリームに共通な全てのメソッドとイベントを持ちます。

#### cleartextStream.authorized

<!--
A boolean that is `true` if the peer certificate was signed by one of the
specified CAs, otherwise `false`
-->
接続相手の証明書が CA の一つによって署名されていれば `true`、
そうでなければ `false` です。

#### cleartextStream.authorizationError

<!--
The reason why the peer's certificate has not been verified. This property
becomes available only when `cleartextStream.authorized === false`.
-->
接続相手の証明書が認証されなかった理由です。
このプロパティは `cleartextStream.authorized === false`
の場合だけ利用可能になります。

#### cleartextStream.getPeerCertificate()

<!--
Returns an object representing the peer's certicicate. The returned object has
some properties corresponding to the field of the certificate.
-->
接続相手の証明書を表現するオブジェクトを返します。
返されるオブジェクトは証明書のフィールドに対応するプロパティを持ちます。

<!--
Example:
-->
例:

    { subject: 
       { C: 'UK',
         ST: 'Acknack Ltd',
         L: 'Rhys Jones',
         O: 'node.js',
         OU: 'Test TLS Certificate',
         CN: 'localhost' },
      issuer: 
       { C: 'UK',
         ST: 'Acknack Ltd',
         L: 'Rhys Jones',
         O: 'node.js',
         OU: 'Test TLS Certificate',
         CN: 'localhost' },
      valid_from: 'Nov 11 09:52:22 2009 GMT',
      valid_to: 'Nov  6 09:52:22 2029 GMT',
      fingerprint: '2A:7A:C2:DD:E5:F9:CC:53:72:35:99:7A:02:5A:71:38:52:EC:8A:DF' }

<!--
If the peer does not provide a certificate, it returns `null` or an empty
object.
-->
接続相手が証明書を提供しなかった場合は、
`null` または空のオブジェクトを返します。
