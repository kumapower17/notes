package main

import "unsafe"

func f() {
	var v *int64 = nil
	println(unsafe.Sizeof(*v)) // 8
}

func g() {
	var t *struct{ s [][16]int } = nil
	println(len(t.s[99])) // 16
}

func main() {
	f()
	g()
}
