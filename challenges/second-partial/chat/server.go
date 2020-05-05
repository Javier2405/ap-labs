// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering   = make(chan client)
	leaving    = make(chan client)
	messages   = make(chan string)            // all incoming client messages
	ip_clients = make(map[string]net.Conn)    //net.Conn conections
	conections = make(map[string]chan string) //channels connected per username
	clients_c  = make(map[string]bool)
	admin      string
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	who := ""
	w := make([]byte, 11)
	_, err := conn.Read(w)

	if err != nil {
		conn.Close()
		log.Print(err)
	} else {
		for i := range w {
			if w[i] != 0 {
				who = who + string(w[i])
			}
		}
	}

	//USED USERNAMES
	for i := range clients_c {
		if i == who {
			ch <- "This username is already in use"
			conn.Close()
			return
		}
	}

	input := bufio.NewScanner(conn)

	if admin == "" {
		ch <- "irc-server > Congrats, you were the first user."
		ch <- "irc-server > You're the new IRC Server ADMIN."
		fmt.Println("irc-server > [" + who + "] was promoted as the channel ADMIN")
		admin = who
	}
	ch <- "irc-server > Your user [" + who + "] is succesfully logged"
	messages <- "irc-server > New connected user[" + who + "]"
	fmt.Println("irc-server > New connected user[" + who + "]")
	entering <- ch

	ip_clients[who] = conn
	clients_c[who] = true
	conections[who] = ch

	for input.Scan() {
		aux := strings.Split(input.Text(), " ")
		if strings.Compare(input.Text(), "") == 0 { //AVOID EMPTY MESSAGES
		} else if strings.Compare(input.Text(), "/users") == 0 { //COMMAND /USERS
			users := ""
			ini := 0
			for i := range clients_c {
				if i != "" {
					if ini == 0 {
						users = i
						ini = 1
					} else {
						users = users + ", " + i
					}
				}
			}
			ch <- users
		} else if strings.Compare(aux[0], "/user") == 0 { //COMMAND /USER
			if userExist(clients_c, aux[1]) {
				aux_ip := strings.Split(ip_clients[aux[1]].RemoteAddr().String(), ":")
				ip := "irc-server > username: " + aux[1] + " ip: " + aux_ip[0]
				ch <- ip
			} else {
				ch <- "The username does not exist. User information failed"
			}
		} else if strings.Compare(aux[0], "/msg") == 0 { //COMMAND /MSG
			msg := who + " [private message] > "
			for i := range aux {
				if i >= 2 {
					msg = msg + " " + aux[i]
				}
			}
			if userExist(clients_c, aux[1]) {
				conections[aux[1]] <- msg
			} else {
				ch <- "The username does not exist. Private message failed"
			}

		} else if strings.Compare(aux[0], "/kick") == 0 { //COMMAND KICK
			if admin == who {
				if userExist(clients_c, aux[1]) {
					conections[aux[1]] <- "You're kicked from this channel"
					conections[aux[1]] <- "Bad language is not allowed on this channel"
					ip_clients[aux[1]].Close()
					messages <- "irc-server > [" + aux[1] + "] was kicked from channel for bad language policy violation"
					fmt.Println("irc-server > [" + aux[1] + "] was kicked from channel for bad language policy violation")
				} else {
					ch <- "The username does not exist. Kicked failed"
				}
			} else {
				ch <- "irc-server > You can't use this command because you are not an admin"
			}

		} else if strings.Compare(input.Text(), "/time") == 0 { //COMMAND TIME
			val, ok := os.LookupEnv("TZ")
			time1 := ""
			if ok {
				time1 = "irc-server > Local Time: " + val + " " + time.Now().Format("15:04")
			} else {
				time1 = "irc-server > Local Time: America/Mexico_City " + time.Now().Format("15:04")
			}
			ch <- time1
		} else if string(input.Text()[0]) == "/" {
			ch <- "Command not valid"
		} else {
			messages <- who + " > " + input.Text()
			fmt.Println(who + " > " + input.Text())
		}
	}
	// ADMIN CONTROL
	delete(clients_c, who)
	fmt.Println("[" + who + "] left channel")
	if who == admin {
		for i := range clients_c {
			fmt.Println(i)
			admin = i
			conections[i] <- "The admin log out. Now you are the admin"
			fmt.Println("irc-server > [" + i + "] was promoted as the channel ADMIN")
			break
		}
		if admin == who {
			fmt.Println("irc-server > There is not channel ADMIN")
			admin = ""
		}
	}
	leaving <- ch

	messages <- "[" + who + "] left channel"

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

func userExist(clients_c map[string]bool, aux string) bool {
	for i := range clients_c {
		if i == aux {
			return true
		}
	}
	return false
}

//!+main
func main() {
	if len(os.Args[1:]) < 4 {
		fmt.Println("Wrong usage. See readme.")
		os.Exit(1)
	}
	host := os.Args[2]
	port := os.Args[4]

	listener, err := net.Listen("tcp", host+":"+port)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("irc-server > Simple IRC Server started at %s:%s", host, port)
	fmt.Println()
	go broadcaster()
	fmt.Println("irc-server > Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
