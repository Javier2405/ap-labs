package main

import (
	"fmt"
	"log"
	"os"
	"runtime"
	"strconv"
	"time"
)

var idle = make(chan struct{})

func pipeline(in chan struct{}, out chan struct{}, current int, total int) {
	if current != total {
		go pipeline(out, in, current+1, total)
		out <- <-in
	} else {
		idle <- struct{}{}
	}
}

func memoryUsage() string {
	var memory runtime.MemStats
	runtime.ReadMemStats(&memory)
	return fmt.Sprintf("%v MiB \n", memory.Sys/1024/1024)

}

func main() {
	if len(os.Args[1:]) != 2 {
		fmt.Println("Wrong usage")
	}

	totalPipes, err := strconv.Atoi(os.Args[2])
	if err != nil {
		log.Print(err)
	}
	file, err := os.Create("pipeline_report.txt")
	if err != nil {
		log.Print(err)
	}

	defer file.Close()

	_, err = file.WriteString("Memory usage before the pipeline:" + memoryUsage())
	if err != nil {
		log.Print(err)
	}

	in := make(chan struct{})

	start := time.Now()
	go pipeline(in, make(chan struct{}), 0, totalPipes)

	in <- struct{}{}
	<-idle

	_, err = file.WriteString(fmt.Sprintf("Memory usage after the pipeline: %s \nTime: %s \nTotal of pipelines: %d", memoryUsage(), time.Since(start), totalPipes))
	if err != nil {
		log.Print(err)
	}
}
