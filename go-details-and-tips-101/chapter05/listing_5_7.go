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
