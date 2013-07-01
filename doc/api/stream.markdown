# Stream

    Stability: 2 - Unstable

<!--
A stream is an abstract interface implemented by various objects in
Node.  For example a request to an HTTP server is a stream, as is
stdout. Streams are readable, writable, or both. All streams are
instances of [EventEmitter][]
-->

ストリームは Node の様々なオブジェクトで実装される抽象的なインタフェースです。
例えば HTTP サーバへのリクエストは標準出力と同様にストリームです。
ストリームは読み込み可能、書き込み可能、またはその両方です。
全てのストリームは [EventEmitter][] のインスタンスです。

<!--
You can load the Stream base classes by doing `require('stream')`.
There are base classes provided for Readable streams, Writable
streams, Duplex streams, and Transform streams.
-->

Stream のベースクラスは `require('stream')` でロードすることができます。
Readable ストリーム、Writable ストリーム、Duplex ストリーム、Transform
ストリームのベースクラスが提供されます。

## Compatibility

<!--
In earlier versions of Node, the Readable stream interface was
simpler, but also less powerful and less useful.
-->

以前のバージョンの Node では、読み込み可能なストリームのインタフェースは
よりシンプルでしたが、強力ではなく使いやすくもありませんでした。

<!--
* Rather than waiting for you to call the `read()` method, `'data'`
  events would start emitting immediately.  If you needed to do some
  I/O to decide how to handle data, then you had to store the chunks
  in some kind of buffer so that they would not be lost.
* The `pause()` method was advisory, rather than guaranteed.  This
  meant that you still had to be prepared to receive `'data'` events
  even when the stream was in a paused state.
-->

* `read()` メソッドを呼び出すのを待つのではなく、
  `'data'` イベントがすぐに生成され始めます。
  もしデータを処理する方法を決定するためにいくらかの I/O をする
  必要がある場合、データが失われないようにするためには
  チャンクを何らかのバッファに保存しなければなりませんでした。
* `pause()` は保証というよりはむしろ助言です。
  それはストリームが中断された状態であったとしても、
  `'data'` イベントを受け取る準備が必要だということを意味します。

<!--
In Node v0.10, the Readable class described below was added.  For
backwards compatibility with older Node programs, Readable streams
switch into "old mode" when a `'data'` event handler is added, or when
the `pause()` or `resume()` methods are called.  The effect is that,
even if you are not using the new `read()` method and `'readable'`
event, you no longer have to worry about losing `'data'` chunks.
-->

Node v0.10 から、以下で説明する Readable クラスが追加されました。
古い Node プログラムとの後方互換性のために、Readable ストリームは
`'data'` イベントのハンドラが加えられた場合や、`pause()` または
`resume()` メソッドが読み出されると、「旧モード」に切り替わります。
その結果として、新しい `read()` メソッドや `'readable'` イベントを
使用していなくても、もう `'data'` イベントのチャンクが失われることを
心配する必要はありません。

<!--
Most programs will continue to function normally.  However, this
introduces an edge case in the following conditions:
-->

ほとんどのプログラムはこれまで通りに機能するでしょう。
しかしながら、以下の条件でエッジケースが存在します。

<!--
* No `'data'` event handler is added.
* The `pause()` and `resume()` methods are never called.
-->

* `'data'` イベントハンドラが登録されていない。
* `pause()` および `resume()` メソッドが呼び出されていない。

<!--
For example, consider the following code:
-->

例えば、以下のコードを考えてみてください:

```javascript
// WARNING!  BROKEN!
net.createServer(function(socket) {

  // we add an 'end' method, but never consume the data
  socket.on('end', function() {
    // It will never get here.
    socket.end('I got your message (but didnt read it)\n');
  });

}).listen(1337);
```

<!--
In versions of node prior to v0.10, the incoming message data would be
simply discarded.  However, in Node v0.10 and beyond, the socket will
remain paused forever.
-->

v0.10 より前の Node では、入ってきたデータは単純に破棄されていました。
しかしながら、Node v0.10 以降では、ソケットは中断したままとなります。

<!--
The workaround in this situation is to call the `resume()` method to
trigger "old mode" behavior:
-->

この状況の回避策は、「旧モード」の振る舞いへのトリガーとして
`resume()` メソッドを呼び出すことです。

```javascript
// Workaround
net.createServer(function(socket) {

  socket.on('end', function() {
    socket.end('I got your message (but didnt read it)\n');
  });

  // start the flow of data, discarding it.
  socket.resume();

}).listen(1337);
```

<!--
In addition to new Readable streams switching into old-mode, pre-v0.10
style streams can be wrapped in a Readable class using the `wrap()`
method.
-->

新しい Readable ストリームを旧モードに切り替えられることに加えて、
`warp()` メソッドを使って v0.10 より前のスタイルのストリームを
Readable クラスでラップすることもできます。

## Class: stream.Readable

<!--type=class-->

<!--
A `Readable Stream` has the following methods, members, and events.
-->

`Readable Stream` には以下のメソッド、メンバー、そしてイベントがあります。

<!--
Note that `stream.Readable` is an abstract class designed to be
extended with an underlying implementation of the `_read(size)`
method. (See below.)
-->

`stream.Readable` は下層の実装である `_read(size)` メソッド (後述)
によって拡張されるように設計された抽象クラスであることに注意してください。

### new stream.Readable([options])

<!--
* `options` {Object}
  * `highWaterMark` {Number} The maximum number of bytes to store in
    the internal buffer before ceasing to read from the underlying
    resource.  Default=16kb
  * `encoding` {String} If specified, then buffers will be decoded to
    strings using the specified encoding.  Default=null
  * `objectMode` {Boolean} Whether this stream should behave
    as a stream of objects. Meaning that stream.read(n) returns
    a single value instead of a Buffer of size n
