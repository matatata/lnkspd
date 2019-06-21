
sudo tee /Library/LaunchDaemons/de.infomac.lnkspd.plist << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
	<dict>
		<key>KeepAlive</key>
		<false/>
		<key>Label</key>
		<string>de.infomac.lnkspd</string>
		<key>ProgramArguments</key>
		<array>
			<string>/usr/local/sbin/lnkspd.sh</string>
			<string>-s</string>
			<string>00:01.2</string>
			<string>-t</string>
			<string>1</string>
		</array>
		<key>LaunchOnlyOnce</key>
		<true/>
	</dict>
</plist>
EOF

sudo launchctl load -w /Library/LaunchDaemons/de.infomac.lnkspd.plist
