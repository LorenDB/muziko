// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Uninstall stuff based on https://nheko.im/nheko-reborn/nheko/-/blob/master/deploy/installer/controlscript.qs.
// Everything else is probably my own random imaginings, with a bit of StackOverflow thrown in.

function silentUninstall()
{
    return installer.value("PleaseDeleteMeWithNaryAWordOfProtest") === "AbsolutelyGoRightAhead";
}

function Controller()
{
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    if (systemInfo.productType !== "windows")
        installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);

    if (silentUninstall() && installer.isUninstaller())
    {
        installer.setDefaultPageVisible(QInstaller.Introduction, false);
        installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
        installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
        installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
        installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);

        this.FinishedPageCallback = function()
        {
            gui.clickButton(buttons.FinishButton);
        }
    }
}

Controller.prototype.PerformInstallationPageCallback = function()
{
    installer.killProcess("@TargetDir@/bin/" + (systemInfo.productType === "windows" ? "muziko.exe" : "muziko"));
}
