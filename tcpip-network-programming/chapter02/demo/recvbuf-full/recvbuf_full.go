package main

import (
	"log"
	"net"
	"strings"
	"time"
)

func main() {
	srv, err := net.Listen("tcp", "127.0.0.1:54940")
	if err != nil {
		panic(err)
	}

	addr := srv.Addr()
	log.Printf("tcp addr %s", addr.String())
	go func() {
		conn, err := net.Dial(addr.Network(), addr.String())
		if err != nil {
			panic(err)
		}
		data := []byte(strings.Repeat("*", 1000))
		for {
			n, err := conn.Write(data)
			if err != nil {
				panic(err)
			}
			log.Printf("write %d bytes", n)
		}
	}()

	conn, err := srv.Accept()
	if err != nil {
		panic(err)
	}
	<-time.NewTimer(time.Hour).C
	_ = conn.Close()
}
