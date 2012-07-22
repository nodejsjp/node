# DNS

    Stability: 3 - Stable

<!--
Use `require('dns')` to access this module. All methods in the dns module
use C-Ares except for `dns.lookup` which uses `getaddrinfo(3)` in a thread
pool. C-Ares is much faster than `getaddrinfo` but the system resolver is
more constant with how other programs operate. When a user does
`net.connect(80, 'google.com')` or `http.get({ host: 'google.com' })` the
`dns.lookup` method is used. Users who need to do a large number of look ups
quickly should use the methods that go through C-Ares.
-->

このモジュールにアクセスするには `require('dns')` を使用します。
dns モジュールの全てのメソッドは C-Ares を使用します。
ただし、`dns.lookup` はスレッドプール上で `getaddrinfo(3)` を使用します。
C-Ares は `getaddrinfo` よりずっと速いものの、
他のシステムと連携するにはシステムリゾルバの方が安定しています。
ユーザが `net.connect(80, 'google.com')` または
`http.get({ host: 'google.com' })` を行った時、`dns.lookup`
メソッドが使われます。
多数のルックアップを素早く実行したいユーザは、
C-Ares を呼び出すメソッドを使用すべきです。

<!--
Here is an example which resolves `'www.google.com'` then reverse
resolves the IP addresses which are returned.
-->

これは `'www.google.com'` を解決して、返された IP アドレスを逆引きで解決する例です。

    var dns = require('dns');

    dns.resolve4('www.google.com', function (err, addresses) {
      if (err) throw err;

      console.log('addresses: ' + JSON.stringify(addresses));

      addresses.forEach(function (a) {
        dns.reverse(a, function (err, domains) {
          if (err) {
            throw err;
          }

          console.log('reverse for ' + a + ': ' + JSON.stringify(domains));
        });
      });
    });

## dns.lookup(domain, [family], callback)

<!--
Resolves a domain (e.g. `'google.com'`) into the first found A (IPv4) or
AAAA (IPv6) record.
The `family` can be the integer `4` or `6`. Defaults to `null` that indicates
both Ip v4 and v6 address family.
-->

ドメイン (例 `'google.com'`) を解決して最初に見つかった
A (IPv4) または AAAA (IPv6) レコードにします。
`family` は整数の `4` または `6` を指定することができます。
デフォルトは `null` で、IP v4 と v6 の両方をアドレスファミリーを意味します。

<!--
The callback has arguments `(err, address, family)`.  The `address` argument
is a string representation of a IP v4 or v6 address. The `family` argument
is either the integer 4 or 6 and denotes the family of `address` (not
necessarily the value initially passed to `lookup`).
-->

コールバックは引数 `(err, address, family)` を持ちます。
`address` 引数は IP v4 または v6 アドレスを表現する文字列です。
`family` 引数は整数の 4 または 6 で、`address` のファミリーを意味します
(この値は必ずしも最初に `lookup` に渡す必要はありません)。

<!--
On error, `err` is an `Error` object, where `err.code` is the error code.
Keep in mind that `err.code` will be set to `'ENOENT'` not only when
the domain does not exist but also when the lookup fails in other ways
such as no available file descriptors.
-->

エラー時、`err` は `Error` オブジェクトで、`err.code` はエラーコードです。
`err.code` が `'ENOENT'` に設定されるのはドメインが存在しない場合だけではなく、
ファイル記述子が使えないなどルックアップが失敗した場合もあることに
注意してください。


## dns.resolve(domain, [rrtype], callback)

<!--
Resolves a domain (e.g. `'google.com'`) into an array of the record types
specified by rrtype. Valid rrtypes are `'A'` (IPV4 addresses, default),
`'AAAA'` (IPV6 addresses), `'MX'` (mail exchange records), `'TXT'` (text
records), `'SRV'` (SRV records), `'PTR'` (used for reverse IP lookups),
`'NS'` (name server records) and `'CNAME'` (canonical name records).
-->

