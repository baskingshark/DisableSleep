###
# First Boot script that is used along with a launchd item.  Delets both itself and the launchd item after completion.
###

# Sets computer to never sleep while plugged in to a power source
pmset -c sleep 0

# Prevent your Mac from going to sleep while idle
pmset powerbutton 0

# Do not spin down the disk
pmset disksleep 0 

# hibernatemode = 25 (binary 0001 1001) is only settable via pmset. The system will store a
# copy of memory to persistent storage (the disk), and will remove power to memory. The system
# will restore from disk image. If you want "hibernation" - slower sleeps, slower wakes, and
# better battery life, you should use this setting.
pmset -a hibernatemode 25

# Disables sleep and causes the menu item to be grayed out
/usr/bin/defaults write /Library/Preferences/SystemConfiguration/com.apple.PowerManagement SystemPowerSettings -dict SleepDisabled -bool YES

logger "github.sheeparegreat.DisableSleep initialisation complete"

# Securely removes the launchd item and script.
srm /Library/LaunchDaemons/github.sheeparegreat.DisableSleep.plist
srm "$0"
