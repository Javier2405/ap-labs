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

type webPage struct {
	url   string
	depth int
}

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(link webPage, last bool) []webPage {
	aux := make([]webPage, 0)
	fmt.Println(link.url)
	if !last {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(link.url)
		if err != nil {
			log.Print(err)
		}
		<-tokens // release the token
		for _, n := range list {
			aux = append(aux, webPage{url: n, depth: link.depth + 1})
		}
	}
	return aux
}

//!-sema

//!+
func main() {
	worklist := make(chan []webPage)
	var n int // number of pending sends to worklist

	if len(os.Args[1:]) < 2 {
		fmt.Println("Wrong usage. See readme.")
		os.Exit(1)
	}

	runes := []rune(os.Args[1])
	aux := string(runes[7:])
	desire_depth, err := strconv.Atoi(aux)
	if err != nil {
		log.Print(err)
	}
	// Start with the command-line arguments.
	n++
	go func() {
		urls := os.Args[1:]
		aux := make([]webPage, 0)
		for _, url := range urls {
			aux = append(aux, webPage{url: url, depth: 0})
		}
		worklist <- aux
	}()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.url] {
				seen[link.url] = true
				if link.depth < desire_depth {
					n++
					go func(link webPage) {
						worklist <- crawl(link, false)
					}(link)
				} else if link.depth == desire_depth {
					n++
					go func(link webPage) {
						worklist <- crawl(link, true)
					}(link)
				}
			}
		}
	}
}

//!-