-->

* `options` {Object} (任意)
  * `highWaterMark` {Number} 下層のリソースから読み込むのを中断するまで
    内部バッファに貯めておくバイト数の最大値。デフォルトは 16kb。
  * `encoding` {String} 指定されるとバッファは指定のエンコーディングで
    デコードされます。デフォルトは `null`。
  * `objectMode` {Boolean} このストリームがオブジェクトストリームとして
    振る舞うべきかどうか。これは `stream.read(n)` がサイズ n のバッファではなく
    一つの値を返すことを意味します。

<!--
In classes that extend the Readable class, make sure to call the
constructor so that the buffering settings can be properly
initialized.
-->

`Readable` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

### readable.\_read(size)

<!--
* `size` {Number} Number of bytes to read asynchronously
-->

* `size` {Number} 非同期に読み込むバイト数

<!--
Note: **This function should NOT be called directly.**  It should be
implemented by child classes, and called by the internal Readable
class methods only.
-->

注意: **この関数は直接呼び出すべきではありません。**
これはサブクラスで実装されるべきであり、Readable クラスの内部からのみ
呼び出されるべきです。

<!--
All Readable stream implementations must provide a `_read` method
to fetch data from the underlying resource.
-->

全ての Readable ストリームは、下層のリソースからデータを
取得するために `_read` メソッドを提供しなければなりません。

<!--
This method is prefixed with an underscore because it is internal to
the class that defines it, and should not be called directly by user
programs.  However, you **are** expected to override this method in
your own extension classes.
-->

このメソッドはこれを定義するクラス内部のものであり、ユーザプログラムから
直接呼び出されるべきものではないため、アンダースコアの接頭辞を持ちます。
しかしながら、あなたの拡張クラスではこのメソッドをオーバーライドすることが
求められて**います**。

<!--
When data is available, put it into the read queue by calling
`readable.push(chunk)`.  If `push` returns false, then you should stop
reading.  When `_read` is called again, you should start pushing more
data.
-->

データが利用可能になれば、`readable.push(chunk)` を呼び出すことで
それを読み込みキューに追加します。
`push` が false を返した場合は、読み込みを止めるべきです。
`_read` が再び呼び出された時が、さらに多くのデータを追加を開始すべき時です。

<!--
The `size` argument is advisory.  Implementations where a "read" is a
single call that returns data can use this to know how much data to
fetch.  Implementations where that is not relevant, such as TCP or
TLS, may ignore this argument, and simply provide data whenever it
becomes available.  There is no need, for example to "wait" until
`size` bytes are available before calling `stream.push(chunk)`.
-->

`size` 引数はアドバイス的です。
"read" が一回の呼び出しでデータを返す実装では、
どれだけのデータを取得すべきか知るためにこれを使うことができます。
TCPやTLSなど、それに関連しない実装ではこの引数は無視され、
利用可能になったデータをシンプルに提供するかもしれません。
たとえば `size` バイトが利用可能になるまで「待つ」必要はありません。

### readable.push(chunk, [encoding])

<!--
* `chunk` {Buffer | null | String} Chunk of data to push into the read queue
* `encoding` {String} Encoding of String chunks.  Must be a valid
  Buffer encoding, such as `'utf8'` or `'ascii'`
* return {Boolean} Whether or not more pushes should be performed
-->

* `chunk` {Buffer | null | String} 読み込みキューにプッシュされる、
  データのチャンク
* `encoding` {String} 文字列チャンクのエンコーディング。
  `'utf8'` や `'ascii'` など、バッファの正しいエンコーディングであること。
* return {Boolean} まだプッシュしてもいいかどうか

<!--
Note: **This function should be called by Readable implementors, NOT
by consumers of Readable streams.**  The `_read()` function will not
be called again until at least one `push(chunk)` call is made.  If no
data is available, then you MAY call `push('')` (an empty string) to
allow a future `_read` call, without adding any data to the queue.
-->

注意: **この関数は Readable の実装から呼び出されるべきものであり、
Readable ストリームの利用者が呼び出すべきではありません。**
少なくとも一回は `push(chunk)` が呼び出されないと、`_read()` 関数が
再び呼び出されることはありません。もし利用可能なデータがない場合は、
将来 `_read` が呼び出されるように `push(''`)` (空文字列)
を呼び出すことができます。

<!--
The `Readable` class works by putting data into a read queue to be
pulled out later by calling the `read()` method when the `'readable'`
event fires.
-->

`Readable` クラスは、`'readable'` イベントの生成時に `read()`
メソッドが呼び出されることで後から取り出されるデータを、
読み込みキューに入れておくことによって機能します。

<!--
The `push()` method will explicitly insert some data into the read
queue.  If it is called with `null` then it will signal the end of the
data.
-->

`push()` メソッドはいくつかのデータを明示的に読み込みキューに挿入します。
もし `null` と共に呼び出されると、それはデータが終了したことを伝えます。

<!--
In some cases, you may be wrapping a lower-level source which has some
sort of pause/resume mechanism, and a data callback.  In those cases,
you could wrap the low-level source object by doing something like
this:
-->

場合によっては、pause/resume メカニズムとデータのコールバックを持つ
低水準のソースをラップするかもしれません。
それらのケースでは、次のように低水準なソースオブジェクトを
ラップすることができます:

```javascript
// source is an object with readStop() and readStart() methods,
// and an `ondata` member that gets called when it has data, and
// an `onend` member that gets called when the data is over.

var stream = new Readable();

