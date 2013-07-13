# Buffer

    Stability: 3 - Stable

<!--
Pure JavaScript is Unicode friendly but not nice to binary data.  When
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
The `Buffer` class is a global, making it very rare that one would need
to ever `require('buffer')`.
-->

`Buffer` クラスはグローバルなので、`require('buffer')` が必要になることは
ほとんどありません。

<!--
Converting between Buffers and JavaScript string objects requires an explicit
encoding method.  Here are the different string encodings.
-->

バッファを JavaScript 文字列オブジェクトとの間で変換するにはエンコーディング方式を明示する必要があります。
いくつかのエンコーディング方式があります。

<!--
* `'ascii'` - for 7 bit ASCII data only.  This encoding method is very fast, and
  will strip the high bit if set.

* `'utf8'` - Multibyte encoded Unicode characters. Many web pages and other
  document formats use UTF-8.

* `'utf16le'` - 2 or 4 bytes, little endian encoded Unicode characters.
  Surrogate pairs (U+10000 to U+10FFFF) are supported.

* `'ucs2'` - Alias of `'utf16le'`.

* `'base64'` - Base64 string encoding.

* `'binary'` - A way of encoding raw binary data into strings by using only
  the first 8 bits of each character. This encoding method is deprecated and
  should be avoided in favor of `Buffer` objects where possible. This encoding
  will be removed in future versions of Node.

* `'hex'` - Encode each byte as two hexadecimal characters.
-->

* `'ascii'` - 7bit の ASCII データ専用です。
  このエンコーディング方式はとても高速で、もし上位ビットがセットされていれば取り除かれます。

* `'utf8'` - 可変長のバイト単位でエンコードされたUnicode文字。
  多くのWebページやその他のドキュメントは UTF-8 を使っています。

* `'utf16le'` - 2 または 4 バイトのリトルエンディアンでエンコードされた
  Unicode 文字。
  サロゲートペア (U+10000～U+10FFFF) もサポートされます。

* `'ucs2'` - `'utf16le'` の別名です。

* `'base64'` - Base64 文字列エンコーディング.

* `'binary'` - 生のバイナリデータを各文字の最初の 8bit として使用するエンコーディング方式。
このエンコーディング方式はもはや価値がなく、`Buffer` オブジェクトでは可能な限り使用すべきではありません。
このエンコーディングは、Node の将来のバージョンで削除される予定です。

* `'hex'` - 各バイトを 2 桁の16進数文字列でエンコードします。

## Class: Buffer

<!--
The Buffer class is a global type for dealing with binary data directly.
It can be constructed in a variety of ways.
-->

Buffer クラスはバイナリデータを直接扱うためのグローバルな型です。
それは様々な方法で構築することができます。

### new Buffer(size)

* `size` Number

<!--
Allocates a new buffer of `size` octets.
-->

`size` オクテットの新しいバッファを割り当てます。

### new Buffer(array)

* `array` Array

<!--
Allocates a new buffer using an `array` of octets.
-->

オクテットの `array` を使用する新しいバッファを割り当てます。

### new Buffer(str, [encoding])

<!--
* `str` String - string to encode.
* `encoding` String - encoding to use, Optional.
-->

* `str` String - エンコードされる文字列
* `encoding` String - 使用するエンコード、Optional、Default: 'utf8'

<!--
Allocates a new buffer containing the given `str`.
`encoding` defaults to `'utf8'`.
-->

与えられた `str` を内容とする新しいバッファを割り当てます。
`encoding` のデフォルトは `'utf8'` です。

### Class Method: Buffer.isEncoding(encoding)

<!--
* `encoding` {String} The encoding string to test
-->

* `encoding` {String} 検証するエンコーディング名

<!--
Returns true if the `encoding` is a valid encoding argument, or false
otherwise.
-->

`encoding` が正しければ `true`、それ以外は `false` を返します。

### Class Method: Buffer.isBuffer(obj)

* `obj` Object
* Return: Boolean

<!--
Tests if `obj` is a `Buffer`.
-->

`obj` が `Buffer` かどうかテストします。

