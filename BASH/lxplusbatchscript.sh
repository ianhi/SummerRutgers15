#!/bin/bash
cd /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/
#cmsenv
eval `scramv1 runtime -sh`
cd /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/SummerRutgers15/
nJobs=22
i=0
while [ $i -le $nJobs ];
do
let "start=i*2"
let "end=(i+1)*2"
export FIRST=$start
export LAST=$end
echo "First = $FIRST and last file = $LAST"
#if [ $i -eq 3 ] || [ $i -eq 5 ] || [ $i -eq 6 ] || [ $i -eq 7 ] || [ $i -eq 8 ] || [ $i -eq 9 ] || [ $i -eq 10 ] || [ $i -eq 14 ] || [ $i -eq 15 ] || [ $i -eq 19 ] || [ $i -eq 20 ]
if [ true ]
then
bsub -R "pool>5000" -M 3000000 -q 8nh -J THRUST_${i} < /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/SummerRutgers15/BASH/submit.sh
fi
let "i++"
done
echo "submit all jobs!"
