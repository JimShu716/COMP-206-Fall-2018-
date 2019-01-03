ls -lR --time-style=full $1 | grep .jpg | sort -u -k6 -k7>sorted_time.txt

# Use ls command to list all the jpg files in the folder with the modified time. 
 
# Use sort command to sort the jpg files in the order of the modified time, and store the  data in a temporary file.


for i in $(cat sorted_time.txt)

do find $1 -name "$i" >>jpgdir.txt

done

#Find the directories of each jpg file and store them in a temporary file.

name=$(echo $1 | tr '/' '_').jpg

# Use tr command to replace the '/' sign in the directories with the '_' sign to avoid syntax error.


convert $(cat jpgdir.txt) -append $name 

# Use "Convert_append" program to produce the picture.

rm jpgdir.txt | rm sorted_time.txt

# Use rm command to remove the temporary files.
