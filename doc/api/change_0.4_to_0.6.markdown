## v0.4 から v0.6 で変更された API

### Changed:

 * Cygwin はもうサポートされません．Windows ネイティブ版を使用してください．

 * `Buffer`
   * `Buffer.write()` にオプションの第3引数 `length` が追加されました．

 * `child_process`
   * `child_process.spawn()` の `customFds` オプションは非推奨になりました．`customFds` は値が -1, 0, 1, 2 で，それらが TTY ファイル記述子の場合だけ動作します．同様の機能として `stdinStream`，`stdoutStream`，`stderrStream` オプションを使うことができます．
   * Unix パイプを通じてファイル記述子を送ることはできなくなりました．`child_process.fork()` を通じてハンドルを渡してください．

 * `dgram`
   * `dgram.createSocket()` で `'unix_dgram'` はサポートされなくなりました．代わりに [bnoordhuis/node-unix-dgram](https://github.com/bnoordhuis/node-unix-dgram) を使用してください．Unix ドメインのデータグラムは，インターネットドメインのデータグラム (UDP) では *ない* ことに注意してください．それはサポートされています．

 * `dns`
   * `dns.lookup` は C-Ares ではなく，スレッドプール上で `getaddrinfo` を使うようになりました．DNS モジュールのその他のメソッドは C-Ares を使用します．ルックアップにシステムリゾルバを使用するのは，mDNS や /et/host ファイル，nsswitch その他を使用してフックしやすいために便利です．従来，`dns.lookup()` は `"****"` のような不正なドメイン名に対して `EBADNAME` エラーを返していました．`getaddrinfo()` はそれとは異なり，不正なドメインに対して `ENOTFOUND` (別名 `NXDOMAIN`) を返していました．そのため，`dns.lookup()` は `"****"` のような不正なドメインに対して `ENOTFOUND` を返すようになりました．

 * `events`
   * C++ `node::EventEmitter` は削除されました．`node::MakeCallback()` を使用してください．
   * `EventEmitter.removeAllListeners()` は全てのリスナを一度に削除できるようになりました．

 * `fs`
   * `fs.mkdir()` および `fs.mkdirSync()` の `mode` 引数がオプションになりました (デフォルトは `0777`)．
   * `fs.symlink()` はオプションの `mode` 引数を受け取るようになりました．それは `'dir'` または `'file'` を指定することができます．デフォルトは `'file'` です．この引数は Windows でのみ必要です (その他のプラットフォームでは無視されます)．

 * `http`
   * `http.request()` と `http.get()` はデフォルトで `Connection: Keep-Alive` を使うようになりました．
   * `http.Agent.appendMessage()` は削除されました．
   * `http.getAgent()` は削除されました. `http.globalAgent` を使用してください．
   * `httpAgent` ではなく `http.ClientRequest` が `'upgrade'` イベントを生成するようになりました．

 * `https`
   * `https.request()` と `https.get()` のデフォルト `Agent` は，`key`，`cert`，`ca` オプションを無視します. カスタムの `Agent` を使用してください．

 * Module system
   * `require.paths` は削除されました (`NODE_PATH` 環境変数を使用してください)．

 * `net`
   * `net.Server.listenFD()` はサポートされなくなりました．

 * `process`
   * `process.ENV` は削除されました．`process.env` を使用してください．
   * `process.ARGV` は削除されました．`process.argv` を使用してください．
   * `process.binding('stdio')` は削除されました．これはプライベート API で，そもそも使用すべきではありません．
   * `process.binding('net')` は削除されました．
   * `process.memoryUsage().vsize` は削除されました．必要になることはないでしょう．
   * `process.stdout` と `process.stderr` は，それらが通常ファイルか TTY ファイル記述子を参照している場合，ブロックします．
   * `process.stdin`，`process.stdout`，`process.stderr` は getter になりました．
     オーバーライドするには (もし本当に必要なら):

     ```javascript
     process.__defineGetter__('stdout', function() {
       return your_object;
     });
     ```

 * `stream`
   * `stream.pipe()` は，`pause()/resume()` を実装していないソースストリーム上で `'pause'/'resume'` イベントを生成しなくなりました．

 * `tty`
   * `tty.setWindowSize(fd, row, col)` は削除されました．
   * `tty.getWindowSize(fd)` は削除されました．代わりに `process.stdout.getWindowSize()` を使用してください．

 * V8 (v3.1 から v3.6)
   * `RegExp` は `Function` ではなくなりました (ES5 準拠)．`RegExp.exec()` を使用してください．
   * `Date` のフォーマット文字列がタイムゾーンを含まない場合 (例: `new Date('2011-06-06')`) は，ローカルのタイムゾーンではなく UTC に基づきます (ES5 準拠).明示的にタイムゾーンを指定してください (例: `new Date('2011-06-06 00:00:00 +09:00')`)．
   * `Error` の全ての標準プロパティは列挙されなくなりました (ES5 準拠). それらを表示したい場合は `util.inspect(err, true)` を使用してください．


### Added:

 * `assert`
   * `assert.ok(val)` の短縮版 `assert(val)`
 * `Buffer`
   * `'hex'` エンコーディング．
   * `Buffer.readInt8()/readInt16LE()/readInt16BE()/readInt32LE()/readInt32BE()`
   * `Buffer.readUInt8()/readUInt16LE()/readUInt16BE()/readUInt32LE()/readUInt32BE()`
   * `Buffer.readFloatLE()/readFloatBE()/readDoubleLE()/readDoubleBE()`
   * `Buffer.writeInt8()/writeInt16LE()/writeInt16BE()/writeInt32LE()/writeInt32BE()`
   * `Buffer.writeUInt8()/writeUInt16LE()/writeUInt16BE()/writeUInt32LE()/writeUInt32BE()`
   * `Buffer.writeFloatLE()/writeFloatBE()/writeDoubleLE()/writeDoubleBE()`
   * `Buffer.fill()`
   * Typed Arrays

 * `child_process`
   * `child_process.fork()`

 * `cluster`
   * `node cluster` [API docs](cluster.html) を参照してください．

 * `crypto`
   * `crypto.createDiffieHellman()`, `crypto.pbkdf2()`, `crypto.randomBytes()`

 * `fs`
   * `fs.watch()`
   * `fs.utimes()/utimesSync()`, `fs.futimes()/futimesSync()`
   * `fs.createReadStream()` と `fs.createWriteStream()` に `start` オプションが追加されました．

 * `http`
   * `http.ClientRequest.setTimeout()/setNoDelay()/setSocketKeepAlive()`
   * `http.request()` に `auth` オプションが追加されました．

 * `https`
   * `https.createServer()`，`https.request()`，`https.get()` に `passphrase` オプションが追加されました．

 * Module system
   * `.json` モジュール．
   * `module.require()`

 * `net`
   * `net.connect()`
   * `net.Socket.remotePort`, `bytesRead`, `bytesWrite`

 * `os`
   * `os.arch()`, `os.platform()`, `os.uptime()`, `os.networkInterfaces()`

 * `path`
   * `path.relative()`

 * `process`
   * `process.arch`, `process.uptime()`

 * `tls`
   * `tls.createServer()` と `tls.connect()` に `passphrase` オプションが追加されました．
   * `tls.createServer()` に `sessionIdContext` オプションが追加されました．
   * `tls.connect()` に `session` オプションが追加されました．
   * `tls.CryptoStream.getSession()`
   * `tls.CleartextStream.address()`, `remoteAddress`, `remotePort`
   * `tls.Server` が [NPN (Next Protocol Negotiation) と SNI (Server Name Indication)](tls.html#nPN_and_SNI) をサポートしました．

 * `util`
   * `util.format()`, `util.isArray()`, `util.isRegExp()`, `uitl.isDate()`, `util.isError()`.

 * `zlib` モジュール
   * [API docs](zlib.html) を参照してください．
