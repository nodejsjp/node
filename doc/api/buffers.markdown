## Buffers

<!--

Pure Javascript is Unicode friendly but not nice to binary data.  When
dealing with TCP streams or the file system, it's necessary to handle octet
streams. Node has several strategies for manipulating, creating, and
consuming octet streams.

-->
純粋な JavaScript は Unicode と相性がいいものの、バイナリデータの扱いはうまくありません。
TCP ストリームやファイルシステムを扱う場合は、オクテットストリームを処理する必要があります。
Node にはオクテットストリームを操作、作成、消費するためにいくつかの戦略があります。

<!--

Raw data is stored in instances of the `Buffer` class. A `Buffer` is similar
to an array of integers but corresponds to a raw memory allocation outside
the V8 heap. A `Buffer` cannot be resized.

-->
生のデータは `Buffer` クラスのインスタンスに保存されます。
`Buffer` は整数の配列と似ていますが、
V8 ヒープの外部に割り当てられた生のメモリに対応します。
`Buffer` のサイズを変更することはできません。

<!--

The `Buffer` object is global.

-->
`Buffer` オブジェクトはグローバルです。

<!--

Converting between Buffers and JavaScript string objects requires an explicit encoding
method.  Here are the different string encodings;

-->
バッファを JavaScript 文字列オブジェクトとの間で変換するにはエンコーディング方式を明示する必要があります。
いくつかのエンコーディング方式があります。

<!--

* `'ascii'` - for 7 bit ASCII data only.  This encoding method is very fast, and will
strip the high bit if set.

* `'utf8'` - Multi byte encoded Unicode characters.  Many web pages and other document formats use UTF-8.

* `'ucs2'` - 2-bytes, little endian encoded Unicode characters. It can encode
only BMP(Basic Multilingual Plane, U+0000 - U+FFFF).

* `'base64'` - Base64 string encoding.

* `'binary'` - A way of encoding raw binary data into strings by using only
the first 8 bits of each character. This encoding method is depreciated and
should be avoided in favor of `Buffer` objects where possible. This encoding
will be removed in future versions of Node.

-->
* `'ascii'` - 7bit の ASCII データ専用です。
  このエンコーディング方式はとても高速で、もし上位ビットがセットされていれば取り除かれます。
* `'utf8'` - 可変長のバイト単位でエンコードされたUnicode文字。
  多くのWebページやその他のドキュメントは UTF-8 を使っています。
* `'ucs2'` - 固定長の2バイト（リトルエンディアン）でエンコードされたUnicode文字。
  BMP (基本多言語面、U+0000～U+FFFF) のみエンコードすることができます。
* `'base64'` - Base64 文字列エンコーディング.
* `'binary'` - 生のバイナリデータを各文字の最初の 8bit として使用するエンコーディング方式。
このエンコーディング方式はもはや価値がなく、`Buffer` オブジェクトでは可能な限り使用すべきではありません。
このエンコーディングは、Node の将来のバージョンで削除される予定です。


### new Buffer(size)

<!--

Allocates a new buffer of `size` octets.

-->
`size` オクテットの新しいバッファを割り当てます。

### new Buffer(array)

<!--

Allocates a new buffer using an `array` of octets.

-->
オクテットの `array` を使用する新しいバッファを割り当てます。

### new Buffer(str, encoding='utf8')

<!--

Allocates a new buffer containing the given `str`.

-->
与えられた `str` を内容とする新しいバッファを割り当てます。

### buffer.write(string, offset=0, encoding='utf8')

<!--

Writes `string` to the buffer at `offset` using the given encoding. Returns
number of octets written.  If `buffer` did not contain enough space to fit
the entire string, it will write a partial amount of the string. In the case
of `'utf8'` encoding, the method will not write partial characters.

-->
与えられたエンコーディングを使用して、`string` をバッファの `offset` から書き込みます。
書き込まれたオクテット数を返します。
もし `buffer` が文字列全体を挿入するのに十分なスペースを含んでいなければ、文字列の一部だけを書き込みます。
`'utf8'` エンコーディングの場合、このメソッドは文字の一部だけを書き込むことはありません。

<!--

Example: write a utf8 string into a buffer, then print it

-->
例: utf8 の文字列をバッファに書き込み、それをプリントします

    buf = new Buffer(256);
    len = buf.write('\u00bd + \u00bc = \u00be', 0);
    console.log(len + " bytes: " + buf.toString('utf8', 0, len));

    // 12 bytes: ½ + ¼ = ¾


