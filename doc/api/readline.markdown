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
terminate until you've closed the interface. Here's how to allow your
program to gracefully exit:
-->

このモジュールを一度起動すると、このインタフェースを
クローズするまで node プログラムは終了しないことに注意してください。
プログラムをスムーズに終了する方法を以下に示します:

    var readline = require('readline');

    var rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout
    });

    rl.question("What do you think of node.js? ", function(answer) {
      // TODO: Log the answer in a database
      console.log("Thank you for your valuable feedback:", answer);

      rl.close();
    });

## readline.createInterface(options)

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

 - `terminal` - pass `true` if the `input` and `output` streams should be
   treated like a TTY, and have ANSI/VT100 escape codes written to it.
   Defaults to checking `isTTY` on the `output` stream upon instantiation.
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
Example:
-->

例:

    function completer(line) {
      var completions = '.help .error .exit .quit .q'.split(' ')
      var hits = completions.filter(function(c) { return c.indexOf(line) == 0 })
      // show all completions if none found
      return [hits.length ? hits : completions, line]
    }

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
Once you have a readline instance, you most commonly listen for the
`"line"` event.
-->

readline のインスタンスを作成すると、ほとんどの場合 `'line'` イベントを
監視することになります。

<!--
If `terminal` is `true` for this instance then the `output` stream will get
the best compatibility if it defines an `output.columns` property, and fires
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

### rl.setPrompt(prompt)

<!--
Sets the prompt, for example when you run `node` on the command line, you see
`> `, which is node's prompt.
-->

プロンプトを設定します。
たとえば、コマンドプロンプトで `node` コマンドを実行すると、
`> ` を見ることができます。これが Node のプロンプトです。

### rl.prompt([preserveCursor])

<!--
Readies readline for input from the user, putting the current `setPrompt`
options on a new line, giving the user a new spot to write. Set `preserveCursor`
to `true` to prevent the cursor placement being reset to `0`.

This will also resume the `input` stream used with `createInterface` if it has
been paused.
-->

ユーザからの入力を 1 行読み込みます。
現在の `setPrompt()` の値を新しい行に出力し、
ユーザに新しい入力エリアを与えます。
`preserveCursor` を `true` に設定すると、カーソル位置が
`0` にリセットされなくなります。

これは、 `createInterface()` によって使われる `input` ストリームが
中断されていれば再開します。

### rl.question(query, callback)

<!--
Prepends the prompt with `query` and invokes `callback` with the user's
response. Displays the query to the user, and then invokes `callback`
with the user's response after it has been typed.

This will also resume the `input` stream used with `createInterface` if
it has been paused.
-->

`query` をプロンプトとして、ユーザが応答すると `callback` を起動します。
ユーザに質問を表示し、ユーザが応答をタイプすると、`callback` が起動されます。

これは、 `createInterface()` によって使われる `input` ストリームが
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

### rl.close()

<!--
Closes the `Interface` instance, relinquishing control on the `input` and
`output` streams. The "close" event will also be emitted.
-->

`Interface` のインスタンスをクローズし、`input` および `output` ストリームの
制御を解放します。`'close'` イベントも生成されます。

### rl.write(data, [key])

<!--
Writes `data` to `output` stream. `key` is an object literal to represent a key
sequence; available if the terminal is a TTY.
-->

`data` を `output` ストリームに出力します。
`key` はキーシーケンスを表現するオブジェクトリテラルです;
ターミナルが TTY の場合に有効です。

<!--
This will also resume the `input` stream if it has been paused.
-->

これは、`input` ストリームが中断されていれば再開します。

<!--
Example:
-->

例:

    rl.write('Delete me!');
    // Simulate ctrl+u to delete the line written previously
    rl.write(null, {ctrl: true, name: 'u'});

## Events

### Event: 'line'

`function (line) {}`

<!--
Emitted whenever the `input` stream receives a `\n`, usually received when the
user hits enter, or return. This is a good hook to listen for user input.
-->

