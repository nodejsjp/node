# Domain

    Stability: 1 - Experimental

<!--
Domains provide a way to handle multiple different IO operations as a
single group.  If any of the event emitters or callbacks registered to a
domain emit an `error` event, or throw an error, then the domain object
will be notified, rather than losing the context of the error in the
`process.on('uncaughtException')` handler, or causing the program to
exit with an error code.
-->

ドメインは複数の異なる I/O 操作を一つのグループとして扱う方法を
提供します。
もし EventEmitter またはコールバックがドメインに登録されると、
`'error'` がイベントが発生したり例外がスローされた場合、
`process.on('uncaughtException')` ハンドラでエラーのコンテキストが失われたり、
プログラムがエラーコードと共に終了する代わりに、
ドメインオブジェクトが通知します

<!--
This feature is new in Node version 0.8.  It is a first pass, and is
expected to change significantly in future versions.  Please use it and
provide feedback.
-->

この機能は Node バージョン 0.8 からの新しいものです。
これはファーストパスであり、将来のバージョンで大きく変更されると予想されます。
是非これを使ってフィードバックをしてください。

<!--
Due to their experimental nature, the Domains features are disabled unless
the `domain` module is loaded at least once.  No domains are created or
registered by default.  This is by design, to prevent adverse effects on
current programs.  It is expected to be enabled by default in future
Node.js versions.
-->

これらは実験的であるため、ドメインの機能は `domain` モジュールが少なくとも
一回はロードされるまで無効になっています。
デフォルトではドメインは作成されず、デフォルトで登録もされません。
それは既存のプログラムに悪影響を与えることを防ぐために設計されています。
将来の Node.js バージョンではデフォルトで有効になることが期待されます。

## Additions to Error objects

<!-- type=misc -->

<!--
Any time an Error object is routed through a domain, a few extra fields
are added to it.
-->

どんな場合でも、ドメインに送られた Error オブジェクトは
いくつかのフィールドが加えられます。

<!--
* `error.domain` The domain that first handled the error.
* `error.domain_emitter` The event emitter that emitted an 'error' event
  with the error object.
* `error.domain_bound` The callback function which was bound to the
  domain, and passed an error as its first argument.
* `error.domain_thrown` A boolean indicating whether the error was
  thrown, emitted, or passed to a bound callback function.
-->

* `error.domain` このエラーを最初に捕まえたドメイン。
* `error.domain_emitter` このエラーオブジェクトと共に `error` イベントを
  生成した EventEmitter。
* `error.domain_bound` ドメインに束縛されたコールバック関数で、
  その第 1 引数にエラーが渡されたもの。
* `error.domain_throw` エラーがスローされたか、EventEmitter から生成されたか、
  それとも束縛されたコールバック関数に渡されたかを示す boolean 値。

## Implicit Binding

<!--type=misc-->

<!--
If domains are in use, then all new EventEmitter objects (including
Stream objects, requests, responses, etc.) will be implicitly bound to
the active domain at the time of their creation.
-->

一度ドメインが作成されると、全ての新しい EventEmitter オブジェクト
(ストリームオブジェクトやリクエスト、レスポンス、その他を含む) は、
それが作成された時点でアクティブなドメインに暗黙的に束縛されます。

<!--
Additionally, callbacks passed to lowlevel event loop requests (such as
to fs.open, or other callback-taking methods) will automatically be
bound to the active domain.  If they throw, then the domain will catch
the error.
-->

加えて、コールバックが低水準のイベントループ要求 (例えば `fs.open()`、
あるいは他のコールバックを受け取るメソッド) もアクティブなドメインに
束縛されます。
もしそれらが例外をスローすると、ドメインはそれを捕まえます。

<!--
In order to prevent excessive memory usage, Domain objects themselves
are not implicitly added as children of the active domain.  If they
were, then it would be too easy to prevent request and response objects
from being properly garbage collected.
-->

必要以上にメモリを消費するのを防ぐため、ドメインオブジェクトそれ自身は
暗黙的にアクティブドメインの子として暗黙的には追加されません。
それをすれば、リクエストやレスポンスオブジェクトがきちんと GC されることを
あまりにも簡単に妨害してしまうでしょう。

<!--
If you *want* to nest Domain objects as children of a parent Domain,
then you must explicitly add them, and then dispose of them later.
-->

もしネストしたドメインを子として他のドメインに *加えたければ*
明示的にそれを加えなければならず、後で破棄しなければなりません。

<!--
Implicit binding routes thrown errors and `'error'` events to the
Domain's `error` event, but does not register the EventEmitter on the
Domain, so `domain.dispose()` will not shut down the EventEmitter.
Implicit binding only takes care of thrown errors and `'error'` events.
-->

暗黙的なドメインはスローされたエラーや `'error'` イベントを、
ドメインの `'error'` イベントにルーティングしますが、
その EventEmitter をドメインに登録しないので、`domain.dispose()` は
EventEmitter をシャットダウンしません。
暗黙的なバインディングはスローされた例外と `'error'` イベントにだけ
注意を払います。

## domain.create()

* return: {Domain}

<!--
Returns a new Domain object.
-->

新しいドメインオブジェクトを返します。

## Class: Domain

<!--
The Domain class encapsulates the functionality of routing errors and
uncaught exceptions to the active Domain object.
-->

ドメインクラスはエラーのルーティングや捕まえられなかった例外をアクティブな
ドメインにルーティングする機能をカプセル化します。

<!--
Domain is a child class of EventEmitter.  To handle the errors that it
catches, listen to its `error` event.
-->

ドメインは EventEmitter の子クラスです。これが捕まえたエラーを扱いたければ、
`'error'` イベントを監視してください。

### domain.members

* {Array}

<!--
An array of timers and event emitters that have been explicitly added
to the domain.
-->

