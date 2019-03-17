# Embedded
My ECE3232 embedded project!

## Setup
### PuTTY
1. Open up `PuTTY`.
1. Select `Serial`.
1. Set the appropriate `Serial line` and set the `Speed (baud)` to `9600` (this should already be set).
1. Go to `Connection` > `Serial`. Set `Data bits` to `7` and set `Flow control` to `None`.

### IDE
1. Open up `Kinetis Design Studio 3 IDE`.
1. Go to `File` > `Import...` > `General` > `Existing Projects into Workspace` and then press `Next >`. Press `Browse...` then select `C:\Freescale\KSDK_1.3.0\lib\ksdk_platform_lib\kds\K64F12` and press `Finish`.
1. Right-click on `ksdk_platform_lib_K64F12` and click `Build Project`.
1. Right-click on `embedded` and go to `Debug As` > `Debug Configurations...` > `GDB SEGGER J-Link Debugging` > `embedded_Debug_Segger`. Go to the `Startup` tab and uncheck `Enable SWO` if enabled.
1. Click `Debug`.
1. Check `Do not show this message again for today` and press `Accept` if the `Terms of use` modal pops up.
1. If the `Confirm Perspective Switch` model pops up, check `Remember my decicion` and click `Yes`.

### Help
> How do you find the device?
1. Go to `Control Panel` > `Device Manager` > `Ports (COM & LPT)`. Look for `JLink CDC UART Port (COMX)` where `COMX` will be port you will use for `PuTTY`. Examples include `COM1`, `COM2`, `COM3`, etc.