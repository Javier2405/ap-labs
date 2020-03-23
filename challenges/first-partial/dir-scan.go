package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) (int,int,int,int,int) {
	tmpDir := dir
	var directories, symlinks, devices, sockets, other int
	os.Chdir(tmpDir)
	err := filepath.Walk(".", func(path string, info os.FileInfo, err error) error {
		if err != nil {
			fmt.Printf("Error in the function Walk at %q: %v\n", path, err)
			return err
		}
		switch mode := info.Mode();{
		case mode.IsDir(): //Directories
			directories++
		case mode&os.ModeSymlink != 0: //Symbolic links
			symlinks++
		case mode&os.ModeDevice != 0: //Devices
			devices++
		case mode&os.ModeSocket != 0: //Sockets
			sockets++
		default: //Others
			other++
		}
		return nil
	})
	if err != nil {
		fmt.Printf("Error in the function Walk at %q: %v\n", tmpDir, err)
	}
	return directories, symlinks, devices, sockets, other
}

func createSpaces(num int) (string){
	var s = ""
	var spaces int = 4
	for ;num>9;{
		num=num/10
		spaces--
	}
	i:=0
	for  ;i<spaces; i++{
		s = s+" "
	}
	return s
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	dir, sym, dev, sock, other := scanDir(os.Args[1])

	fmt.Printf("+-------------------+--------+")
	fmt.Printf("\n| Path              |%s    |", os.Args[1])
	fmt.Printf("\n+-------------------+--------+")
	fmt.Printf("\n| Directories       |  %d %s|", dir,createSpaces(dir))
	fmt.Printf("\n| Symbolic Links    |  %d %s|", sym, createSpaces(sym))
	fmt.Printf("\n| Devices           |  %d %s|", dev, createSpaces(dev))
	fmt.Printf("\n| Sockets           |  %d %s|", sock, createSpaces(sock))
	fmt.Printf("\n| Other files       |  %d %s|", other, createSpaces(other))
	fmt.Printf("\n+-------------------+--------+\n")
}
