# Comparisons Related

## 5.2 More ways to compare byte slices

> The first way: bytes.Compare(x, y) == 0.
> The second way: `string(x) == string(y)`. Due to an optimization made by the oﬀicial standard Go compiler, no underlying bytes will be duplicated in this way. In fact, the `bytes.Equal` function uses this way to do the comparison.

https://cs.opensource.google/go/go/+/refs/tags/go1.21.0:src/bytes/bytes.go;l=18

https://stackoverflow.com/questions/35911953/which-optimisations-does-the-go-1-6-compiler-apply-when-converting-between-byt

## 5.6 Struct values are compared field by field

> when comparing two struct values, their fields will be compared one by one. Once two corresponding fields are found unequal, the whole comparison stops and a false result is resulted. The whole comparison might also stop for a panic produced in comparing two interfaces.

``` go
package main

type T struct {
	x interface{}
	y interface{}
}

func main() {
	var a = T{x: 1, y: func() {}}
	var b = T{x: 2, y: func() {}}
	println(a == b) // false

	var c = T{x: 2, y: func() {}}
	var d = T{x: 2, y: func() {}}
	println(c == d) // panics
}
```

## 5.7 The _ fields in struct comparisons are ignored

``` go
package main

type T struct {
	_ int
	x string
}

func main() {
	var x = T{123, "Go"}
	var y = T{789, "Go"}
	println(x == y) // true
}
```
But please note that, as shown in a previous section, if a struct type contains a _ field of an incomparable type, then the struct type is also incomparable.

