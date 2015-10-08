#!/bin/sh
# 1st parameter : day number
# 2nd parameter : type of collision

echo day scanned is : $1
currentFile="DAY_$2_$1"
echo current file is $currentFile
# get the runumbers for this day 
#get_file_list.pl -keys 'runnumber' -cond 'trgsetupname=CosmicLocalClock,daynumber='"$1"',sst=1,filename~st_cosmic_adc,filetype=online_daq,storage=hpss' -limit 0 -delim / -distinct > all
get_file_list.pl -keys 'runnumber' -cond 'trgsetupname=CosmicLocalClock,daynumber='"$1"',sst=1,filename~st_cosmic_adc,filetype=online_daq,storage=hpss' -limit 0 -delim / -distinct > all

#sort the file
sort all > allOrder

#get the number of entries
max=$(cat allOrder | wc -l)
echo max is $max

#loop over runnumbers and count the # of events
#MYFILE=${currentFile}
#MYFILE="./TEST_$1"
#echo "RUN NUMBER   #EVENTS" >> MYFILE
sum=0
echo "RUN NUMBER   #EVENTS" >> $currentFile
for file in `cat allOrder`
do
echo runnumber is "$file"
get_file_list.pl -keys 'sum(events)' -cond 'trgsetupname=CosmicLocalClock,runnumber='"$file"',sst=1,filename~st_cosmic_adc,filetype=online_daq,storage=hpss' -limit 0 -delim / -distinct
res="$(get_file_list.pl -keys 'sum(events)' -cond 'trgsetupname=CosmicLocalClock,runnumber='"$file"',sst=1,filename~st_cosmic_adc,filetype=online_daq,storage=hpss' -limit 0 -delim / -distinct)"
echo "$file     $res" >> $currentFile
if [ $res > 0 ];then
sum=`expr $sum + $res`
fi
done
echo total for day $1 : $sum
echo "sum : $sum" >> $currentFile
