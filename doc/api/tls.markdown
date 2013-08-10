# TLS (SSL)

    Stability: 3 - Stable

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

<!--
To create .pfx or .p12, do this:
-->

.pfx または .p12 を作成するには次のようにします:

    openssl pkcs12 -export -in agent5-cert.pem -inkey agent5-key.pem \
        -certfile ca-cert.pem -out agent5.pfx

  - `in`:  certificate
  - `inkey`: private key
  - `certfile`: all CA certs concatenated in one file like
    `cat ca1-cert.pem ca2-cert.pem > ca-cert.pem`


## Client-initiated renegotiation attack mitigation

<!-- type=misc -->

<!--
The TLS protocol lets the client renegotiate certain aspects of the TLS session.
Unfortunately, session renegotiation requires a disproportional amount of
server-side resources, which makes it a potential vector for denial-of-service
attacks.

To mitigate this, renegotiations are limited to three times every 10 minutes. An
error is emitted on the [tls.TLSSocket][] instance when the threshold is
exceeded. The limits are configurable:

  - `tls.CLIENT_RENEG_LIMIT`: renegotiation limit, default is 3.

  - `tls.CLIENT_RENEG_WINDOW`: renegotiation window in seconds, default is
    10 minutes.

Don't change the defaults unless you know what you are doing.

