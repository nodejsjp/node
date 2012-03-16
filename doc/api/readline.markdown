# Readline

    Stability: 3 - Stable

<!--
To use this module, do `require('readline')`. Readline allows reading of a
stream (such as STDIN) on a line-by-line basis.
-->

このモジュールを使用するには、`require('readline')` をします。
Readline はストリーム (たとえば標準入力) を行ごとに読み込むことを可能にします。

<!--
Note that once you've invoked this module, your node program will not
terminate until you've paused the interface. Here's how to allow your
program to gracefully pause:
-->

このモジュールを一度起動すると、このインタフェースを
中断するまで node プログラムは終了しないことに注意してください。
プログラムをスムーズに中断する方法を以下に示します:

    var rl = require('readline');

    var i = rl.createInterface(process.stdin, process.stdout, null);
    i.question("What do you think of node.js?", function(answer) {
      // TODO: Log the answer in a database
      console.log("Thank you for your valuable feedback.");

      i.pause();
    });

## rl.createInterface(input, output, completer)

<!--
Takes two streams and creates a readline interface. The `completer` function
is used for autocompletion. When given a substring, it returns `[[substr1,
substr2, ...], originalsubstring]`.
-->

二つのストリームを受け取り、readline のインタフェースを作成します。
`completer` 関数は補完のために使われます。
それは部分文字列を与えられると、`[[substr1, substr2, ...], originalsubstring]`
を返します。

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

    var readline = require('readline'),
      rl = readline.createInterface(process.stdin, process.stdout);

## Class: Interface

<!--
The class that represents a readline interface with a stdin and stdout
stream.
-->

標準入力と標準出力を持つ readline インタフェースを表現するクラスです。

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
-->

`in` ストリームからの入力を中断します。
必要なら後で再開することができます。

### rl.resume()

<!--
Resumes the readline `in` stream.
-->

`in` ストリームからの入力を再開します。

### rl.write()

<!--
Writes to tty.

This will also resume the `in` stream used with `createInterface` if it has
been paused.
-->

tty へ出力します。

これは、 `createInterface()` によって使われる `in` ストリームが
中断されていれば再開します。

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
