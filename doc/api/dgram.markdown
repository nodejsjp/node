# UDP / Datagram Sockets

<!-- name=dgram -->

<!--
Datagram sockets are available through `require('dgram')`.
-->

データグラムソケットは `require('dgram')` で利用可能になります。

## dgram.createSocket(type, [callback])

<!--
* `type` String. Either 'udp4' or 'udp6'
* `callback` Function. Attached as a listener to `message` events.
  Optional
* Returns: Socket object
-->

* `type` String. 'udp4' または 'udp6' のいずれか
* `callback` Function. `'message'` イベントのリスナとして割り当てられる、
  Optional
* Returns: Socket object

<!--
Creates a datagram Socket of the specified types.  Valid types are `udp4`
and `udp6`.

Takes an optional callback which is added as a listener for `message` events.

Call `socket.bind` if you want to receive datagrams. `socket.bind()` will bind
to the "all interfaces" address on a random port (it does the right thing for
both `udp4` and `udp6` sockets). You can then retrieve the address and port
with `socket.address().address` and `socket.address().port`.
-->

指定された種類のデータグラムソケットを作成します。
妥当な種類は `udp4` と `udp6`です。

オプションのコールバックは `message` イベントのリスナーとして加えられます。

データグラムを受信したい場合は `socket.bind()` を呼び出します。
`socket.bind()` は「全てのインタフェース」のアドレスにランダムなポート
(`udp4` と `udp6` ソケットの両方で正しいものです) をバインドします。
そのアドレスとポートは `socket.address().address` および
`socket.address().port` で取得することができます。

## Class: Socket

<!--
The dgram Socket class encapsulates the datagram functionality.  It
should be created via `dgram.createSocket(type, [callback])`.
-->

dgram Scoket クラスはデータグラム機能をカプセル化します。
それは `dgram.createSocket(type, [callback])` を通じて生成されます。

### Event: 'message'

<!--
* `msg` Buffer object. The message
* `rinfo` Object. Remote address information
-->

* `msg` Buffer object. メッセージ
* `rinfo` Object. リモートアドレスの情報

<!--
Emitted when a new datagram is available on a socket.  `msg` is a `Buffer` and `rinfo` is
an object with the sender's address information and the number of bytes in the datagram.
-->

ソケット上で新しいデータグラムが到着した時に生成されます。`msg` は `Buffer` で、
`rinfo` は送信者のアドレス情報とデータグラムのバイト数を持ったオブジェクトです。

### Event: 'listening'

<!--
Emitted when a socket starts listening for datagrams.  This happens as soon as UDP sockets
are created.
-->

ソケットでデータグラムの待ち受けを開始すると生成されます。
これは UDP ソケットが作成されるとすぐに発生します。

### Event: 'close'

<!--
Emitted when a socket is closed with `close()`.  No new `message` events will be emitted
on this socket.
-->

`close()` によってソケットがクローズすると生成されます。
このソケットでは新しい `message` イベントは生成されなくなります。

### Event: 'error'

* `exception` Error object

<!--
Emitted when an error occurs.
-->

エラーが発生すると生成されます。

### dgram.send(buf, offset, length, port, address, [callback])

<!--
* `buf` Buffer object.  Message to be sent
* `offset` Integer. Offset in the buffer where the message starts.
* `length` Integer. Number of bytes in the message.
* `port` Integer. destination port
* `address` String. destination IP
* `callback` Function. Callback when message is done being delivered.
  Optional.
-->

* `buf` Buffer object.  送信されるメッセージ
* `offset` Integer. メッセージの開始位置となるバッファ内のオフセット
* `length` Integer. メッセージのバイト長
* `port` Integer. 接続先のポート番号
* `address` String. 接続先の IP
* `callback` Function. メッセージの配信が完了した後にコールバックされる、
  Optional.

<!--
For UDP sockets, the destination port and IP address must be specified.  A string
may be supplied for the `address` parameter, and it will be resolved with DNS.  An
optional callback may be specified to detect any DNS errors and when `buf` may be
re-used.  Note that DNS lookups will delay the time that a send takes place, at
least until the next tick.  The only way to know for sure that a send has taken place
is to use the callback.

If the socket has not been previously bound with a call to `bind`, it's
assigned a random port number and bound to the "all interfaces" address
(0.0.0.0 for `udp4` sockets, ::0 for `udp6` sockets).
-->

UDP ソケット用です。相手先のポートと IP アドレスは必ず指定しなければなりません。
`address` パラメータに文字列を提供すると、それは DNS によって解決されます。
DNS エラーと `buf` が再利用可能になった時のためにオプションのコールバックを指定することができます。
DNS ルックアップは送信を少なくとも次の機会まで遅らせることに注意してください。
送信が行われたことを確実に知る唯一の手段はコールバックを使うことです。

