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
