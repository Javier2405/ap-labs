package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	var a[][]uint8
	a = make([][]uint8, dy)
	for i:= 0; i < dy; i++ {
		a[i] = make([]uint8, dx)
		for j:=0; j < dx; j++ {
			a[i][j] = uint8((1500*j^i)^(j*i/(3*(j^i)+1)))
		}
	}
	return a
}

func main() {
	pic.Show(Pic)
}
