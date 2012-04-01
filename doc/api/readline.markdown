# Readline

    Stability: 2 - Unstable

<!--
To use this module, do `require('readline')`. Readline allows reading of a
stream (such as `process.stdin`) on a line-by-line basis.
-->

このモジュールを使用するには、`require('readline')` をします。
Readline はストリーム (たとえば `process.stdin`)
を行ごとに読み込むことを可能にします。

<!--
Note that once you've invoked this module, your node program will not
terminate until you've paused the interface. Here's how to allow your
program to gracefully pause:
-->

このモジュールを一度起動すると、このインタフェースを
中断するまで node プログラムは終了しないことに注意してください。
プログラムをスムーズに中断する方法を以下に示します:

    var rl = require('readline');

    var i = rl.createInterface({
      input: process.stdin,
      output: process.stdout
    });

    i.question("What do you think of node.js? ", function(answer) {
      // TODO: Log the answer in a database
      console.log("Thank you for your valuable feedback:", answer);

      i.pause();
    });

## rl.createInterface(options)

<!--
Creates a readline `Interface` instance. Accepts an "options" Object that takes
the following values:
-->

行を読み込む `Interface` のインスタンスを作成します。
以下の値を含む `options` オブジェクトを受け取ります。

<!--
 - `input` - the readable stream to listen to (Required).

 - `output` - the writable stream to write readline data to (Required).

 - `completer` - an optional function that is used for Tab autocompletion. See
   below for an example of using this.

 - `terminal` - pass `true` if the `input` and `output` streams should be treated
   like a TTY, and have ANSI/VT100 escape codes written to it. Defaults to
   checking `isTTY` on the `output` stream upon instantiation.
-->

 - `input` - 監視する入力ストリーム (必須)。

 - `output` - 読み込んだデータを書くための出力ストリーム (必須)。

 - `completer` - タブによる自動補完のための関数 (オプション)。
   後述の例を参照してください。

 - `terminal` - `input` および `output` ストリームが TTY デバイスで、
   ANSI/VT100 エスケープコードを出力する場合は `true` を渡します。
   デフォルトはインスタンス生成時に `output` に対して
   `isTTY` でチェックします。

<!--
The `completer` function is given a the current line entered by the user, and
is supposed to return an Array with 2 entries:
-->

`completer` 関数にはユーザが入力した現在の行が与えられ、
2 つのエントリを含む配列を返すことが期待されます:

<!--
 1. An Array with matching entries for the completion.

 2. The substring that was used for the matching.
-->

 1. 補完によってマッチするエントリの配列。

 2. マッチングに使用された部分文字列。

<!--
Which ends up looking something like:
`[[substr1, substr2, ...], originalsubstring]`.
-->

それは次のようになります:
`[[substr1, substr2, ...], originalsubstring]`。

<!--
Also `completer` can be run in async mode if it accepts two arguments:
-->

`completer` が二つの引数を持つなら、それは非同期モードで実行されます。

    function completer(linePartial, callback) {
      callback(null, [['123'], linePartial]);
    }

<!--
`createInterface` is commonly used with `process.stdin` and
`process.stdout` in order to accept user input:
-->

`createInterface` には通常、ユーザからの入力を受け取るために `process.stdin` と
`process.stdout` が使用されます。

    var readline = require('readline');
    var rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout
    });

<!--
Once you have a readline instance, you most commonly listen for the `"line"` event.
-->

readline のインスタンスを作成すると、ほとんどの場合 `'line'` イベントを
監視することになります。

<!--
If `terminal` is `true` for this instance then the `output` stream will get the
best compatability if it defines an `output.columns` property, and fires
a `"resize"` event on the `output` if/when the columns ever change
(`process.stdout` does this automatically when it is a TTY).
-->

もしこのインスタンスの `terminal` が `true` の場合、
`output` ストリームはもし `outout.columns` プロパティが定義されていれば
それに適合し、カラム幅が変更されると `output` 上で
`'resize'` イベントが生成されます
(`process.stdout` が TTY の場合、それは自動的に行われます)。

