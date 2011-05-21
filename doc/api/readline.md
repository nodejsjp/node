## Readline

<!--
To use this module, do `require('readline')`. Readline allows reading of a
stream (such as STDIN) on a line-by-line basis.
-->
このモジュールを使うには、`require('readline')`します。
Readlineはストリーム (標準入力など) から行単位に読み込む基礎となります。

<!--
Note that once you've invoked this module, your node program will not
terminate until you've closed the interface, and the STDIN stream. Here's how
to allow your program to gracefully terminate:
-->
一度このモジュールを起動すると、インタフェースまたは標準入力をクローズするまで
node プログラムは終了しないことに注意してください。
プログラムをきれいに終了する方法を以下に示します:

    var rl = require('readline');

    var i = rl.createInterface(process.sdtin, process.stdout, null);
    i.question("What do you think of node.js?", function(answer) {
      // TODO: Log the answer in a database
      console.log("Thank you for your valuable feedback.");

      // These two lines together allow the program to terminate. Without
      // them, it would run forever.
      i.close();
      process.stdin.destroy();
    });

### rl.createInterface(input, output, completer)

<!--
Takes two streams and creates a readline interface. The `completer` function
is used for autocompletion. When given a substring, it returns `[[substr1,
substr2, ...], originalsubstring]`.
-->
二つのストリームを受け取って readline のインタフェースを作成します。
`completer` 関数は補完のために使われます。
部分文字列が与えられると、`[[substr1, substr2, ...], originalsubstring]`
を返します。

<!--
`createInterface` is commonly used with `process.stdin` and
`process.stdout` in order to accept user input:
-->
`createInterface` は一般的に、ユーザ入力を受け取るために `process.stdin`
および `process.stdout` とともに使われます:

    var readline = require('readline'),
      rl = readline.createInterface(process.stdin, process.stdout);

### rl.setPrompt(prompt, length)

<!--
Sets the prompt, for example when you run `node` on the command line, you see
`> `, which is node's prompt.
-->
プロンプトを設定します。
たとえば `node` をコマンドラインで実行すると、node のプロンプト `> `
を目にします。

### rl.prompt()

<!--
Readies readline for input from the user, putting the current `setPrompt`
options on a new line, giving the user a new spot to write.
-->
ユーザからの入力を準備します。
新しいライン上に現在の (`setPrompt` で設定された) プロンプトを表示し、
ユーザが新たに書き込む位置を与えます。

<!-- ### rl.getColumns() Not available? -->

### rl.question(query, callback)

<!--
Prepends the prompt with `query` and invokes `callback` with the user's
response. Displays the query to the user, and then invokes `callback` with the
user's response after it has been typed.
-->
`query` をプロンプトとして先頭に出力し、ユーザの応答を引数として
`callback` を呼び出します。
`query` をユーザに表示し、ユーザが応答をタイプすると `callback` 
が起動されます。

<!--
Example usage:
-->
使用例:

    interface.question('What is your favorite food?', function(answer) {
      console.log('Oh, so your favorite food is ' + answer);
    });

### rl.close()

<!--
  Closes tty.
-->

  tty をクローズします。

### rl.pause()

<!--
  Pauses tty.
-->
  tty を一時中断します。

### rl.resume()

<!--
  Resumes tty.
-->
  tty を再開します。

### rl.write()

<!--
  Writes to tty.
-->
  tty に出力します。

### Event: 'line'

`function (line) {}`

<!--
Emitted whenever the `in` stream receives a `\n`, usually received when the
user hits enter, or return. This is a good hook to listen for user input.
-->
入力ストリームが `\n` を受け取ると常に生成されます。
通常、ユーザがエンターキーを打つと受信します。
これは、ユーザ入力を受け取るよいフックとなります

<!--
Example of listening for `line`:
-->
`line` イベントを監視する例:

    rl.on('line', function (cmd) {
      console.log('You just typed: '+cmd);
    });

### Event: 'close'

`function () {}`

<!--
Emitted whenever the `in` stream receives a `^C` or `^D`, respectively known
as `SIGINT` and `EOT`. This is a good way to know the user is finished using
your program.
-->
入力ストリームから `^C` または `^D` を受け取ると生成されます。
それぞれ `SIGINT` および `EOT` として知られています。
これは、ユーザがプログラムを終了しようとしていることを知るよい方法です。

<!--
Example of listening for `close`, and exiting the program afterward:
-->
`close` を監視して、その後プログラムを終了する例:

    rl.on('close', function() {
      console.log('goodbye!');
      process.exit(0);
    });

<!--
Here's an example of how to use all these together to craft a tiny command
line interface:
-->
全てを同時に使用する方法を示す、小さなコマンドラインインタフェースの例:

    var readline = require('readline'),
      rl = readline.createInterface(process.stdin, process.stdout),
      prefix = 'OHAI> ';

    rl.on('line', function(line) {
      switch(line.trim()) {
        case 'hello':
          console.log('world!');
          break;
        default:
          console.log('Say what? I might have heard `' + line.trim() + '`');
          break;
      }
      rl.setPrompt(prefix, prefix.length);
      rl.prompt();
    }).on('close', function() {
      console.log('Have a great day!');
      process.exit(0);
    });
    console.log(prefix + 'Good to see you. Try typing stuff.');
    rl.setPrompt(prefix, prefix.length);
    rl.prompt();

<!--
Take a look at this slightly more complicated
[example](https://gist.github.com/901104), and
[http-console](http://github.com/cloudhead/http-console) for a real-life use
case.
-->

少しだけ複雑な[例](https://gist.github.com/901104)を見てください。
[http-console](http://github.com/cloudhead/http-console) 
は実際的なユースケースです。
