package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"time"
)

func connection(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}

func main() {
	if len(os.Args[1:]) != 2 {
		fmt.Println("Wrong usage")
	}

	seconds, err := strconv.Atoi(os.Args[2])
	if err != nil {
		log.Print(err)
	}

	ping := make(chan int)
	pong := make(chan int)

	file, err := os.Create("pingpong_report.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	go connection(ping, pong)
	go connection(pong, ping)
	totalMessages := 0
	for x := 0; x < seconds; x++ {
		pong <- x
		time.Sleep(time.Duration(seconds) * time.Second)
		aux := <-ping
		totalMessages += aux
	}

	_, err = file.WriteString(fmt.Sprintf("Total messages sent: %d\nAverage meessage per second: %d\n", totalMessages, totalMessages/(seconds)))
	if err != nil {
		panic(err)
	}
}
