# URL

    Stability: 3 - Stable

<!--
This module has utilities for URL resolution and parsing.
Call `require('url')` to use it.
-->

このモジュールはURLの解決や解析の為のユーティリティを持ちます。
利用するには `require('url')` を呼び出してください。

<!--
Parsed URL objects have some or all of the following fields, depending on
whether or not they exist in the URL string. Any parts that are not in the URL
string will not be in the parsed object. Examples are shown for the URL
-->

解析されたURLオブジェクトは、URL文字列の中に存在するかどうかに応じて
次に示すフィールドをいくつかもしくは全てを持ちます。
URL文字列に含まれないフィールドは解析結果のオブジェクトに含まれません。
次のURLで例を示します。

`'http://user:pass@host.com:8080/p/a/t/h?query=string#hash'`

<!--
* `href`: The full URL that was originally parsed. Both the protocol and host are lowercased.

    Example: `'http://user:pass@host.com:8080/p/a/t/h?query=string#hash'`

* `protocol`: The request protocol, lowercased.

    Example: `'http:'`

* `host`: The full lowercased host portion of the URL, including port
  information.

    Example: `'host.com:8080'`

* `auth`: The authentication information portion of a URL.

    Example: `'user:pass'`

* `hostname`: Just the lowercased hostname portion of the host.

    Example: `'host.com'`

* `port`: The port number portion of the host.

    Example: `'8080'`

* `pathname`: The path section of the URL, that comes after the host and
  before the query, including the initial slash if present.

    Example: `'/p/a/t/h'`

* `search`: The 'query string' portion of the URL, including the leading
  question mark.

    Example: `'?query=string'`

* `path`: Concatenation of `pathname` and `search`.

    Example: `'/p/a/t/h?query=string'`

* `query`: Either the 'params' portion of the query string, or a
  querystring-parsed object.

    Example: `'query=string'` or `{'query':'string'}`

* `hash`: The 'fragment' portion of the URL including the pound-sign.

    Example: `'#hash'`
-->

* `href`: 解析する前の完全な URL。protocol と host はどちらも小文字化されます。
  
    例: `'http://user:pass@host.com:8080/p/a/t/h?query=string#hash'`

* `protocol`: リクエストのプロトコル。小文字化されます。
  
    例: `'http:'`

* `host`: URL の完全で小文字化されたホスト情報。ポート番号を含みます。
  
    例: `'host.com:8080'`

* `auth`: URL の認証情報。
  
    例: `'user:pass'`

* `hostname`: ホスト情報の中の小文字化されたホスト名。

    例: `'host.com'`

* `port`: ホスト情報の中のポート番号。

    例: `'8080'`

* `pathname`: URL のパス部分。ホスト情報からクエリまでの間に位置し、
  最初にスラッシュが存在する場合はそれも含みます。

    例: `'/p/a/t/h'`

* `search`: URL のクエリ文字列。先頭の ? マークも含みます。

    例: `'?query=string'`

* `path`: `pathname` と `search` を連結した文字列。

    例: `'/p/a/t/h?query=string'`

* `query`: クエリの変数部分の文字列、もしくはクエリ文字列を解析した
  オブジェクト。

    例: `'query=string'` or `{'query':'string'}`

* `hash`: URL の # マークを含む部分。

    例: `'#hash'`

<!--
The following methods are provided by the URL module:
-->

以下のメソッドはURLモジュールにより提供されます:

## url.parse(urlStr, [parseQueryString], [slashesDenoteHost])

<!--
Take a URL string, and return an object.

Pass `true` as the second argument to also parse
the query string using the `querystring` module.
Defaults to `false`.

Pass `true` as the third argument to treat `//foo/bar` as
`{ host: 'foo', pathname: '/bar' }` rather than
`{ pathname: '//foo/bar' }`. Defaults to `false`.
-->

URL文字列を引数に取り、解析結果のオブジェクトを返します。

`querystring` モジュールを使ってクエリ文字列も解析したい場合は、
第 2 引数に `true` を渡してください。
デフォルトは `false` です。

`//foo/bar` を `{ pathname: '//foo/bar' }` ではなく
`{ host: 'foo', pathname: '/bar' }` としたい場合は、
第 3 引数に `true` を渡してください。
デフォルトは `false` です。

## url.format(urlObj)

<!--
Take a parsed URL object, and return a formatted URL string.
-->

URL オブジェクトを引数に取り、フォーマットした URL 文字列を返します。

<!--
* `href` will be ignored.
* `protocol`is treated the same with or without the trailing `:` (colon).
  * The protocols `http`, `https`, `ftp`, `gopher`, `file` will be
    postfixed with `://` (colon-slash-slash).
  * All other protocols `mailto`, `xmpp`, `aim`, `sftp`, `foo`, etc will
    be postfixed with `:` (colon)
* `auth` will be used if present.
* `hostname` will only be used if `host` is absent.
* `port` will only be used if `host` is absent.
* `host` will be used in place of `hostname` and `port`
* `pathname` is treated the same with or without the leading `/` (slash)
* `search` will be used in place of `query`
* `query` (object; see `querystring`) will only be used if `search` is absent.
* `search` is treated the same with or without the leading `?` (question mark)
* `hash` is treated the same with or without the leading `#` (pound sign, anchor)
-->

* `href` は無視されます。
* `protocol` の末尾に `:` (コロン) があってもなくても同じように扱われます。
  * `http`、`https`、`ftp`、`gopher`、`file` は末尾に `://`
    (コロン、スラッシュ、スラッシュ) が付けられます。
  * `mailto`、`xmpp`、`aim`、`sftp`、`foo` など、その他のプロトコルは末尾に `:`
    (コロン) が付けられます。
* `auth` が与えられると使われます。
* `hostname` は `host` が与えられなかった場合だけ使われます。
* `port` は `host` が与えられなかった場合だけ使われます。
* `host` は `hostname`、`port` の位置で使われます。
* `pathname` の先頭に `/` (スラッシュ) があってもなくても同じように扱われます。
* `search` は `query` の位置で使われます。
* `query` (文字列ではなくオブジェクトです; `querystring` を参照してください) は `search` が与えられなかった場合だけ使われます。
* `search` の先頭に `?` (クエスチョンマーク) があってもなくても同じように扱われます。
* `hash` の先頭に `#` (シャープ, アンカー) があってもなくても同じように扱われます。

## url.resolve(from, to)

<!--
Take a base URL, and a href URL, and resolve them as a browser would for
an anchor tag.
-->

ベースとなる URL と相対 URL を引数に取り、ブラウザがアンカータグに対して行うのと同様に URL を解決します。
