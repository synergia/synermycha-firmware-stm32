from pathlib import Path
import os
import pathlib
home = str(Path.home())
if os.path.exists(home+"/.platformio/packages/framework-stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_conf_template.c"):
    os.remove(home+"/.platformio/packages/framework-stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_conf_template.c")
if os.path.exists(home+"/.platformio/packages/framework-stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_desc_template.c"):
    os.remove(home+"/.platformio/packages/framework-stm32cubef4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_desc_template.c")