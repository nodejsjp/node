## Global Objects

<!--

These object are available in all modules. Some of these objects aren't
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

### require.paths

<!--

An array of search paths for `require()`.  This array can be modified to add
custom paths.

-->
`require()` のためのサーチパスの配列です。
この配列はカスタムパスを追加するために変更することができます。

<!--

Example: add a new path to the beginning of the search list

-->
例: サーチリストの先頭に新しいパスを追加する

    require.paths.unshift('/usr/local/node');


### __filename

<!--

The filename of the script being executed.  This is the absolute path, and not necessarily
the same filename passed in as a command line argument.

-->
実行されているスクリプトのファイル名です。これは絶対パスであり、
必ずしもコマンドライン引数で渡されたファイル名と同じではありません。

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

The dirname of the script being executed.

-->
スクリプトが実行されているディレクトリ名です。

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
`module.exports` is the same as the `exports` object. See `src/node.js`
for more information.
`module` isn't actually a global but rather local to each module.

-->
現在のモジュールへの参照です。
特に `module.exports` は `exports` オブジェクトと同じです。
より詳しくは `src/node.js` を参照してください。
`module` は実際はグローバルではなく、各モジュール毎のローカルです。

### setTimeout(cb, ms)
### clearTimeout(t)
### setInterval(cb, ms)
### clearInterval(t)

<!--

The timer functions are global variables. See the [timers](timers.html) section.
-->
タイマー関数はグローバル変数です。[タイマー](timers.html) を参照してください。
