该项目需要的依赖，务必导入

# 安装你的依赖

## Ubuntu(推荐)

```shell
apt-get install lib[xxx]-dev
```

## 对于Windwos建议使用*vcpkg*

CMAKE参数使用
```cmake
-DVCPKG_TARGET_TRIPLET=x64-mingw-static
```
或者
```cmake
-DVCPKG_TARGET_TRIPLET=x64-windows-static
```
然后按照如下格式执行命令
```shell
vcpkg install [xxx]:x64-mingw-static
```

# 需要安装的依赖库

 - uv
 - cpp-httplib
 - uvw
 - spdlog
