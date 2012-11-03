# Stream

    Stability: 2 - Unstable

<!--
A stream is an abstract interface implemented by various objects in
Node.  For example a request to an HTTP server is a stream, as is
stdout. Streams are readable, writable, or both. All streams are
instances of [EventEmitter][]

You can load up the Stream base class by doing `require('stream')`.
-->

ストリームは Node の様々なオブジェクトで実装される抽象的なインタフェースです。
例えば HTTP サーバへのリクエストは標準出力と同様にストリームです。
ストリームは読み込み可能、書き込み可能、またはその両方です。
全てのストリームは [EventEmitter][] のインスタンスです。

Stream のベースクラスは `require('stream')` でロードすることができます。

## Readable Stream

<!--type=class-->

<!--
A `Readable Stream` has the following methods, members, and events.
-->

`Readable Stream` には以下のメソッド、メンバー、そしてイベントがあります。

### Event: 'data'

`function (data) { }`

<!--
The `'data'` event emits either a `Buffer` (by default) or a string if
`setEncoding()` was used.

Note that the __data will be lost__ if there is no listener when a
`Readable Stream` emits a `'data'` event.
-->

`'data'` イベントは `Buffer` (デフォルト) または、
`setEncoding()` された場合は文字列のどちらかを生成します

`Readable Stream` が `'data'` イベントを生成した時にリスナが存在しなければ、
__データは失われる__ことに注意してください。

### Event: 'end'

`function () { }`

<!--
Emitted when the stream has received an EOF (FIN in TCP terminology).
Indicates that no more `'data'` events will happen. If the stream is
also writable, it may be possible to continue writing.
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
Emitted when the underlying resource (for example, the backing file
descriptor) has been closed. Not all streams will emit this.
-->

下層のリソース (例えば背後のファイル記述子) がクローズされた時に生成されます。
全てのストリームがこのイベントを発生するわけではありません。

### stream.readable

<!--
A boolean that is `true` by default, but turns `false` after an
`'error'` occurred, the stream came to an `'end'`, or `destroy()` was
called.
-->

デフォルトでは `true` ですが、`'error'` が発生した後、
ストリームが `'end'` に達した後、または `destroy()` が呼ばれた後で、
`false` に設定される boolean です。

### stream.setEncoding([encoding])

<!--
Makes the `'data'` event emit a string instead of a `Buffer`. `encoding`
can be `'utf8'`, `'utf16le'` (`'ucs2'`), `'ascii'`, or `'hex'`. Defaults
to `'utf8'`.
-->

`'data'` イベントが `Buffer` ではなく文字列を生成するようにします。
`encoding` には `'utf8'`、`'utf16le'` (`'ucs2'`)、`'ascii'`、
または `'hex'` を指定することができます。
デフォルトは `'utf8'` です。

### stream.pause()

<!--
Issues an advisory signal to the underlying communication layer,
requesting that no further data be sent until `resume()` is called.
-->

アドバイス的なシグナルを下層の通信レイヤに発し、`resume()` が呼ばれるまで
データが送られないように要求します。

<!--
Note that, due to the advisory nature, certain streams will not be
paused immediately, and so `'data'` events may be emitted for some
indeterminate period of time even after `pause()` is called. You may
wish to buffer such `'data'` events.
-->

アドバイス的な性質のために、ストリームによっては即座に中断しないこと、
および `pause()` を呼び出してから不特定の時間 `'data'` イベントが
生成されるかもしれないことに注意してください。
`'data'` イベントをバッファリングするのもいいでしょう。

### stream.resume()

<!--
Resumes the incoming `'data'` events after a `pause()`.
-->

`pause()` の後で `'data'` イベントの到着を再開します。

### stream.destroy()

<!--
Closes the underlying file descriptor. Stream is no longer `writable`
nor `readable`.  The stream will not emit any more 'data', or 'end'
events. Any queued write data will not be sent.  The stream should emit
'close' event once its resources have been disposed of.
-->

下層のファイル記述子をクローズします。
ストリームはもう `writable` でも `readable` でもなくなります。
ストリームはそれ以上 `'data'` あるいは `'end'` イベントを生成しなくなります。
キューイングされたデータは送信されません。
ストリームはそのリソースが破棄されたときに `'close'` イベントを生成すべきです。

### stream.pipe(destination, [options])

<!--
This is a `Stream.prototype` method available on all `Stream`s.
-->

これは全ての `Stream` で利用可能な `Stream.prototype` メソッドです。

<!--
Connects this read stream to `destination` WriteStream. Incoming data on
this stream gets written to `destination`. The destination and source
streams are kept in sync by pausing and resuming as necessary.
-->

読み込みストリームを `destination` の書き込みストリームに接続します。
このストリームに入ってきたデータは `destination` に書き込まれます。
接続先と接続元のストリームは、必要に応じて中断と再開することで同期を保ちます。

<!--
This function returns the `destination` stream.
-->

この関数は `destination` ストリームを返します。

<!--
Emulating the Unix `cat` command:
-->

