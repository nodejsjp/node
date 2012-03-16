# File System

    Stability: 3 - Stable

<!--name=fs-->

<!--
File I/O is provided by simple wrappers around standard POSIX functions.  To
use this module do `require('fs')`. All the methods have asynchronous and
synchronous forms.
-->

File I/O は POSIX 標準の関数に対する単純なラッパーとして提供されます。
このモジュールを使用するには `require('fs')` してください。
全てのメソッドは非同期と同期の形式があります。

<!--
The asynchronous form always take a completion callback as its last argument.
The arguments passed to the completion callback depend on the method, but the
first argument is always reserved for an exception. If the operation was
completed successfully, then the first argument will be `null` or `undefined`.

When using the synchronous form any exceptions are immediately thrown.
You can use try/catch to handle exceptions or allow them to bubble up.
-->

非同期の形式は常に最後の引数として完了コールバックを受け取ります。
引数として渡される完了コールバックはメソッドに依存しますが、
最初の引数は常に例外のために予約されています。
操作が成功で完了すると最初の引数は `null` または `undefined` となります

同期の形式では、全ての例外はすぐにスローされます。
例外は try/catch で捕まえることも、そのまま通過させることもできます。

<!--
Here is an example of the asynchronous version:
-->

非同期バージョンの例です:

    var fs = require('fs');

    fs.unlink('/tmp/hello', function (err) {
      if (err) throw err;
      console.log('successfully deleted /tmp/hello');
    });

<!--
Here is the synchronous version:
-->

同期バージョンです:

    var fs = require('fs');

    fs.unlinkSync('/tmp/hello')
    console.log('successfully deleted /tmp/hello');

<!--
With the asynchronous methods there is no guaranteed ordering. So the
following is prone to error:
-->

非同期メソッドでは順序の保証はありません。
以下のような傾向のエラーがあります。

    fs.rename('/tmp/hello', '/tmp/world', function (err) {
      if (err) throw err;
      console.log('renamed complete');
    });
    fs.stat('/tmp/world', function (err, stats) {
      if (err) throw err;
      console.log('stats: ' + JSON.stringify(stats));
    });

<!--
It could be that `fs.stat` is executed before `fs.rename`.
The correct way to do this is to chain the callbacks.
-->

`fs.stat` は `fs.rename` より先に実行される可能性がありrます。
正しい方法はコールバックをチェーンすることです。

    fs.rename('/tmp/hello', '/tmp/world', function (err) {
      if (err) throw err;
      fs.stat('/tmp/world', function (err, stats) {
        if (err) throw err;
        console.log('stats: ' + JSON.stringify(stats));
      });
    });

<!--
In busy processes, the programmer is _strongly encouraged_ to use the
asynchronous versions of these calls. The synchronous versions will block
the entire process until they complete--halting all connections.

Relative path to filename can be used, remember however that this path will be relative
to `process.cwd()`.
-->

忙しいプロセスでは、プログラマはこれらの非同期バージョンを使うことが*強く推奨*されます。
同期バージョンはそれが完了するまでプロセス全体をブロックします － 全ての接続を停止します。

ファイル名には相対パスを使うことが出来ます。しかし、このパスは
`process.cwd()` からの相対パスであることを思い出してください。

## fs.rename(path1, path2, [callback])

