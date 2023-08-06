package main

import (
	"log"
	"net"
	"time"
)

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:8999")
	if err != nil {
		panic(err)
	}
	log.Printf("dial %s ok", conn.RemoteAddr())
	time.Sleep(time.Hour)
}