このドメインに明示的に加えられたタイマまたは EventEmitter の配列です。

### domain.add(emitter)

<!--
* `emitter` {EventEmitter | Timer} emitter or timer to be added to the domain
-->

* `emitter` {EventEmitter | Timer} ドメインに加えられる EventEmitter
  またはタイマ

<!--
Explicitly adds an emitter to the domain.  If any event handlers called by
the emitter throw an error, or if the emitter emits an `error` event, it
will be routed to the domain's `error` event, just like with implicit
binding.
-->

明示的に EventEmitter をドメインに追加します。
この EventEmitter から呼ばれたどのイベントハンドラがエラーをスローしても、
あるいはこの EventEmitter が `'error'` イベントを発生しても、
暗黙的にバインディングされたのと同様、それはこのドメインの `'error`'
イベントにルーティングされます。

<!--
This also works with timers that are returned from `setInterval` and
`setTimeout`.  If their callback function throws, it will be caught by
the domain 'error' handler.
-->

これは同様に `setIntervalu` および `setTimeout` から返されるタイマでも
働きます。それらのコールバック関数がエラーをスローすると、それは
ドメインの `'error'` ハンドに捕まえられます。

<!--
If the Timer or EventEmitter was already bound to a domain, it is removed
from that one, and bound to this one instead.
-->

もしタイマまたは EventEmitter が既に他のドメインに束縛されていた場合、
それは元のドメインから削除され、代わりにこのドメインに束縛されます。

### domain.remove(emitter)

<!--
* `emitter` {EventEmitter | Timer} emitter or timer to be removed from the domain
-->

* `emitter` {EventEmitter | Timer} このドメインから削除される EventEmitter
  またはタイマ

<!--
The opposite of `domain.add(emitter)`.  Removes domain handling from the
specified emitter.
-->

これは `domain.add(emitter)` と対照的です。指定された EventEmitter を
ドメインから削除します。

### domain.bind(cb)

<!--
* `cb` {Function} The callback function
* return: {Function} The bound function
-->

* `cb` {Function} コールバック関数
* return: {Function} 束縛された関数

<!--
The returned function will be a wrapper around the supplied callback
function.  When the returned function is called, any errors that are
thrown will be routed to the domain's `error` event.
-->

返される関数は与えられたコールバック関数のラッパーです。
返された関数が呼び出されると、スローされたエラーはドメインの `'error'`
イベントにルーティングされます。

#### Example

    var d = domain.create();

    function readSomeFile(filename, cb) {
      fs.readFile(filename, d.bind(function(er, data) {
        // if this throws, it will also be passed to the domain
        return cb(er, JSON.parse(data));
      }));
    }

    d.on('error', function(er) {
      // an error occurred somewhere.
      // if we throw it now, it will crash the program
      // with the normal line number and stack message.
    });

### domain.intercept(cb)

<!--
* `cb` {Function} The callback function
* return: {Function} The intercepted function
-->

* `cb` {Function} コールバック関数
* return: {Function} インターセプトされた関数

<!--
This method is almost identical to `domain.bind(cb)`.  However, in
addition to catching thrown errors, it will also intercept `Error`
objects sent as the first argument to the function.
-->

このメソッドはほとんど `domain.bind(cb)` と同じです。
ただし、スローされたエラーを捕まえることに加えて、関数に渡される最初の引数が
`Error` オブジェクトの場合もインターセプトします。

<!--
In this way, the common `if (er) return cb(er);` pattern can be replaced
with a single error handler in a single place.
-->

これは、一般的な `if (er) return cb(er);` パターンを一カ所で単一の
エラーハンドラに置き換えることができます。

#### Example

    var d = domain.create();

    function readSomeFile(filename, cb) {
      fs.readFile(filename, d.intercept(function(er, data) {
        // if this throws, it will also be passed to the domain
        // additionally, we know that 'er' will always be null,
        // so the error-handling logic can be moved to the 'error'
        // event on the domain instead of being repeated throughout
        // the program.
        return cb(er, JSON.parse(data));
      }));
    }

    d.on('error', function(er) {
      // an error occurred somewhere.
      // if we throw it now, it will crash the program
      // with the normal line number and stack message.
    });

### domain.dispose()

<!--
The dispose method destroys a domain, and makes a best effort attempt to
clean up any and all IO that is associated with the domain.  Streams are
aborted, ended, closed, and/or destroyed.  Timers are cleared.
Explicitly bound callbacks are no longer called.  Any error events that
are raised as a result of this are ignored.
-->

`dispose()` メソッドはドメインを破棄し、ドメインに関連づけられた全ての
I/O を可能な限りクリーンナップしようと試みます。
ストリームはアボートされ、終了され、クローズされ、そして／または破棄されます。
タイマはクリアされます。明示的に束縛されたコールバックはもう呼び出されません。
結果として発生するどんなエラーイベントも無視されます。

<!--
The intention of calling `dispose` is generally to prevent cascading
errors when a critical part of the Domain context is found to be in an
error state.
-->

`dispose()` を呼び出す意図は通常、エラー状態の中でドメインコンテキストの
クリティカルな部分が見つかった場合に、カスケードしたエラーを防ぐことです。

<!--
Note that IO might still be performed.  However, to the highest degree
possible, once a domain is disposed, further errors from the emitters in
that set will be ignored.  So, even if some remaining actions are still
in flight, Node.js will not communicate further about them.
-->

I/O がまだ行われているかもしれないことに注意してください。
しかしながら、可能な限り高い視点から見ると、一度ドメインが破棄されれば、
その集合の中の EventEmitter からのさらなるエラーは無視されます。
そのため、たとえ残りのアクションがまだ実行中だとしても、
Node.js はそれらとコミュニケートしません。
