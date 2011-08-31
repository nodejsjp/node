## UDP / Datagram Sockets

<!--

Datagram sockets are available through `require('dgram')`.

-->
データグラムソケットは `require('dgram')` で利用可能になります。

### Event: 'message'

`function (msg, rinfo) { }`

<!--

Emitted when a new datagram is available on a socket.  `msg` is a `Buffer` and `rinfo` is
an object with the sender's address information and the number of bytes in the datagram.

-->
ソケット上で新しいデータグラムが到着した時に生成されます。`msg` は `Buffer` で、
`rinfo` は送信者のアドレス情報とデータグラムのバイト数を持ったオブジェクトです。

### Event: 'listening'

`function () { }`

<!--

Emitted when a socket starts listening for datagrams.  This happens as soon as UDP sockets
are created.

-->
ソケットでデータグラムの待ち受けを開始すると生成されます。
これは UDP ソケットが作成されるとすぐに発生します。

### Event: 'close'

`function () { }`

<!--

Emitted when a socket is closed with `close()`.  No new `message` events will be emitted
on this socket.

-->
`close()` によってソケットがクローズすると生成されます。
このソケットでは新しい `message` イベントは生成されなくなります。

### dgram.createSocket(type, [callback])

<!--

Creates a datagram socket of the specified types.  Valid types are `udp4`
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

### dgram.send(buf, offset, length, port, address, [callback])

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
    client.send(message, 0, message.length, 41234, "localhost");
    client.close();


### dgram.bind(port, [address])

<!--

For UDP sockets, listen for datagrams on a named `port` and optional `address`.  If
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

<!--

Sets or clears the `SO_BROADCAST` socket option.  When this option is set, UDP packets
may be sent to a local interface's broadcast address.

-->
ソケットのオプション `SO_BROADCAST` を設定またはクリアします。
このオプションが設定されると、UDP パケットはローカルインタフェースのブロードキャスト用アドレスに送信されます。

### dgram.setTTL(ttl)

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

<!--

Sets or clears the `IP_MULTICAST_LOOP` socket option.  When this option is set, multicast
packets will also be received on the local interface.

-->
`IP_MULTICAST_LOOP` ソケットオプションを設定またはクリアします。
このオプションが設定されると、マルチキャストのパケットはローカルインタフェースでも受信できるようになります。

### dgram.addMembership(multicastAddress, [multicastInterface])

<!--

Tells the kernel to join a multicast group with `IP_ADD_MEMBERSHIP` socket option.

If `multicastInterface` is not specified, the OS will try to add membership to all valid
interfaces.

-->
`IP_ADD_MEMBERSHIP` ソケットオプションを設定し、マルチキャストグループに参加することをカーネルに伝えます。

`multicastInterface` が指定されなかった場合は、全ての妥当なインタフェースをメンバーシップに加えようとします。

### dgram.dropMembership(multicastAddress, [multicastInterface])

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
