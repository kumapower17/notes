package main

import "unsafe"

type A [0][256]int

type S struct {
	x A

	y [1 << 30]A

	z [1 << 30]struct{}
}

type T [1 << 30]S

func main() {
	var a A
	var s S
	var t T
	println(unsafe.Sizeof(a)) // 0
	println(unsafe.Sizeof(s)) // 0
	println(unsafe.Sizeof(t)) // 0
}