Unix の `cat` コマンドのエミュレート:

    process.stdin.resume(); process.stdin.pipe(process.stdout);


<!--
By default `end()` is called on the destination when the source stream
emits `end`, so that `destination` is no longer writable. Pass `{ end:
false }` as `options` to keep the destination stream open.
-->

デフォルトでは接続元ストリームで `end` イベントが生成されると、
接続先の `end()` が呼ばれるので、もう書き込みはできません。
`option` に `{ end: false }` を渡すと接続先はストリームはオープンされたままとなります。

<!--
This keeps `process.stdout` open so that "Goodbye" can be written at the
end.
-->

これは `process.stdout` をオープンしたままにして最後に "Goodbye" と出力します。

    process.stdin.resume();

    process.stdin.pipe(process.stdout, { end: false });

    process.stdin.on("end", function() {
    process.stdout.write("Goodbye\n"); });


## Writable Stream

<!--type=class-->

<!--
A `Writable Stream` has the following methods, members, and events.
-->

`Writable Stream` には以下のメソッド、メンバー、そしてイベントがあります。

### Event: 'drain'

`function () { }`

<!--
Emitted when the stream's write queue empties and it's safe to write without
buffering again. Listen for it when `stream.write()` returns `false`.
-->

ストリームの書き込みキューが空になり、バッファリングされることなく
再び安全に書き込みができるようになった場合に生成されます。
`stream.write()` が `false` を返した場合に監視してください。

<!--
The `'drain'` event can happen at *any* time, regardless of whether or not
`stream.write()` has previously returned `false`. To avoid receiving unwanted
`'drain'` events, listen using `stream.once()`.
-->

`stream.write()` が以前に `false` を返したかどうかにかかわらず、
`'drain'` イベントは *いつでも* 発生します。
意図しない `'drain'` イベントを受信しないようにするには、`stream.once()`
を使って監視してください。

### Event: 'error'

`function (exception) { }`

<!--
Emitted on error with the exception `exception`.
-->

`exception` 例外によるエラーについて生成されます。

### Event: 'close'

`function () { }`

<!--
Emitted when the underlying file descriptor has been closed.
-->

下層でファイル記述子がクローズされた時に生成されます。

### Event: 'pipe'

`function (src) { }`

<!--
Emitted when the stream is passed to a readable stream's pipe method.
-->

このストリームが読み込み可能ストリームの pipe メソッドに渡された時に生成されます。

### stream.writable

<!--
A boolean that is `true` by default, but turns `false` after an
`'error'` occurred or `end()` / `destroy()` was called.
-->

デフォルトでは `true` ですが、`'error'` が発生した後、
`end()` / `destroy()` が呼ばれた後で `false` に設定される boolean です。

### stream.write(string, [encoding])

<!--
Writes `string` with the given `encoding` to the stream.  Returns `true`
if the string has been flushed to the kernel buffer.  Returns `false` to
indicate that the kernel buffer is full, and the data will be sent out
in the future. The `'drain'` event will indicate when the kernel buffer
is empty again. The `encoding` defaults to `'utf8'`.
-->

与えられた `encoding` で `string` を書き込みます。
文字列がカーネルバッファにフラッシュされた場合は `true` が返ります。
カーネルバッファがいっぱいの場合は、データが将来カーネルバッファに送られることを示すために、
`false` が返ります。
`'drain'` イベントがカーネルバッファが再び空いたことを示します。
`encoding` のデフォルトは `'utf8'` です。

### stream.write(buffer)

<!--
Same as the above except with a raw buffer.
-->

生のバッファを使うこと以外は上記と同じです。

### stream.end()

<!--
Terminates the stream with EOF or FIN.  This call will allow queued
write data to be sent before closing the stream.
-->

ストリームを EOF または FIN で終了します。
この呼び出しは、ストリームがクローズされる前にキューイングされたデータが
送信されることを許します。

### stream.end(string, encoding)

<!--
Sends `string` with the given `encoding` and terminates the stream with
EOF or FIN. This is useful to reduce the number of packets sent.
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
Closes the underlying file descriptor. Stream is no longer `writable`
nor `readable`.  The stream will not emit any more 'data', or 'end'
events. Any queued write data will not be sent.  The stream should emit
'close' event once its resources have been disposed of.
-->

下層のファイル記述子をクローズします。
ストリームはもう `writable` でも `readable` でもなくなります。
ストリームはそれ以上 `'data'` あるいは `'end'` イベントを生成しなくなります。
キューイングされたデータは送信されません。
ストリームはそのリソースが破棄されたときに `'close'` イベントを生成すべきです。

### stream.destroySoon()

<!--
After the write queue is drained, close the file descriptor.
`destroySoon()` can still destroy straight away, as long as there is no
data left in the queue for writes.
-->

出力キューが空になった後、ファイル記述子をクローズします。
出力キューにデータが存在しない場合、`destroySoon()` はすぐに破棄します。

[EventEmitter]: events.html#events_class_events_eventemitter
