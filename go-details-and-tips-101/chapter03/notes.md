# Syntax and Semantics Related

## 3.3 Don’t put a zero-size field as the final field of a struct type

> In the current standard Go runtime implementation, as long as a memory block is referenced by at least one alive pointer, that memory block will not be viewed as garbage and will not be collected.

> All the fields of an addressable struct value can be taken addresses. If the size of the final field in a non-zero-size struct value is zero, then taking the address of the final field in the struct value will return an address which is beyond the allocated memory block for the struct value. The returned address may point to another allocated memory block which closely follows the one allocated for the non-zero-size struct value. As long as the returned address is stored in an alive pointer value, the other allocated memory block will not get garbage collected, which may cause memory leaking.

``` go
package main

import "unsafe"

type Ty struct {
	_ [0]func()
	y int64
}

type Tz struct {
	z int64
	_ [0]func()
}

func main() {
	var y Ty
	var z Tz
	println(unsafe.Sizeof(y)) // 8
	println(unsafe.Sizeof(z)) // 16
}
```

## 3.5 There are several ways to create a slice

``` go
package main

func main() {
    var s0 = make([]int, 100)
    var s1 = []int{99: 0}
    var s2 = (&[100]int{})[:]
    var s3 = new([100]int)[:]

    // 100 100 100 100
    println(len(s0), len(s1), len(s2), len(s3))
}
```

## 3.6 for i, v = range aContainer actually iterates a copy of aContainer

``` go
package main

// 123
func main() {
	var a = [...]int{1, 2, 3}
	for i, n := range a {
		if i == 0 {
			a[1], a[2] = 8, 9
		}
		print(n)
	}
}
```

> If the ranged container is a large array, then the cost of making the copy will be large.

> There is an exception: if the second iteration variable in a for-range is omitted or ignored, then the ranged container will not get copied, because it is unnecessary to make the copy.

## 3.8 Some function calls are evaluated at compile time

``` go
package main

import (
	"unsafe"
)

func main() {
	const _ = unsafe.Sizeof(int64(0))
}
```

## 3.11 Composite literals are unaddressable, but they may be taken addresses

> Composite literals include struct/array/slice/map value literals. They are unaddressable. Generally speaking, unaddressable values may not be taken addresses. However, there is an exception (a syntax sugar) made in Go: composite literals may be taken addresses.

``` go
package main

type T struct{ x int }

func main() {
	// All the address taking operations are legal.
	_ = &T{}
	_ = &[8]byte{}
	_ = &[]byte{7: 0}
	_ = &map[int]bool{}
}
```

## 3.12 One-line trick to create pointers to a non-zero bool/numeric/string values

``` go
// var pb = &true
var pb = &(&[1]bool{true})[0]

// The following way is less verbose
// but a little less efficient.
var pb2 = &([]bool{true})[0]
```


## 3.23 For a deferred function call, its arguments and the called function expression are evaluated when the deferred call is registered

``` go
package main

func main() {
	var f = func(x int) {
		println(x)
	}
	var n = 1
	defer f(n)
	f = func(x int) {
		println(3)
	}
	n = 2
}

// 1
```

## 3.25 If the left operand of a non-constant bit-shift expression is untyped, then its type is determined as the assumed type of the expression

> The following program prints 002. The reason is the first two bit-shift expressions are both non-constant, so the respective untyped integer 1s in them are deduced as values of the assume type, byte, so each 1 << n is evaluated as 0 at run time (because 256 overflows byte values).

> On the other hand, the third bit-shift expression is a constant expression, so it is evaluated at compile time. In fact the whole expression (1 << N) / 128 is evaluated at compile time, as 2.

``` go
package main

func main() {
	var n = 8
	var x byte = 1 << n / 128
	print(x) // 0
	var y = byte(1 << n / 128)
	print(y) // 0

	const N = 8
	var z byte = 1 << N / 128
	println(z) // 2
}
```

## 3.32 The switch expression is optional and its default value is a typed value true of the builtin type bool
> In an expression switch, the switch expression is evaluated and the case expressions, which need not be constants, are evaluated left-to-right and top-to-bottom; the first one that equals the switch expression triggers execution of the statements of the associated case; the other cases are skipped. If no case matches and there is a "default" case, its statements are executed. There can be at most one default case and it may appear anywhere in the "switch" statement. A missing switch expression is equivalent to the boolean value true.

https://go.dev/ref/spec#Switch_statements

``` go
switch
case x < y: f1()
case x < z: f2()
case x == 4: f3()
}
```

## 3.38 Each method corresponds a function which first parameter is the receiver parameter of that method

For example, in the following code,
• the type `T` has one method `M1` which corresponds a function `T.M1`.
• the type `*T` has two methods: `M1` and `M2`, which correspond functions `(*T).M1` and `(*T).M2`, respectively.

``` go
package main

type T struct { X int }

func (t T) M1() int {
    return t.X
}

func (t *T) M2() int {
    return t.X
}

func main() {
    var t = T{X: 3}
    _ = T.M1(t)
    _ = (*T).M1(&t)
    _ = (*T).M2(&t)
}
```

## 3.46 A detail in const specification auto-complete

https://github.com/golang/go/issues/49157#issuecomment-1168162456

``` go
package main

const X = 3

func main() {
	const (
		X = X + X
		Y
	)
	println(X, Y)
}
```
> Running with Go 1.18 we get the result 6 and 12, which corresponds exactly to using textually X + X for Y as the spec prescribes.

> Running with 1.17 we get the result 6 and 6, corresponding to a copy of the syntax tree for X + X with the variables X resolved to the outer `X.

