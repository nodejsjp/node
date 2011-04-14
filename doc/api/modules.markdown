## Standard Modules

<!--

Node comes with a number of modules that are compiled in to the process,
most of which are documented below.  The most common way to use these modules
is with `require('name')` and then assigning the return value to a local
variable with the same name as the module.

-->
Node は多くのコンパイル済みのモジュールを備えています。
そのうちの多くは以降のドキュメントに記述されています。
モジュールを使用するもっとも一般的な方法は、 `require('name')` を呼びその戻り値をモジュールと同名のローカル変数に代入することです。

<!--

Example:

-->
例:

    var util = require('util');

<!--

It is possible to extend node with other modules.  See `'Modules'`

-->
標準モジュール以外のモジュールを用いて Node を拡張することがでいます。 `'Modules'` をご覧ください。

## Modules

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

Variables
local to the module will be private. In this example the variable `PI` is
private to `circle.js`.

-->
`circle.js` モジュールは `area()` と `circumference()` をエクスポートしています。
オブジェクトをエクスポートするには、 `exports` という特別なオブジェクトに加えます。
モジュールのローカル変数はプライベートです。
この例の場合、変数 `PI` は `circle.js` のプライベート変数です。

### Core Modules

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

### File Modules

<!--

If the exact filename is not found, then node will attempt to load the
required filename with the added extension of `.js`, and then `.node`.

`.js` files are interpreted as JavaScript text files, and `.node` files
are interpreted as compiled addon modules loaded with `dlopen`.

A module prefixed with `'/'` is an absolute path to the file.  For
example, `require('/home/marco/foo.js')` will load the file at
`/home/marco/foo.js`.

A module prefixed with `'./'` is relative to the file calling `require()`.
That is, `circle.js` must be in the same directory as `foo.js` for
`require('./circle')` to find it.

Without a leading '/' or './' to indicate a file, the module is either a
"core module" or is loaded from a `node_modules` folder.

-->
指定された名前のファイルが見つからなかったら、 Node は指定されたファイル名に `.js` を付けたものと `.node` を付けたものを読み込もうとします。

`.js` ファイルは JavaScript ファイルとして解釈されます。
一方 `.node` ファイルはコンパイル済みのアドオンモジュールとして解釈され、 `dlopen` を使って読み込まれます。

`'/'` から始まるモジュールは、ファイルへの絶対パスと見なされます。
例えば、 `require('/home/marco/foo.js')` は `/home/macro/foo.js` を読み込みます。

`'./'` から始まるモジュールは、 `require()` を呼んだファイルからの相対パスになります。
すなわち、 `foo.js` から `require('./circle')` によって `circle.js` を読み込むには、 `circle.js` は `foo.js` と同じディレクトリに存在していなければなりません。

'/' や './' が先頭になければ、モジュールは "コアモジュール" であるかもしくは `node_modules` フォルダから読み込まれることになります。

### Loading from `node_modules` Folders

<!--

If the module identifier passed to `require()` is not a native module,
and does not begin with `'/'`, `'../'`, or `'./'`, then node starts at the
parent directory of the current module, and adds `/node_modules`, and
attempts to load the module from that location.

If it is not found there, then it moves to the parent directory, and so
on, until either the module is found, or the root of the tree is
reached.

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

#### Optimizations to the `node_modules` Lookup Process

<!--

When there are many levels of nested dependencies, it is possible for
these file trees to get fairly long.  The following optimizations are thus
made to the process.

First, `/node_modules` is never appended to a folder already ending in
`/node_modules`.

Second, if the file calling `require()` is already inside a `node_modules`
hierarchy, then the top-most `node_modules` folder is treated as the
root of the search tree.

For example, if the file at
`'/home/ry/projects/foo/node_modules/bar/node_modules/baz/quux.js'`
called `require('asdf.js')`, then node would search the following
locations:

-->
ネストした依存関係には様々な深さがあるため、ファイルツリーがかなり長くなってしまうことがあります。
それ故、次のような最適化が行われています。

まず、 `/node_modules` はフォルダ名が `node_modules` で終わっている場合には追加されません。

次に、 `require()` を呼び出しているファイルがすでに `node_modules` 配下のフォルダ内に存在していれば、一番上に存在している `node_modules` フォルダが検索ツリーのrootとして扱われます。

例えば、 `'/home/ry/projects/foo/node_modules/bar/node_modules/baz/quux.js'` にあるファイルが `require('asdf.js')` を呼んでいた場合、 Node は次の順番で探しにいきます。

* `/home/ry/projects/foo/node_modules/bar/node_modules/baz/node_modules/asdf.js`
* `/home/ry/projects/foo/node_modules/bar/node_modules/asdf.js`
* `/home/ry/projects/foo/node_modules/asdf.js`

### Folders as Modules

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

### Caching

<!--

Modules are cached after the first time they are loaded.  This means
(among other things) that every call to `require('foo')` will get
exactly the same object returned, if it would resolve to the same file.

-->
モジュールは初めて読み込まれたときにキャッシュされます。
すなわち（他のキャッシュと同様に） `require('foo')` を呼ぶたびに、もし引数の意味するものが同一のファイルであったなら全く同一のオブジェクトが返されます。

### module.exports

<!--

The `exports` object is created by the Module system. Sometimes this is not
acceptable, many want their module to be an instance of some class. To do this
assign the desired export object to `module.exports`. For example suppose we
were making a module called `a.js`

-->
`exports` オブジェクトはモジュールシステムによって作成されます。
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

