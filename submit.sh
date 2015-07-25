#!/bin/bash
cd /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/SummerRutgers15/
#cmsenv
eval `scramv1 runtime -sh`
#Added by Ian
export X509_USER_PROXY=~/x509_user_proxy/proxy
voms-proxy-init --noregen
#</Ian>
cd /afs/cern.ch/work/i/ihuntisa/WORK/RUTGERS/CMSSW_5_3_20/src/SummerRutgers15/
echo "root -b -q thrust.C(FIRST, LAST)"
echo "First = $FIRST and last file = $LAST"
root -b  <<EOF
.x thrust.C($FIRST,$LAST);
.q
EOF
echo "Done all jobs!"
