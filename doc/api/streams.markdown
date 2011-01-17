## Streams

<!--

A stream is an abstract interface implemented by various objects in Node.
For example a request to an HTTP server is a stream, as is stdout. Streams
are readable, writable, or both. All streams are instances of `EventEmitter`.

-->
ストリームは Node の様々なオブジェクトで実装される抽象的なインタフェースです。
例えば HTTP サーバへのリクエストは標準出力と同様にストリームです。
ストリームは読み込み可能、書き込み可能、またはその両方です。
全てのストリームは `EventEmitter` のインスタンスです。

## Readable Stream

<!--

A `Readable Stream` has the following methods, members, and events.

-->
`Readable Stream` には以下のメソッド、メンバー、そしてイベントがあります。

### Event: 'data'

`function (data) { }`

<!--

The `'data'` event emits either a `Buffer` (by default) or a string if
`setEncoding()` was used.

-->
`'data'` イベントは `Buffer` (デフォルト) または、
`setEncoding()` された場合は文字列のどちらかを生成します

### Event: 'end'

`function () { }`

<!--

Emitted when the stream has received an EOF (FIN in TCP terminology).
Indicates that no more `'data'` events will happen. If the stream is also
writable, it may be possible to continue writing.

-->
ストリームが EOF (TCP 用語では FIN) を受信した時に生成されます。
`'data'` イベントがもう発生しないことを示します。
ストリームがもし書き込み可能でもあるなら、書き込みを続けることは可能かもしれません。

### Event: 'error'

`function (exception) { }`

<!--

Emitted if there was an error receiving data.

-->
データ受信でエラーがあると生成されます。

### Event: 'close'

`function () { }`

<!--

Emitted when the underlying file descriptor has be closed. Not all streams
will emit this.  (For example, an incoming HTTP request will not emit
`'close'`.)

-->
下層でファイル記述子がクローズされた時に生成されます。
全てのストリームがこのイベントを発生するわけではありません。
(例えば、インカミングの HTTP リクエストは `'close'` イベントを生成しません。)

### Event: 'fd'

`function (fd) { }`

<!--

Emitted when a file descriptor is received on the stream. Only UNIX streams
support this functionality; all others will simply never emit this event.

-->
ストリームに関するファイル記述子を受け取った時に生成されます。
UNIX ストリームだけがこの機能をサポートしています;
その他の全てのストリームはこのイベントを生成しません。

### stream.readable

<!--

A boolean that is `true` by default, but turns `false` after an `'error'`
occured, the stream came to an `'end'`, or `destroy()` was called.

-->
デフォルトでは `true` ですが、`'error'` が発生した後、
ストリームが `'end'` に達した後、または `destroy()` が呼ばれた後で、
`false` に設定される boolean です。

### stream.setEncoding(encoding)
<!--

Makes the data event emit a string instead of a `Buffer`. `encoding` can be
`'utf8'`, `'ascii'`, or `'base64'`.

-->
data イベントが `Buffer` ではなく文字列を生成するようにします。
`encoding` には `'utf8'`、`'ascii'`、
または `'base64'` を指定することができます。

### stream.pause()

<!--

Pauses the incoming `'data'` events.

-->
`'data'` イベントの到着を中断します。

### stream.resume()

<!--

Resumes the incoming `'data'` events after a `pause()`.

-->
`pause()` の後で `'data'` イベントの到着を再開します。

### stream.destroy()

<!--

Closes the underlying file descriptor. Stream will not emit any more events.

-->
下層のファイル記述子をクローズします。ストリームはそれ以上イベントを生成しなくなります。

### stream.destroySoon()

<!--

After the write queue is drained, close the file descriptor.

-->
書き込みキューがからになった後、ファイル記述子をクローズします。

### stream.pipe(destination, [options])

<!--

This is a `Stream.prototype` method available on all `Stream`s.

-->
これは全ての `Stream` で利用可能な `Stream.prototype` メソッドです。

<!--

Connects this read stream to `destination` WriteStream. Incoming
data on this stream gets written to `destination`. The destination and source
streams are kept in sync by pausing and resuming as necessary.

-->
読み込みストリームを `destination` の書き込みストリームに接続します。
このストリームに入ってきたデータは `destination` に書き込まれます。
接続先と接続元のストリームは、必要に応じて中断と再開することで同期を保ちます。

<!--

Emulating the Unix `cat` command:

-->
Unix の `cat` コマンドのエミュレート:

    process.stdin.resume();
    process.stdin.pipe(process.stdout);


