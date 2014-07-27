# WordCrack Solver

## What is it?
A tool to find valid words in a [WordCrack](http://www.wordcrack.com/) board.

## How to use it
After cloning, just run `make CONF=Release`. You will find the executable under `dist/Release/<architecurte>/wordcracksolver-st`.

### But I run `./wordcracksolver-st` and it complains!
Yes, you need to put a `dict.txt` and a `config` file alongside it.

 * The `dict.txt` must contain a list of valid words. There is already a file for spanish words (which is not
   complete and contains some words not considered valid by WordCrack --mainly some forms of verb conjugation)
 * The `config` file has the boards letters, and the special tiles (double letter, double word, triple letter, triple word). Detail of the `config` file:
   * `letters` = 16 letters, that compose the board: ABCDEFGHIJKLMNOP represent the board:
      
        |ABCD|
        |EFGH|
        |IJKL|
        |MNOP|


## I hant to hack it, tell me more


## License
[GPL v3](http://www.gnu.org/licenses/gpl-3.0.txt)
