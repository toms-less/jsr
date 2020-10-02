/**
 * 
 * Bind the system functions.
*/
jsr.bind("console",
    {
        "log": "binding::Console::log",
        "info": "binding::Console::info",
        "warn": "binding::Console::warn",
        "debug": "binding::Console::debug",
        "error": "binding::Console::error",
        "time": "binding::Console::time",
        "timeEnd": "binding::Console::timeEnd"
    }
);