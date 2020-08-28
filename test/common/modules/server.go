package main

import (
	"log"
	"modules/handler"
	"net/http"
)

func main() {
	http.HandleFunc("/", handler.Dispatch)
	err := http.ListenAndServe("127.0.0.1:8080", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
