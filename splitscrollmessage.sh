
OIFS=$IFS

IFS=';'

msg=$1

for x in $msg
do
    bash scrollmessage.sh "$x" 1 $2 $3
done

IFS=$OIFS

