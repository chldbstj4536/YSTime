# YSTime

**시간 관련 기능들을 제공해주는 라이브러리**

## 현재 제공하는 기능들

- Timer
- Stopwatch
- TimerManager

### 요구 사항

- C++20 지원 컴파일러
- CMake (3.25버전 이상)

## 사용 방법

CMake에서 빌드 후 라이브러리 사용 또는 CMake 프로젝트에 subdirectory로 복제한 프로젝트를 등록 

## CMake에서 빌드 방법

YSTime 저장소를 복제한다.

```
git clone --recurse-submodules https://github.com/chldbstj4536/YSTime.git
```

저장소로 이동한다.

```
cd YSTime
```

cmake 명령어를 통해 build 폴더에 프로젝트를 생성한다.

```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="./out"
```

cmake 명령어를 통해 생성된 프로젝트를 빌드하고 결과물을 out폴더에 설치한다.

```
cmake --build build --target install
```

lib/inc 폴더에 헤더와 라이브러리가 생성된다.

## CMake프로젝트에서 서브디렉토리로 설정 방법

라이브러리 설치 방법을 따라서 설치를 먼저 한다.

test 폴더로 이동한다.
```
cd test
```

test 프로젝트를 생성한다.
```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="./out"
```

위까지 진행하면 자신의 플렛폼에 설치된 IDE에 맞게 build 폴더에 프로젝트 파일이 생성된다.

cmake 명령어를 통해 생성된 테스트 프로젝트를 빌드한다.

```
cmake --build build
```

build/Debug 폴더 안에 테스트 프로그램 실행파일이 생성되어있다.

아래 명령어를 통해 예제 프로그램 결과를 확인할 수 있다.

```
build/Debug/YSTimeTest.exe
```

## 테스트 프로젝트 생성 방법

YSTime 저장소를 복제한다.

```
git clone https://github.com/chldbstj4536/YSTime.git
```

CMake를 통해 프로젝트를 생성한다.

```
cd 복제한_저장소_경로/
cmake -S test -B build
```

build 폴더에 사용하는 빌드도구에 맞게 프로젝트가 생성된다.