<!--
Asynchronous rename(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の rename(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.renameSync(path1, path2)

<!--
Synchronous rename(2).
-->

同期の rename(2)。

## fs.truncate(fd, len, [callback])

<!--
Asynchronous ftruncate(2). No arguments other than a possible exception are
given to the completion callback.
-->

非同期の ftruncate(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.truncateSync(fd, len)

<!--
Synchronous ftruncate(2).
-->

同期の ftruncate(2)。

## fs.chown(path, uid, gid, [callback])

<!--
Asynchronous chown(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の chown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.chownSync(path, uid, gid)

<!--
Synchronous chown(2).
-->

同期の chown(2)。

## fs.fchown(fd, uid, gid, [callback])

<!--
Asynchronous fchown(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の fchown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.fchownSync(fd, uid, gid)

<!--
Synchronous fchown(2).
-->

同期の fchown(2)。

## fs.lchown(path, uid, gid, [callback])

<!--
Asynchronous lchown(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の lchown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.lchownSync(path, uid, gid)

<!--
Synchronous lchown(2).
-->

同期の lchown(2)。

## fs.chmod(path, mode, [callback])

<!--
Asynchronous chmod(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の chmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.chmodSync(path, mode)

<!--
Synchronous chmod(2).
-->

同期の chmod(2)。

## fs.fchmod(fd, mode, [callback])

<!--
Asynchronous fchmod(2). No arguments other than a possible exception
are given to the completion callback.
-->

非同期の fchmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.fchmodSync(fd, mode)

<!--
Synchronous fchmod(2).
-->

同期の fchmod(2)。

## fs.lchmod(path, mode, [callback])

<!--
Asynchronous lchmod(2). No arguments other than a possible exception
are given to the completion callback.
-->

非同期の lchmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.lchmodSync(path, mode)

<!--
Synchronous lchmod(2).
-->

同期の lchmod(2)。

## fs.stat(path, [callback])

<!--
Asynchronous stat(2). The callback gets two arguments `(err, stats)` where
`stats` is a [fs.Stats](#fs.Stats) object.  See the [fs.Stats](#fs.Stats)
section below for more information.
-->

非同期の stat(2)。コールバックは 2 つの引数を受け取る `(err, stats)`で、
`stats` は [fs.Stats](#fs.Stats) オブジェクトです。
詳細は [fs.Stats](#fs.Stats) の節を参照してください。

<!--
See the [fs.Stats](#fs.Stats) section below for more information.
-->

より詳しくは後述の [fs.Stats](#fs.Stats) の節を参照してください。

## fs.lstat(path, [callback])

<!--
Asynchronous lstat(2). The callback gets two arguments `(err, stats)` where
`stats` is a `fs.Stats` object. `lstat()` is identical to `stat()`, except that if
`path` is a symbolic link, then the link itself is stat-ed, not the file that it
refers to.
-->

非同期の lstat(2)。コールバックは 2 つの引数を受け取る `(err, stats)`で、
`stats` は `fs.Stats` オブジェクトです。
`lstat()` はパスがシンボリックリンクだった場合に、
参照先のファイルではなくそのリンク自身が調べられる点を除いて `stat()` と同じす。

## fs.fstat(fd, [callback])

<!--
Asynchronous fstat(2). The callback gets two arguments `(err, stats)` where
`stats` is a `fs.Stats` object. `fstat()` is identical to `stat()`, except that
the file to be stat-ed is specified by the file descriptor `fd`.
-->

非同期の fstat(2)。コールバックは 2 つの引数を受け取る `(err, stats)` で、
`stats` は `fs.Stats` オブジェクトです。
状態を取得するファイルをファイル記述子 `fd` で指定することを除いて、
`fstat()` は `stat()` と同じです。

## fs.statSync(path)

<!--
Synchronous stat(2). Returns an instance of `fs.Stats`.
-->

同期の stat(2)。`fs.Stats` のインスタンスを返します。

## fs.lstatSync(path)

<!--
Synchronous lstat(2). Returns an instance of `fs.Stats`.
-->

同期の lstat(2)。`fs.Stats` のインスタンスを返します。

## fs.fstatSync(fd)

<!--
Synchronous fstat(2). Returns an instance of `fs.Stats`.
-->

同期の fstat(2)。`fs.Stats` のインスタンスを返します。

## fs.link(srcpath, dstpath, [callback])

<!--
Asynchronous link(2). No arguments other than a possible exception are given to
the completion callback.
-->

非同期の link(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.linkSync(srcpath, dstpath)

<!--
Synchronous link(2).
-->

同期の link(2)。

## fs.symlink(linkdata, path, [type], [callback])

<!--
Asynchronous symlink(2). No arguments other than a possible exception are given
to the completion callback.
`type` argument can be either `'dir'` or `'file'` (default is `'file'`).  It is only 
used on Windows (ignored on other platforms).
-->

非同期の symlink(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。
`type` 引数は `'dir'` または `'file'` (デフォルトは `'file'`) です。
これは Windows でのみ使われます (他のプラットフォームでは無視されます)。

## fs.symlinkSync(linkdata, path, [type])

<!--
Synchronous symlink(2).
-->

同期の symlink(2)。

## fs.readlink(path, [callback])

<!--
Asynchronous readlink(2). The callback gets two arguments `(err,
linkString)`.
-->

非同期の readlink(2)。コールバックは 2 つの引数を受け取る `(err, linkString)`です。

## fs.readlinkSync(path)

<!--
Synchronous readlink(2). Returns the symbolic link's string value.
-->

同期の readlink(2)。シンボリックリンクの持つ文字列値を返します。

## fs.realpath(path, [callback])

<!--
Asynchronous realpath(2).  The callback gets two arguments `(err,
resolvedPath)`.  May use `process.cwd` to resolve relative paths.
-->

非同期の realpath(2)。コールバックは 2 つの引数を受け取る `(err, resolvedPath)`です。
相対パスを解決するために `process.cwd` を使用することができます。

## fs.realpathSync(path)

<!--
Synchronous realpath(2). Returns the resolved path.
-->

同期の realpath(2)。解決されたパスを返します。

## fs.unlink(path, [callback])

<!--
Asynchronous unlink(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の unlink(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.unlinkSync(path)

<!--
Synchronous unlink(2).
-->

同期の unlink(2)。

## fs.rmdir(path, [callback])

<!--
Asynchronous rmdir(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の rmdir(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.rmdirSync(path)

<!--
Synchronous rmdir(2).
-->

同期の rmdir(2)。

## fs.mkdir(path, [mode], [callback])

<!--
Asynchronous mkdir(2). No arguments other than a possible exception are given
to the completion callback. `mode` defaults to `0777`.
-->

非同期の mkdir(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。
`mode` のデフォルトは `0777` です。

## fs.mkdirSync(path, [mode])

<!--
Synchronous mkdir(2).
-->

同期の mkdir(2)。

## fs.readdir(path, [callback])

<!--
Asynchronous readdir(3).  Reads the contents of a directory.
The callback gets two arguments `(err, files)` where `files` is an array of
the names of the files in the directory excluding `'.'` and `'..'`.
-->

非同期の readdir(3)。ディレクトリの内容を読み込みます。
コールバックは 2 つの引数を受け取る `(err, files)`で、
`files` は `'.'` と `'..'` を除くディレクトリ内のファイル名の配列です。

## fs.readdirSync(path)

<!--
Synchronous readdir(3). Returns an array of filenames excluding `'.'` and
`'..'`.
-->

同期の readdir(3)。`'.'` と `'..'` を除くディレクトリ内のファイル名の配列を返します。

## fs.close(fd, [callback])

<!--
Asynchronous close(2).  No arguments other than a possible exception are given
to the completion callback.
-->

非同期の close(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.closeSync(fd)

<!--
Synchronous close(2).
-->

同期の close(2)。

## fs.open(path, flags, [mode], [callback])

<!--
Asynchronous file open. See open(2). `flags` can be:

* `'r'` - Open file for reading.
An exception occurs if the file does not exist.

* `'r+'` - Open file for reading and writing.
An exception occurs if the file does not exist.

* `'w'` - Open file for writing.
The file is created (if it does not exist) or truncated (if it exists).

* `'wx'` - Like `'w'` but opens the file in exclusive mode.

* `'w+'` - Open file for reading and writing.
The file is created (if it does not exist) or truncated (if it exists).

* `'wx+'` - Like `'w+'` but opens the file in exclusive mode.

* `'a'` - Open file for appending.
The file is created if it does not exist.

* `'ax'` - Like `'a'` but opens the file in exclusive mode.

* `'a+'` - Open file for reading and appending.
The file is created if it does not exist.

* `'ax+'` - Like `'a+'` but opens the file in exclusive mode.

`mode` defaults to `0666`. The callback gets two arguments `(err, fd)`.

Exclusive mode (`O_EXCL`) ensures that `path` is newly created. `fs.open()`
fails if a file by that name already exists. On POSIX systems, symlinks are
not followed. Exclusive mode may or may not work with network file systems.
-->

非同期のファイルオープン。open(2) を参照してください。
フラグは以下になります:

* `'r'` - 読み込み専用でオープンします。
ファイルが存在しない場合は例外が発生します。

* `'r+'` - 読み書き両用でオープンします。
ファイルが存在しない場合は例外が発生します。

* `'w'` - 書き込み専用でオープンします。
ファイルは作成される (存在しない場合) または長さ 0 に切り詰められます
(存在する場合)。

* `'wx'` - `'w'` と似ていますが、ファイルを排他モードでオープンします。

* `'w+'` - 読み書き両用でオープンします。
ファイルは作成される (存在しない場合) または長さ 0 に切り詰められます
(存在する場合)。

* `'wx'` - `'w+'` と似ていますが、ファイルを排他モードでオープンします。

* `'a'` - 追記用でオープンします。
ファイルが存在しない場合は作成されます。

* `'ax'` - `'a'` と似ていますが、ファイルを排他モードでオープンします。

* `'a+'` - 読み込みおよび追記用でオープンします。
ファイルが存在しない場合は作成されます。

* `'ax+'` - `'a+'` と似ていますが、ファイルを排他モードでオープンします。

`mode` のデフォルトは 0666 です。
コールバックは 2 つの引数を受け取る `(err, fd)`です。

排他モード (`O_EXCL`) は、`path` が新しいファイルとして作成されることを
確実にします。 指定された名前のファイルが既に存在する場合、
`fs.open()` は失敗します。
POSIX システムでは、シンボリックリンクは辿られません。
排他モードはネットワークファイルシステムでは動くかもしれませんし、
動かないかもしれません。

## fs.openSync(path, flags, [mode])

<!--
Synchronous open(2).
-->

同期の open(2)。

## fs.utimes(path, atime, mtime, [callback])
## fs.utimesSync(path, atime, mtime)

<!--
Change file timestamps.
Change file timestamps of the file referenced by the supplied path.
-->

渡されたパスが参照するファイルのタイムスタンプを変更します。

## fs.futimes(fd, atime, mtime, [callback])
## fs.futimesSync(fd, atime, mtime)

<!--
Change the file timestamps of a file referenced by the supplied file
descriptor.
-->

渡されたファイル記述子が参照するファイルのタイムスタンプを変更します。

## fs.fsync(fd, [callback])

<!--
Asynchronous fsync(2). No arguments other than a possible exception are given
to the completion callback.
-->

非同期の fsync(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

## fs.fsyncSync(fd)

<!--
Synchronous fsync(2).
-->

同期の fsync(2)。

## fs.write(fd, buffer, offset, length, position, [callback])

<!--
Write `buffer` to the file specified by `fd`.
-->

`fd` で指定されたファイルに `buffer` を書き込みます。

<!--
`offset` and `length` determine the part of the buffer to be written.
-->

`offset` と `length` は書き込まれるバッファの部分を決定します。

<!--
`position` refers to the offset from the beginning of the file where this data
should be written. If `position` is `null`, the data will be written at the
current position.
See pwrite(2).
-->

`position` はデータが書き込まれる位置をファイルの先頭からのオフセットで示します。
`position` が `null` の場合、データは現在の位置から書き込まれます。
pwrite(2) を参照してください。

<!--
The callback will be given two arguments `(err, written)` where `written`
specifies how many _bytes_ were written.
The callback will be given three arguments `(err, written, buffer)` where `written`
specifies how many _bytes_ were written from `buffer`.
-->

コールバックは 3 つの引数が与えられる `(err, written, buffer)` で、
`written` は `buffer` から書き込まれた*バイト数*を示します。

<!--
Note that it is unsafe to use `fs.write` multiple times on the same file
without waiting for the callback. For this scenario,
`fs.createWriteStream` is strongly recommended.
-->

同じファイルに対してコールバックされるのを待つことなく `fs.write()` を何度も呼び出すことは、安全ではないことに注意してください。
このシナリオでは、 `fs.createWriteStream()` を強く推奨します。

## fs.writeSync(fd, buffer, offset, length, position)

<!--
Synchronous version of buffer-based `fs.write()`. Returns the number of bytes
written.
-->
同期版のバッファに基づく `fs.write()`。書き込まれたバイト数を返します。

## fs.writeSync(fd, str, position, [encoding])

<!--
Synchronous version of string-based `fs.write()`. `encoding` defaults to
`'utf8'`. Returns the number of _bytes_ written.
-->

同期版の文字列に基づく `fs.write()`。
`encoding` のデフォルトは `'utf8'` です。
書き込まれたバイト数を返します。

## fs.read(fd, buffer, offset, length, position, [callback])

<!--
Read data from the file specified by `fd`.
-->

`fd` で指定されたファイルからデータを読み込みます。

<!--
`buffer` is the buffer that the data will be written to.
-->

`buffer` はデータが書き込まれるバッファです。

<!--
`offset` is offset within the buffer where writing will start.
-->

`offset` は書き込みを開始するバッファ内のオフセットです。

<!--
`length` is an integer specifying the number of bytes to read.
-->

`length` は読み込むバイト数を指定する整数です。

<!--
`position` is an integer specifying where to begin reading from in the file.
If `position` is `null`, data will be read from the current file position.
-->

`position` はファイルの読み込みを開始する位置を指定する整数です。
`position` が `null` の場合、データは現在の位置から読み込まれます。

<!--
The callback is given the three arguments, `(err, bytesRead, buffer)`.
-->

コールバックは3つの引数が与えられる `(err, bytesRead, buffer)` です。

## fs.readSync(fd, buffer, offset, length, position)

<!--
Synchronous version of buffer-based `fs.read`. Returns the number of
`bytesRead`.
-->

同期版のバッファに基づく `fs.read`。`bytesRead` の数を返します。

## fs.readSync(fd, length, position, encoding)

<!--
Synchronous version of string-based `fs.read`. Returns the number of
`bytesRead`.
-->

同期版の文字列に基づく `fs.read`。`bytesRead` の数を返します。

## fs.readFile(filename, [encoding], [callback])

<!--
Asynchronously reads the entire contents of a file. Example:
-->

ファイル全体の内容を非同期に読み込みます。例:

    fs.readFile('/etc/passwd', function (err, data) {
      if (err) throw err;
      console.log(data);
    });

<!--
The callback is passed two arguments `(err, data)`, where `data` is the
contents of the file.
-->

コールバックは 2 つの引数が渡される `(err, data)` で、`data` はファイルの内容です。

<!--
If no encoding is specified, then the raw buffer is returned.
-->

エンコーディングが指定されなければ、生のバッファが渡されます。

## fs.readFileSync(filename, [encoding])

<!--
Synchronous version of `fs.readFile`. Returns the contents of the `filename`.
-->

同期版の `fs.readFile`。`filename` の内容を返します。

<!--
If `encoding` is specified then this function returns a string. Otherwise it
returns a buffer.
-->

`encoding` が指定されるとこの関数は文字列を返します。
そうでなければバッファを返します。

## fs.writeFile(filename, data, [encoding], [callback])

<!--
Asynchronously writes data to a file, replacing the file if it already exists.
`data` can be a string or a buffer. The `encoding` argument is ignored if
`data` is a buffer. It defaults to `'utf8'`.
-->

非同期にデータをファイルに書き込みます。
ファイルが既に存在する場合は置き換えられます。
`data` は文字列またはバッファです。
`data` がバッファの場合、`encoding` は無視されます。
デフォルトは `'utf8'` です。

<!--
Example:
-->

例:

    fs.writeFile('message.txt', 'Hello Node', function (err) {
      if (err) throw err;
      console.log('It\'s saved!');
    });

## fs.writeFileSync(filename, data, [encoding])

<!--
The synchronous version of `fs.writeFile`.
-->

同期版の `fs.writeFile`。

## fs.appendFile(filename, data, encoding='utf8', [callback])

<!--
Asynchronously append data to a file, creating the file if it not yet exists.
`data` can be a string or a buffer. The `encoding` argument is ignored if
`data` is a buffer.
-->

非同期にデータをファイルに追加します。
ファイルが存在しなければ作成されます。
`data` は文字列またはバッファです。
`data` がバッファの場合、`encoding` は無視されます。
デフォルトは `'utf8'` です。

<!--
Example:
-->

例:

    fs.appendFile('message.txt', 'data to append', function (err) {
      if (err) throw err;
      console.log('The "data to append" was appended to file!');
    });

## fs.appendFileSync(filename, data, encoding='utf8')

<!--
The synchronous version of `fs.appendFile`.
-->

同期版の `fs.appendFile`。

## fs.watchFile(filename, [options], listener)

    Stability: 2 - Unstable.  Use fs.watch instead, if available.

<!--
Watch for changes on `filename`. The callback `listener` will be called each
time the file is accessed.
-->

`filename` の変更を監視します。コールバックの `listener` はファイルがアクセスされる度に呼び出されます。

<!--
The second argument is optional. The `options` if provided should be an object
containing two members a boolean, `persistent`, and `interval`. `persistent`
indicates whether the process should continue to run as long as files are
being watched. `interval` indicates how often the target should be polled,
in milliseconds. (On Linux systems with inotify, `interval` is ignored.) The
default is `{ persistent: true, interval: 0 }`.
-->

第 2 引数はオプションです．
`options` が与えられる場合、それは boolean の `persistent` と `interval`
の二つのメンバを含むオブジェクトです。
`persistent` はファイルが監視されている間、
プロセスが実行し続けることを示します。
`interval` は対象をポーリングする間隔をミリ秒で示します
(inotify を備えた Linux システムでは `interval` は無視されます)。
デフォルトは `{ persistent: true, interval: 0}` です。

<!--
The `listener` gets two arguments the current stat object and the previous
stat object:
-->

`listener` は現在の状態オブジェクトと前の状態オブジェクトの 2 つの引数を受け取ります:

    fs.watchFile('message.text', function (curr, prev) {
      console.log('the current mtime is: ' + curr.mtime);
      console.log('the previous mtime was: ' + prev.mtime);
    });

<!--
These stat objects are instances of `fs.Stat`.

If you want to be notified when the file was modified, not just accessed
you need to compare `curr.mtime` and `prev.mtime`.
-->

これらの状態オブジェクトは `fs.Stat` のインスタンスです。

もしファイルがアクセスされただけでなく、変更された時の通知が必要であれば、`curr.mtime` と `prev.mtime` を比較する必要があります。

## fs.unwatchFile(filename)

    Stability: 2 - Unstable.  Use fs.watch instead, if available.

<!--
Stop watching for changes on `filename`.
-->

`filename` の変更に対する監視を終了します。

## fs.watch(filename, [options], listener)

    Stability: 2 - Unstable.  Not available on all platforms.

<!--
Watch for changes on `filename`, where `filename` is either a file or a
directory.  The returned object is a [fs.FSWatcher](#fs_class_fs_fswatcher).

The second argument is optional. The `options` if provided should be an object
containing a boolean member `persistent`, which indicates whether the process
should continue to run as long as files are being watched. The default is
`{ persistent: true }`.

The listener callback gets two arguments `(event, filename)`.  `event` is either
'rename' or 'change', and `filename` is the name of the file which triggered
the event.
-->

`filename` の変更を監視します。
`filename` はファイルまたはディレクトリのどちらかです。
戻り値のオブジェクトは [fs.FSWatcher](#fs.FSWatcher) です。

第 2 引数はオプションです。
もし指定されるなら、`options` は boolean の `persistent` プロパティを
持つオブジェクトであるべきです。
`persistent` はファイルが監視されている間、
プロセスが実行し続けることを示します。
デフォルトは `{ persistent: true }` です。

リスナーコールバックは二つの引数 `(event, filename)` を与えられます。
`event` は `'rename'` または `'change'`、そして `filename` はイベントを
引き起こしたファイルの名前です。

### Caveats

<!--type=misc-->

<!--
The `fs.watch` API is not 100% consistent across platforms, and is
unavailable in some situations.
-->

`fs.watch` API はプラットフォーム間で 100% 完全ではありmせんし、
いくつかのシチュエーションで利用不可能です。

#### Availability

<!--type=misc-->

<!--
This feature depends on the underlying operating system providing a way
to be notified of filesystem changes.

* On Linux systems, this uses `inotify`.
* On BSD systems (including OS X), this uses `kqueue`.
* On SunOS systems (including Solaris and SmartOS), this uses `event ports`.
* On Windows systems, this feature depends on `ReadDirectoryChangesW`.

If the underlying functionality is not available for some reason, then
`fs.watch` will not be able to function.  You can still use
`fs.watchFile`, which uses stat polling, but it is slower and less
reliable.
-->

この機能は下層のオペレーティングシステムが提供するファイルシステム変更の
通知に依存します。

* Linux システムでは `inotify` が使われます。
* BSD システム (OS X を含みます) では `kqueue` が使われます。
* SunOS システム (Solaris および SmartOS を含みます) では `event ports`
  が使われます。
* Windows システムでは、この機能は `ReadDirectoryChangesW` に依存します。

何らかの理由で下層の機能が使えない場合、`fs.watch` は使えません。
stat をポーリングする `fs.watchFile` を使うことはできますが、
それは遅くて信頼性はより低くなります。

#### Filename Argument

<!--type=misc-->

<!--
Providing `filename` argument in the callback is not supported
on every platform (currently it's only supported on Linux and Windows).  Even
on supported platforms `filename` is not always guaranteed to be provided.
Therefore, don't assume that `filename` argument is always provided in the
callback, and have some fallback logic if it is null.
-->

コールバックに提供される `filename` 引数は、
全てのプラットフォームでサポートされるわけではありません
(現時点では Linux と Windows でのみサポートされます)。
サポートされるプラットフォームであっても、`filename` が常に提供されることが
保証されているわけではありません。
そのため、コールバックは `filename` 引数が常に提供されると仮定せず、
それが `null` だったときの代替手段を持つべきです。

    fs.watch('somedir', function (event, filename) {
      console.log('event is: ' + event);
      if (filename) {
        console.log('filename provided: ' + filename);
      } else {
        console.log('filename not provided');
      }
    });

## fs.exists(p, [callback])

<!--
Test whether or not the given path exists by checking with the file system.
Then call the `callback` argument with either true or false.  Example:
-->

与えられたパスがファイルシステム上に存在するかどうか検査します。
そして引数の `callback` を真か偽か検査の結果とともに呼び出します。
例:

    fs.exists('/etc/passwd', function (exists) {
      util.debug(exists ? "it's there" : "no passwd!");
    });


## fs.existsSync(p)

<!--
Synchronous version of `fs.exists`.
-->

同期版の `fs.exists` です。

## Class: fs.Stats

<!--
Objects returned from `fs.stat()`, `fs.lstat()` and `fs.fstat()` and their
synchronous counterparts are of this type.
-->

`fs.stat()`、`fs.lstat()`、`fs.fstat()`、そしてそれらの同期版 から返される
オブジェクトはこの型です。

<!--
 - `stats.isFile()`
 - `stats.isDirectory()`
 - `stats.isBlockDevice()`
 - `stats.isCharacterDevice()`
 - `stats.isSymbolicLink()` (only valid with  `fs.lstat()`)
 - `stats.isFIFO()`
 - `stats.isSocket()`
-->

 - `stats.isFile()`
 - `stats.isDirectory()`
 - `stats.isBlockDevice()`
 - `stats.isCharacterDevice()`
 - `stats.isSymbolicLink()` (`fs.lstat()` でのみ有効)
 - `stats.isFIFO()`
 - `stats.isSocket()`

<!--
For a regular file `util.inspect(stats)` would return a string very
similar to this:
-->

`util.inspect(stats)` は通常のファイルに対して次のような文字列を返します。

    { dev: 2114,
      ino: 48064969,
      mode: 33188,
      nlink: 1,
      uid: 85,
      gid: 100,
      rdev: 0,
      size: 527,
      blksize: 4096,
      blocks: 8,
      atime: Mon, 10 Oct 2011 23:24:11 GMT,
      mtime: Mon, 10 Oct 2011 23:24:11 GMT,
      ctime: Mon, 10 Oct 2011 23:24:11 GMT }

<!--
Please note that `atime`, `mtime` and `ctime` are instances
of [Date][MDN-Date] object and to compare the values of
these objects you should use appropriate methods. For most
general uses [getTime()][MDN-Date-getTime] will return
the number of milliseconds elapsed since _1 January 1970
00:00:00 UTC_ and this integer should be sufficient for
any comparison, however there additional methods which can
be used for displaying fuzzy information. More details can
be found in the [MDN JavaScript Reference][MDN-Date] page.
-->

`atime`、`mtime`、そして `ctime` は [Date][MDN-Date] オブジェクトであり、
その値を比較するには適切な方法があるということに注意してください。
もっとも一般的に使われる [getTime()][MDN-Date-getTime] は _1970 年 1 月
1 日_からの経過時間をミリ秒単位で返します。
それは比較には十分ですが、曖昧な情報を表示するには別の方法を使ってください。
より詳しい情報は [MDN JavaScript Reference][MDN-Date] で探すことができます。

[MDN-Date]: https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Date
[MDN-Date-getTime]: https://developer.mozilla.org/en/JavaScript/Reference/Global_Objects/Date/getTime


## fs.createReadStream(path, [options])

<!--
Returns a new ReadStream object (See `Readable Stream`).
-->

新しい ReadStream オブジェクトを返します (`Readable Stream` を参照してください)。

<!--
`options` is an object with the following defaults:
-->

`options` は以下のデフォルト値を持つオブジェクトです:

    { flags: 'r',
      encoding: null,
      fd: null,
      mode: 0666,
      bufferSize: 64 * 1024
    }

<!--
`options` can include `start` and `end` values to read a range of bytes from
the file instead of the entire file.  Both `start` and `end` are inclusive and
start at 0.
-->

ファイル全体を読み込む代わりに一部の範囲を読み込むため、
`options` に `start` および `end` を含めることができます。
`start` と `end` はどちらも包含的で0から始まります。

<!--
An example to read the last 10 bytes of a file which is 100 bytes long:
-->

100 バイトの長さを持つファイルの最後の 10 バイトを読み込む例:

    fs.createReadStream('sample.txt', {start: 90, end: 99});


## Class: fs.ReadStream

<!--
`ReadStream` is a [Readable Stream](stream.html#readable_stream).
-->

`ReadStream` は [Readable Stream](stream.html#readable_Stream) です。

### Event: 'open'

<!--
* `fd` {Integer} file descriptor used by the ReadStream.

Emitted when the ReadStream's file is opened.
-->

* `fd` {Integer} ReadStream で使われる ファイル記述子。

ReadStream のファイルがオープンされた場合に生成されます。


## fs.createWriteStream(path, [options])

<!--
Returns a new WriteStream object (See `Writable Stream`).
-->

新しい WriteStream オブジェクトを返します (`Writable Stream` を参照してください)。

<!--
`options` is an object with the following defaults:
-->

`options` は以下のデフォルト値を持つオブジェクトです:

    { flags: 'w',
      encoding: null,
      mode: 0666 }

<!--
`options` may also include a `start` option to allow writing data at
some position past the beginning of the file.  Modifying a file rather
than replacing it may require a `flags` mode of `r+` rather than the
default mode `w`.
-->

`options` にはデータをファイルのどの位置に書き込むかを指定する
`start` を含めることができます。
ファイルを置換するのではなく変更する場合は、 `flags` にデフォルトの
`w` ではなく `r+` が必要となります。

## fs.WriteStream

<!--
`WriteStream` is a [Writable Stream](stream.html#writable_stream).
-->

`WriteStream` は [Writable Stream](stream.html#writable_Stream) です。

### Event: 'open'

<!--
* `fd` {Integer} file descriptor used by the WriteStream.

Emitted when the WriteStream's file is opened.
-->

* `fd` {Integer} WriteStream で使われる ファイル記述子。

WriteStream のファイルがオープンされた場合に生成されます。

### file.bytesWritten

<!--
The number of bytes written so far. Does not include data that is still queued
for writing.
-->

これまでに書き込まれたバイト数。
書き込みがキューイングされたままのデータは含まれません。

## Class: fs.FSWatcher

<!--
Objects returned from `fs.watch()` are of this type.
-->
`fs.watch()` が返すオブジェクトはこの型です。

### watcher.close()

<!--
Stop watching for changes on the given `fs.FSWatcher`.
-->
`fs.FSWatcher` に与えられたファイルの監視を終了します。

### Event: 'change'

<!--
* `event` {String} The type of fs change
* `filename` {String} The filename that changed (if relevant/available)
-->

* `event` {String} ファイルシステム変更の種類です。
* `filename` {String} 変更されたファイル名です (もし利用可能であれば)。

<!--
Emitted when something changes in a watched directory or file.
See more details in [fs.watch](#fs.watch).
-->

監視しているファイルまたはディレクトリに変更があると生成されます。
詳しくは [fs.watch](#fs.watch) を参照してください。

### Event: 'error'

* `error` {Error object}

<!--
Emitted when an error occurs.
-->

エラーが発生すると生成されます。
