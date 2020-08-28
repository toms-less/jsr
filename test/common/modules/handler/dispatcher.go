package handler

import (
	"fmt"
	"net/http"
	"strings"
)

const (
	simple = "/simple"
)

// Dispatch of module server.
func Dispatch(writer http.ResponseWriter, request *http.Request) {
	uri := request.RequestURI
	// simple test
	if strings.HasPrefix(uri, simple) {
		simpleHandler(writer, request)
		return
	}
	fmt.Printf("Unkown uri '%s'\n", uri)
	writer.WriteHeader(404)
	writer.Write([]byte("Unkown uri '" + uri + "'\n"))
}
