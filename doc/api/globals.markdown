# Global Objects

<!-- type=misc -->

<!--
These objects are available in all modules. Some of these objects aren't
actually in the global scope but in the module scope - this will be noted.
-->

これらのオブジェクトは全てのモジュールで有効です。
これらのオブジェクトのいくつかは実際はグローバルスコープではなくモジュールスコープです - 注意してください。

## global

<!-- type=global -->

<!--
* {Object} The global namespace object.
-->

* {Object} グローバルなネームスペースのオブジェクト

<!--
In browsers, the top-level scope is the global scope. That means that in
browsers if you're in the global scope `var something` will define a global
variable. In Node this is different. The top-level scope is not the global
scope; `var something` inside a Node module will be local to that module.
-->

ブラウザでは、トップレベルのスコープはグローバルスコープです。
これは、ブラウザではグローバルスコープで `var something` と定義するとグローバル変数になることを意味します。
Node では異なります。
トップレベルのスコープはグローバルスコープではありません;
Node のモジュール内での `var something` はそのモジュールでローカルになります。

## process

<!-- type=global -->

* {Object}

<!--
The process object. See the [process object][] section.
-->

プロセスオブジェクトです。[process オブジェクト][] の節を参照してください。

## console

<!-- type=global -->

* {Object}

<!--
Used to print to stdout and stderr. See the [stdio][] section.
-->

標準出力および標準エラー出力へのプリントに使われます。
[標準入出力][] を参照してください。

## Class: Buffer

<!-- type=global -->

* {Function}

<!--
Used to handle binary data. See the [buffer section][]
-->

バイナリデータを扱うために使われます。
[バッファセクション][] を参照してください。

## require()

<!-- type=var -->

* {Function}

<!--
To require modules. See the [Modules][] section.  `require` isn't actually a
global but rather local to each module.
-->

require モジュールを指します。[モジュール][] の節を参照してください。
`require` は実際はグローバルではなく、各モジュール毎のローカルです。

### require.resolve()

<!--
Use the internal `require()` machinery to look up the location of a module,
but rather than loading the module, just return the resolved filename.
-->

`require()` の内部でモジュールの位置を検索するために使われます。
モジュールのロードは行わず、ファイル名を解決して返すだけです。

### require.cache

* {Object}

<!--
Modules are cached in this object when they are required. By deleting a key
value from this object, the next `require` will reload the module.
-->

モジュールが要求されると、このオブジェクトの中にキャッシュされます。
このオブジェクトからキーと値を削除すると、次にそのモジュールが
`require` されたときにリロードされます。

### require.extensions

* {Array}

<!--
Instruct `require` on how to handle certain file extensions.

Process files with the extension `.sjs` as `.js`:
-->

`require` にファイル拡張子を扱う方法を教えます。

拡張子が `.sjs` であるファイルの処理を `.js` と同じにするには:

    require.extensions['.sjs'] = require.extensions['.js'];

<!--
Write your own extension handler:
-->

独自の拡張子ハンドラを記述するには:

    require.extensions['.sjs'] = function(module, filename) {
      var content = fs.readFileSync(filename, 'utf8');
      // Parse the file content and give to module.exports
      module.exports = content;
    };

## __filename

<!-- type=var -->

* {String}

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

## __dirname

<!-- type=var -->

* {String}

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


## module

<!-- type=var -->

* {Object}

<!--
A reference to the current module. In particular
`module.exports` is the same as the `exports` object.
`module` isn't actually a global but rather local to each module.

See the [module system documentation][] for more information.
-->

現在のモジュールへの参照です。
特に `module.exports` は `exports` オブジェクトと同じです。
`module` は実際はグローバルではなく、各モジュール毎のローカルです。

より詳しくは [モジュールシステムのドキュメント][] を参照してください。

## exports

<!-- type=var -->

<!--
An object which is shared between all instances of the current module and
made accessible through `require()`.
`exports` is the same as the `module.exports` object.
`exports` isn't actually a global but rather local to each module.

See the [module system documentation][] for more information.

See the [module section][] for more information.
-->

現在のモジュールの全てのインスタンス間で共有されるオブジェクトで、
`require` を通じてアクセス可能になります。
`exports` は `module.exports` と同じオブジェクトです。
`exports` は実際はグローバルではなく、各モジュール毎のローカルです。

より詳しくは [モジュールシステムのドキュメント][] を参照してください。

## setTimeout(cb, ms)
## clearTimeout(t)
## setInterval(cb, ms)
## clearInterval(t)

<!--type=global-->

<!--
The timer functions are global variables. See the [timers][] section.
-->

タイマー関数はグローバル変数です。[タイマー][] を参照してください。

<!--
[buffer section]: buffer.html
[module section]: modules.html
[module system documentation]: modules.html
[Modules]: modules.html#modules_modules
[process object]: process.html#process_process
[stdio]: stdio.html
[timers]: timers.html
-->

[バッファセクション]: buffer.html
[モジュールセクション]: modules.html
[モジュールシステムのドキュメント]: modules.html
[モジュール]: modules.html#modules_modules
[process オブジェクト]: process.html#process_process
[標準入出力]: stdio.html
[タイマー]: timers.html
