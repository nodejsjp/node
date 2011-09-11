## Readline

<!--
To use this module, do `require('readline')`. Readline allows reading of a
stream (such as STDIN) on a line-by-line basis.
-->
このモジュールを使用するには、`require('readline')` をします。
Readline はストリーム (たとえば標準入力) を行ごとに読み込むことを可能にします。

<!--
Note that once you've invoked this module, your node program will not
terminate until you've closed the interface, and the STDIN stream. Here's how
to allow your program to gracefully terminate:
-->
このモジュールを一度起動すると、このインタフェースと 
(標準入力などの) ストリームをクローズするまで node
プログラムは終了しないことに注意してください。
きれいに終了する方法を以下に示します:

    var rl = require('readline');

    var i = rl.createInterface(process.stdin, process.stdout, null);
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
二つのストリームを受け取り、readline のインタフェースを作成します。
`completer` 関数は補完のために使われます。
それは部分文字列を与えられると、`[[substr1, substr2, ...], originalsubstring]`
を返します。

<!--
`createInterface` is commonly used with `process.stdin` and
`process.stdout` in order to accept user input:
-->
`createInterface` には通常、ユーザからの入力を受け取るために `process.stdin` と
`process.stdout` が使用されます。

    var readline = require('readline'),
      rl = readline.createInterface(process.stdin, process.stdout);

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
-->
ユーザからの入力を 1 行読み込みます。
現在の `setPrompt()` の値を新しい行に出力し、
ユーザに新しい入力エリアを与えます。

<!-- ### rl.getColumns() Not available? -->

### rl.question(query, callback)

<!--
Prepends the prompt with `query` and invokes `callback` with the user's
response. Displays the query to the user, and then invokes `callback` with the
user's response after it has been typed.
-->
`query` をプロンプトとして、ユーザが応答すると `callback` を起動します。
ユーザに質問を表示し、ユーザが応答をタイプすると、`callback` が起動されます。

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
tty からの入力を中断します。

### rl.resume()

<!--
  Resumes tty.
-->
tty からの入力を再開します。

### rl.write()

<!--
  Writes to tty.
-->
tty へ出力します。

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

### Event: 'close'

`function () {}`

<!--
Emitted whenever the `in` stream receives a `^C` or `^D`, respectively known
as `SIGINT` and `EOT`. This is a good way to know the user is finished using
your program.
-->
`in` ストリームから `^C` または `^D` を受信すると生成されます。
それぞれ `SIGINT` および `EOT` として知られています。
これはユーザがプログラムの使用を終わらせようとしていることを知るよい方法です。

<!--
Example of listening for `close`, and exiting the program afterward:
-->
`close` を監視し、その後プログラムを終了する例:

    rl.on('close', function() {
      console.log('goodbye!');
      process.exit(0);
    });

<!--
Here's an example of how to use all these together to craft a tiny command
line interface:
-->
全てを一緒に使う、小さなコマンドラインインタフェースの例:

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
より複雑な [例](https://gist.github.com/901104) や、実践的なユースケースとして
[http-console](http://github.com/cloudhead/http-console)
を見ることが出来ます。
