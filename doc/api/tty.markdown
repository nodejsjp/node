## TTY

<!--
Use `require('tty')` to access this module.
-->
`require('tty')` でこのモジュールにアクセスします。


### tty.open(path, args=[])

<!--
Spawns a new process with the executable pointed to by `path` as the session
leader to a new pseudo terminal.
-->
`path で指定された実行ファイルから新しいプロセスを新しい疑似端末のセッションリーダとして起動します。

<!--
Returns an array `[slaveFD, childProcess]`. `slaveFD` is the file descriptor
of the slave end of the pseudo terminal. `childProcess` is a child process
object.
-->
配列 `[slaveFD, childProcess]` を返します。
`slaveFD` は疑似端末のスレーブ側のファイル記述子です。
`childProcess` は子プロセスのオブジェクトです。


### tty.isatty(fd)

<!--
Returns `true` or `false` depending on if the `fd` is associated with a
terminal.
-->
`fd` が端末に関連づけられているかどうかを `true` または `false` で返します。


### tty.setRawMode(mode)

<!--
`mode` should be `true` or `false`. This sets the properies of the current
process's stdin fd to act either as a raw device or default.
-->
`mode` は `true` または `false` です。
これは現在のプロセスの標準入力ファイル記述子をローデバイスまたはデフォルトに振る舞うよう設定します。


### tty.getColumns()

<!--
Returns the number of columns associated with the current process's TTY.
-->
現在のプロセスに関連づけられた TTY のカラム数を返します。

<!--
Note that each time this number is changed the process receives a `SIGWINCH`
signal. So you can keep a cache of it like this:
-->
この数はプロセスが `SIGWINCH` を受信する度に変化することに注意してください。
次のようにキャッシュを保持することができます。

    var columns = tty.getColumns();
    process.on('SIGWINCH', function() {
      columns = tty.getColumns();
    });