### buffer.toString(encoding, start=0, end=buffer.length)

<!--

Decodes and returns a string from buffer data encoded with `encoding`
beginning at `start` and ending at `end`.

-->
`encoding` でエンコードされたバッファデータの `start` から
`end` までをデコードした文字列を返します。

<!--

See `buffer.write()` example, above.

-->
上の `buffer.write()` の例を参照してください。


### buffer[index]

<!--

Get and set the octet at `index`. The values refer to individual bytes,
so the legal range is between `0x00` and `0xFF` hex or `0` and `255`.

-->
`index` の位置のオクテットを取得および設定します。
その値は個々のバイトを参照するので、妥当な範囲は 16 進の `0x00` から `0xFF`
または `0` から`255`までの間です。

<!--

Example: copy an ASCII string into a buffer, one byte at a time:

-->
例: ASCII 文字列を 1 バイトずつバッファにコピーします

    str = "node.js";
    buf = new Buffer(str.length);

    for (var i = 0; i < str.length ; i++) {
      buf[i] = str.charCodeAt(i);
    }

    console.log(buf);

    // node.js

### Buffer.isBuffer(obj)

Tests if `obj` is a `Buffer`.

### Buffer.byteLength(string, encoding='utf8')

<!--

Gives the actual byte length of a string.  This is not the same as
`String.prototype.length` since that returns the number of *characters* in a
string.

-->
文字列の実際のバイト数を返します。
これは文字列の*文字*数を返す `String.prototype.length` と同じではありません。

<!--

Example:

-->
例:

    str = '\u00bd + \u00bc = \u00be';

    console.log(str + ": " + str.length + " characters, " +
      Buffer.byteLength(str, 'utf8') + " bytes");

    // ½ + ¼ = ¾: 9 characters, 12 bytes


### buffer.length

<!--

The size of the buffer in bytes.  Note that this is not necessarily the size
of the contents. `length` refers to the amount of memory allocated for the
buffer object.  It does not change when the contents of the buffer are changed.

-->
バイト数によるバッファのサイズ。
これは実際の内容のサイズではないことに注意してください。
`length` はバッファオブジェクトに割り当てられたメモリ全体を参照します。

    buf = new Buffer(1234);

    console.log(buf.length);
    buf.write("some string", "ascii", 0);
    console.log(buf.length);

    // 1234
    // 1234

### buffer.copy(targetBuffer, targetStart=0, sourceStart=0, sourceEnd=buffer.length)

<!--

Does a memcpy() between buffers.

-->
バッファ間で memcpy() をします。

<!--

Example: build two Buffers, then copy `buf1` from byte 16 through byte 19
into `buf2`, starting at the 8th byte in `buf2`.

-->
例: バッファを2個作成し、`buf1` の 16 バイト目から 19 バイト目を、
`buf2` の 8 バイト目から始まる位置へコピーします。

    buf1 = new Buffer(26);
    buf2 = new Buffer(26);

    for (var i = 0 ; i < 26 ; i++) {
      buf1[i] = i + 97; // 97 is ASCII a
      buf2[i] = 33; // ASCII !
    }

    buf1.copy(buf2, 8, 16, 20);
    console.log(buf2.toString('ascii', 0, 25));

    // !!!!!!!!qrst!!!!!!!!!!!!!


### buffer.slice(start, end=buffer.length)

<!--

Returns a new buffer which references the
same memory as the old, but offset and cropped by the `start` and `end`
indexes.

-->
元のバッファと同じメモリを参照しますが、`start` と `end`
で示されるオフセットで始まり短くされた新しいバッファを返します。

<!--

**Modifying the new buffer slice will modify memory in the original buffer!**

-->
**新しいバッファスライスの変更は、オリジナルバッファのメモリを変更することになります！**

<!--

Example: build a Buffer with the ASCII alphabet, take a slice, then modify one byte
from the original Buffer.

-->
例: ASCII のアルファベットでバッファを構築してスライスし、元のバッファで 1 バイトを変更します。

    var buf1 = new Buffer(26);

    for (var i = 0 ; i < 26 ; i++) {
      buf1[i] = i + 97; // 97 is ASCII a
    }

    var buf2 = buf1.slice(0, 3);
    console.log(buf2.toString('ascii', 0, buf2.length));
    buf1[0] = 33;
    console.log(buf2.toString('ascii', 0, buf2.length));

    // abc
    // !bc
