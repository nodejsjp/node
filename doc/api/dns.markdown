## DNS

<!--

Use `require('dns')` to access this module.

-->
このモジュールにアクセスするには `require('dns')` を使用します。

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
            console.log('reverse for ' + a + ' failed: ' +
              err.message);
          } else {
            console.log('reverse for ' + a + ': ' +
              JSON.stringify(domains));
          }
        });
      });
    });

### dns.lookup(domain, family=null, callback)

<!--

Resolves a domain (e.g. `'google.com'`) into the first found A (IPv4) or
AAAA (IPv6) record.

-->
ドメイン (例 `'google.com'`) を解決して最初に見つかった
A (IPv4) または AAAA (IPv6) レコードにします。

<!--

The callback has arguments `(err, address, family)`.  The `address` argument
is a string representation of a IP v4 or v6 address. The `family` argument
is either the integer 4 or 6 and denotes the family of `address` (not
neccessarily the value initially passed to `lookup`).

-->
コールバックは引数 `(err, address, family)` を持ちます。
`address` 引数は IP v4 または v6 アドレスを表現する文字列です。
`family` 引数は 4 または 6 の整数で、`address` のファミリーを意味します
(この値は必ずしも最初に `lookup` に渡す必要はありません)。


### dns.resolve(domain, rrtype='A', callback)

<!--

Resolves a domain (e.g. `'google.com'`) into an array of the record types
specified by rrtype. Valid rrtypes are `A` (IPV4 addresses), `AAAA` (IPV6
addresses), `MX` (mail exchange records), `TXT` (text records), `SRV` (SRV
records), and `PTR` (used for reverse IP lookups).

-->
ドメイン (例 `'google.com'`) を解決して `rrtype` で指定されたレコードタイプの配列にします。
妥当な `rrtype` は `A` (IPV4アドレス)、`AAAA` (IPV6アドレス)、
`MX` (mail exchangeレコード), `TXT` (テキストレコード)、`SRV` (SRVレコード)、
`PTR` (IP を逆引きでルックアップするために使われる) です。

<!--

The callback has arguments `(err, addresses)`.  The type of each item
in `addresses` is determined by the record type, and described in the
documentation for the corresponding lookup methods below.

-->
コールバックは引数 `(err, addresses)` を持ちます。
`addresses` の各要素の種類はレコードの種類によって決まり、
対応する後述のルックアップメソッドで記述されます。

<!--

On error, `err` would be an instanceof `Error` object, where `err.errno` is
one of the error codes listed below and `err.message` is a string describing
the error in English.

-->
エラー発生時、`err` は `Error` オブジェクトのインスタンスであり、
`err.errno` は後述するエラーコードのいずれか、
`err.message` はエラーを英語で説明する文字列となります。


### dns.resolve4(domain, callback)

<!--

The same as `dns.resolve()`, but only for IPv4 queries (`A` records).
`addresses` is an array of IPv4 addresses (e.g.
`['74.125.79.104', '74.125.79.105', '74.125.79.106']`).

-->
`dns.resolve()` と同じですが、IPv4 アドレス (`A` レコード) だけを問い合わせます。
`addresses` は IPv4 アドレスの配列です (例<br />
`['74.125.79.104', '74.125.79.105', '74.125.79.106']`)

### dns.resolve6(domain, callback)

<!--

The same as `dns.resolve4()` except for IPv6 queries (an `AAAA` query).

-->
IPv6 (`AAAA` レコード) を問い合わせることを除いて `dns.resolve4()` と同じです。


### dns.resolveMx(domain, callback)

<!--

The same as `dns.resolve()`, but only for mail exchange queries (`MX` records).

-->
`dns.resolve()` と同じですが、mail exchange (`MX` レコード) だけを問い合わせます。

<!--

`addresses` is an array of MX records, each with a priority and an exchange
attribute (e.g. `[{'priority': 10, 'exchange': 'mx.example.com'},...]`).

-->
`addresses`は MX レコードの配列で、それぞれは priority と exchange の属性を持ちます
(例 `[{'priority': 10, 'exchange': 'mx.example.com'},...]`)。

### dns.resolveTxt(domain, callback)

<!--

The same as `dns.resolve()`, but only for text queries (`TXT` records).
`addresses` is an array of the text records available for `domain` (e.g.,
`['v=spf1 ip4:0.0.0.0 ~all']`).

-->
`dns.resolve()` と同じですが、テキスト (`TXT` レコード) だけを問い合わせます。
`addresses` は利用可能な `domain` のテキストレコードの配列です。
(例、`['v=spf1 ip4:0.0.0.0 ~all']`)

### dns.resolveSrv(domain, callback)

<!--

The same as `dns.resolve()`, but only for service records (`SRV` records).
`addresses` is an array of the SRV records available for `domain`. Properties
of SRV records are priority, weight, port, and name (e.g.,
`[{'priority': 10, {'weight': 5, 'port': 21223, 'name': 'service.example.com'}, ...]`).

-->
`dns.resolve()` と同じですが、サービスレコード (`SRV` レコード) だけを問い合わせます。
`addresses` は利用可能な `domain` の SRV レコードの配列です。
SRV レコードのプロパティは priority、weight、port、そして name です
(例 `[{'priority': 10, {'weight': 5, 'port': 21223, 'name': 'service.example.com'}, ...]`)。

### dns.reverse(ip, callback)

<!--

Reverse resolves an ip address to an array of domain names.

-->
IP アドレスからドメイン名の配列へ逆引きで解決します。

<!--

The callback has arguments `(err, domains)`.

-->
コールバックは引数 `(err, domains)` を持ちます。

<!--

If there an an error, `err` will be non-null and an instanceof the Error
object.

-->
エラーがあった場合、`err` は非 null で Error オブジェクトのインスタンスとなります。

<!--

Each DNS query can return an error code.

-->
どの DNS 問い合わせもエラーコードを返せます。

<!--

- `dns.TEMPFAIL`: timeout, SERVFAIL or similar.
- `dns.PROTOCOL`: got garbled reply.
- `dns.NXDOMAIN`: domain does not exists.
- `dns.NODATA`: domain exists but no data of reqd type.
- `dns.NOMEM`: out of memory while processing.
- `dns.BADQUERY`: the query is malformed.

-->
- `dns.TEMPFAIL`: タイムアウト、SERVFAIL あるいは同様のもの。
- `dns.PROTOCOL`: 応答が不正。
- `dns.NXDOMAIN`: ドメインが存在しない。
- `dns.NODATA`: ドメインは存在するが、要求された種類のデータがない。
- `dns.NOMEM`: 処理中にメモリが不足。
- `dns.BADQUERY`: 問い合わせが不正な形式。
