# FROM https://markuspeitl.github.io/my-linux-scripts/install-appimage.html

START_DIR=$(pwd)
APP_IMG_PATH=~/RufusAdminQt6.AppImage
APP_IMG_NAME=$(basename $APP_IMG_PATH)
NEW_NAME=Rufus

cd ~
echo "Executing: rm -r ~/Deploy"
rm -r ~/Deploy
printf "\n"

#Making the tree
echo "Executing: mkdir -p ~/Deploy/usr/share/icons/hicolor/256x256/apps"
mkdir -p ~/Deploy/usr/share/icons/hicolor/256x256/apps
printf "\n"
echo "Executing: mkdir -p ~/Deploy/usr/share/applications"
mkdir -p ~/Deploy/usr/share/applications 
printf "\n"
echo "Executing: mkdir -p ~/Deploy/usr/bin"
mkdir -p ~/Deploy/usr/bin 
printf "\n"
echo "Executing: cp ~/RufusAdmin/build_tools/Linux/rufusadmin.desktop ~/Deploy/usr/share/applications"
cp ~/RufusAdminQt6/build_tools/Linux/rufusadmin.desktop ~/Deploy/usr/share/applications
printf "\n"
echo "Executing: cp ~/RufusAdmin/build_tools/Linux/rufusadmin.png ~/Deploy/usr/share/icons/hicolor/256x256/apps"
cp ~/RufusAdminQt6/build_tools/Linux/rufusadmin.png ~/Deploy/usr/share/icons/hicolor/256x256/apps
printf "\n"

echo "Executing: cp ~/RufusAdminApp ~/Deploy/usr/bin"
cp ~/RufusAdminApp ~/Deploy/usr/bin
printf "\n"

echo "Executing ~/linuxdeployqt ~/Deploy/usr/share/applications/rufusadmmin.desktop -appimage"
~/linuxdeployqt ~/Deploy/usr/share/applications/rufusadmin.desktop -appimage
printf "\n"

echo "Executing: mv RufusAdmin-x86_64.AppImage RufusAdmin.AppImage"
mv RufusAdmin-x86_64.AppImage $APP_IMG_NAME
printf "\n"

#Making the appimage executable
echo "Executing: chmod +x $APP_IMG_PATH"
chmod +x $APP_IMG_PATH
printf "\n"

cd ~
echo "Executing: rm -r ~/Deploy"
rm -r ~/Deploy
