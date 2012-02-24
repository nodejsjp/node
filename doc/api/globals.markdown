## Global Objects

<!--

These objects are available in all modules. Some of these objects aren't
actually in the global scope but in the module scope - this will be noted.

-->
これらのオブジェクトは全てのモジュールで有効です。
これらのオブジェクトのいくつかは実際はグローバルスコープではなくモジュールスコープです - 注意してください。

### global

<!--

The global namespace object.

In browsers, the top-level scope is the global scope. That means that in
browsers if you're in the global scope `var something` will define a global
variable. In Node this is different. The top-level scope is not the global
scope; `var something` inside a Node module will be local to that module.

-->
グローバルなネームスペースのオブジェクトです。

ブラウザでは、トップレベルのスコープはグローバルスコープです。
これは、ブラウザではグローバルスコープで `var something` と定義するとグローバル変数になることを意味します。
Node では異なります。
トップレベルのスコープはグローバルスコープではありません;
Node のモジュール内での `var something` はそのモジュールでローカルになります。

### process

<!--

The process object. See the [process object](process.html#process) section.

-->
プロセスオブジェクトです。[process object](process.html#process) の節を参照してください。

### console

<!--

Used to print to stdout and stderr. See the [stdio](stdio.html) section.

-->
標準出力および標準エラー出力へのプリントに使われます。
[標準入出力](stdio.html) を参照してください。

### Buffer

<!--

Used to handle binary data. See the [buffers](buffers.html) section.

-->
バイナリデータを扱うために使われます。
[buffers](buffers.html) を参照してください。

### require()

<!--

To require modules. See the [Modules](modules.html#modules) section.
`require` isn't actually a global but rather local to each module.

-->
require モジュールを指します。[モジュール](modules.html#modules) の節を参照してください。
`require` は実際はグローバルではなく、各モジュール毎のローカルです。

### require.resolve()

<!--

Use the internal `require()` machinery to look up the location of a module,
but rather than loading the module, just return the resolved filename.

-->
`require()` の内部でモジュールの位置を検索するために使われます。
モジュールのロードは行わず、ファイル名を解決して返すだけです。

### require.cache

<!--

Modules are cached in this object when they are required. By deleting a key
value from this object, the next `require` will reload the module.

-->
モジュールが要求されると、このオブジェクトの中にキャッシュされます。
このオブジェクトからキーと値を削除すると、次にそのモジュールが
`require` されたときにリロードされます。

### __filename

<!--

The filename of the code being executed.  This is the resolved absolute path
of this code file.  For a main program this is not necessarily the same
filename used in the command line.  The value inside a module is the path
to that module file.

-->
実行されているコードのファイル名です。これは解決された絶対パスです。
メインプログラムでは、必ずしもコマンドライン引数で使われたファイル名と
同じではありません。
モジュールの中では、この値はそのモジュールファイルのパスとなります。

<!--

Example: running `node example.js` from `/Users/mjr`

-->
例: `node example.js` を `/Users/mjr` で実行する

    console.log(__filename);
    // /Users/mjr/example.js

<!--

`__filename` isn't actually a global but rather local to each module.

-->
`__filename` は実際はグローバルではなく、各モジュール毎のローカルです。

### __dirname

<!--

The name of the directory that the currently executing script resides in.

-->
現在実行されているスクリプトが存在するディレクトリの名前です。

<!--

Example: running `node example.js` from `/Users/mjr`

-->
例: `node example.js` を `/Users/mjr` で実行する

    console.log(__dirname);
    // /Users/mjr

<!--

`__dirname` isn't actually a global but rather local to each module.

-->
`__dirname` は実際はグローバルではなく、各モジュール毎のローカルです。


### module

<!--

A reference to the current module. In particular
`module.exports` is the same as the `exports` object.
`module` isn't actually a global but rather local to each module.

See the [module system documentation](modules.html) for more
information.

-->
現在のモジュールへの参照です。
特に `module.exports` は `exports` オブジェクトと同じです。
`module` は実際はグローバルではなく、各モジュール毎のローカルです。

より詳しくは [module system documentation](modules.html) を参照してください。


### exports

<!--

An object which is shared between all instances of the current module and
made accessible through `require()`.
`exports` is the same as the `module.exports` object.
`exports` isn't actually a global but rather local to each module.

See the [module system documentation](modules.html) for more
information.
-->

現在のモジュールの全てのインスタンス間で共有されるオブジェクトで、
`require` を通じてアクセス可能になります。
`exports` は `module.exports` と同じオブジェクトです。
`exports` は実際はグローバルではなく、各モジュール毎のローカルです。

より詳しくは [module system documentation](modules.html) を参照してください。


### setTimeout(cb, ms)
### clearTimeout(t)
### setInterval(cb, ms)
### clearInterval(t)

<!--

The timer functions are global variables. See the [timers](timers.html) section.
-->
タイマー関数はグローバル変数です。[タイマー](timers.html) を参照してください。