`input` ストリームから `\n` を読み込むごとに生成されます。
通常、ユーザがエンターまたはリターンを打つごとに受信します。
これはユーザ入力のよいフックとなります。

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
Emitted whenever the `input` stream is paused.
-->

`input` ストリームが中断されたときに生成されます。

<!--
Also emitted whenever the `input` stream is not paused and receives the
`SIGCONT` event. (See events `SIGTSTP` and `SIGCONT`)
-->

`input` ストリームが中断されていない時に `SIGCONT` イベントを受信した際にも
生成されます (`SIGTSTP` および `SIGCONT` も参照してください)。

<!--
Example of listening for `pause`:
-->

`'pause'` を監視する例:

    rl.on('pause', function() {
      console.log('Readline paused.');
    });

### Event: 'resume'

`function () {}`

<!--
Emitted whenever the `input` stream is resumed.

Example of listening for `resume`:
-->

`input` ストリームが再開された時に生成されます。

`'resume'` を監視する例:

    rl.on('resume', function() {
      console.log('Readline resumed.');
    });

### Event: 'close'

`function () {}`

<!--
Emitted when `close()` is called.
-->

`close()` が呼ばれた場合に生成されます。

<!--
Also emitted when the `input` stream receives its "end" event. The `Interface`
instance should be considered "finished" once this is emitted. For example, when
the `input` stream receives `^D`, respectively known as `EOT`.
-->

`input` ストリームが `'end'` イベントを受け取った場合にも生成されます。
これが生成された後、`Interface` インスタンスは完了したと考えられるべきです。
例えば、`input` ストリームが `EOT` として知られる `^D` を受け取った場合。

<!--
This event is also called if there is no `SIGINT` event listener present when
the `input` stream receives a `^C`, respectively known as `SIGINT`.
-->

このイベントは `SIGINT` イベントリスナが与えられていない場合に、
`input` ストリームが `SIGINT` として知られる `^C` を受け取った場合にも
生成されます。

### Event: 'SIGINT'

`function () {}`

<!--
Emitted whenever the `input` stream receives a `^C`, respectively known as
`SIGINT`. If there is no `SIGINT` event listener present when the `input`
stream receives a `SIGINT`, `pause` will be triggered.

Example of listening for `SIGINT`:
-->

`input` ストリームが `SIGINT` として知られる `^C` を受信した場合に
生成されます。
もし `input` ストリームが `SIGINT` を受信した時に `'SIGINT'` イベントの
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

Emitted whenever the `input` stream receives a `^Z`, respectively known as
`SIGTSTP`. If there is no `SIGTSTP` event listener present when the `input`
stream receives a `SIGTSTP`, the program will be sent to the background.

When the program is resumed with `fg`, the `pause` and `SIGCONT` events will be
emitted. You can use either to resume the stream.

The `pause` and `SIGCONT` events will not be triggered if the stream was paused
before the program was sent to the background.

Example of listening for `SIGTSTP`:
-->

**これは Windows では動作しません。**

`input` ストリームが `SIGTSTP` として知られる `^Z` を受信した場合に
生成されます。
もし `input` ストリームが `SIGTSTP` を受信した時に `'SIGTSTP'` イベントの
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

Emitted whenever the `input` stream is sent to the background with `^Z`,
respectively known as `SIGTSTP`, and then continued with `fg(1)`. This event
only emits if the stream was not paused before sending the program to the
background.

Example of listening for `SIGCONT`:
-->

**これは Windows では動作しません。**

`input` ストリームが `SIGTSTP` として知られる `^Z` によってバックグラウンドに
送られた後で、`fg(1)` によって再開されるた場合に生成されます。
このイベントはプログラムがバックグラウンドに送られる前にストリームが中断されていなかった場合にのみ生成されます。

`'SIGCONT'` を監視する例:

    rl.on('SIGCONT', function() {
      // `prompt` will automatically resume the stream
      rl.prompt();
    });

## Example: Tiny CLI

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
    }).on('close', function() {
      console.log('Have a great day!');
      process.exit(0);
    });
