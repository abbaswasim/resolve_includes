#Resolve includes

Simple single file C++11 header only library to resolve includes in text files like shaders.

##Usage 

```c++
bool resolve_with_markers{false};
std::string data{loaded_however};
resolve_includes(data, resolve_with_markers);

print(data);
```
