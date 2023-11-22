# File paths used
INPUT="in"
CORRECT="out"
ATTEMPT="attempt"
FLUID="$1"

# Checks that the output/exit code of the program is the expected
# $1: Test name
# $2: Output stream to check (1 for stdout, 2 for stderr)
# $3: Expected output message
# $4: Expected exit code
# $5: Amount of time steps
# $6: Input file path
# $7: Output file path
# $8: Extra argument
check_output () {
   printf "\033[1mChecking $1\033[0m\t"
   # Runs the program, capturing the exit code and the specified output stream
   if [ "$2" == "2" ]; then
      X=$($FLUID $5 $6 $7 $8 2>&1 >/dev/null);
      CODE=$?;
   else
      X=$($FLUID $5 $6 $7 $8);
      CODE=$?;
   fi
   # Checks that both the output message and exit code match
   if [ "$X" == "$3" ] && [ "$CODE" == "$4" ]; then
      echo -e "\033[32;1mCORRECT\033[0m"
   else
      echo -e "\033[31;1mFAILURE\033[0m"
      echo "Output:   $X"
      echo "Expected: $3"
      echo "Exit code: $CODE"
      echo "Expected:  $4"
   fi
}

# Write test files in binary
# Numbers are stored in little-endian, so the first byte of each number is the least significant byte of the number
echo -ne "\0\0\xc0\x43\xff\xff\xff\xff" > np_negative # Negative amount of particles in header
echo -ne "\0\0\xc0\x43\0\0\0\0" > np_zero # 0 partícles
echo -ne "\0\0\xc0\x43\x01\0\0\0" > np_lower # Less particles in the file than specified in the header
echo -ne "\0\0\xc0\x43\x01\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" > np_greater # More particles in the file than especified in the header
touch read.fld # Read only file
chmod -w read.fld

# Error codes. Must be manually converted to the [0, 255] range
ERROR_1=$((-1 + 256))
ERROR_2=$((-2 + 256))
ERROR_3=$((-3 + 256))
ERROR_4=$((-4 + 256))
ERROR_5=$((-5 + 256))

# Expected simulation information output
read -r -d '' SMALL_OUT << EOF
Number of particles: 4800
Particles per meter: 204
Smoothing length: 0.00830882
Particle mass: 0.00011779
Grid size: 15 x 21 x 15
Number of blocks: 4725
Block size: 0.00866667 x 0.00857143 x 0.00866667
EOF

# Tests for argument validation errors
check_output "Less arguments                            " 2 "Error: Invalid number of arguments: 2." $ERROR_1 "5" "in.fld"
check_output "More arguments                            " 2 "Error: Invalid number of arguments: 4." $ERROR_1 "5" "in.fld" "out.fld" "test"
check_output "Non-Numeric time steps                    " 2 "Error: time steps must be numeric." $ERROR_1 "test" "in.fld" "out.fld"
check_output "Invalid time steps                        " 2 "Error: Invalid number of time steps." $ERROR_2 "-1" "in.fld" "out.fld"

# Tests for file loading errors
check_output "Can't open input file                     " 2 "Error: Cannot open in.fld for reading" $ERROR_3 "1" "in.fld" "out.fld"
check_output "Can't open output file                    " 2 "Error: Cannot open read.fld for writing" $ERROR_4 "1" "$INPUT/small.fld" "read.fld"
check_output "Amount of particles negative              " 2 "Error: Invalid number of particles: -1." $ERROR_5 1 "np_negative" "out.fld"
check_output "Amount of particles 0                     " 2 "Error: Invalid number of particles: 0." $ERROR_5 1 "np_zero" "out.fld"
check_output "Amount of particles lower than in header  " 2 "Error: Number of particles mismatch. Header: 1, Found: 0." $ERROR_5 1 "np_lower" "out.fld"
check_output "Amount of particles greater than in header" 2 "Error: Number of particles mismatch. Header: 1, Found: 2." $ERROR_5 1 "np_greater" "out.fld"

# Test for checking correct output for valid input
check_output "Print simulation parameters               " 1 "$SMALL_OUT" 0 0 "$INPUT/small.fld" "out.fld"

rm -f np_negative np_zero np_lower np_greater read.fld # Clear generated files

echo ""
mkdir -p "$ATTEMPT" # Creates folder for output files

# Checks that the output is correct for each pair of model and time steps amount
for size in "small" "large"; do
   for count in "1" "2" "3" "4" "5"; do
      printf "\033[1mChecking $size $count\033[0m\t"
      input="$INPUT/$size.fld"
      output="$ATTEMPT/$size-$count.fld"
      # Runs the program
      $FLUID $count "$input" "$output" >/dev/null
      # Checks that the result file matches the expected result
      X="$(diff "$CORRECT/$size-$count.fld" $output)"
      if [ "$X" == "" ]; then
         echo -e "\033[32;1mCORRECT\033[0m"
      else
         echo -e "\033[31;1mFAILURE\033[0m"
         echo $X
      fi
   done
done

rm -rf "$ATTEMPT" # Clears generated files
