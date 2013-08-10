# Domain

    Stability: 2 - Unstable

<!--
Domains provide a way to handle multiple different IO operations as a
single group.  If any of the event emitters or callbacks registered to a
domain emit an `error` event, or throw an error, then the domain object
will be notified, rather than losing the context of the error in the
`process.on('uncaughtException')` handler, or causing the program to
exit immediately with an error code.
-->

ドメインは複数の異なる I/O 操作を一つのグループとして扱う方法を
提供します。
もし EventEmitter またはコールバックがドメインに登録されると、
`'error'` がイベントが発生したり例外がスローされた場合、
`process.on('uncaughtException')` ハンドラでエラーのコンテキストが失われたり、
プログラムがエラーコードと共に即座に終了する代わりに、
ドメインオブジェクトに通知されます

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

## Warning: Don't Ignore Errors!

<!-- type=misc -->

<!--
Domain error handlers are not a substitute for closing down your
process when an error occurs.
-->

ドメインのエラーハンドラは、エラーが発生した時に
プロセスを終了する代わりにはなりません。

<!--
By the very nature of how `throw` works in JavaScript, there is almost
never any way to safely "pick up where you left off", without leaking
references, or creating some other sort of undefined brittle state.
-->

JavaScript において「throw」がどのように働くかという性質により、
参照のリークや未定義の脆弱な状態を作ることなく「中断したところを取得する」
方法はほとんどありません。

<!--
The safest way to respond to a thrown error is to shut down the
process.  Of course, in a normal web server, you might have many
connections open, and it is not reasonable to abruptly shut those down
because an error was triggered by someone else.
-->

スローされたエラーに対処するもっとも安全な方法はプロセスを終了することです。
もちろん、通常の Web サーバは多くの接続をオープンしていており、
他の誰かによって引き起こされたエラーのためにそれらをシャットダウンすることは
合理的ではありません。

<!--
The better approach is send an error response to the request that
triggered the error, while letting the others finish in their normal
time, and stop listening for new requests in that worker.
-->

よりよいアプローチは、エラーを引き起こしたリクエストにエラーを応答し、
それ以外の接続が正常に終了するまでの間、ワーカは新しいリクエストのリスニングを
止めることです。

<!--
In this way, `domain` usage goes hand-in-hand with the cluster module,
since the master process can fork a new worker when a worker
encounters an error.  For node programs that scale to multiple
machines, the terminating proxy or service registry can take note of
the failure, and react accordingly.
-->

この方法では、`domain` はクラスタモジュールと手を取り合う利用方法により、
ワーカプロセスがエラーに遭遇した場合に新しいワーカをフォークできます。
複数のマシンにスケールする node プログラムでは、
終端のプロキシやサービスレジストリは障害に注意し、
それに応じて対処することができます。

<!--
For example, this is not a good idea:
-->

たとえば、これはいいアイディアではありません:

```javascript
// XXX WARNING!  BAD IDEA!

var d = require('domain').create();
d.on('error', function(er) {
  // The error won't crash the process, but what it does is worse!
  // Though we've prevented abrupt process restarting, we are leaking
  // resources like crazy if this ever happens.
  // This is no better than process.on('uncaughtException')!
  console.log('error, but oh well', er.message);
});
d.run(function() {
  require('http').createServer(function(req, res) {
    handleRequest(req, res);
  }).listen(PORT);
});
```

<!--
By using the context of a domain, and the resilience of separating our
program into multiple worker processes, we can react more
appropriately, and handle errors with much greater safety.
-->

domain の利用と、プログラムを複数のワーカプロセスに分割することによる
復元力により、とても安全にエラーを扱う、より適切な対処をすることができます。

```javascript
// Much better!

var cluster = require('cluster');
var PORT = +process.env.PORT || 1337;

if (cluster.isMaster) {
  // In real life, you'd probably use more than just 2 workers,
  // and perhaps not put the master and worker in the same file.
  //
  // You can also of course get a bit fancier about logging, and
  // implement whatever custom logic you need to prevent DoS
  // attacks and other bad behavior.
  //
  // See the options in the cluster documentation.
  //
  // The important thing is that the master does very little,
  // increasing our resilience to unexpected errors.

  cluster.fork();
  cluster.fork();

  cluster.on('disconnect', function(worker) {
    console.error('disconnect!');
    cluster.fork();
  });

} else {
  // the worker
  //
  // This is where we put our bugs!

  var domain = require('domain');

  // See the cluster documentation for more details about using
  // worker processes to serve requests.  How it works, caveats, etc.

  var server = require('http').createServer(function(req, res) {
    var d = domain.create();
    d.on('error', function(er) {
      console.error('error', er.stack);

      // Note: we're in dangerous territory!
      // By definition, something unexpected occurred,
      // which we probably didn't want.
      // Anything can happen now!  Be very careful!

      try {
        // make sure we close down within 30 seconds
        var killtimer = setTimeout(function() {
          process.exit(1);
        }, 30000);
        // But don't keep the process open just for that!
        killtimer.unref();

        // stop taking new requests.
        server.close();

        // Let the master know we're dead.  This will trigger a
        // 'disconnect' in the cluster master, and then it will fork
        // a new worker.
        cluster.worker.disconnect();

        // try to send an error to the request that triggered the problem
        res.statusCode = 500;
        res.setHeader('content-type', 'text/plain');
        res.end('Oops, there was a problem!\n');
      } catch (er2) {
        // oh well, not much we can do at this point.
        console.error('Error sending 500!', er2.stack);
      }
    });

    // Because req and res were created before this domain existed,
    // we need to explicitly add them.
    // See the explanation of implicit vs explicit binding below.
    d.add(req);
    d.add(res);

    // Now run the handler function in the domain.
    d.run(function() {
      handleRequest(req, res);
    });
  });
  server.listen(PORT);
}

// This part isn't important.  Just an example routing thing.
// You'd put your fancy application logic here.
function handleRequest(req, res) {
  switch(req.url) {
    case '/error':
      // We do some async stuff, and then...
      setTimeout(function() {
        // Whoops!
        flerb.bark();
      });
      break;
    default:
      res.end('ok');
  }
}
```

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
* `error.domainEmitter` The event emitter that emitted an 'error' event
  with the error object.