source.ondata = function(chunk) {
  // if push() returns false, then we need to stop reading from source
  if (!stream.push(chunk))
    source.readStop();
};

source.onend = function() {
  stream.push(null);
};

// _read will be called when the stream wants to pull more data in
// the advisory size argument is ignored in this case.
stream._read = function(n) {
  source.readStart();
};
```

### readable.unshift(chunk)

<!--
* `chunk` {Buffer | null | String} Chunk of data to unshift onto the read queue
* return {Boolean} Whether or not more pushes should be performed
-->

* `chunk` {Buffer | null | String} 読み込みキューの先頭に戻されるデータのチャンク
* return {Boolean} より多くの追加が実行されるべきかどうか

<!--
Note: **This function should usually be called by Readable consumers,
NOT by implementors of Readable subclasses.**  It does not indicate
the end of a `_read()` transaction in the way that
`readable.push(chunk)` does.  If you find that you have to call
`this.unshift(chunk)` in your Readable class, then there's a good
chance you ought to be using the
[stream.Transform](#stream_class_stream_transform) class instead.
-->

注意: **この関数は通常、Readable サブクラスの実装者ではなく、
Readable の消費者から呼び出されるべきものです。**
それは `readable.push(chunk)` のように `_read()` トランザクションの終了を
示すことはありません。
もし Readable クラスの中で `this.unshift(chunk)` を呼び出す必要性を
見出したなら、代わりに [stream.Transform](#stream_class_stream_transform) を
使用すべき可能性があります。

<!--
This is the corollary of `readable.push(chunk)`.  Rather than putting
the data at the *end* of the read queue, it puts it at the *front* of
the read queue.
-->

これは `readable.push(chunk)` からの帰結です。
データを読み込みキューの *末尾* に加えるのではなく、
読み込みキューの *先頭* に加えます。

<!--
This is useful in certain cases where a stream is being consumed by a
parser, which needs to "un-consume" some data that it has
optimistically pulled out of the source, so that the stream can be
passed on to some other party.
-->

これはストリームがパーサによって消費されるケースにおいて有用です。
それはソースから楽観的に取り出したデータを「消費しなかった」ことにして、
ストリームが他のところにデータを渡せるようにする場合に必要です。

```javascript
// A parser for a simple data protocol.
// The "header" is a JSON object, followed by 2 \n characters, and
// then a message body.
//
// NOTE: This can be done more simply as a Transform stream!
// Using Readable directly for this is sub-optimal.  See the
// alternative example below under the Transform section.

function SimpleProtocol(source, options) {
  if (!(this instanceof SimpleProtocol))
    return new SimpleProtocol(options);

  Readable.call(this, options);
  this._inBody = false;
  this._sawFirstCr = false;

  // source is a readable stream, such as a socket or file
  this._source = source;

  var self = this;
  source.on('end', function() {
    self.push(null);
  });

  // give it a kick whenever the source is readable
  // read(0) will not consume any bytes
  source.on('readable', function() {
    self.read(0);
  });

  this._rawHeader = [];
  this.header = null;
}

SimpleProtocol.prototype = Object.create(
  Readable.prototype, { constructor: { value: SimpleProtocol }});

SimpleProtocol.prototype._read = function(n) {
  if (!this._inBody) {
    var chunk = this._source.read();

    // if the source doesn't have data, we don't have data yet.
    if (chunk === null)
      return this.push('');

    // check if the chunk has a \n\n
    var split = -1;
    for (var i = 0; i < chunk.length; i++) {
      if (chunk[i] === 10) { // '\n'
        if (this._sawFirstCr) {
          split = i;
          break;
        } else {
          this._sawFirstCr = true;
        }
      } else {
        this._sawFirstCr = false;
      }
    }

    if (split === -1) {
      // still waiting for the \n\n
      // stash the chunk, and try again.
      this._rawHeader.push(chunk);
      this.push('');
    } else {
      this._inBody = true;
      var h = chunk.slice(0, split);
      this._rawHeader.push(h);
      var header = Buffer.concat(this._rawHeader).toString();
      try {
        this.header = JSON.parse(header);
      } catch (er) {
        this.emit('error', new Error('invalid simple protocol data'));
        return;
      }
      // now, because we got some extra data, unshift the rest
      // back into the read queue so that our consumer will see it.
      var b = chunk.slice(split);
      this.unshift(b);

      // and let them know that we are done parsing the header.
      this.emit('header', this.header);
    }
  } else {
    // from there on, just provide the data to our consumer.
    // careful not to push(null), since that would indicate EOF.
    var chunk = this._source.read();
    if (chunk) this.push(chunk);
  }
};

// Usage:
var parser = new SimpleProtocol(source);
// Now parser is a readable stream that will emit 'header'
// with the parsed header data.
```

### readable.wrap(stream)

<!--
* `stream` {Stream} An "old style" readable stream
-->

* `stream` {Stream} 「古いスタイル」の読み込み可能なストリーム

<!--
If you are using an older Node library that emits `'data'` events and
has a `pause()` method that is advisory only, then you can use the
`wrap()` method to create a Readable stream that uses the old stream
as its data source.
-->

もし `'data'` イベントを生成し、アドバイスだけを行う `pause()`
メソッドを持つ、古い Node ライブラリを使っているなら、
`wrap()`` メソッドは古いストリームをデータソースとして使用する
Readable ストリームを作成します。

<!--
For example:
-->

例:

```javascript
var OldReader = require('./old-api-module.js').OldReader;
var oreader = new OldReader;
var Readable = require('stream').Readable;
var myReader = new Readable().wrap(oreader);

myReader.on('readable', function() {
  myReader.read(); // etc.
});
```

### Event: 'readable'