### Class Method: Buffer.byteLength(string, [encoding])

<!--
* `string` String
* `encoding` String, Optional, Default: 'utf8'
* Return: Number
-->

* `string` String
* `encoding` String, 任意, デフォルト: 'utf8'
* Return: Number

<!--
Gives the actual byte length of a string. `encoding` defaults to `'utf8'`.
This is not the same as `String.prototype.length` since that returns the
number of *characters* in a string.
-->

文字列の実際のバイト数を返します。`encoding` のデフォルトは `'utf8'` です。
これは文字列の*文字*数を返す `String.prototype.length` と同じではありません。

<!--
Example:
-->

例:

    str = '\u00bd + \u00bc = \u00be';

    console.log(str + ": " + str.length + " characters, " +
      Buffer.byteLength(str, 'utf8') + " bytes");

    // ½ + ¼ = ¾: 9 characters, 12 bytes

### Class Method: Buffer.concat(list, [totalLength])

<!--
* `list` {Array} List of Buffer objects to concat
* `totalLength` {Number} Total length of the buffers when concatenated
-->

* `list` {Array} 結合するバッファのリスト
* `totalLength` {Number} 結合されるバッファ全体の長さ

<!--
Returns a buffer which is the result of concatenating all the buffers in
the list together.
-->

リストに含まれるバッファ全体を結合した結果のバッファを返します。

<!--
If the list has no items, or if the totalLength is 0, then it returns a
zero-length buffer.
-->

リストが空の場合、または `totalLength` が 0 の場合は長さが
0 のバッファを返します。

<!--
If the list has exactly one item, then the first item of the list is
returned.
-->

リストが一つだけの要素を持つ場合、リストの先頭要素が返されます。

<!--
If the list has more than one item, then a new Buffer is created.
-->

リストが複数の要素を持つ場合、新しいバッファが作成されます。

<!--
If totalLength is not provided, it is read from the buffers in the list.
However, this adds an additional loop to the function, so it is faster
to provide the length explicitly.
-->

`totalLength` が与えられない場合はリスト中のバッファから求められます。
しかし、これは余計なループが必要になるため、明示的に長さを指定する方が
高速です。

### Class Method: Buffer.alloc(length, [receiver])

<!--
* `length` Number
* `receiver` Object, Optional, Default: `new Object`
-->

* `length` Number
* `receiver` Object, 任意, デフォルト: `new Object`


<!--
**(EXPERIMENTAL)** Returns object with allocated external array data.
-->

**(実験的)** オブジェクトを外部配列のデータに割り当てて返します。

<!--
Buffers are backed by a simple allocator that only handles the assignation of
external raw memory. This exposes that functionality.
-->

Buffer は外部にある生のメモリを割り当てるだけの単純なアロケータによって
扱われます。これはその機能を公開します。

<!--
No pooling is performed for these allocations. So there's no form of memory
leak.
-->

これらの割り当てにプーリングは適用されません。
そのため、メモリリークする形式はありません。

<!--
This can be used to create your own Buffer-like classes.
-->

これは Buffer に似た独自のクラスを作成するために使用することができます。

    function SimpleData(n) {
      this.length = n;
      Buffer.alloc(this.length, this);
    }

    SimpleData.prototype = { /* ... */ };

### Class Method: Buffer.dispose(obj)

* `obj` Object


<!--
**(EXPERIMENTAL)** Free memory that has been allocated to an object via
`Buffer.alloc`.
-->

**(実験的)** `Buffer.alloc()` によって割り当てられたオブジェクトを解放します。

    var a = {};
    Buffer.alloc(3, a);

    // { '0': 0, '1': 0, '2': 0 }

    Buffer.dispose(a);

    // {}

### buf.length

* Number

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
    buf.write("some string", 0, "ascii");
    console.log(buf.length);

    // 1234
    // 1234

### buf.write(string, [offset], [length], [encoding])

<!--
* `string` String - data to be written to buffer
* `offset` Number, Optional, Default: 0
* `length` Number, Optional, Default: `buffer.length - offset`
* `encoding` String, Optional, Default: 'utf8'
-->