ソケットが以前に `bind` の呼び出しによってバインドされていない場合は、
ランダムなポート番号が「全てのインタフェース」アドレスに対してバインドされます
(`udp4` ソケットでは 0.0.0.0、`udp6` では ::0)。
<!--
Example of sending a UDP packet to a random port on `localhost`;
-->

`localhost` の適当なポートに UDP パケットを送信する例;

    var dgram = require('dgram');
    var message = new Buffer("Some bytes");
    var client = dgram.createSocket("udp4");
    client.send(message, 0, message.length, 41234, "localhost", function(err, bytes) {
      client.close();
    });

<!--
**A Note about UDP datagram size**

The maximum size of an `IPv4/v6` datagram depends on the `MTU` (_Maximum Transmission Unit_)
and on the `Payload Length` field size.

- The `Payload Length` field is `16 bits` wide, which means that a normal payload
  cannot be larger than 64K octets including internet header and data
  (65,507 bytes = 65,535 − 8 bytes UDP header − 20 bytes IP header);
  this is generally true for loopback interfaces, but such long datagrams
  are impractical for most hosts and networks.

- The `MTU` is the largest size a given link layer technology can support for datagrams.
  For any link, `IPv4` mandates a minimum `MTU` of `68` octets, while the recommended `MTU`
  for IPv4 is `576` (typically recommended as the `MTU` for dial-up type applications),
  whether they arrive whole or in fragments.

  For `IPv6`, the minimum `MTU` is `1280` octets, however, the mandatory minimum
  fragment reassembly buffer size is `1500` octets.
  The value of `68` octets is very small, since most current link layer technologies have
  a minimum `MTU` of `1500` (like Ethernet).

Note that it's impossible to know in advance the MTU of each link through which
a packet might travel, and that generally sending a datagram greater than
the (receiver) `MTU` won't work (the packet gets silently dropped, without
informing the source that the data did not reach its intended recipient).
-->

**UDP データグラムのサイズについて**

`IPv4/v6` データグラムの最大のサイズは `MTU` (_Maximum Transmission Unit_) と、
`Payload Length` フィールドサイズに依存します。

- `Payload Length` フィールドサイズは 16bit 長で、これは通常のペイロードが
  IP ヘッダとデータ含めて 64K オクテットより長くなれないことを意味します
  (65,507 バイト = 65,535 − 8 バイトの UDP ヘッダ − 20 バイトの IP ヘッダ);
  これは一般的にループバックインタフェースでは正しいものの、
  ほとんどのホストとネットワークにとって長大なデータグラムは
  現実的ではありません。

- `MTU` はリンク層により大きなサイズを与える技術で、
データグラムもサポートできます。
  どんなリンクでも、それらが全体として到着するか断片化されるかに関わらず、
  `IPv4` は最低 `69` オクテット必要で、推奨される `IPv4` の `MTU` は `576` です
  (典型的なダイヤルアップ型アプリケーションの `MUT` 推奨値)。

  `IPv6` では最小の `MTU` は `1280` オクテットですが、フラグメントを再構築する
  バッファサイズは最低 `1500` オクテットが必要です。
  `68` オクテットはとても小さいので、もっとも現代的なリンク層技術では、
  最小の `MTU` は `1500` です (イーサネットと同じです)。

パケットが通過する各リンクの MTU をあらかじめ知ることは
できないこと、(受信側の) `MTU` より大きなデータグラムを送信しても
通常は動作しないことに注意してください
(パケットは送り主に知らされることなく黙って捨てられ、
意図した受信者に到達することはありません)。


### dgram.bind(port, [address])

* `port` Integer
* `address` String, Optional

<!--
For UDP sockets, listen for datagrams on a named `port` and optional `address`. If
`address` is not specified, the OS will try to listen on all addresses.
-->

UDP ソケット用です。`port` とオプションの `address` でデータグラムを待ち受けます。
`address` が指定されなければ、OS は全てのアドレスからの待ち受けを試みます。

<!--
Example of a UDP server listening on port 41234:
-->

41234 番ポートを待ち受ける UDP サーバの例:

    var dgram = require("dgram");

    var server = dgram.createSocket("udp4");

    server.on("message", function (msg, rinfo) {
      console.log("server got: " + msg + " from " +
        rinfo.address + ":" + rinfo.port);
    });

    server.on("listening", function () {
      var address = server.address();
      console.log("server listening " +
          address.address + ":" + address.port);
    });

    server.bind(41234);
    // server listening 0.0.0.0:41234


### dgram.close()

<!--
Close the underlying socket and stop listening for data on it.
-->

下層のソケットをクローズし、データの待ち受けを終了します。

