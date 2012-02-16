## Crypto

<!--

Use `require('crypto')` to access this module.

-->
このモジュールにアクセスするには `require('crypto')` を使用します。

<!--

The crypto module requires OpenSSL to be available on the underlying platform.
It offers a way of encapsulating secure credentials to be used as part
of a secure HTTPS net or http connection.

-->
暗号化モジュールは下層のプラットフォームで OpenSSL が有効であることを必要とします。
それは安全な HTTPS ネットワークや http コネクションの一部として使われる、
安全な認証情報をカプセル化する方法を提供します。

<!--

It also offers a set of wrappers for OpenSSL's hash, hmac, cipher, decipher, sign and verify methods.

-->
同時に OpenSSL のハッシュ、HMAC、暗号、復号、署名、そして検証へのラッパーを一式提供します。

### crypto.createCredentials(details)

<!--

Creates a credentials object, with the optional details being a dictionary with keys:

-->
認証情報オブジェクトを作成します。オプションの `details` は以下のキーを持つ辞書です:

<!--

* `key` : a string holding the PEM encoded private key
* `cert` : a string holding the PEM encoded certificate
* `ca` : either a string or list of strings of PEM encoded CA certificates to trust.
* `ciphers`: a string describing the ciphers to use or exclude. Consult
  <http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT> for details
  on the format.

-->
* `key` : PEM でエンコードされた秘密鍵を保持する文字列
* `cert` : PEM でエンコードされた証明書を保持する文字列
* `ca` : 信頼できる認証局の証明書が PEM でエンコードされた文字列または文字列の配列
* `ciphers`: 使用または除外する暗号を記述した文字列。
  詳細は <http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT>
  を参照してください。

<!--

If no 'ca' details are given, then node.js will use the default publicly trusted list of CAs as given in
<http://mxr.mozilla.org/mozilla/source/security/nss/lib/ckfw/builtins/certdata.txt>.

-->
'ca' の詳細が与えられなかった場合、node.js はデフォルトとして
<http://mxr.mozilla.org/mozilla/source/security/nss/lib/ckfw/builtins/certdata.txt>
で与えられる、信頼できる認証局の公開されたリストを使用します。


### crypto.createHash(algorithm)

<!--

Creates and returns a hash object, a cryptographic hash with the given algorithm
which can be used to generate hash digests.

-->
ハッシュオブジェクトを生成して返します。
与えられたアルゴリズムによる暗号ハッシュ関数はダイジェストの生成に使われます。

<!--

`algorithm` is dependent on the available algorithms supported by the version
of OpenSSL on the platform. Examples are `'sha1'`, `'md5'`, `'sha256'`, `'sha512'`, etc.
On recent releases, `openssl list-message-digest-algorithms` will display the available digest algorithms.

-->
`algorithm` は、プラットフォーム上の OpenSSL 
のバージョンでサポートされている利用可能なアルゴリズムに依存します。
例えば `'sha1'`、`'md5'`、`'sha256'`、`'sha512'`、などです。
最近のリリースでは、`openssl list-message-digest-algorithms`
で利用可能なダイジェストアルゴリズムが表示されます。

<!--

Example: this program that takes the sha1 sum of a file

-->
例: このプログラムはファイルのsha1ハッシュ値を求めます。

    var filename = process.argv[2];
    var crypto = require('crypto');
    var fs = require('fs');

    var shasum = crypto.createHash('sha1');

    var s = fs.ReadStream(filename);
    s.on('data', function(d) {
      shasum.update(d);
    });

    s.on('end', function() {
      var d = shasum.digest('hex');
      console.log(d + '  ' + filename);
    });

### hash.update(data, [input_encoding])

<!--

Updates the hash content with the given `data`, the encoding of which is given
in `input_encoding` and can be `'utf8'`, `'ascii'` or `'binary'`.
Defaults to `'binary'`.
This can be called many times with new data as it is streamed.