To test your server, connect to it with `openssl s_client -connect address:port`
and tap `R<CR>` (that's the letter `R` followed by a carriage return) a few
times.
-->

TLS プロトコルでは、クライアントに TLS セッションの再ネゴシエーションを
許します。

残念ながら、セッション再ネゴシエーション要求はサーバサイドに過度なリソースを
要求するため、それは潜在的なサーバ強制停止攻撃となります。

これを軽減するために、再ネゴシエーションは 10 分当たり 3 回までに
制限されています。この制限を超えると、[tls.TLSSocket][]
のインスタンス上でエラーが生成されます。この制限は変更可能です:

  - `tls.CLIENT_RENEG_LIMIT`: 再ネゴシエーションの上限、デフォルトは 3 です。

  - `tls.CLIENT_RENEG_WINDOW`: 秒単位の再ネゴシエーションウィンドウ、
    デフォルトは 10 分です。

あなたが何をしようとしているか十分に理解していない限り、
デフォルトを変更しないでください。

サーバをテストするために、`openssl s_client -connect address:port`
および `R<CR>` (`R` キーの後に続けてリターンキー) を
数回繰り返します。


## NPN and SNI

<!-- type=misc -->

<!--
NPN (Next Protocol Negotiation) and SNI (Server Name Indication) are TLS
handshake extensions allowing you:

  * NPN - to use one TLS server for multiple protocols (HTTP, SPDY)
  * SNI - to use one TLS server for multiple hostnames with different SSL
    certificates.
-->

NPN (Next Protocol Negotitation) と SNI (Server Name Indication) は
TLS の拡張で、以下を可能にします。

  * NPN - 一つの TLS サーバで複数のプロトコル (HTTP、SPDY) を使用。
  * SNI - 一つの TLS サーバでホスト名の異なる複数の証明書を使用。


## tls.getCiphers()

<!--
Returns an array with the names of the supported SSL ciphers.
-->

サポートされている SSL 暗号名の配列を返します。

<!--
Example:
-->

例:

    var ciphers = tls.getCiphers();
    console.log(ciphers); // ['AES128-SHA', 'AES256-SHA', ...]


## tls.createServer(options, [secureConnectionListener])

<!--
Creates a new [tls.Server][].  The `connectionListener` argument is
automatically set as a listener for the [secureConnection][] event.  The
`options` object has these possibilities:
-->

新しい [tls.Server][] を作成します。
`connectionListener` は [secureConnection][] イベントのリスナとして
自動的に登録されます。
`options` は以下を持つことができます:


<!--
  - `pfx`: A string or `Buffer` containing the private key, certificate and
    CA certs of the server in PFX or PKCS12 format. (Mutually exclusive with
    the `key`, `cert` and `ca` options.)

  - `key`: A string or `Buffer` containing the private key of the server in
    PEM format. (Required)

  - `passphrase`: A string of passphrase for the private key or pfx.

  - `cert`: A string or `Buffer` containing the certificate key of the server in
    PEM format. (Required)

  - `ca`: An array of strings or `Buffer`s of trusted certificates. If this is
    omitted several well known "root" CAs will be used, like VeriSign.
    These are used to authorize connections.

  - `crl` : Either a string or list of strings of PEM encoded CRLs (Certificate
    Revocation List)

  - `ciphers`: A string describing the ciphers to use or exclude.

    To mitigate [BEAST attacks] it is recommended that you use this option in
    conjunction with the `honorCipherOrder` option described below to
    prioritize the non-CBC cipher.

    Defaults to `AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL:!EDH`.
    Consult the [OpenSSL cipher list format documentation] for details on the
    format. ECDH (Elliptic Curve Diffie-Hellman) ciphers are not yet supported.


    `AES128-GCM-SHA256` is used when node.js is linked against OpenSSL 1.0.1
    or newer and the client speaks TLS 1.2, RC4 is used as a secure fallback.

    **NOTE**: Previous revisions of this section suggested `AES256-SHA` as an
    acceptable cipher. Unfortunately, `AES256-SHA` is a CBC cipher and therefore
    susceptible to BEAST attacks. Do *not* use it.

  - `handshakeTimeout`: Abort the connection if the SSL/TLS handshake does not
    finish in this many milliseconds. The default is 120 seconds.

    A `'clientError'` is emitted on the `tls.Server` object whenever a handshake
    times out.

  - `honorCipherOrder` : When choosing a cipher, use the server's preferences
    instead of the client preferences.

    Note that if SSLv2 is used, the server will send its list of preferences
    to the client, and the client chooses the cipher.

    Although, this option is disabled by default, it is *recommended* that you
    use this option in conjunction with the `ciphers` option to mitigate
    BEAST attacks.

  - `requestCert`: If `true` the server will request a certificate from
    clients that connect and attempt to verify that certificate. Default:
    `false`.

  - `rejectUnauthorized`: If `true` the server will reject any connection
    which is not authorized with the list of supplied CAs. This option only
    has an effect if `requestCert` is `true`. Default: `false`.

  - `NPNProtocols`: An array or `Buffer` of possible NPN protocols. (Protocols
    should be ordered by their priority).

  - `SNICallback(servername, cb)`: A function that will be called if client
    supports SNI TLS extension. Two argument will be passed to it: `servername`,
    and `cb`. `SNICallback` should invoke `cb(null, ctx)`, where `ctx` is a
    SecureContext instance.
    (You can use `crypto.createCredentials(...).context` to get proper
    SecureContext). If `SNICallback` wasn't provided - default callback with
    high-level API will be used (see below).

  - `sessionTimeout`: An integer specifying the seconds after which TLS
    session identifiers and TLS session tickets created by the server are
    timed out. See [SSL_CTX_set_timeout] for more details.

  - `sessionIdContext`: A string containing a opaque identifier for session
    resumption. If `requestCert` is `true`, the default is MD5 hash value
    generated from command-line. Otherwise, the default is not provided.

  - `secureProtocol`: The SSL method to use, e.g. `SSLv3_method` to force
    SSL version 3. The possible values depend on your installation of
    OpenSSL and are defined in the constant [SSL_METHODS][].

-->

  - `pfx` : PFX または PKCS12 でエンコードされた秘密鍵、証明書、および CA の
    証明書を含む文字列またはバッファ
    (`key`、`cert`、および `ca` オプションとは相互に排他的です)。

  - `key`: PEM フォーマットによるサーバの秘密鍵を持つ文字列または `Buffer` です
    (必須)。

  - `passphrase`: 秘密鍵または pfx のパスフレーズを表す文字列です。

  - `cert`: PEM フォーマットによる証明書の鍵を持つ文字列または `Buffer` です
    (必須)。

  - `ca`: 信頼できる証明書の文字列または `Buffer` の配列です。
    省略された場合、ベリサインなどのよく知られた「ルート」認証局が使われます。
    これらはコネクションの認証に使われます。

  - `crl` : PEM でエンコードされた CRL (Certificate Revocation List、
  失効した証明書の一覧) の文字列または文字列の配列。

  - `ciphers`: 使用または除外する暗号を記述した文字列です。

    [BEAST 攻撃]を抑制するために、このオプションと以下に示す `honorCipherOrder`
    を共に使って、非 CBC 暗号を優先することを推奨します。

    デフォルトは `AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL:!EDH` です。
    詳細は [OpenSSL 暗号リストフォーマットのドキュメント] を参照してください。
    ECDH (Elliptic Curve Diffie-Hellman) 暗号はまだサポートされていません。

    `AES128-GCM-SHA256` は、Node.js が OpenSSL 1.0.1 以降とリンクされていて、
    クライアントが TLS 1.2 をサポートしている場合に使われます。
    RC4 は安全なフォールバックとして使われます。

    **注意**: 以前のバージョンのこのセクションは `AES256-SHA` を
    受け入れ可能な暗号であるかのように示していました。
    残念ながら、`AES256-SHA` は CBC 暗号であり、したがって BEAST
    攻撃には弱いです。

  - `handshakeTimeout`: SSL/TLS ハンドシェークがこの時間 (ミリ秒)
    以内に終了しなかった場合は接続をアボートします。
    デフォルトは 120 秒です。

    ハンドシェークがタイムアウトすると、`tls.Server` オブジェクトで
    `'clientError'` イベントが生成されます。

  - `honorCipherOrder` :
    暗号を選択する際に、クライアントではなくサーバの設定を使用します。

    SSLv2 が使われる場合は、サーバは設定のリストをクライアントに送信し、
    クライアントが暗号を選択することに注意してください。

    このオプションはデフォルトでは無効ですが、BEAST 攻撃を抑制するために
    `ciphers` オプションと共に使用することを *推奨* します。

  - `requestCert`: `true` の場合、サーバは接続しようとするクライアントからの
    証明書を要求します。デフォルトは `false` です。

  - `rejectUnauthorized`: `true` の場合、サーバは提供された認証局の
    リストによって認証されていないコネクションを破棄します．
    このオプションは `requestCert` が `true` の場合だけ効果があります。
    デフォルトは `false` です。

  - `NPNProtocols`: NPN プロトコルで使用可能な文字列または `Buffer` の配列
    (プロトコルはその優先度に応じて並んでいる必要があります)。

  - `SNICallback(servername, cb)`: クライアントが TLS 拡張の SNI を
    サポートしている場合に呼び出される関数です。
    二つの引数、`servername` と `cb` が渡されます。
    `SNICallback` は、`cb(null, ctx)` を呼び出す必要があります。
    `ctx` はSecureContext のインスタンスです
    (SecureContext を取得するために `crypto.createCredentials(...).context`
    を使用することができます)。
    `SNICallback` が渡されなかった場合は、デフォルトのコールバックとして
    後述する高水準 API が使用されます。

  - `sessionTimeout`: サーバによって作成された TLS セッション識別子および TLS
    セッションチケットがタイムアウトするまでの秒数を指定する整数値です。
    より詳細は [SSL_CTX_set_timeout] を参照してください。

  - `sessionIdContext`: セッション再開のための識別子となる文字列です。
    `requestCedrt` が `true` の場合、デフォルトはコマンドライン引数から
    生成された MD5 ハッシュ値となります。
    そうでない場合はデフォルトは提供されません。

  - `secureProtocol`: 使用する SSL メソッド、たとえば `SSLv3_method` は
    SSL version 3 の使用を強制します。可能な値は使用する OpenSSL によって
    定義される [SSL_METHODS][] 定数に依存します。

<!--
Here is a simple example echo server:
-->

これはシンプルはエコーサーバの例です:

    var tls = require('tls');
    var fs = require('fs');

    var options = {
      key: fs.readFileSync('server-key.pem'),
      cert: fs.readFileSync('server-cert.pem'),

      // This is necessary only if using the client certificate authentication.
      requestCert: true,

      // This is necessary only if the client uses the self-signed certificate.
      ca: [ fs.readFileSync('client-cert.pem') ]
    };

    var server = tls.createServer(options, function(socket) {
      console.log('server connected',
                  socket.authorized ? 'authorized' : 'unauthorized');
      socket.write("welcome!\n");
      socket.setEncoding('utf8');
      socket.pipe(socket);
    });
    server.listen(8000, function() {
      console.log('server bound');
    });

<!--
Or
-->

あるいは:

    var tls = require('tls');
    var fs = require('fs');

    var options = {
      pfx: fs.readFileSync('server.pfx'),

      // This is necessary only if using the client certificate authentication.
      requestCert: true,

    };

    var server = tls.createServer(options, function(socket) {
      console.log('server connected',
                  socket.authorized ? 'authorized' : 'unauthorized');
      socket.write("welcome!\n");
      socket.setEncoding('utf8');
      socket.pipe(socket);
    });
    server.listen(8000, function() {
      console.log('server bound');
    });

<!--
You can test this server by connecting to it with `openssl s_client`:
-->

`openssl s_client` を使用してこのサーバに接続するテストを行うことができます。

    openssl s_client -connect 127.0.0.1:8000


## tls.connect(options, [callback])
## tls.connect(port, [host], [options], [callback])

<!--
Creates a new client connection to the given `port` and `host` (old API) or
`options.port` and `options.host`. (If `host` is omitted, it defaults to
`localhost`.) `options` should be an object which specifies:
-->

与えられた `port` と `host` (旧 API) または `options.port` と `options.host`
で新しいクライアントコネクションを作成します
(`host` が省略された場合、デフォルトは `localhost` です)。
`options` は以下を指定したオブジェクトです:

<!--
  - `host`: Host the client should connect to

  - `port`: Port the client should connect to

  - `socket`: Establish secure connection on a given socket rather than
    creating a new socket. If this option is specified, `host` and `port`
    are ignored.

  - `pfx`: A string or `Buffer` containing the private key, certificate and
    CA certs of the server in PFX or PKCS12 format.

  - `key`: A string or `Buffer` containing the private key of the client in
    PEM format.

  - `passphrase`: A string of passphrase for the private key or pfx.

  - `cert`: A string or `Buffer` containing the certificate key of the client in
    PEM format.

  - `ca`: An array of strings or `Buffer`s of trusted certificates. If this is
    omitted several well known "root" CAs will be used, like VeriSign.
    These are used to authorize connections.

  - `rejectUnauthorized`: If `true`, the server certificate is verified against
    the list of supplied CAs. An `'error'` event is emitted if verification
    fails. Default: `true`.

  - `NPNProtocols`: An array of string or `Buffer` containing supported NPN
    protocols. `Buffer` should have following format: `0x05hello0x05world`,
    where first byte is next protocol name's length. (Passing array should
    usually be much simpler: `['hello', 'world']`.)

  - `servername`: Servername for SNI (Server Name Indication) TLS extension.
