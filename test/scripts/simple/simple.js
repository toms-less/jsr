import { sum, div } from 'http://127.0.0.1/simple/module/math.js'
import { append } from 'http://127.0.0.1/simple/module/strings.js'

export function simple(request, response) {
    try {
        let headers = request.headers();
        let num = sum(headers.sum1, headers.sum2) + div(headers.div1, headers.div2);
        let data = request.data();

        response.send(append(num, data));
    } catch (error) {
    }
}