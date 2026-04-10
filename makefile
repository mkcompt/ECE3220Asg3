#Kyle Compton (mkcompt)
 # ECE 3220
 #Assignment 3
 #04/14/2026
 #makefile

comp = gcc
compflags = -Wall -Wextra -g


raid5: raid5.c
	$(comp) $(compflags) raid5.c -o raid5 

clean:
	rm -f raid5