ドメイン (例 `'google.com'`) を解決して `rrtype` で指定されたレコードタイプの配列にします。
妥当な `rrtype` は `'A'` (IPV4アドレス)、`'AAAA'` (IPV6アドレス)、
`'MX'` (mail exchangeレコード), `'TXT'` (テキストレコード)、
`'SRV'` (SRVレコード)、`'PTR'` (IP を逆引きでルックアップするために使われる)、
`'NS'` (ネームサーバレコード)、そして `'CNAME'` (別名レコード) です。

<!--
The callback has arguments `(err, addresses)`.  The type of each item
in `addresses` is determined by the record type, and described in the
documentation for the corresponding lookup methods below.
-->

コールバックは引数 `(err, addresses)` を持ちます。
`addresses` の各要素の種類はレコードの種類によって決まり、
対応する後述のルックアップメソッドで記述されます。

<!--
On error, `err` is an `Error` object, where `err.code` is
one of the error codes listed below.
-->

エラー時、`err` は `Error` オブジェクトで、
`err.errno` は後述するエラーコードのいずれかです。


## dns.resolve4(domain, callback)

<!--
The same as `dns.resolve()`, but only for IPv4 queries (`A` records).
`addresses` is an array of IPv4 addresses (e.g.
`['74.125.79.104', '74.125.79.105', '74.125.79.106']`).
-->

`dns.resolve()` と同じですが、IPv4 アドレス (`A` レコード) だけを問い合わせます。
`addresses` は IPv4 アドレスの配列です (例<br />
`['74.125.79.104', '74.125.79.105', '74.125.79.106']`)

## dns.resolve6(domain, callback)

<!--
The same as `dns.resolve4()` except for IPv6 queries (an `AAAA` query).
-->

IPv6 (`AAAA` レコード) を問い合わせることを除いて `dns.resolve4()` と同じです。


## dns.resolveMx(domain, callback)

<!--
The same as `dns.resolve()`, but only for mail exchange queries (`MX` records).
-->

`dns.resolve()` と同じですが、mail exchange (`MX` レコード) だけを問い合わせます。

<!--
`addresses` is an array of MX records, each with a priority and an exchange
attribute (e.g. `[{'priority': 10, 'exchange': 'mx.example.com'},...]`).
-->

`addresses` は MX レコードの配列で、それぞれは priority と exchange の属性を持ちます
(例 `[{'priority': 10, 'exchange': 'mx.example.com'},...]`)。

## dns.resolveTxt(domain, callback)

<!--
The same as `dns.resolve()`, but only for text queries (`TXT` records).
`addresses` is an array of the text records available for `domain` (e.g.,
`['v=spf1 ip4:0.0.0.0 ~all']`).
-->

`dns.resolve()` と同じですが、テキスト (`TXT` レコード) だけを問い合わせます。
`addresses` は `domain` で利用可能なテキストレコードの配列です。
(例、`['v=spf1 ip4:0.0.0.0 ~all']`)

## dns.resolveSrv(domain, callback)

<!--
The same as `dns.resolve()`, but only for service records (`SRV` records).
`addresses` is an array of the SRV records available for `domain`. Properties
of SRV records are priority, weight, port, and name (e.g.,
`[{'priority': 10, {'weight': 5, 'port': 21223, 'name': 'service.example.com'}, ...]`).
-->

`dns.resolve()` と同じですが、サービスレコード (`SRV` レコード) だけを問い合わせます。
`addresses` は `domain` で利用可能な SRV レコードの配列です。
SRV レコードのプロパティは priority、weight、port、そして name です
(例 `[{'priority': 10, {'weight': 5, 'port': 21223, 'name': 'service.example.com'}, ...]`)。

## dns.resolveNs(domain, callback)

<!--
The same as `dns.resolve()`, but only for name server records (`NS` records).
`addresses` is an array of the name server records available for `domain`
(e.g., `['ns1.example.com', 'ns2.example.com']`).
-->

`dns.resolve()` と同じですが、ネームサーバレコード (`NS` レコード) 
だけを問い合わせます。
`address` は `domain` で利用可能なネームサーバレコードの配列です
(例 `['ns1.example.com', 'ns2.example.com']`)。

## dns.resolveCname(domain, callback)

<!--
The same as `dns.resolve()`, but only for canonical name records (`CNAME`
records). `addresses` is an array of the canonical name records available for
`domain` (e.g., `['bar.example.com']`).
-->

