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
