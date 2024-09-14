import os

os.system("pyside6-uic UI/mainWindow.ui -o UI_Main/mainWindow.py")
os.system("pyside6-uic UI/SettingsWindow.ui -o UI_Main/SettingsWindow.py")
os.system("pyside6-uic UI/MesgWindow.ui -o UI_Main/mesgPopUp.py")
os.system("pyside6-uic UI/ComapreTeach.ui -o UI_Main/CompareTeach.py")
os.system("pyside6-uic UI/CreateOrModifyFile.ui -o UI_Main/createOrModifyRecipe.py")
os.system("pyside6-rcc UI/StaticAssets.qrc -o UI_Main/StaticAssets_rc.py")