* `error.domainBound` The callback function which was bound to the
  domain, and passed an error as its first argument.
* `error.domainThrown` A boolean indicating whether the error was
  thrown, emitted, or passed to a bound callback function.
-->

* `error.domain` このエラーを最初に捕まえたドメイン。
* `error.domainEmitter` このエラーオブジェクトと共に `error` イベントを
  生成した EventEmitter。
* `error.domainBound` ドメインに束縛されたコールバック関数で、
  その第 1 引数にエラーが渡されたもの。
* `error.domainThrown` エラーがスローされたか、EventEmitter から生成されたか、
  それとも束縛されたコールバック関数に渡されたかを示す boolean 値。

## Implicit Binding

<!--type=misc-->

<!--
If domains are in use, then all **new** EventEmitter objects (including
Stream objects, requests, responses, etc.) will be implicitly bound to
the active domain at the time of their creation.
-->

一度ドメインが作成されると、全ての **新しい** EventEmitter オブジェクト
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
then you must explicitly add them.
-->

もしネストしたドメインを子として他のドメインに *加えたければ*
明示的にそれを加えなければなりません。

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

## Explicit Binding

<!--type=misc-->

<!--
Sometimes, the domain in use is not the one that ought to be used for a
specific event emitter.  Or, the event emitter could have been created
in the context of one domain, but ought to instead be bound to some
other domain.
-->

時には、使用中のドメインは特定の EventEmitter に使用されるべきではありません。
あるいは、EventEmitter はあるドメインのコンテキスト中で作成されますが、
その他のドメインに結びつけられるべきかもしれません。

<!--
For example, there could be one domain in use for an HTTP server, but
perhaps we would like to have a separate domain to use for each request.
-->

例えば、HTTP サーバで使われるドメインが一つあるとしても、
おそらくリクエスト毎に独立したドメインを持ちたいでしょう。

<!--
That is possible via explicit binding.
-->

これは明示的なバインディングによって可能となります。

<!--
For example:
-->

例:

```
// create a top-level domain for the server
var serverDomain = domain.create();

serverDomain.run(function() {
  // server is created in the scope of serverDomain
  http.createServer(function(req, res) {
    // req and res are also created in the scope of serverDomain
    // however, we'd prefer to have a separate domain for each request.
    // create it first thing, and add req and res to it.
    var reqd = domain.create();
    reqd.add(req);
    reqd.add(res);
    reqd.on('error', function(er) {
      console.error('Error', er, req.url);
      try {
        res.writeHead(500);
        res.end('Error occurred, sorry.');
      } catch (er) {
        console.error('Error sending 500', er, req.url);
      }
    });
  }).listen(1337);
});
```

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
Domain is a child class of [EventEmitter][].  To handle the errors that it
catches, listen to its `error` event.
-->

ドメインは [EventEmitter][] の子クラスです。
これが捕まえたエラーを扱いたければ、`'error'` イベントを監視してください。

### domain.run(fn)

* `fn` {Function}

<!--
Run the supplied function in the context of the domain, implicitly
binding all event emitters, timers, and lowlevel requests that are
created in that context.
-->

与えられた関数をこのドメインのコンテキストで実行します。
このコンテキストで作成される全ての EventEmitter、タイマ、そして低水準の要求は
暗黙的にバインドされます。

<!--
This is the most basic way to use a domain.
-->

これはドメインを使用するもっとも一般的な方法です。

<!--
Example:
-->

例:

```
var d = domain.create();
d.on('error', function(er) {
  console.error('Caught error!', er);
});
d.run(function() {
  process.nextTick(function() {
    setTimeout(function() { // simulating some various async stuff
      fs.open('non-existent file', 'r', function(er, fd) {
        if (er) throw er;
        // proceed...
      });
    }, 100);
  });
});
```

<!--
In this example, the `d.on('error')` handler will be triggered, rather
than crashing the program.
-->

この例では、プログラムはクラッシュせずに `d.on('error')` ハンドラが
呼び出されます。

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

