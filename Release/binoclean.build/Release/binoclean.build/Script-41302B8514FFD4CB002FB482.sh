#!/bin/sh
#!/bin/bash
# Auto Increment Version Script
buildPlist="binoclean/binoclean-Info.plist"
test=`date`
buildVersion=$(/usr/libexec/PlistBuddy -c "Print CFBuildVersion" $buildPlist)
buildNumber=$(/usr/libexec/PlistBuddy -c "Print CFBuildNumber" $buildPlist)
buildNumber=$(($buildNumber + 1))
/usr/libexec/PlistBuddy -c "Set :CFBuildNumber $buildNumber" $buildPlist
/usr/libexec/PlistBuddy -c "Set :CFBundleVersion $buildVersion.$buildNumber.$test" $buildPlist
/usr/libexec/PlistBuddy -c "Set :CFBundleShortVersionString $buildVersion.$buildNumber" $buildPlist
