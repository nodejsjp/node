# Modules

    Stability: 5 - Locked

<!--name=module-->

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

    var circle = require('./circle.js');
    console.log( 'The area of a circle of radius 4 is '
               + circle.area(4));

<!--
The contents of `circle.js`:
-->

`circle.js` の内容:

    var PI = Math.PI;

    exports.area = function (r) {
      return PI * r * r;
    };

    exports.circumference = function (r) {
      return 2 * PI * r;
    };

<!--
The module `circle.js` has exported the functions `area()` and
`circumference()`.  To export an object, add to the special `exports`
object.
-->

`circle.js` モジュールは `area()` と `circumference()`
をエクスポートしています。
オブジェクトをエクスポートするには、 `exports`
という特別なオブジェクトに加えます。

<!--
Note that `exports` is a reference to `module.exports` making it suitable
for augmentation only. If you are exporting a single item such as a
constructor you will want to use `module.exports` directly instead.
-->

`exports` は `module.exports` を参照しているに過ぎないことに注意してください。
もしコンストラクタなど、一つの項目だけをエクスポートしたいのであれば、
`module.exports` を直接使いたくなるでしょう。

    function MyConstructor (opts) {
      //...
    }

    // BROKEN: Does not modify exports
    exports = MyConstructor;

    // exports the constructor properly
    module.exports = MyConstructor;

<!--
Variables
local to the module will be private. In this example the variable `PI` is
private to `circle.js`.
-->

モジュールのローカル変数はプライベートです。
この例の場合、変数 `PI` は `circle.js` のプライベート変数です。

<!--
The module system is implemented in the `require("module")` module.
-->

モジュールシステムは `require("module")` モジュールによって実装されます。

## Cycles

<!--type=misc-->

<!--
When there are circular `require()` calls, a module might not be
done being executed when it is returned.

Consider this situation:
-->

`require()` が循環的に呼び出される場合、実行が完了していないモジュールが
返されることがあります。

次の状況を考えてください:

`a.js`:

    console.log('a starting');
    exports.done = false;
    var b = require('./b.js');
    console.log('in a, b.done = %j', b.done);
    exports.done = true;
    console.log('a done');

`b.js`:

    console.log('b starting');
    exports.done = false;
    var a = require('./a.js');
    console.log('in b, a.done = %j', a.done);
    exports.done = true;
    console.log('b done');

`main.js`:

    console.log('main starting');
    var a = require('./a.js');
    var b = require('./b.js');
    console.log('in main, a.done=%j, b.done=%j', a.done, b.done);

<!--
When `main.js` loads `a.js`, then `a.js` in turn loads `b.js`.  At that
point, `b.js` tries to load `a.js`.  In order to prevent an infinite
loop an **unfinished copy** of the `a.js` exports object is returned to the
`b.js` module.  `b.js` then finishes loading, and its `exports` object is
provided to the `a.js` module.

By the time `main.js` has loaded both modules, they're both finished.
The output of this program would thus be:
-->

`main.js` が `a.js` をロードすると、`a.js` は `b.js` をロードします。
ポイントは、`b.js` は `a.js` のロードを試みることです。
無限ループを避けるため、`a.js` がエクスポートしたオブジェクトの
**未完了のコピー** が `b.js` モジュールに返されます。
`b.js` のロードが完了すると、`exports` オブジェクトが `a.js`
モジュールに提供されます。

`main.js` が両方のモジュールをロードするまでには、どちらも完了します。
このプログラムの出力はこのようになります:

    $ node main.js
    main starting
    a starting
    b starting
    in b, a.done = false
    b done
    in a, b.done = true
    a done
    in main, a.done=true, b.done=true

<!--
If you have cyclic module dependencies in your program, make sure to
plan accordingly.
-->

プログラムが循環参照するモジュールを持つ場合は、計画が適切か確認してください。


## Core Modules

<!--type=misc-->

<!--
Node has several modules compiled into the binary.  These modules are
described in greater detail elsewhere in this documentation.

The core modules are defined in node's source in the `lib/` folder.

Core modules are always preferentially loaded if their identifier is
passed to `require()`.  For instance, `require('http')` will always
return the built in HTTP module, even if there is a file by that name.
-->

