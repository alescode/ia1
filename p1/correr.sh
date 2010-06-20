ulimit -S -v $((512*1024))
ulimit -S -t $((60*60))
for i in ejemplos/*
do
    ./dodgson -bfs -prop "$i" > "proba/$i-resultado.out"
done
