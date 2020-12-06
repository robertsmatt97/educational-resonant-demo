# Educational Demos for Resonant Converter Phenomena

This repository stores all of the project files corresponding to the paper published under the same name. The files are necessary for building, simulating, and operating the converter described in the paper.

## Navigating the Project Files

1. User Manual
    - A detailed guide for setting up and operating the educational board.
    - The User Manual is listed already in the published paper, but will be regularly updated here.
2. Bill of Materials
    - An Excel spreadsheet that contains every component used for the educational converter.
    - URLs, part numbers, and prices are listed.
    - Mouser cart link attached (To be added) is at the bottom of the spreadsheet, for easy purchasing.
    - It is recommended to buy extra parts.
3. GERBER Files Zip File
    - A zip file for PCB manufacturers for convinient PCB purchasing.
    - The zip file is suitable for Smart Prototyping, which was used to create the prototype.
    - Some PCB manufacturers require different export options. The User Manual describes how to properly export it.
4. "Videos" Folder
    - Videos demonstrating the capabilities of the converter. 
    - YouTube links are listed below as well. (To be added)
    - Demonstration 1: (To be added)
    - Demonstration 2: (To be added)
5. "KiCAD" Folder
    - KiCAD project files, containing the schematic and PCB of the converter.
    - Library paths may need to be configured to properly see symbols and footprints.
6. "MATLAB" Folder
    - MATLAB scripts to show hypothetical, simulation, and converter data over different combinations of resonant converter design parameters.
    - 'Resonant_Design.m' shows hypothetical and simulation data only. The board does not need to be connected.
    - 'Resonant_Design_Serial.m' shows hypothetical, simulation, and converter data. The board needs to be connected and the CCS script needs to be running.
    - MATLAB 2019b or higher is required to run the 'Resonant_Design_Serial.m' script. 
7. "LTSpice" Folder
    - Simluation files for the resonant converter. 
    - Configure the parameters where it is listed.
    - Follow the User Manual for instructions on additional operation.
8. "CCS" Folder
    - Code Composer Studio (CCS) project files containing the code needed for operation. 
    - The latest version of CCS is recommended for operation to work.

### To Do List

1. Add Mouser Cart link of all the components. 
2. Add YouTube links of video demonstrations.
3. Add more visuals to User Manual.
3. Add additional microcontroller features to shorten down time for demonstrations.

#### Contact Information for Help

You can contact me at my email below for assistance in getting the board to work. Please be detailed in your issues.
Email: robertsmatt97@gmail.com
