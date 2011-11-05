## TTY

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



### tty.isatty(fd)

<!--

Returns `true` or `false` depending on if the `fd` is associated with a
terminal.

-->
`fd` が端末に関連づけられているかどうかを `true` または `false` で返します。


### tty.setRawMode(mode)

<!--

`mode` should be `true` or `false`. This sets the properties of the current
process's stdin fd to act either as a raw device or default.

-->
`mode` は `true` または `false` です。
これは現在のプロセスの標準入力ファイル記述子をローデバイスまたはデフォルトに振る舞うよう設定します。


### tty.setWindowSize(fd, row, col)

<!--

`ioctl`s the window size settings to the file descriptor.

-->
ファイル記述子に関するウィンドウサイズの設定を `ioctl` します。

### tty.getWindowSize(fd)

<!--

Returns `[row, col]` for the TTY associated with the file descriptor.

-->
ファイル記述子に関連づけられた TTY の `[row, col]` を返します。