`dns.resolve()` と同じですが、別名レコード (`CNAME` レコード) 
だけを問い合わせます。
`address` は `domain` で利用可能な別名レコードの配列です
`domain` (e.g., `['bar.example.com']`)。

## dns.reverse(ip, callback)

<!--
Reverse resolves an ip address to an array of domain names.
-->

IP アドレスからドメイン名の配列へ逆引きで解決します。

<!--
The callback has arguments `(err, domains)`.
-->

コールバックは引数 `(err, domains)` を持ちます。

<!--
On error, `err` is an `Error` object, where `err.code` is
one of the error codes listed below.
-->

エラー時、`err` は `Error` オブジェクトで、
`err.errno` は後述するエラーコードのいずれかです。

## Error codes

<!--
Each DNS query can return one of the following error codes:
-->

どの DNS 問い合わせも以下のエラーコードの一つを返します:

<!--
- `dns.NODATA`: DNS server returned answer with no data.
- `dns.FORMERR`: DNS server claims query was misformatted.
- `dns.SERVFAIL`: DNS server returned general failure.
- `dns.NOTFOUND`: Domain name not found.
- `dns.NOTIMP`: DNS server does not implement requested operation.
- `dns.REFUSED`: DNS server refused query.
- `dns.BADQUERY`: Misformatted DNS query.
- `dns.BADNAME`: Misformatted domain name.
- `dns.BADFAMILY`: Unsupported address family.
- `dns.BADRESP`: Misformatted DNS reply.
- `dns.CONNREFUSED`: Could not contact DNS servers.
- `dns.TIMEOUT`: Timeout while contacting DNS servers.
- `dns.EOF`: End of file.
- `dns.FILE`: Error reading file.
- `dns.NOMEM`: Out of memory.
- `dns.DESTRUCTION`: Channel is being destroyed.
- `dns.BADSTR`: Misformatted string.
- `dns.BADFLAGS`: Illegal flags specified.
- `dns.NONAME`: Given hostname is not numeric.
- `dns.BADHINTS`: Illegal hints flags specified.
- `dns.NOTINITIALIZED`: c-ares library initialization not yet performed.
- `dns.LOADIPHLPAPI`: Error loading iphlpapi.dll.
- `dns.ADDRGETNETWORKPARAMS`: Could not find GetNetworkParams function.
- `dns.CANCELLED`: DNS query cancelled.
-->

- `dns.NODATA`: DNS サーバがデータがないと応答した。
- `dns.FORMERR`: DNS サーバが問い合わせフォーマットが不正だと主張した。
- `dns.SERVFAIL`: DNS サーバが一般的な失敗を返した。
- `dns.NOTFOUND`: ドメイン名が見つからない。
- `dns.NOTIMP`: DNS サーバは要求された操作を実装していない。
- `dns.REFUSED`: DNS サーバが問い合わせを拒否した。
- `dns.BADQUERY`: DNS 問い合わせのフォーマットが不正。
- `dns.BADNAME`: ドメイン名のフォーマットが不正。
- `dns.BADFAMILY`: サポートされないアドレスファミリー。
- `dns.BADRESP`: DNS 応答のフォーマットが不正。
- `dns.CONNREFUSED`: DNS サーバに接続できない。
- `dns.TIMEOUT`: DNS サーバへの接続がタイムアウトした。
- `dns.EOF`: エンドオブファイル。
- `dns.FILE`: ファイルの読み込みがエラー。
- `dns.NOMEM`: メモリ不足。
- `dns.DESTRUCTION`: チャネルが壊れている。
- `dns.BADSTR`: 文字列のフォーマットが不正。
- `dns.BADFLAGS`: 不正なフラグが指定された。
- `dns.NONAME`: 与えられたホスト名が数値ではない。
- `dns.BADHINTS`: 不正なヒントフラグが指定された。
- `dns.NOTINITIALIZED`: c-ares ライブラリが初期化されていない。
- `dns.LOADIPHLPAPI`: iphlpapi.dll のローディングでエラー。
- `dns.ADDRGETNETWORKPARAMS`: GetNetworkParams 関数が見つからない。
- `dns.CANCELLED`: DNS 問い合わせがキャンセルされた。