* `string` String - バッファに書き込まれるデータ
* `offset` Number, Optional, Default: 0
* `length` Number, Optional
* `encoding` String, Optional, Default: 'utf8'

<!--
Writes `string` to the buffer at `offset` using the given encoding.
`offset` defaults to `0`, `encoding` defaults to `'utf8'`. `length` is
the number of bytes to write. Returns number of octets written. If `buffer` did
not contain enough space to fit the entire string, it will write a partial
amount of the string. `length` defaults to `buffer.length - offset`.
The method will not write partial characters.
-->

与えられたエンコーディングを使用して、`string` をバッファの `offset` から書き込みます。
`offset` のデフォルトは `0`、`encoding` のデフォルトは `'utf8'` です。
`length` は書き込むバイト数です。書き込まれたオクテット数を返します。
もし `buffer` が文字列全体を挿入するのに十分なスペースを含んでいなければ、文字列の一部だけを書き込みます。
`length` のデフォルトは `buffer.length - offset` です。
このメソッドは文字の一部だけを書き込むことはありません。

<!--
Example: write a utf8 string into a buffer, then print it
-->

例: utf8 の文字列をバッファに書き込み、それをプリントします

    buf = new Buffer(256);
    len = buf.write('\u00bd + \u00bc = \u00be', 0);
    console.log(len + " bytes: " + buf.toString('utf8', 0, len));


### buf.toString([encoding], [start], [end])

* `encoding` String, Optional, Default: 'utf8'
* `start` Number, Optional, Default: 0
* `end` Number, Optional, Default: `buffer.length`

<!--
Decodes and returns a string from buffer data encoded with `encoding`
(defaults to `'utf8'`) beginning at `start` (defaults to `0`) and ending at
`end` (defaults to `buffer.length`).
-->

`encoding` (デフォルトは `'utf8'`) でエンコードされたバッファデータの
`start` (デフォルトは `0`) から `end` (デフォルトは `buffer.length`)
までをデコードした文字列を返します。

<!--
See `buffer.write()` example, above.
-->

上の `buffer.write()` の例を参照してください。


### buf.toJSON()

<!--
Returns a JSON-representation of the Buffer instance.  `JSON.stringify`
implicitly calls this function when stringifying a Buffer instance.
-->

バッファインスタンスの JSON 表現を返します。
`JSON.stringify()` は Buffer のインスタンスを文字列化する際に、
この関数を暗黙的に呼び出します。

<!--
Example:
-->

例:

    var buf = new Buffer('test');
    var json = JSON.stringify(buf);

    console.log(json);
    // '{"type":"Buffer","data":[116,101,115,116]}'

    var copy = JSON.parse(json, function(key, value) {
        return value && value.type === 'Buffer'
          ? new Buffer(value.data)
          : value;
      });

    console.log(copy);
    // <Buffer 74 65 73 74>

### buf[index]

<!--type=property-->

<!--name=[index]-->

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

### buf.copy(targetBuffer, [targetStart], [sourceStart], [sourceEnd])

<!--
* `targetBuffer` Buffer object - Buffer to copy into
* `targetStart` Number, Optional, Default: 0
* `sourceStart` Number, Optional, Default: 0
* `sourceEnd` Number, Optional, Default: `buffer.length`
-->

* `targetBuffer` Buffer object - コピー先の Buffer
* `targetStart` Number, Optional, Default: 0
* `sourceStart` Number, Optional, Default: 0
* `sourceEnd` Number, Optional, Default: `buffer.length`

<!--
Does copy between buffers. The source and target regions can be overlapped.
`targetStart` and `sourceStart` default to `0`.
`sourceEnd` defaults to `buffer.length`.
-->

バッファ間でコピーします。
ソースとターゲットの領域は重なっていても構いません。
`targetStart` と `sourceStart` のデフォルトは `0` です。
`sourceEnd` のデフォルトは `buffer.length` です。

<!--
All values passed that are `undefined`/`NaN` or are out of bounds are set equal
to their respective defaults.
-->