<!--

By default `end()` is called on the destination when the source stream emits
`end`, so that `destination` is no longer writable. Pass `{ end: false }` as
`options` to keep the destination stream open.

-->
デフォルトでは接続元ストリームで `end` イベントが生成されると、
接続先の `end()` が呼ばれるので、もう書き込みはできません。
`option` に `{ end: false }` を渡すと接続先はストリームはオープンされたままとなります。

<!--

This keeps `process.stdout` open so that "Goodbye" can be written at the end.

-->
これは `process.stdout` をオープンしたままにして最後に "Goodbye" と出力します。

    process.stdin.resume();

    process.stdin.pipe(process.stdout, { end: false });

    process.stdin.on("end", function() {
      process.stdout.write("Goodbye\n");
    });

<!--

NOTE: If the source stream does not support `pause()` and `resume()`, this function
adds simple definitions which simply emit `'pause'` and `'resume'` events on
the source stream.

-->
注意: もし接続元ストリームが `pauses()` と `resume()` をサポートしない場合、
この関数は単に `'pause'` と `'resume'` イベントを接続もとストリームで生成します。

## Writable Stream

<!--

A `Writable Stream` has the following methods, members, and events.

-->
`Writable Stream` には以下のメソッド、メンバー、そしてイベントがあります。

### Event: 'drain'

`function () { }`

<!--

Emitted after a `write()` method was called that returned `false` to
indicate that it is safe to write again.

-->
呼び出された `write()` メソッドが `false` で戻った後に生成され、
再び安全に書き込むことができるようになったことを示します。

### Event: 'error'

`function (exception) { }`

<!--

Emitted on error with the exception `exception`.

-->
`exception`例外によるエラーについて生成されます。

### Event: 'close'

`function () { }`

<!--

Emitted when the underlying file descriptor has been closed.

-->
下層でファイル記述子がクローズされた時に生成されます。

### stream.writable

<!--

A boolean that is `true` by default, but turns `false` after an `'error'`
occurred or `end()` / `destroy()` was called.

-->
デフォルトでは `true` ですが、`'error'` が発生した後、
`end()` / `destroy()` が呼ばれた後で `false` に設定される boolean です。

### stream.write(string, encoding='utf8', [fd])

<!--

Writes `string` with the given `encoding` to the stream.  Returns `true` if
the string has been flushed to the kernel buffer.  Returns `false` to
indicate that the kernel buffer is full, and the data will be sent out in
the future. The `'drain'` event will indicate when the kernel buffer is
empty again. The `encoding` defaults to `'utf8'`.

-->
与えられた `encoding` で `string` を書き込みます。
文字列がカーネルバッファにフラッシュされた場合は `true` が返ります。
カーネルバッファがいっぱいの場合は、データが将来カーネルバッファに送られることを示すために、
`false` が返ります。
`'drain'` イベントがカーネルバッファが再び空いたことを示します。
`encoding` のデフォルトは `'utf8'` です。

<!--

If the optional `fd` parameter is specified, it is interpreted as an integral
file descriptor to be sent over the stream. This is only supported for UNIX
streams, and is silently ignored otherwise. When writing a file descriptor in
this manner, closing the descriptor before the stream drains risks sending an
invalid (closed) FD.

-->
オプションの `fd` 引数が指定されると、
ストリームに送信するための基礎となるファイル記述子として解釈されます。
これは UNIX ストリームでのみサポートされており、その他では黙って無視されます。
このようにファイル記述子に書き込む場合、ストリームが流れきる前にファイル記述子をクローズすると、
データが不正な (クローズされた) ファイル記述子に送られるリスクがあります。

### stream.write(buffer)

<!--

Same as the above except with a raw buffer.

-->
生のバッファを使うこと以外は上記と同じです。

### stream.end()

<!--

Terminates the stream with EOF or FIN.

-->
ストリームを EOF または FIN で終了します。

### stream.end(string, encoding)

<!--

Sends `string` with the given `encoding` and terminates the stream with EOF
or FIN. This is useful to reduce the number of packets sent.

-->
与えられた `encoding` で `string` を送信してからEOFまたはFINでストリームを終了します。
これは送信するパケットの数を減らすために便利です。

### stream.end(buffer)

<!--

Same as above but with a `buffer`.

-->
`buffer` であること以外は上記と同じです。

### stream.destroy()

<!--

Closes the underlying file descriptor. Stream will not emit any more events.

-->
下層のファイル記述子をクローズします。ストリームはそれ以上イベントを生成しなくなります。
