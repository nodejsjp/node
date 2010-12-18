## Global Objects

<!--

These object are available in the global scope and can be accessed from anywhere.

-->
これらのオブジェクトはグローバルスコープで有効であり、どこからでもアクセスすることができます。

### global

<!--

The global namespace object.

-->
グローバルなネームスペースのオブジェクトです。

### process

<!--

The process object. See the `'process object'` section.

-->
プロセスオブジェクトです。`'process object'` の節を参照してください。

### require()

<!--

To require modules. See the `'Modules'` section.

-->
require モジュールを指します。`'Modules'` の節を参照してください。

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


### module

<!--

A reference to the current module. In particular
`module.exports` is the same as the `exports` object. See `src/node.js`
for more information.

-->
現在のモジュールへの参照です。
特に `module.exports` は `exports` オブジェクトと同じです。
より詳しくは `src/node.js` を参照してください。