`undefined`/`NaN` またはその他の不正な値が渡された場合は、
それぞれのデフォルトが設定されます。

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


### buf.slice([start], [end])

* `start` Number, Optional, Default: 0
* `end` Number, Optional, Default: `buffer.length`

<!--
Returns a new buffer which references the same memory as the old, but offset
and cropped by the `start` (defaults to `0`) and `end` (defaults to
`buffer.length`) indexes.  Negative indexes start from the end of the buffer.
-->

元のバッファと同じメモリを参照しますが、`start` (デフォルトは `0`) と
`end` (デフォルトは `buffer.length`) で示されるオフセットと長さを持つ
新しいバッファを返します。
負のインデックスはバッファの末尾から開始します。

<!--
**Modifying the new buffer slice will modify memory in the original buffer!**
-->

**新しいバッファスライスの変更は、オリジナルバッファのメモリを変更することになります！**

<!--
Example: build a Buffer with the ASCII alphabet, take a slice, then modify one
byte from the original Buffer.
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

### buf.readUInt8(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads an unsigned 8 bit integer from the buffer at the specified offset.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.
-->

バッファの指定された位置を符号無し 8bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

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
      console.log(buf.readUInt8(ii));
    }

    // 0x3
    // 0x4
    // 0x23
    // 0x42

### buf.readUInt16LE(offset, [noAssert])
### buf.readUInt16BE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads an unsigned 16 bit integer from the buffer at the specified offset with
specified endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.
-->

バッファの指定された位置を符号無し 16bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);

    buf[0] = 0x3;
    buf[1] = 0x4;
    buf[2] = 0x23;
    buf[3] = 0x42;

    console.log(buf.readUInt16BE(0));
    console.log(buf.readUInt16LE(0));
    console.log(buf.readUInt16BE(1));
    console.log(buf.readUInt16LE(1));
    console.log(buf.readUInt16BE(2));
    console.log(buf.readUInt16LE(2));

    // 0x0304
    // 0x0403
    // 0x0423
    // 0x2304
    // 0x2342
    // 0x4223

### buf.readUInt32LE(offset, [noAssert])
### buf.readUInt32BE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads an unsigned 32 bit integer from the buffer at the specified offset with
specified endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.
-->

バッファの指定された位置を符号無し 32bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);

    buf[0] = 0x3;
    buf[1] = 0x4;
    buf[2] = 0x23;
    buf[3] = 0x42;

    console.log(buf.readUInt32BE(0));
    console.log(buf.readUInt32LE(0));

    // 0x03042342
    // 0x42230403

### buf.readInt8(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads a signed 8 bit integer from the buffer at the specified offset.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.

Works as `buffer.readUInt8`, except buffer contents are treated as two's
complement signed values.
-->

バッファの指定された位置を符号付き 8bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt8` と同じように動作します。

### buf.readInt16LE(offset, [noAssert])
### buf.readInt16BE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads a signed 16 bit integer from the buffer at the specified offset with
specified endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.

Works as `buffer.readUInt16*`, except buffer contents are treated as two's
complement signed values.
-->

バッファの指定された位置を符号付き 16bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt16` と同じように動作します。


### buf.readInt32LE(offset, [noAssert])
### buf.readInt32BE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads a signed 32 bit integer from the buffer at the specified offset with
specified endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.

Works as `buffer.readUInt32*`, except buffer contents are treated as two's
complement signed values.
-->

バッファの指定された位置を符号付き 32bit 整数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

バッファの内容を 2 の補数による符号付き値として扱うこと以外は
`buffer.readUInt32` と同じように動作します。


### buf.readFloatLE(offset, [noAssert])
### buf.readFloatBE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads a 32 bit float from the buffer at the specified offset with specified
endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.
-->

バッファの指定された位置を 32bit 浮動小数点数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);

    buf[0] = 0x00;
    buf[1] = 0x00;
    buf[2] = 0x80;
    buf[3] = 0x3f;

    console.log(buf.readFloatLE(0));

    // 0x01

### buf.readDoubleLE(offset, [noAssert])
### buf.readDoubleBE(offset, [noAssert])

