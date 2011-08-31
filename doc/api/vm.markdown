## Executing JavaScript

<!--

You can access this module with:

-->
次のようにすることで、このモジュールにアクセスすることができます:

    var vm = require('vm');

<!--

JavaScript code can be compiled and run immediately or compiled, saved, and run later.

-->
JavaScript コードは、コンパイルされてすぐに実行されるか、コンパイルおよび保存されて後から実行されます。


### vm.runInThisContext(code, [filename])

<!--

`vm.runInThisContext()` compiles `code` as if it were loaded from `filename`,
runs it and returns the result. Running code does not have access to local scope. `filename` is optional.

-->
`vm.runInThisContext()` は `code` を `filename` からロードされたかのようにコンパイルし、
それを実行して結果を返します。実行されるコードはローカルスコープにアクセスしません。
`filename` はオプションです。

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


### vm.runInNewContext(code, [sandbox], [filename])

<!--

`vm.runInNewContext` compiles `code` to run in `sandbox` as if it were loaded from `filename`,
then runs it and returns the result. Running code does not have access to local scope and
the object `sandbox` will be used as the global object for `code`.
`sandbox` and `filename` are optional.

-->
`vm.runInNewContext` は `code` を `filename` からロードされたかのようにコンパイルし、
それを `sandbox` の中で実行して結果を返します。
実行されるコードはローカルスコープにアクセスせず、
`sandbox` が `code` にとってのグローバルオブジェクトとして使われます。
`sandbox` および `filename` はオプションです。

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

### vm.runInContext(code, context, [filename])

<!--
`vm.runInContext` compiles `code` to run in context `context` as if it were loaded from `filename`,
then runs it and returns the result. A (V8) context comprises a global object, together with a
set of built-in objects and functions. Running code does not have access to local scope and
the global object held within `context` will be used as the global object for `code`.
`filename` is optional.

Example: compile and execute code in a existing context.
-->
`vm.runInContext` は `code` をコンパイルして、
`filename` としてロードされたかのように、`context` をコンテキストとして
実行し、その結果を返します。
(V8 の) コンテキストは組み込みのオブジェクトと関数と共に、
グローバルオブジェクトを含みます。
実行されるコードはローカルスコープにアクセスせず、
`context` が `code` にとってのグローバルオブジェクトとして使われます。
`filename` はオプションです。

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
initialise the global object of the freshly constructed context.

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


### vm.createContext([initSandbox])

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

### vm.createScript(code, [filename])

<!--

`createScript` compiles `code` as if it were loaded from `filename`,
but does not run it. Instead, it returns a `vm.Script` object representing this compiled code.
This script can be run later many times using methods below.
The returned script is not bound to any global object.
It is bound before each run, just for that run. `filename` is optional.

-->
`createScript` は `code` を
`filename` からロードされたかのようにコンパイルしますが、実行はしません。
代わりに、コンパイルされたコードを表現する `vm.Script` オブジェクトを返します。
このスクリプトは後述のメソッドを使って後から何度でも実行することができます。
返されるスクリプトはどのグローバルオブジェクトとも結びつけられていません。
それぞれの実行前に結びつけることで、そのとおりに実行されます。
`filename` はオプションです。

<!--

In case of syntax error in `code`, `createScript` prints the syntax error to stderr
and throws an exception.

-->
`code` が文法エラーとなるケースでは、
`createScript` は標準エラーに文法エラーを出力して例外をスローします。


### script.runInThisContext()

<!--

Similar to `vm.runInThisContext` but a method of a precompiled `Script` object.
`script.runInThisContext` runs the code of `script` and returns the result.
Running code does not have access to local scope, but does have access to the `global` object
(v8: in actual context).

-->
`vm.runInThisContext` と同様ですが、
こちらはプリコンパイル済みの `Script` オブジェクトのメソッドです。
`script.runInThisContext` は `script` のコードを実行してその結果を返します。
実行されるコードはローカルスコープにアクセスしませんが、
`global` オブジェクト (v8: 実際のコンテキスト) にはアクセスします。

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


### script.runInNewContext([sandbox])

<!--

Similar to `vm.runInNewContext` a method of a precompiled `Script` object.
`script.runInNewContext` runs the code of `script` with `sandbox` as the global object and returns the result.
Running code does not have access to local scope. `sandbox` is optional.

-->
`vm.runInNewContext` と同様ですが、
こちらはプリコンパイル済みの `Script` オブジェクトのメソッドです。
`script.runInNewContext` は `sandbox` がグローバルオブジェクトであるかのように
`script` のコードを実行してその結果を返します。
実行されるコードはローカルスコープにアクセスしません。
`sandbox` はオプションです。

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