-->
与えられた `data` でハッシュの内容を更新します。
そのエンコーディングは `input_encoding` で与えられ、`'utf8'`、`'ascii'`、
または `'binary'` を指定することができます。
デフォルトは `'binary'` です。
これは新しいデータがストリームに流される際に何度も呼び出されます。

### hash.digest([encoding])

<!--

Calculates the digest of all of the passed data to be hashed.
The `encoding` can be `'hex'`, `'binary'` or `'base64'`.
Defaults to `'binary'`.

Note: `hash` object can not be used after `digest()` method been called.

-->
渡された全てのデータがハッシュ化されたダイジェストを計算します。
`encoding` は `'hex'`、`'binary'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。

注意: `digest()` メソッドを呼び出した後で `hash` 
オブジェクトを使うことはできません。


### crypto.createHmac(algorithm, key)

<!--

Creates and returns a hmac object, a cryptographic hmac with the given algorithm and key.

-->
与えられたアルゴリズムとキーで HMAC を計算する、HMAC オブジェクトを作成して返します。

<!--

`algorithm` is dependent on the available algorithms supported by OpenSSL - see createHash above.
`key` is the hmac key to be used.

-->
`algorithm` は OpenSSL でサポートされているアルゴリズムに依存します － 
前述の `createHash` を参照してください。

### hmac.update(data)

<!--

Update the hmac content with the given `data`.
This can be called many times with new data as it is streamed.

-->
与えられた `data` で HMAC の内容を更新します。
これは新しいデータがストリームに流される際に何度も呼び出されます。

### hmac.digest([encoding])

<!--

Calculates the digest of all of the passed data to the hmac.
The `encoding` can be `'hex'`, `'binary'` or `'base64'`.
Defaults to `'binary'`.

Note: `hmac` object can not be used after `digest()` method been called.

-->
渡された全てのデータが HMAC 化されたダイジェストを計算します。
`encoding` は `'hex'`、`'binary'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。

注意: `digest()` メソッドを呼び出した後で `hmac` 
オブジェクトを使うことはできません。


### crypto.createCipher(algorithm, password)

<!--

Creates and returns a cipher object, with the given algorithm and password.

`algorithm` is dependent on OpenSSL, examples are `'aes192'`, etc.
On recent releases, `openssl list-cipher-algorithms` will display the
available cipher algorithms.
`password` is used to derive key and IV, which must be `'binary'` encoded
string (See the [Buffers](buffers.html) for more information).

-->
与えられたアルゴリズムとパスワードを使用する暗号オブジェクトを作成して返します。
`algorithm` は、OpenSSL に依存します。例えば `'aes192'` などです。
最近のリリースでは、`openssl list-cipher-algorithms`
で利用可能な暗号アルゴリズムが表示されます。
`password` はキーと IV の生成に使用されます。
これは `'binary'` でエンコードされた文字列でなければなりません
(より詳細は [Buffers](buffers.html) を参照してください)。

### crypto.createCipheriv(algorithm, key, iv)

<!--

Creates and returns a cipher object, with the given algorithm, key and iv.

`algorithm` is the same as the `createCipher()`. `key` is a raw key used in
algorithm. `iv` is an Initialization vector. `key` and `iv` must be `'binary'`
encoded string (See the [Buffers](buffers.html) for more information).

-->
与えられたアルゴリズムとキーおよび IV を使用する暗号オブジェクトを作成して返します。
`algorithm` は `createCSipher()` と同じです。
`key` はアルゴリズムで使用される生のキーです。 `iv` は初期化ベクトルです。
`key` と `iv` は `'binary'` でエンコードされた文字列でなければなりません
(より詳細は [Buffers](buffers.html) を参照してください)。

### cipher.update(data, [input_encoding], [output_encoding])

<!--

Updates the cipher with `data`, the encoding of which is given in
`input_encoding` and can be `'utf8'`, `'ascii'` or `'binary'`.
Defaults to `'binary'`.

The `output_encoding` specifies the output format of the enciphered data,
and can be `'binary'`, `'base64'` or `'hex'`. Defaults to `'binary'`.

