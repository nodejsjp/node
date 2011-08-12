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
Note that this encoding converts a null character (`'\0'` or `'\u0000'`) into
`0x20` (character code of a space). If you want to convert a null character
into `0x00`, you should use `'utf8'`.

* `'utf8'` - Multi byte encoded Unicode characters.  Many web pages and other document formats use UTF-8.

* `'ucs2'` - 2-bytes, little endian encoded Unicode characters. It can encode
only BMP(Basic Multilingual Plane, U+0000 - U+FFFF).

* `'base64'` - Base64 string encoding.

* `'binary'` - A way of encoding raw binary data into strings by using only
the first 8 bits of each character. This encoding method is deprecated and
should be avoided in favor of `Buffer` objects where possible. This encoding
will be removed in future versions of Node.

* `'hex'` - Encode each byte as two hexidecimal characters.

-->
* `'ascii'` - 7bit の ASCII データ専用です。
  このエンコーディング方式はとても高速で、もし上位ビットがセットされていれば取り除かれます。
このエンコーディングは、null 文字 (`'\0'` または `'\u0000'`) を `0x20`
(スペースの文字コード) に変換することに注意してください。
null 文字を 0x00 に変換したい場合は `'utf8'` を使用してください。
* `'utf8'` - 可変長のバイト単位でエンコードされたUnicode文字。
  多くのWebページやその他のドキュメントは UTF-8 を使っています。
* `'ucs2'` - 固定長の2バイト（リトルエンディアン）でエンコードされたUnicode文字。
  BMP (基本多言語面、U+0000～U+FFFF) のみエンコードすることができます。
* `'base64'` - Base64 文字列エンコーディング.
* `'binary'` - 生のバイナリデータを各文字の最初の 8bit として使用するエンコーディング方式。
このエンコーディング方式はもはや価値がなく、`Buffer` オブジェクトでは可能な限り使用すべきではありません。
このエンコーディングは、Node の将来のバージョンで削除される予定です。
* `'hex'` - 各バイトを 2 桁の16進数文字列でエンコードします。

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

### buffer.write(string, offset=0, length=buffer.length-offset, encoding='utf8')
<!--

Writes `string` to the buffer at `offset` using the given encoding. `length` is
the number of bytes to write. Returns number of octets written. If `buffer` did
not contain enough space to fit the entire string, it will write a partial
amount of the string. The method will not write partial characters.

-->
与えられたエンコーディングを使用して、`string` をバッファの `offset` から書き込みます。
`length` は書き込むバイト数です。書き込まれたオクテット数を返します。
もし `buffer` が文字列全体を挿入するのに十分なスペースを含んでいなければ、文字列の一部だけを書き込みます。
このメソッドは文字の一部だけを書き込むことはありません。

<!--

Example: write a utf8 string into a buffer, then print it

-->
例: utf8 の文字列をバッファに書き込み、それをプリントします

    buf = new Buffer(256);
    len = buf.write('\u00bd + \u00bc = \u00be', 0);
    console.log(len + " bytes: " + buf.toString('utf8', 0, len));

<!--

The number of characters written (which may be different than the number of
bytes written) is set in `Buffer._charsWritten` and will be overwritten the
next time `buf.write()` is called.

-->
書き込まれた文字数 (書き込まれたバイト数とは異なる場合があります) は、
次に `buf.write()` が呼び出されて上書きされるまで
`Buffer._charsWritten` に設定されています。

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

### buffer.readUInt8(offset, bigEndian)

<!--
Reads an unsigned 8 bit integer from the buffer at the specified offset. If
`bigEndian` is true, reads bytes in a big endian format, otherwise reads them as
little endian.
-->
バッファの指定された位置を符号無し 8bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイトを読み込みます。

<!--
Example:
-->
例:

    var buf = new Buffer(4);

    buf[0] = 0x3;
    buf[1] = 0x4;
    buf[2] = 0x23;
    buf[3] = 0x42;

    for (ii = 0; ii < buf.length; ii++) {
      console.log(buf.readUInt8(ii, true));
      console.log(buf.readUInt8(ii, false));
    }

    // 0x3
    // 0x3
    // 0x4
    // 0x4
    // 0x23
    // 0x23
    // 0x42
    // 0x42

### buffer.readUInt16(offset, bigEndian)

<!--
Reads an unsigned 16 bit integer from the buffer at the specified offset. If
`bigEndian` is true, reads bytes in a big endian format, otherwise reads them as
little endian.
-->
バッファの指定された位置を符号無し 16bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。

