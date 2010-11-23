## util

<!--
These functions are in the module `'util'`. Use `require('util')` to access
them.
-->
これらの関数はモジュール `'util'` 内にあります。
`require('util')` を使うことでこれらにアクセスします。


### util.debug(string)

<!--
A synchronous output function. Will block the process and
output `string` immediately to `stderr`.
-->
同期的な出力関数です。プロセスをブロックして即座に `string` を `stderr` に出力します。

    require('util').debug('message on stderr');


### util.log(string)

<!--
Output with timestamp on `stdout`.
-->
タイムスタンプとともに `stdout` へ出力します。

    require('util').log('Timestmaped message.');


### util.inspect(object, showHidden=false, depth=2)

<!--
Return a string representation of `object`, which is useful for debugging.
-->
デバッグで有用な、`object` の文字列表現を返します。

<!--
If `showHidden` is `true`, then the object's non-enumerable properties will be
shown too.
-->
`showHidden` が `true` の場合、
オブジェクトの Enumerable でないプロパティも表示されます。

<!--
If `depth` is provided, it tells `inspect` how many times to recurse while
formatting the object. This is useful for inspecting large complicated objects.
-->
`depth` が与えられた場合、
オブジェクトをフォーマットするために何回再帰するかを `inspect` に伝えます。
これは巨大で複雑なオブジェクトを調査する場合に便利です。

<!--
The default is to only recurse twice.  To make it recurse indefinitely, pass
in `null` for `depth`.
-->
デフォルトでは 2 回だけ再帰します。
無限に再帰するには、`depth` に `null` を渡します。


<!--
Example of inspecting all properties of the `util` object:
-->
`util` オブジェクトの全てのプロパティを調査する例:

    var util = require('util');

    console.log(util.inspect(util, true, null));


### util.pump(readableStream, writeableStream, [callback])

<!--
Experimental
-->
実験的

<!--
Read the data from `readableStream` and send it to the `writableStream`.
When `writeableStream.write(data)` returns `false` `readableStream` will be
paused until the `drain` event occurs on the `writableStream`. `callback` gets
an error as its only argument and is called when `writableStream` is closed or
when an error occurs.
-->
`readableStream` からデータを読み、それ を`writableStream` に送ります。
`writeableStream.write(data)` が `false` を返す場合、
`writableStream` が `drain` イベントを生成するまで、
`readableStream` は中断します。
`writableStream` がクローズされるかエラーが発生すると、`callback` は error を唯一の引数として呼び出されます。
