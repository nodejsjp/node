## Query String

<!--

This module provides utilities for dealing with query strings.
It provides the following methods:

-->
このモジュールはクエリ文字列を処理するユーティリティを提供します。 以下のメソッドから成ります:

### querystring.stringify(obj, sep='&', eq='=')

<!--

Serialize an object to a query string.
Optionally override the default separator and assignment characters.

-->
クエリオブジェクトを文字列へ直列化します。オプションとしてデフォルトの区切り文字と代入文字を上書き指定できます。

<!--

Example:

-->
例:

    querystring.stringify({foo: 'bar'})
    // returns
    'foo=bar'

    querystring.stringify({foo: 'bar', baz: 'bob'}, ';', ':')
    // returns
    'foo:bar;baz:bob'

### querystring.parse(str, sep='&', eq='=')

<!--

Deserialize a query string to an object.
Optionally override the default separator and assignment characters.

-->
クエリ文字列をオブジェクトに復元します。オプションとしてデフォルトの区切り文字と代入文字を上書き指定できます。

<!--

Example:

-->
例:

    querystring.parse('a=b&b=c')
    // returns
    { a: 'b', b: 'c' }

### querystring.escape

<!--

The escape function used by `querystring.stringify`,
provided so that it could be overridden if necessary.

-->
escape 関数は `querystring.stringify` で使用されていて、必要な場合にオーバーライドできるよう提供されています。

### querystring.unescape

<!--

The unescape function used by `querystring.parse`,
provided so that it could be overridden if necessary.

-->
unescape関数は `querystring.parse` で使用されていて、必要な場合にオーバーライドできるよう提供されています。
