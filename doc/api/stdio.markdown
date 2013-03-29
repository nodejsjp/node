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

<!--
The console functions are synchronous when the destination is a terminal or
a file (to avoid lost messages in case of premature exit) and asynchronous
when it's a pipe (to avoid blocking for long periods of time).
-->

コンソール関数は出力先がターミナルまたはファイルの場合は同期
(早すぎる終了によりメッセージが失われるケースを防ぐため)、
パイプの場合は非同期 (長時間ブロックすることを防ぐため) です。

<!--
That is, in the following example, stdout is non-blocking while stderr
is blocking:
-->

つまり、以下の例では標準出力はノンブロッキングですが、
標準エラー出力はブロッキングです:

    $ node script.js 2> error.log | tee info.log

<!--
In daily use, the blocking/non-blocking dichotomy is not something you
should worry about unless you log huge amounts of data.
-->

通常の使用では、膨大な量のデータを記録するのではない限り、
ブロッキング／ノンブロッキングのどちらなのかを心配する必要はありません。


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
is used on each argument.  See [util.format()][] for more information.
-->

最初の引数文字列からフォーマット要素が見つからなかった場合は、
`util.inspect` が各引数に使われます。
より詳細は [util.format()][] を参照してください。

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
Uses `util.inspect` on `obj` and prints resulting string to stdout. This functi
bypasses any custom `inspect()` function on `obj`.
-->

`util.inspect` を使って `obj` を文字列化した結果を標準出力にプリントします。
この関数はあらゆるオブジェクトのカスタム `inspect()` 関数をバイパスします。

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
Same as [assert.ok()][] where if the `expression` evaluates as `false` throw an
AssertionError with `message`.
-->

[assert.ok()][] と同様に、もし `expression` が `false` に評価されると、
`message` を持つ AssertionError がスローされます。

[assert.ok()]: assert.html#assert_assert_value_message_assert_ok_value_message
[util.format()]: util.html#util_util_format_format
