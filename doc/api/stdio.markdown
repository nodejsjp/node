# console

    Stability: 4 - API Frozen

* {Object}

<!--type=global-->

<!--
For printing to stdout and stderr.  Similar to the console object functions
provided by most web browsers, here the output is sent to stdout or stderr.
-->

標準出力と標準エラーに出力するためのものです。
ほとんどのブラウザで提供されているコンソールオブジェクトと同様ですが、
出力は標準出力か標準エラー出力に送られます。

## console.log([data], [...])

<!--
Prints to stdout with newline. This function can take multiple arguments in a
`printf()`-like way. Example:
-->

改行を伴って標準出力へプリントします。
この関数は `printf()` のように複数の引数を受け付けます。

    console.log('count: %d', count);

<!--
If formatting elements are not found in the first string then `util.inspect`
is used on each argument.
See [util.format()](util.html#util.format) for more information.
-->

最初の引数文字列からフォーマット要素が見つからなかった場合は、
`util.inspect` が各引数に使われます。
より詳細は [util.format()](util.html#util.format) を参照してください。

## console.info([data], [...])

<!--
Same as `console.log`.
-->

`console.log` と同じです。

## console.error([data], [...])

<!--
Same as `console.log` but prints to stderr.
-->

`console.log` と同様ですが、標準エラー出力にプリントします。

## console.warn([data], [...])

<!--
Same as `console.error`.
-->

`console.error()` と同じです。

## console.dir(obj)

<!--
Uses `util.inspect` on `obj` and prints resulting string to stdout.
-->

`util.inspect` を使って `obj` を文字列化した結果を標準出力にプリントします。

## console.time(label)

<!--
Mark a time.
-->

タイマを作成します。

## console.timeEnd(label)

<!--
Finish timer, record output. Example:
-->

タイマを終了し、結果を出力します。例:

    console.time('100-elements');
    for (var i = 0; i < 100; i++) {
      ;
    }
    console.timeEnd('100-elements');

## console.trace(label)

<!--
Print a stack trace to stderr of the current position.
-->

現在のスタックトレースを標準エラー出力にプリントします。

## console.assert(expression, [message])

<!--
Same as [assert.ok()](assert.html#assert_assert_value_message_assert_ok_value_message)
where if the `expression` evaluates as `false` throw an AssertionError with `message`.
-->

[assert.ok()](assert.html#assert_assert_value_message_assert_ok_value_message)
を参照してください。
もし `expression` が `false` に評価されると、`message` を持つ AssertionError
がスローされます。
