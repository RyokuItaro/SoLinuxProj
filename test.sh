#!/bin/bash
mkdir ~/daemonTestSourceDir ~/daemonTestDestDir
mkdir ~/daemonTestSourceDir/recursiveTest
touch ~/daemonTestSourceDir/fileA
touch ~/daemonTestSourceDir/recursiveTest/fileB
demon -r -s 10 ~/daemonTestSourceDir ~/daemonTestDestDir
diff ~/daemonTestSourceDir ~/daemonTestDestDir