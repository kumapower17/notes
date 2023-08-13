package main

func main() {
	var f = func(x int) {
		println(x)
	}
	var n = 1
	defer f(n)
	f = func(x int) {
		println(3)
	}
	n = 2
}