<!--
Example:
-->
例:

    var buf = new Buffer(4);

    buf[0] = 0x3;
    buf[1] = 0x4;
    buf[2] = 0x23;
    buf[3] = 0x42;

    console.log(buf.readUInt16(0, true));
    console.log(buf.readUInt16(0, false));
    console.log(buf.readUInt16(1, true));
    console.log(buf.readUInt16(1, false));
    console.log(buf.readUInt16(2, true));
    console.log(buf.readUInt16(2, false));

    // 0x0304
    // 0x0403
    // 0x0423
    // 0x2304
    // 0x2342
    // 0x4223

### buffer.readUInt32(offset, bigEndian)

<!--
Reads an unsigned 32 bit integer from the buffer at the specified offset. If
`bigEndian` is true, reads bytes in a big endian format, otherwise reads them as
little endian.
-->
バッファの指定された位置を符号無し 32bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。


<!--
Example:
-->
例:

    var buf = new Buffer(4);

    buf[0] = 0x3;
    buf[1] = 0x4;
    buf[2] = 0x23;
    buf[3] = 0x42;

    console.log(buf.readUInt32(0, true));
    console.log(buf.readUInt32(0, false));

    // 0x03042342
    // 0x42230403

### buffer.readInt8(offset, bigEndian)

<!--
Reads a signed 8 bit integer from the buffer at the specified offset. Endian
must be either 'big' or 'little' and specifies what endian ordering to read the
bytes from the buffer in.
-->
バッファの指定された位置を符号付き 8bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイトを読み込みます。

<!--
Works as `buffer.readUInt8`, except buffer contents are treated as twos
complement signed values.
-->
バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt8` と同じように動作します。

### buffer.readInt16(offset, bigEndian)

<!--
Reads a signed 16 bit integer from the buffer at the specified offset. If
`bigEndian` is true, reads bytes in a big endian format, otherwise reads them as
little endian.
-->
バッファの指定された位置を符号付き 16bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。

<!--
Works as `buffer.readUInt16`, except buffer contents are treated as twos
complement signed values.
-->
バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt16` と同じように動作します。


### buffer.readInt32(offset, bigEndian)

<!--
Reads a signed 32 bit integer from the buffer at the specified offset. If
`bigEndian` is true, reads bytes in a big endian format, otherwise reads them as
little endian.
-->
バッファの指定された位置を符号付き 32bit 整数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。

<!--
Works as `buffer.readUInt32`, except buffer contents are treated as twos
complement signed values.
-->
バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt32` と同じように動作します。


### buffer.readFloat(offset, bigEndian)

<!--
Reads a 32 bit float from the buffer at the specified offset.
-->
バッファの指定された位置を 32bit 浮動小数点数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。

<!--
Example:
-->
例:

    var buf = new Buffer(4);

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x80;
    buf[3] = 0x3f;

    console.log(buf.readFloat(0, false));

    // 0x01

### buffer.readDouble(offset, bigEndian)

<!--
Reads a 64 bit double from the buffer at the specified offset. Endian must be
either true or false and specifies what endian ordering to read the bytes
from the buffer in.
-->
バッファの指定された位置を 64bit 倍精度浮動小数点数として読み込みます。
もし `bigEndian` が `true` ならビッグエンディアン、
それ以外ならリトルエンディアンでバイト列を読み込みます。

<!--
Example:
-->
例:

    var buf = new Buffer(8);

    buf[0] = 0x55;
    buf[1] = 0x55;
    buf[2] = 0x55;
    buf[3] = 0x55;
    buf[4] = 0x55;
    buf[5] = 0x55;
    buf[6] = 0xd5;
    buf[7] = 0x3f;

    console.log(buf.readDouble(0, false));

    // 0.3333333333333333

### buffer.writeUInt8(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 8 bit unsigned integer.
-->
`value` を符号無し 8bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 8bit 符号無し整数でなければならないことに注意してください。

<!--
Example:
-->
例:

    var buf = new Buffer(4);
    buf.writeUInt8(0x3, 0, true);
    buf.writeUInt8(0x4, 1, true);
    buf.writeUInt8(0x23, 2, true);
    buf.writeUInt8(0x42, 3, true);

    console.log(buf);

    buf.writeUInt8(0x3, 0, false);
    buf.writeUInt8(0x4, 1, false);
    buf.writeUInt8(0x23, 2, false);
    buf.writeUInt8(0x42, 3, false);

    console.log(buf);

    // <Buffer 03 04 23 42>
    // <Buffer 03 04 23 42>

### buffer.writeUInt16(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 16 bit unsigned integer.
-->
`value` を符号無し 16bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 16bit 符号無し整数でなければならないことに注意してください。

<!--
Example:
-->
例:

    var buf = new Buffer(4);
    buf.writeUInt16(0xdead, 0, true);
    buf.writeUInt16(0xbeef, 2, true);

    console.log(buf);

    buf.writeUInt16(0xdead, 0, false);
    buf.writeUInt16(0xbeef, 2, false);

    console.log(buf);

    // <Buffer de ad be ef>
    // <Buffer ad de ef be>

### buffer.writeUInt32(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 32 bit unsigned integer.
-->
`value` を符号無し 32bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 32bit 符号無し整数でなければならないことに注意してください。

<!--
Example:
-->
例:

    var buf = new Buffer(4);
    buf.writeUInt32(0xfeedface, 0, true);

    console.log(buf);

    buf.writeUInt32(0xfeedface, 0, false);

    console.log(buf);

    // <Buffer fe ed fa ce>
    // <Buffer ce fa ed fe>

### buffer.writeInt8(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 16 bit signed integer.

Works as `buffer.writeUInt8`, except value is written out as a two's complement
signed integer into `buffer`.
-->
`value` を符号付き 8bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 8bit 符号付き整数でなければならないことに注意してください。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt8` と同じように動作します。

