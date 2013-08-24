# Stream

    Stability: 2 - Unstable

<!--
A stream is an abstract interface implemented by various objects in
Node.  For example a [request to an HTTP
server](http.html#http_http_incomingmessage) is a stream, as is
[stdout][]. Streams are readable, writable, or both. All streams are
instances of [EventEmitter][]
-->

ストリームは Node の様々なオブジェクトで実装される抽象的なインタフェースです。
例えば [HTTP サーバへのリクエスト](http.html#http_http_incomingmessage)は
[標準出力][]と同様にストリームです。
ストリームは読み込み可能、書き込み可能、またはその両方です。
全てのストリームは [EventEmitter][] のインスタンスです。

<!--
You can load the Stream base classes by doing `require('stream')`.
There are base classes provided for [Readable][] streams, [Writable][]
streams, [Duplex][] streams, and [Transform][] streams.
-->

Stream のベースクラスは `require('stream')` でロードすることができます。
[Readable][] ストリーム、[Writable][] ストリーム、[Duplex][] ストリーム、
[Transform][] ストリームのベースクラスが提供されます。

<!--
This document is split up into 3 sections.  The first explains the
parts of the API that you need to be aware of to use streams in your
programs.  If you never implement a streaming API yourself, you can
stop there.
-->

このドキュメントは 3 つのセクションに分かれています。
最初に、プログラムでストリームを利用するために知っておく必要がある
API について説明します。
もし独自のストリーミング API を実装しないのであれば、
そこで終わりにすることができます。

<!--
The second section explains the parts of the API that you need to use
if you implement your own custom streams yourself.  The API is
designed to make this easy for you to do.
-->

2番目のセクションでは、独自のストリームを実装する場合に必要となる
API について説明します。
この API はそれが簡単にできるように設計されています。

<!--
The third section goes into more depth about how streams work,
including some of the internal mechanisms and functions that you
should probably not modify unless you definitely know what you are
doing.
-->

3番目のセクションは、理解することなく変更してはならない
内部的なメカニズムや関数群を含めて、ストリームがどのように動作するかについて
より詳しく説明します。


## API for Stream Consumers

<!--type=misc-->

<!--
Streams can be either [Readable][], [Writable][], or both ([Duplex][]).
-->

ストリームは、[Readable][]、[Writable][]、またはその両方 ([Duplex][])
のいずれかになることができます。

<!--
All streams are EventEmitters, but they also have other custom methods
and properties depending on whether they are Readable, Writable, or
Duplex.
-->

全てのストリームは EventEmitter ですが、Readable、Writable、または Duplex
のいずれであるかによって、独自のメソッドやプロパティを持ちます。

<!--
If a stream is both Readable and Writable, then it implements all of
the methods and events below.  So, a [Duplex][] or [Transform][] stream is
fully described by this API, though their implementation may be
somewhat different.
-->

もしストリームが [Readable][] と[Writable][] の両方であるなら、
それは以下の全てのメソッドとイベントを実装します。
[Duplex][] または [Transform][] ストリームの実装は多少異なる場合がありますが、
この API によって詳細に説明されます。

<!--
It is not necessary to implement Stream interfaces in order to consume
streams in your programs.  If you **are** implementing streaming
interfaces in your own program, please also refer to
[API for Stream Implementors][] below.
-->

プログラムの中でストリームからのデータを消費するために、
ストリームのインターフェースを実装する必要はありません。
もしプログラムの中でストリーミングインターフェースを実装 **する** なら、
以下の [ストリーム実装者向けの API][] を参照してください。

<!--
Almost all Node programs, no matter how simple, use Streams in some
way.  Here is an example of using Streams in a Node program:
-->

ほとんど全ての Node プログラムは、どんなに単純であっても、
何らかの方法でストリームを利用します。
これはストリームを利用する Node プログラムの例です:

```javascript
var http = require('http');

var server = http.createServer(function (req, res) {
  // req is an http.IncomingMessage, which is a Readable Stream
  // res is an http.ServerResponse, which is a Writable Stream

  var body = '';
  // we want to get the data as utf8 strings
  // If you don't set an encoding, then you'll get Buffer objects
  req.setEncoding('utf8');

  // Readable streams emit 'data' events once a listener is added
  req.on('data', function (chunk) {
    body += chunk;
  })

  // the end event tells you that you have entire body
  req.on('end', function () {
    try {
      var data = JSON.parse(body);
    } catch (er) {
      // uh oh!  bad json!
      res.statusCode = 400;
      return res.end('error: ' + er.message);
    }

    // write back something interesting to the user:
    res.write(typeof data);
    res.end();
  })
})

server.listen(1337);

// $ curl localhost:1337 -d '{}'
// object
// $ curl localhost:1337 -d '"foo"'
// string
// $ curl localhost:1337 -d 'not json'
// error: Unexpected token o
```

### Class: stream.Readable

<!--type=class-->

<!--
The Readable stream interface is the abstraction for a *source* of
data that you are reading from.  In other words, data comes *out* of a
Readable stream.
-->

Readable ストリームのインターフェースは、あなたが読み込むデータの抽象的な
*発生源* です。言い換えると、データは Readable ストリームから *出て* きます。

<!--
A Readable stream will not start emitting data until you indicate that
you are ready to receive it.
-->

Readable ストリームは、あなたがデータを受け取る準備ができたと指示するまでは、
データの生成を開始しません。

<!--
Readable streams have two "modes": a **flowing mode** and a **non-flowing
mode**.  When in flowing mode, data is read from the underlying system
and provided to your program as fast as possible.  In non-flowing
mode, you must explicitly call `stream.read()` to get chunks of data
out.
-->

Readable ストリームは二つの "モード": **flowing モード** と
**non-flowing モード** を持っています。
flowing モードに入ると、データは下層のシステムから読み込まれると、
可能な限り素早くあなたのプログラムに届けられます。
non-flowing モードでは、データの断片を取り出すために、明示的に
`stream.read()` を呼び出す必要があります。

<!--
Examples of readable streams include:
-->

Readable ストリームを含む例:

<!--
* [http responses, on the client](http.html#http_http_incomingmessage)
* [http requests, on the server](http.html#http_http_incomingmessage)
* [fs read streams](fs.html#fs_class_fs_readstream)
* [zlib streams][]
* [crypto streams][]
* [tcp sockets][]
* [child process stdout and stderr][]
* [process.stdin][]
-->

* [クライアントの http レスポンス](http.html#http_http_incomingmessage)
* [サーバの http リクエスト](http.html#http_http_incomingmessage)
* [fs の ReadStream](fs.html#fs_class_fs_readstream)
* [zlib のストリーム][]
* [crypto のストリーム][]
* [tcp のソケット][]
* [child_process の標準出力と標準エラー出力][]
* [process.stdin][]

#### Event: 'readable'

<!--
When a chunk of data can be read from the stream, it will emit a
`'readable'` event.
-->

ストリームからデータの断片を読み込むことが可能となった時、
`'readable'` イベントが生成されます。

<!--
In some cases, listening for a `'readable'` event will cause some data
to be read into the internal buffer from the underlying system, if it
hadn't already.
-->

あるケースでは、`'readable'` イベントを監視することは下層のシステムからデータを内部バッファへ読み込む原因となります (それがまだ行われていなかった場合)。

```javascript
var readable = getReadableStreamSomehow();
readable.on('readable', function() {
  // there is some data to read now
})
```

<!--
Once the internal buffer is drained, a `readable` event will fire
again when more data is available.
-->

内部バッファが空になると、データが利用可能になった時に
`'readable'` イベントは再び生成されます。

#### Event: 'data'

<!--
* `chunk` {Buffer | String} The chunk of data.
-->

* `chunk` {Buffer | String} データの断片。

<!--
If you attach a `data` event listener, then it will switch the stream
into flowing mode, and data will be passed to your handler as soon as
it is available.
-->

`'data'` イベントのリスナを追加すると、ストリームは flowing モードに切り替わり、
データは利用可能になるとすぐにあなたのハンドラに渡されます。

<!--
If you just want to get all the data out of the stream as fast as
possible, this is the best way to do so.
-->

ストリームから出てくる全てのデータをできるだけ素早く欲しいのなら、
これが最善の方法です。

```javascript
var readable = getReadableStreamSomehow();
readable.on('data', function(chunk) {
  console.log('got %d bytes of data', chunk.length);
})
```

#### Event: 'end'

<!--
This event fires when no more data will be provided.
-->

このイベントは、提供するデータがもう無くなった場合に生成されます。

<!--
Note that the `end` event **will not fire** unless the data is
completely consumed.  This can be done by switching into flowing mode,
or by calling `read()` repeatedly until you get to the end.
-->

`'end'` イベントはデータが完全に消費されるまでは **生成されない**
ことに注意してください。
それは flowing モードに切り替えることによって、または終わりに達するまで
`read()` を繰り返し呼び出すことによって達成することができます。

```javascript
var readable = getReadableStreamSomehow();
readable.on('data', function(chunk) {
  console.log('got %d bytes of data', chunk.length);
})
readable.on('end', function() {
  console.log('there will be no more data.');
});
```

#### Event: 'close'

<!--
Emitted when the underlying resource (for example, the backing file
descriptor) has been closed. Not all streams will emit this.
-->

下層のリソース (例えば背後のファイル記述子) がクローズされた時に生成されます。
全てのストリームがこのイベントを発生するわけではありません。

#### Event: 'error'

<!--
Emitted if there was an error receiving data.
-->

データの受信でエラーがあると生成されます。

#### readable.read([size])

<!--
* `size` {Number} Optional argument to specify how much data to read.
* Return {String | Buffer | null}
-->

* `size` {Number} どれだけのデータを読み込むか指定するオプションの引数。
* Return {String | Buffer | null}

<!--
The `read()` method pulls some data out of the internal buffer and
returns it.  If there is no data available, then it will return
`null`.
-->

`read()` メソッドは内部バッファからデータを取り出して返します。
もし利用可能なデータが無ければ、`null` を返します。

<!--
If you pass in a `size` argument, then it will return that many
bytes.  If `size` bytes are not available, then it will return `null`.
-->

`size` 引数を指定すると、その長さ (バイト数または文字数) のデータを返します。
もし `size` で指定された長さのデータが揃っていない場合は `null` を返します。

<!--
If you do not specify a `size` argument, then it will return all the
data in the internal buffer.
-->

`size` 引数を指定しなかった場合は、内部バッファにある全てのデータが返されます。

<!--
This method should only be called in non-flowing mode.  In
flowing-mode, this method is called automatically until the internal
buffer is drained.
-->

このメソッドは non-flowing モードの場合に限って呼び出されるべきです。
flowing モードでは、内部バッファが空になるまで
このメソッドは自動的に呼び出されます。

```javascript
var readable = getReadableStreamSomehow();
readable.on('readable', function() {
  var chunk;
  while (null !== (chunk = readable.read())) {
    console.log('got %d bytes of data', chunk.length);
  }
});
```

#### readable.setEncoding(encoding)

<!--
* `encoding` {String} The encoding to use.
-->

* `encoding` {String} 使用するエンコーディング。

<!--
Call this function to cause the stream to return strings of the
specified encoding instead of Buffer objects.  For example, if you do
`readable.setEncoding('utf8')`, then the output data will be
interpreted as UTF-8 data, and returned as strings.  If you do
`readable.setEncoding('hex')`, then the data will be encoded in
hexadecimal string format.
-->

この関数を呼び出すと、ストリームは Buffer オブジェクトの代わりに
指定されたエンコーディングによる文字列を返すようになります。
例えば、`readable.setEncoding('utf8')` とすると、得られるデータは
UTF-8 のデータとして解釈され、文字列が返されます。
`readable.setEncoding('hex')` とすると、データは 16 進フォーマットの
文字列にエンコードされます。

<!--
This properly handles multi-byte characters that would otherwise be
potentially mangled if you simply pulled the Buffers directly and
called `buf.toString(encoding)` on them.  If you want to read the data
as strings, always use this method.
-->

これは、Buffer を直接取得して単純に `buf.toString(encoding)`
を呼び出した場合は潜在的にめちゃくちゃになるのとは異なり、
マルチバイト文字を正しく扱います。
データを文字列として読み込みたければ、常にこのメソッドを使用してください。

```javascript
var readable = getReadableStreamSomehow();
readable.setEncoding('utf8');
readable.on('data', function(chunk) {
  assert.equal(typeof chunk, 'string');
  console.log('got %d characters of string data', chunk.length);
})
```

#### readable.resume()

<!--
This method will cause the readable stream to resume emitting `data`
events.
-->

このメソッドは Readable ストリームが `'data'` イベントの生成を
再開するようにします。

<!--
This method will switch the stream into flowing-mode.  If you do *not*
want to consume the data from a stream, but you *do* want to get to
its `end` event, you can call `readable.resume()` to open the flow of
data.
-->

このメソッドはストリームを flowing モードに切り替えます。
もしストリームからのデータを消費する必要が *なく*、しかし `'end'` イベントを
受け取る必要が *ある* なら、`readable.resume()` を呼び出してデータのフローを
開くことができます。

```javascript
var readable = getReadableStreamSomehow();
readable.resume();
readable.on('end', function(chunk) {
  console.log('got to the end, but did not read anything');
})
```

#### readable.pause()

<!--
This method will cause a stream in flowing-mode to stop emitting
`data` events.  Any data that becomes available will remain in the
internal buffer.
-->

このメソッドはストリームを flowing モードに切り替えて、
`'data'` イベントの生成を中断します。
利用可能になったデータは内部バッファの中に残ります。

<!--
This method is only relevant in flowing mode.  When called on a
non-flowing stream, it will switch into flowing mode, but remain
paused.
-->

このメソッドは flowing モードにだけ関連します。
non-flowing モードのストリームで呼ばれた場合は、flowing モードに切り替わり、
残りのデータは休止します。

```javascript
var readable = getReadableStreamSomehow();
readable.on('data', function(chunk) {
  console.log('got %d bytes of data', chunk.length);
  readable.pause();
  console.log('there will be no more data for 1 second');
  setTimeout(function() {
    console.log('now data will start flowing again');
    readable.resume();
  }, 1000);
})
```

#### readable.pipe(destination, [options])

<!--
* `destination` {[Writable][] Stream} The destination for writing data
* `options` {Object} Pipe options
  * `end` {Boolean} End the writer when the reader ends. Default = `true`
-->

* `destination` {[Writable][] Stream} データの書き込み先。
* `options` {Object} パイプオプション
  * `end` {Boolean} 読み込み元が終了すると書き込み先を終了します。
    デフォルトは `true`

<!--
This method pulls all the data out of a readable stream, and writes it
to the supplied destination, automatically managing the flow so that
the destination is not overwhelmed by a fast readable stream.
-->

このメソッドは Readable ストリームから全てのデータを引き出し、
与えられた行き先に書き込みます。
高速な Readable ストリームによって出力先が圧迫されないように、
自動的にフロー制御を行います。

<!--
Multiple destinations can be piped to safely.
-->

複数の出力先を安全に連結することができます。

```javascript
var readable = getReadableStreamSomehow();
var writable = fs.createWriteStream('file.txt');
// All the data from readable goes into 'file.txt'
readable.pipe(writable);
```

<!--
This function returns the destination stream, so you can set up pipe
chains like so:
-->

この関数は出力先となるストリーム返すので、このようにパイプのチェーンを
組み立てることができます。

```javascript
var r = fs.createReadStream('file.txt');
var z = zlib.createGzip();
var w = fs.createWriteStream('file.txt.gz');
r.pipe(z).pipe(w);
```

<!--
For example, emulating the Unix `cat` command:
-->

Unix の `cat` コマンドをエミュレートする例:

```javascript
process.stdin.pipe(process.stdout);
```

<!--
By default [`end()`][] is called on the destination when the source stream
emits `end`, so that `destination` is no longer writable. Pass `{ end:
false }` as `options` to keep the destination stream open.
-->

デフォルトでは、出力先の [`end()`][] は入力元のストリームで
`'end'` が生成された時に呼び出されます。そのため、`destination`
はもう書き込み可能ではなくなります。
`{end: false }` を `options` として渡すことにより、出力先ストリームを
オープンしたままにしておくことができます。

<!--
This keeps `writer` open so that "Goodbye" can be written at the
end.
-->

これは `writer` をオープンしたままにすることにより、最後に
`"Goodbye"` と書き込むことができます。

```javascript
reader.pipe(writer, { end: false });
reader.on('end', function() {
  writer.end('Goodbye\n');
});
```

<!--
Note that `process.stderr` and `process.stdout` are never closed until
the process exits, regardless of the specified options.
-->

`process.stderr` および `process.stdout` は、オプションの指定に関係なく、
プロセスが終了するまで決してクローズされないことに注意してください。

#### readable.unpipe([destination])

<!--
* `destination` {[Writable][] Stream} Optional specific stream to unpipe
-->

* `destination` {[Writable][] Stream} オプションのパイプを解除するストリーム

<!--
This method will remove the hooks set up for a previous `pipe()` call.
-->

このメソッドは以前の `pipe()` 呼び出しで設定されたフックを取り除きます。

<!--
If the destination is not specified, then all pipes are removed.
-->

`destination` が指定されなかった場合は、全てのパイプが取り除かれます。

<!--
If the destination is specified, but no pipe is set up for it, then
this is a no-op.
-->

`destination` が指定されたものの、それがパイプされていなかった場合、
これは何もしません。

```javascript
var readable = getReadableStreamSomehow();
var writable = fs.createWriteStream('file.txt');
// All the data from readable goes into 'file.txt',
// but only for the first second
readable.pipe(writable);
setTimeout(function() {
  console.log('stop writing to file.txt');
  readable.unpipe(writable);
  console.log('manually close the file stream');
  writable.end();
}, 1000);
```

#### readable.unshift(chunk)

<!--
* `chunk` {Buffer | String} Chunk of data to unshift onto the read queue
-->

* `chunk` {Buffer | String} 読み込みキューの先頭に戻されるデータの断片

<!--
This is useful in certain cases where a stream is being consumed by a
parser, which needs to "un-consume" some data that it has
optimistically pulled out of the source, so that the stream can be
passed on to some other party.
-->

これはストリームがパーサによって消費されるケースにおいて有用です。
それはソースから楽観的に取り出したデータを「消費しなかった」ことにして、
ストリームが他のところにデータを渡せるようにする場合に必要です。

<!--
If you find that you must often call `stream.unshift(chunk)` in your
programs, consider implementing a [Transform][] stream instead.  (See API
for Stream Implementors, below.)
-->

`stream.unshift(chunk)` を頻繁に呼び出さなくてはならないとしたら、
代わりに [Transform][] ストリームを実装することを検討してください
(後述する [ストリーム実装者向けの API][] を参照してください)。

```javascript
// Pull off a header delimited by \n\n
// use unshift() if we get too much
// Call the callback with (error, header, stream)
var StringDecoder = require('string_decoder').StringDecoder;
function parseHeader(stream, callback) {
  stream.on('error', callback);
  stream.on('readable', onReadable);
  var decoder = new StringDecoder('utf8');
  var header = '';
  function onReadable() {
    var chunk;
    while (null !== (chunk = stream.read())) {
      var str = decoder.write(chunk);
      if (str.match(/\n\n/)) {
        // found the header boundary
        var split = str.split(/\n\n/);
        header += split.shift();
        var remaining = split.join('\n\n');
        var buf = new Buffer(remaining, 'utf8');
        if (buf.length)
          stream.unshift(buf);
        stream.removeListener('error', callback);
        stream.removeListener('readable', onReadable);
        // now the body of the message can be read from the stream.
        callback(null, header, stream);
      } else {
        // still reading the header.
        header += str;
      }
    }
  }
}
```

#### readable.wrap(stream)

<!--
* `stream` {Stream} An "old style" readable stream
-->

* `stream` {Stream} 「古いスタイル」の Readable ストリーム

<!--
Versions of Node prior to v0.10 had streams that did not implement the
entire Streams API as it is today.  (See "Compatibility" below for
more information.)
-->

v0.10 より前のバージョンの Node には、今日の全ストリーム API を実装していない
ストリームがありました (より詳細は後述する「[互換性][]」を参照してください)。

<!--
If you are using an older Node library that emits `'data'` events and
has a `pause()` method that is advisory only, then you can use the
`wrap()` method to create a [Readable][] stream that uses the old stream
as its data source.
-->

もし、`'data'` イベントを生成し、アドバイスだけを行う `pause()`
メソッドを持つ、古い Node ライブラリを使っているなら、
`wrap()` メソッドは古いストリームをデータソースとして使用する
[Readable][] ストリームを作成します。

<!--
You will very rarely ever need to call this function, but it exists
as a convenience for interacting with old Node programs and libraries.
-->

この関数を呼び出す必要は滅多にありませんが、これは古い Node
プログラム及びライブラリと相互作用するための利便性のために存在します。

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


### Class: stream.Writable

<!--type=class-->

<!--
The Writable stream interface is an abstraction for a *destination*
that you are writing data *to*.
-->

Writable ストリームのインターフェースは、あなたがデータを書き込む抽象的な
*行き先* です。

<!--
Examples of writable streams include:
-->

Writable ストリームを含む例:

<!--
* [http requests, on the client](http.html#http_class_http_clientrequest)
* [http responses, on the server](http.html#http_class_http_serverresponse)
* [fs write streams](fs.html#fs_class_fs_writestream)
* [zlib streams][]
* [crypto streams][]
* [tcp sockets][]
* [child process stdin](child_process.html#child_process_child_stdin)
* [process.stdout][], [process.stderr][]
-->

* [クライアントの http リクエスト](http.html#http_class_http_clientrequest)
* [サーバの http レスポンス](http.html#http_class_http_serverresponse)
* [fs の WriteStream](fs.html#fs_class_fs_writestream)
* [zlib のストリーム][]
* [crypto のストリーム][]
* [tcp のソケット][]
* [child_process の標準入力](child_process.html#child_process_child_stdin)
* [process.stdout][], [process.stderr][]

#### writable.write(chunk, [encoding], [callback])

<!--
* `chunk` {String | Buffer} The data to write
* `encoding` {String} The encoding, if `chunk` is a String
* `callback` {Function} Callback for when this chunk of data is flushed
* Returns: {Boolean} True if the data was handled completely.
-->

* `chunk` {String | Buffer} 書き込まれるデータ
* `encoding` {String} もし `chunk` が文字列なら、そのエンコーディング
* `callback` {Function} データが掃き出された時に呼び出されるコールバック
* Returns: {Boolean} データが完全に処理された場合は `true`。

<!--
This method writes some data to the underlying system, and calls the
supplied callback once the data has been fully handled.
-->

このメソッドはデータを下層のシステムに書き込み、データが完全に処理されると
与えられたコールバックを一度だけ呼び出します。

<!--
The return value indicates if you should continue writing right now.
If the data had to be buffered internally, then it will return
`false`.  Otherwise, it will return `true`.
-->

戻り値は書き込みをすぐに続けていいかどうかを示します。
もしデータが内部にバッファリングされなければならないなら `false` を返します。
そうでなければ `true` を返します。

<!--
This return value is strictly advisory.  You MAY continue to write,
even if it returns `false`.  However, writes will be buffered in
memory, so it is best not to do this excessively.  Instead, wait for
the `drain` event before writing more data.
-->

この戻り値は完全にアドバイス的です。
もしこれが `false` を返しても、あなたは書き込みを続けることが「できます」。
しかしながら、書き込まれたデータはメモリにバッファリングされるため、
これを過剰にしないことが最善です。
代わりに、より多くのデータを書く前に `'drain'` イベントを待機してください。

#### Event: 'drain'

<!--
If a [`writable.write(chunk)`][] call returns false, then the `drain`
event will indicate when it is appropriate to begin writing more data
to the stream.
-->

[`write(chunk, encoding, callback)`][] の呼び出しが `false` を返した場合、
より多くのデータをいつストリームに書き始めるのが適切かを
`'drain'` イベントによって示します。

```javascript
// Write the data to the supplied writable stream 1MM times.
// Be attentive to back-pressure.
function writeOneMillionTimes(writer, data, encoding, callback) {
  var i = 1000000;
  write();
  function write() {
    var ok = true;
    do {
      i -= 1;
      if (i === 0) {
        // last time!
        writer.write(data, encoding, callback);
      } else {
        // see if we should continue, or wait
        // don't pass the callback, because we're not done yet.
        ok = writer.write(data, encoding);
      }
    } while (i > 0 && ok);
    if (i > 0) {
      // had to stop early!
      // write some more once it drains
      writer.once('drain', write);
    }
  }
}
```

#### writable.end([chunk], [encoding], [callback])

<!--
* `chunk` {String | Buffer} Optional data to write
* `encoding` {String} The encoding, if `chunk` is a String
* `callback` {Function} Optional callback for when the stream is finished
-->

* `chunk` {String | Buffer} オプションの書き込まれるデータ
* `encoding` {String} もし `chunk` が文字列なら、そのエンコーディング
* `callback` {Function} ストリームが終了時に呼び出される、
  オプションのコールバック

<!--
Call this method when no more data will be written to the stream.  If
supplied, the callback is attached as a listener on the `finish` event.
-->

これ以上データをストリームに書き込まない場合に呼び出してください。
コールバックが与えられた場合、それは `'finish'` イベントのリスナとして
アタッチされます。

<!--
Calling [`write()`][] after calling [`end()`][] will raise an error.
-->

[`end()`][] を呼び出した後で [`write()`][] を呼び出すとエラーになります。

```javascript
// write 'hello, ' and then end with 'world!'
http.createServer(function (req, res) {
  res.write('hello, ');
  res.end('world!');
  // writing more now is not allowed!
});
```

#### Event: 'finish'

<!--
When the [`end()`][] method has been called, and all data has been flushed
to the underlying system, this event is emitted.
-->

[`end()`][] メソッドが呼び出されて、全てのデータが下層のシステムに
掃き出されると、このイベントが生成されます。

```javascript
var writer = getWritableStreamSomehow();
for (var i = 0; i < 100; i ++) {
  writer.write('hello, #' + i + '!\n');
}
writer.end('this is the end\n');
write.on('finish', function() {
  console.error('all writes are now complete.');
});
```

#### Event: 'pipe'

<!--
* `src` {[Readable][] Stream} source stream that is piping to this writable
-->

* `src` {[Readable][] Stream} この Writable ストリームにつながれた
  入力元のストリーム

<!--
This is emitted whenever the `pipe()` method is called on a readable
stream, adding this writable to its set of destinations.
-->

これは、Readable ストリームの `pipe()` メソッドが呼び出されて、
この Writable ストリームが出力先として加えられた時に生成されます。

```javascript
var writer = getWritableStreamSomehow();
var reader = getReadableStreamSomehow();
writer.on('pipe', function(src) {
  console.error('something is piping into the writer');
  assert.equal(src, reader);
});
reader.pipe(writer);
```

#### Event: 'unpipe'

<!--
* `src` {[Readable][] Stream} The source stream that [unpiped][] this writable
-->

* `src` {[Readable][] Stream} この Writable ストリームと[切り離された][]
  入力元の Readable ストリーム

<!--
This is emitted whenever the [`unpipe()`][] method is called on a
readable stream, removing this writable from its set of destinations.
-->

これは、Readable ストリームで [`unpipe()`][] メソッドが呼び出され、
この Writable ストリームが出力先から取り除かれた時に生成されます。

```javascript
var writer = getWritableStreamSomehow();
var reader = getReadableStreamSomehow();
writer.on('unpipe', function(src) {
  console.error('something has stopped piping into the writer');
  assert.equal(src, reader);
});
reader.pipe(writer);
reader.unpipe(writer);
```

### Class: stream.Duplex

<!--
Duplex streams are streams that implement both the [Readable][] and
[Writable][] interfaces.  See above for usage.
-->

Duplex ストリームは [Readable][] と [Writable][] 両方のインターフェースを
実装したストリームです。使い方は上記を参照してください。

<!--
Examples of Duplex streams include:
-->

Duplex ストリームを含む例:

<!--
* [tcp sockets][]
* [zlib streams][]
* [crypto streams][]
-->

* [tcp のソケット][]
* [zlib のストリーム][]
* [crypto のストリーム][]


### Class: stream.Transform

<!--
Transform streams are [Duplex][] streams where the output is in some way
computed from the input.  They implement both the [Readable][] and
[Writable][] interfaces.  See above for usage.
-->

Transform ストリームは、入力から何らかの方法で出力が計算される [Duplex][]
ストリームです。
それらは [Readable][] と [Writable][] 両方のインターフェースを実装します。
使い方は上記を参照してください。

<!--
Examples of Transform streams include:
-->

Transform ストリームを含む例:

<!--
* [zlib streams][]
* [crypto streams][]
-->

* [zlib のストリーム][]
* [crypto のストリーム][]


## API for Stream Implementors

<!--type=misc-->

<!--
To implement any sort of stream, the pattern is the same:
-->

どのストリームを実装する場合でも、パターンは同じです:

<!--
1. Extend the appropriate parent class in your own subclass.  (The
   [`util.inherits`][] method is particularly helpful for this.)
2. Call the appropriate parent class constructor in your constructor,
   to be sure that the internal mechanisms are set up properly.
2. Implement one or more specific methods, as detailed below.
-->

1. それぞれの親クラスを拡張して、独自のサブクラスを作成する
   (特に [`util.inherits`][] メソッドはそのために役立ちます)。
2. 内部のメカニズムがきちんとセットアップされることを確実にするために、
   サブクラスのコンストラクタの中から親クラスのコンストラクタを呼び出す。
3. 以下で詳述される、いくつかの特別なメソッドを実装する。

<!--
The class to extend and the method(s) to implement depend on the sort
of stream class you are writing:
-->

拡張するクラスと実装するメソッドは、あなたが書こうとしているストリームの種類に
依存します。

<!--
<table>
  <thead>
    <tr>
      <th>
        <p>Use-case</p>
      </th>
      <th>
        <p>Class</p>
      </th>
      <th>
        <p>Method(s) to implement</p>
      </th>
    </tr>
  </thead>
  <tr>
    <td>
      <p>Reading only</p>
    </td>
    <td>
      <p>[Readable](#stream_class_stream_readable_1)</p>
    </td>
    <td>
      <p><code>[_read][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>Writing only</p>
    </td>
    <td>
      <p>[Writable](#stream_class_stream_writable_1)</p>
    </td>
    <td>
      <p><code>[_write][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>Reading and writing</p>
    </td>
    <td>
      <p>[Duplex](#stream_class_stream_duplex_1)</p>
    </td>
    <td>
      <p><code>[_read][]</code>, <code>[_write][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>Operate on written data, then read the result</p>
    </td>
    <td>
      <p>[Transform](#stream_class_stream_transform_1)</p>
    </td>
    <td>
      <p><code>_transform</code>, <code>_flush</code></p>
    </td>
  </tr>
</table>
-->

<table>
  <thead>
    <tr>
      <th>
        <p>ユースケース</p>
      </th>
      <th>
        <p>クラス</p>
      </th>
      <th>
        <p>実装するメソッド</p>
      </th>
    </tr>
  </thead>
  <tr>
    <td>
      <p>読み込みのみ</p>
    </td>
    <td>
      <p>[Readable](#stream_class_stream_readable_1)</p>
    </td>
    <td>
      <p><code>[_read][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>書き込みのみ</p>
    </td>
    <td>
      <p>[Writable](#stream_class_stream_writable_1)</p>
    </td>
    <td>
      <p><code>[_write][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>読み込みと書き込み</p>
    </td>
    <td>
      <p>[Duplex](#stream_class_stream_duplex_1)</p>
    </td>
    <td>
      <p><code>[_read][]</code>, <code>[_write][]</code></p>
    </td>
  </tr>
  <tr>
    <td>
      <p>書き込まれたデータを変換し、その結果を読み込む</p>
    </td>
    <td>
      <p>[Transform](#stream_class_stream_transform_1)</p>
    </td>
    <td>
      <p><code>[_transform][]</code>, <code>[_flush][]</code></p>
    </td>
  </tr>
</table>

<!--
In your implementation code, it is very important to never call the
methods described in [API for Stream Consumers][] above.  Otherwise, you
can potentially cause adverse side effects in programs that consume
your streaming interfaces.
-->

あなたの実装コードの中では、決して [ストリーム利用者のための API][]
で説明されたメソッドを呼び出さないことがとても重要です。
そうでなければ、あなたのストリーミングインターフェースを利用するプログラムに
有害な副作用を引き起こす原因となり得ます。

### Class: stream.Readable

<!--type=class-->

<!--
`stream.Readable` is an abstract class designed to be extended with an
underlying implementation of the [`_read(size)`][] method.
-->

`stream.Readable` は抽象クラスで、下層の実装として [`_read(size)`][]
メソッドを実装することで拡張されるように設計されています。

<!--
Please see above under [API for Stream Consumers][] for how to consume
streams in your programs.  What follows is an explanation of how to
implement Readable streams in your programs.
-->

プログラムの中で Readable ストリームを利用する方法については、
前述の [ストリーム利用者のための API][] を参照してください。
この後に続くのは、あなたのプログラムの中で Readable ストリームを
実装する方法の説明です。

#### Example: A Counting Stream

<!--type=example-->

<!--
This is a basic example of a Readable stream.  It emits the numerals
from 1 to 1,000,000 in ascending order, and then ends.
-->

これは Readable ストリームの基本的な例です。
それは 1 から 1,000,000 までの数を昇順で生成し、そして終了します。

```javascript
var Readable = require('stream').Readable;
var util = require('util');
util.inherits(Counter, Readable);

function Counter(opt) {
  Readable.call(this, opt);
  this._max = 1000000;
  this._index = 1;
}

Counter.prototype._read = function() {
  var i = this._index++;
  if (i > this._max)
    this.push(null);
  else {
    var str = '' + i;
    var buf = new Buffer(str, 'ascii');
    this.push(buf);
  }
};
```

#### Example: SimpleProtocol v1 (Sub-optimal)

<!--
This is similar to the `parseHeader` function described above, but
implemented as a custom stream.  Also, note that this implementation
does not convert the incoming data to a string.
-->

これは前に説明した `parseHeader` 関数とよく似ていますが、
独自のストリームとして実装されています。
また、この実装は入ってくるデータを文字列に変換しないことに注意してください。

<!--
However, this would be better implemented as a [Transform][] stream.  See
below for a better implementation.
-->

しかしながら、これは [Transform][] ストリームを使うことでよりうまく実装できます。
後述のよりよい実装を参照してください。

```javascript
// A parser for a simple data protocol.
// The "header" is a JSON object, followed by 2 \n characters, and
// then a message body.
//
// NOTE: This can be done more simply as a Transform stream!
// Using Readable directly for this is sub-optimal.  See the
// alternative example below under the Transform section.

var Readable = require('stream').Readable;
var util = require('util');

util.inherits(SimpleProtocol, Readable);

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
// var parser = new SimpleProtocol(source);
// Now parser is a readable stream that will emit 'header'
// with the parsed header data.
```


#### new stream.Readable([options])

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
Readable constructor so that the buffering settings can be properly
initialized.
-->

`Readable` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

#### readable.\_read(size)

<!--
* `size` {Number} Number of bytes to read asynchronously
-->

* `size` {Number} 非同期に読み込むバイト数

<!--
Note: **Implement this function, but do NOT call it directly.**
-->

注意: **この関数を実装してください、しかし直接呼び出さないでください。**

<!--
This function should NOT be called directly.  It should be implemented
by child classes, and only called by the internal Readable class
methods.
-->

この関数は直接呼び出すべきではありません。
これはサブクラスで実装されるべきであり、Readable クラスの内部から
呼び出されるべきです。

<!--
All Readable stream implementations must provide a `_read` method to
fetch data from the underlying resource.
-->

全ての Readable ストリームは、下層のリソースからデータを
取得するために `_read()` メソッドを提供しなければなりません。

<!--
This method is prefixed with an underscore because it is internal to
the class that defines it, and should not be called directly by user
programs.  However, you **are** expected to override this method in
your own extension classes.
-->

このメソッドはこれを定義するクラス内部のものであり、ユーザプログラムから
直接呼び出されるべきものではないため、アンダースコアの接頭辞を持ちます。
しかしながら、あなたの拡張クラスではこのメソッドをオーバーライドすることが
**求められて**います。

<!--
When data is available, put it into the read queue by calling
`readable.push(chunk)`.  If `push` returns false, then you should stop
reading.  When `_read` is called again, you should start pushing more
data.
-->

データが利用可能になれば、`readable.push(chunk)` を呼び出すことで
それを読み込みキューに追加します。
`push()` が false を返した場合は、読み込みを止めるべきです。
`_read()` が再び呼び出された時が、さらに多くのデータを追加を開始すべき時です。

<!--
The `size` argument is advisory.  Implementations where a "read" is a
single call that returns data can use this to know how much data to
fetch.  Implementations where that is not relevant, such as TCP or
TLS, may ignore this argument, and simply provide data whenever it
becomes available.  There is no need, for example to "wait" until
`size` bytes are available before calling [`stream.push(chunk)`][].
-->

`size` 引数はアドバイス的です。
"read()" が一回の呼び出しでデータを返す実装では、
どれだけのデータを取得すべきか知るためにこれを使うことができます。
TCPやTLSなど、それに関連しない実装ではこの引数は無視され、
利用可能になったデータをシンプルに提供するかもしれません。
たとえば [`stream.push(chunk)`][] が呼び出されるより前に、
`size` バイトが利用可能になるまで「待つ」必要はありません。

#### readable.push(chunk, [encoding])

<!--
* `chunk` {Buffer | null | String} Chunk of data to push into the read queue
* `encoding` {String} Encoding of String chunks.  Must be a valid
  Buffer encoding, such as `'utf8'` or `'ascii'`
* return {Boolean} Whether or not more pushes should be performed
-->

* `chunk` {Buffer | null | String} 読み込みキューにプッシュされる、
  データのチャンク
* `encoding` {String} 文字列チャンクのエンコーディング。
  `'utf8'` や `'ascii'` など、Buffer の正しいエンコーディングの必要があります。
* return {Boolean} さらにプッシュしてもいいかどうか

<!--
Note: **This function should be called by Readable implementors, NOT
by consumers of Readable streams.**
-->

注意: **この関数は Readable の実装から呼び出されるべきものであり、
Readable ストリームの利用者が呼び出すべきではありません。**

<!--
The `_read()` function will not be called again until at least one
`push(chunk)` call is made.
-->

少なくとも一回は `push(chunk)` が呼び出されないと、`_read()` 関数が
再び呼び出されることはありません。

<!--
The `Readable` class works by putting data into a read queue to be
pulled out later by calling the `read()` method when the `'readable'`
event fires.
-->

`Readable` クラスは、`read()` メソッドが呼び出されることで
後から取り出されるデータを、`'readable'` イベントの生成時に
読み込みキューに入れておくことによって機能します。

<!--
The `push()` method will explicitly insert some data into the read
queue.  If it is called with `null` then it will signal the end of the
data (EOF).
-->

`push()` メソッドはいくつかのデータを明示的に読み込みキューに挿入します。
もし `null` と共に呼び出されると、それはデータが終了した (EOF) ことを伝えます。

<!--
This API is designed to be as flexible as possible.  For example,
you may be wrapping a lower-level source which has some sort of
pause/resume mechanism, and a data callback.  In those cases, you
could wrap the low-level source object by doing something like this:
-->

この API は可能な限り柔軟に設計されています。
例えば、ある種の中断／再開メカニズムとデータのコールバックを持つ、
より低水準のデータソースをラップするかもしれません。
それらのケースでは、このように低水準のソースオブジェクトを
ラップすることができます。

```javascript
// source is an object with readStop() and readStart() methods,
// and an `ondata` member that gets called when it has data, and
// an `onend` member that gets called when the data is over.

util.inherits(SourceWrapper, Readable);

function SourceWrapper(options) {
  Readable.call(this, options);

  this._source = getLowlevelSourceObject();
  var self = this;

  // Every time there's data, we push it into the internal buffer.
  this._source.ondata = function(chunk) {
    // if push() returns false, then we need to stop reading from source
    if (!self.push(chunk))
      self._source.readStop();
  };

  // When the source ends, we push the EOF-signalling `null` chunk
  this._source.onend = function() {
    self.push(null);
  };
}

// _read will be called when the stream wants to pull more data in
// the advisory size argument is ignored in this case.
SourceWrapper.prototype._read = function(size) {
  this._source.readStart();
};
```


### Class: stream.Writable

<!--type=class-->

<!--
`stream.Writable` is an abstract class designed to be extended with an
underlying implementation of the [`_write(chunk, encoding, callback)`][] method.
-->

`stream.Writable` は抽象クラスで、下層の実装として
[`_write(chunk, encoding, callback)`][] メソッドを実装することで
拡張されるように設計されています。

<!--
Please see above under [API for Stream Consumers][] for how to consume
writable streams in your programs.  What follows is an explanation of
how to implement Writable streams in your programs.
-->

プログラムの中で Writable ストリームを利用する方法については、
前述の [ストリーム利用者のための API][] を参照してください。
この後に続くのは、あなたのプログラムの中で Writable ストリームを
実装する方法の説明です。

#### new stream.Writable([options])

<!--
* `options` {Object}
  * `highWaterMark` {Number} Buffer level when [`write()`][] starts
    returning false. Default=16kb
  * `decodeStrings` {Boolean} Whether or not to decode strings into
    Buffers before passing them to [`_write()`][].  Default=true
-->

* `options` {Object} (任意)
  * `highWaterMark` {Number} [`write()`][] が `false` を返し始める
    バッファレベル。デフォルトは 16kb。
  * `decodeStrings` {Boolean} 文字列が [`_write()`][] に渡される前に
    バッファにデコードするかどうか。デフォルトは `true`。

<!--
In classes that extend the Writable class, make sure to call the
constructor so that the buffering settings can be properly
initialized.
-->

`Writable` クラスを拡張するクラスでは、バッファリングの設定を確実に
初期化することができるように、必ずコンストラクタを呼び出してください。

#### writable.\_write(chunk, encoding, callback)

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
All Writable stream implementations must provide a [`_write()`][]
method to send data to the underlying resource.
-->

全ての Writable ストリームは、下層のリソースにデータを
送るために [`_write()`][] メソッドを提供しなければなりません。

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
**求められて**います。


### Class: stream.Duplex

<!--type=class-->

<!--
A "duplex" stream is one that is both Readable and Writable, such as a
TCP socket connection.
-->

"duplex" ストリームは、TCP ソケットコネクションのように
Readable であり Writable でもあるストリームの一種です。

<!--
Note that `stream.Duplex` is an abstract class designed to be extended
with an underlying implementation of the `_read(size)` and
[`_write(chunk, encoding, callback)`][] methods as you would with a
Readable or Writable stream class.
-->

`stream.Duplex` は、Readable および Writable ストリームクラスと同様、
下層の実装である [`_read(size)`][] および
[`_write(chunk, encoding, callback)`][] メソッドによって拡張されるように
設計された抽象クラスであることに注意してください。

<!--
Since JavaScript doesn't have multiple prototypal inheritance, this
class prototypally inherits from Readable, and then parasitically from
Writable.  It is thus up to the user to implement both the lowlevel
`_read(n)` method as well as the lowlevel
[`_write(chunk, encoding, callback)`][] method on extension duplex classes.
-->

JavaScript は複数のプロトタイプ継承を持つことができないため、
このクラスは Readable からプロトタイプを継承したうえで、
Writable から寄生的な方法 (プロトタイプメンバーのコピー) を行います。
低水準の [`_read(size)`][] および [`_write(chunk, encoding, callback)`][]
を実装することは、Duplex クラスを拡張するユーザの責務です。

#### new stream.Duplex(options)

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


### Class: stream.Transform

<!--
A "transform" stream is a duplex stream where the output is causally
connected in some way to the input, such as a [zlib][] stream or a
[crypto][] stream.
-->

"Transform" ストリームは、[zlib][] ストリームや [crypto][] ストリームのように、
入力が何らかの方法で出力の元となっているような Duplex ストリームです。

<!--
There is no requirement that the output be the same size as the input,
the same number of chunks, or arrive at the same time.  For example, a
Hash stream will only ever have a single chunk of output which is
provided when the input is ended.  A zlib stream will produce output
that is either much smaller or much larger than its input.
-->

出力は、入力と同じサイズ、同じ数のチャンク、同時に到着することを
要求されません。
たとえば、Hash ストリームは入力が終了すると一つだけのチャンクを出力します。
zlib ストリームは、入力より小さいか、またはより大きい出力を生成します。

<!--
Rather than implement the [`_read()`][] and [`_write()`][] methods, Transform
classes must implement the `_transform()` method, and may optionally
also implement the `_flush()` method.  (See below.)
-->

[`_read()`][] および [`_write()`][] メソッドの代わりに、Transform クラスでは
`_transform()` メソッドを実装しなければなりません。
また、任意で `_flush()` メソッドを実装することもできます (後述)。

#### new stream.Transform([options])

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

#### transform.\_transform(chunk, encoding, callback)

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
`_transform()` メソッドを提供しなければなりません。

<!--
`_transform` should do whatever has to be done in this specific
Transform class, to handle the bytes being written, and pass them off
to the readable portion of the interface.  Do asynchronous I/O,
process things, and so on.
-->

書き込まれるバイトを処理し、読み込み可能なインタフェースに渡すなど、
Transform クラスでしなければならないことは全て `_transform()`
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
**求められて**います。

#### transform.\_flush(callback)

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
ただし読み込み側の終了を知らせる `'end'` が生成される前) に呼び出される
`_flush()` メソッドを実装することができます。
`_transform()` と同様、`transform.push(chunk)` を何度 (0 回以上) でも
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
**求められて**います。

#### Example: `SimpleProtocol` parser v2

<!--
The example above of a simple protocol parser can be implemented
simply by using the higher level [Transform][] stream class, similar to
the `parseHeader` and `SimpleProtocol v1` examples above.
-->

前述した単純なプロトコルパーサの例は、より高水準な [Transform][] ストリームクラスを
使うことで、さらにシンプルに実装することができます。
前述の `parseHeader` および `SimpleProtocol v1` とよく似た例です。

<!--
In this example, rather than providing the input as an argument, it
would be piped into the parser, which is a more idiomatic Node stream
approach.
-->

この例では、入力を引数で与えるのではなく、Node のストームにおける
より慣用的なアプローチとしてパーサにパイプで送られます。

```javascript
var util = require('util');
var Transform = require('stream').Transform;
util.inherits(SimpleProtocol, Transform);

function SimpleProtocol(options) {
  if (!(this instanceof SimpleProtocol))
    return new SimpleProtocol(options);

  Transform.call(this, options);
  this._inBody = false;
  this._sawFirstCr = false;
  this._rawHeader = [];
  this.header = null;
}

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

// Usage:
// var parser = new SimpleProtocol();
// source.pipe(parser)
// Now parser is a readable stream that will emit 'header'
// with the parsed header data.
```


### Class: stream.PassThrough

<!--
This is a trivial implementation of a [Transform][] stream that simply
passes the input bytes across to the output.  Its purpose is mainly
for examples and testing, but there are occasionally use cases where
it can come in handy as a building block for novel sorts of streams.
-->

これは [Transform][] ストリームの取るに足らない実装で、
入力したバイト列を出力に単純に渡すだけです。
これの主な目的はサンプル及びテストですが、新しい種類のストリームのための
ビルディングブロックとして、何かと便利となるユースケースが時折存在します。


## Streams: Under the Hood

<!--type=misc-->

### Buffering

<!--type=misc-->

<!--
Both Writable and Readable streams will buffer data on an internal
object called `_writableState.buffer` or `_readableState.buffer`,
respectively.
-->

Readable 及び Writable ストリームはそれぞれ、`_writableState.buffer` または
`_readableState.buffer` と呼ばれる内部オブジェクトにデータを
バッファリングします。

<!--
The amount of data that will potentially be buffered depends on the
`highWaterMark` option which is passed into the constructor.
-->

バッファリングされるデータの量は、コンストラクタに渡される `highWaterMark`
オプションに依存します。

<!--
Buffering in Readable streams happens when the implementation calls
[`stream.push(chunk)`][].  If the consumer of the Stream does not call
`stream.read()`, then the data will sit in the internal queue until it
is consumed.
-->

Readable ストリームにおけるバッファリングは、実装が [`stream.push(chunk)`][]
を呼び出した時に起こります。
ストリームの利用者が `stream.read()` を呼び出さないと、
データはそれが消費されるまで内部キューに留まります。

<!--
Buffering in Writable streams happens when the user calls
[`stream.write(chunk)`][] repeatedly, even when `write()` returns `false`.
-->

Writable ストリームにおけるバッファリングは、利用者が [`stream.write(chunk)`][]
を繰り返し呼び出すと、`write()` が `false` を返した場合でも起こります。

<!--
The purpose of streams, especially with the `pipe()` method, is to
limit the buffering of data to acceptable levels, so that sources and
destinations of varying speed will not overwhelm the available memory.
-->

ストリーム、特に `pipe()` メソッドの目的は、データのバッファリングを
許容できるレベルに制限することです。そのため、様々な速度の入力元と出力先で、
利用可能なメモリを圧迫しません。

### `stream.read(0)`

<!--
There are some cases where you want to trigger a refresh of the
underlying readable stream mechanisms, without actually consuming any
data.  In that case, you can call `stream.read(0)`, which will always
return null.
-->

実際にデータを消費することなく、下層の Readable ストリームのメカニズムを
リフレッシュするきっかけが欲しくなるケースがあります。
そのケースでは、常に `null` を返す `stream.read(0)` を呼び出すことができます。

<!--
If the internal read buffer is below the `highWaterMark`, and the
stream is not currently reading, then calling `read(0)` will trigger
a low-level `_read` call.
-->

内部バッファが `highWaterMark` を下回っていて、
ストリームが現在読み込み中でなければ、`read(0)` の呼び出しは低水準の
`_read()` を呼び出すきっかけとなります。

<!--
There is almost never a need to do this.  However, you will see some
cases in Node's internals where this is done, particularly in the
Readable stream class internals.
-->

これをする必要はほとんどありません。
しかしながら Node の内部、特に Readable ストリームクラスの内部で、
これが使われているケースを見ることができるでしょう。

### `stream.push('')`

<!--
Pushing a zero-byte string or Buffer (when not in [Object mode][]) has an
interesting side effect.  Because it *is* a call to
[`stream.push()`][], it will end the `reading` process.  However, it
does *not* add any data to the readable buffer, so there's nothing for
a user to consume.
-->

ゼロバイトの長さの文字列またはバッファをプッシュすると、
([オブジェクトモード][]の場合を除き) 面白い副作用が起こります。
それは [`stream.push()`][] を呼び出すので、`reading` プロセスを終了します。
しかしながら、それは読み込みバッファにどんなデータも加え *ない* ので、
ユーザが消費するものは何もありません。

<!--
Very rarely, there are cases where you have no data to provide now,
but the consumer of your stream (or, perhaps, another bit of your own
code) will know when to check again, by calling `stream.read(0)`.  In
those cases, you *may* call `stream.push('')`.
-->

ごくまれに、今は提供するデータが無い場合があります。しかし、`stream.read(0)`
を呼び出すことにより、ストリームの利用者 (あるいは、もしかするとあなたの
コードの一部) は再びチェックすべきなのがいつかを知ることができます。
このケースでは、`stream.push('')` を呼び出すことが *できます* 。

<!--
So far, the only use case for this functionality is in the
[tls.CryptoStream][] class, which is deprecated in Node v0.12.  If you
find that you have to use `stream.push('')`, please consider another
approach, because it almost certainly indicates that something is
horribly wrong.
-->

現在の所、この機能の唯一のユースケースは v0.12 で廃止予定の
[tls.CryptoStream][] の中にあります。
もし `stream.push('')` を使わなければならないことになったら、それはおそらく
何かが恐ろしく間違っていることを示すので、他の方法を検討してください。

### Compatibility with Older Node Versions

<!--type=misc-->

<!--
In versions of Node prior to v0.10, the Readable stream interface was
simpler, but also less powerful and less useful.
-->

v0.10 より前のバージョンの Node では、Readable ストリームのインタフェースは
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

* `read()` メソッドが呼び出されるのを待つのではなく、
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
switch into "flowing mode" when a `'data'` event handler is added, or
when the `pause()` or `resume()` methods are called.  The effect is
that, even if you are not using the new `read()` method and
`'readable'` event, you no longer have to worry about losing `'data'`
chunks.
-->

Node v0.10 から、上記で説明した Readable クラスが追加されました。
古い Node プログラムとの後方互換性のために、Readable ストリームは
`'data'` イベントのハンドラが加えられた場合や、`pause()` または
`resume()` メソッドが読み出されると、「flowing モード」に切り替わります。
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

この状況の回避策は、「flowwing モード」の振る舞いへのトリガーとして
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
In addition to new Readable streams switching into flowing-mode, pre-v0.10
style streams can be wrapped in a Readable class using the `wrap()`
method.
-->

新しい Readable ストリームを flowing モードに切り替えられることに加えて、
`wrap()` メソッドを使って v0.10 より前のスタイルのストリームを
Readable クラスでラップすることもできます。


### Object Mode

<!--type=misc-->

<!--
Normally, Streams operate on Strings and Buffers exclusively.
-->

通常、ストリームは文字列またはバッファのみを扱います。

<!--
Streams that are in **object mode** can emit generic JavaScript values
other than Buffers and Strings.
-->

**オブジェクトモード** のストリームは、文字列及びバッファ以外の
一般的なJavaScriptの値を扱うことができます。

<!--
A Readable stream in object mode will always return a single item from
a call to `stream.read(size)`, regardless of what the size argument
is.
-->

オブジェクトモードの Readable ストリームは、`stream.read(size)` のサイズ引数が
いくつであるかに関わらず、常に一つの項目を返します。

<!--
A Writable stream in object mode will always ignore the `encoding`
argument to `stream.write(data, encoding)`.
-->

オブジェクトモードの Writable ストリームは、`stream.write(data, encoding)`
の `encoding` 引数を常に無視します。

<!--
The special value `null` still retains its special value for object
mode streams.  That is, for object mode readable streams, `null` as a
return value from `stream.read()` indicates that there is no more
data, and [`stream.push(null)`][] will signal the end of stream data
(`EOF`).
-->

特別な値 `null` は、オブジェクトモードのストリームにおいても
特別な値を持ちます。
すなわち、オブジェクトモードの Readable ストリームでは、`stream.read()`
の戻り値 `null` はもうデータが無いことを、[`stream.push(null)`][]
はストリームデータの終端を示します (`EOF`)。

<!--
No streams in Node core are object mode streams.  This pattern is only
used by userland streaming libraries.
-->

Node のコアライブラリにはオブジェクトモードのストリームは存在しません。
このパターンはユーザランドのライブラリでのみ使われます。

<!--
You should set `objectMode` in your stream child class constructor on
the options object.  Setting `objectMode` mid-stream is not safe.
-->

ストリームのサブクラスはコストラクタの `options` オブジェクトで `objectMode`
を設定すべきです。
`objectMode` をストリームの途中で設定することは安全ではありません。

### State Objects

<!--
[Readable][] streams have a member object called `_readableState`.
[Writable][] streams have a member object called `_writableState`.
[Duplex][] streams have both.
-->

[Readable][] ストリームは `_readableState` と呼ばれるメンバを持っています。
[Writable][] ストリームは `_writableState` と呼ばれるメンバを持っています。
[Duplex][] ストリームは両方を持っています。

<!--
**These objects should generally not be modified in child classes.**
However, if you have a Duplex or Transform stream that should be in
`objectMode` on the readable side, and not in `objectMode` on the
writable side, then you may do this in the constructor by setting the
flag explicitly on the appropriate state object.
-->

**通常、これらのオブジェクトはサブクラスで変更すべきではありません。**
しかしながら、もし Duplex または Transform ストリームの読み込み側が
`objectMode` で、書き込み側が `objectMode` ではない場合、コンストラクタで
適切なステートオブジェクトにフラグを明示的に設定することになるかもしれません。

```javascript
var util = require('util');
var StringDecoder = require('string_decoder').StringDecoder;
var Transform = require('stream').Transform;
util.inherits(JSONParseStream, Transform);

// Gets \n-delimited JSON string data, and emits the parsed objects
function JSONParseStream(options) {
  if (!(this instanceof JSONParseStream))
    return new JSONParseStream(options);

  Transform.call(this, options);
  this._writableState.objectMode = false;
  this._readableState.objectMode = true;
  this._buffer = '';
  this._decoder = new StringDecoder('utf8');
}

JSONParseStream.prototype._transform = function(chunk, encoding, cb) {
  this._buffer += this._decoder.write(chunk);
  // split on newlines
  var lines = this._buffer.split(/\r?\n/);
  // keep the last partial line buffered
  this._buffer = lines.pop();
  for (var l = 0; l < lines.length; l++) {
    var line = lines[l];
    try {
      var obj = JSON.parse(line);
    } catch (er) {
      this.emit('error', er);
      return;
    }
    // push the parsed object out to the readable consumer
    this.push(obj);
  }
  cb();
};

JSONParseStream.prototype._flush = function(cb) {
  // Just handle any leftover
  var rem = this._buffer.trim();
  if (rem) {
    try {
      var obj = JSON.parse(rem);
    } catch (er) {
      this.emit('error', er);
      return;
    }
    // push the parsed object out to the readable consumer
    this.push(obj);
  }
  cb();
};
```

<!--
The state objects contain other useful information for debugging the
state of streams in your programs.  It is safe to look at them, but
beyond setting option flags in the constructor, it is **not** safe to
modify them.
-->

ステートオブジェクトは、デバッグで役に立つストリームの状態を
情報として持ちます。それを見ることは安全ですが、しかしコンストラクタで設定した
オプションフラグを変更することは安全では **ありません**。


<!--
[EventEmitter]: events.html#events_class_events_eventemitter
[Object mode]: #stream_object_mode
[`stream.push(chunk)`]: #stream_readable_push_chunk_encoding
[`stream.push(null)`]: #stream_readable_push_chunk_encoding
[`stream.push()`]: #stream_readable_push_chunk_encoding
[`unpipe()`]: #stream_readable_unpipe_destination
[unpiped]: #stream_readable_unpipe_destination
[tcp sockets]: net.html#net_class_net_socket
[zlib streams]: zlib.html
[zlib]: zlib.html
[crypto streams]: crypto.html
[crypto]: crypto.html
[tls.CryptoStream]: tls.html#tls_class_cryptostream
[process.stdin]: process.html#process_process_stdin
[stdout]: process.html#process_process_stdout
[process.stdout]: process.html#process_process_stdout
[process.stderr]: process.html#process_process_stderr
[child process stdout and stderr]: child_process.html#child_process_child_stdout
[API for Stream Consumers]: #stream_api_for_stream_consumers
[API for Stream Implementors]: #stream_api_for_stream_implementors
[Readable]: #stream_class_stream_readable
[Writable]: #stream_class_stream_writable
[Duplex]: #stream_class_stream_duplex
[Transform]: #stream_class_stream_transform
[`_read(size)`]: #stream_readable_read_size_1
[`_read()`]: #stream_readable_read_size_1
[_read]: #stream_readable_read_size_1
[`write(chunk, encoding, callback)`]: #stream_writable_write_chunk_encoding_callback
[`write()`]: #stream_writable_write_chunk_encoding_callback
[`stream.write(chunk)`]: #stream_writable_write_chunk_encoding_callback
[`_write(chunk, encoding, callback)`]: #stream_writable_write_chunk_encoding_callback_1
[`_write()`]: #stream_writable_write_chunk_encoding_callback_1
[_write]: #stream_writable_write_chunk_encoding_callback_1
[`util.inherits`]: util.html#util_util_inherits_constructor_superconstructor
[`end()`]: #stream_writable_end_chunk_encoding_callback
-->

[EventEmitter]: events.html#events_class_events_eventemitter
[オブジェクトモード]: #stream_object_mode
[`stream.push(chunk)`]: #stream_readable_push_chunk_encoding
[`stream.push(null)`]: #stream_readable_push_chunk_encoding
[`stream.push()`]: #stream_readable_push_chunk_encoding
[`unpipe()`]: #stream_readable_unpipe_destination
[切り離された]: #stream_readable_unpipe_destination
[tcp のソケット]: net.html#net_class_net_socket
[zlib のストリーム]: zlib.html
[zlib]: zlib.html
[crypto のストリーム]: crypto.html
[crypto]: crypto.html
[tls.CryptoStream]: tls.html#tls_class_cryptostream
[process.stdin]: process.html#process_process_stdin
[標準出力]: process.html#process_process_stdout
[process.stdout]: process.html#process_process_stdout
[process.stderr]: process.html#process_process_stderr
[child_process の標準出力と標準エラー出力]: child_process.html#child_process_child_stdout
[ストリーム利用者のための API]: #stream_api_for_stream_consumers
[ストリーム実装者向けの API]: #stream_api_for_stream_implementors
[互換性]: #stream_compatibility_with_older_node_versions
[Readable]: #stream_class_stream_readable
[Writable]: #stream_class_stream_writable
[Duplex]: #stream_class_stream_duplex
[Transform]: #stream_class_stream_transform
[`_read(size)`]: #stream_readable_read_size_1
[`_read()`]: #stream_readable_read_size_1
[_read]: #stream_readable_read_size_1
[`write(chunk, encoding, callback)`]: #stream_writable_write_chunk_encoding_callback
[`write()`]: #stream_writable_write_chunk_encoding_callback
[`stream.write(chunk)`]: #stream_writable_write_chunk_encoding_callback
[`_write(chunk, encoding, callback)`]: #stream_writable_write_chunk_encoding_callback_1
[`_write()`]: #stream_writable_write_chunk_encoding_callback_1
[_write]: #stream_writable_write_chunk_encoding_callback_1
[`util.inherits`]: util.html#util_util_inherits_constructor_superconstructor
[`end()`]: #stream_writable_end_chunk_encoding_callback
[_transform]: #stream_transform_transform_chunk_encoding_callback
[_flush]: #stream_transform_flush_callback
