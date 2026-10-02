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
- 작업은 `feat/...` 브랜치 → PR, `main` 직접 커밋 금지
- 커밋 전 VS 캐시가 안 올라가는지 `git status` 확인

## 진행 방식

필수 요구사항을 **각자 브랜치에서 따로 구현 → 비교 세션 → 더 나은 쪽 기반으로 합치기**.
둘 다 처음부터 끝까지 한 번씩 만들어 보고, 비교하면서 서로의 코드를 이해하는 게 목적.

### 1. 공통 요구사항 (둘 다 이 목록 그대로 AI 프롬프트에 넣고 시작)
- [ ] 연산자 선택 버그 수정 — `DlgCompositeOption`의 연산자 콤보에 `SetItemData`가 없어 `GetItemData(GetCurSel())`가 항상 0 → `GetCurSel()` 사용
- [ ] `OnProcessComposite`에 `+` `−` 픽셀 연산 구현 (현재 `RGB2GRAY` 가짜 코드), 결과는 0~255로 클램핑
- [ ] 두 이미지 크기가 다를 때 처리
- [ ] CxImage `GetPixelColor`/`SetPixelColor`/`GetWidth`/`GetHeight` 등만 사용 — `RGB2GRAY` 등 내장 처리 함수·OpenCV 금지
- [ ] head + body → 결과 − points 순서로 실행해 결과 이미지를 `TeamProject1/DOC/`에 저장 (`Save As`만 사용)
- [ ] 메뉴·리소스 파일(`.rc`, `resource.h`)은 건드리지 않기 — 합칠 때 충돌 방지 (`.rc`는 UTF-16)

### 2. 브랜치
| 사람 | 브랜치 |
|---|---|
| 성준 | `feat/sj-required` |
| 윤교 | `feat/yg-required` |

`main`에 직접 커밋하지 않기.

### 3. 비교 세션 체크리스트 (10/9)
| 항목 | 확인 방법 |
|---|---|
| 빌드 | 각 브랜치 checkout → Debug \| Win32 빌드 |
| 결과 일치 | 결과 이미지 나란히 비교, 채널별 픽셀 평균값 비교 |
| 금지 함수 | `RGB2GRAY`, OpenCV, CxImage 내장 처리 함수 검색 |
| 클램핑 | 255 초과·0 미만 픽셀이 잘리는지 |
| **상대 코드 설명** | 내 코드가 아니라 **상대 코드를** 내가 설명 |
| 선택 | 기반 브랜치 결정, 다른 쪽에서 가져올 부분 정리 |

세션은 캡처해서 보고서 ④(회의 기록)에 사용.

### 4. 합치기
1. 기반으로 고른 브랜치만 `main`에 merge
2. 다른 브랜치의 좋은 부분은 작은 커밋으로 옮기기
3. 두 브랜치를 그대로 둘 다 merge하지 않기 (같은 함수 수정 → 충돌)

## 로드맵
| 기간 | 내용 |
|---|---|
| **10/3~** | 킥오프(캡처): 둘 다 빌드 확인, Q&A 게시판 질문 (png/jpg 입력 형식, extra 요구사항, 결과 배경색 변해도 되는지) |
| **~10/8** | 킥오프 직후부터 각자 브랜치에서 필수 요구사항 구현 → **10/8까지 push** |
| **10/9** | 비교 세션 → **`main` 합치기 완료** → 최소 제출본 확보 |
| **10/10~16** | 개선 병렬 — 원클릭 Squid 메뉴(같은 폴더 3장 자동 로드), `×` `÷`, 예외 처리 / 마스크 기반 합성 옵션 (담당은 비교 세션에서 결정) |
| **10/17~18** | PR 교차 리뷰, 변형 입력 테스트, 다른 PC 클린 빌드 |
| **10/19~23** | 중간고사 — 작업 없음 |
| **10/24~26** | extra (Q&A 답변 받은 뒤 분담) |
| **10/27** | 보고서 — ②설계 성준 / ①④⑥ 윤교 / ⑤소감 각자 / ③소요시간 같이 |
| **10/28** | `.sln` 이름 변경 → clone 빌드 확인 → zip 2개 제출 |
| 10/29 | 마감 22:00 (예비일) |

> AI 사용 규정이 강의에 있는지 확인하고, 보고서 ⑥에 활용 방식 기록.

## 제출 전 체크리스트
- [ ] `.sln` 이름을 `teamNumber_teamName.sln`으로 변경
- [ ] 다른 PC에서 clone → 무수정 빌드·실행 확인
- [ ] 결과 이미지 보고서에 포함
- [ ] zip 이름 `TeamNumber_TeamName.zip` / extra는 `_extra.zip`