### buffer.writeInt16(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 16 bit unsigned integer.

Works as `buffer.writeUInt16`, except value is written out as a two's complement
signed integer into `buffer`.
-->
`value` を符号付き 16bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 16bit 符号付き整数でなければならないことに注意してください。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt16` と同じように動作します。

### buffer.writeInt32(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 32 bit signed integer.

Works as `buffer.writeUInt32`, except value is written out as a two's complement
signed integer into `buffer`.
-->
`value` を符号付き 32bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 32bit 符号付き整数でなければならないことに注意してください。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt32` と同じように動作します。

### buffer.writeFloat(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 32 bit float.
-->
`value` を 32bit 浮動小数点数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 32bit 浮動小数点数でなければならないことに注意してください。

<!--
Example:
-->

    var buf = new Buffer(4);
    buf.writeFloat(0xcafebabe, 0, true);

    console.log(buf);

    buf.writeFloat(0xcafebabe, 0, false);

    console.log(buf);

    // <Buffer 4f 4a fe bb>
    // <Buffer bb fe 4a 4f>

### buffer.writeDouble(value, offset, bigEndian)

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 64 bit double.
-->
`value` を 64bit 倍精度浮動小数点数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 64bit 倍精度浮動小数点数でなければならないことに注意してください。

<!--
Example:
-->
例:

    var buf = new Buffer(8);
    buf.writeFloat(0xdeadbeefcafebabe, 0, true);

    console.log(buf);

    buf.writeFloat(0xdeadbeefcafebabe, 0, false);

    console.log(buf);

    // <Buffer 43 eb d5 b7 dd f9 5f d7>
    // <Buffer d7 5f f9 dd b7 d5 eb 43>

### buffer.readUInt8NoChk(value, offset, bigEndian)
### buffer.readUInt16NoChk(value, offset, bigEndian)
### buffer.readUInt32NoChk(value, offset, bigEndian)
### buffer.writeUInt8NoChk(value, offset, bigEndian)
### buffer.writeUInt16NoChk(value, offset, bigEndian)
### buffer.writeUInt32NoChk(value, offset, bigEndian)

<!--
These functions all work as per the versions without the NoChk suffix. These
functions allow you to do use the raw functionality without any kind of
validation for correctness. This means that value may be too large for the
specific function and offset may be beyond the end of the buffer leading to the
values being silently dropped. These should not be used unless you are certain
of correctness.
-->
これらの関数は、接尾辞 NoChk 無しのバージョンと同様に動作します。
これらの関数は正確性のための検証をすることなく、生の機能を利用可能にします。
これは、特定の関数で扱えるよりも大きな値や、
バッファの終端を越えるオフセットが指定された場合は静かに切り捨てられることを
意味します。
正確性に確信がない限り、これらを使用すべきではありません。

### buffer.fill(value, offset=0, length=-1)

<!--
Fills the buffer with the specified value. If the offset and length are not
given it will fill the entire buffer.
-->
指定された値でバッファを埋めます。
オフセットと長さが与えられなかった場合はバッファ全体を埋めます。

    var b = new Buffer(50);
    b.fill("h");

### INSPECT_MAX_BYTES

How many bytes will be returned when `b.inspect()` is called. This can
be overriden by user modules.