<!--
When there is data ready to be consumed, this event will fire.
-->

データが消費される準備ができた時、このイベントが生成されます。

<!--
When this event emits, call the `read()` method to consume the data.
-->

このイベントが生成された場合、`read()` を呼んでデータを消費してください。

### Event: 'end'

<!--
Emitted when the stream has received an EOF (FIN in TCP terminology).
Indicates that no more `'data'` events will happen. If the stream is
also writable, it may be possible to continue writing.
-->

ストリームが EOF (TCP 用語では FIN) を受信した時に生成されます。
`'data'` イベントがもう発生しないことを示します。
もしストリームが書き込み可能でもあるなら、
書き込みを続けることは可能かもしれません。

### Event: 'data'

<!--
The `'data'` event emits either a `Buffer` (by default) or a string if
`setEncoding()` was used.
-->

`'data'` イベントは `Buffer` (デフォルト) または、`setEncoding()`
された場合は文字列のどちらかを生成します。

<!--
Note that adding a `'data'` event listener will switch the Readable
stream into "old mode", where data is emitted as soon as it is
available, rather than waiting for you to call `read()` to consume it.
-->

`'data'` イベントリスナが加えられた場合のみ、Readable ストリームは
「旧モード」に切り替わることに注意してください。
データを消費するために `read()` メソッドが呼び出されるまで待つ代わりに、
データが利用可能になるとすぐにイベントが生成されます。

### Event: 'error'

<!--
Emitted if there was an error receiving data.
-->

データ受信でエラーがあると生成されます。

### Event: 'close'

<!--
Emitted when the underlying resource (for example, the backing file
descriptor) has been closed. Not all streams will emit this.
-->

下層のリソース (例えば背後のファイル記述子) がクローズされた時に生成されます。
全てのストリームがこのイベントを発生するわけではありません。

### readable.setEncoding(encoding)

<!--
Makes the `'data'` event emit a string instead of a `Buffer`. `encoding`
can be `'utf8'`, `'utf16le'` (`'ucs2'`), `'ascii'`, or `'hex'`.
-->

`'data'` イベントが `Buffer` ではなく文字列を生成するようにします。
`encoding` には `'utf8'`、`'utf16le'` (`'ucs2'`)、`'ascii'`、
または `'hex'` を指定することができます。

<!--
The encoding can also be set by specifying an `encoding` field to the
constructor.
-->

エンコーディングはコンストラクタに渡す `encoding`
フィールドでも指定することができます。

### readable.read([size])

<!--
* `size` {Number | null} Optional number of bytes to read.
* Return: {Buffer | String | null}
-->

* `size` {Number | null} 読み込むバイト数 (任意)
* Return: {Buffer | String | null}

<!--
Note: **This function SHOULD be called by Readable stream users.**
-->

注意: **この関数は Redable ストリームのユーザが呼び出すべきです。**

<!--
Call this method to consume data once the `'readable'` event is
emitted.
-->

`'readable'` イベントが生成した後にデータを消費するために呼び出してください。

<!--
The `size` argument will set a minimum number of bytes that you are
interested in.  If not set, then the entire content of the internal
buffer is returned.
-->

`size` 引数は、必要な最小のバイト数を指定します。
指定しなかった場合、内部バッファ全体の内容が返されます。

<!--
If there is no data to consume, or if there are fewer bytes in the
internal buffer than the `size` argument, then `null` is returned, and
a future `'readable'` event will be emitted when more is available.
-->

もし消費できるデータがなかったり、内部バッファに `size`
引数で指定されたよりも少ないバイト数しかデータがなかったりした場合は、
`null` が返されます。
その後、より多くのデータが利用可能になると `'readable'` イベントが生成されます。

<!--
Calling `stream.read(0)` will always return `null`, and will trigger a
refresh of the internal buffer, but otherwise be a no-op.
-->

`stream.read(0)` は常に `null` を返します。
それは内部バッファをリフレッシュしますが、それ以外の処理はしません。

### readable.pipe(destination, [options])

<!--
* `destination` {Writable Stream}
* `options` {Object} Optional
  * `end` {Boolean} Default=true
-->

* `destination` {Writable Stream}
* `options` {Object} (任意)
  * `end` {Boolean} デフォルトは `true`

<!--
Connects this readable stream to `destination` WriteStream. Incoming
data on this stream gets written to `destination`.  Properly manages
back-pressure so that a slow destination will not be overwhelmed by a
fast readable stream.

読み込み可能なストリームを `destination` の書き込みストリームに接続します。
このストリームに入ってきたデータは `destination` に書き込まれます。
遅い出力先が速い読み込み可能なストリームに圧倒されないよう、
背圧を適切に管理します。

<!--
This function returns the `destination` stream.
-->

<!--
For example, emulating the Unix `cat` command:
-->

Unix の `cat` コマンドのエミュレートする例:

    process.stdin.pipe(process.stdout);

<!--
By default `end()` is called on the destination when the source stream
emits `end`, so that `destination` is no longer writable. Pass `{ end:
false }` as `options` to keep the destination stream open.
-->

デフォルトでは接続元ストリームで `end` イベントが生成されると、
接続先の `end()` が呼ばれるので、もう書き込みはできません。
`option` に `{ end: false }` を渡すと接続先はストリームはオープンされたままとなります。

<!--
This keeps `writer` open so that "Goodbye" can be written at the
end.
-->

これは `writer` をオープンしたままにして、最後に "Goodbye" と出力します。

    reader.pipe(writer, { end: false });
    reader.on("end", function() {
      writer.end("Goodbye\n");
    });

<!--
Note that `process.stderr` and `process.stdout` are never closed until
the process exits, regardless of the specified options.
-->