-->

  - `host`: クライアントが接続するホスト。

  - `port`: クライアントが接続するポート番号。

  - `socket`: 新しいソケットを生成するのではなく、与えられたソケット上で
    セキュアな接続を確立します。
    このオプションが指定された場合、`host` および `port` は無視されます。

  - `pfx` : PFX または PKCS12 でエンコードされた秘密鍵、証明書、
    およびサーバに対する CA の証明書を含む文字列またはバッファ。

  - `key`: PEM フォーマットによるサーバの秘密鍵を持つ文字列または
    `Buffer` です。

  - `passphrase`: 秘密鍵または pfx のパスフレーズを表す文字列です。

  - `cert`: PEM フォーマットによる証明書の鍵を持つ文字列または `Buffer` です。

  - `secureProtocol`: The SSL method to use, e.g. `SSLv3_method` to force
    SSL version 3. The possible values depend on your installation of
    OpenSSL and are defined in the constant [SSL_METHODS][].

  - `ca`: 信頼できる証明書の文字列または `Buffer` の配列です。
    省略された場合、ベリサインなどのよく知られた「ルート」認証局が使われます。
    これらはコネクションの認証に使われます。

  - `rejectUnauthorized`: `true` の場合、サーバ証明書は提供された認証局の
    リストによって検証されます。
    認証されなかった場合は `'error'` イベントが生成されます。
    認証は HTTP リクエストが送信される *前* にコネクションレベルで行われます。
    デフォルトは true です。

  - `NPNProtocols`: サポートする NPN プロトコルの文字列または `Buffer` 
    の配列です。
    `Buffer` は次のような形式です: `0x05hello0x5world`
    最初のバイトは次のプロトコル名の長さです
    (通常、配列を渡す方がシンプルです: `['hello', 'world']`)。

  - `servername`: TLS 拡張である SNI (Server Name Indication) のサーバ名です。

  - `secureProtocol`: 使用する SSL メソッド、たとえば `SSLv3_method` は
    SSL version 3 の使用を強制します。可能な値は使用する OpenSSL によって
    定義される [SSL_METHODS][] 定数に依存します。

