INPUT="build/input"
CORRECT="build/correct"
ATTEMPT="build/attempt"
FLUID="build/fluid/fluid"

for size in "small" "large"; do
   for count in "1" "2" "3" "4" "5"; do
      printf "\033[1mChecking $size $count\033[0m\t"
      input="$INPUT/$size.fld"
      output="$ATTEMPT/$size-$count.fld"
      $FLUID $count "$input" "$output"
      X="$(diff "$CORRECT/$size-$count.fld" $output)"
      if [ "$X" == "" ]; then
         echo -e "\033[32;1mCORRECT\033[0m"
      else
         echo -e "\033[31;1mFAILURE\033[0m"
      fi
      echo $X
   done
done
