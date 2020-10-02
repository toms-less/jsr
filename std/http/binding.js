/**
 * 
 * Bind the system functions.
*/
jsr.bind("http",
    {
        "get": "binding::Http::get",
        "post": "binding::Http::post",
        "options": "binding::Http::options",
        "patch": "binding::Http::patch",
        "put": "binding::Http::put",
        "delete": "binding::Http::del"
    }
);