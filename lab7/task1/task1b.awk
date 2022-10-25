#! /usr/bin/awk -f

BEGIN { FS = ","
      } 
      {   
        print $4 "\t" $3 "\t" $2
      } 