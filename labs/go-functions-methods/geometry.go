// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

//package geometry

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

// A Path is a journey connecting the points with straight lines.
type Path []Point

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
/*func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}*/

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func randomPoint() Point {
	rand.Seed(time.Now().UnixNano())
	return Point{math.Round((rand.Float64() * 200) - 100), math.Round((rand.Float64() * 200) - 100)}
}

func generateFigure(vertices int) Path {
	var point Point
	var ret Path
	var flag bool = true
	for i := 0; i < vertices; i++ {
		if i <= 2 {
			point = randomPoint()
			ret = append(ret, point)
		} else {
			for flag {
				point = randomPoint()
				for j := 0; j < len(ret)-2; j++ {
					if !doIntersect(ret[j], ret[j+1], ret[i-1], point) {
						flag = false
					} else {
						flag = true
						break
					}
				}
			}
			ret = append(ret, point)
			flag = true
		}
	}
	return ret
}

func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)
	if (o1 != o2) && (o3 != o4) {
		return true
	}
	if (o1 == 0) && onSegment(p1, p2, q1) {
		return true
	}
	if (o2 == 0) && onSegment(p1, q2, q1) {
		return true
	}
	if (o3 == 0) && onSegment(p2, p1, q2) {
		return true
	}
	if (o4 == 0) && onSegment(p2, q1, q2) {
		return true
	}
	return false
}

func onSegment(p, q, r Point) bool {
	if (q.X() <= math.Max(p.X(), r.X())) && (q.X() >= math.Min(p.X(), r.X())) && (q.Y() <= math.Max(q.Y(), r.Y())) && (q.Y() >= math.Min(p.Y(), r.Y())) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 {
		return 0
	}
	if val > 0 {
		return 1
	}
	return 2
}

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := 0; i < len(path)-1; i++ {
		//fmt.Printf("Distance from %d to %d : %f\n", i, i+1, Distance(path[i], path[i+1]))
		sum += Distance(path[i], path[i+1])
	}
	//fmt.Printf("Distance from %d to %d : %f\n", len(path)-1, 0, Distance(path[len(path)-1], path[0]))
	sum += Distance(path[len(path)-1], path[0])
	return sum
}

func main() {
	vertices, _ := strconv.Atoi(os.Args[1])
	if vertices < 3 {
		fmt.Println("\nWe cant build a figure with that number of vertex")
		return
	}
	fmt.Printf("\n- Generating a [%d] vertices figure", vertices)
	fmt.Println("\n- Figure's vertices")
	figure := Path{}
	figure = generateFigure(vertices)
	for i := 0; i < len(figure); i++ {
		fmt.Printf("Point: {%f , %f}\n", figure[i].X(), figure[i].Y())
	}
	fmt.Printf("The sum of the perimeter is: %f\n", figure.Distance())
}
