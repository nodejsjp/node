# TTY

<!--
Use `require('tty')` to access this module.
-->

`require('tty')` でこのモジュールにアクセスします。

<!--
Example:
-->

例:

    var tty = require('tty');
    process.stdin.resume();
    tty.setRawMode(true);
    process.stdin.on('keypress', function(char, key) {
      if (key && key.ctrl && key.name == 'c') {
        console.log('graceful exit');
        process.exit()
      }
    });



## tty.isatty(fd)

<!--
Returns `true` or `false` depending on if the `fd` is associated with a
terminal.
-->

`fd` が端末に関連づけられているかどうかを `true` または `false` で返します。


## tty.setRawMode(mode)

<!--
`mode` should be `true` or `false`. This sets the properties of the current
process's stdin fd to act either as a raw device or default.
-->

`mode` は `true` または `false` です。
これは現在のプロセスの標準入力ファイル記述子をローデバイスまたはデフォルトに振る舞うよう設定します。


## tty.setWindowSize(fd, row, col)

<!--
This function was removed in v0.6.0.
-->

この関数は v0.6.0 で削除されました。

## tty.getWindowSize(fd)

<!--
This function was removed in v0.6.0. Use `process.stdout.getWindowSize()`
instead.
-->

この関数は v0.6.0 で削除されました。
代わりに `process.stdout.getWindowSize()` を使用してください。