* `offset` Number
* `noAssert` Boolean, Optional, Default: false
* Return: Number

<!--
Reads a 64 bit double from the buffer at the specified offset with specified
endian format.

Set `noAssert` to true to skip validation of `offset`. This means that `offset`
may be beyond the end of the buffer. Defaults to `false`.
-->

バッファの指定された位置を 64bit 倍精度浮動小数点数として読み込みます。

もし `noAssert` が `true` なら `offset` の検証をスキップします。
これは `offset` がバッファの終端を越えてしまう場合があることを意味します。
デフォルトは `false` です。

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

    console.log(buf.readDoubleLE(0));

    // 0.3333333333333333

### buf.writeUInt8(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset. Note, `value` must be a
valid unsigned 8 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.
-->

`value` を符号無し 8bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 8bit 符号無し整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);
    buf.writeUInt8(0x3, 0);
    buf.writeUInt8(0x4, 1);
    buf.writeUInt8(0x23, 2);
    buf.writeUInt8(0x42, 3);

    console.log(buf);

    // <Buffer 03 04 23 42>

### buf.writeUInt16LE(value, offset, [noAssert])
### buf.writeUInt16BE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid unsigned 16 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.
-->

`value` を符号無し 16bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 16bit 符号無し整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);
    buf.writeUInt16BE(0xdead, 0);
    buf.writeUInt16BE(0xbeef, 2);

    console.log(buf);

    buf.writeUInt16LE(0xdead, 0);
    buf.writeUInt16LE(0xbeef, 2);

    console.log(buf);

    // <Buffer de ad be ef>
    // <Buffer ad de ef be>

### buf.writeUInt32LE(value, offset, [noAssert])
### buf.writeUInt32BE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid unsigned 32 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.
-->

`value` を符号無し 32bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 32bit 符号無し整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);
    buf.writeUInt32BE(0xfeedface, 0);

    console.log(buf);

    buf.writeUInt32LE(0xfeedface, 0);

    console.log(buf);

    // <Buffer fe ed fa ce>
    // <Buffer ce fa ed fe>

### buf.writeInt8(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset. Note, `value` must be a
valid signed 8 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.

Works as `buffer.writeUInt8`, except value is written out as a two's complement
signed integer into `buffer`.
-->

`value` を符号付き 8bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 8bit 符号付き整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt8` と同じように動作します。

### buf.writeInt16LE(value, offset, [noAssert])
### buf.writeInt16BE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid signed 16 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.

Works as `buffer.writeUInt16*`, except value is written out as a two's
complement signed integer into `buffer`.
-->

`value` を符号付き 16bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 16bit 符号付き整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt16` と同じように動作します。

### buf.writeInt32LE(value, offset, [noAssert])
### buf.writeInt32BE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid signed 32 bit integer.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.

Works as `buffer.writeUInt32*`, except value is written out as a two's
complement signed integer into `buffer`.
-->

`value` を符号付き 32bit 整数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 32bit 符号付き整数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

`value` を 2 の補数による符号付き値として書き込むこと以外は 
`buffer.writeUInt32` と同じように動作します。

### buf.writeFloatLE(value, offset, [noAssert])
### buf.writeFloatBE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, behavior is unspecified if `value` is not a 32 bit float.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.
-->

