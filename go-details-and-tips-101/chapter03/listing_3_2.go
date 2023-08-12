package main

var g *[0]int
var a, b [0]int

//go:noinline
func f() *[0]int { return new([0]int) }

func main() {
	// x and y are allocated on stack.
	var x, y, z, w [0]int
	g = &z
	g = &w

	// Make z and w escape to heap.
	println(&b == &a)  // false
	println(&x == &y)  // false
	println(&z == &w)  // true
	println(&z == f()) // true
}
