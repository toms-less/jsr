package handler

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
)

func simpleHandler(writer http.ResponseWriter, request *http.Request) {
	base, err := os.Getwd()
	if err != nil {
		panic(err)
	}
	path := base + scriptHome + request.RequestURI
	content, err := ioutil.ReadFile(path)
	if err != nil {
		fmt.Printf("Error: %s\n", err)
		writer.WriteHeader(404)
		writer.Write([]byte(err.Error() + "\n"))
	}
	writer.WriteHeader(200)
	writer.Write(content)
}