`value` を 32bit 浮動小数点数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` が 32bit 浮動小数点数でない場合の振る舞いは未定義であることに
注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(4);
    buf.writeFloatBE(0xcafebabe, 0);

    console.log(buf);

    buf.writeFloatLE(0xcafebabe, 0);

    console.log(buf);

    // <Buffer 4f 4a fe bb>
    // <Buffer bb fe 4a 4f>

### buf.writeDoubleLE(value, offset, [noAssert])
### buf.writeDoubleBE(value, offset, [noAssert])

* `value` Number
* `offset` Number
* `noAssert` Boolean, Optional, Default: false

<!--
Writes `value` to the buffer at the specified offset with specified endian
format. Note, `value` must be a valid 64 bit double.

Set `noAssert` to true to skip validation of `value` and `offset`. This means
that `value` may be too large for the specific function and `offset` may be
beyond the end of the buffer leading to the values being silently dropped. This
should not be used unless you are certain of correctness. Defaults to `false`.
-->

`value` を 64bit 倍精度浮動小数点数としてバッファの指定された位置に、
指定されたエンディアンで書き込みます。
`value` は妥当な 64bit 倍精度浮動小数点数でなければならないことに注意してください。

もし `noAssert` が `true` なら，`value` と `offset` の検証をスキップします。
これは、`value` がこの関数で扱えるより大きな場合や、`offset` 
がバッファの終端を越えてしまう場合は、静かに捨てられることを意味します。
正確性に確信がない限り、これらを使用すべきではありません。
デフォルトは `false` です。

<!--
Example:
-->

例:

    var buf = new Buffer(8);
    buf.writeDoubleBE(0xdeadbeefcafebabe, 0);

    console.log(buf);

    buf.writeDoubleLE(0xdeadbeefcafebabe, 0);

    console.log(buf);

    // <Buffer 43 eb d5 b7 dd f9 5f d7>
    // <Buffer d7 5f f9 dd b7 d5 eb 43>

### buf.fill(value, [offset], [end])

* `value`
* `offset` Number, Optional
* `end` Number, Optional

<!--
Fills the buffer with the specified value. If the `offset` (defaults to `0`)
and `end` (defaults to `buffer.length`) are not given it will fill the entire
buffer.
-->

指定された値でバッファを埋めます。
`offset` (デフォルトは `0`) と `end` (デフォルトは `buffer.length`)
Fが与えられなかった場合はバッファ全体を埋めます。

    var b = new Buffer(50);
    b.fill("h");

## buffer.INSPECT_MAX_BYTES

* Number, Default: 50

<!--
How many bytes will be returned when `buffer.inspect()` is called. This can
be overridden by user modules.

Note that this is a property on the buffer module returned by
`require('buffer')`, not on the Buffer global, or a buffer instance.
-->

`buffer.inspect()` が呼び出された場合に返すバイト数です。
これはユーザモジュールによって上書きすることができます。

これはグローバルの Buffer やそのインスタンスではなく、 `requrie('buffer')`
によって返される buffer モジュールのプロパティであることに注意してください。

## Class: SlowBuffer

<!--
Returns an un-pooled `Buffer`.
-->

プールされていない `Buffer` オブジェクトを返します。

<!--
In order to avoid the garbage collection overhead of creating many individually
allocated Buffers, by default allocations under 4KB are sliced from a single
larger allocated object. This approach improves both performance and memory
usage since v8 does not need to track and cleanup as many `Persistent` objects.
-->

多くの独立したバッファを割り当てることによるガーベッジコレクションの
オーバーヘッドを避けるため、デフォルトでは 4KB 以下のバッファは大きな
単一のバッファから切り出されて割り当てられます。
このアプローチは、v8 が多くの「永続的な」オブジェクトを追跡して
クリーンアップする必要を無くすため、パフォーマンスとメモリ使用量の両方を
改善します。

<!--
In the case where a developer may need to retain a small chunk of memory from a
pool for an indeterminate amount of time it may be appropriate to create an
un-pooled Buffer instance using SlowBuffer and copy out the relevant bits.
-->

プールから得た小さなメモリ片を不確定の時間保持する必要がある場合は、
`SlowBuffer` を使ってプールされていない `Buffer` のインスタンスを作成し、
関連したビットを全てコピーすることが適切な場合があります。

    // need to keep around a few small chunks of memory
    var store = [];

    socket.on('readable', function() {
      var data = socket.read();
      // allocate for retained data
      var sb = new SlowBuffer(10);
      // copy the data into the new allocation
      data.copy(sb, 0, 0, 10);
      store.push(sb);
    });

<!--
Though this should used sparingly and only be a last resort *after* a developer
has actively observed undue memory retention in their applications.
-->

しかし、これはアプリケーションで不適切なメモリの保持が盛大に観測された *後で*、
最後の手段として控えめに使用されるべきです。
