# HTTPS

    Stability: 3 - Stable

<!--
HTTPS is the HTTP protocol over TLS/SSL. In Node this is implemented as a
separate module.
-->

HTTPS は TLS/SSL 上の HTTP プロトコルです。
Node ではこれらは別のモジュールとして実装されています。

## Class: https.Server

<!--
This class is a subclass of `tls.Server` and emits events same as
`http.Server`. See `http.Server` for more information.
-->

このクラスは `tls.Server` のサブクラスで、`http.Server` と同様のイベントを生成します。
より詳しくは `http.Server` を参照してください。

## https.createServer(options, [requestListener])

<!--
Returns a new HTTPS web server object. The `options` is similar to
[tls.createServer()](tls.html#tls.createServer).  The `requestListener` is
a function which is automatically added to the `'request'` event.
-->

新しい HTTPS Web サーバオブジェクトを返します。
`option` は `tls.createServer()` と同じです。
`requestListener` は関数で、 `'request'` イベントに自動的に追加されます。

<!--
Example:
-->

例:

    // curl -k https://localhost:8000/
    var https = require('https');
    var fs = require('fs');

    var options = {
      key: fs.readFileSync('test/fixtures/keys/agent2-key.pem'),
      cert: fs.readFileSync('test/fixtures/keys/agent2-cert.pem')
    };

    https.createServer(options, function (req, res) {
      res.writeHead(200);
      res.end("hello world\n");
    }).listen(8000);

<!--
Or
-->

または:

    var https = require('https');
    var fs = require('fs');

    var options = {
      pfx: fs.readFileSync('server.pfx')
    };

    https.createServer(options, function (req, res) {
      res.writeHead(200);
      res.end("hello world\n");
    }).listen(8000);

## https.request(options, callback)

<!--
Makes a request to a secure web server.
All options from [http.request()](http.html#http.request) are valid.
-->

セキュアな Web サーバへのリクエストを作成します。
[http.request()](http.html#http.request) の全てと同様のオプションが指定できます。

<!--
Example:
-->

例:

    var https = require('https');

    var options = {
      host: 'encrypted.google.com',
      port: 443,
      path: '/',
      method: 'GET'
    };

    var req = https.request(options, function(res) {
      console.log("statusCode: ", res.statusCode);
      console.log("headers: ", res.headers);

      res.on('data', function(d) {
        process.stdout.write(d);
      });
    });
    req.end();

    req.on('error', function(e) {
      console.error(e);
    });

<!--
The options argument has the following options
-->

`options` 引数は以下のオプションを持ちます。

<!--
- host: IP or domain of host to make request to. Defaults to `'localhost'`.
- port: port of host to request to. Defaults to 443.
- path: Path to request. Default `'/'`.
- method: HTTP request method. Default `'GET'`.

- `host`: A domain name or IP address of the server to issue the request to.
  Defaults to `'localhost'`.
- `hostname`: To support `url.parse()` `hostname` is preferred over `host`
- `port`: Port of remote server. Defaults to 443.
- `method`: A string specifying the HTTP request method. Defaults to `'GET'`.
- `path`: Request path. Defaults to `'/'`. Should include query string if any.
  E.G. `'/index.html?page=12'`
- `headers`: An object containing request headers.
- `auth`: Basic authentication i.e. `'user:password'` to compute an
  Authorization header.
- `agent`: Controls [Agent](#https.Agent) behavior. When an Agent is
  used request will default to `Connection: keep-alive`. Possible values:
 - `undefined` (default): use [globalAgent](#https.globalAgent) for this
   host and port.
 - `Agent` object: explicitly use the passed in `Agent`.
 - `false`: opts out of connection pooling with an Agent, defaults request to
   `Connection: close`.
-->

- `host`: リクエストを発行するサーバのドメイン名または IP アドレス。
   デフォルトは `'localhost'` です。
- `hostname`: `url.parse()` サポート。`hostname` は `host` を上書きします。
- `port`: リモートサーバのポート。デフォルトは 443 です。
- `method`: HTTPS リクエストのメソッドの文字列。デフォルトは `'GET'` です。
- `path`: リクエストのパス。デフォルトは `'/'` です。
  必要なら問い合わせ文字列を含めるべきです．
  例 `'/index.html?page=12'`
- `headers`: リクエストヘッダを含むオブジェクト。
- `auth`: べーしく認証すなわち Authorization ヘッダのための `'user:password'`。
- `agent`: [Agent](#https.Agent) の振る舞いを制御します。
  エージェントが使われる場合、`Connection:keep-alive` がデフォルトになります。
  可能な値は:
  - `undefined` (デフォルト): ホストとポートで
    [グローバル Agent](#https.globalAgent) を使用します。
  - `Agent` オブジェクト: 明示的に渡された `Agent` を使用します。
  - `false`: Agent によるコネクションプーリングを使用しません。
    `Connection:close` の場合のデフォルトです。

<!--
The following options from [tls.connect()](tls.html#tls.connect) can also be
specified. However, a [globalAgent](#https.globalAgent) silently ignores these.
-->

以下の [tls.connect()](tls.html#tls.connect) 由来のオプションを
指定することもできますが、
グローバル [エージェント](http.html#http.Agent) はこれらを無視します。

<!--
- `pfx`: Certificate, Private key and CA certificates to use for SSL. Default `null`.
- `key`: Private key to use for SSL. Default `null`.
- `passphrase`: A string of passphrase for the private key or pfx. Default `null`.
- `cert`: Public x509 certificate to use. Default `null`.
- `ca`: An authority certificate or array of authority certificates to check
  the remote host against.
- `ciphers`: A string describing the ciphers to use or exclude. Consult
  <http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT> for
  details on the format.
- `rejectUnauthorized`: If `true`, the server certificate is verified against
  the list of supplied CAs. An `'error'` event is emitted if verification
  fails. Verification happens at the connection level, *before* the HTTP
  request is sent. Default `false`.
-->

- `pfx`: SSL で使用する証明書、秘密鍵、認証局の証明書。
   デフォルトは `null` です。
- `key`: SSL で使用する秘密鍵。デフォルトは `null` です。
- `passphrase`: 秘密鍵または pfx のパスフレーズを表す文字列。
   デフォルトは `null` です。
- `cert`: x509公開証明書。デフォルトは `null` です。
- `ca`: リモートホストをチェックする信頼できる認証局または認証局の配列。
- `ciphers`: 使用または除外する暗号を記述した文字列。
  詳細は <http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT>
  を参照してください。
- `rejectUnauthorized`: `true` の場合、サーバ証明書は提供された認証局の
  リストによって検証されます。
  認証されなかった場合は `'error'` イベントが生成されます。
  認証は HTTP リクエストが送信される *前* にコネクションレベルで行われます。
  デフォルトは false です。

<!--
In order to specify these options, use a custom `Agent`.
-->

これらのオプションを指定するには、カスタムエージェントを使用します。

<!--
Example:
-->

例:

    var options = {
      host: 'encrypted.google.com',
      port: 443,
      path: '/',
      method: 'GET',
      key: fs.readFileSync('test/fixtures/keys/agent2-key.pem'),
      cert: fs.readFileSync('test/fixtures/keys/agent2-cert.pem')
    };
    options.agent = new https.Agent(options);

    var req = https.request(options, function(res) {
      ...
    }

<!--
Or does not use an `Agent`.
-->

あるいは、エージェントを使用しません。

<!--
Example:
-->

例:

    var options = {
      host: 'encrypted.google.com',
      port: 443,
      path: '/',
      method: 'GET',
      key: fs.readFileSync('test/fixtures/keys/agent2-key.pem'),
      cert: fs.readFileSync('test/fixtures/keys/agent2-cert.pem'),
      agent: false
    };

    var req = https.request(options, function(res) {
      ...
    }


## https.get(options, callback)

<!--
Like `http.get()` but for HTTPS.
-->

`http.get()` と同様ですが HTTPS です。

<!--
Example:
-->

例:

    var https = require('https');

    https.get({ host: 'encrypted.google.com', path: '/' }, function(res) {
      console.log("statusCode: ", res.statusCode);
      console.log("headers: ", res.headers);

      res.on('data', function(d) {
        process.stdout.write(d);
      });

    }).on('error', function(e) {
      console.error(e);
    });


## Class: https.Agent

<!--
An Agent object for HTTPS similar to [http.Agent](http.html#http.Agent).
See [https.request()](#https.request) for more information.
-->

HTTPS 用の Agent オブジェクトで，
[http.Agent](http.html#http.Agent) と同様です。
詳細は [https.request()](#https.request) を参照してください。

## https.globalAgent

<!--
Global instance of [https.Agent](#https.Agent) which is used as the default
for all HTTPS client requests.
-->

全ての HTTPS クライアントリクエストで使用される、デフォルトの
[https.Agent](#https.Agent) のインスタンスです。