### dgram.address()

<!--
Returns an object containing the address information for a socket.  For UDP sockets,
this object will contain `address` and `port`.
-->

オブジェクトが持っているソケットのアドレス情報を返します。
このオブジェクトは `address` と `port` を持っています。

### dgram.setBroadcast(flag)

* `flag` Boolean

<!--
Sets or clears the `SO_BROADCAST` socket option.  When this option is set, UDP packets
may be sent to a local interface's broadcast address.
-->

ソケットのオプション `SO_BROADCAST` を設定またはクリアします。
このオプションが設定されると、UDP パケットはローカルインタフェースのブロードキャスト用アドレスに送信されます。

### dgram.setTTL(ttl)

* `ttl` Integer

<!--
Sets the `IP_TTL` socket option.  TTL stands for "Time to Live," but in this context it
specifies the number of IP hops that a packet is allowed to go through.  Each router or
gateway that forwards a packet decrements the TTL.  If the TTL is decremented to 0 by a
router, it will not be forwarded.  Changing TTL values is typically done for network
probes or when multicasting.
-->

ソケットオプションの `IP_TTL` を設定します。
TTL は「生存期間」を表しますが、このコンテキストではパケットが通過を許可される IP のホップ数を指定します。
各ルータまたはゲートウェイはパケットを送出する際 TTL をデクリメントします。
ルータによって TTL がデクリメントされて 0 になるとそれは送出されません。
TTL 値の変更は通常、ネットワークの調査やマルチキャストで使われます。

<!--
The argument to `setTTL()` is a number of hops between 1 and 255.  The default on most
systems is 64.
-->

`setTTL()` の引数は 1 から 255 のホップ数でです。ほとんどのシステムでデフォルトは 64 です。

### dgram.setMulticastTTL(ttl)

* `ttl` Integer

<!--
Sets the `IP_MULTICAST_TTL` socket option.  TTL stands for "Time to Live," but in this
context it specifies the number of IP hops that a packet is allowed to go through,
specifically for multicast traffic.  Each router or gateway that forwards a packet
decrements the TTL. If the TTL is decremented to 0 by a router, it will not be forwarded.

The argument to `setMulticastTTL()` is a number of hops between 0 and 255.  The default on most
systems is 64.
-->

`IP_MULTICAST_TTL` ソケットオプションを設定します。
TTL は「生存期間」を表しますが、この文脈では特にマルチキャストのトラフィックにおいてパケットが通過できるIPホップの数を指定します。
それぞれのルーターまたはゲートウェイは、パケットを転送する際に TTL をデクリメントします。
TTL がルーターによって 0 までデクリメントされると、それは転送されません。
`setMulticastTTL()` の引数はホップを表す数値で、0 から 255 の間です。
ほとんどのシステムでデフォルトは 64 です。

### dgram.setMulticastLoopback(flag)

* `flag` Boolean

<!--
Sets or clears the `IP_MULTICAST_LOOP` socket option.  When this option is set, multicast
packets will also be received on the local interface.
-->

`IP_MULTICAST_LOOP` ソケットオプションを設定またはクリアします。
このオプションが設定されると、マルチキャストのパケットはローカルインタフェースでも受信できるようになります。

### dgram.addMembership(multicastAddress, [multicastInterface])

* `multicastAddress` String
* `multicastInterface` String, Optional

<!--
Tells the kernel to join a multicast group with `IP_ADD_MEMBERSHIP` socket option.

If `multicastInterface` is not specified, the OS will try to add membership to all valid
interfaces.
-->

`IP_ADD_MEMBERSHIP` ソケットオプションを設定し、マルチキャストグループに参加することをカーネルに伝えます。

`multicastInterface` が指定されなかった場合は、全ての妥当なインタフェースをメンバーシップに加えようとします。

### dgram.dropMembership(multicastAddress, [multicastInterface])

* `multicastAddress` String
* `multicastInterface` String, Optional

<!--
Opposite of `addMembership` - tells the kernel to leave a multicast group with
`IP_DROP_MEMBERSHIP` socket option. This is automatically called by the kernel
when the socket is closed or process terminates, so most apps will never need to call
this.

If `multicastInterface` is not specified, the OS will try to drop membership to all valid
interfaces.
-->

`addMembership` の反対です - `IP_DROP_MEMBERSHIP` ソケットオプションによって、マルチキャストグループから抜けることをカーネルに伝えます。
これはソケットのクローズ時やプロセスの終了時にカーネルによって自動的に呼び出されるため、ほとんどのアプリケーションはこれを呼び出す必要がありません。

`multicastInterface` が指定されなかった場合は、全ての妥当なインタフェースをメンバーシップから削除しようとします。