## Class: Interface

<!--
The class that represents a readline interface with an input and output
stream.
-->

入力と出力を持つ readline インタフェースを表現するクラスです。

### rl.setPrompt(prompt, length)

<!--
Sets the prompt, for example when you run `node` on the command line, you see
`> `, which is node's prompt.
-->

プロンプトを設定します。
たとえば、コマンドプロンプトで `node` コマンドを実行すると、
`> ` を見ることができます。これが Node のプロンプトです。

### rl.prompt()

<!--
Readies readline for input from the user, putting the current `setPrompt`
options on a new line, giving the user a new spot to write.

This will also resume the `in` stream used with `createInterface` if it has
been paused.
-->

ユーザからの入力を 1 行読み込みます。
現在の `setPrompt()` の値を新しい行に出力し、
ユーザに新しい入力エリアを与えます。

これは、 `createInterface()` によって使われる `in` ストリームが
中断されていれば再開します。

### rl.question(query, callback)

<!--
Prepends the prompt with `query` and invokes `callback` with the user's
response. Displays the query to the user, and then invokes `callback` with the
user's response after it has been typed.

This will also resume the `in` stream used with `createInterface` if it has
been paused.
-->

`query` をプロンプトとして、ユーザが応答すると `callback` を起動します。
ユーザに質問を表示し、ユーザが応答をタイプすると、`callback` が起動されます。

これは、 `createInterface()` によって使われる `in` ストリームが
中断されていれば再開します。

<!--
Example usage:
-->

使用例:

    interface.question('What is your favorite food?', function(answer) {
      console.log('Oh, so your favorite food is ' + answer);
    });

### rl.pause()

<!--
Pauses the readline `in` stream, allowing it to be resumed later if needed.
Pauses the readline `input` stream, allowing it to be resumed later if needed.
-->

`input` ストリームからの入力を中断します。
必要なら後で再開することができます。

### rl.resume()

<!--
Resumes the readline `input` stream.
-->

`input` ストリームからの入力を再開します。

### rl.write()

<!--
Writes to `output` stream.

This will also resume the `input` stream if it has been paused.
-->

`output` へ出力します。

これは、`input` ストリームが中断されていれば再開します。

### Event: 'line'

`function (line) {}`

<!--
Emitted whenever the `in` stream receives a `\n`, usually received when the
user hits enter, or return. This is a good hook to listen for user input.
-->

`in` ストリームから `\n` を読み込むごとに生成されます。
通常、ユーザがエンターまたはリターンを打つごとに受信します。
これはユーザ入浴のよいフックとなります。

<!--
Example of listening for `line`:
-->

`line` を監視する例:

    rl.on('line', function (cmd) {
      console.log('You just typed: '+cmd);
    });

### Event: 'pause'

`function () {}`

<!--
Emitted whenever the `in` stream is paused or receives `^D`, respectively known
as `EOT`. This event is also called if there is no `SIGINT` event listener
present when the `in` stream receives a `^C`, respectively known as `SIGINT`.

Also emitted whenever the `in` stream is not paused and receives the `SIGCONT`
event. (See events `SIGTSTP` and `SIGCONT`)

Example of listening for `pause`:
-->

`in` ストリームが中断された時、または `EOT` として知られる `^D` 
を受信すると生成されます。
このイベントは、`in` ストリームが `SIGINT` として知られる `^C` を受信した際に、
`SIGINT` イベントのリスナが存在しない場合にも生成されます。

`in` ストリームが中断されていない時に `SIGCONT` イベントを受信した際にも
生成されます (`SIGTSTP` および `SIGCONT` も参照してください)。

`'pause'` を監視する例:

    rl.on('pause', function() {
      console.log('Readline paused.');
    });

### Event: 'resume'

`function () {}`

<!--
Emitted whenever the `in` stream is resumed.

Example of listening for `resume`:
-->

`in` ストリームが再開された時に生成されます。

`'resume'` を監視する例:

    rl.on('resume', function() {
      console.log('Readline resumed.');
    });

### Event: 'end'

