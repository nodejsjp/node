# Events

    Stability: 4 - API Frozen

<!--type=module-->

<!--
Many objects in Node emit events: a `net.Server` emits an event each time
a peer connects to it, a `fs.readStream` emits an event when the file is
opened. All objects which emit events are instances of `events.EventEmitter`.
You can access this module by doing: `require("events");`
-->

Node のオブジェクトの多くはイベントを生成します:
`net.Server`は相手から接続するたびにイベントを生成し、
`fs.readStream`はファイルがオープンされるたびにイベントを生成します。
イベントを生成する全てのオブジェクトは `events.EventEmitter` のインスタンスです。
次のようにすることでこのモジュールにアクセスできます: `require("events");`

<!--
Typically, event names are represented by a camel-cased string, however,
there aren't any strict restrictions on that, as any string will be accepted.
-->

通常、イベント名はキャメル記法による文字列で表現されますが、
厳格な制限ではなく、どんな文字列でも受け入れられます。

<!--
Functions can then be attached to objects, to be executed when an event
is emitted. These functions are called _listeners_.
-->

関数をオブジェクトにアタッチすることができ、それはイベントが生成された時に実行されます。
これらの関数は*リスナー*と呼ばれます。


## Class: events.EventEmitter

<!--
To access the EventEmitter class, `require('events').EventEmitter`.
-->

EventEmitterクラスにアクセスするには、`require('events').EventEmitter` を使います。

<!--
When an `EventEmitter` instance experiences an error, the typical action is
to emit an `'error'` event.  Error events are treated as a special case in node.
If there is no listener for it, then the default action is to print a stack
trace and exit the program.
-->

`EventEmitter` のインスタンスがエラーに遭遇した時、
典型的な動作は `'error'` イベントを生成することです。
node ではエラーイベントは特別に扱われます．
もしそのリスナーがなければ、デフォルトの動作はスタックトレースを出力してプログラムを終了することです。

<!--
All EventEmitters emit the event `'newListener'` when new listeners are
added and `'removeListener'` when a listener is removed.
-->

全ての EventEmitter は、新しいリスナーが加えられるとイベント `'newListener'` を生成し、リスナーが削除されると `'removeListener'` を生成します。

### emitter.addListener(event, listener)
### emitter.on(event, listener)

<!--
Adds a listener to the end of the listeners array for the specified event.
-->

指定されたイベントに対するリスナー配列の最後にリスナーを追加します。

    server.on('connection', function (stream) {
      console.log('someone connected!');
    });

### emitter.once(event, listener)

<!--
Adds a **one time** listener for the event. This listener is
invoked only the next time the event is fired, after which
it is removed.
-->

**一回限り**のリスナーをイベントに追加します。
このリスナーは次にイベントが発生した時に限り起動され、その後で削除されます。

    server.once('connection', function (stream) {
      console.log('Ah, we have our first user!');
    });

### emitter.removeListener(event, listener)

<!--
Remove a listener from the listener array for the specified event.
**Caution**: changes array indices in the listener array behind the listener.
-->

指定されたイベントに対するリスナー配列からリスナーを削除します。
**注意**: リスナーの背後にあるリスナー配列のインデックスが変化します。

    var callback = function(stream) {
      console.log('someone connected!');
    };
    server.on('connection', callback);
    // ...
    server.removeListener('connection', callback);


### emitter.removeAllListeners([event])

<!--
Removes all listeners, or those of the specified event.
-->

全てのリスナーまたは指定されたイベントに対するリスナーを削除します。

### emitter.setMaxListeners(n)

<!--
By default EventEmitters will print a warning if more than 10 listeners are
added for a particular event. This is a useful default which helps finding memory leaks.
Obviously not all Emitters should be limited to 10. This function allows
that to be increased. Set to zero for unlimited.
-->

デフォルトでは、EventEmitter は 10 を越えるリスナが特定のイベントに追加されると警告を出力します。
これはメモリリークを見つけるために役に立つデフォルト値です。
全ての EventEmitter が 10 に制限されなければならないわけではないことは明らかです。
この関数は制限を増やすことを許可します。
0 を設定すると無制限になります。

### emitter.listeners(event)

<!--
Returns an array of listeners for the specified event.
-->

指定されたイベントに対するリスナー配列を返します。

    server.on('connection', function (stream) {
      console.log('someone connected!');
    });
    console.log(util.inspect(server.listeners('connection'))); // [ [Function] ]


### emitter.emit(event, [arg1], [arg2], [...])

<!--
Execute each of the listeners in order with the supplied arguments.
-->

提供された引数の並びでそれぞれのリスナーを実行します。


### Class Method: EventEmitter.listenerCount(emitter, event)

<!--
Return the number of listeners for a given event.
-->

与えられたイベントのリスナー数を返します。


### Event: 'newListener'

* `event` {String} The event name
* `listener` {Function} The event handler function

<!--
This event is emitted any time someone adds a new listener.
-->

このイベントは新しいリスナーが追加されるたびに生成されます。


### Event: 'removeListener'

<!--
* `event` {String} The event name
* `listener` {Function} The event handler function
-->

* `event` {String} イベント名
* `listener` {Function} イベントハンドラ関数


<!--
This event is emitted any time someone removes a listener.
-->

このイベントはリスナが取り除かれるたびに生成されます。
