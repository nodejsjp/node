## util

<!--

These functions are in the module `'util'`. Use `require('util')` to access
them.

-->
これらの関数はモジュール `'util'` 内にあります。
`require('util')` を使うことでこれらにアクセスします。


### util.format()

<!--
Returns a formatted string using the first argument as a `printf`-like format.
-->
最初の引数を `printf` のようなフォーマットとして使用して、フォーマット化された
文字列を返します。

<!--
The first argument is a string that contains zero or more *placeholders*.
Each placeholder is replaced with the converted value from its corresponding
argument. Supported placeholders are:
-->
第一引数は文字列で、0 個以上の *プレースホルダ* を含みます。
それぞれのプレースホルダは対応する引数を変換した値で置換されます。
サポートするプレースホルダは:

<!--
* `%s` - String.
* `%d` - Number (both integer and float).
* `%j` - JSON.
* `%%` - single percent sign (`'%'`). This does not consume an argument.
-->
* `%s` - 文字列。
* `%d` - 数値 (整数と浮動小数点数の両方)。
* `%j` - JSON。
* `%%` - 一つのパーセント記号 (`'%'`)。これは引数を消費しません。

<!--
If the placeholder does not have a corresponding argument, the placeholder is
not replaced.
-->
プレースホルダに対応する引数が無い場合、そのプレースホルダは置換されません。

    util.format('%s:%s', 'foo'); // 'foo:%s'

<!--
If there are more arguments than placeholders, the extra arguments are
converted to strings with `util.inspect()` and these strings are concatenated,
delimited by a space.
-->
プレースホルダより多くの引数がある場合、余った引数は `util.inspect()` によって
文字列化され、それらはスペース区切りで連結されます。

    util.format('%s:%s', 'foo', 'bar', 'baz'); // 'foo:bar baz'

<!--
If the first argument is not a format string then `util.format()` returns
a string that is the concatenation of all its arguments separated by spaces.
Each argument is converted to a string with `util.inspect()`.
-->
第一引数がフォーマット文字列ではない場合、`util.format()` は全ての引数を
スペース区切りで連結して返します。
ここの引数は `util.inspect()` で文字列に変換されます。

    util.format(1, 2, 3); // '1 2 3'


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

    require('util').log('Timestamped message.');


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


### util.pump(readableStream, writableStream, [callback])

<!--

Experimental

-->
実験的

<!--

Read the data from `readableStream` and send it to the `writableStream`.
When `writableStream.write(data)` returns `false` `readableStream` will be
paused until the `drain` event occurs on the `writableStream`. `callback` gets
an error as its only argument and is called when `writableStream` is closed or
when an error occurs.

-->
`readableStream` からデータを読み、それ を`writableStream` に送ります。
`writableStream.write(data)` が `false` を返す場合、
`writableStream` が `drain` イベントを生成するまで、
`readableStream` は中断します。
`writableStream` がクローズされるかエラーが発生すると、`callback` は error を唯一の引数として呼び出されます。


### util.inherits(constructor, superConstructor)

<!--

Inherit the prototype methods from one
[constructor](https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Object/constructor)
into another.  The prototype of `constructor` will be set to a new
object created from `superConstructor`.

-->
ある
[コンストラクタ](https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Object/constructor)
からプロトタイプメソッドを継承します。
`constructor` のプロトタイプは `superConstructor` から作られたオブジェクトに設定されます。

<!--

As an additional convenience, `superConstructor` will be accessible
through the `constructor.super_` property.

-->
さらなる利便性のために、`superConstructor` は `constructor.super_`
プロパティを通じてアクセスすることができるようになります。

    var util = require("util");
    var events = require("events");

    function MyStream() {
        events.EventEmitter.call(this);
    }

    util.inherits(MyStream, events.EventEmitter);

    MyStream.prototype.write = function(data) {
        this.emit("data", data);
    }

    var stream = new MyStream();

    console.log(stream instanceof events.EventEmitter); // true
    console.log(MyStream.super_ === events.EventEmitter); // true

    stream.on("data", function(data) {
        console.log('Received data: "' + data + '"');
    })
    stream.write("It works!"); // Received data: "It works!"
