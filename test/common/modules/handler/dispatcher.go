package handler

import (
	"fmt"
	"net/http"
	"strings"
)

const (
	simple              = "/simple"
	simpleWithNamespace = "/scripts/simple"
)

// Dispatch the requests.
func Dispatch(writer http.ResponseWriter, request *http.Request) {
	uri := request.RequestURI
	// simple test
	if strings.HasPrefix(uri, simple) || strings.HasPrefix(uri, simpleWithNamespace) {
		simpleHandler(writer, request)
		return
	}
	fmt.Printf("Unknown uri '%s'\n", uri)
	writer.WriteHeader(404)
	writer.Write([]byte("Unknown uri '" + uri + "'\n"))
}
