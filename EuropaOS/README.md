How to Flash EuropaOS onto STM32 NUCELO-L4R5ZI:
   - Clone MSD-EuropaExplore MSD Code Repository
   - Open STM32CubeIDE
   - Import project, select EuropaOS.ioc
   - Make sure the project is being built for the STM32 NUCELO-L4R5ZI
   - Build Project, (Ctrl + B)
   - Plug in STM32 NUCELO-L4R5ZI into USB COM Port
   - Flash Project with green arrow in top middle STM32CubeIDE

Connection to Board:
STM32 will communicate through uart, open this uart communiction with putty or python applition.
   - Open device manager, under COM, see what COM port the STM32 is connected to.
   - Open PuTTYy or Python appliction.
   - Set the STM32's COM port from the device manager.
   - If using PuTTy, set buad rate to 209700.
   - Open, and receive communication.  
