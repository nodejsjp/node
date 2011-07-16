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

-->
* `key` : PEM でエンコードされた秘密鍵を保持する文字列
* `cert` : PEM でエンコードされた証明書を保持する文字列
* `ca` : 信頼できる認証局の証明書が PEM でエンコードされた文字列または文字列の配列

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

### hash.update(data)

<!--

Updates the hash content with the given `data`.
This can be called many times with new data as it is streamed.

-->
与えられた `data` でハッシュの内容を更新します。
これは新しいデータがストリームに流される際に何度も呼び出されます。

### hash.digest(encoding='binary')

<!--

Calculates the digest of all of the passed data to be hashed.
The `encoding` can be `'hex'`, `'binary'` or `'base64'`.

-->
渡された全てのデータがハッシュ化されたダイジェストを計算します。
`encoding` は `'hex'`、`'binary'`、または `'base64'` のいずれかです。


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

### hmac.digest(encoding='binary')

<!--

Calculates the digest of all of the passed data to the hmac.
The `encoding` can be `'hex'`, `'binary'` or `'base64'`.

-->
渡された全てのデータが HMAC 化されたダイジェストを計算します。
`encoding` は `'hex'`、`'binary'`、または `'base64'` のいずれかです。


### crypto.createCipher(algorithm, key)

<!--

Creates and returns a cipher object, with the given algorithm and key.

-->
与えられたアルゴリズムとキーを使用する暗号オブジェクトを作成して返します。

<!--

`algorithm` is dependent on OpenSSL, examples are `'aes192'`, etc.
On recent releases, `openssl list-cipher-algorithms` will display the available cipher algorithms.

-->
`algorithm` は、OpenSSL に依存します。例えば `'aes192'` などです。
最近のリリースでは、`openssl list-cipher-algorithms`
で利用可能な暗号アルゴリズムが表示されます。

### cipher.update(data, input_encoding='binary', output_encoding='binary')

<!--

Updates the cipher with `data`, the encoding of which is given in `input_encoding`
and can be `'utf8'`, `'ascii'` or `'binary'`. The `output_encoding` specifies
the output format of the enciphered data, and can be `'binary'`, `'base64'` or `'hex'`.

-->
`data` で暗号を更新します。
`input_encoding` で与えられるエンコーディングは `'utf8'`、`'ascii'`、`'binary'` のいずれかです。
`output_encoding` は暗号化されたデータの出力フォーマットを指定するもので、
`'utf8'`、`'ascii'` または `'binary'` のいずれかです。

<!--

Returns the enciphered contents, and can be called many times with new data as it is streamed.

-->
暗号化されたコンテンツが返されます。これは新しいデータがストリームに流される際に何度も呼び出されます。

### cipher.final(output_encoding='binary')

<!--

Returns any remaining enciphered contents, with `output_encoding` being one of: `'binary'`, `'base64'` or `'hex'`.

-->
暗号化されたコンテンツの残りを返します。
`output_encoding` は次のいずれかです: `'binary'`、`'base64'` または `'hex'`

### crypto.createDecipher(algorithm, key)

<!--

Creates and returns a decipher object, with the given algorithm and key.
This is the mirror of the cipher object above.

-->
与えられたアルゴリズムとキーを使用する復号オブジェクトを作成して返します。
これは前述の暗号オブジェクトの鏡写しです。

### decipher.update(data, input_encoding='binary', output_encoding='binary')

<!--

Updates the decipher with `data`, which is encoded in `'binary'`, `'base64'` or `'hex'`.
The `output_decoding` specifies in what format to return the deciphered plaintext: `'binary'`, `'ascii'` or `'utf8'`.

-->
`'binary'`、`'base64'` または `'hex'` のいずれかでエンコードされた復号を `data` で更新します。
`output_decoding` は復号化されたプレーンテキストのフォーマットを指定するもので、
`'binary'`、`'ascii'` あるいは `'utf8'` のいずれかです。

### decipher.final(output_encoding='binary')

<!--

Returns any remaining plaintext which is deciphered,
with `output_encoding` being one of: `'binary'`, `'ascii'` or `'utf8'`.

-->
復号化されたプレーンテキストの残りを返します。
`output_decoding` は `'binary'`、`'ascii'` あるいは `'utf8'` のいずれかです。


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

### signer.sign(private_key, output_format='binary')

<!--

Calculates the signature on all the updated data passed through the signer.
`private_key` is a string containing the PEM encoded private key for signing.

