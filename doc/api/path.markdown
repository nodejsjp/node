## Path

<!--

This module contains utilities for handling and transforming file
paths.  Almost all these methods perform only string transformations.
The file system is not consulted to check whether paths are valid.

`path.exists` and `path.existsSync` are the exceptions, and should
logically be found in the fs module as they do access the file system.

Use `require('path')` to use this module.  The following methods are provided:

-->
このモジュールはファイルのパスに対する処理や変換を行うユーティリティを含みます。
ほとんどのメソッドは文字列の変換だけを行います。
パスが正しいか検証するためにファイルシステムに尋ねることはありません。

例外は `path.exists` および `path.existsSync` で、
これらはファイルシステムにアクセスするため、
論理的には fs モジュールにあるべきです。

このモジュールを利用するには`require('path')`を呼び出してください。
このモジュールは以下のメソッドを提供します。

### path.normalize(p)

<!--

Normalize a string path, taking care of `'..'` and `'.'` parts.

->
文字列によるパスを正規化します。`'..'` と `'.'` の要素には注意してください。

<!--

When multiple slashes are found, they're replaced by a single one;
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

### path.relative(from, to)

<!--
Solve the relative path from `from` to `to`.
-->
`from` から `to` への相対パスを解決します。

<!--
At times we have two absolute paths, and we need to derive the relative
path from one to the other.  This is actually the reverse transform of
`path.resolve`, which means we see that:
-->
二つの絶対パスがあり、一方から他方への相対パスを得なければならない場合があります。
これは実際のところ、`path.resolve()` とは逆の変換です。
それは以下を意味します:



    path.resolve(from, path.relative(from, to)) == path.resolve(to)

<!--
Examples:
-->
例:

    path.relative('C:\\orandea\\test\\aaa', 'C:\\orandea\\impl\\bbb')
    // returns
    '..\\..\\impl\\bbb'

    path.relative('/data/orandea/test/aaa', '/data/orandea/impl/bbb')
    // returns
    '../../impl/bbb'

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

Return the extension of the path, from the last '.' to end of string
in the last portion of the path.  If there is no '.' in the last portion
of the path or the first character of it is '.', then it returns
an empty string.  Examples:

-->
パスの最後の要素について、最後の '.' から文字列の最後までのパスの拡張子を返します。
最後の要素に '.' が含まれていなかった場合、もしくは '.' が最初の文字だった場合は、空の文字列を返します。
例:

    path.extname('index.html')
    // returns
    '.html'

    path.extname('index.')
    // returns
    '.'

    path.extname('index')
    // returns
    ''

### path.exists(p, [callback])

<!--

Test whether or not the given path exists by checking with the file system.
Then call the `callback` argument with either true or false.  Example:

-->
与えられたパスがファイルシステム上に存在するかどうか検査します。
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