y.js

    var x = require('./x');
    console.log(x.a);


### All Together...

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

    require(X)
    1. If X is a core module,
       a. return the core module
       b. STOP
    2. If X begins with `./` or `/`,
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
    2. LOAD_AS_FILE(X/index)

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
    6. return DIRS

### Loading from the `require.paths` Folders

<!--

In node, `require.paths` is an array of strings that represent paths to
be searched for modules when they are not prefixed with `'/'`, `'./'`, or
`'../'`.  For example, if require.paths were set to:

-->
`require.paths` は、指定されたモジュール名が `'/'` や `'./'` や `'../'` から始まっていないときにモジュールを探しにいくパスを文字列として保持している配列です。
例えば、require.pathsが次のように設定されているとします: 

    [ '/home/micheil/.node_modules',
      '/usr/local/lib/node_modules' ]

<!--

Then calling `require('bar/baz.js')` would search the following
locations:

-->
そして、 `require('baz/baz.js')` を呼ぶと次の場所を探しにいきます:

* 1: `'/home/micheil/.node_modules/bar/baz.js'`
* 2: `'/usr/local/lib/node_modules/bar/baz.js'`

<!--

The `require.paths` array can be mutated at run time to alter this
behavior.

It is set initially from the `NODE_PATH` environment variable, which is
a colon-delimited list of absolute paths.  In the previous example,
the `NODE_PATH` environment variable might have been set to:

-->
これらの動作を修正することができるよう、 `require.paths` 配列は実行時に変更することができます。

`require.paths` は `NODE_PATH` という環境変数の値によって初期化されます。
`NODE_PATH` にはコロンで区切った複数のパスを記述することができます。
上の例では、 `NODE_PATH` には次のような値がセットされていたものと考えられます:

    /home/micheil/.node_modules:/usr/local/lib/node_modules

<!--

Loading from the `require.paths` locations is only performed if the
module could not be found using the `node_modules` algorithm above.
Global modules are lower priority than bundled dependencies.

-->
`require.paths` からロードされるのは、モジュールが前述の `node_modules` アルゴリズムで見つけられなかった場合だけです。
グローバルモジュールはバンドルされた依存性よりも低プライオリティです。

#### **Note:** Please Avoid Modifying `require.paths`

<!--

For compatibility reasons, `require.paths` is still given first priority
in the module lookup process.  However, it may disappear in a future
release.

While it seemed like a good idea at the time, and enabled a lot of
useful experimentation, in practice a mutable `require.paths` list is
often a troublesome source of confusion and headaches.

-->
後方互換性を保つために `require.paths` は今のところモジュールを探す過程において最も優先されます。
しかし、将来のバージョンでは `require.paths` は無くなる予定です。

実装当時はよいアイデアだと思われ実験的に使う分にはとても有用でしたが、実際に使いだしてみると変更可能な `require.paths` のリストというものはやっかいな混乱と頭痛の種になることがしばしばあったのです。

##### Setting `require.paths` to some other value does nothing.

<!--

This does not do what one might expect:

-->
次のコードは、希望通りには動きません:

    require.paths = [ '/usr/lib/node' ];

<!--

All that does is lose the reference to the *actual* node module lookup
paths, and create a new reference to some other thing that isn't used
for anything.

-->
これは、 *実際の* Node モジュールへの参照パスを消滅させ、使われることのないパスに対する新たな参照を作り出しています。

##### Putting relative paths in `require.paths` is... weird.

<!--

If you do this:

-->
次のようにするとします:

    require.paths.push('./lib');

<!--

then it does *not* add the full resolved path to where `./lib`
is on the filesystem.  Instead, it literally adds `'./lib'`,
meaning that if you do `require('y.js')` in `/a/b/x.js`, then it'll look
in `/a/b/lib/y.js`.  If you then did `require('y.js')` in
`/l/m/n/o/p.js`, then it'd look in `/l/m/n/o/lib/y.js`.

-->
これは、ファイルシステム上の `./lib` のある場所への絶対パスを追加する *わけではありません* 。
代わりに、文字通り `'./lib'` を追加します。
すなわち、もし `/a/b/x.js` ファイル内で `require('y.js')` した場合、 `/a/b/lib/y.js` を探しにいきます。
同様に `/l/m/n/o/p.js` ファイル内で `require('y.js')` した場合は、 `/l/m/n/o/lib/y.js` を探しにいきます。

<!--

In practice, people have used this as an ad hoc way to bundle
dependencies, but this technique is brittle.

-->
実際に、これらはアドホックな依存性をバンドルするために用いられています。
しかしこれらのテクニックはとても不安定です。

##### Zero Isolation

<!--

There is (by regrettable design), only one `require.paths` array used by
all modules.

As a result, if one node program comes to rely on this behavior, it may
permanently and subtly alter the behavior of all other node programs in
the same process.  As the application stack grows, we tend to assemble
functionality, and it is a problem with those parts interact in ways
that are difficult to predict.

-->
（残念なことに）たった一つの `require.paths` 配列が全てのモジュールによって使われるという設計になっています。

その結果、もしある Node プログラムが上記の挙動を行っていたら、同じプロセス上にいる他の全ての Node プログラムの動作も永遠にそして微妙に変化させてしまいます。
アプリケーションが成長するにつれ、私たちは機能をまとめていきます。
そしてその中に上記のような他へ影響を及ぼす箇所があると、それはとても予測するのが難しい問題となってしまいます。

## Addenda: Package Manager Tips

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

