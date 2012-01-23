## Addons

<!--

Addons are dynamically linked shared objects. They can provide glue to C and
C++ libraries. The API (at the moment) is rather complex, involving
knowledge of several libraries:

-->
アドオンは動的に共有オブジェクトをリンクします。
それらは、C や C++ のライブラリに接合点を提供します。
API はいくつかのライブラリの知識が必要で、(現時点では) かなり複雑です。

<!--

 - V8 JavaScript, a C++ library. Used for interfacing with JavaScript:
   creating objects, calling functions, etc.  Documented mostly in the
   `v8.h` header file (`deps/v8/include/v8.h` in the Node source tree).

-->
 - V8 JavaScript は C++ のライブラリです。
   JavaScript のオブジェクト作成や関数呼び出し等のインタフェースに使用されます。
   ドキュメントは主に、`v8.h` のヘッダファイル
   (Node のソースツリーの中の `deps/v8/include/v8.h`) に記されています。

<!--

 - [libuv](https://github.com/joyent/libuv), C event loop library. Anytime one
   needs to wait for a file descriptor to become readable, wait for a timer, or
   wait for a signal to received one will need to interface with libuv. That is,
   if you perform any I/O, libuv will need to be used.

-->
 - [libuv](https://github.com/joyent/libuv) は
   C のイベントループライブラリです。
   ファイル記述子が読み取り可能になるのを待つとき、タイマーを待つとき、
   シグナルを受信するのを待つときはいつでも、
   libv のインタフェースが必要になります。
   つまり、何らかの I/O 処理をすると必ず libuv を使う必要があるということです。

<!--

 - Internal Node libraries. Most importantly is the `node::ObjectWrap`
   class which you will likely want to derive from.

-->
 - Node の内部ライブラリ。
   もっとも重要なのは `node::ObjectWrap` クラスで、
   このクラスから派生させることが多くなるでしょう。

<!--

 - Others. Look in `deps/` for what else is available.

-->
 - その他。どのような物が利用できるかは `deps/` 以下を参照してさい。

<!--

Node statically compiles all its dependencies into the executable. When
compiling your module, you don't need to worry about linking to any of these
libraries.

-->
Node は全ての依存ライブラリを実行ファイルに静的にコンパイルします。
モジュールのコンパイル時には、それらのリンクについて一切気にする必要は有りません。

<!--

To get started let's make a small Addon which is the C++ equivalent of
the following Javascript code:

-->
では、 以下の JavaScript コードと同じ様に動作する小さなアドオンを
C++ で作成してみましょう。

    exports.hello = function() { return 'world'; };

<!--

To get started we create a file `hello.cc`:

-->
まず `hello.cc` というファイルを作成します:


    #include <node.h>
    #include <v8.h>

    using namespace v8;

    Handle<Value> Method(const Arguments& args) {
      HandleScope scope;
      return scope.Close(String::New("world"));
    }

    void init(Handle<Object> target) {
      NODE_SET_METHOD(target, "hello", Method);
    }
    NODE_MODULE(hello, init)

<!--

Note that all Node addons must export an initialization function:

-->
全ての Node アドオンは初期化関数をエクスポートしなければならないことに
注意してください。

    void Initialize (Handle<Object> target);
    NODE_MODULE(module_name, Initialize)

<!--
There is no semi-colon after `NODE_MODULE` as it's not a function (see `node.h`).

The `module_name` needs to match the filename of the final binary (minus the
.node suffix).

The source code needs to be built into `hello.node`, the binary Addon. To
do this we create a file called `wscript` which is python code and looks
like this:

-->
`NODE_MODULE` は関数ではないので、その後にセミコロンを付けてはいけません
(`node.h` を参照してください)。

`module_name` は最終的なバイナリのファイル名 (拡張子 .node を除く)
とマッチする必要があります。

このソースコードは、`hello.node` というバイナリアドオンとしてビルドされる必要が有ります。
そのために `wscript` と呼ばれる以下のようなコードを Python で書きました:

    srcdir = '.'
    blddir = 'build'
    VERSION = '0.0.1'

    def set_options(opt):
      opt.tool_options('compiler_cxx')

    def configure(conf):
      conf.check_tool('compiler_cxx')
      conf.check_tool('node_addon')

    def build(bld):
      obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
      obj.target = 'hello'
      obj.source = 'hello.cc'

<!--

Running `node-waf configure build` will create a file
`build/default/hello.node` which is our Addon.

-->
`node-waf configure build` を実行すると、`build/default/hello.node` が作成されます。これが作成したアドオンです。

<!--

`node-waf` is just [WAF](http://code.google.com/p/waf), the python-based build system. `node-waf` is
provided for the ease of users.

-->
`node-waf` 単に [WAF](http://code.google.com/p/waf) は Python ベースのビルドシステムです。
`node-waf` は、ユーザの負担を減らすために提供されています。

<!--

You can now use the binary addon in a Node project `hello.js` by pointing `require` to
the recently built module:

-->
ビルドされたモジュールを `require` で指定することにより、
このバイナリアドオンを Node プロジェクトの `hello.js` から利用することが
可能になります。

    var addon = require('./build/Release/hello');

    console.log(addon.hello()); // 'world'

<!--

For the moment, that is all the documentation on addons. Please see
<https://github.com/pietern/hiredis-node> for a real example.

-->
現時点では、アドオンのドキュメントはこれで全てです。
実際の例は、<https://github.com/pietern/hiredis-node> を参照してください。
