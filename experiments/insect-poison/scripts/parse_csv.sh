
input=$1

tempfile="temp.data"
tail -n +5 $input > $tempfile

# Atom(uuid), EnteredAt, LastSeenAt, STI, DurationInAF, IsNLPParseOutput, DirectSTI, GainFromSpreading
while IFS=',' read -r f1 f2 f3 f4 f5 f6 f7 f8
do 
  case "$2" in
    "1")
      echo "$f1"
      ;;
    "2")
      echo "$f2"
      ;;
    "3")
      echo "$f3"
      ;;
    "4")
      echo "$f4"
      ;;
    "5")
      echo "$f5"
      ;;
    "6")
      echo "$f6"
      ;;
    "7")
      echo "$f7"
      ;;
    "8")
      echo "$f8"
      ;;
    *)
      echo "ERR: The number of columns are only 8. Column $2 was not found."
      exit
      ;;
  esac
#  echo "$f7"
done < "$tempfile"

rm $tempfile
