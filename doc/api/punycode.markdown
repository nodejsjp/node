# punycode

    Stability: 2 - Unstable

<!--
[Punycode.js](http://mths.be/punycode) is bundled with Node.js v0.6.2+. Use
`require('punycode')` to access it. (To use it with other Node.js versions,
use npm to install the `punycode` module first.)
-->

[Punycode.js](http://mths.be/punycode) は Node.js v0.6.2 以降に
バンドルされています。
アクセスするには `require('punycode')` を使用します
(他のバージョンの Node.js でこれを使用するには、先に npm を使用して `punycode`
モジュールをインストールしてください)。

## punycode.decode(string)

<!--
Converts a Punycode string of ASCII code points to a string of Unicode code
points.
-->

ASCII コードポイントによる Punycode 文字列を Unicode コードポイントに
変換します。

    // decode domain name parts
    punycode.decode('maana-pta'); // 'mañana'
    punycode.decode('--dqo34k'); // '☃-⌘'

## punycode.encode(string)

<!--
Converts a string of Unicode code points to a Punycode string of ASCII code
points.
-->

Unicode コードポイントを ASCII コードポイントによる Punycode 文字列に
変換します。

    // encode domain name parts
    punycode.encode('mañana'); // 'maana-pta'
    punycode.encode('☃-⌘'); // '--dqo34k'

## punycode.toUnicode(domain)

<!--
Converts a Punycode string representing a domain name to Unicode. Only the
Punycoded parts of the domain name will be converted, i.e. it doesn't matter if
you call it on a string that has already been converted to Unicode.
-->

Punycode 文字列で表現されたドメイン名を Unicode に変換します。
ドメイン名の中の Punycode 化された文字列だけが変換されます。
そのため、すでに Unicode に変換された文字列でも気にせずに渡すことができます。

    // decode domain names
    punycode.toUnicode('xn--maana-pta.com'); // 'mañana.com'
    punycode.toUnicode('xn----dqo34k.com'); // '☃-⌘.com'

## punycode.toASCII(domain)

<!--
Converts a Unicode string representing a domain name to Punycode. Only the
non-ASCII parts of the domain name will be converted, i.e. it doesn't matter if
you call it with a domain that's already in ASCII.
-->

Unicode 文字列で表現されたドメイン名を Punycode に変換します。
ドメイン名の中の非 ASCII 文字だけが変換されます。
すなわち、すでに ASCII 化されたドメインでも気にせずに渡すことができます。

    // encode domain names
    punycode.toASCII('mañana.com'); // 'xn--maana-pta.com'
    punycode.toASCII('☃-⌘.com'); // 'xn----dqo34k.com'

## punycode.ucs2

### punycode.ucs2.decode(string)

<!--
Creates an array containing the decimal code points of each Unicode character
in the string. While [JavaScript uses UCS-2
internally](http://mathiasbynens.be/notes/javascript-encoding), this function
will convert a pair of surrogate halves (each of which UCS-2 exposes as
separate characters) into a single code point, matching UTF-16.
-->

文字列中の Unicode 文字のコードポイントに対応する数値を含む配列を作成します。
[JavaScript uses UCS-2 internally](http://mathiasbynens.be/notes/javascript-encoding)
のように、この関数はサロゲートペア (それぞれは UCS-2 の独立した文字) を
UTF-16 にマッチする一つのコードポイントに変換します。

    punycode.ucs2.decode('abc'); // [97, 98, 99]
    // surrogate pair for U+1D306 tetragram for centre:
    punycode.ucs2.decode('\uD834\uDF06'); // [0x1D306]

### punycode.ucs2.encode(codePoints)

<!--
Creates a string based on an array of decimal code points.
-->

コードポイントの数値を含む配列を元に文字列を作成します。

    punycode.ucs2.encode([97, 98, 99]); // 'abc'
    punycode.ucs2.encode([0x1D306]); // '\uD834\uDF06'

## punycode.version

<!--
A string representing the current Punycode.js version number.
-->

現在の Punycode.js のバージョン番号を表す文字列です。
