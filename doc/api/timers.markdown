## Timers

### setTimeout(callback, delay, [arg], [...])

<!--

To schedule execution of a one-time `callback` after `delay` milliseconds. Returns a
`timeoutId` for possible use with `clearTimeout()`. Optionally you can
also pass arguments to the callback.

-->
`delay` ミリ秒が経過した後で、
`callback` が一度だけ実行されるようにスケジュールします。
`clearTimeout()` で使うことができる `timeoutId` を返します。
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


### clearTimeout(timeoutId)

<!--

Prevents a timeout from triggering.

-->
タイムアウトがトリガーされるのを止めます。

### setInterval(callback, delay, [arg], [...])

<!--

To schedule the repeated execution of `callback` every `delay` milliseconds.
Returns a `intervalId` for possible use with `clearInterval()`. Optionally
you can also pass arguments to the callback.

-->
`delay` ミリ秒が経過するごとに繰り返し `callback` が実行されるようにスケジュールします。
`clearInterval()` で使うことができる `intervalId` を返します。
オプションとして、コールバックへの引数を渡すこともできます。

### clearInterval(intervalId)

<!--

Stops a interval from triggering.

-->
インターバルがトリガーされるのを止めます。
