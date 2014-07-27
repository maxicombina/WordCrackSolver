# WordCrack Solver

## What is it?
A tool to find valid words in a [WordCrack](http://www.wordcrack.com/) board.

## How to use it
After cloning, just run `make CONF=Release`. You will find the executable under `dist/Release/<architecurte>/wordcracksolver-st`.

### But I run `./wordcracksolver-st` and it complains!
You need to put a `dict.txt` and a `config` file alongside it.

 * The `dict.txt` must contain a list of valid words. There is already a file for spanish words (which is not
   complete and contains some words not considered valid by WordCrack --mainly some forms of verb conjugation)
 * The `config` file has the boards letters, and the special tiles (double letter, double word, triple letter, triple word). Detail of the `config` file:

### `config` file format

 * `letters` = 16 letters, that compose the board: ABCDEFGHIJKLMNOP represent the board:
      
``` 
        | A | B | C | D |
        | E | F | G | H |
        | I | J | K | L |
        | M | N | O | P |
```

 * Use the letter `q` for representing `Qu` and `#` for `Ñ`
 * `minLen` and `maxLen`: the minimum and maximun *path* length to compute. A word with **N** letters has **N-1** path length. Thus, `minLen 1` means "words of 2 letters", and `maxLen 15` means "words of 16 letters". In practice, paths lengths greater than 11 or 12 do not find any extra word, and add a lot of time of computation. Recomended values are `minLen=4` and `maxLen=10`
 * `dl`, `tl`, `dw` and `tw`: DoubleLetter, TripleLetter, DoubleWord and TripleWord modifiers. The upper left tile of the Board (`A` in the example above) has coordinates "00", `B` has "01", `C` has "02", ..., `E` has "10" and so on, up to `P` that has "33". Enter the coordinates for each modifier using "," to separate. Example:
 
```
        dl = 00, 11
        tl = 
        dw =
        tw = 31
```    


   This means that `A` and `F` have the DoubleLetter modifier, and that `N` has the TripleWord modifier.

## I want to hack it, tell me more

 * The project can be opened with Netbeans.
  * There are 2 configs, as usual, `Debug` and `Release`.

 * The `master` branch is a single-thread version, that computes all the possible paths every time. There are 2 other branches:
  * `mt` with multi-threading (though I did not find it specially faster. I must be missing something) and 
  * `path_preload`: all the possible paths are already pre-computed. This saves some time, but uses more memory.

## License
[GPL v3](http://www.gnu.org/licenses/gpl-3.0.txt)