Node にはバイナリにコンパイル済みのいくつかのモジュールがあります。
これらのモジュールについては、このドキュメントの他の場所でより詳しく記述されています。

コアモジュールは、 Node のソースの `lib/` フォルダにて定義されています。

`require()` では常にコアモジュールの識別名を優先的に解釈します。
例えば `require('http')` は、例え同名のファイルが存在していたとしても、常にビルトイインの HTTP モジュールを返します。

## File Modules

<!--type=misc-->

<!--
If the exact filename is not found, then node will attempt to load the
required filename with the added extension of `.js`, `.json`, and then `.node`.

`.js` files are interpreted as JavaScript text files, and `.json` files are
parsed as JSON text files. `.node` files are interpreted as compiled addon
modules loaded with `dlopen`.

A module prefixed with `'/'` is an absolute path to the file.  For
example, `require('/home/marco/foo.js')` will load the file at
`/home/marco/foo.js`.

A module prefixed with `'./'` is relative to the file calling `require()`.
That is, `circle.js` must be in the same directory as `foo.js` for
`require('./circle')` to find it.

Without a leading '/' or './' to indicate a file, the module is either a
"core module" or is loaded from a `node_modules` folder.
-->

指定された名前のファイルが見つからなければ、 Node は指定されたファイル名に
`.js`、`.json`、または `.node` を付けたものを読み込もうとします。

`.js` ファイルは JavaScript ファイルとして解釈され、
`.json` ファイルは JSON ファイルとして解釈されます。
一方 `.node` ファイルはコンパイル済みのアドオンモジュールとして解釈され、
`dlopen` を使って読み込まれます。

`'/'` から始まるモジュールは、ファイルへの絶対パスと見なされます。
例えば、 `require('/home/marco/foo.js')` は `/home/macro/foo.js` を読み込みます。

`'./'` から始まるモジュールは、 `require()` を呼んだファイルからの相対パスになります。
すなわち、 `foo.js` から `require('./circle')` によって `circle.js` を読み込むには、 `circle.js` は `foo.js` と同じディレクトリに存在していなければなりません。

'/' や './' が先頭になければ、モジュールは "コアモジュール" であるかもしくは `node_modules` フォルダから読み込まれることになります。

<!--
If the given path does not exist, `require()` will throw an Error with its
`code` property set to `'MODULE_NOT_FOUND'`.
-->

与えられたパスが存在しなかった場合、`require()` は `code` プロパティに
`'MODULE_NOT_FOUND'` を設定したエラーをスローします。

## Loading from `node_modules` Folders

<!--type=misc-->

<!--
If the module identifier passed to `require()` is not a native module,
and does not begin with `'/'`, `'../'`, or `'./'`, then node starts at the
parent directory of the current module, and adds `/node_modules`, and
attempts to load the module from that location.

If it is not found there, then it moves to the parent directory, and so
on, until the root of the tree is reached.

For example, if the file at `'/home/ry/projects/foo.js'` called
`require('bar.js')`, then node would look in the following locations, in
this order:
-->

もし `require()` に渡されたモジュール識別子がネイティブモジュールではなく、かつ  `'/'` や `'../'` や `'./'` から始まらないならば、 Node は現在のモジュールの親ディレクトリに `'/node_modules'` を付与してそこからモジュールを読み込もうとします。

そこに見つからない場合はさらに親ディレクトリに移動し、モジュールが見つかるか root ディレクトリに到達するまで同様のことを繰り返していきます。

例えば `'/home/ry/projects/foo.js'` の中で `require('bar.js')` を呼んでいた場合、 Node は下記の位置を上から順番に見ていきます。

* `/home/ry/projects/node_modules/bar.js`
* `/home/ry/node_modules/bar.js`
* `/home/node_modules/bar.js`
* `/node_modules/bar.js`

<!--
This allows programs to localize their dependencies, so that they do not
clash.
-->

この仕組みによって、プログラムはクラッシュを避けるために依存関係を上書きすることができるのです。

## Folders as Modules

<!--type=misc-->

