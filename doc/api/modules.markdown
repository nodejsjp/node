## Standard Modules

<!--

Node comes with a number of modules that are compiled in to the process,
most of which are documented below.  The most common way to use these modules
is with `require('name')` and then assigning the return value to a local
variable with the same name as the module.

-->
Node にはプロセス内にコンパイルされたいくつかのモジュールが含まれており、
そのほとんどは以下に文書化されています。
これらのモジュールを使用するもっとも一般的な方法は、
`require('name')` の戻り値を、モジュールと同じ名前のローカル変数に割り当てることです。

<!--

Example:

-->
例:

    var util = require('util');

<!--

It is possible to extend node with other modules.  See `'Modules'`

-->
その他のモジュールで node を拡張することも可能です。「モジュール」を参照してください。

## Modules

<!--

Node uses the CommonJS module system.

-->
Node は CommonJS のモジュールシステムを使います。

<!--

Node has a simple module loading system.  In Node, files and modules are in
one-to-one correspondence.  As an example, `foo.js` loads the module
`circle.js` in the same directory.

-->
Node はシンプルなモジュールローディングシステムを持ちます。
Node では、ファイルとモジュールは１対１に対応します。
例として、 `foo.js` は、同じディレクトリにある `circle.js` をロードしています。

<!--

The contents of `foo.js`:

-->
`foo.js` の内容:

    var circle = require('./circle');
    console.log( 'The area of a circle of radius 4 is '
               + circle.area(4));

<!--

The contents of `circle.js`:

-->
`circle.js` の内容:

    var PI = 3.14;

    exports.area = function (r) {
      return PI * r * r;
    };

    exports.circumference = function (r) {
      return 2 * PI * r;
    };

<!--

The module `circle.js` has exported the functions `area()` and
`circumference()`.  To export an object, add to the special `exports`
object.  (Alternatively, one can use `this` instead of `exports`.) Variables
local to the module will be private. In this example the variable `PI` is
private to `circle.js`. The function `puts()` comes from the module `'util'`,
which is a built-in module. Modules which are not prefixed by `'./'` are
built-in module--more about this later.

-->
`circle.js` モジュールは `area()` と `circumference()` をエクスポートしています。
エクスポートされたオブジェクトは、 `exports` という特別なオブジェクトに加えられます
(`exports` の代わりに `this` を使うことも出来ます)。
モジュールのローカル変数はプライベートです。
この例の場合、変数 `PI` は `circle.js` のプライベート変数です。
関数 `puts()` は ビルトインモジュールである `'sys'` の物です。
プレフィックス `'./'` のないモジュールはビルトインモジュールです。詳細は以降で説明します。

### Module Resolving

<!--

A module prefixed with `'./'` is relative to the file calling `require()`.
That is, `circle.js` must be in the same directory as `foo.js` for
`require('./circle')` to find it.

-->
プレフィックス `'./'` が付けられたモジュールは `require()` を呼び出したモジュールからの相対パスとなります。
つまり `circle.js` は `require('./circle')` が見つけられるように `foo.js` と同じディレクトリにある必要が有ります。

<!--

Without the leading `'./'`, like `require('assert')` the module is searched
for in the `require.paths` array. `require.paths` on my system looks like
this:

-->
先頭の `'./'` 無しで、例えば `require('assert')` の様にモジュールを指定した場合、
モジュールは `require.paths` の配列内の場所を起点に検索されます。
私のシステムでは、 `require.paths` はこの様になっています: 

`[ '/home/ryan/.node_modules' ]`

<!--

That is, when `require('foo')` is called Node looks for:

-->
これによって、`require('foo')` が呼ばれると Node は以下の順でモジュールを検索します

* 1: `/home/ryan/.node_modules/foo`
* 2: `/home/ryan/.node_modules/foo.js`
* 3: `/home/ryan/.node_modules/foo.node`
* 4: `/home/ryan/.node_modules/foo/index.js`
* 5: `/home/ryan/.node_modules/foo/index.node`

<!--

interrupting once a file is found. Files ending in `'.node'` are binary Addon
Modules; see 'Addons' below. `'index.js'` allows one to package a module as
a directory.

-->
ファイルが見つかると、その時点で検索は終了します。
ファイル名が `'.node'` で終わるファイルは、バイナリ形式のアドオンモジュールです。
詳細は「アドオン」を参照してください。
`'index.js'` は、ディレクトリをモジュールとして一つにまとめることを可能にします。

<!--

`require.paths` can be modified at runtime by simply unshifting new
paths onto it, or at startup with the `NODE_PATH` environmental
variable (which should be a list of paths, colon separated).
Additionally node will search for directories called `node_modules` starting
at the current directory (of the module calling `require`) and upwards
towards the root of the package tree.
This feature makes it easy to have different module versions for different
environments. Imagine the situation where you have a devopment environment
and a production environment each with a different version of the `foo`
module: `projects/x/development/node_modules/foo` and
`projects/x/production/node_modules/foo`.

-->
`require.paths` は、配列に新しいパスを加えるか、`NODE_PATH` 環境変数と共に起動することで変更することが出来ます
(この場合は、コロンで区切られたパスのリストを渡す必要があります)。
加えて、node は `node_modules` と呼ばれるディレクトリを
(モジュールが `require` で呼び出された)、現在のディレクトリからパッケージツリーのルートまで、
上位に向かって検索します。
この機能は、異なった環境のために異なったバージョンのモジュールを使うことを容易にします。
開発環境とプロダクション環境で異なったバージョンの `foo` モジュール:
`projects/x/development/node_modules/foo` と `projects/x/production/node_modules/foo`
を使う状況を想像してください。


<!--

The second time `require('foo')` is called, it is not loaded again from
disk. It looks in the `require.cache` object to see if it has been loaded
before.

-->
`require('foo')` の 2 度目の呼び出しでは、ディスクから再度読み込むことはしません。
既にロード済みかを調べるために `require.cache` オブジェクトをチェックします。

<!--

To get the exact filename that will be loaded when `require()` is called, use
the `require.resolve()` function.

-->
`require()` が呼び出されると、正確なファイル名を得るために
`require.resolve()` 関数が使われます。
