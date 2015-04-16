:run.bat
:
:runs Message Passing Socket solution

start .\Socket\Debug\Executive.exe localhost 9999 localhost 8888 ./SrcFolder/test.txt
start .\Socket\Debug\Executive.exe localhost 8888 localhost 7777 ./SrcFolder/Complexities.png
start .\Socket\Debug\Executive.exe localhost 7777 localhost 9999 ./SrcFolder/pr3s15.pdf
