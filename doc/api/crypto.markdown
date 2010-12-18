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
http://mxr.mozilla.org/mozilla/source/security/nss/lib/ckfw/builtins/certdata.txt
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
例えば sha1、md5、sha256、sha512、などです。
最近のリリースでは、`openssl list-message-digest-algorithms`
で利用可能なダイジェストアルゴリズムが表示されます。

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
`encoding` は 'hex'、'binary'、または 'base64' のいずれかです。


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
`encoding` は 'hex'、'binary'、または 'base64' のいずれかです。


### crypto.createCipher(algorithm, key)

<!--
Creates and returns a cipher object, with the given algorithm and key.
-->
与えられたアルゴリズムとキーを使用する暗号オブジェクトを作成して返します。

<!--
`algorithm` is dependent on OpenSSL, examples are `'aes192'`, etc.
On recent releases, `openssl list-cipher-algorithms` will display the available cipher algorithms.
-->
`algorithm` は、OpenSSL に依存します。例えば aes192 などです。
最近のリリースでは、`openssl list-cipher-algorithms`
で利用可能な暗号アルゴリズムが表示されます。

### cipher.update(data, input_encoding='binary', output_encoding='binary')

<!--
Updates the cipher with `data`, the encoding of which is given in `input_encoding`
and can be `'utf8'`, `'ascii'` or `'binary'`. The `output_encoding` specifies
the output format of the enciphered data, and can be `'binary'`, `'base64'` or `'hex'`.
-->
`data` で暗号を更新します。
`input_encoding` で与えられるエンコーディングは 'utf8'、'ascii'、'binary' のいずれかです。
`output_encoding` は暗号化されたデータの出力フォーマットを指定するもので、
'binary'、'base64' または 'hex' のいずれかです。

<!--
Returns the enciphered contents, and can be called many times with new data as it is streamed.
-->
暗号化されたコンテンツが返されます。これは新しいデータがストリームに流される際に何度も呼び出されます。

### cipher.final(output_encoding='binary')

<!--
Returns any remaining enciphered contents, with `output_encoding` being one of: `'binary'`, `'ascii'` or `'utf8'`.
-->
暗号化されたコンテンツの残りを返します。
`output_encoding` は次のいずれかです: 'binary'、'base64' または 'hex'

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
'binary'、'base64' または 'hex' のいずれかでエンコードされた復号を `data` で更新します。
`output_decoding` は復号化されたプレーンテキストのフォーマットを指定するもので、
'binary'、'ascii' あるいは 'utf8' のいずれかです。

### decipher.final(output_encoding='binary')

<!--
Returns any remaining plaintext which is deciphered,
with `output_encoding' being one of: `'binary'`, `'ascii'` or `'utf8'`.
-->
復号化されたプレーンテキストの残りを返します。
`output_decoding` は 'binary'、'ascii' あるいは 'utf8' のいずれかです。


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
'binary'、'hex'、あるいは 'base64' のいずれかを指定した `output_format` による署名を返します。

### crypto.createVerify(algorithm)

<!--
Creates and returns a verification object, with the given algorithm.
This is the mirror of the signing object above.
-->
与えられたアルゴリズムで検証オブジェクトを作成して返します。これは前述の署名オブジェクトと鏡写しです。

### verifier.update(data)

<!--
Updates the verifyer object with data.
This can be called many times with new data as it is streamed.
-->
検証オブジェクトをデータで更新します。
これは新しいデータがストリームに流される際に何度も呼び出されます。

### verifier.verify(public_key, signature, signature_format='binary')

<!--
Verifies the signed data by using the `public_key` which is a string containing
the PEM encoded public key, and `signature`, which is the previously calculates
signature for the data, in the `signature_format` which can be `'binary'`, `'hex'` or `'base64'`.
-->
署名されたデータを `public_key` と `signature` で検証します。
`public_key` は PEM でエンコードされた公開鍵を含む文字列です。
`signature` は先に計算したデータの署名で、
その `signature_format` は 'binary'、'hex'、または 'base64' のいずれかです。

<!--
Returns true or false depending on the validity of the signature for the data and public key.
-->
署名されたデータと公開鍵による検証の結果によって true または false を返します。
