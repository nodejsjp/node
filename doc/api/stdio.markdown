## console

<!--
Browser-like object for printing to stdout and stderr.
-->
標準出力と標準エラー出力に対するブラウザライクなオブジェクトです。

### console.log()

<!--
Prints to stdout with newline. This function can take multiple arguments in a
`printf()`-like way. Example:
-->
改行を伴って標準出力へプリントします。
この関数は `printf()` のように複数の引数を受け付けます。

    console.log('count: %d', count);

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
-->
* `%s` - 文字列。
* `%d` - 数値 (整数と浮動小数点数の両方)。
* `%j` - JSON。

<!--
If the placeholder does not have a corresponding argument, `undefined` is used.
-->
プレースホルダに対応する引数が無い場合は、`undefined` が使われます

    console.log('%s:%s', 'foo'); // 'foo:undefined'

<!--
If there are more arguments than placeholders, the extra arguments are
converted to strings with `util.inspect()` and these strings are concatenated,
delimited by a space.
-->
プレースホルダより多くの引数がある場合、余った引数は `util.inspect()` によって
文字列化され、それらはスペース区切りで連結されます。

    console.log('%s:%s', 'foo', 'bar', 'baz'); // 'foo:bar baz'

<!--
If the first argument is not a format string then `console.log()` prints
a string that is the concatenation of all its arguments separated by spaces.
Each argument is converted to a string with `util.inspect()`.
-->
第一引数がフォーマット文字列ではない場合、`console.log()` は全ての引数を
スペース区切りで連結して出力します。
個々の引数は `util.inspect()` で文字列に変換されます。

    console.log(1, 2, 3); // '1 2 3'

### console.info()

<!--
Same as `console.log`.
-->
`console.log` と同じです。

### console.warn()
### console.error()

<!--
Same as `console.log` but prints to stderr.
-->
`console.log` と同様ですが、標準エラー出力にプリントします。

### console.dir(obj)

<!--
Uses `util.inspect` on `obj` and prints resulting string to stderr.
-->
`util.inspect` を使って `obj` を文字列化した結果を標準エラー出力にプリントします。

### console.time(label)

<!--
Mark a time.
-->
タイマを作成します。


### console.timeEnd(label)

<!--
Finish timer, record output. Example
-->
タイマを終了し、結果を出力します。例

    console.time('100-elements');
    for (var i = 0; i < 100; i++) {
      ;
    }
    console.timeEnd('100-elements');


### console.trace()

<!--
Print a stack trace to stderr of the current position.
-->
現在のスタックトレースを標準エラー出力にプリントします。

### console.assert()

<!--
Same as `assert.ok()`.
-->
`assert.ok()` と同じです。