### domain.bind(callback)

<!--
* `callback` {Function} The callback function
* return: {Function} The bound function
-->

* `callback` {Function} コールバック関数
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
      fs.readFile(filename, 'utf8', d.bind(function(er, data) {
        // if this throws, it will also be passed to the domain
        return cb(er, data ? JSON.parse(data) : null);
      }));
    }

    d.on('error', function(er) {
      // an error occurred somewhere.
      // if we throw it now, it will crash the program
      // with the normal line number and stack message.
    });

### domain.intercept(callback)

<!--
* `callback` {Function} The callback function
* return: {Function} The intercepted function
-->

* `callback` {Function} コールバック関数
* return: {Function} インターセプトされた関数

<!--
This method is almost identical to `domain.bind(callback)`.  However, in
addition to catching thrown errors, it will also intercept `Error`
objects sent as the first argument to the function.
-->

このメソッドはほとんど `domain.bind(callback)` と同じです。
ただし、スローされたエラーを捕まえることに加えて、関数に渡される最初の引数が
`Error` オブジェクトの場合もインターセプトします。

<!--
In this way, the common `if (er) return callback(er);` pattern can be replaced
with a single error handler in a single place.
-->

これは、一般的な `if (er) return callback(er);` パターンを一カ所で単一の
エラーハンドラに置き換えることができます。

#### Example

    var d = domain.create();

    function readSomeFile(filename, cb) {
      fs.readFile(filename, 'utf8', d.intercept(function(data) {
        // note, the first argument is never passed to the
        // callback since it is assumed to be the 'Error' argument
        // and thus intercepted by the domain.

        // if this throws, it will also be passed to the domain
        // so the error-handling logic can be moved to the 'error'
        // event on the domain instead of being repeated throughout
        // the program.
        return cb(null, JSON.parse(data));
      }));
    }

    d.on('error', function(er) {
      // an error occurred somewhere.
      // if we throw it now, it will crash the program
      // with the normal line number and stack message.
    });

### domain.enter()

<!--
The `enter` method is plumbing used by the `run`, `bind`, and `intercept`
methods to set the active domain. It sets `domain.active` and `process.domain`
to the domain, and implicitly pushes the domain onto the domain stack managed
by the domain module (see `domain.exit()` for details on the domain stack). The
call to `enter` delimits the beginning of a chain of asynchronous calls and I/O
operations bound to a domain.
-->

`enter()` メソッドは、`run()`、`bind()`、そして `intercept()` メソッドを
アクティブなドメインに結びつけるために使われます。
これは (このドメインオブジェクト、すなわち `this` を) `domain.active` および
`process.domain` を設定し、ドメインモジュールによって管理される
ドメインのスタックに暗黙的に積み上げます (ドメインのスタックに関する詳細は
`domain.exit()` を参照)。

<!--
Calling `enter` changes only the active domain, and does not alter the domain
itself. `Enter` and `exit` can be called an arbitrary number of times on a
single domain.
-->

`enter()` の呼び出しはアクティブなドメインを変更することだけで、
ドメイン自身は変化しません。
`enter()` と `exit()` は一つのドメインに対して何度でも呼び出すことができます。

<!--
If the domain on which `enter` is called has been disposed, `enter` will return
without setting the domain.
-->

もし `enter()` が呼び出されたドメインが破棄済みだと、
`enter()` は何も設定せずにリターンします。

### domain.exit()

<!--
The `exit` method exits the current domain, popping it off the domain stack.
Any time execution is going to switch to the context of a different chain of
asynchronous calls, it's important to ensure that the current domain is exited.
The call to `exit` delimits either the end of or an interruption to the chain
of asynchronous calls and I/O operations bound to a domain.
-->

`exit()` メソッドは現在のドメインから抜け出し、スタックから取り除きます。
非同期呼び出しのチェーンが異なるコンテキストに切り替わる場合はどんな時でも、
現在のドメインから確実に抜け出すことは重要です。
`exit()` の呼び出しは、ドメインに束縛された非同期呼び出しおよび
I/O 操作のチェーンを、終端または途中で区切ります。

<!--
If there are multiple, nested domains bound to the current execution context,
`exit` will exit any domains nested within this domain.
-->

もし複数のネストしたドメインが現在の実行コンテキストに束縛されていると、
`exit()` はネストしたどのドメインからも抜け出します。

<!--
Calling `exit` changes only the active domain, and does not alter the domain
itself. `Enter` and `exit` can be called an arbitrary number of times on a
single domain.
-->

`exit()` の呼び出しはアクティブなドメインを変更することだけで、
ドメイン自身は変化しません。
`enter()` と `exit()` は一つのドメインに対して何度でも呼び出すことができます。

<!--
If the domain on which `exit` is called has been disposed, `exit` will return
without exiting the domain.
-->

もし `exit()` が呼び出されたドメインが破棄済みだと、
`exit()` は何も設定せずにリターンします。

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
Once the domain is disposed the `dispose` event will emit.
-->

ドメインが破棄されると、`'dispose'` イベントが生成されます。

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

[EventEmitter]: events.html#events_class_events_eventemitter
