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