-->
`data` で暗号を更新します。
`input_encoding` で与えられるエンコーディングは `'utf8'`、`'ascii'`、`'binary'` のいずれかです。
デフォルトは `'binary'` です。

`output_encoding` は暗号化されたデータの出力フォーマットを指定するもので、
`'utf8'`、`'ascii'` または `'binary'` のいずれかです。
デフォルトは `'binary'` です。

<!--

Returns the enciphered contents, and can be called many times with new data as it is streamed.

-->
暗号化されたコンテンツが返されます。これは新しいデータがストリームに流される際に何度も呼び出されます。

### cipher.final([output_encoding])

<!--

Returns any remaining enciphered contents, with `output_encoding` being one of:
`'binary'`, `'base64'` or `'hex'`. Defaults to `'binary'`.

Note: `cipher` object can not be used after `final()` method been called.

-->
暗号化されたコンテンツの残りを返します。
`output_encoding` は次のいずれかです: `'binary'`、`'base64'` または `'hex'`。
デフォルトは `'binary'` です。

注意: `final()` メソッドを呼び出した後で `cipher` 
オブジェクトを使うことはできません。

### cipher.setAutoPadding(auto_padding=true)

<!--
You can disable automatic padding of the input data to block size. If `auto_padding` is false,
the length of the entire input data must be a multiple of the cipher's block size or `final` will fail.
Useful for non-standard padding, e.g. using `0x0` instead of PKCS padding. You must call this before `cipher.final`.
-->
入力データが自動的にブロックサイズにパディングされることを抑止することができます。
`auto_padding` が `false` の場合、入力データ全体の長さは
暗号ブロックサイズの倍数でなければなりません。
でなければ、`final()` は失敗します。
これは非標準のパディング、たとえば PKCS パディングの代わりに
`0x0` を使う場合に便利です。
`cipher.final()` の前に呼び出す必要があります。


### crypto.createDecipher(algorithm, password)

<!--