<!--
The `callback` parameter will be added as a listener for the
['secureConnect'][] event.

`tls.connect()` returns a [tls.TLSSocket][] object.

Here is an example of a client of echo server as described previously:
-->

`callback` 引数は ['secureConnect'][] イベントのリスナとして
加えられます。

`tls.connect()` は [tls.TLSSocket][] オブジェクトを返します。

これは前述のエコーサーバに接続するクライアントの例です:

    var tls = require('tls');
    var fs = require('fs');

    var options = {
      // These are necessary only if using the client certificate authentication
      key: fs.readFileSync('client-key.pem'),
      cert: fs.readFileSync('client-cert.pem'),
    
      // This is necessary only if the server uses the self-signed certificate
      ca: [ fs.readFileSync('server-cert.pem') ]
    };

    var socket = tls.connect(8000, options, function() {
      console.log('client connected',
                  socket.authorized ? 'authorized' : 'unauthorized');
      process.stdin.pipe(socket);
      process.stdin.resume();
    });
    socket.setEncoding('utf8');
    socket.on('data', function(data) {
      console.log(data);
    });
    socket.on('end', function() {
      server.close();
    });

<!--
Or
-->

または:

    var tls = require('tls');
    var fs = require('fs');

    var options = {
      pfx: fs.readFileSync('client.pfx')
    };

    var socket = tls.connect(8000, options, function() {
      console.log('client connected',
                  socket.authorized ? 'authorized' : 'unauthorized');
      process.stdin.pipe(socket);
      process.stdin.resume();
    });
    socket.setEncoding('utf8');
    socket.on('data', function(data) {
      console.log(data);
    });
    socket.on('end', function() {
      server.close();
    });