オプションの指定にかかわらず、`process.stderr` および `process.stdout`
はプロセスが終了するまで決してクローズされないことに注意してください。

### readable.unpipe([destination])

<!--
* `destination` {Writable Stream} Optional
-->

* `destination` {Writable Stream} (任意)

<!--
Undo a previously established `pipe()`.  If no destination is
provided, then all previously established pipes are removed.
-->

以前に構築した `pipe()` を取り消します。
もし接続先が与えられなかった場合は、
以前に構築された全てのパイプが削除されます。

### readable.pause()

<!--
Switches the readable stream into "old mode", where data is emitted
using a `'data'` event rather than being buffered for consumption via
the `read()` method.
-->

読み込み可能なストリームを「旧モード」に切り替えます。
データはバッファリングされて `read()` メソッドによって消費される代わりに、
`'data'` イベントによって生成されます。

<!--
Ceases the flow of data.  No `'data'` events are emitted while the
stream is in a paused state.
-->

データの流れは中断します。
ストリームが中断状態にある限り、`'data'` イベントは生成されません。

### readable.resume()

<!--
Switches the readable stream into "old mode", where data is emitted
using a `'data'` event rather than being buffered for consumption via
the `read()` method.
-->

読み込み可能なストリームを「旧モード」に切り替えます。
データはバッファリングされて `read()` メソッドによって消費される代わりに、
`'data'` イベントによって生成されます。

<!--
Resumes the incoming `'data'` events after a `pause()`.
-->

`pause()` の後で中断されていた `'data'` イベントを再開します。

## Class: stream.Writable

<!--type=class-->

<!--
A `Writable` Stream has the following methods, members, and events.
-->

`Writable` ストリームには以下のメソッド、メンバー、そしてイベントがあります。

<!--
Note that `stream.Writable` is an abstract class designed to be
extended with an underlying implementation of the
`_write(chunk, encoding, cb)` method. (See below.)
-->

`stream.Writable` は下層の実装である `_write(chunk, encoding, cb)`
メソッド (後述) によって拡張されるように設計された抽象クラスであることに
注意してください。

### new stream.Writable([options])

<!--
* `options` {Object}
  * `highWaterMark` {Number} Buffer level when `write()` starts
    returning false. Default=16kb
  * `decodeStrings` {Boolean} Whether or not to decode strings into
    Buffers before passing them to `_write()`.  Default=true
-->

* `options` {Object} (任意)
  * `highWaterMark` {Number} `write()` が `false` を返し始めるバッファレベル。
    デフォルトは 16kb。
  * `decodeStrings` {Boolean} 文字列が `_write()` に渡される前に
    バッファにデコードするかどうか。デフォルトは `true`。

<!--
In classes that extend the Writable class, make sure to call the
constructor so that the buffering settings can be properly
initialized.
-->

`Writable` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

### writable.\_write(chunk, encoding, callback)

<!--
* `chunk` {Buffer | String} The chunk to be written.  Will always
  be a buffer unless the `decodeStrings` option was set to `false`.
* `encoding` {String} If the chunk is a string, then this is the
  encoding type.  Ignore chunk is a buffer.  Note that chunk will
  **always** be a buffer unless the `decodeStrings` option is
  explicitly set to `false`.
* `callback` {Function} Call this function (optionally with an error
  argument) when you are done processing the supplied chunk.
-->

* `chunk` {Buffer | Array} 書き込まれるデータ。
  `decodeStrings` オプションが `false` に設定されない限り常にバッファです。
* `encoding` {String} チャンクが文字列の場合のエンコーディング方式。
  チャンクがバッファの場合は無視されます。
  `decodeStrings` オプションが明示的に `false` に設定されない限り、
  チャンクは *常に* バッファであるべき事に注意してください。
* `callback` {Function} チャンクを提供する処理が終了した時に、
  (任意のエラー引数と共に) この関数を呼び出してください。

<!--
All Writable stream implementations must provide a `_write` method to
send data to the underlying resource.
-->

全ての Writable ストリームは、下層のリソースにデータを
送るために `_write` メソッドを提供しなければなりません。

<!--
Note: **This function MUST NOT be called directly.**  It should be
implemented by child classes, and called by the internal Writable
class methods only.
-->

注意: **この関数は直接呼び出してはいけません。**
これはサブクラスで実装されるべきであり、Writable クラスの内部からのみ
呼び出されるべきです。

<!--
Call the callback using the standard `callback(error)` pattern to
signal that the write completed successfully or with an error.
-->

コールバックは出力が成功して完了したか、エラーが発生したかを伝えるために、
標準的な `callback(error)` パターンを使って呼び出します。

<!--
If the `decodeStrings` flag is set in the constructor options, then
`chunk` may be a string rather than a Buffer, and `encoding` will
indicate the sort of string that it is.  This is to support
implementations that have an optimized handling for certain string
data encodings.  If you do not explicitly set the `decodeStrings`
option to `false`, then you can safely ignore the `encoding` argument,
and assume that `chunk` will always be a Buffer.
-->

コンストラクタオプションの `decodeStrings` フラグがセットされると、
`chunk` を Buffer ではなく文字列にし、`encoding` でその文字列の
種類を示すことができます。
これは、実装が文字列データのエンコーディングを最適化できるようにするためです。
`decodeStrings` オプションを明示的に `false` に設定しない場合、
`endocing` 引数は安全に無視することができます。
そして `chunk` は常に Buffer であると見なせます。

<!--
This method is prefixed with an underscore because it is internal to
the class that defines it, and should not be called directly by user
programs.  However, you **are** expected to override this method in
your own extension classes.
-->

