package handler

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
)

const (
	simple = "/simple"
)

func Dispatch(writer http.ResponseWriter, request *http.Request) {
	uri := request.RequestURI
	// simple test
	if strings.HasPrefix(uri, simple) {
		simpleTest(writer, request)
		return
	}
	fmt.Printf("Unkown uri '%s'\n", uri)
	writer.WriteHeader(404)
	writer.Write([]byte("Unkown uri '" + uri + "'\n"))
}

func simpleTest(writer http.ResponseWriter, request *http.Request) {
	base, err := os.Getwd()
	if err != nil {
		panic(err)
	}
	path := base + request.RequestURI
	content, err := ioutil.ReadFile(path)
	if err != nil {
		fmt.Printf("Error: %s\n", err)
		writer.WriteHeader(404)
		writer.Write([]byte(err.Error() + "\n"))
	}
	writer.WriteHeader(200)
	writer.Write(content)
}
