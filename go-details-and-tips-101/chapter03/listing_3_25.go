package main

func main() {
	var n = 8
	var x byte = 1 << n / 128
	print(x) // 0
	var y = byte(1 << n / 128)
	print(y) // 0

	const N = 8
	var z byte = 1 << N / 128
	println(z) // 2
}
