// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"strconv"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

//!-sema

//!+
func main() {
	worklist := make(chan []string)
	var n int // number of pending sends to worklist

	if len(os.Args[1:]) != 2 {
		fmt.Println("Wrong usage. See readme.")
		os.Exit(1)
	}

	runes := []rune(os.Args[1])
	aux := string(runes[7:])
	desire_depth, err := strconv.Atoi(aux)
	if err != nil {
		log.Print(err)
	}

	depth := 0
	// Start with the command-line arguments.
	n++
	go func() { worklist <- os.Args[2:] }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				if depth <= desire_depth {
					n++
					go func(link string) {
						worklist <- crawl(link)
					}(link)
				}
			}
		}
		depth++
	}
}

//!-
