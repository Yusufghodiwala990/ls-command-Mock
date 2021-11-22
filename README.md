# ls-command-Mock
This C program imitates what an 'ls' command does in UNIX like operating systems. Arguments can be passed to the program to modify and filter the results(files) from the command.
Arguments may include "-y, -o, -l, -s" which are "younger than, older than, larger than, smaller than". Multiple arguments can be passed to filter the search results. e.g
$ "[Path] -y 4 -o 1 -s 4000 -l 1500" - This means look for a file that is younger than 4 days but older than 1 day, that is smaller than 4000 bytes but larger than 1500 bytes
