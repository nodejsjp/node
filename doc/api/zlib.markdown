## Zlib

<!--
You can access this module with:
-->
このモジュールは次のようにアクセスできます。

    var zlib = require('zlib');
    var gzip = zlib.createGzip();
    var fs = require('fs');
    var inp = fs.createReadStream('input.txt');
    var out = fs.createWriteStream('input.txt.gz');

    inp.pipe(gzip).pipe(out);

<!--
This provides bindings to Gzip/Gunzip, Deflate/Inflate, and
DeflateRaw/InflateRaw classes.  Each class takes the same options, and
is a readable/writable Stream.
-->
これは Gzip/Gunzip、Deflate/Inflate、そしてDeflateRaw/InflateRaw へバインディングするクラスを提供します。各クラスは同じオプションを持ち、読み込みと書き込みが可能なストリームです。

### Constants

<!--
All of the constants defined in zlib.h are also defined on
`require('zlib')`.  They are described in more detail in the zlib
documentation.  See <http://zlib.net/manual.html#Constants>
for more details.
-->
zlib.ht に定義された定数は `require('zlib')` でも定義されます。
それらは zlib のドキュメントでより詳しく説明されます。
詳細は <http://zlib.net/manual.html#Constants> を参照してください。

### zlib.Gzip

<!--
Compress data using gzip.
-->
gzip を使ってデータを圧縮します。

### zlib.Gunzip

<!--
Decompress a gzip stream.
-->
gzip ストリームを解凍します。

### zlib.Deflate

<!--
Compress data using deflate.
-->
deflate を使ってデータを圧縮します。

### zlib.Inflate

<!--
Decompress a deflate stream.
-->
deflate ストリームを解凍します。

### zlib.DeflateRaw

<!--
Compress data using deflate, and do not append a zlib header.
-->
deflate を使ってデータを圧縮しますが、zlib ヘッダを付加しません。

### zlib.InflateRaw

<!--
Decompress a raw deflate stream.
-->
生の deflate ストリームを解凍します。

### zlib.Unzip

<!--
Decompress either a Gzip- or Deflate-compressed stream by auto-detecting
the header.
-->
Gzip または Deflate で圧縮されたストリームをヘッダで自動判別して解凍します。

### Options

<!--
Each class takes an options object.  All options are optional.

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
全てのオプションは任意です。

いくつかのオプションは圧縮にだけ関連し、
解凍するクラスでは無視されることに注意してください。

* chunkSize (デフォルト: 16*1024)
* windowBits
* level (圧縮のみ)
* memLevel (圧縮のみ)
* strategy (圧縮のみ)

これらの詳細は <http://zlib.net/manual.html#Advanced> の
`deflateInit2` および `inflateInit2` の説明を参照してください。

### Memory Usage Tuning

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
-->
この場合、windowBits=15 (デフォルト値) の場合 32K に加えて数キロバイトが
小さなオブジェクトのために使われます。

これは、デフォルト値 16K の `chunkSize` で指定されたサイズの内部バッファに加えられます。
