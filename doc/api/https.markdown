## HTTPS

<!--

HTTPS is the HTTP protocol over TLS/SSL. In Node this is implemented as a
separate module.

-->
HTTPS は TLS/SSL 上の HTTP プロトコルです。
Node ではこれらは別のモジュールとして実装されています。

## https.Server
## https.createServer

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


## https.request(options, callback)

<!--

Makes a request to a secure web server.
Similar options to `http.request()`.

-->
セキュアな Web サーバへのリクエストを作成します。
`http.request()` と同様のオプションが指定できます。

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
- key: Private key to use for SSL. Default `null`.
- cert: Public x509 certificate to use. Default `null`.
- ca: An authority certificate or array of authority certificates to check
  the remote host against.

-->
- `host`: リクエストするホストのIPまたはドメイン。デフォルトは `'localhost'` です。
- `port`: リクエストするホストのポート。デフォルトは 443 です。
- `path`: リクエストのパス。デフォルトは `'/'` です。
- `method`: HTTP りくえすとのメソッド。デフォルトは `'GET'` です。
- `key`: SSLで使用する秘密鍵。デフォルトは `null` です。
- `cert`: x509公開証明書。デフォルトは `null` です。
- `ca`: リモートホストをチェックする信頼できる認証局または認証局の配列。

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




