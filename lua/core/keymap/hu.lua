local char_modifier = require 'core/keymap/char_modifier'

local k = {}

k[char_modifier.NONE] = {}
k[char_modifier.SHIFT] = {}
k[char_modifier.ALTGR] = {}

k[char_modifier.NONE].SPACE = ' '
k[char_modifier.NONE].A = 'a'
k[char_modifier.NONE].B = 'b'
k[char_modifier.NONE].C = 'c'
k[char_modifier.NONE].D = 'd'
k[char_modifier.NONE].E = 'e'
k[char_modifier.NONE].F = 'f'
k[char_modifier.NONE].G = 'g'
k[char_modifier.NONE].H = 'h'
k[char_modifier.NONE].I = 'i'
k[char_modifier.NONE].J = 'j'
k[char_modifier.NONE].K = 'k'
k[char_modifier.NONE].L = 'l'
k[char_modifier.NONE].M = 'm'
k[char_modifier.NONE].N = 'n'
k[char_modifier.NONE].O = 'o'
k[char_modifier.NONE].P = 'p'
k[char_modifier.NONE].Q = 'q'
k[char_modifier.NONE].R = 'r'
k[char_modifier.NONE].S = 's'
k[char_modifier.NONE].T = 't'
k[char_modifier.NONE].U = 'u'
k[char_modifier.NONE].V = 'v'
k[char_modifier.NONE].W = 'w'
k[char_modifier.NONE].X = 'x'
k[char_modifier.NONE].Y = 'z'
k[char_modifier.NONE].Z = 'y'
k[char_modifier.NONE]['1'] = '1'
k[char_modifier.NONE]['2'] = '2'
k[char_modifier.NONE]['3'] = '3'
k[char_modifier.NONE]['4'] = '4'
k[char_modifier.NONE]['5'] = '5'
k[char_modifier.NONE]['6'] = '6'
k[char_modifier.NONE]['7'] = '7'
k[char_modifier.NONE]['8'] = '8'
k[char_modifier.NONE]['9'] = '9'
k[char_modifier.NONE].GRAVE = '0'
k[char_modifier.NONE].BACKSLASH = '\\'
k[char_modifier.NONE].COMMA = ','
k[char_modifier.NONE].DOT = '.'
k[char_modifier.NONE].SLASH = '-'
k[char_modifier.SHIFT].SPACE = ' '
k[char_modifier.SHIFT].A = 'A'
k[char_modifier.SHIFT].B = 'B'
k[char_modifier.SHIFT].C = 'C'
k[char_modifier.SHIFT].D = 'D'
k[char_modifier.SHIFT].E = 'E'
k[char_modifier.SHIFT].F = 'F'
k[char_modifier.SHIFT].G = 'G'
k[char_modifier.SHIFT].H = 'H'
k[char_modifier.SHIFT].I = 'I'
k[char_modifier.SHIFT].J = 'J'
k[char_modifier.SHIFT].K = 'K'
k[char_modifier.SHIFT].L = 'L'
k[char_modifier.SHIFT].M = 'M'
k[char_modifier.SHIFT].N = 'N'
k[char_modifier.SHIFT].O = 'O'
k[char_modifier.SHIFT].P = 'P'
k[char_modifier.SHIFT].Q = 'Q'
k[char_modifier.SHIFT].R = 'R'
k[char_modifier.SHIFT].S = 'S'
k[char_modifier.SHIFT].T = 'T'
k[char_modifier.SHIFT].U = 'U'
k[char_modifier.SHIFT].V = 'V'
k[char_modifier.SHIFT].W = 'W'
k[char_modifier.SHIFT].X = 'X'
k[char_modifier.SHIFT].Y = 'Z'
k[char_modifier.SHIFT].Z = 'Y'
k[char_modifier.SHIFT]['0'] = ')'
k[char_modifier.SHIFT]['1'] = '\''
k[char_modifier.SHIFT]['2'] = '"'
k[char_modifier.SHIFT]['3'] = '+'
k[char_modifier.SHIFT]['4'] = '!'
k[char_modifier.SHIFT]['5'] = '%'
k[char_modifier.SHIFT]['6'] = '/'
k[char_modifier.SHIFT]['7'] = '='
k[char_modifier.SHIFT]['8'] = '('
k[char_modifier.SHIFT]['9'] = ')'
k[char_modifier.SHIFT].MINUS = '_'
k[char_modifier.SHIFT].EQUAL = '+'
k[char_modifier.SHIFT].LEFTBRACE = '{'
k[char_modifier.SHIFT].RIGHTBRACE = '}'
k[char_modifier.SHIFT].BACKSLASH = '|'
k[char_modifier.SHIFT].SEMICOLON = ':'
k[char_modifier.SHIFT].APOSTROPHE = '"'
k[char_modifier.SHIFT].COMMA = '<'
k[char_modifier.SHIFT].DOT = ':'
k[char_modifier.SHIFT].SLASH = '_'
k[char_modifier.ALTGR]['1'] = '~'
k[char_modifier.ALTGR]['3'] = '^'
k[char_modifier.ALTGR]['7'] = '`'
k[char_modifier.ALTGR].Q = '('
k[char_modifier.ALTGR].W = '|'
k[char_modifier.ALTGR].F = '['
k[char_modifier.ALTGR].G = ']'
k[char_modifier.ALTGR].SEMICOLON = '$'
k[char_modifier.ALTGR].Z = '>'
k[char_modifier.ALTGR].X = '#'
k[char_modifier.ALTGR].C = '&'
k[char_modifier.ALTGR].V = '@'
k[char_modifier.ALTGR].B = '{'
k[char_modifier.ALTGR].N = '}'
k[char_modifier.ALTGR].SLASH = '*'


return k
