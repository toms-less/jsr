
function home(request, response) {
    try {
        let headers = request.headers();
        response.send(JSON.stringify(headers));
    } catch (error) {
    }
}