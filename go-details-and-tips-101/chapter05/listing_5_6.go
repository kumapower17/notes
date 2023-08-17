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
