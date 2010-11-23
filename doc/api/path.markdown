## Path

<!--
This module contains utilities for dealing with file paths.  Use
`require('path')` to use it.  It provides the following methods:
-->
このモジュールはファイルパスを扱うユーティリティを含みます。
利用するには`require('path')`を呼び出してください。
このモジュールは以下のメソッドを提供します。

### path.join([path1], [path2], [...])

<!--
Join all arguments together and resolve the resulting path.
-->
全ての引数を一つに結合し、結果として得られるパスを決定します。

<!--
Example:
-->
例:

    node> require('path').join(
    ...   '/foo', 'bar', 'baz/asdf', 'quux', '..')
    '/foo/bar/baz/asdf'

### path.normalizeArray(arr)

<!--
Normalize an array of path parts, taking care of `'..'` and `'.'` parts.
-->
パスの要素の配列を正規化します。`'..'` と `'.'` の要素には注意してください。

<!--
Example:
-->
例:

    path.normalizeArray(['', 
      'foo', 'bar', 'baz', 'asdf', 'quux', '..'])
    // returns
    [ '', 'foo', 'bar', 'baz', 'asdf' ]

### path.normalize(p)

<!--
Normalize a string path, taking care of `'..'` and `'.'` parts.
-->
文字列によるパスを正規化します。`'..'` と `'.'` の要素には注意してください。

<!--
Example:
-->
例:

    path.normalize('/foo/bar/baz/asdf/quux/..')
    // returns
    '/foo/bar/baz/asdf'

### path.dirname(p)

<!--
Return the directory name of a path.  Similar to the Unix `dirname` command.
-->
パスに含まれるディレクトリ名を返します。Unixの `dirname` コマンドと同様です。

<!--
Example:
-->

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
Test whether or not the given path exists.  Then, call the `callback` argument with either true or false.  Example:
-->
与えられたパスが存在するかどうか検査します。
そして引数の `callback` を真か偽か検査の結果とともに呼び出します。
例:

    path.exists('/etc/passwd', function (exists) {
      util.debug(exists ? "it's there" : "no passwd!");
    });