<!--
It is convenient to organize programs and libraries into self-contained
directories, and then provide a single entry point to that library.
There are three ways in which a folder may be passed to `require()` as
an argument.

The first is to create a `package.json` file in the root of the folder,
which specifies a `main` module.  An example package.json file might
look like this:
-->

プログラムとライブラリをディレクトリ内にまとめて、そのエントリポイントを提示するという便利な方法もあります。
それには `require()` に引数として何を渡すかによって3通りの方法があります。

1つ目は、 `package.json` というファイルをフォルダ直下に作成し、 `main` モジュールを指定するという方法です。
例えば、 package.json は以下のようなファイルになります:

    { "name" : "some-library",
      "main" : "./lib/some-library.js" }

<!--
If this was in a folder at `./some-library`, then
`require('./some-library')` would attempt to load
`./some-library/lib/some-library.js`.

This is the extent of Node's awareness of package.json files.

If there is no package.json file present in the directory, then node
will attempt to load an `index.js` or `index.node` file out of that
directory.  For example, if there was no package.json file in the above
example, then `require('./some-library')` would attempt to load:
-->

もし `./some-library` フォルダ内にこのファイルがあれば、 `require('./some-library')` は `./some-library/lib/some-library.js` を読みにいきます。

これは、 Node が package.json の存在に気づくことによってもたらされます。

もし package.json がディレクトリに存在していなければ、 Node はそのディレクトリで `index.js` もしくは `index.node` を探します。
例えば、もし上の例で package.json がいるが存在しないとすると、 `require('./some-library')` は以下のファイルを読み込もうとします: 

* `./some-library/index.js`
* `./some-library/index.node`

## Caching

<!--type=misc-->

<!--
Modules are cached after the first time they are loaded.  This means
(among other things) that every call to `require('foo')` will get
exactly the same object returned, if it would resolve to the same file.
-->

モジュールは初めて読み込まれたときにキャッシュされます。
すなわち（他のキャッシュと同様に） `require('foo')` を呼ぶたびに、もし引数の意味するものが同一のファイルであったなら全く同一のオブジェクトが返されます。

<!--
Multiple calls to `require('foo')` may not cause the module code to be
executed multiple times.  This is an important feature.  With it,
"partially done" objects can be returned, thus allowing transitive
dependencies to be loaded even when they would cause cycles.
-->

`require('foo')` が複数回呼び出されても、モジュールが複数回実行されることにはなりません。
これは重要な特徴です。
そのため、「部分的に完了した」オブジェクトを返すことで、
推移的な依存関係が循環していてもロードすることができます。

<!--
If you want to have a module execute code multiple times, then export a
function, and call that function.
-->

もしモジュールを複数回実行したければ、関数を公開して、
その関数を呼び出してください。

### Module Caching Caveats

<!--type=misc-->

<!--
Modules are cached based on their resolved filename.  Since modules may
resolve to a different filename based on the location of the calling
module (loading from `node_modules` folders), it is not a *guarantee*
that `require('foo')` will always return the exact same object, if it
would resolve to different files.
-->

モジュールは解決されたファイル名に基づいてキャッシュされます。
異なる場所にあるモジュールから呼び出されたモジュールは、
(`node_module` フォルダからロードされるため) 異なったファイル名で
解決されることがあるため、 `require('foo')` が常に同じオブジェクトを返す
*保証*はなく、異なるファイルとして解決されます。

## The `module` Object

<!-- type=var -->

<!-- name=module -->

* {Object}

<!--
In each module, the `module` free variable is a reference to the object
representing the current module.  In particular
`module.exports` is accessible via the `exports` module-global.
`module` isn't actually a global but rather local to each module.
-->

どのモジュールでも、`module` 自由変数は現在のモジュールを表現するオブジェクトを
参照します。特に、`module.exports` はモジュールグローバルの `exports`
を通じてアクセスすることができます。
`module` は実際はグローバルではなく、各モジュールにローカルです。

### module.exports

* {Object}

<!--
The `module.exports` object is created by the Module system. Sometimes this is not
acceptable, many want their module to be an instance of some class. To do this
assign the desired export object to `module.exports`. For example suppose we
were making a module called `a.js`
-->