## Class: tls.TLSSocket

<!--
Wrapper for instance of [net.Socket][], replaces internal socket read/write
routines to perform transparent encryption/decryption of incoming/outgoing data.
-->


[net.Socket][] のラッパーです。内部的なソケットの read/write 処理を、
入出力データを透過的に暗号化／復号化するように置き換えます。


## new tls.TLSSocket(socket, options)

<!--
Construct a new TLSSocket object from existing TCP socket.
-->

既存の TCP ソケットから新しい TLSSocket オブジェクトを構築します。

<!--
`socket` is an instance of [net.Socket][]
-->

`socket` は [net.Socket][] のインスタンスです。

<!--
`options` is an object that might contain following properties:
-->

`options` は以下のプロパティを持つことができるオブジェクトです。

<!--
  - `credentials`: An optional credentials object from
     `crypto.createCredentials( ... )`

  - `isServer`: If true - TLS socket will be instantiated in server-mode

  - `server`: An optional [net.Server][] instance

  - `requestCert`: Optional, see [tls.createSecurePair][]

  - `rejectUnauthorized`: Optional, see [tls.createSecurePair][]

  - `NPNProtocols`: Optional, see [tls.createServer][]

  - `SNICallback`: Optional, see [tls.createServer][]
-->

  - `credentials`: `crypto.createCredentials( ... )` から得られる
    オプションの認証情報

  - `isServer`: もし `true` なら、TLS ソケットはサーバも土で作成されます

  - `server`: オプションの [net.Server][] インスタンス

  - `requestCert`: オプション、[tls.createSecurePair][] を参照

  - `rejectUnauthorized`: オプション、[tls.createSecurePair][] を参照

  - `NPNProtocols`: オプション、[tls.createServer][] を参照

  - `SNICallback`: オプション、[tls.createServer][] を参照

## tls.createSecurePair([credentials], [isServer], [requestCert], [rejectUnauthorized])

    Stability: 0 - Deprecated. Use tls.TLSSocket instead.

<!--
Creates a new secure pair object with two streams, one of which reads/writes
encrypted data, and one reads/writes cleartext data.
Generally the encrypted one is piped to/from an incoming encrypted data stream,
and the cleartext one is used as a replacement for the initial encrypted stream.
-->

二つのストリームを持つセキュアペアオブジェクトを作成します。
一つは暗号化されたデータを読み書きし、もう一つは平文のデータを読み書きします。
通常、暗号化されたストリームに外部からの暗号化されたデータが連結され、
暗号化されたストリームの代わりに平文のストリームが使われます。