`function () {}`

<!--
Emitted when the `input` stream receives its "end" event, or when `^D` is
pressed by the user. It's generally a good idea to consider this `Interface`
instance as completed after this is emitted.
-->

`input` ストリームが `'end'` イベントを受け取った場合、
またはユーザが `^D` を推した場合に生成されます。
通常、これが生成された後で `Interface` インスタンスは完了すると考えられます。

### Event: 'SIGINT'

`function () {}`

<!--
Emitted whenever the `in` stream receives a `^C`, respectively known as
`SIGINT`. If there is no `SIGINT` event listener present when the `in` stream
receives a `SIGINT`, `pause` will be triggered.

Example of listening for `SIGINT`:
-->

`in` ストリームが `SIGINT` として知られる `^C` を受信した場合に生成されます。
もし `in` ストリームが `SIGINT` を受信した時に `'SIGINT'` イベントの
リスナが存在しなければ、`'pause'` イベントがトリガされます。

`'SIGINT'` を監視する例:

    rl.on('SIGINT', function() {
      rl.question('Are you sure you want to exit?', function(answer) {
        if (answer.match(/^y(es)?$/i)) rl.pause();
      });
    });

### Event: 'SIGTSTP'

`function () {}`

<!--
**This does not work on Windows.**

Emitted whenever the `in` stream receives a `^Z`, respectively known as
`SIGTSTP`. If there is no `SIGTSTP` event listener present when the `in` stream
receives a `SIGTSTP`, the program will be sent to the background.

When the program is resumed with `fg`, the `pause` and `SIGCONT` events will be
emitted. You can use either to resume the stream.

The `pause` and `SIGCONT` events will not be triggered if the stream was paused
before the program was sent to the background.

Example of listening for `SIGTSTP`:
-->

**これは Windows では動作しません。**

`in` ストリームが `SIGTSTP` として知られる `^Z` を受信した場合に生成されます。
もし `in` ストリームが `SIGTSTP` を受信した時に `'SIGTSTP'` イベントの
リスナが存在しなければ、プログラムはバックグラウンドに送られます。

プログラムが `fg` により再開されると、`'pause'` および `'SIGCONT'` イベントが
生成されます。どちらもストリームを再開するために使うことができます。

プログラムがバックグラウンドに送られる前にストリームが中断されていると、
`'pause'` および `'SIGCONT'` イベントは生成されません。

`'SIGTSTP'` を監視する例:

    rl.on('SIGTSTP', function() {
      // This will override SIGTSTP and prevent the program from going to the
      // background.
      console.log('Caught SIGTSTP.');
    });

### Event: 'SIGCONT'

`function () {}`

<!--
**This does not work on Windows.**

Emitted whenever the `in` stream is sent to the background with `^Z`,
respectively known as `SIGTSTP`, and then continued with `fg`. This event only
emits if the stream was not paused before sending the program to the
background.

Example of listening for `SIGCONT`:
-->

**これは Windows では動作しません。**

`in` ストリームが `SIGTSTP` として知られる `^Z` によってバックグラウンドに
送られた後で、`fg` によって再開されるた場合に生成されます。
このイベントはプログラムがバックグラウンドに送られる前にストリームが中断されていなかった場合にのみ生成されます。

`'SIGCONT'` を監視する例:

    rl.on('SIGCONT', function() {
      // `prompt` will automatically resume the stream
      rl.prompt();
    });

<!--
Here's an example of how to use all these together to craft a tiny command
line interface:
-->

全てを一緒に使う、小さなコマンドラインインタフェースの例:

    var readline = require('readline'),
        rl = readline.createInterface(process.stdin, process.stdout);

    rl.setPrompt('OHAI> ');
    rl.prompt();

    rl.on('line', function(line) {
      switch(line.trim()) {
        case 'hello':
          console.log('world!');
          break;
        default:
          console.log('Say what? I might have heard `' + line.trim() + '`');
          break;
      }
      rl.prompt();
    }).on('pause', function() {
      console.log('Have a great day!');
      process.exit(0);
    });