`module.exports` オブジェクトはモジュールシステムによって作成されます。
時々これは受け入れられず、多くのモジュールは何らかのクラスのインスタンスであることを望みます。
それには公開したいオブジェクトを `module.exports` に割り当てます。
例えば `a.js` と呼ばれるモジュールを作るとしたら


    var EventEmitter = require('events').EventEmitter;

    module.exports = new EventEmitter();

    // Do some work, and after some time emit
    // the 'ready' event from the module itself.
    setTimeout(function() {
      module.exports.emit('ready');
    }, 1000);

<!--
Then in another file we could do
-->

そして別のファイルで

    var a = require('./a');
    a.on('ready', function() {
      console.log('module a is ready');
    });


<!--
Note that assignment to `module.exports` must be done immediately. It cannot be
done in any callbacks.  This does not work:
-->

`module.exports` への代入はすぐに行わなければなりません。
コールバックの中ではできません。以下は動きません。

x.js:

    setTimeout(function() {
      module.exports = { a: "hello" };
    }, 0);

y.js:

    var x = require('./x');
    console.log(x.a);


### module.require(id)

<!--
* `id` {String}
* Return: {Object} `module.exports` from the resolved module
-->

* `id` {String}
* Return: {Object} 解決されたモジュールの `module.exports`

<!--
The `module.require` method provides a way to load a module as if
`require()` was called from the original module.
-->

`module.require` メソッドは、元のモジュールが `require()`
を呼び出したかのようにモジュールをロードするために提供されています。

<!--
Note that in order to do this, you must get a reference to the `module`
object.  Since `require()` returns the `module.exports`, and the `module` is
typically *only* available within a specific module's code, it must be
explicitly exported in order to be used.
-->

それには `module` オブジェクトの参照が必要なことに注意してください。
`require()` が `module.exports` を返した後、一般的に `module` 
はそのモジュールのコードで *のみ* 利用可能です。
それが使われるようにするには、明示的にエクスポートする必要があります。

### module.id

* {String}

<!--
The identifier for the module.  Typically this is the fully resolved
filename.
-->

モジュールの識別子。通常は完全に解決されたファイル名です。


### module.filename

* {String}

<!--
The fully resolved filename to the module.
-->

完全に解決されたモジュールのファイル名です。


### module.loaded

* {Boolean}

<!--
Whether or not the module is done loading, or is in the process of
loading.
-->

モジュールのロードが完了したか，あるいはローディング中かを示します。


### module.parent

* {Module Object}

<!--
The module that required this one.
-->

このモジュールを要求したモジュールです。


### module.children

* {Array}

<!--
The module objects required by this one.
-->

このモジュールが要求したモジュールです。



## All Together...

<!-- type=misc -->

<!--
To get the exact filename that will be loaded when `require()` is called, use
the `require.resolve()` function.
-->

`require()` が呼び出されると、正確なファイル名を得るために `require.resolve()` が使われます。

<!--
Putting together all of the above, here is the high-level algorithm
in pseudocode of what require.resolve does:
-->

上で述べたことをまとめると、 require.resolve は以下の擬似コードで記述されるようなハイレベルなアルゴリズムに則っています:

    require(X) from module at path Y
    1. If X is a core module,
       a. return the core module
       b. STOP
    2. If X begins with './' or '/' or '../'
       a. LOAD_AS_FILE(Y + X)
       b. LOAD_AS_DIRECTORY(Y + X)
    3. LOAD_NODE_MODULES(X, dirname(Y))
    4. THROW "not found"

    LOAD_AS_FILE(X)
    1. If X is a file, load X as JavaScript text.  STOP
    2. If X.js is a file, load X.js as JavaScript text.  STOP
    3. If X.node is a file, load X.node as binary addon.  STOP

    LOAD_AS_DIRECTORY(X)
    1. If X/package.json is a file,
       a. Parse X/package.json, and look for "main" field.
       b. let M = X + (json main field)
       c. LOAD_AS_FILE(M)
    2. If X/index.js is a file, load X/index.js as JavaScript text.  STOP
    3. If X/index.node is a file, load X/index.node as binary addon.  STOP

    LOAD_NODE_MODULES(X, START)
    1. let DIRS=NODE_MODULES_PATHS(START)
    2. for each DIR in DIRS:
       a. LOAD_AS_FILE(DIR/X)
       b. LOAD_AS_DIRECTORY(DIR/X)

    NODE_MODULES_PATHS(START)
    1. let PARTS = path split(START)
    2. let ROOT = index of first instance of "node_modules" in PARTS, or 0
    3. let I = count of PARTS - 1
    4. let DIRS = []
    5. while I > ROOT,
       a. if PARTS[I] = "node_modules" CONTINUE
       c. DIR = path join(PARTS[0 .. I] + "node_modules")
       b. DIRS = DIRS + DIR
       c. let I = I - 1
    6. return DIRS

