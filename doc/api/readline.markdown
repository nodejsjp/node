## Readline

<!--
This module allows reading of a stream (such as STDIN) on a line-by-line basis.
-->
このモジュールはストリーム (たとえば標準入力)
を行ごとに読み込むことを可能にします。

<!--
Note that once you've invoked this module, your node program will not terminate
until you've closed the interface, and the STDIN stream. Here's how to allow
your program to gracefully terminate:
-->
このモジュールを一度起動すると、このインタフェースと 
(標準入力などの) ストリームをクローズするまで node
プログラムは終了しないことに注意してください。
きれいに終了する方法を以下に示します:

<pre>
var readline = require('readline');

var i = readline.createInterface(process.sdtin, process.stdout, null);
i.question("What do you think of node.js?", function(answer) {
  //TODO: Log the answer in a database
  console.log("Thank you for your valuable feedback.");
  i.close();                //These two lines together allow the program to
  process.stdin.destroy();  //terminate. Without them, it would run forever.
});
</pre>

### createInterface(input, output, completer)

<!--
Returns an interface object, which reads from input, and writes to output.
TODO: I think "completer" is used for tab-completion, but not sure.
-->
`input` を読み込み、`output` へ書き込むインターフェースオブジェクトを返します。
TODO: `completer` は TAB による補完に使われると思いますが、確かではありません。

### interface.setPrompt(prompt, length)

TODO

### interface.prompt()

<!--
TODO: Appears to trigger showing the prompt.
-->
TODO: プロンプトを表示するために呼び出されると思われる。

### interface.question(query, cb)

<!--
Displays the query to the user, and then calls the callback after the user
has typed in their response.
-->
`query` を表示しして、ユーザが応答をタイプするとコールバックが呼び出されます。

<!--
Example usage:
-->
使用例:

<pre>
interface.question("What is your favorite food?", function(answer) {
  console.log("Oh, so your favorite food is " + answer);
});
</pre>

### interface.close()

TODO

### interface.pause()

TODO

### interface.resume()

TODO

### interface.write()

TODO