<!--
 - `credentials`: A credentials object from crypto.createCredentials( ... )

 - `isServer`: A boolean indicating whether this tls connection should be
   opened as a server or a client.

 - `requestCert`: A boolean indicating whether a server should request a
   certificate from a connecting client. Only applies to server connections.

 - `rejectUnauthorized`: A boolean indicating whether a server should
   automatically reject clients with invalid certificates. Only applies to
   servers with `requestCert` enabled.
-->

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

<!--
`tls.createSecurePair()` returns a SecurePair object with `cleartext` and
`encrypted` stream properties.
-->

`tls.createSequrePair()` は、`cleartext` と `encrypted` ストリームを
プロパティとして持つ `SecurePair` オブジェクトを返します。

<!--
NOTE: `cleartext` has the same APIs as [tls.TLSSocket][]
-->

注意: `cleartext` は [tls.TLSSocket][] API と同じです。

## Class: SecurePair

<!--
Returned by tls.createSecurePair.
-->

`tls.createSecurePair` から返されます。

### Event: 'secure'

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

## Class: tls.Server

<!--
This class is a subclass of `net.Server` and has the same methods on it.
Instead of accepting just raw TCP connections, this accepts encrypted
connections using TLS or SSL.
-->

このクラスは `net.Server` のサブクラスで、同じメソッドを持っています。
生の TCP コネクションを受け入れる代わりに、
TLS または SSL を使った暗号化されたコネクションを受け付けます。

### Event: 'secureConnection'

`function (tlsSocket) {}`

<!--
This event is emitted after a new connection has been successfully
handshaked. The argument is a instance of [tls.TLSSocket][]. It has all the
common stream methods and events.
-->

このイベントは、新しい接続のハンドシェークが成功した場合に生成されます。
引数は [tls.TLSSocket][] のインスタンスです。
これはストリームに共通する全てのメソッドとイベントを持っています。

<!--
`socket.authorized` is a boolean value which indicates if the
client has verified by one of the supplied certificate authorities for the
server. If `socket.authorized` is false, then
`socket.authorizationError` is set to describe how authorization
failed. Implied but worth mentioning: depending on the settings of the TLS
server, you unauthorized connections may be accepted.
`socket.npnProtocol` is a string containing selected NPN protocol.
`socket.servername` is a string containing servername requested with
SNI.
-->

`socket.authorized` は提供された認証局のいずれかによって
認証されたかを示す boolean 値です。
`socket.authorized` が false の場合、`socket.authorizationError` には
どのように認証が失敗したのかが設定されます。
暗黙的ですが言及する価値のあること: TLS サーバの設定に依存しますが、
認証されていないコネクションも受け入れられることがあります。
`socket.npnProtocol` は、選択された NPN プロトコルを持つ文字列です。
`socket.servername` は、SNI でリクエストされたサーバ名を持つ文字列です。

### Event: 'clientError'

`function (exception, tlsSocket) { }`

<!--
When a client connection emits an 'error' event before secure connection is
established - it will be forwarded here.
-->

セキュアコネクションが確立される前にクライアントコネクションが
`'error'` イベントを発した場合 － ここに転送されます。

<!--
`tlsSocket` is the [tls.TLSSocket][] that the error originated from.
-->

`tlsSocket` はエラーが発生した [tls.TLSSocket][] です。


### Event: 'newSession'

`function (sessionId, sessionData) { }`

<!--
Emitted on creation of TLS session. May be used to store sessions in external
storage.
-->

TLS セッションが作成された場合に生成されます。
セッションを外部ストレージに保存する場合に使えるでしょう。

<!--
NOTE: adding this event listener will have an effect only on connections
established after addition of event listener.
-->

注意: このイベントリスナの追加は、イベントリスナが追加された後に確立される
接続に対してのみ効果があります。


### Event: 'resumeSession'

`function (sessionId, callback) { }`

