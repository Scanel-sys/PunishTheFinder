# How to use
```bash
Injector.exe {-pid <target_process_pid> | -name <target_process_name>} {-func <target_function_name> | -hide <target_file_name | -func <target_function_name> -hide <target_file_name>}
```


## Main functionality
____
```–pid <target_process_pid>``` – selecting proces by PID

```–name <target_process_name>``` – selecting proces by name.

```–func <target_function_name>``` – selecting function to monitor

```–hide <target_file_name>``` – selecting file to hide from process


## Examples
_____
```bash
Monitor.exe –pid 123 –func CreateFile
Monitor.exe –name explorer.exe –func CreateFile
Monitor.exe –name explorer.exe –hide “C:\hello.txt”
```

## Also
- Monitor includes Injector class from Injector project
- Monitor looks for Vaccine.dll in Vaccine Debug folder and amongs its own source files
- press "ESC" if you would like to leave from Monitor and detach the Vaccine
