/**
 * 
 * Bind the system functions.
*/
toms.bind("http",
    {
        "get": "sysfunc::Http::get",
        "post": "sysfunc::Http::post",
        "options": "sysfunc::Http::options",
        "patch": "sysfunc::Http::patch",
        "put": "sysfunc::Http::put",
        "delete": "sysfunc::Http::del"
    }
);