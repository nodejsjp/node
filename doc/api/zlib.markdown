# Zlib

    Stability: 3 - Stable

<!--
You can access this module with:
-->

このモジュールは次のようにアクセスできます。

    var zlib = require('zlib');

<!--
This provides bindings to Gzip/Gunzip, Deflate/Inflate, and
DeflateRaw/InflateRaw classes.  Each class takes the same options, and
is a readable/writable Stream.
-->

これは Gzip/Gunzip、Deflate/Inflate、そして DeflateRaw/InflateRaw
へバインディングするクラスを提供します。
どのクラスも同じオプションを持つ、読み込みと書き込みが可能なストリームです。

## Examples

<!--
Compressing or decompressing a file can be done by piping an
fs.ReadStream into a zlib stream, then into an fs.WriteStream.
-->

ファイルを圧縮および解凍するには、fs.ReadStream から zlib へ、
そして fs.WriteStream へパイプをつなぐだけです。

    var gzip = zlib.createGzip();
    var fs = require('fs');
    var inp = fs.createReadStream('input.txt');
    var out = fs.createWriteStream('input.txt.gz');

    inp.pipe(gzip).pipe(out);

<!--
Compressing or decompressing data in one step can be done by using
the convenience methods.
-->

データの圧縮または解凍は [簡易メソッド](#convenience_Methods)
を使うことにより、ワンステップで行うことができます。

    var input = '.................................';
    zlib.deflate(input, function(err, buffer) {
      if (!err) {
        console.log(buffer.toString('base64'));
      }
    });

    var buffer = new Buffer('eJzT0yMAAGTvBe8=', 'base64');
    zlib.unzip(buffer, function(err, buffer) {
      if (!err) {
        console.log(buffer.toString());
      }
    });

<!--
To use this module in an HTTP client or server, use the
[accept-encoding](http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.3)
on requests, and the
[content-encoding](http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.11)
header on responses.

**Note: these examples are drastically simplified to show
the basic concept.**  Zlib encoding can be expensive, and the results
ought to be cached.  See [Memory Usage Tuning](#memory_Usage_Tuning)
below for more information on the speed/memory/compression
tradeoffs involved in zlib usage.
-->

このモジュールを HTTP クライアントとサーバで使うには、リクエストに
[accept-encoding](http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.3)
ヘッダを、レスポンスに
[content-encoding](http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.11)
ヘッダを使用します。

**注意:
これらのサンプルは基本コンセプトを見せるためにとても単純化されています。**
Zlib エンコーディングは高価なので、結果はキャッシュされるべきです。
zlibの使い方に関する速度／メモリ／圧縮率のトレードオフについてより詳しくは、
後述の [Memory Usage Tuning](#memory_Usage_Tuning) を参照してください。

    // client request example
    var zlib = require('zlib');
    var http = require('http');
    var fs = require('fs');
    var request = http.get({ host: 'izs.me',
                             path: '/',
                             port: 80,
                             headers: { 'accept-encoding': 'gzip,deflate' } });
    request.on('response', function(response) {
      var output = fs.createWriteStream('izs.me_index.html');

      switch (response.headers['content-encoding']) {
        // or, just use zlib.createUnzip() to handle both cases
        case 'gzip':
          response.pipe(zlib.createGunzip()).pipe(output);
          break;
        case 'deflate':
          response.pipe(zlib.createInflate()).pipe(output);
          break;
        default:
          response.pipe(output);
          break;
      }
    });

    // server example
    // Running a gzip operation on every request is quite expensive.
    // It would be much more efficient to cache the compressed buffer.
    var zlib = require('zlib');
    var http = require('http');
    var fs = require('fs');
    http.createServer(function(request, response) {
      var raw = fs.createReadStream('index.html');
      var acceptEncoding = request.headers['accept-encoding'];
      if (!acceptEncoding) {
        acceptEncoding = '';
      }

      // Note: this is not a conformant accept-encoding parser.
      // See http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.3
      if (acceptEncoding.match(/\bdeflate\b/)) {
        response.writeHead(200, { 'content-encoding': 'deflate' });
        raw.pipe(zlib.createDeflate()).pipe(response);
      } else if (acceptEncoding.match(/\bgzip\b/)) {
        response.writeHead(200, { 'content-encoding': 'gzip' });
        raw.pipe(zlib.createGzip()).pipe(response);
      } else {
        response.writeHead(200, {});
        raw.pipe(response);
      }
    }).listen(1337);

## Constants

<!--type=misc-->

<!--
All of the constants defined in zlib.h are also defined on
`require('zlib')`.  They are described in more detail in the zlib
documentation.  See <http://zlib.net/manual.html#Constants>
for more details.
-->

zlib.h に定義された定数は `require('zlib')` でも定義されます。
それらは zlib のドキュメントでより詳しく説明されます。
詳細は <http://zlib.net/manual.html#Constants> を参照してください。

## zlib.createGzip([options])

<!--
Returns a new [Gzip](#zlib.Gzip) object with an [options](#options).
-->

[options](#options) によって作られた新しい [Gzip](#zlib.Gzip)
オブジェクトを返します。

## zlib.createGunzip([options])

<!--
Returns a new [Gunzip](#zlib.Gunzip) object with an [options](#options).
-->

[options](#options) によって作られた新しい [Gunzip](#zlib.Gunzip)
オブジェクトを返します。

## zlib.createDeflate([options])

<!--
Returns a new [Deflate](#zlib.Deflate) object with an [options](#options).
-->

[options](#options) によって作られた新しい [Deflate](#zlib.Deflate)
オブジェクトを返します。

## zlib.createInflate([options])

<!--
Returns a new [Inflate](#zlib.Inflate) object with an [options](#options).
-->

[options](#options) によって作られた新しい [Inflate](#zlib.Inflate)
オブジェクトを返します。

## zlib.createDeflateRaw([options])

<!--
Returns a new [DeflateRaw](#zlib.DeflateRaw) object with an [options](#options).
-->

[options](#options) によって作られた新しい [DeflateRaw](#zlib.DeflateRaw)
オブジェクトを返します。

## zlib.createInflateRaw([options])

<!--
Returns a new [InflateRaw](#zlib.InflateRaw) object with an [options](#options).
-->

[options](#options) によって作られた新しい [InflateRaw](#zlib.InflateRaw)
オブジェクトを返します。

## zlib.createUnzip([options])

<!--
Returns a new [Unzip](#zlib.Unzip) object with an [options](#options).
-->

[options](#options) によって作られた新しい [Unzip](#zlib.Unzip)
オブジェクトを返します。


## Class: zlib.Gzip

<!--
Compress data using gzip.
-->

gzip を使ってデータを圧縮します。

## Class: zlib.Gunzip

<!--
Decompress a gzip stream.
-->

gzip ストリームを解凍します。

## Class: zlib.Deflate

<!--
Compress data using deflate.
-->

deflate を使ってデータを圧縮します。

## Class: zlib.Inflate

<!--
Decompress a deflate stream.
-->

deflate ストリームを解凍します。

## Class: zlib.DeflateRaw

<!--
Compress data using deflate, and do not append a zlib header.
-->

deflate を使ってデータを圧縮しますが、zlib ヘッダを付加しません。

## Class: zlib.InflateRaw

<!--
Decompress a raw deflate stream.
-->

生の deflate ストリームを解凍します。

## Class: zlib.Unzip

<!--
Decompress either a Gzip- or Deflate-compressed stream by auto-detecting
the header.
-->

Gzip または Deflate で圧縮されたストリームをヘッダで自動判別して解凍します。

## Convenience Methods

<!--type=misc-->

<!--
All of these take a string or buffer as the first argument, and call the
supplied callback with `callback(error, result)`.  The
compression/decompression engine is created using the default settings
in all convenience methods.  To supply different options, use the
zlib classes directly.
-->

これらは全て第 1 引数として文字列またはバッファを受け取り、
与えられたコールバック `callback(error, result)` を呼び出します。
全ての簡易メソッドで、圧縮・解凍エンジンはデフォルトの設定で作成されます。
異なったオプションを与えるには、zlib のクラスを直接使用してください。

## zlib.deflate(buf, callback)

<!--
Compress a string with Deflate.
-->

Deflate で文字列を圧縮します。

## zlib.deflateRaw(buf, callback)

<!--
Compress a string with DeflateRaw.
-->

DeflateRaw で文字列を圧縮します。

## zlib.gzip(buf, callback)

<!--
Compress a string with Gzip.
-->

Gzip で文字列を圧縮します。

## zlib.gunzip(buf, callback)

<!--
Decompress a raw Buffer with Gunzip.
-->

Gunzip で生のバッファを解凍します。

## zlib.inflate(buf, callback)

<!--
Decompress a raw Buffer with Inflate.
-->

Infrate で生のバッファを解凍します。

## zlib.inflateRaw(buf, callback)

<!--
Decompress a raw Buffer with InflateRaw.
-->

InflateRaw で生のバッファを解凍します。

## zlib.unzip(buf, callback)

<!--
Decompress a raw Buffer with Unzip.
-->

Unzip で生のバッファを解凍します。

## Options

<!--type=misc-->

<!--
Each class takes an options object.  All options are optional.  (The
convenience methods use the default settings for all options.)

Note that some options are only
relevant when compressing, and are ignored by the decompression classes.

* chunkSize (default: 16*1024)
* windowBits
* level (compression only)
* memLevel (compression only)
* strategy (compression only)

See the description of `deflateInit2` and `inflateInit2` at
<http://zlib.net/manual.html#Advanced> for more information on these.
-->

どのクラスもオプションオブジェクトを受け取ります。
全てのオプションは任意です
(簡易メソッドは全てのオプションでデフォルト値を使用します)。

いくつかのオプションは圧縮にだけ関連し、
解凍するクラスでは無視されることに注意してください。

* chunkSize (デフォルト: 16*1024)
* windowBits
* level (圧縮のみ)
* memLevel (圧縮のみ)
* strategy (圧縮のみ)

これらの詳細は <http://zlib.net/manual.html#Advanced> の
`deflateInit2` および `inflateInit2` の説明を参照してください。

## Memory Usage Tuning

<!--type=misc-->

<!--
From `zlib/zconf.h`, modified to node's usage:

The memory requirements for deflate are (in bytes):
-->

node は `zlib/zconf.h` を変更して使っています:

    (1 << (windowBits+2)) +  (1 << (memLevel+9))

<!--
that is: 128K for windowBits=15  +  128K for memLevel = 8
(default values) plus a few kilobytes for small objects.

For example, if you want to reduce
the default memory requirements from 256K to 128K, set the options to:
-->

すなわち: windowBits = 15 の場合 128K + 
memLevel = 8 の場合 128K (デフォルト値) に加えて数キロバイトが
小さなオブジェクトのために使われます。

たとえば、デフォルトで要求されるメモリを 256K から 128K へ縮小したければ、
次のオプションを設定します:

    { windowBits: 14, memLevel: 7 }

<!--
Of course this will generally degrade compression (there's no free lunch).

The memory requirements for inflate are (in bytes)
-->

もちろん、これは圧縮率を悪化します (ただ飯ははありません)。


    1 << windowBits

<!--
that is, 32K for windowBits=15 (default value) plus a few kilobytes
for small objects.

This is in addition to a single internal output slab buffer of size
`chunkSize`, which defaults to 16K.

The speed of zlib compression is affected most dramatically by the
`level` setting.  A higher level will result in better compression, but
will take longer to complete.  A lower level will result in less
compression, but will be much faster.

In general, greater memory usage options will mean that node has to make
fewer calls to zlib, since it'll be able to process more data in a
single `write` operation.  So, this is another factor that affects the
speed, at the cost of memory usage.
-->

この場合、windowBits=15 (デフォルト値) の場合 32K に加えて数キロバイトが
小さなオブジェクトのために使われます。

これは、デフォルト値 16K の `chunkSize` で指定されたサイズの内部バッファに加えられます。

zlib の圧縮速度は `level` の設定で劇的に変化します
高レベルにするとより圧縮できますが、完了までの時間が長くなります。
低レベルにするとあまり圧縮されませんが、高速になります。

一般的に、メモリをより多く使うオプションにすると node が zlib を呼び出す回数が
少なくなることを意味し、
一回の `write` 操作でより多くのデータを処理できることになります。
これはあスピードに影響するもう一つのファクタで、メモリ使用量を犠牲にします。
