"Make" to compile

"./demo" to watch a demo

go into "challenge" dir
"tar -xvzf in.tar.gz" to unzip all challenges matrices.
install package "parallel" to run multiple instances of some code.

"parallel --jobs 2 --eta "[ ! -f challenge/out/answer{} ] && cat challenge/in/flood{} | bin/flood_solve bx=9 by=9 > /tmp/out/answer{}" ::: $(seq 1 100000)" to solve the challenge

or 

parallel --eta -S 4/:,1/rpib,1/rpibp,4/sycorax,2/compaq,2/tosh "cat ~/local-docs/flood/challenge/in/flood{}|~/local-docs/flood/bin/flood_solve bx=9 by=9 mp=900" ::: {1..10000} > answers 

there is a program, called avg, that can show the average, and multiply by the end number of matrices to solve, to give an idea. usage : avg answers 10000

"i=123 ; cat challenge/out/answer$i challenge/in/flood$i | bin/flood_print bx=9 by=9" to display the solution for board 123