## Loading from the global folders

<!-- type=misc -->

<!--
If the `NODE_PATH` environment variable is set to a colon-delimited list
of absolute paths, then node will search those paths for modules if they
are not found elsewhere.  (Note: On Windows, `NODE_PATH` is delimited by
semicolons instead of colons.)

Additionally, node will search in the following locations:
-->

`NODE_PATH` 環境変数に絶対パスをコロンで区切ったリストを設定すると、
node は他で見つからなかったモジュールをそれらのパスから探します。
(注意: Windows では、`NODE_PATH` はコロンではなくセミコロンで区切られます)

加えると、node は以下の場所から検索します。

* 1: `$HOME/.node_modules`
* 2: `$HOME/.node_libraries`
* 3: `$PREFIX/lib/node`

<!--
Where `$HOME` is the user's home directory, and `$PREFIX` is node's
configured `node_prefix`.

These are mostly for historic reasons.  You are highly encouraged to
place your dependencies locally in `node_modules` folders.  They will be
loaded faster, and more reliably.
-->

`$HOME` はユーザのホームディレクトリ、`$PREFIX` は node を
configure した時の `node_prefix` です。

これらは主に歴史的な理由によるものです。
あなたが依存するものはローカルの `node_modules` フォルダに置くことが
強く推奨されます。それは素早くロードされ、確実です。

## Accessing the main module

<!-- type=misc -->

<!--
When a file is run directly from Node, `require.main` is set to its
`module`. That means that you can determine whether a file has been run
directly by testing
-->

ファイルがNodeによって直接実行される場合、その `module` が
`require.main` に設定されます。
これは、ファイルが直接実行されたかを決定できることを意味します。

    require.main === module

<!--
For a file `foo.js`, this will be `true` if run via `node foo.js`, but
`false` if run by `require('./foo')`.

Because `module` provides a `filename` property (normally equivalent to
`__filename`), the entry point of the current application can be obtained
by checking `require.main.filename`.
-->

`foo.js` ファイルの場合、`node foo.js` と実行された場合にこれは `true` 
になりますが、`require('./foo')` で実行された場合は `false` になります。

`module` は `filename` プロパティ (通常 `__filename` と同じです) 
を提供するため、現在のアプリケーションのエントリポイントは
`require.main.filename` をチェックすることで得ることができます。

## Addenda: Package Manager Tips

<!-- type=misc -->

<!--
The semantics of Node's `require()` function were designed to be general
enough to support a number of sane directory structures. Package manager
programs such as `dpkg`, `rpm`, and `npm` will hopefully find it possible to
build native packages from Node modules without modification.
-->

Node の `require()` は普通のディレクトリ構造をサポートできるよう汎用的に設計されています。
`dpkg` や `rpm` や `npm` のようなパッケージ管理プログラムは修正なしに Node モジュールからネイティブパッケージを組み立てることができるでしょう。

<!--
Below we give a suggested directory structure that could work:

Let's say that we wanted to have the folder at
`/usr/lib/node/<some-package>/<some-version>` hold the contents of a
specific version of a package.
-->

推奨するディレクトリ構造は次のようになります:

例えば `/usr/lib/node/<some-package>/<some-version>` フォルダに、あるパッケージの特定のバージョンを保持する形式です。

