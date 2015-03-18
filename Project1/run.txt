:run.bat file for OOD Project 1- File Catalogue

@echo off
echo.
echo.
echo Demonstrating requirement 3
echo.
echo Input command - FileCatalogue.exe Test *.cpp *.txt *.oo /s
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.cpp *.txt *.oo /s
echo.
echo Input command - FileCatalogue.exe Test *.cpp
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.cpp

echo.
echo.
echo Demonstrating requirement 5
echo.
echo Input command - FileCatalogue.exe Test *.* /s /d
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.* /s /d
echo.
echo Input command - FileCatalogue.exe Test *.* /d
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.* /d

echo.
echo.
echo Demonstrating requirement 6
echo.
echo Input command - FileCatalogue.exe Test *.* /s /d /f "ankit kumar"
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.* /s /d /f "ankit kumar"

echo.
echo.
echo Demonstrating requirement 7
echo.
echo Input command - FileCatalogue.exe
echo.
echo %CD%
.\FileCatalogue\Debug\FileCatalogue.exe 

echo.
echo.
echo Demonstrating requirement 8
echo.
echo Input command - FileCatalogue.exe Test *.* /s
echo.
.\FileCatalogue\Debug\FileCatalogue.exe Test *.* /s