<!--
Emitted when client wants to resume previous TLS session. Event listener may
perform lookup in external storage using given `sessionId`, and invoke
`callback(null, sessionData)` once finished. If session can't be resumed
(i.e. doesn't exist in storage) one may call `callback(null, null)`. Calling
`callback(err)` will terminate incoming connection and destroy socket.
-->

クライアントが以前の TLS セッションを再開を要求した場合に生成されます。
イベントリスナは与えられた `sessionId` を使用して外部ストレージから
セッションを見つけた場合、`callback(null, sessionData)` を一度呼び出すことが
できます。
セッションを再開できない場合 (すなわち、ストレージに存在しない場合)、
`callback(null, null)` を呼ぶことができます。
`callback(err)` を呼び出すと接続を終了し、ソケットを破棄します。

NOTE: adding this event listener will have an effect only on connections
established after addition of event listener.


### server.listen(port, [host], [callback])

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


### server.close()

<!--
Stops the server from accepting new connections. This function is
asynchronous, the server is finally closed when the server emits a `'close'`
event.
-->

サーバが新しい接続を受け入れることを終了します。
この関数は非同期で、サーバが最終的にクローズされるとサーバは `'close'` イベントを生成します。

### server.address()

<!--
Returns the bound address, the address family name and port of the
server as reported by the operating system.  See [net.Server.address()][] for
more information.
-->

オペレーティングシステムから報告された、サーバにバインドされたアドレスと
アドレスファミリ名、ポートを返します。 
より詳しくは [net.Server.address()][] を参照してください。

### server.addContext(hostname, credentials)

<!--
Add secure context that will be used if client request's SNI hostname is
matching passed `hostname` (wildcards can be used). `credentials` can contain
`key`, `cert` and `ca`.
-->

クライアントが要求してきた SNI ホスト名と `hostname` (ワイルドカードを使用可能)
がマッチした場合のセキュリティコンテキストを追加します。
`credentials` は `key`、`cert`、そして `ca` を含むことができます。

### server.maxConnections

<!--
Set this property to reject connections when the server's connection count
gets high.
-->

このプロパティを設定すると、サーバの接続数がこれを越えた場合に接続を破棄します。
### server.connections

<!--
The number of concurrent connections on the server.
-->

サーバの並行コネクションの数です。

## Class: CryptoStream

    Stability: 0 - Deprecated. Use tls.TLSSocket instead.

<!--
This is an encrypted stream.
-->

これは暗号化されたストリームです。

### cryptoStream.bytesWritten

<!--
A proxy to the underlying socket's bytesWritten accessor, this will return
the total bytes written to the socket, *including the TLS overhead*.
-->

下層にあるソケットの bytesWritten にアクセスするプロキシで、
*TLS のオーバーヘッドを含めて* ソケットに書き込まれたトータルのバイト数を
返します。

## Class: tls.TLSSocket

<!--
This is a wrapped version of [net.Socket][] that does transparent encryption
of written data and all required TLS negotiation.

This instance implements a duplex [Stream][] interfaces.  It has all the
common stream methods and events.
-->

これは暗号化されたデータの透過的な書き込みなど、 TLS ネゴシエーションによって
要求される全てを行う [net.Socket][] のラップされたバージョンです。

このインスタンスは双方向の [Stream][] インタフェースを実装します。
ストリームに共通な全てのメソッドとイベントを持ちます。

### Event: 'secureConnect'

<!--
This event is emitted after a new connection has been successfully handshaked.
The listener will be called no matter if the server's certificate was
authorized or not. It is up to the user to test `tlsSocket.authorized`
to see if the server certificate was signed by one of the specified CAs.
If `tlsSocket.authorized === false` then the error can be found in
`tlsSocket.authorizationError`. Also if NPN was used - you can check
`tlsSocket.npnProtocol` for negotiated protocol.
-->

新しいコネクションの TLS/SSL ハンドシェークが成功すると生成されます。
リスナはサーバの証明書が認証されたかどうかに関わらず呼び出されます。
サーバ証明書が指定した認証局に承認されたかチェックするために
`tlsSocket.authorized` を確認するかはユーザ次第です。
`tlsSocket.authorized === false`の場合、
`tlsSocket.authorizationError` からエラーを見つけることができます。
同様に NPN が使われている場合は `tlsSocket.npnProtocol`
から合意されたプロトコルをチェックすることが出来ます。

### tlsSocket.authorized

<!--
A boolean that is `true` if the peer certificate was signed by one of the
specified CAs, otherwise `false`
-->

接続相手の証明書が CA の一つによって署名されていれば `true`、
そうでなければ `false` です。

### tlsSocket.authorizationError

<!--
The reason why the peer's certificate has not been verified. This property
becomes available only when `tlsSocket.authorized === false`.
-->

接続相手の証明書が認証されなかった理由です。
このプロパティは `tlsSocket.authorized === false`
の場合だけ利用可能になります。

### tlsSocket.getPeerCertificate()

<!--
Returns an object representing the peer's certificate. The returned object has
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

### tlsSocket.getCipher()

<!--
Returns an object representing the cipher name and the SSL/TLS
protocol version of the current connection.
-->

現在の接続における暗号と SSL/TLS プロトコルのバージョンを表現する
オブジェクトを返します。

<!--
Example:
-->

例:

    { name: 'AES256-SHA', version: 'TLSv1/SSLv3' }

<!--
See SSL_CIPHER_get_name() and SSL_CIPHER_get_version() in
http://www.openssl.org/docs/ssl/ssl.html#DEALING_WITH_CIPHERS for more
information.
-->

詳細は
http://www.openssl.org/docs/ssl/ssl.html#DEALING_WITH_CIPHERS
で `SSL_CIPHER_get_name()` および `SSL_CIPHER_get_version()` を
参照してください。

### tlsSocket.address()

<!--
Returns the bound address, the address family name and port of the
underlying socket as reported by the operating system. Returns an
object with three properties, e.g.
`{ port: 12346, family: 'IPv4', address: '127.0.0.1' }`
-->

オペレーティングシステムから報告された、ソケットにバインドされたアドレスと
アドレスファミリ名、ポートを返します。
返されるオブジェクトは二つのプロパティを持ちます。例:
`{ port: 12346, family: 'IPv4', address: '127.0.0.1' }`

### tlsSocket.remoteAddress

<!--
The string representation of the remote IP address. For example,
`'74.125.127.100'` or `'2001:4860:a005::68'`.
-->

リモートの IP アドレスを表現する文字列です。
例えば、`'74.125.127.100'` あるいは `'2001:4860:a005::68'`。

### tlsSocket.remotePort

<!--
The numeric representation of the remote port. For example, `443`.
-->

リモートポートの数値表現です。
例えば、`443`。

### tlsSocket.localAddress

<!--
The string representation of the local IP address.
-->

文字列表現によるローカル IP アドレスです。

### tlsSocket.localPort

<!--
The numeric representation of the local port.
-->

数値表現によるローカルポートです。

<!--
[OpenSSL cipher list format documentation]: http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT
[BEAST attacks]: http://blog.ivanristic.com/2011/10/mitigating-the-beast-attack-on-tls.html
[tls.createServer]: #tls_tls_createserver_options_secureconnectionlistener
[tls.createSecurePair]: #tls_tls_createsecurepair_credentials_isserver_requestcert_rejectunauthorized
[tls.TLSSocket]: #tls_class_tls_tlssocket
[net.Server]: net.html#net_class_net_server
[net.Socket]: net.html#net_class_net_socket
[net.Server.address()]: net.html#net_server_address
['secureConnect']: #tls_event_secureconnect
[secureConnection]: #tls_event_secureconnection
[Stream]: stream.html#stream_stream
[SSL_METHODS]: http://www.openssl.org/docs/ssl/ssl.html#DEALING_WITH_PROTOCOL_METHODS
[tls.Server]: #tls_class_tls_server
[SSL_CTX_set_timeout]: http://www.openssl.org/docs/ssl/SSL_CTX_set_timeout.html
-->

[OpenSSL 暗号リストフォーマットのドキュメント]: http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT
[BEAST 攻撃]: http://blog.ivanristic.com/2011/10/mitigating-the-beast-attack-on-tls.html
[tls.createServer]: #tls_tls_createserver_options_secureconnectionlistener
[tls.createSecurePair]: #tls_tls_createsecurepair_credentials_isserver_requestcert_rejectunauthorized
[tls.TLSSocket]: #tls_class_tls_tlssocket
[net.Server]: net.html#net_class_net_server
[net.Socket]: net.html#net_class_net_socket
[net.Server.address()]: net.html#net_server_address
[net.Socket]: net.html#net_class_net_socket
['secureConnect']: #tls_event_secureconnect
[secureConnection]: #tls_event_secureconnection
[Stream]: stream.html#stream_stream
[SSL_METHODS]: http://www.openssl.org/docs/ssl/ssl.html#DEALING_WITH_PROTOCOL_METHODS
[tls.Server]: #tls_class_tls_server
[SSL_CTX_set_timeout]: http://www.openssl.org/docs/ssl/SSL_CTX_set_timeout.html