このメソッドはこれを定義するクラス内部のものであり、ユーザプログラムから
直接呼び出されるべきものではないため、アンダースコアの接頭辞を持ちます。
しかしながら、あなたの拡張クラスではこのメソッドをオーバーライドすることが
求められて**います**。

### writable.\_writev(chunks, callback)

<!--
* `chunks` {Array} The chunks to be written.  Each chunk has following
  format: `{ chunk: ..., encoding: ... }`.
* `callback` {Function} Call this function (optionally with an error
  argument) when you are done processing the supplied chunks.
-->

* `chunks` {Array} 書き込まれるチャンクの配列。
  各チャンクは次のフォーマットになります:
  `{ chunk: ..., encoding: ... }` 。
* `callback` {Function} チャンクを提供する処理が終了した時に、
  (任意のエラー引数と共に) この関数を呼び出してください。

<!--
This function is completely optional to implement. In the most cases
it is unnecessary.  If implemented, it will be called with all the
chunks that are buffered in the write queue.
-->

この関数の実装は完全に任意です。ほとんどのケースでは必要ありません。
もし実装されるなら、書き込みキューにバッファリングされた全ての
チャンクと共に呼び出されるでしょう。

### writable.write(chunk, [encoding], [callback])

<!--
* `chunk` {Buffer | String} Data to be written
* `encoding` {String} Optional.  If `chunk` is a string, then encoding
  defaults to `'utf8'`
* `callback` {Function} Optional.  Called when this chunk is
  successfully written.
* Returns {Boolean}
-->

* `chunk` {Buffer | String} 書き込まれるデータ
* `encoding` {String} `chunk` が文字列の場合のエンコーディング (任意)。
  デフォルトは `'utf8'`。
* `callback` {Function} チャンクが正常に書き込まれた場合に呼び出される (任意)。
* Returns {Boolean}


<!--
Writes `chunk` to the stream.  Returns `true` if the data has been
flushed to the underlying resource.  Returns `false` to indicate that
the buffer is full, and the data will be sent out in the future. The
`'drain'` event will indicate when the buffer is empty again.
-->

`chunk` をストリームに書き込みます。
データが下層のリソースに書き込まれると `true` を返します。
バッファが一杯であることを伝えるために `false` が返され、
データは将来送り出されます。
`'drain'` イベントはバッファが再び空になったことを伝えます。

<!--
The specifics of when `write()` will return false, is determined by
the `highWaterMark` option provided to the constructor.
-->

`write()` がいつ `false` を返すかは、コンストラクタに与えられる
`highWaterMark` オプションによって決定されます。

### writable.cork()

<!--
Forces buffering of all writes.
-->

全ての書き込みを強制的にバッファリングします。

<!--
Buffered data will be flushed either at `.uncork()` or at `.end()` call.
-->

バッファリングされたデータは `.uncork()` または `.end()` のどちらかが
呼び出されるとフラッシュされます。

### writable.uncork()

<!--
Flush all data, buffered since `.cork()` call.
-->

`.cork()` の呼び出しによってバッファリングされた
全てのデータをフラッシュします。

### writable.end([chunk], [encoding], [callback])

<!--
* `chunk` {Buffer | String} Optional final data to be written
* `encoding` {String} Optional.  If `chunk` is a string, then encoding
  defaults to `'utf8'`
* `callback` {Function} Optional.  Called when the final chunk is
  successfully written.
-->

* `chunk` {Buffer | String} 書き込まれる最後のデータ (任意)。
* `encoding` {String} `chunk` が文字列の場合のエンコーディング (任意)。
  デフォルトは `'utf8'`。
* `callback` {Function} 最後のチャンクが正常に書き込まれた場合に
  呼び出される (任意)。

<!--
Call this method to signal the end of the data being written to the
stream.
-->

ストリームに書き込まれるデータが終わることを合図するために、
このメソッドを呼び出してください。

### Event: 'drain'

<!--
Emitted when the stream's write queue empties and it's safe to write
without buffering again. Listen for it when `stream.write()` returns
`false`.
-->

ストリームの書き込みキューが空になり、バッファリングされることなく
再び安全に書き込みができるようになった場合に生成されます。
`stream.write()` が `false` を返した場合に監視してください。

### Event: 'error'

<!--
Emitted if there was an error receiving data.
-->

データ送信でエラーがあった場合に生成されます。

### Event: 'close'

<!--
Emitted when the underlying resource (for example, the backing file
descriptor) has been closed. Not all streams will emit this.
-->

下層のリソース (たとえば、背後のファイル記述子)
がクローズされた時に生成されます。
全てのストリームがこれを生成するわけではありません。

### Event: 'finish'

<!--
When `end()` is called and there are no more chunks to write, this
event is emitted.
-->

`end()` が呼び出されてそれ以上書き込まれるチャンクがない場合、
このイベントが生成されます。

### Event: 'pipe'

* `source` {Readable Stream}

<!--
Emitted when the stream is passed to a readable stream's pipe method.
-->

このストリームが読み込み可能ストリームの pipe メソッドに渡された時に生成されます。

### Event 'unpipe'

* `source` {Readable Stream}

<!--
Emitted when a previously established `pipe()` is removed using the
source Readable stream's `unpipe()` method.
-->

以前に構築された `pipe()` が、接続元である Readable ストリームの
`unpipe()` メソッドによって取り除かれた場合に生成されます。

## Class: stream.Duplex

<!--type=class-->

<!--
A "duplex" stream is one that is both Readable and Writable, such as a
TCP socket connection.
-->

"duplex" ストリームは、TCP ソケットコネクションのように
Readable であり Writable でもあるストリームの一種です。