-->
署名オブジェクトに渡された全ての更新データで署名を計算します。
`private_key` は PEM でエンコードされた秘密鍵を内容とする文字列です。

<!--

Returns the signature in `output_format` which can be `'binary'`, `'hex'` or `'base64'`.

-->
`'binary'`、`'hex'`、あるいは `'base64'` のいずれかを指定した `output_format` による署名を返します。

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

### verifier.verify(object, signature, signature_format='binary')

<!--

Verifies the signed data by using the `object` and `signature`. `object` is  a
string containing a PEM encoded object, which can be one of RSA public key,
DSA public key, or X.509 certificate. `signature` is the previously calculated
signature for the data, in the `signature_format` which can be `'binary'`,
`'hex'` or `'base64'`.

-->
署名されたデータを `object` と `signature` で検証します。
`object` は RSA 公開鍵、DSA 公開鍵、X.509証明書のいずれかを
PEM でエンコードしたオブジェクトです。
`signature` は先に計算したデータの署名で、
その `signature_format` は `'binary'`、`'hex'`、または `'base64'` のいずれかです。

<!--

Returns true or false depending on the validity of the signature for the data and public key.

-->
署名されたデータと公開鍵による検証の結果によって true または false を返します。

### crypto.createDiffieHellman(prime_length)

<!--

Creates a Diffie-Hellman key exchange object and generates a prime of the
given bit length. The generator used is `2`.

-->
ディフィー・ヘルマン鍵共有オブジェクトを作成し、
与えられた長さの素数を生成します。生成元は `2` です。

### crypto.createDiffieHellman(prime, encoding='binary')

<!--
Creates a Diffie-Hellman key exchange object using the supplied prime. The
generator used is `2`. Encoding can be `'binary'`, `'hex'`, or `'base64'`.
-->
与えられた素数からディフィー・ヘルマン鍵共有オブジェクトを作成します。
生成元は `2` です。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。

### diffieHellman.generateKeys(encoding='binary')

<!--
Generates private and public Diffie-Hellman key values, and returns the
public key in the specified encoding. This key should be transferred to the
other party. Encoding can be `'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法で秘密および公開鍵を作成し、
指定の方法でエンコーディングされた公開鍵を返します。
この鍵は相手側に渡されるものです。

### diffieHellman.computeSecret(other_public_key, input_encoding='binary', output_encoding=input_encoding)

<!--
Computes the shared secret using `other_public_key` as the other party's
public key and returns the computed shared secret. Supplied key is
interpreted using specified `input_encoding`, and secret is encoded using
specified `output_encoding`. Encodings can be `'binary'`, `'hex'`, or
`'base64'`. If no output encoding is given, the input encoding is used as
output encoding.
-->
`other_public_key` を相手側の公開鍵として共有の秘密鍵を計算して返します。
与えられた公開鍵は指定の `input_encoding` を使って解釈され、
秘密鍵は `output_encoding` で指定された方法でエンコードされます。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
出力のエンコーディングが与えられなかった場合は、入力のエンコーディングが
出力エンコーディングとして使われます。

### diffieHellman.getPrime(encoding='binary')

<!--
Returns the Diffie-Hellman prime in the specified encoding, which can be
`'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法の素数を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。

### diffieHellman.getGenerator(encoding='binary')

<!--
Returns the Diffie-Hellman prime in the specified encoding, which can be
`'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法の生成元を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。

### diffieHellman.getPublicKey(encoding='binary')

<!--
Returns the Diffie-Hellman public key in the specified encoding, which can
be `'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法による公開鍵を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。

### diffieHellman.getPrivateKey(encoding='binary')

<!--
Returns the Diffie-Hellman private key in the specified encoding, which can
be `'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法による秘密鍵を指定のエンコーディングで返します。
エンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。

### diffieHellman.setPublicKey(public_key, encoding='binary')

<!--
Sets the Diffie-Hellman public key. Key encoding can be `'binary'`, `'hex'`,
or `'base64'`.
-->
ディフィー・ヘルマン法による公開鍵を設定します。
鍵のエンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。


### diffieHellman.setPrivateKey(public_key, encoding='binary')

<!--
Sets the Diffie-Hellman private key. Key encoding can be `'binary'`, `'hex'`, or `'base64'`.
-->
ディフィー・ヘルマン法による秘密鍵を設定します。
鍵のエンコーディングは `'binary'`、`'hex'`、または `'base64'` のいずれかです。
