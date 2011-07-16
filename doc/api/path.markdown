## Path

<!--

This module contains utilities for dealing with file paths.  Use
`require('path')` to use it.  It provides the following methods:

-->
このモジュールはファイルパスを扱うユーティリティを含みます。
利用するには`require('path')`を呼び出してください。
このモジュールは以下のメソッドを提供します。

### path.normalize(p)

<!--

Normalize a string path, taking care of `'..'` and `'.'` parts.

-->
文字列によるパスを正規化します。`'..'` と `'.'` の要素には注意してください。

<!--

When multiple slashes are found, they're replaces by a single one;
when the path contains a trailing slash, it is preserved.
On windows backslashes are used. 

-->
複数のスラッシュが見つかると、それらは一つに置換されます;
パスの最後にスラッシュが含まれていると、それは維持されます。
Windows ではバックスラッシュが使われます。

<!--

Example:

-->
例:

    path.normalize('/foo/bar//baz/asdf/quux/..')
    // returns
    '/foo/bar/baz/asdf'

### path.join([path1], [path2], [...])

<!--

Join all arguments together and normalize the resulting path.
Non-string arguments are ignored.

-->
全ての引数を一つに結合し、結果として得られるパスを正規化します。
文字列でない引数は無視されます。

<!--

Example:

-->
例:

    path.join('/foo', 'bar', 'baz/asdf', 'quux', '..')
    // returns
    '/foo/bar/baz/asdf'

    path.join('foo', {}, 'bar')
    // returns
    'foo/bar'

### path.resolve([from ...], to)

<!--

Resolves `to` to an absolute path.

-->
`to` の絶対パスを解決します。

<!--

If `to` isn't already absolute `from` arguments are prepended in right to left
order, until an absolute path is found. If after using all `from` paths still
no absolute path is found, the current working directory is used as well. The
resulting path is normalized, and trailing slashes are removed unless the path 
gets resolved to the root directory. Non-string arguments are ignored.
-->
もし `to` が既に絶対パスでなければ、絶対パスが見つかるまで `from` 引数を右から左の順で先頭に加えます。
全ての `from` を加えた後、パスがまだ絶対パスでなければ、カレントワーキングディレクトリが同様に使われます。
結果のパスは正規化され、解決されたパスがルートディレクトリでない限り末尾のスラッシュは削除されます。
文字列でない引数は無視されます。

<!--

Another way to think of it is as a sequence of `cd` commands in a shell.

-->
それはシェルにおける `cd` コマンドの列だと考えることができます。

<!--

Examples:

-->
例:

    path.resolve('foo/bar', '/tmp/file/', '..', 'a/../subfile')

<!--

Is similar to:

-->
これは以下と同様です。

    cd foo/bar
    cd /tmp/file/
    cd ..
    cd a/../subfile
    pwd

<!--

The difference is that the different paths don't need to exist and may also be
files.

-->
違いは、それぞれのパスが必ずしも存在する必要がないことと、ファイルでも構わないことです。

<!--

Examples:

-->
例:

    path.resolve('/foo/bar', './baz')
    // returns
    '/foo/bar/baz'

    path.resolve('/foo/bar', '/tmp/file/')
    // returns
    '/tmp/file'

    path.resolve('wwwroot', 'static_files/png/', '../gif/image.gif')
    // if currently in /home/myself/node, it returns
    '/home/myself/node/wwwroot/static_files/gif/image.gif'

### path.dirname(p)

<!--

Return the directory name of a path.  Similar to the Unix `dirname` command.

-->
パスに含まれるディレクトリ名を返します。Unixの `dirname` コマンドと同様です。

<!--

Example:

-->
例:

    path.dirname('/foo/bar/baz/asdf/quux')
    // returns
    '/foo/bar/baz/asdf'

### path.basename(p, [ext])

<!--

Return the last portion of a path.  Similar to the Unix `basename` command.

-->
パスの最後の要素を返します。Unixの `basename` コマンドと同様です。

<!--

Example:

-->
例:

    path.basename('/foo/bar/baz/asdf/quux.html')
    // returns
    'quux.html'

    path.basename('/foo/bar/baz/asdf/quux.html', '.html')
    // returns
    'quux'

### path.extname(p)

<!--

Return the extension of the path.  Everything after the last '.' in the last portion
of the path.  If there is no '.' in the last portion of the path or the only '.' is
the first character, then it returns an empty string.  Examples:

-->
パスの拡張子を返します。
パスの最後の要素について、最後の '.' から後にある文字列が対象になります。
最後の要素に '.' が含まれていなかった場合、もしくは '.' が最初の文字だった場合は、空の文字列を返します。
例:

    path.extname('index.html')
    // returns
    '.html'

    path.extname('index')
    // returns
    ''

### path.exists(p, [callback])

<!--

Test whether or not the given path exists.  Then, call the `callback` argument
with either true or false. Example:

-->
与えられたパスが存在するかどうか検査します。
そして引数の `callback` を真か偽か検査の結果とともに呼び出します。
例:

    path.exists('/etc/passwd', function (exists) {
      util.debug(exists ? "it's there" : "no passwd!");
    });


### path.existsSync(p)

<!--

Synchronous version of `path.exists`.

-->
同期版の `path.exists` です。
