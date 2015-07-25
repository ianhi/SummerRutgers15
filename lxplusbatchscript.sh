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
bsub -R "pool>5000" -M 3000000 -q 8nh -J merge_job_${i} < /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/SummerRutgers15/submit.sh
let "i++"
done
echo "submit all jobs!"
