/**
 * 
 * Bind the system functions.
*/
sysfunc.bind("console",
    {
        "log": "sysfunc::Console::log",
        "info": "sysfunc::Console::info",
        "warn": "sysfunc::Console::warn",
        "debug": "sysfunc::Console::debug",
        "error": "sysfunc::Console::error",
        "time": "sysfunc::Console::time",
        "timeEnd": "sysfunc::Console::timeEnd"
    }
);