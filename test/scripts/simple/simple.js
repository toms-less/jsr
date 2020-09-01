import { sum, div } from 'http://127.0.0.1:8080/simple/module/math.js'
import { append } from 'http://127.0.0.1:8080/simple/module/strings.js'

export function simple(request, response) {
    try {
        let headers = request.headers();
        let num = sum(parseInt(headers.sum1), parseInt(headers.sum2)) + div(parseInt(headers.div1), parseInt(headers.div2));
        let data = request.data();

        response.send(append(num, data));
    } catch (error) {
    }
}