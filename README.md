"Make" to compile

"./demo" to watch a demo

go into "challenge" dir
"tar -xvzf in.tar.gz" to unzip all challenges matrices.
install package "parallel" to run multiple instances of some code.

"parallel --jobs 2 --eta "[ ! -f challenge/out/answer{} ] && cat challenge/in/flood{} | bin/flood_solve bx=9 by=9 > /tmp/out/answer{}" ::: $(seq 1 100000)" to solve the challenge

"i=123 ; cat challenge/out/answer$i challenge/in/flood$i | bin/flood_print bx=9 by=9" to display the solution for board 123
