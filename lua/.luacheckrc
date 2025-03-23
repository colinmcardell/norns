-- Global objects from norns
globals = {
    -- Core objects
    "audio",
    "clock",
    "controlspec",
    "crow",
    "engine",
    "grid",
    "hook",
    "hid",
    "midi",
    "norns",
    "osc",
    "params",
    "poll",
    "screen",
    "softcut",
    "tab",
    "util",
    "wifi",
    -- Global constants
    "_norns",
    "_path",
    "paths",
    "_VERSION",
    -- Test framework
    "describe",
    "it",
    "assert"
}

-- Ignore unused self parameters in methods
self = false

-- Don't report unused arguments starting with _
allow_defined_top = true

-- Relax line length limit slightly from default
max_line_length = 100

-- File patterns to exclude
exclude_files = {
    "lua/lib/test/luaunit.lua"
}
