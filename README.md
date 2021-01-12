# BallSortSolver

## Description
Aims to be a solver for one of the many Ball Sort apps for iOS or Android. The aim of the game is to sort by color all of the colored balls in
a series of test tubes. The only rule is that you can only put a ball on top of another ball of the same color *or* in an empty vial.

The game that I used to test this solver out is called "Ball Sort Puzzle" by "IEC Global Pty Ltd" on the Google Play store.
I believe it goes under the same name on the Apple store as well.

First of all, there are *no guarantees* this will solve a particular puzzle, and if it does, it's probably not going to be the optimal solution.
I have worked out most of the bugs and it seems to work well, but I'm still not 100% sure it's flawless.

## Building

This program was built and tested on Ubuntu 18.04.5 LTS using the gcc 7.5.0 compiler. A makefile is included for use with GNU Make.

Simply run
`$ make`
to make the executable

## Usage

`$./solver <NumVials> <<Vial1 Contents> <Vial2 Contents>...>`

`NumVials` is the total number of vials for this puzzle.
Each `Vial Contents` should contain up to 4 characters, with each character corresponding to a color of ball

as an example:

`$./solver 3 ABAB BABA`

This will run the program with 3 vials, the last of which is an empty vial.
The first vial contains the color `A` on top, followed by the colors `B`, `A`, and `B` underneath.
Any character can be used for any color, as long as you're consistent with the labeling of course.

## License

Distributed under the GNU General Public License v3.0. See `LICENSE` for more information
