# MAVSDK 설치/빌드 및 px4 데이터 습득환경 빌드

## Environment 
<br>

### OS : ubuntu 18.04
### 빌드 툴 : VScode with cmake
### MAVSDK 버전 : v0.35.1 stable
### device : pixhawk fmu_v2
### 

<Br><br>

---

## 1. MAVSDK의 설치
<br>


참조 주소 
<li>https://mavsdk.mavlink.io/develop/en/contributing/build.html

간단하게 정리하면 아래와 같다.

1. dependence 설치 
<br>

```
    sudo apt-get update -y
    sudo apt-get install cmake build-essential colordiff git doxygen -y
```

2. git으로 MAVSDK 라이브러리 다운로드 및 업데이트

```
    git clone https://github.com/mavlink/MAVSDK.git
	cd MAVSDK && git checkout master
    git submodule update  --init --recursive
```

3. c++ 라이브러리 빌드

```
    cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -Bbuild/default -H.
	cmake --build build/default
```

4. build 파일 인스톨 - system install : linux에 c++ 라이브러리 추가

```
sudo cmake --build build/default --target install 

# 아래줄은 최초실행시에만 사용하며, 공유라이브러리의 캐시를 재설정해주는 코드이다.
sudo ldconfig 
```
<br>

---

## 2. MAVSDK 빌드 환경 구성 

### 1. 참조 주소

현재 코드작성에 사용된 메뉴얼사이트는 아래와 같다.

    라이브러리 빌드 참조  : https://mavsdk.mavlink.io/develop/en/cpp/ <br>
    c++ property 구성 참조 : https://github.com/microsoft/vscode-cpptools/issues/6348

### 2. vscode 빌드 환경

먼저 vscode에서 빌드를 g++ 로 선택한다.

이후 빌드를 진행하면 .vscode 가 생기며 아래의 파일이 생성된다.

    launch.json
    settings.json
    tasks.json

여기서 tasks.json 파일을 열어 args 내부에 아래의 코드로 수정한다.


물론 이 작업을 하기전에 mavsdk가 설치되어 있어야하며, usr/local/include/mavsdk 파일이 존재하는지 확인해야 한다.


```
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++ build active file",
            "command": "/usr/lib/ccache/g++",
            "args": [
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-I/usr/local/include/mavsdk",
                "-lcurses",
               "-lmavsdk",
               "-lmavsdk_action",
               "-lmavsdk_telemetry",
               "-lmavsdk_calibration",

            ],
            "options": {
                "cwd":  "/usr/lib/ccache"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Task generated by Debugger."
        }
    ],
    "version": "2.0.0"
}
```

이다음에는 settings.json 파일을 수정한다

```
{
    "C_Cpp.errorSquiggles": "Disabled",
    "files.associations": {
        "*.html": "html",
        "*.tcc": "cpp",
        "array": "cpp",
        "atomic": "cpp",
        "cctype": "cpp",
        "chrono": "cpp",
        "clocale": "cpp",
        "cmath": "cpp",
        "condition_variable": "cpp",
        "csignal": "cpp",
        "cstdarg": "cpp",
        "cstddef": "cpp",
        "cstdint": "cpp",
        "cstdio": "cpp",
        "cstdlib": "cpp",
        "cstring": "cpp",
        "ctime": "cpp",
        "cwchar": "cpp",
        "cwctype": "cpp",
        "deque": "cpp",
        "forward_list": "cpp",
        "list": "cpp",
        "unordered_map": "cpp",
        "unordered_set": "cpp",
        "vector": "cpp",
        "exception": "cpp",
        "algorithm": "cpp",
        "functional": "cpp",
        "iterator": "cpp",
        "map": "cpp",
        "memory": "cpp",
        "memory_resource": "cpp",
        "numeric": "cpp",
        "optional": "cpp",
        "random": "cpp",
        "ratio": "cpp",
        "set": "cpp",
        "string": "cpp",
        "string_view": "cpp",
        "system_error": "cpp",
        "tuple": "cpp",
        "type_traits": "cpp",
        "utility": "cpp",
        "fstream": "cpp",
        "future": "cpp",
        "initializer_list": "cpp",
        "iomanip": "cpp",
        "iosfwd": "cpp",
        "iostream": "cpp",
        "istream": "cpp",
        "limits": "cpp",
        "mutex": "cpp",
        "new": "cpp",
        "ostream": "cpp",
        "sstream": "cpp",
        "stdexcept": "cpp",
        "streambuf": "cpp",
        "thread": "cpp",
        "cinttypes": "cpp",
        "typeinfo": "cpp",
        "variant": "cpp"
    }
}
```



현재 Access_Sensor 코드에서는 하나의 요소를 입력받으며 0입력시에 pixhawk의 magnetometer 센서의 캘리브레이션(calibration)이 진행되며 이 외에는 스킵하는 형식으로 빌드되었다.

이를 위해서 launch.json 의 args에 1을 넣어 초기값으로 캘리브레이션이 진행되지 않도록 한다.

```
    "args": ["1"], # 필요시에는 0을 입력
```


마지막으로 c_cpp_properties.json을 추가하여 자동완성 기능을 사용가능하도록 한다.

여기서는 mavsdk 의 버전기준인 gcc-arm-none-eabi-9-2020-q2 버전을 이용하였으며, 이 또한 설치되어있는지 /opt 에서 확인이 가능하다.

2020-q2 버전을 기준으로 c-standard는 gnu17, c++-strandard 는 gnu++14가 사용되었다.

```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/local/include/**"

            ],
            "defines": [],
            "compilerPath": "/opt/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-gcc",
            "cStandard": "gnu17",
            "cppStandard": "gnu++14",
            "intelliSenseMode": "linux-gcc-arm"
        }
    ],
    "version": 4
}
```


<br>

---
<br>

## 3. MAVSDK 코드 작성 

<br>

### 1. 참조

```
    MAVSDK telemetry 클래스 참조 : https://mavsdk.mavlink.io/develop/en/guide/telemetry.html<br>
    예제 실행 참조 : https://mavsdk.mavlink.io/develop/en/examples/<br>
```

자세한 내용은 참조사이트에 c++ 클래스에 대한 내용이 나와있다.

현 코드에서 라이브러리 참조와 칼리브레이션 및 시스템 응답확인을 위한 함수는 mav_sdk.h와 mav_sdk.cpp 항목에 담겨있다.

<br>



### 2. 센서 오차수정을 위한 calibration 옵션

MAVSDK 에서 제공되는 Calibration 클래스를 참조하여 사용했으며, 이는 mav_sdk.h 에서 확인이 가능하다.

총 2개의 센서 calibration 코드가 사용되었으며, 실행파일의 argv에 따라 실행코드가 다르게 되도록 구성되었다.

```
실행방법 : ./Access_Sensor 숫자

0 : magnetometor calibration

1 : nothing

2 : accelerometor calibration
```

이렇게 3개의 옵션을 추고 실행하도록 하였다.















