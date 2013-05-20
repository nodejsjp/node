# Executing JavaScript

    Stability: 2 - Unstable. See Caveats, below.

<!--name=vm-->

<!--
You can access this module with:
-->

次のようにすることで、このモジュールにアクセスすることができます:

    var vm = require('vm');

<!--
JavaScript code can be compiled and run immediately or compiled, saved, and run later.
-->

JavaScript コードは、コンパイルされてすぐに実行されるか、コンパイルおよび保存されて後から実行されます。

## Caveats

<!--
The `vm` module has many known issues and edge cases. If you run into
issues or unexpected behavior, please consult
[the open issues on GitHub](https://github.com/joyent/node/issues/search?q=vm).
Some of the biggest problems are described below.
-->

`vm` モジュールには既知の課題や特殊ケースがたくさんあります。
もし問題や期待と異なる振る舞いを見つけた場合は、
[the open issues on GitHub](https://github.com/joyent/node/issues/search?q=vm)
に相談してください。
大きな問題のいくつかは以下に示されます。

### Sandboxes

<!--
The `sandbox` argument to `vm.runInNewContext` and `vm.createContext`,
along with the `initSandbox` argument to `vm.createContext`, do not
behave as one might normally expect and their behavior varies
between different versions of Node.
-->

`vm.runInNewContext` と、`vm.createContext` に `initSandbox` と共に渡される
`sandbox` 引数は、通常期待される振る舞いをせず、それは Node のバージョン間で
異なった振る舞いをします。

<!--
The key issue to be aware of is that V8 provides no way to directly
control the global object used within a context. As a result, while
properties of your `sandbox` object will be available in the context,
any properties from the `prototype`s of the `sandbox` may not be
available. Furthermore, the `this` expression within the global scope
of the context evaluates to the empty object (`{}`) instead of to
your sandbox.
-->

知られている重要問題は、コンテキスト内で使用されるグローバルオブジェクトを
直接制御する方法を V8 が提供しないことです。
その結果、`sandbox` オブジェクトのプロパティがコンテキストから
利用可能な間、`sandbox` のプロトタイプのプロパティを利用することが
できないかもしれません。
さらには、コンテキスト内のグローバルスコープにおける `this` が
サンドボックスではなく、空のオブジェクト (`{}`) に評価されます。

<!--
Your sandbox's properties are also not shared directly with the script.
Instead, the properties of the sandbox are copied into the context at
the beginning of execution, and then after execution, the properties
are copied back out in an attempt to propagate any changes.
-->

サンドボックスのプロパティはまた、スクリプトに直接共有されません。
代わりに、サンドボックスのプロパティは実行前にコンテキストにコピーされ、
実行後、変更を伝播するためにプロパティはサンドボックスにコピーされます。

### Globals

<!--
Properties of the global object, like `Array` and `String`, have
different values inside of a context. This means that common
expressions like `[] instanceof Array` or
`Object.getPrototypeOf([]) === Array.prototype` may not produce
expected results when used inside of scripts evaluated via the `vm` module.
-->

`Array` や `String` などのグローバルオブジェクトのプロパティは、
コンテキストの中では異なる値を持ちます。
これは  `[] instanceof Array` や
`Object.getPrototypeOf([]) === Array.prototype` などのよくある式は、
`vm` モジュールによって評価されるスクリプトの中で使われると、
期待した結果にならないことを意味します。

<!--
Some of these problems have known workarounds listed in the issues for
`vm` on GitHub. for example, `Array.isArray` works around
the example problem with `Array`.
-->

GitHub 上の `vm` に関する課題には、これらの問題に対する回避策があります。
例えば、`Array.isArray` は `Array` に関する問題の例でもうまく動きます。

## vm.runInThisContext(code, [filename])

<!--
`vm.runInThisContext()` compiles `code`, runs it and returns the result. Running
code does not have access to local scope. `filename` is optional, it's used only
in stack traces.
-->

`vm.runInThisContext()` は `code` をコンパイルして実行し、結果を返します。
実行されるコードはローカルスコープにアクセスしません。
`filename` はオプションで、スタックトレースでのみ使用されます。

<!--
Example of using `vm.runInThisContext` and `eval` to run the same code:
-->

`vm.runInThisContext` と `eval` で同じコードを実行する例:

    var localVar = 123,
        usingscript, evaled,
        vm = require('vm');

    usingscript = vm.runInThisContext('localVar = 1;',
      'myfile.vm');
    console.log('localVar: ' + localVar + ', usingscript: ' +
      usingscript);
    evaled = eval('localVar = 1;');
    console.log('localVar: ' + localVar + ', evaled: ' +
      evaled);

    // localVar: 123, usingscript: 1
    // localVar: 1, evaled: 1

<!--
`vm.runInThisContext` does not have access to the local scope, so `localVar` is unchanged.
`eval` does have access to the local scope, so `localVar` is changed.
-->

`vm.runInThisContext` はローカルスコープにアクセスしないので、`localVar` は変更されません。
`eval` はローカルスコープにアクセスするので、`localVar` は変更されます。

<!--
In case of syntax error in `code`, `vm.runInThisContext` emits the syntax error to stderr
and throws an exception.
-->

`code` が文法エラーとなるケースでは、`vm.runInThisContext` は標準エラーに文法エラーを出力し、
例外をスローします。


## vm.runInNewContext(code, [sandbox], [filename], [timeout])

<!--
`vm.runInNewContext` compiles `code`, then runs it in `sandbox` and returns the
result. Running code does not have access to local scope. The object `sandbox`
will be used as the global object for `code`.
`sandbox` and `filename` are optional, `filename` is only used in stack traces.
`timeout` specifies an optional number of milliseconds to execute `code` before
terminating execution. If execution is terminated, `null` will be thrown.
-->

`vm.runInNewContext` は `code` をコンパイルし、それを `sandbox` の中で実行して
結果を返します。
実行されるコードはローカルスコープにアクセスしません。
`sandbox` が `code` にとってのグローバルオブジェクトとして使われます。
`sandbox` および `filename` はオプションで、`filename` 
スタックトレースでのみ使用されます。
`timeout` はオプションで、`code` の実行が打ち切られるまでの時間を
ミリ秒単位の数値で指定します。
実行が打ち切られると、`null` がスローされます。

<!--
Example: compile and execute code that increments a global variable and sets a new one.
These globals are contained in the sandbox.
-->

例: グローバル変数をインクリメントして新しい値をセットするコードをコンパイルおよび実行します。
これらのグローバル変数はサンドボックスに含まれます。

    var util = require('util'),
        vm = require('vm'),
        sandbox = {
          animal: 'cat',
          count: 2
        };

    vm.runInNewContext('count += 1; name = "kitty"', sandbox, 'myfile.vm');
    console.log(util.inspect(sandbox));

    // { animal: 'cat', count: 3, name: 'kitty' }

<!--
Note that running untrusted code is a tricky business requiring great care.  To prevent accidental
global variable leakage, `vm.runInNewContext` is quite useful, but safely running untrusted code
requires a separate process.
-->

慎重を要するビジネスでは、信頼できないコードの実行は細心の注意が求められることに注意してください。
偶然グローバル変数が漏れてしまうことを防ぐために、`vm.runInNewContext` はとても役立ちますが、
信頼できないコードを安全に実行するために別のプロセスを要求します。

<!--
In case of syntax error in `code`, `vm.runInNewContext` emits the syntax error to stderr
and throws an exception.
-->

`code` が文法エラーとなるケースでは、
`vm.runInNewContext` は標準エラーに文法エラーを出力し、例外をスローします。

## vm.runInContext(code, context, [filename], [timeout])

<!--
`vm.runInContext` compiles `code`, then runs it in `context` and returns the
result. A (V8) context comprises a global object, together with a set of
built-in objects and functions. Running code does not have access to local scope
and the global object held within `context` will be used as the global object
for `code`.
`filename` is optional, it's used only in stack traces.
`timeout` specifies an optional number of milliseconds to execute `code` before
terminating execution. If execution is terminated, `null` will be thrown.
-->

`vm.runInContext` は `code` をコンパイルして、
`context` をコンテキストとして実行し、その結果を返します。
(V8 の) コンテキストは組み込みのオブジェクトと関数と共に、
グローバルオブジェクトを含みます。
実行されるコードはローカルスコープにアクセスせず、
`context` が `code` にとってのグローバルオブジェクトとして使われます。
`filename` はオプションで、スタックトレースでのみ使用されます。
`timeout` はオプションで、`code` の実行が打ち切られるまでの時間を
ミリ秒単位の数値で指定します。
実行が打ち切られると、`null` がスローされます。

<!--
Example: compile and execute code in a existing context.
-->

例: コードをコンパイルして既存のコンテキストで実行します。

    var util = require('util'),
        vm = require('vm'),
        initSandbox = {
          animal: 'cat',
          count: 2
        },
        context = vm.createContext(initSandbox);

    vm.runInContext('count += 1; name = "CATT"', context, 'myfile.vm');
    console.log(util.inspect(context));

    // { animal: 'cat', count: 3, name: 'CATT' }

<!--
Note that `createContext` will perform a shallow clone of the supplied sandbox object in order to
initialize the global object of the freshly constructed context.

Note that running untrusted code is a tricky business requiring great care.  To prevent accidental
global variable leakage, `vm.runInContext` is quite useful, but safely running untrusted code
requires a separate process.

In case of syntax error in `code`, `vm.runInContext` emits the syntax error to stderr
and throws an exception.
-->

`createContext` は、新たに構築されたコンテキストのグローバルオブジェクトを
初期化するために、与えられた `context` オブジェクトの浅いクローンを
作成することに注意してください。

慎重を要するビジネスでは、信頼できないコードの実行は細心の注意が求められることに注意してください。
偶然グローバル変数が漏れてしまうことを防ぐために、`vm.runInContext` はとても役立ちますが、
信頼できないコードを安全に実行するために別のプロセスを要求します。


## vm.createContext([initSandbox])

<!--
`vm.createContext` creates a new context which is suitable for use as the 2nd argument of a subsequent
call to `vm.runInContext`. A (V8) context comprises a global object together with a set of
build-in objects and functions. The optional argument `initSandbox` will be shallow-copied
to seed the initial contents of the global object used by the context.
-->

`vm.createContext` は、続けて呼び出される `vm.runInContext` の第 2 引数として
使用するのに適した新しいコンテキストを作成します。
(V8 の) コンテキストは組み込みのオブジェクトと関数と共に、
グローバルオブジェクトを含みます。
オプションの引数 `initSandbox` は、このコンテキストで使用される
グローバルオブジェクトの初期値としてシャローコピーされます。

## vm.createScript(code, [filename])

<!--
`createScript` compiles `code` but does not run it. Instead, it returns a
`vm.Script` object representing this compiled code. This script can be run
later many times using methods below. The returned script is not bound to any
global object. It is bound before each run, just for that run. `filename` is
optional, it's only used in stack traces.
-->

`createScript` は `code` をコンパイルしますが、実行はしません。
代わりに、コンパイルされたコードを表現する `vm.Script` オブジェクトを返します。
このスクリプトは後述のメソッドを使って後から何度でも実行することができます。
返されるスクリプトはどのグローバルオブジェクトとも結びつけられていません。
それぞれの実行前に結びつけることで、そのとおりに実行されます。
`filename` はオプションで、スタックトレースでのみ使用されます。

<!--
In case of syntax error in `code`, `createScript` prints the syntax error to stderr
and throws an exception.
-->

`code` が文法エラーとなるケースでは、
`createScript` は標準エラーに文法エラーを出力して例外をスローします。


## Class: Script

<!--
A class for running scripts.  Returned by vm.createScript.
-->

スクリプトを実行するクラスです。`vm.createScript()` によって返されます。

### script.runInThisContext([timeout])

<!--
Similar to `vm.runInThisContext` but a method of a precompiled `Script` object.
`script.runInThisContext` runs the code of `script` and returns the result.
Running code does not have access to local scope, but does have access to the `global` object
(v8: in actual context).
`timeout` specifies an optional number of milliseconds to execute `code` before
terminating execution. If execution is terminated, `null` will be thrown.
-->

`vm.runInThisContext` と同様ですが、
こちらはプリコンパイル済みの `Script` オブジェクトのメソッドです。
`script.runInThisContext` は `script` のコードを実行してその結果を返します。
実行されるコードはローカルスコープにアクセスしませんが、
`global` オブジェクト (v8: 実際のコンテキスト) にはアクセスします。
`timeout` はオプションで、`code` の実行が打ち切られるまでの時間を
ミリ秒単位の数値で指定します。
実行が打ち切られると、`null` がスローされます。

<!--
Example of using `script.runInThisContext` to compile code once and run it multiple times:
-->

`script.runInThisContext` を使ってコードを一度だけコンパイルし、複数回実行する例:

    var vm = require('vm');

    globalVar = 0;

    var script = vm.createScript('globalVar += 1', 'myfile.vm');

    for (var i = 0; i < 1000 ; i += 1) {
      script.runInThisContext();
    }

    console.log(globalVar);

    // 1000


### script.runInNewContext([sandbox], [timeout])

<!--
Similar to `vm.runInNewContext` a method of a precompiled `Script` object.
`script.runInNewContext` runs the code of `script` with `sandbox` as the global object and returns the result.
Running code does not have access to local scope. `sandbox` is optional.
`timeout` specifies an optional number of milliseconds to execute `code` before
terminating execution. If execution is terminated, `null` will be thrown.
-->

`vm.runInNewContext` と同様ですが、
こちらはプリコンパイル済みの `Script` オブジェクトのメソッドです。
`script.runInNewContext` は `sandbox` がグローバルオブジェクトであるかのように
`script` のコードを実行してその結果を返します。
実行されるコードはローカルスコープにアクセスしません。
`sandbox` はオプションです。
`timeout` はオプションで、`code` の実行が打ち切られるまでの時間を
ミリ秒単位の数値で指定します。
実行が打ち切られると、`null` がスローされます。

<!--
Example: compile code that increments a global variable and sets one, then execute this code multiple times.
These globals are contained in the sandbox.
-->

例: グローバル変数をインクリメントしてセットするコードをコンパイルして、このコードを複数回実行します。
これらのグローバル変数はサンドボックスに含まれます。

    var util = require('util'),
        vm = require('vm'),
        sandbox = {
          animal: 'cat',
          count: 2
        };

    var script = vm.createScript('count += 1; name = "kitty"', 'myfile.vm');

    for (var i = 0; i < 10 ; i += 1) {
      script.runInNewContext(sandbox);
    }

    console.log(util.inspect(sandbox));

    // { animal: 'cat', count: 12, name: 'kitty' }

<!--
Note that running untrusted code is a tricky business requiring great care.  To prevent accidental
global variable leakage, `script.runInNewContext` is quite useful, but safely running untrusted code
requires a separate process.
-->

慎重を要するビジネスでは、信頼できないコードの実行は細心の注意が求められることに注意してください。
偶然グローバル変数が漏れてしまうことを防ぐために、`script.runInNewContext` はとても役立ちますが、
信頼できないコードを安全に実行するために別のプロセスを要求します。
