# Timers

    Stability: 5 - Locked

<!--
All of the timer functions are globals.  You do not need to `require()`
this module in order to use them.
-->

全てのタイマ関数はグローバルです。
このモジュールを使うために `require()` をする必要はありません。

## setTimeout(callback, delay, [arg], [...])

<!--
To schedule execution of a one-time `callback` after `delay` milliseconds. Returns a
`timeoutObject` for possible use with `clearTimeout()`. Optionally you can
also pass arguments to the callback.
-->

`delay` ミリ秒が経過した後で、
`callback` が一度だけ実行されるようにスケジュールします。
`clearTimeout()` で使うことができる `timeoutObject` を返します。
オプションとして、コールバックへの引数を渡すこともできます。

<!--
It is important to note that your callback will probably not be called in exactly
`delay` milliseconds - Node.js makes no guarantees about the exact timing of when
the callback will fire, nor of the ordering things will fire in. The callback will
be called as close as possible to the time specified.
-->

コールバックが正確に `delay` ミリ秒後に呼び出されるとは限らない点に
注目することは重要です - 
Node.js はコールバックが呼び出される正確なタイミングも、
呼び出される順番も保証しません。
コールバックはできるだけ指定された時間に近いタイミングで呼び出されます。

## clearTimeout(timeoutObject)

<!--
Prevents a timeout from triggering.
-->

タイムアウトがトリガーされるのを止めます。

## setInterval(callback, delay, [arg], [...])

<!--
To schedule the repeated execution of `callback` every `delay` milliseconds.
Returns a `intervalObject` for possible use with `clearInterval()`. Optionally
you can also pass arguments to the callback.
-->

`delay` ミリ秒が経過するごとに繰り返し `callback` が実行されるようにスケジュールします。
`clearInterval()` で使うことができる `intervalObject` を返します。
オプションとして、コールバックへの引数を渡すこともできます。

## clearInterval(intervalObject)

<!--
Stops a interval from triggering.
-->

インターバルがトリガーされるのを止めます。

## unref()

<!--
The opaque value returned by `setTimeout` and `setInterval` also has the method
`timer.unref()` which will allow you to create a timer that is active but if
it is the only item left in the event loop won't keep the program running.
If the timer is already `unref`d calling `unref` again will have no effect.
-->

`setTimeout()` あるいは `setInterval()` が返す不透明な値は、
アクティブであるにもかかわらず、それがイベントループの最後の一つになっても
プログラムの実行を継続しないタイマを作ることを可能にする、
`timer.unref()` メソッドを持っています。
既に `unref` されたタイマで再び `unref` が呼び出されても影響はありません。

<!--
In the case of `setTimeout` when you `unref` you create a separate timer that
will wakeup the event loop, creating too many of these may adversely effect
event loop performance -- use wisely.
-->

`setTimeout()` が `unref` された場合、イベントループを起こすために独立した
タイマが作成されるため、それらがあまりに多く作成されるとイベントループの
パフォーマンスに悪影響を与えます -- 賢明に使ってください。

## ref()

<!--
If you had previously `unref()`d a timer you can call `ref()` to explicitly
request the timer hold the program open. If the timer is already `ref`d calling
`ref` again will have no effect.
-->

以前に `unref` されたタイマは、明示的に `ref()` を呼び出すことで
プログラムを実行したままにするよう要求することができます。
既に `ref` されたタイマで再び `ref` が呼び出されても影響はありません。

## setImmediate(callback, [arg], [...])

<!--
To schedule the "immediate" execution of `callback` after I/O events
callbacks and before `setTimeout` and `setInterval` . Returns an
`immediateObject` for possible use with `clearImmediate()`. Optionally you
can also pass arguments to the callback.
-->

`callback` を「即時」 (I/O イベントのコールバックより後、`setTimeout` および
`setInterval` よりも前) に実行するようスケジュールします。
`clearImmediate()` に渡すことのできる `immediateObject` を返します。
オプションとして、コールバックへの引数を渡すことができます。

<!--
Immediates are queued in the order created, and are popped off the queue once
per loop iteration. This is different from `process.nextTick` which will
execute `process.maxTickDepth` queued callbacks per iteration. `setImmediate`
will yield to the event loop after firing a queued callback to make sure I/O is
not being starved. While order is preserved for execution, other I/O events may
fire between any two scheduled immediate callbacks.
-->

`setImmediate()` は作成された順でキューに入れられ、イベントループの
繰り返し毎に一度、キューから取り出されます。
これは繰り返し毎に `process.maxTickDepth` で指定された回数までキューに
入れられた関数が実行される `process.nextTick()` とは異なります。
`setImmediate()` は I/O が沈み込まないように、キューイングされたコールバックが
呼び出された後イベントループに制御を譲ります。
二つの `setImmediate()` のコールバックが実行される順序は維持されますが、
他の I/O が間に挟まるかもしれません。

## clearImmediate(immediateObject)

<!--
Stops an immediate from triggering.
-->

`setImmediate()` に渡した関数が呼び出されることを中止します。
