## os Module

<!--

Use `require('os')` to access this module.

-->
`require('os')` によってこのモジュールにアクセスします。

### os.hostname()

<!--

Returns the hostname of the operating system.

-->
オペレーティングシステムのホスト名を返します。

### os.type()

<!--

Returns the operating system name.

-->
オペレーティングシステムの名前を返します。

### os.release()

<!--

Returns the operating system release.

->
オペレーティングシステムのリリースを返します。

### os.uptime()

<!--

Returns the system uptime in seconds.

-->

システムが起動してからの秒数を返します。

### os.loadavg()

<!--

Returns an array containing the 1, 5, and 15 minute load averages.

-->
1 分、5 分、15 分間のロードアベレージを含んだ配列を返します。

### os.totalmem()

<!--

Returns the total amount of system memory in bytes.

-->
システム全体が使用しているメモリのバイト数を返します。

### os.freemem()

<!--

Returns the amount of free system memory in bytes.

-->
システム全体で空いているメモリのバイト数を返します。

### os.cpus()

<!--

Returns an array of objects containing information about each CPU/core installed: model, speed (in MHz), and times (an object containing the number of CPU ticks spent in: user, nice, sys, idle, and irq).

-->
インストールされている CPU/ コアごとの情報を含んだオブジェクトの配列を返します。
情報はモデル、スピード (MHz)、そして時間 (CPU が使用した user, nice, sys, idle, irq 時間を含んだオブジェクト) です。

<!--

Example inspection of os.cpus:

-->
os.cpus の例:

    [ { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 252020,
           nice: 0,
           sys: 30340,
           idle: 1070356870,
           irq: 0 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 306960,
           nice: 0,
           sys: 26980,
           idle: 1071569080,
           irq: 0 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 248450,
           nice: 0,
           sys: 21750,
           idle: 1070919370,
           irq: 0 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 256880,
           nice: 0,
           sys: 19430,
           idle: 1070905480,
           irq: 20 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 511580,
           nice: 20,
           sys: 40900,
           idle: 1070842510,
           irq: 0 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 291660,
           nice: 0,
           sys: 34360,
           idle: 1070888000,
           irq: 10 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 308260,
           nice: 0,
           sys: 55410,
           idle: 1071129970,
           irq: 880 } },
      { model: 'Intel(R) Core(TM) i7 CPU         860  @ 2.80GHz',
        speed: 2926,
        times:
         { user: 266450,
           nice: 1480,
           sys: 34920,
           idle: 1072572010,
           irq: 30 } } ]
