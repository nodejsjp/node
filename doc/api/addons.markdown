# Addons

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
   `v8.h` header file (`deps/v8/include/v8.h` in the Node source
   tree), which is also available
   [online](http://izs.me/v8-docs/main.html).
-->

 - V8 JavaScript は C++ のライブラリです。
   JavaScript のオブジェクト作成や関数呼び出し等のインタフェースに使用されます。
   ドキュメントは主に、`v8.h` のヘッダファイル
   (Node のソースツリーの中の `deps/v8/include/v8.h`) に記されていますが、
   [オンライン](http://izs.me/v8-docs/main.html) で参照することもできます。

<!--
 - [libuv](https://github.com/joyent/libuv), C event loop library.
   Anytime one needs to wait for a file descriptor to become readable,
   wait for a timer, or wait for a signal to be received one will need
   to interface with libuv. That is, if you perform any I/O, libuv will
   need to be used.
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
Node statically compiles all its dependencies into the executable.
When compiling your module, you don't need to worry about linking to
any of these libraries.
-->

Node は全ての依存ライブラリを実行ファイルに静的にコンパイルします。
モジュールのコンパイル時には、それらのリンクについて一切気にする必要は有りません。

<!--
All of the following examples are available for
[download](https://github.com/rvagg/node-addon-examples) and may be
used as a starting-point for your own Addon.
-->

以下のサンプルの全ては
[ダウンロード](https://github.com/rvagg/node-addon-examples)
から利用することができ、独自のアドオンの出発点になるでしょう。

## Hello world

<!--
To get started let's make a small Addon which is the C++ equivalent of
the following JavaScript code:
-->

では、 以下の JavaScript コードと同じ様に動作する小さなアドオンを
C++ で作成してみましょう。

    module.exports.hello = function() { return 'world'; };

<!--
First we create a file `hello.cc`:
-->

最初に `hello.cc` というファイルを作成します:


    #include <node.h>

    using namespace v8;

    Handle<Value> Method(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);
      return scope.Close(String::New("world"));
    }

    void init(Handle<Object> exports) {
      exports->Set(String::NewSymbol("hello"),
          FunctionTemplate::New(Method)->GetFunction());
    }

    NODE_MODULE(hello, init)

<!--
Note that all Node addons must export an initialization function:
-->

全ての Node アドオンは初期化関数をエクスポートしなければならないことに
注意してください。

    void Initialize (Handle<Object> exports);
    NODE_MODULE(module_name, Initialize)

<!--
There is no semi-colon after `NODE_MODULE` as it's not a function (see
`node.h`).

The `module_name` needs to match the filename of the final binary (minus the
.node suffix).

The source code needs to be built into `hello.node`, the binary Addon. To
do this we create a file called `binding.gyp` which describes the configuration
to build your module in a JSON-like format. This file gets compiled by
[node-gyp](https://github.com/TooTallNate/node-gyp).
-->

`NODE_MODULE` は関数ではないので、その後にセミコロンを付けてはいけません
(`node.h` を参照してください)。

`module_name` は最終的なバイナリのファイル名 (拡張子 .node を除く)
とマッチする必要があります。

このソースコードは、`hello.node` というバイナリアドオンとしてビルドされる必要が有ります。
そのために `binding.gyp` と呼ばれる、あなたのモジュールをビルドするための
構成を JSON 的なフォーマットで記述したファイルを作成します。
このファイルは [node-gyp](https://github.com/TooTallNate/node-gyp)
によってコンパイルされます。

    {
      "targets": [
        {
          "target_name": "hello",
          "sources": [ "hello.cc" ]
        }
      ]
    }

<!--
The next step is to generate the appropriate project build files for the
current platform. Use `node-gyp configure` for that.
-->

次のステップは現在のプラットフォームに適したプロジェクトビルドファイルを
生成することです。
`node-gyp configure` を使います。

<!--
Now you will have either a `Makefile` (on Unix platforms) or a `vcxproj` file
(on Windows) in the `build/` directory. Next invoke the `node-gyp build`
command.
-->

これで、`Makefile` (Unix プラットフォームの場合)、または `vcxproj` ファイル
(Windows の場合) が `build/` ディレクトリに作られます。
次に `node-gyp build` コマンドを起動します。

<!--
Now you have your compiled `.node` bindings file! The compiled bindings end up
in `build/Release/`.
-->

これでコンパイルされた `.node` バインディングファイルが作成されます！
コンパイルされたバインディングファイルは `build/Release/` にあります。

<!--
You can now use the binary addon in a Node project `hello.js` by pointing
`require` to the recently built `hello.node` module:
-->

ビルドされた `hello.node` モジュールを `require` で指定することにより、
このバイナリアドオンを Node プロジェクトの `hello.js` から利用することが
可能になります。

    var addon = require('./build/Release/hello');

    console.log(addon.hello()); // 'world'

<!--
Please see patterns below for further information or
<https://github.com/arturadib/node-qt> for an example in production.
-->

さらに詳しい情報については下記のパターンか、
<https://github.com/arturadib/node-qt> を実際のプロダクトにおける
例として参照してください。

## Addon patterns

<!--
Below are some addon patterns to help you get started. Consult the online
[v8 reference](http://izs.me/v8-docs/main.html) for help with the various v8
calls, and v8's [Embedder's Guide](http://code.google.com/apis/v8/embed.html)
for an explanation of several concepts used such as handles, scopes,
function templates, etc.
-->

以下は初心者に役立つアドオンのパターンです。
v8 の様々な API についてはオンラインの
[v8 reference](http://izs.me/v8-docs/main.html) が、
そして ハンドルやスコープ、関数テンプレートなどいくつかの概念については
v8 の [Embedder's Guide](http://code.google.com/apis/v8/embed.html) が
役に立つでしょう。

<!--
In order to use these examples you need to compile them using `node-gyp`.
Create the following `binding.gyp` file:
-->

このサンプルを利用できるようにするには、`node-gyp` を使用して
コンパイルする必要があります。
以下の `binding.gyp` ファイルを作成します。

    {
      "targets": [
        {
          "target_name": "addon",
          "sources": [ "addon.cc" ]
        }
      ]
    }

<!--
In cases where there is more than one `.cc` file, simply add the file name to
the `sources` array, e.g.:
-->

一つ以上の `.cc` ファイルがある場合は、単純に `sources` 配列にファイル名を
加えるだけです。例:

    "sources": ["addon.cc", "myexample.cc"]

<!--
Now that you have your `binding.gyp` ready, you can configure and build the
addon:
-->

これで `binding.gyp` の準備ができました。
アドオンをコンフィギュアおよびビルドするには:

    $ node-gyp configure build


### Function arguments

<!--
The following pattern illustrates how to read arguments from JavaScript
function calls and return a result. This is the main and only needed source
`addon.cc`:
-->

以下のパターンは JavaScript から呼び出された関数で引数を読み出したり、
結果を返す方法を示します。これは `addon.cc` でのみ必要となります。

    #include <node.h>

    using namespace v8;

    Handle<Value> Add(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      if (args.Length() < 2) {
        ThrowException(Exception::TypeError(
            String::New("Wrong number of arguments")));
        return scope.Close(Undefined(isolate));
      }

      if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined(isolate));
      }

      Local<Number> num = Number::New(args[0]->NumberValue() +
          args[1]->NumberValue());
      return scope.Close(num);
    }

    void Init(Handle<Object> exports) {
      exports->Set(String::NewSymbol("add"),
          FunctionTemplate::New(Add)->GetFunction());
    }

    NODE_MODULE(addon, Init)

<!--
You can test it with the following JavaScript snippet:
-->

以下の JavaScript コード片でテストすることができます。

    var addon = require('./build/Release/addon');

    console.log( 'This should be eight:', addon.add(3,5) );


### Callbacks

<!--
You can pass JavaScript functions to a C++ function and execute them from
there. Here's `addon.cc`:
-->

JavaScript の関数を C++ の関数に渡してそこから呼び出すことができます。
これは `addon.cc` です:

    #include <node.h>

    using namespace v8;

    Handle<Value> RunCallback(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      Local<Function> cb = Local<Function>::Cast(args[0]);
      const unsigned argc = 1;
      Local<Value> argv[argc] = { String::New("hello world") };
      cb->Call(Context::GetCurrent()->Global(), argc, argv);

      return scope.Close(Undefined(isolate));
    }

    void Init(Handle<Object> exports, Handle<Object> module) {
      module->Set(String::NewSymbol("exports"),
          FunctionTemplate::New(RunCallback)->GetFunction());
    }

    NODE_MODULE(addon, Init)

<!--
Note that this example uses a two-argument form of `Init()` that receives
the full `module` object as the second argument. This allows the addon
to completely overwrite `exports` with a single function instead of
adding the function as a property of `exports`.
-->

この例は二つの引数を取る形式の `Init()` を使用して、第2引数で完全な `module`
オブジェクトを受け取っていることに注意してください。
これは、`exports` のプロパティとして関数を加える代わりに、アドオンが
一つの関数で `exports` を完全に上書きすることを可能にします。

<!--
To test it run the following JavaScript snippet:
-->

以下の JavaScript コード片でテストすることができます。

    var addon = require('./build/Release/addon');

    addon(function(msg){
      console.log(msg); // 'hello world'
    });


### Object factory

<!--
You can create and return new objects from within a C++ function with this
`addon.cc` pattern, which returns an object with property `msg` that echoes
the string passed to `createObject()`:
-->

C++ 関数の中から新しいオブジェクトを作成して返すことができます。
以下の `addon.cc` のパターンでは、`createObject()` に渡された文字列を
反映する `msg` プロパティを持ったオブジェクトを返します。

    #include <node.h>

    using namespace v8;

    Handle<Value> CreateObject(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      Local<Object> obj = Object::New();
      obj->Set(String::NewSymbol("msg"), args[0]->ToString());

      return scope.Close(obj);
    }

    void Init(Handle<Object> exports, Handle<Object> module) {
      module->Set(String::NewSymbol("exports"),
          FunctionTemplate::New(CreateObject)->GetFunction());
    }

    NODE_MODULE(addon, Init)

<!--
To test it in JavaScript:
-->

テスト用の JavaScript:

    var addon = require('./build/Release/addon');

    var obj1 = addon('hello');
    var obj2 = addon('world');
    console.log(obj1.msg+' '+obj2.msg); // 'hello world'


### Function factory

<!--
This pattern illustrates how to create and return a JavaScript function that
wraps a C++ function:
-->

このパターンは C++ 関数をラップした JavaScript 関数を作成して返す方法を
示します。

    #include <node.h>

    using namespace v8;

    Handle<Value> MyFunction(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);
      return scope.Close(String::New("hello world"));
    }

    Handle<Value> CreateFunction(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      Local<FunctionTemplate> tpl = FunctionTemplate::New(MyFunction);
      Local<Function> fn = tpl->GetFunction();

      // omit this to make it anonymous
      fn->SetName(String::NewSymbol("theFunction"));

      return scope.Close(fn);
    }

    void Init(Handle<Object> exports, Handle<Object> module) {
      module->Set(String::NewSymbol("exports"),
          FunctionTemplate::New(CreateFunction)->GetFunction());
    }

    NODE_MODULE(addon, Init)

<!--
To test:
-->

テスト:

    var addon = require('./build/Release/addon');

    var fn = addon();
    console.log(fn()); // 'hello world'


### Wrapping C++ objects

<!--
Here we will create a wrapper for a C++ object/class `MyObject` that can be
instantiated in JavaScript through the `new` operator. First prepare the main
module `addon.cc`:
-->

ここでは、
C++ オブジェクト／クラスをラップし、JavaScript から new 演算子を使って
インスタンス化できる `MyObject` を作成します。
最初にメインモジュール `addon.cc` を準備します:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    void InitAll(Handle<Object> exports) {
      MyObject::Init(exports);
    }

    NODE_MODULE(addon, InitAll)

<!--
Then in `myobject.h` make your wrapper inherit from `node::ObjectWrap`:
-->

次に、`node::ObjectWrap` を継承したラッパーを `myobject.h` に作成します。

    #ifndef MYOBJECT_H
    #define MYOBJECT_H

    #include <node.h>

    class MyObject : public node::ObjectWrap {
     public:
      static void Init(v8::Handle<v8::Object> exports);

     private:
      MyObject();
      ~MyObject();

      static v8::Handle<v8::Value> New(const v8::Arguments& args);
      static v8::Handle<v8::Value> PlusOne(const v8::Arguments& args);
      double counter_;
    };

    #endif

<!--
And in `myobject.cc` implement the various methods that you want to expose.
Here we expose the method `plusOne` by adding it to the constructor's
prototype:
-->

公開したい様々なメソッドを `myobject.cc` に実装します。
ここでは、コンストラクタに渡された値に加算する `plusOne` を公開しています:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    MyObject::MyObject() {};
    MyObject::~MyObject() {};

    void MyObject::Init(Handle<Object> exports) {
      Isolate* isolate = Isolate::GetCurrent();

      // Prepare constructor template
      Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
      tpl->SetClassName(String::NewSymbol("MyObject"));
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      // Prototype
      tpl->PrototypeTemplate()->Set(String::NewSymbol("plusOne"),
          FunctionTemplate::New(PlusOne)->GetFunction());

      Persistent<Function> constructor
          = Persistent<Function>::New(isolate, tpl->GetFunction());

      exports->Set(String::NewSymbol("MyObject"), constructor);
    }

    Handle<Value> MyObject::New(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj = new MyObject();
      obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
      obj->Wrap(args.This());

      return args.This();
    }

    Handle<Value> MyObject::PlusOne(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
      obj->counter_ += 1;

      return scope.Close(Number::New(obj->counter_));
    }

<!--
Test it with:
-->

これでテストします:

    var addon = require('./build/Release/addon');

    var obj = new addon.MyObject(10);
    console.log( obj.plusOne() ); // 11
    console.log( obj.plusOne() ); // 12
    console.log( obj.plusOne() ); // 13

### Factory of wrapped objects

<!--
This is useful when you want to be able to create native objects without
explicitly instantiating them with the `new` operator in JavaScript, e.g.
-->

JavaScript の `new` 演算子で明示的にインスタンス化することなく、
ネイティブオブジェクトを作成できるようにしたい場合に便利です。例:

    var obj = addon.createObject();
    // instead of:
    // var obj = new addon.Object();

<!--
Let's register our `createObject` method in `addon.cc`:
-->

createObject` を `addon.cc` に登録しましょう:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    Handle<Value> CreateObject(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);
      return scope.Close(MyObject::NewInstance(args));
    }

    void InitAll(Handle<Object> exports, Handle<Object> module) {
      MyObject::Init();

      module->Set(String::NewSymbol("exports"),
          FunctionTemplate::New(CreateObject)->GetFunction());
    }

    NODE_MODULE(addon, InitAll)

<!--
In `myobject.h` we now introduce the static method `NewInstance` that takes
care of instantiating the object (i.e. it does the job of `new` in JavaScript):
-->

`myobject.h` にオブジェクトを生成する static メソッド `NewInstance` を
導入しましょう (すなわち，それが JavaScript 内の `new` の働きをします)。

    #ifndef MYOBJECT_H
    #define MYOBJECT_H

    #include <node.h>

    class MyObject : public node::ObjectWrap {
     public:
      static void Init();
      static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

     private:
      MyObject();
      ~MyObject();

      static v8::Persistent<v8::Function> constructor;
      static v8::Handle<v8::Value> New(const v8::Arguments& args);
      static v8::Handle<v8::Value> PlusOne(const v8::Arguments& args);
      double counter_;
    };

    #endif

<!--
The implementation is similar to the above in `myobject.cc`:
-->

実装は前述の `myobject.cc` と同様です:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    MyObject::MyObject() {};
    MyObject::~MyObject() {};

    Persistent<Function> MyObject::constructor;

    void MyObject::Init() {
      Isolate* isolate = Isolate::GetCurrent();
      // Prepare constructor template
      Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
      tpl->SetClassName(String::NewSymbol("MyObject"));
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      // Prototype
      tpl->PrototypeTemplate()->Set(String::NewSymbol("plusOne"),
          FunctionTemplate::New(PlusOne)->GetFunction());

      constructor = Persistent<Function>::New(isolate, tpl->GetFunction());
    }

    Handle<Value> MyObject::New(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj = new MyObject();
      obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
      obj->Wrap(args.This());

      return args.This();
    }

    Handle<Value> MyObject::NewInstance(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      const unsigned argc = 1;
      Handle<Value> argv[argc] = { args[0] };
      Local<Object> instance = constructor->NewInstance(argc, argv);

      return scope.Close(instance);
    }

    Handle<Value> MyObject::PlusOne(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
      obj->counter_ += 1;

      return scope.Close(Number::New(obj->counter_));
    }

<!--
Test it with:
-->

これでテストします:

    var createObject = require('./build/Release/addon');

    var obj = createObject(10);
    console.log( obj.plusOne() ); // 11
    console.log( obj.plusOne() ); // 12
    console.log( obj.plusOne() ); // 13

    var obj2 = createObject(20);
    console.log( obj2.plusOne() ); // 21
    console.log( obj2.plusOne() ); // 22
    console.log( obj2.plusOne() ); // 23


### Passing wrapped objects around

<!--
In addition to wrapping and returning C++ objects, you can pass them around
by unwrapping them with Node's `node::ObjectWrap::Unwrap` helper function.
In the following `addon.cc` we introduce a function `add()` that can take on two
`MyObject` objects:
-->

C++ オブジェクトをラップして返すことに加えて、Node が提供するヘルパ関数
`node::ObjectWrap::Unwrap` を使用してアンラップすることもできます。
以下の `addon.cc` では、二つの `MyObject` オブジェクトを受け取る `add()`
関数を導入します:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    Handle<Value> CreateObject(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);
      return scope.Close(MyObject::NewInstance(args));
    }

    Handle<Value> Add(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj1 = node::ObjectWrap::Unwrap<MyObject>(
          args[0]->ToObject());
      MyObject* obj2 = node::ObjectWrap::Unwrap<MyObject>(
          args[1]->ToObject());

      double sum = obj1->Val() + obj2->Val();
      return scope.Close(Number::New(sum));
    }

    void InitAll(Handle<Object> exports) {
      MyObject::Init();

      exports->Set(String::NewSymbol("createObject"),
          FunctionTemplate::New(CreateObject)->GetFunction());

      exports->Set(String::NewSymbol("add"),
          FunctionTemplate::New(Add)->GetFunction());
    }

    NODE_MODULE(addon, InitAll)

<!--
To make things interesting we introduce a public method in `myobject.h` so we
can probe private values after unwrapping the object:
-->

よりおもしろくするために、`myobject.h` にパブリックメソッドを導入しましょう。
したがって、アンラップされたオブジェクトのプライベート変数を調べることが
できます。

    #ifndef MYOBJECT_H
    #define MYOBJECT_H

    #include <node.h>

    class MyObject : public node::ObjectWrap {
     public:
      static void Init();
      static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
      double Val() const { return val_; }

     private:
      MyObject();
      ~MyObject();

      static v8::Persistent<v8::Function> constructor;
      static v8::Handle<v8::Value> New(const v8::Arguments& args);
      double val_;
    };

    #endif

<!--
The implementation of `myobject.cc` is similar as before:
-->

`myobject.cc` の実装はこれまでと同様です:

    #include <node.h>
    #include "myobject.h"

    using namespace v8;

    MyObject::MyObject() {};
    MyObject::~MyObject() {};

    Persistent<Function> MyObject::constructor;

    void MyObject::Init() {
      Isolate* isolate = Isolate::GetCurrent();

      // Prepare constructor template
      Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
      tpl->SetClassName(String::NewSymbol("MyObject"));
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      constructor = Persistent<Function>::New(isolate, tpl->GetFunction());
    }

    Handle<Value> MyObject::New(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      MyObject* obj = new MyObject();
      obj->val_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
      obj->Wrap(args.This());

      return args.This();
    }

    Handle<Value> MyObject::NewInstance(const Arguments& args) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope scope(isolate);

      const unsigned argc = 1;
      Handle<Value> argv[argc] = { args[0] };
      Local<Object> instance = constructor->NewInstance(argc, argv);

      return scope.Close(instance);
    }

<!--
Test it with:
-->

これでテストします:

    var addon = require('./build/Release/addon');

    var obj1 = addon.createObject(10);
    var obj2 = addon.createObject(20);
    var result = addon.add(obj1, obj2);

    console.log(result); // 30
