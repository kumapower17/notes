package main

import (
	"log"
	"net"
	"strings"
	"time"
)

func main() {
	srv, err := net.Listen("tcp", "127.0.0.1:8999")
	if err != nil {
		panic(err)
	}

	addr := srv.Addr()
	log.Printf("tcp addr %s", addr.String())

	conn, err := srv.Accept()
	if err != nil {
		panic(err)
	}
	b := []byte(strings.Repeat("*", 4096))

	conn.SetWriteDeadline(time.Now().Add(10 * time.Second))
	for {
		_, err = conn.Write(b)
		if err != nil {
			log.Printf("write to client error %v", err)
			break
		}
	}

	if err = conn.Close(); err != nil {
		panic(err)
	}
	log.Println("server close sock")
	time.Sleep(time.Hour)
}
