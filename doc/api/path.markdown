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

-->
全ての引数を一つに結合し、結果として得られるパスを正規化します。

<!--

Example:

-->
例:

    node> require('path').join(
    ...   '/foo', 'bar', 'baz/asdf', 'quux', '..')
    '/foo/bar/baz/asdf'

### path.resolve([from ...], to)

<!--

Resolves `to` to an absolute path name and normalizes it.

-->
`to` の絶対パスを解決して正規化します。

<!--
One ore more `from` arguments may be provided to specify the the starting
point from where the path will be resolved. `resolve` will prepend `from`
arguments from right to left until an absolute path is found. If no `from`
arguments are specified, or after prepending them still no absolute path is
found, the current working directory will be prepended eventually.
-->
パスを解決する際の基準点を指定するために一つ以上の `from` 引数を与えることができます。
 `resolve()` は絶対パスが見つかるまで `from` 引数を右から左へ先頭に追加します。
`from` 引数が指定されなかった場合や、絶対パスが見つからなかった場合は、
最終的に現在の作業ディレクトリを先頭に追加されます。

<!--

Trailing slashes are removed unless the path gets resolved to the root
directory.

-->
解決した結果がルートディレクトリの場合を除いて、末尾のスラッシュは削除されます。

<!--

Examples:

-->
例:

    path.resolve('index.html')
    // returns
    '/home/tank/index.html'

    path.resolve('/foo/bar', './baz')
    // returns
    '/foo/baz/baz'

    path.resolve('/foo/bar', '/tmp/file/')
    // returns
    '/tmp/file'

    path.resolve('wwwroot', 'static_files/png/', '../gif/image.gif')
    // returns
    '/home/tank/wwwroot/static_files/gif/image.gif'

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
