## Assert

<!--
This module is used for writing unit tests for your applications, you can
access it with `require('assert')`.
-->
このモジュールはアプリケーションの単体テストを記述するために使用され、
`require('assert')` でアクセスできます。

### assert.fail(actual, expected, message, operator)

<!--
Tests if `actual` is equal to `expected` using the operator provided.
-->
`actual` が `expected` と等しいか、提供された演算子を使ってテストします。

### assert.ok(value, [message])

<!--
Tests if value is a `true` value, it is equivalent to `assert.equal(true, value, message);`
-->
`value` が `true` かテストします、
これは `assert.equal(true, value, message);` と等価です。

### assert.equal(actual, expected, [message])

<!--
Tests shallow, coercive equality with the equal comparison operator ( `==` ).
-->
`==` 演算子を強制して浅い同値性をテストします。

### assert.notEqual(actual, expected, [message])

<!--
Tests shallow, coercive non-equality with the not equal comparison operator ( `!=` ).
-->
`==` 演算子を強制して浅い非同値性をテストします。

### assert.deepEqual(actual, expected, [message])

<!--
Tests for deep equality.
-->
深い同値性をテストします。

### assert.notDeepEqual(actual, expected, [message])

<!--
Tests for any deep inequality.
-->
深い非同値性をテストします。

### assert.strictEqual(actual, expected, [message])

<!--
Tests strict equality, as determined by the strict equality operator ( `===` )
-->
`===` 演算子で厳密な同値性をテストします。

### assert.notStrictEqual(actual, expected, [message])

<!--
Tests strict non-equality, as determined by the strict not equal operator ( `!==` )
-->
`!==` 演算子で厳密な非同値性をテストします。

### assert.throws(block, [error], [message])

<!--
Expects `block` to throw an error.
-->
`block` がエラーをスローすることを期待します。

Expects `block` to throw an error. `error` can be constructor, regexp or 
validation function.

Validate instanceof using constructor:

    assert.throws(
      function() {
        throw new Error("Wrong value");
      },
      Error
    );

Validate error message using RegExp:

    assert.throws(
      function() {
        throw new Error("Wrong value");
      },
      /value/
    );

Custom error validation:

    assert.throws(
      function() {
        throw new Error("Wrong value");
      },
      function(err) {
        if ( !(err instanceof Error) || !/value/.test(err) ) {
          return false;
        }
      },
      "unexpected error"
    );

### assert.doesNotThrow(block, [error], [message])

<!--
Expects `block` not to throw an error, see assert.throws for details.
-->
`block` がエラーをスローしないことを期待します。

### assert.ifError(value)

<!--
Tests if value is not a false value, throws if it is a true value. Useful when
testing the first argument, `error` in callbacks.
-->
`value` が false でないことをテストし、true だったらそれをスローします。
コールバックの第 1 引数である `error` をテストするのに便利です。