<!--
Note that `stream.Duplex` is an abstract class designed to be
extended with an underlying implementation of the `_read(size)`
and `_write(chunk, encoding, callback)` methods as you would with a Readable or
Writable stream class.
-->

`stream.Duplex` は、Readable および Writable ストリームクラスと同様、
下層の実装である `_read(size)` および `_write(chunk, encoding, cb)`
メソッドによって拡張されるように設計された抽象クラスであることに
注意してください。

<!--
Since JavaScript doesn't have multiple prototypal inheritance, this
class prototypally inherits from Readable, and then parasitically from
Writable.  It is thus up to the user to implement both the lowlevel
`_read(n)` method as well as the lowlevel `_write(chunk, encoding, cb)` method
on extension duplex classes.
-->

JavaScript は複数のプロトタイプ継承を持つことができないため、
このクラスは Readable からプロトタイプを継承したうえで、
Writable から寄生的な方法 (プロトタイプメンバーのコピー) を行います。
低水準の `_read(n)` および `_write(chunk, encoding, cb)` を実装することは、
Duplex クラスを拡張するユーザの責務です。

### new stream.Duplex(options)

<!--
* `options` {Object} Passed to both Writable and Readable
  constructors. Also has the following fields:
  * `allowHalfOpen` {Boolean} Default=true.  If set to `false`, then
    the stream will automatically end the readable side when the
    writable side ends and vice versa.
-->

* `options` {Object} Writable および Readable のコンストラクタに渡されます。
  以下のフィールドを持つこともできます:
  * `allowHalfOpen` {Boolean} デフォルトは `true`。
    もし `false` に設定された場合、読み込み側が閉じられると
    自動的に書き込み側も閉じられます。

<!--
In classes that extend the Duplex class, make sure to call the
constructor so that the buffering settings can be properly
initialized.
-->

`Duplex` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

## Class: stream.Transform

<!--
A "transform" stream is a duplex stream where the output is causally
connected in some way to the input, such as a zlib stream or a crypto
stream.
-->

"transform" ストリームは、zlib ストリームや暗号化ストリームのように、
入力が何らかの方法で出力の元となっているような双方向ストリームです。

<!--
There is no requirement that the output be the same size as the input,
the same number of chunks, or arrive at the same time.  For example, a
Hash stream will only ever have a single chunk of output which is
provided when the input is ended.  A zlib stream will either produce
much smaller or much larger than its input.
-->

出力は、入力と同じサイズ、同じ数のチャンク、同時に到着することを
要求されません。
たとえば、Hash ストリームは入力が終了すると一つだけのチャンクを出力します。
zlib ストリームは、入力より小さいか、またはより大きい出力を生成します。

<!--
Rather than implement the `_read()` and `_write()` methods, Transform
classes must implement the `_transform()` method, and may optionally
also implement the `_flush()` method.  (See below.)
-->

`_read()` および `_write()` メソッドの代わりに、Transform クラスでは
`_transform()` メソッドを実装しなければなりません。
また、任意で `_flush()` メソッドを実装することもできます (後述)。

### new stream.Transform([options])

<!--
* `options` {Object} Passed to both Writable and Readable
  constructors.
-->

* `options` {Object} Writable および Readable のコンストラクタに渡されます。

<!--
In classes that extend the Transform class, make sure to call the
constructor so that the buffering settings can be properly
initialized.
-->

`Transform` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

### transform.\_transform(chunk, encoding, callback)

<!--
* `chunk` {Buffer | String} The chunk to be transformed.  Will always
  be a buffer unless the `decodeStrings` option was set to `false`.
* `encoding` {String} If the chunk is a string, then this is the
  encoding type.  (Ignore if `decodeStrings` chunk is a buffer.)
* `callback` {Function} Call this function (optionally with an error
  argument) when you are done processing the supplied chunk.
-->

* `chunk` {Buffer | Array} 書き込まれるデータ。
  `decodeStrings` オプションが `false` に設定されない限り常にバッファです。
* `encoding` {String} チャンクが文字列の場合のエンコーディング方式
  (チャンクがバッファの場合は無視されます)。
* `callback` {Function} チャンクを提供する処理が終了した時に、
  (任意のエラー引数と共に) この関数を呼び出してください。

<!--
Note: **This function MUST NOT be called directly.**  It should be
implemented by child classes, and called by the internal Transform
class methods only.
-->

注意: **この関数は直接呼び出してはいけません。**
これはサブクラスで実装されるべきであり、Transform クラスの内部からのみ
呼び出されるべきです。

<!--
All Transform stream implementations must provide a `_transform`
method to accept input and produce output.
-->

全ての Transform ストリームの実装は、入力を受け取って出力を提供するために
`_transform` メソッドを提供しなければなりません。

<!--
`_transform` should do whatever has to be done in this specific
Transform class, to handle the bytes being written, and pass them off
to the readable portion of the interface.  Do asynchronous I/O,
process things, and so on.
-->

書き込まれるバイトを処理し、読み込み可能なインタフェースに渡すなど、
Transform クラスでしなければならないことは全て `_transform`
で行わなければなりません。非同期 I/O、何かの処理、その他。

<!--
Call `transform.push(outputChunk)` 0 or more times to generate output
from this input chunk, depending on how much data you want to output
as a result of this chunk.
-->

この入力チャンクからの出力を生成するために、`transform.push(outputChunk)`
を 0 回以上呼び出してください。
それはこのチャンクの結果としてどれだけのデータを出力したいのかに依存します。

<!--
Call the callback function only when the current chunk is completely
consumed.  Note that there may or may not be output as a result of any
particular input chunk.
-->

