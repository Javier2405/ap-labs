package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"strings"
)

func handleConn2(conn net.Conn, place string) {
	for {
		hour := make([]byte, 11)
		_, err := conn.Read(hour)
		if err != nil {
			conn.Close()
			log.Print(err)
		} else {
			fmt.Printf("%s: %s", place, hour)
		}
	}
}

func main() {

	if len(os.Args[1:]) < 1 {
		fmt.Println("Wrong usage. See readme.")
		os.Exit(1)
	}

	for i := 0; i < len(os.Args[1:]); i++ {
		clockData := strings.Split(os.Args[i+1], "=")
		if len(clockData) != 2 {
			fmt.Println("Wrong usage. See readme.")
		}
		conn, err := net.Dial("tcp", clockData[1])
		if err != nil {
			log.Fatal(err)
		}
		go handleConn2(conn, clockData[0])

	}
	for {
	} //We need to make the main thread to continue in order to
	//avoid the go routines to finish

}
