# Smalloc

    Stability: 1 - Experimental

## smalloc.alloc(length[, receiver])

<!--
* `length` Number `<= smalloc.kMaxLength`
* `receiver` Object, Optional, Default: `new Object`
-->

* `length` Number `<= smalloc.kMaxLength`
* `receiver` Object, オプション、デフォルトは `new Object`

<!--
Returns `receiver` with allocated external array data. If no `receiver` is
passed then a new Object will be created and returned.
-->

外部の配列データが割り当てられた `receiver` を返します。
`receiver` が渡されなかった場合は新しいオブジェクトが作成されて返されます。

<!--
Buffers are backed by a simple allocator that only handles the assignation of
external raw memory. Smalloc exposes that functionality.
-->

バッファは、外部の生のメモリを割り当てるだけの単純なアロケータに
支えられています。Smallocはその機能を公開します。

<!--
This can be used to create your own Buffer-like classes. No other properties are
set, so the user will need to keep track of other necessary information (e.g.
`length` of the allocation).
-->

これはバッファのような独自クラスの作成に用いることができます。
他のどんなプロパティも設定されないので、利用者は必要な情報
(例えば割り当てた長さの `length`) を追跡できるようにする必要があります。

    function SimpleData(n) {
      this.length = n;
      smalloc.alloc(this.length, this);
    }

    SimpleData.prototype = { /* ... */ };

<!--
It only checks if the `receiver` is an Object, and also not an Array. Because of
this it is possible to allocate external array data to more than a plain Object.
-->

`receiver` は配列以外のオブジェクトであることがチェックされるだけです。
このため、外部の配列データをプレーンなオブジェクト以上のものに
割り当てることができます。

    function allocMe() { }
    smalloc.alloc(3, allocMe);

    // { [Function allocMe] '0': 0, '1': 0, '2': 0 }

<!--
v8 does not support allocating external array data to an Array, and if passed
will throw.
-->

v8は外部の配列データを Array オブジェクトに割り当てることを
サポートしていないため、もし渡すと例外がスローされます。

## smalloc.copyOnto(source, sourceStart, dest, destStart, copyLength);

<!--
* `source` Object with external array allocation
* `sourceStart` Position to begin copying from
* `dest` Object with external array allocation
* `destStart` Position to begin copying onto
* `copyLength` Length of copy
-->

* `source` 外部の配列が割り当てられたオブジェクト
* `sourceStart` コピーを開始する位置
* `dest` 外部の配列が割り当てられたオブジェクト
* `destStart` コピーを開始する位置
* `copyLength` コピーする長さ

<!--
Copy memory from one external array allocation to another. No arguments are
optional, and any violation will throw.
-->

外部の配列に割り当てられたメモリの一つから別のメモリへコピーします。
全ての引数は必須で、どんな違反でも例外がスローされます。

    var a = smalloc.alloc(4);
    var b = smalloc.alloc(4);

    for (var i = 0; i < 4; i++) {
      a[i] = i;
      b[i] = i * 2;
    }

    // { '0': 0, '1': 1, '2': 2, '3': 3 }
    // { '0': 0, '1': 2, '2': 4, '3': 6 }

    smalloc.copyOnto(b, 2, a, 0, 2);

    // { '0': 4, '1': 6, '2': 2, '3': 3 }

<!--
`copyOnto` automatically detects the length of the allocation internally, so no
need to set any additional properties for this to work.
-->

`copyOnto()` は内部で割り当てられた長さを自動的に検出するので、
これが動作するために追加のプロパティを設定する必要はありません。

## smalloc.dispose(obj)

* `obj` Object

<!--
Free memory that has been allocated to an object via `smalloc.alloc`.
-->

`smalloc.alloc()` によってオブジェクトに割り当てられたメモリを解放します。

    var a = {};
    smalloc.alloc(3, a);

    // { '0': 0, '1': 0, '2': 0 }

    smalloc.dispose(a);

    // {}

<!--
This is useful to reduce strain on the garbage collector, but developers must be
careful. Cryptic errors may arise in applications that are difficult to trace.
-->

これはガーベッジコレクションの負荷を軽減しますが、開発者は注意が必要です。
不可解なエラーが追跡の難しいアプリケーションで発生するかもしれません。

    var a = smalloc.alloc(4);
    var b = smalloc.alloc(4);

    // perform this somewhere along the line
    smalloc.dispose(b);

    // now trying to copy some data out
    smalloc.copyOnto(b, 2, a, 0, 2);

    // now results in:
    // Error: source has no external array data

<!--
`dispose()` does not support Buffers, and will throw if passed.
-->

`dispose()` は `Buffer` をサポートしません。もし渡されると例外をスローします。

## smalloc.kMaxLength

<!--
Size of maximum allocation. This is also applicable to Buffer creation.
-->

割り当てられる最大長です。
これは `Buffer` を作成する際にも適用されます。