現在のチャンクの処理が完全に終了した場合のみ、コールバック関数を呼び出します。
特定の入力チャンクからの結果として、出力があるかもしれないし、
無いかもしれないことに注意してください。

<!--
This method is prefixed with an underscore because it is internal to
the class that defines it, and should not be called directly by user
programs.  However, you **are** expected to override this method in
your own extension classes.
-->

このメソッドはこれを定義するクラス内部のものであり、ユーザプログラムから
直接呼び出されるべきものではないため、アンダースコアの接頭辞を持ちます。
しかしながら、あなたの拡張クラスではこのメソッドをオーバーライドすることが
求められて**います**。

### transform.\_flush(callback)

<!--
* `callback` {Function} Call this function (optionally with an error
  argument) when you are done flushing any remaining data.
-->

* `callback` {Function} 与えられたチャンクの処理が終了した場合に、
  (任意のエラー引数と共に) この関数を呼び出してください。

<!--
Note: **This function MUST NOT be called directly.**  It MAY be implemented
by child classes, and if so, will be called by the internal Transform
class methods only.
-->

注意: **この関数は直接呼び出してはいけません。**
これはサブクラスで実装されるかもしれず、Transform クラスの内部からのみ
呼び出されるべきです。

<!--
In some cases, your transform operation may need to emit a bit more
data at the end of the stream.  For example, a `Zlib` compression
stream will store up some internal state so that it can optimally
compress the output.  At the end, however, it needs to do the best it
can with what is left, so that the data will be complete.
-->

場合によっては、変換操作はストリームの終端でより多くのデータを
生成する必要があります。
たとえば、`Zlib` 圧縮ストリームは出力を最適に圧縮できるように、
いくつかの内部状態を持ちます。
一方、終端ではデータが完全なものになるように、
残されたものに最善を尽くす必要があります。

<!--
In those cases, you can implement a `_flush` method, which will be
called at the very end, after all the written data is consumed, but
before emitting `end` to signal the end of the readable side.  Just
like with `_transform`, call `transform.push(chunk)` zero or more
times, as appropriate, and call `callback` when the flush operation is
complete.
-->

この場合、最後の最後 (書き込まれた全てのデータが消費された後、
ただし読み込み側の終了を知らせる `end` が生成される前) に呼び出される
`_flush` メソッドを実装することができます。
`_transform` と同様、`transform.push(chunk)` を何度 (0 回以上) でも
適切に呼び出し、フラッシュ操作が完了した時に `callback` を呼び出します。

<!--
This method is prefixed with an underscore because it is internal to
the class that defines it, and should not be called directly by user
programs.  However, you **are** expected to override this method in
your own extension classes.
-->

このメソッドはこれを定義するクラス内部のものであり、ユーザプログラムから
直接呼び出されるべきものではないため、アンダースコアの接頭辞を持ちます。
しかしながら、あなたの拡張クラスではこのメソッドをオーバーライドすることが
求められて**います**。

### Example: `SimpleProtocol` parser

<!--
The example above of a simple protocol parser can be implemented much
more simply by using the higher level `Transform` stream class.
-->

前述した単純なプロトコルパーサの例は、より高水準な `Transform`
ストリームクラスを使うことで、さらにシンプルに実装することができます。

<!--
In this example, rather than providing the input as an argument, it
would be piped into the parser, which is a more idiomatic Node stream
approach.
-->

この例では、入力を引数で与えるのではなく、Node のストームにおける
より慣用的なアプローチとしてパーサにパイプで送られます。

```javascript
function SimpleProtocol(options) {
  if (!(this instanceof SimpleProtocol))
    return new SimpleProtocol(options);

  Transform.call(this, options);
  this._inBody = false;
  this._sawFirstCr = false;
  this._rawHeader = [];
  this.header = null;
}

SimpleProtocol.prototype = Object.create(
  Transform.prototype, { constructor: { value: SimpleProtocol }});

SimpleProtocol.prototype._transform = function(chunk, encoding, done) {
  if (!this._inBody) {
    // check if the chunk has a \n\n
    var split = -1;
    for (var i = 0; i < chunk.length; i++) {
      if (chunk[i] === 10) { // '\n'
        if (this._sawFirstCr) {
          split = i;
          break;
        } else {
          this._sawFirstCr = true;
        }
      } else {
        this._sawFirstCr = false;
      }
    }

    if (split === -1) {
      // still waiting for the \n\n
      // stash the chunk, and try again.
      this._rawHeader.push(chunk);
    } else {
      this._inBody = true;
      var h = chunk.slice(0, split);
      this._rawHeader.push(h);
      var header = Buffer.concat(this._rawHeader).toString();
      try {
        this.header = JSON.parse(header);
      } catch (er) {
        this.emit('error', new Error('invalid simple protocol data'));
        return;
      }
      // and let them know that we are done parsing the header.
      this.emit('header', this.header);

      // now, because we got some extra data, emit this first.
      this.push(chunk.slice(split));
    }
  } else {
    // from there on, just provide the data to our consumer as-is.
    this.push(chunk);
  }
  done();
};

var parser = new SimpleProtocol();
source.pipe(parser)

// Now parser is a readable stream that will emit 'header'
// with the parsed header data.
```


## Class: stream.PassThrough

<!--
This is a trivial implementation of a `Transform` stream that simply
passes the input bytes across to the output.  Its purpose is mainly
for examples and testing, but there are occasionally use cases where
it can come in handy.
-->

これは入力したバイト列を出力に単純に渡すだけの、
`Transform` のつまらない実装です。
これの主な目的はサンプル及びテストですが、何かと便利となるユースケースが
時折存在します。


[EventEmitter]: events.html#events_class_events_eventemitter
