for i in `ls | grep -vi "tunel" | grep -vi "icon"`
do
    cp -R $i tunel/
done
