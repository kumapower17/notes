# Runtime Related

## 6.2 64-bit word alignment problem

https://godbolt.org/z/a9P8o1P15

> 64-bit atomic operations on a 64-bit integer require the address of the 64-bit integer must be 8-byte aligned in memory. On 64-bit architectures, 64-bit integers are always 8-byte aligned, so the requirement is always satisfied on 64-bit architectures. This is not always true on 32-bit architectures.
>
> The docs of the sync/atomic standard package states that a qualified Go compiler should make sure that the first (64-bit) word (think it as an int64 or uint64 integer) in a (declared) variable or in an allocated struct, array, or slice can be relied upon to be 64-bit aligned.

----

> On ARM, 386, and 32-bit MIPS, it is the caller's responsibility to arrange for 64-bit alignment of 64-bit words accessed atomically via the primitive atomic functions (types Int64 and Uint64 are automatically aligned). The first word in an allocated struct, array, or slice; in a global variable; or in a local variable (because the subject of all atomic operations will escape to the heap) can be relied upon to be 64-bit aligned.

https://pkg.go.dev/sync/atomic#pkg-note-BUG

https://groups.google.com/g/golang-nuts/c/_nHK6P8_lhw

https://groups.google.com/g/golang-nuts/c/2BVboKRzDdo

https://godbolt.org/z/j4zPfTq5v

http://www.catb.org/esr/structure-packing/#_go

## 6.6 The address of a value might change at run time

``` go
package main

//go:noinline
func f(i int) byte {
	var a [1 << 12]byte
	return a[i]
}

func main() {
	var x int
	println(&x)
	f(100)
	// make stack grow
	println(&x)
}
```