<!--
Packages can depend on one another. In order to install package `foo`, you
may have to install a specific version of package `bar`.  The `bar` package
may itself have dependencies, and in some cases, these dependencies may even
collide or form cycles.
-->

パッケージは相互に依存しあうことがあります。
`foo` パッケージをインストールするためにはある特定のバージョンの `bar` パッケージをインストールする必要があります。
`bar` パッケージ自身も依存関係をもっているので、ときには依存関係が衝突したり循環したりすることがあります。

<!--
Since Node looks up the `realpath` of any modules it loads (that is,
resolves symlinks), and then looks for their dependencies in the
`node_modules` folders as described above, this situation is very simple to
resolve with the following architecture:
-->

Node はモジュールの `realpath` （シンボリックリンクを解釈します）を調べ、その依存関係を上述の `node_modules` フォルダの仕組みで探しにいきます。
これによって次のような構造をとてもシンプルに解釈することができます。

<!--
* `/usr/lib/node/foo/1.2.3/` - Contents of the `foo` package, version 1.2.3.
* `/usr/lib/node/bar/4.3.2/` - Contents of the `bar` package that `foo`
  depends on.
* `/usr/lib/node/foo/1.2.3/node_modules/bar` - Symbolic link to
  `/usr/lib/node/bar/4.3.2/`.
* `/usr/lib/node/bar/4.3.2/node_modules/*` - Symbolic links to the packages
  that `bar` depends on.
-->

* `/usr/lib/node/foo/1.2.3/` - `foo` パッケージの中身。バージョン1.2.3。
* `/usr/lib/node/bar/4.3.2/` - `bar` パッケージの中身。 `foo` が依存している。
* `/usr/lib/node/foo/1.2.3/node_modules/bar` - `/usr/lib/node/bar/4.3.2/` へのシンボリックリンク。
* `/usr/lib/node/bar/4.3.2/node_modules/*` - `bar` が依存しているパッケージへのシンボリックリンク。

<!--
Thus, even if a cycle is encountered, or if there are dependency
conflicts, every module will be able to get a version of its dependency
that it can use.

When the code in the `foo` package does `require('bar')`, it will get the
version that is symlinked into `/usr/lib/node/foo/1.2.3/node_modules/bar`.
Then, when the code in the `bar` package calls `require('quux')`, it'll get
the version that is symlinked into
`/usr/lib/node/bar/4.3.2/node_modules/quux`.

Furthermore, to make the module lookup process even more optimal, rather
than putting packages directly in `/usr/lib/node`, we could put them in
`/usr/lib/node_modules/<name>/<version>`.  Then node will not bother
looking for missing dependencies in `/usr/node_modules` or `/node_modules`.

In order to make modules available to the node REPL, it might be useful to
also add the `/usr/lib/node_modules` folder to the `$NODE_PATH` environment
variable.  Since the module lookups using `node_modules` folders are all
relative, and based on the real path of the files making the calls to
`require()`, the packages themselves can be anywhere.
-->

このようにして、もし仮に依存関係に循環や衝突が見つかったとしても、全てのモジュールは依存しているパッケージの特定のバージョンを取得することができます。

`foo` パッケージの中で `require('bar')` したら、 `/usr/lib/node/foo/1.2.3/node_modules/bar` からリンクされているバージョンを取得します。
そして、 `bar` パッケージ内で `require('quux')` を呼んだら、 `/usr/lib/node/bar/4.3.2/node_modules/quux` からリンクされているバージョンを取得します。

さらに、モジュールを探索する過程をより最適化するために、 `/usr/lib/node` にパッケージを置くよりも `/usr/lib/node_modules/<name>/<version>` に置くのをお勧めします。
そうすることで Node は見つからない依存パッケージを `/usr/node_modules` や `/node_modules` に探しにいかなくてもようなります。

Node の REPL でモジュールを使えるようにするために、 `/usr/lib/node_modules` フォルダを `$NODE_PATH` 環境変数に追加するとよいでしょう。
`node_modules` フォルダを使ったモジュール探索は全て相対的なものであり、 `require()` を呼び出したファイルの絶対パスを基準としているので、パッケージ自体はどこにでも配置することができます。
