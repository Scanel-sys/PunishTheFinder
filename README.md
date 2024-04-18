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
Injector.exe –pid 123 –func CreateFile
Injector.exe –name explorer.exe –func CreateFile
Injector.exe –name explorer.exe –hide “C:\hello.txt”
```
