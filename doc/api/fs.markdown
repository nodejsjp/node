## File System

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

-->
非同期の形式は常に最後の引数として完了コールバックを受け取ります。
引数として渡される完了コールバックはメソッドに依存しますが、
最初の引数は常に例外のために予約されています。
操作が成功で完了すると最初の引数は `null` または `undefined` となります

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

### fs.rename(path1, path2, [callback])

<!--

Asynchronous rename(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の rename(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.renameSync(path1, path2)

<!--

Synchronous rename(2).

-->
同期の rename(2)。

### fs.truncate(fd, len, [callback])

<!--

Asynchronous ftruncate(2). No arguments other than a possible exception are
given to the completion callback.

-->
非同期の ftruncate(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.truncateSync(fd, len)

<!--

Synchronous ftruncate(2).

-->
同期の ftruncate(2)。

### fs.chown(path, mode, [callback])

<!--

Asycnronous chown(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の chown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.chownSync(path, mode)

<!--

Synchronous chown(2).

-->
同期の chown(2)。

### fs.fchown(path, mode, [callback])

<!--

Asycnronous fchown(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の fchown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.fchownSync(path, mode)

<!--

Synchronous fchown(2).

-->
同期の fchown(2)。

### fs.lchown(path, mode, [callback])

<!--

Asycnronous lchown(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の lchown(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.lchownSync(path, mode)

<!--

Synchronous lchown(2).

-->
同期の lchown(2)。

### fs.chmod(path, mode, [callback])

<!--

Asynchronous chmod(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の chmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.chmodSync(path, mode)

<!--

Synchronous chmod(2).

-->
同期の chmod(2)。
  
### fs.fchmod(fd, mode, [callback])

<!--

Asynchronous fchmod(2). No arguments other than a possible exception
are given to the completion callback.

-->
非同期の fchmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.fchmodSync(path, mode)

<!--

Synchronous fchmod(2).

-->
同期の fchmod(2)。

### fs.lchmod(fd, mode, [callback])

<!--

Asynchronous lchmod(2). No arguments other than a possible exception
are given to the completion callback.

-->
非同期の lchmod(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.lchmodSync(path, mode)

<!--

Synchronous lchmod(2).

-->
同期の lchmod(2)。

### fs.stat(path, [callback])

<!--

Asynchronous stat(2). The callback gets two arguments `(err, stats)` where
`stats` is a [`fs.Stats`](#fs.Stats) object. It looks like this:

-->
非同期の stat(2)。コールバックは 2 つの引数を受け取る `(err, stats)`で、
`stats` は `fs.Stats` オブジェクトです。次のようになります。

    { dev: 2049,
      ino: 305352,
      mode: 16877,
      nlink: 12,
      uid: 1000,
      gid: 1000,
      rdev: 0,
      size: 4096,
      blksize: 4096,
      blocks: 8,
      atime: '2009-06-29T11:11:55Z',
      mtime: '2009-06-29T11:11:40Z',
      ctime: '2009-06-29T11:11:40Z' }

<!--

See the [fs.Stats](#fs.Stats) section below for more information.

-->
より詳しくは後述の [fs.Stats](#fs.Stats) の節を参照してください。

### fs.lstat(path, [callback])

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

### fs.fstat(fd, [callback])

<!--

Asynchronous fstat(2). The callback gets two arguments `(err, stats)` where
`stats` is a `fs.Stats` object. `fstat()` is identical to `stat()`, except that
the file to be stat-ed is specified by the file descriptor `fd`.

-->
非同期の fstat(2)。コールバックは 2 つの引数を受け取る `(err, stats)` で、
`stats` は `fs.Stats` オブジェクトです。
状態を取得するファイルをファイル記述子 `fd` で指定することを除いて、
`fstat()` は `stat()` と同じです。

### fs.statSync(path)

<!--

Synchronous stat(2). Returns an instance of `fs.Stats`.

-->
同期の stat(2)。`fs.Stats` のインスタンスを返します。

### fs.lstatSync(path)

<!--

Synchronous lstat(2). Returns an instance of `fs.Stats`.

-->
同期の lstat(2)。`fs.Stats` のインスタンスを返します。

### fs.fstatSync(fd)

<!--

Synchronous fstat(2). Returns an instance of `fs.Stats`.

-->
同期の fstat(2)。`fs.Stats` のインスタンスを返します。

### fs.link(srcpath, dstpath, [callback])

<!--

Asynchronous link(2). No arguments other than a possible exception are given to
the completion callback.

-->
非同期の link(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.linkSync(srcpath, dstpath)

<!--

Synchronous link(2).

-->
同期の link(2)。

### fs.symlink(linkdata, path, [callback])

<!--

Asynchronous symlink(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の symlink(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.symlinkSync(linkdata, path)

<!--

Synchronous symlink(2).

-->
同期の symlink(2)。

### fs.readlink(path, [callback])

<!--

Asynchronous readlink(2). The callback gets two arguments `(err,
resolvedPath)`.

-->
非同期の readlink(2)。コールバックは 2 つの引数を受け取る `(err, resolvedPath)`です。

### fs.readlinkSync(path)

<!--

Synchronous readlink(2). Returns the resolved path.

-->
同期の readlink(2)。解決されたパスを返します。

### fs.realpath(path, [callback])

<!--

Asynchronous realpath(2).  The callback gets two arguments `(err,
resolvedPath)`.

-->
非同期の realpath(2)。コールバックは 2 つの引数を受け取る `(err, resolvedPath)`です。

### fs.realpathSync(path)

<!--

Synchronous realpath(2). Returns the resolved path.

-->
同期の realpath(2)。解決されたパスを返します。

### fs.unlink(path, [callback])

<!--

Asynchronous unlink(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の unlink(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.unlinkSync(path)

<!--

Synchronous unlink(2).

-->
同期の unlink(2)。

### fs.rmdir(path, [callback])

<!--

Asynchronous rmdir(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の rmdir(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.rmdirSync(path)

<!--

Synchronous rmdir(2).

-->
同期の rmdir(2)。

### fs.mkdir(path, mode, [callback])

<!--

Asynchronous mkdir(2). No arguments other than a possible exception are given
to the completion callback.

-->
非同期の mkdir(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.mkdirSync(path, mode)

<!--

Synchronous mkdir(2).

-->
同期の mkdir(2)。

### fs.readdir(path, [callback])

<!--

Asynchronous readdir(3).  Reads the contents of a directory.
The callback gets two arguments `(err, files)` where `files` is an array of
the names of the files in the directory excluding `'.'` and `'..'`.

-->
非同期の readdir(3)。ディレクトリの内容を読み込みます。
コールバックは 2 つの引数を受け取る `(err, files)`で、
`files` は `'.'` と `'..'` を除くディレクトリ内のファイル名の配列です。

### fs.readdirSync(path)

<!--

Synchronous readdir(3). Returns an array of filenames excluding `'.'` and
`'..'`.

-->
同期の readdir(3)。`'.'` と `'..'` を除くディレクトリ内のファイル名の配列を返します。

### fs.close(fd, [callback])

<!--

Asynchronous close(2).  No arguments other than a possible exception are given
to the completion callback.

-->
非同期の close(2)。完了コールバックには発生し得る例外以外に引数が渡されることはありません。

### fs.closeSync(fd)

<!--

Synchronous close(2).

-->
同期の close(2)。

### fs.open(path, flags, [mode], [callback])

<!--

Asynchronous file open. See open(2). `flags` can be:

* `'r'` - Open file for reading.
An exception occurs if the file does not exist.

* `'r+'` - Open file for reading and writing. 
An exception occurs if the file does not exist.

* `'w'` - Open file for writing.
The file is created (if it does not exist) or truncated (if it exists).

* `'w+'` - Open file for reading and writing.
The file is created (if it does not exist) or truncated (if it exists).

* `'a'` - Open file for appending.
The file is created if it does not exist.

* `'a+'` - Open file for reading and appending.
The file is created if it does not exist.

`mode` defaults to `0666`. The callback gets two arguments `(err, fd)`.

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

* `'r+'` - 読み書き両用でオープンします。
ファイルは作成される (存在しない場合) または長さ 0 に切り詰められます
(存在する場合)。

* `'a'` - 追記用でオープンします。
ファイルが存在しない場合は作成されます。

* `'a+'` - 読み込みおよび追記用でオープンします。
ファイルが存在しない場合は作成されます。

`mode` のデフォルトは 0666 です。
コールバックは 2 つの引数を受け取る `(err, fd)`です。

### fs.openSync(path, flags, [mode])

<!--

Synchronous open(2).

-->
同期の open(2)。

### fs.write(fd, buffer, offset, length, position, [callback])

<!--

Write `buffer` to the file specified by `fd`.

-->
`fd` で指定されたファイルに `buffer` を書き込みます。

<!--

`offset` and `length` determine the part of the buffer to be written.

-->

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
specifies how many _bytes_ were written into `buffer`.

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

### fs.writeSync(fd, buffer, offset, length, position)

<!--

Synchronous version of buffer-based `fs.write()`. Returns the number of bytes
written.

-->
同期版のバッファに基づく `fs.write()`。書き込まれたバイト数を返します。

### fs.writeSync(fd, str, position, encoding='utf8')

<!--

Synchronous version of string-based `fs.write()`. Returns the number of bytes
written.

-->
同期版の文字列に基づく `fs.write()`。書き込まれたバイト数を返します。

### fs.read(fd, buffer, offset, length, position, [callback])

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

### fs.readSync(fd, buffer, offset, length, position)

<!--

Synchronous version of buffer-based `fs.read`. Returns the number of
`bytesRead`.

-->
同期版のバッファに基づく `fs.read`。`bytesRead` の数を返します。

### fs.readSync(fd, length, position, encoding)

<!--

Synchronous version of string-based `fs.read`. Returns the number of
`bytesRead`.

-->
同期版の文字列に基づく `fs.read`。`bytesRead` の数を返します。

### fs.readFile(filename, [encoding], [callback])

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


### fs.readFileSync(filename, [encoding])

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


### fs.writeFile(filename, data, encoding='utf8', [callback])

<!--

Asynchronously writes data to a file, replacing the file if it already exists.
`data` can be a string or a buffer.

-->
非同期にデータをファイルに書き込みます。
ファイルが既に存在する場合は置き換えられます。
`data` は文字列またはバッファです。

<!--

Example:

-->
例:

    fs.writeFile('message.txt', 'Hello Node', function (err) {
      if (err) throw err;
      console.log('It\'s saved!');
    });

### fs.writeFileSync(filename, data, encoding='utf8')

<!--

The synchronous version of `fs.writeFile`.

-->
同期版の `fs.writeFile`。

### fs.watchFile(filename, [options], listener)

<!--

Watch for changes on `filename`. The callback `listener` will be called each
time the file is accessed.

-->
`filename` の変更を監視します。コールバックの `listener` はファイルがアクセスされる度に呼び出されます。

<!--

The second argument is optional. The `options` if provided should be an object
containing two members a boolean, `persistent`, and `interval`, a polling
value in milliseconds. The default is `{ persistent: true, interval: 0 }`.

-->
第 2 引数はオプションです．
`options` が与えられる場合、それは `persistent` とポーリング間隔をミリ秒で表す `interval` の二つの boolean メンバを含むオブジェクトです。
デフォルトは `{ persistent: true, interval: 0}` です。

<!--

The `listener` gets two arguments the current stat object and the previous
stat object:

-->
`listener` は現在の状態オブジェクトと前の状態オブジェクトの 2 つの引数を受け取ります:

    fs.watchFile(f, function (curr, prev) {
      console.log('the current mtime is: ' + curr.mtime);
      console.log('the previous mtime was: ' + prev.mtime);
    });

<!--

These stat objects are instances of `fs.Stat`.

If you want to be notified when the file was modified, not just accessed
you need to compare `curr.mtime` and `prev.mtime.
-->

これらの状態オブジェクトは `fs.Stat` のインスタンスです。

もしファイルがアクセスされただけでなく、変更された時の通知が必要であれば、`curr.mtime` と `prev.mtime` を比較する必要があります。

### fs.unwatchFile(filename)

<!--

Stop watching for changes on `filename`.

-->
`filename` の変更に対する監視を終了します。

## fs.Stats

<!--

Objects returned from `fs.stat()` and `fs.lstat()` are of this type.

-->
`fs.stat()` と `fs.lstat()` から返されるオブジェクトはこの型です。

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
 - `stats.isSymbolicLink()` (`fs.lstat()`でのみ有効)
 - `stats.isFIFO()`
 - `stats.isSocket()`

## fs.ReadStream

<!--

`ReadStream` is a `Readable Stream`.

-->
`ReadStream` は `Readable Stream` です。

### Event: 'open'

`function (fd) { }`

<!--

 `fd` is the file descriptor used by the ReadStream.

-->
`fd` は ReadStream に使われているファイル記述子です。

### fs.createReadStream(path, [options])

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


## fs.WriteStream

<!--

`WriteStream` is a `Writable Stream`.

-->
`WriteStream` は `Writable Stream` です。

### Event: 'open'

`function (fd) { }`

<!--

 `fd` is the file descriptor used by the WriteStream.

-->
`fd` は WriteStream に使われているファイル記述子です。

### fs.createWriteStream(path, [options])

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
