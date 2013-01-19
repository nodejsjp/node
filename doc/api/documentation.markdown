# About this Documentation

<!-- type=misc -->

<!--
The goal of this documentation is to comprehensively explain the Node.js
API, both from a reference as well as a conceptual point of view.  Each
section describes a built-in module or high-level concept.

Where appropriate, property types, method arguments, and the arguments
provided to event handlers are detailed in a list underneath the topic
heading.

Every `.html` document has a corresponding `.json` document presenting
the same information in a structured manner.  This feature is
experimental, and added for the benefit of IDEs and other utilities that
wish to do programmatic things with the documentation.

Every `.html` and `.json` file is generated based on the corresponding
`.markdown` file in the `doc/api/` folder in node's source tree.  The
documentation is generated using the `tools/doc/generate.js` program.
The HTML template is located at `doc/template.html`.
-->

このドキュメントのゴールは、Node.js の API についてリファレンスとしても，
概念的な視点としても，包括的な説明をすることです。
それぞれのセクションは組込のモジュールまたは高水準の概念について記述します。

必要に応じて、プロパティの型やメソッドの引数、そしてイベントハンドラに
与えられる引数などの詳細は見出し直後のリストで与えられます。

すべての `.html` ドキュメントは、対応する `.json` ドキュメントを持ちます。
それは同じ情報を同様の構造で表現します。
これは実験的で、ドキュメントをプログラム的に扱いたい IDE や他の
ユーティリティのために加えられました。

すべての `.html` と `.json` ファイルは、node ソースツリーの `doc/api/`
フォルダにある、対応する `.markdown` ファイルを基に生成されます。
ドキュメントの生成には `tools/doc/generate.js` が使われます。
HTML のテンプレートは `doc/template.html` にあります。

## Stability Index

<!--type=misc-->

<!--
Throughout the documentation, you will see indications of a section's
stability.  The Node.js API is still somewhat changing, and as it
matures, certain parts are more reliable than others.  Some are so
proven, and so relied upon, that they are unlikely to ever change at
all.  Others are brand new and experimental, or known to be hazardous
and in the process of being redesigned.
-->

ドキュメント全体を通して、セクションの安定度に関する目安を見ることが
できるでしょう。
Node.js の API はまだ少し変更されます。
それが成熟することにより、ある部分は他よりも信頼できるようになります。
いくつかはそのように証明され、したがって信頼され、それらはおそらく
変更されそうもありません。
その他は新しくて実験的か、危険が知られていたり、再実装が始まっていたりします。

<!--
The stability indices are as follows:
-->

Stability (安定度) を以下のように示します:

<!--
```
Stability: 0 - Deprecated
This feature is known to be problematic, and changes are
planned.  Do not rely on it.  Use of the feature may cause warnings.  Backwards
compatibility should not be expected.
```
-->

```
Stability: 0 - 廃止予定
この機能には問題があることが知られていて、変更が計画されています。
これに依存しないでください。この機能を使用すると警告が出されるでしょう。
後方互換性を期待すべきではありません。
```

<!--
```
Stability: 1 - Experimental
This feature was introduced recently, and may change
or be removed in future versions.  Please try it out and provide feedback.
If it addresses a use-case that is important to you, tell the node core team.
```
-->

```
Stability: 1 - 実験的
この機能は最近導入され、将来のバージョンで変更されるか削除されるかもしれません。
それを試してフィードバックをしてください。
重要なユースケースで使われるなら、node コアチームに教えてください。
```

<!--
```
Stability: 2 - Unstable
The API is in the process of settling, but has not yet had
sufficient real-world testing to be considered stable. Backwards-compatibility
will be maintained if reasonable.
```
-->

```
Stability: 2 - 不安定
API は安定化の途中ですが、まだ安定していると考えられるほどには
現実世界でテストされていません。
もし合理的なら後方互換性が維持されるでしょう。
```

<!--
```
Stability: 3 - Stable
The API has proven satisfactory, but cleanup in the underlying
code may cause minor changes.  Backwards-compatibility is guaranteed.
```
-->

```
Stability: 3 - 安定
API は要求を満たすことがわかりましたが、実装コードをクリーンナップするために
小さな変更が行われるかもしれません。
後方互換性は保証されます。
```

<!--
```
Stability: 4 - API Frozen
This API has been tested extensively in production and is
unlikely to ever have to change.
```
-->

```
Stability: 4 - API 凍結
API は実運用で広範囲にテストされており、おそらく変更されることはありません。
```

<!--
```
Stability: 5 - Locked
Unless serious bugs are found, this code will not ever
change.  Please do not suggest changes in this area; they will be refused.
-->

```
Stability: 5 - 固定
深刻なバグが見つからない限り、コードは変更されません。
このエリアの変更を提案しないでください; そえは拒否されます。
```

## JSON Output

<!--
    Stability: 1 - Experimental
-->

    Stability: 1 - 実験的

<!--
Every HTML file in the markdown has a corresponding JSON file with the
same data.
-->

markdown から作られる全ての HTML ファイルは、対応する JSON ファイルを持ちます。

<!--
This feature is new as of node v0.6.12.  It is experimental.
-->

これは v0.6.12 からの新機能で、実験的です。