Creates and returns a decipher object, with the given algorithm and key.
This is the mirror of the [createCipher()](#crypto.createCipher) above.

-->
与えられたアルゴリズムとパスワードを使用する復号オブジェクトを作成して返します。
これは前述の [createCipher()](#crypto.createCipher) の鏡写しです。

### crypto.createDecipheriv(algorithm, key, iv)

<!--

Creates and returns a decipher object, with the given algorithm, key and iv.
This is the mirror of the [createCipheriv()](#crypto.createCipheriv) above.

-->
与えられたアルゴリズムとキー、IV を使用する復号オブジェクトを作成して返します。
これは前述の [createCipheriv()](#crypto.createCipheriv) の鏡写しです。


### decipher.update(data, [input_encoding], [output_encoding])

<!--

Updates the decipher with `data`, which is encoded in `'binary'`, `'base64'`
or `'hex'`. Defaults to `'binary'`.

The `output_decoding` specifies in what format to return the deciphered
plaintext: `'binary'`, `'ascii'` or `'utf8'`. Defaults to `'binary'`.

-->
`'binary'`、`'base64'` または `'hex'` のいずれかでエンコードされた復号を
`data` で更新します。デフォルトは `'binary'` です。

`output_decoding` は復号化されたプレーンテキストのフォーマットを指定するもので、
`'binary'`、`'ascii'` あるいは `'utf8'` のいずれかです。
デフォルトは `'binary'` です。


### decipher.final([output_encoding])

<!--

Returns any remaining plaintext which is deciphered,
with `output_encoding` being one of: `'binary'`, `'ascii'` or `'utf8'`.
Defaults to `'binary'`.

Note: `decipher` object can not be used after `final()` method been called.
-->
復号化されたプレーンテキストの残りを返します。
`output_decoding` は `'binary'`、`'ascii'` あるいは `'utf8'` のいずれかです。
デフォルトは `'binary'` です。

注意: `final()` メソッドを呼び出した後で `decipher` 
オブジェクトを使うことはできません。

### decipher.setAutoPadding(auto_padding=true)

<!--
You can disable auto padding if the data has been encrypted without standard block padding to prevent
`decipher.final` from checking and removing it. Can only work if the input data's length is a multiple of the
ciphers block size. You must call this before streaming data to `decipher.update`.
-->
データブロックが非標準のパディングで暗号化されている場合、
`decipher.final()` によるチェックを無効にすることができます。
入力データの長さが暗号ブロックサイズの倍数の場合のみ動作します。
`decipher.update()` の前に呼び出す必要があります。


### crypto.createSign(algorithm)

<!--

Creates and returns a signing object, with the given algorithm.
On recent OpenSSL releases, `openssl list-public-key-algorithms` will display
the available signing algorithms. Examples are `'RSA-SHA256'`.

-->
与えられたアルゴリズムで署名オブジェクトを作成して返します。
最近のOpenSSLのリリースでは、`openssl list-public-key-algorithms`
で利用可能な署名アルゴリズムの一覧が表示されます。例えば 'RSA-SHA256'。

### signer.update(data)

<!--

Updates the signer object with data.
This can be called many times with new data as it is streamed.

-->
署名オブジェクトをデータで更新します。
これは新しいデータがストリームに流される際に何度も呼び出されます。


### signer.sign(private_key, [output_format])

<!--

Calculates the signature on all the updated data passed through the signer.
`private_key` is a string containing the PEM encoded private key for signing.

-->
署名オブジェクトに渡された全ての更新データで署名を計算します。
`private_key` は PEM でエンコードされた秘密鍵を内容とする文字列です。

<!--

Returns the signature in `output_format` which can be `'binary'`, `'hex'` or
`'base64'`. Defaults to `'binary'`.

Note: `signer` object can not be used after `sign()` method been called.

-->
`'binary'`、`'hex'`、あるいは `'base64'` のいずれかを指定した `output_format`
による署名を返します。デフォルトは `'binary'` です。

注意: `sign()` メソッドを呼び出した後で `signer` 
オブジェクトを使うことはできません。


### crypto.createVerify(algorithm)

<!--

Creates and returns a verification object, with the given algorithm.
This is the mirror of the signing object above.

-->
与えられたアルゴリズムで検証オブジェクトを作成して返します。これは前述の署名オブジェクトと鏡写しです。

### verifier.update(data)

<!--

Updates the verifier object with data.
This can be called many times with new data as it is streamed.

-->
検証オブジェクトをデータで更新します。
これは新しいデータがストリームに流される際に何度も呼び出されます。


### verifier.verify(object, signature, [signature_format])

<!--

Verifies the signed data by using the `object` and `signature`. `object` is  a
string containing a PEM encoded object, which can be one of RSA public key,
DSA public key, or X.509 certificate. `signature` is the previously calculated
signature for the data, in the `signature_format` which can be `'binary'`,
`'hex'` or `'base64'`. Defaults to `'binary'`.

-->
署名されたデータを `object` と `signature` で検証します。
`object` は RSA 公開鍵、DSA 公開鍵、X.509証明書のいずれかを
PEM でエンコードしたオブジェクトです。
`signature` は先に計算したデータの署名で、
その `signature_format` は `'binary'`、`'hex'`、または `'base64'`
のいずれかです。デフォルトは `'binary'` です。

<!--

Returns true or false depending on the validity of the signature for the data and public key.

Note: `verifier` object can not be used after `verify()` method been called.

-->
署名されたデータと公開鍵による検証の結果によって true または false を返します。

注意: `verify()` メソッドを呼び出した後で `verifier` 
オブジェクトを使うことはできません。


### crypto.createDiffieHellman(prime_length)

<!--

Creates a Diffie-Hellman key exchange object and generates a prime of the
given bit length. The generator used is `2`.

-->
ディフィー・ヘルマン鍵共有オブジェクトを作成し、
与えられた長さの素数を生成します。生成元は `2` です。


### crypto.createDiffieHellman(prime, [encoding])

<!--
Creates a Diffie-Hellman key exchange object using the supplied prime. The
generator used is `2`. Encoding can be `'binary'`, `'hex'`, or `'base64'`.
Defaults to `'binary'`.
-->
与えられた素数からディフィー・ヘルマン鍵共有オブジェクトを作成します。
生成元は `2` です。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.generateKeys([encoding])

<!--
Generates private and public Diffie-Hellman key values, and returns the
public key in the specified encoding. This key should be transferred to the
other party. Encoding can be `'binary'`, `'hex'`, or `'base64'`.
Defaults to `'binary'`.
-->
ディフィー・ヘルマン法で秘密および公開鍵を作成し、
指定の方法でエンコーディングされた公開鍵を返します。
この鍵は相手側に渡されるものです。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.computeSecret(other_public_key, [input_encoding], [output_encoding])

<!--
Computes the shared secret using `other_public_key` as the other party's
public key and returns the computed shared secret. Supplied key is
interpreted using specified `input_encoding`, and secret is encoded using
specified `output_encoding`. Encodings can be `'binary'`, `'hex'`, or
`'base64'`. The input encoding defaults to `'binary'`.
If no output encoding is given, the input encoding is used as output encoding.
-->
`other_public_key` を相手側の公開鍵として共有の秘密鍵を計算して返します。
与えられた公開鍵は指定の `input_encoding` を使って解釈され、
秘密鍵は `output_encoding` で指定された方法でエンコードされます。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
入力エンコーディングのデフォルトは `'binary'` です。
出力のエンコーディングが与えられなかった場合は、入力のエンコーディングが
出力エンコーディングとして使われます。


### diffieHellman.getPrime([encoding])

<!--
Returns the Diffie-Hellman prime in the specified encoding, which can be
`'binary'`, `'hex'`, or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法の素数を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.getGenerator([encoding])

<!--
Returns the Diffie-Hellman prime in the specified encoding, which can be
`'binary'`, `'hex'`, or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法の生成元を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.getPublicKey([encoding])

<!--
Returns the Diffie-Hellman public key in the specified encoding, which can
be `'binary'`, `'hex'`, or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法による公開鍵を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.getPrivateKey([encoding])

<!--
Returns the Diffie-Hellman private key in the specified encoding, which can
be `'binary'`, `'hex'`, or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法による秘密鍵を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.setPublicKey(public_key, [encoding])

<!--
Sets the Diffie-Hellman public key. Key encoding can be `'binary'`, `'hex'`,
or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法による公開鍵を設定します。
鍵のエンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### diffieHellman.setPrivateKey(public_key, [encoding])

<!--
Sets the Diffie-Hellman private key. Key encoding can be `'binary'`, `'hex'`,
or `'base64'`. Defaults to `'binary'`.
-->
ディフィー・ヘルマン法による秘密鍵を設定します。
鍵のエンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
デフォルトは `'binary'` です。


### pbkdf2(password, salt, iterations, keylen, callback)

<!--
Asynchronous PBKDF2 applies pseudorandom function HMAC-SHA1 to derive
a key of given length from the given password, salt and iterations.
The callback gets two arguments `(err, derivedKey)`.
-->
疑似乱数を HMAC-SHA1 関数に適用して、与えられたパスワードと salt 
(ランダムなバイト値)、および繰り返しから、指定された長さの鍵を生成する、
非同期の PBKDF2 です。
コールバック関数は二つの引数を受け取る `(err, derivedKey)` です。


### randomBytes(size, [callback])

<!--
Generates cryptographically strong pseudo-random data. Usage:
-->
暗号学的に強い疑似乱数データを生成します。使用法:

    // async
    crypto.randomBytes(256, function(ex, buf) {
      if (ex) throw ex;
      console.log('Have %d bytes of random data: %s', buf.length, buf);
    });

    // sync
    try {
      var buf = crypto.randomBytes(256);
      console.log('Have %d bytes of random data: %s', buf.length, buf);
    } catch (ex) {
      // handle error
    }
