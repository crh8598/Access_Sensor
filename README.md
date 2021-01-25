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

## 2. MAVSDK 엑세스 코드 작성

### 1. 참조 주소

현재 코드작성에 사용된 메뉴얼사이트는 아래와 같다.

    라이브러리 빌드 참조  : https://mavsdk.mavlink.io/develop/en/cpp/ <br>
    MAVSDK telemetry 클래스 참조 : https://mavsdk.mavlink.io/develop/en/guide/telemetry.html<br>
    예제 실행 참조 : https://mavsdk.mavlink.io/develop/en/examples/<br>

### 2. vscode cmake 빌드 환경

먼저 vscode에서 빌드를 cmake 로 선택한다.

이후 빌드를 진행하면 .vscode 가 생기며 아래의 파일이 생성된다.

    launch.json
    settings.json
    tasks.json

여기서 tasks.json 파일을 열어 args 내부에 아래의 코드를 추가한다.

```
 "args": [
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-I/usr/local/include/mavsdk",
               "-lmavsdk",
               "-lmavsdk_telemetry",
               "-lmavsdk_calibration",
            ],
```

현재 Access_Sensor 코드에서는 하나의 요소를 입력받으며 0입력시에 pixhawk의 magnetometer 센서의 캘리브레이션(calibration)이 진행되며 이 외에는 스킵하는 형식으로 빌드되었다.

이를 위해서 launch.json 의 args에 1을 넣어 초기값으로 캘리브레이션이 진행되지 않도록 한다.

```
    "args": ["1"], # 필요시에는 0을 입력
```

위와 같은 방법으로 빌드 환경을 구성하였으며, 현재 코드는 pixhawk 의 방위각을 받아 출력하도록 구성되어있다.

만약 gps 센서의 값을 읽고 싶다면 telemetry의 

---
