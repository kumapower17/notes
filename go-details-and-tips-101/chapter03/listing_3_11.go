package main

type T struct{ x int }

func main() {
	// All the address taking operations are legal.
	_ = &T{}
	_ = &[8]byte{}
	_ = &[]byte{7: 0}
	_ = &map[int]bool{}
}
