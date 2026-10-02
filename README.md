# dip-squid-composite

중앙대 디지털영상처리(최광남 교수님) Term Project 1 — CxImage로 오징어게임 이미지 합성

- 스펙: [docs/2026_Term_Project_01_Specification.pdf](docs/2026_Term_Project_01_Specification.pdf)
- 마감: **2026-10-29(목) 22:00** (본과제 / extra 각각 제출)

## 요구사항 요약
1. `squid_head` + `squid_body` 합성
2. 결과 − `squid_points`
3. 겹치는 부분은 기존 도형과 **다른 색**
4. 입력 도형의 모양·위치·크기는 약간 바뀔 수 있음 → 좌표 하드코딩 금지
5. CxImage의 `GetPixelColor`/`SetPixelColor`/`GetWidth`/`GetHeight` 등만 사용, 처리 함수는 직접 구현

## 폴더 구조
```
dip-squid-composite/
├── cximage/            CxImage 소스 + 빌드된 Debug 라이브러리(x86/lib/*_wd.lib)
├── TeamProject1/
│   ├── SRC/            MFC 프로젝트 (ImageProcessing.sln)
│   └── DOC/            결과 이미지, 보고서
├── sample image/       과제 샘플 입력 (jpg)
└── docs/               스펙 PDF
```
> ⚠️ `ImageProcessing.vcxproj`가 `..\..\cximage` 상대경로를 참조하므로 **이 구조를 바꾸지 말 것.**

## 빌드
1. Visual Studio 2019(v142) + "C++용 MFC" 구성 요소
2. `TeamProject1/SRC/ImageProcessing.sln` 열기
3. 구성 **Debug | Win32** 로 빌드 (Release 구성에는 CxImage 경로가 설정돼 있지 않음)

`cximage/x86/lib`에 라이브러리가 들어 있어 CxImage를 따로 빌드할 필요 없음.
라이브러리가 깨졌다면 `cximage/CxImgLib.sln`을 Unicode Debug로 다시 빌드.

## 작업 규칙
- 저장은 `File > Save As`만 (`Ctrl+S`는 원본 덮어씀)
- 브랜치: `main` 보호, 작업은 `feat/...` 브랜치 → PR
- 커밋 전 VS 캐시가 안 올라가는지 `git status` 확인

## 분담
| | 담당 | 내용 |
|---|---|---|
| A | 알고리즘 | 배경색 추정, 마스크 추출, 합성 규칙, 겹침 색, 변형 입력 테스트 |
| B | 통합·제출 | 메뉴/3장 로드/결과 창/저장, 클린 빌드 검증, 보고서·회의록 |

인터페이스:
```cpp
bool MakeSquid(CxImage* head, CxImage* body, CxImage* points, CxImage* out);
```

## 제출 전 체크리스트
- [ ] `.sln` 이름을 `teamNumber_teamName.sln`으로 변경
- [ ] 다른 PC에서 clone → 무수정 빌드·실행 확인
- [ ] 결과 이미지 보고서에 포함
- [ ] zip 이름 `TeamNumber_TeamName.zip` / extra는 `_extra.zip`
