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
git clone https://github.com/chldbstj4536/YSTime.git
git submodule update --init --recursive
```

CMake를 통해 빌드파일들을 생성하고 빌드한다.

```
cd 복제한_저장소_경로/
cmake -S . -B build
cmake --build build --target install
```

lib/inc 폴더에 헤더와 라이브러리가 생성된다.

## CMake프로젝트에서 서브디렉토리로 설정 방법

YSTime 저장소를 복제한다.  

```
git clone https://github.com/chldbstj4536/YSTime.git
git submodule update --init --recursive
```

subdirectory로 복제한 폴더를 추가한다.

```cmake
add_subdirectory(복제한_저장소_경로)
```

cmake 실행
```
cmake -S . -B build
```

build 폴더에 사용하는 빌드도구에 맞게 프로젝트가 생성된다.

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
