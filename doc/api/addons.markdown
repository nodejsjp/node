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

To get started let's make a small Addon which does the following except in
C++:

-->
では、 C++ で以下の様に動作する小さなアドオンを作成してみましょう。

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
      NODE_SET_METHOD(target, "method", Method);
    }
    NODE_MODULE(hello, init)

<!--

This source code needs to be built into `hello.node`, the binary Addon. To
do this we create a file called `wscript` which is python code and looks
like this:

-->
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

All Node addons must export an initialization function:

-->
全ての Node アドオンは初期化関数をエクスポートする必要が有ります:

    void Initialize (Handle<Object> target);
    NODE_MODULE(hello, Initialize)

<!--

For the moment, that is all the documentation on addons. Please see
<https://github.com/ry/node_postgres> for a real example.

-->
現時点では、アドオンのドキュメントはこれで全てです。
実際の例は、<https://github.com/ry/node_postgres> を参照してください。
