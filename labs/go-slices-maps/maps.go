package main

import (
	"strings"
	"golang.org/x/tour/wc"
)

func WordCount(s string) map[string]int {
	total := make(map[string]int)
	words := strings.Fields(s)
	for i := range words {
		total[words[i]] = total[words[i]] + 1
	}
	return total
}

func main() {
	wc.Test(WordCount)
}
