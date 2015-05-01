:run.bat
:
:runs RemoteCodeManagement solution

start .\Debug\Executive.exe localhost 5555 localhost 1111
start .\Debug\Executive.exe localhost 6666 localhost 1111
start .\Debug\CppCli-WPF-App.exe
