package main

//go:noinline
func f(i int) byte {
	var a [1 << 12]byte
	return a[i]
}

func main() {
	var x int
	println(&x)
	f(100) // make stack grow
	println(&x)
}
