
// ImageProcessingDoc.cpp : Implement a CImageProcessingDoc Class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "DlgBrightnessOption.h"
#include "DlgMosaicOption.h"
#include "DlgCompositeOption.h"
#include "DlgIntensityTransformOption.h"
#include "DlgContrastStretchOption.h"

#include "ImageProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingDoc

IMPLEMENT_DYNCREATE(CImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessingDoc, CDocument)
	ON_COMMAND(ID_PROCESS_MOSAIC, &CImageProcessingDoc::OnProcessMosaic)
	ON_COMMAND(ID_PROCESS_BRIGHTNESS, &CImageProcessingDoc::OnProcessBrightness)
	ON_COMMAND(ID_PROCESS_COMPOSITE, &CImageProcessingDoc::OnProcessComposite)
	ON_COMMAND(ID_PROCESS_INTENSITY_TRANSFORM, &CImageProcessingDoc::OnProcessIntensityTransform)
	ON_COMMAND(ID_PROCESS_CONTRAST_STRETCH, &CImageProcessingDoc::OnProcessContrastStretch)
	ON_COMMAND(ID_PROCESS_EQUALIZATION, &CImageProcessingDoc::OnProcessEqualization)
	ON_COMMAND(ID_32783, &CImageProcessingDoc::OnHistogramSpecification)
END_MESSAGE_MAP()


// CImageProcessingDoc Contruction/Destuction

CImageProcessingDoc::CImageProcessingDoc()
{
	//// TODO: Add an one-time generating code here
	m_pImage = NULL;
}

CImageProcessingDoc::~CImageProcessingDoc()
{
	if (NULL != m_pImage)
		delete m_pImage;
}

BOOL CImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: load imagefile // DONE
	m_pImage = new CxImage;
	m_pImage->Load(lpszPathName, FindType(lpszPathName));
	m_strSourcePath = lpszPathName;	// 원본 경로 기억

	CalculateHistogram();

	return TRUE;
}

// 파일 저장: 기본 Serialize()는 저장 코드가 비어 있어 0바이트 파일이 만들어지므로 CxImage로 직접 저장
BOOL CImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (m_pImage == NULL)
		return FALSE;

	// 원본 입력 이미지를 덮어쓰지 않도록, 처음 열었던 파일 경로로는 저장하지 않음
	// - GetPathName()은 Save As 이후 결과 파일 경로로 바뀌므로, 열 때 기억해 둔 m_strSourcePath와 비교
	//   (결과 파일은 Ctrl+S로 다시 저장 가능)
	// - FALSE를 반환하면 MFC가 Save As 대상 파일을 지워버리므로, 저장하지 않고 TRUE 반환
	if (!m_strSourcePath.IsEmpty() && m_strSourcePath.CompareNoCase(lpszPathName) == 0) {
		AfxMessageBox(_T("Not saved: the original image is protected. Use File > Save As with a new file name."));
		return TRUE;
	}

	int type = FindType(FindExtension(lpszPathName));
	if (type == CXIMAGE_FORMAT_UNKNOWN) {
		AfxMessageBox(_T("Unsupported file extension. Please use .bmp, .png or .jpg"));
		return FALSE;
	}

	if (!m_pImage->Save(lpszPathName, type)) {
		AfxMessageBox(_T("Failed to save the image."));
		return FALSE;
	}

	SetModifiedFlag(FALSE);
	return TRUE;
}

BOOL CImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//// TODO: Add a re-initialization code here
	//// SDI documents will reuse this article

	return TRUE;
}




// CImageProcessingDoc serialization

void CImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//// TODO: Add a saving code here
	}
	else
	{
		//// TODO: Add a loading code here
	}
}


// CImageProcessingDoc diagnosis

#ifdef _DEBUG
void CImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProcessingDoc command

CString CImageProcessingDoc::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString(_T(""));
}

CString CImageProcessingDoc::RemoveExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(0,i);
		}
	}
	return name;
}

int CImageProcessingDoc::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}

void CImageProcessingDoc::CalculateHistogram()
{
	// TODO: Add a calculating histogram code here
	if (m_pImage) {
		// Histogram function, which is implemented in Cximage
		 //m_histogramMax = m_pImage->Histogram(m_histogramRed, m_histogramGreen, m_histogramBlue, m_histogramGray);

		 
		// 가짜코드
		for (int i = 0; i < 256; i++){
			m_histogramRed[i]   = rand() % 200;
			m_histogramGreen[i] = rand() % 200;
			m_histogramBlue[i]  = rand() % 200;
			m_histogramGray[i]  = rand() % 200;
		}

		m_histogramMax = 0;
		for (int i = 0; i < 256; i++){
			m_histogramMax = max(m_histogramMax, m_histogramRed[i]  );
			m_histogramMax = max(m_histogramMax, m_histogramGreen[i]);
			m_histogramMax = max(m_histogramMax, m_histogramBlue[i] );
			m_histogramMax = max(m_histogramMax, m_histogramGray[i] );
		}
		//////////////////////////////////////////////////////////////
	}
}

void CImageProcessingDoc::OnProcessBrightness()
{
	// TODO: Add a changing the brightness histogram code here
	if (m_pImage) {
		DlgBrightnessOption dlg;

		if (dlg.DoModal() == IDOK) {
			// write your own code
			// for applying your effect, you must use m_pImage
			// this code is a simple example for manufacturing image : grayscaling

			int nPlusMinus = dlg.m_nPlusMinus;
			BYTE byModifyValue = dlg.m_byModifyValue;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;

			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);

					newcolor.rgbBlue  = (BYTE)RGB2GRAY(color.rgbRed, color.rgbGreen, color.rgbBlue);
					newcolor.rgbGreen = (BYTE)RGB2GRAY(color.rgbRed, color.rgbGreen, color.rgbBlue);
					newcolor.rgbRed   = (BYTE)RGB2GRAY(color.rgbRed, color.rgbGreen, color.rgbBlue);

					m_pImage->SetPixelColor(x, y, newcolor);
				}
			}

			// code to view overflow, Keep observation with the debugger
			BYTE a = 255;
			BYTE b = 1;
			int  c = 10;
			BYTE r;

			r = a + b;
			r = a + c;
			r = b + c;
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessMosaic()
{
	if (m_pImage) {
		DlgMosaicOption dlg;

		if (dlg.DoModal() == IDOK) {
			DWORD dwWindowSize = dlg.m_dwWindowSize;
			if (dwWindowSize == 0) return;               // 0으로 나누기 방지

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();

			// 윈도우 크기만큼 건너뛰며 이동 (Blur처럼 1픽셀씩 X)
			for (DWORD by = 0; by < height; by += dwWindowSize) {
				for (DWORD bx = 0; bx < width; bx += dwWindowSize) {

					// 가장자리: 블록이 이미지 밖으로 나가지 않게 자르기
					DWORD ey = min(by + dwWindowSize, height);
					DWORD ex = min(bx + dwWindowSize, width);

					// 1) 블록 안의 R, G, B를 각각 합산 (정수로 누적)
					DWORD sumR = 0, sumG = 0, sumB = 0, cnt = 0;
					for (DWORD y = by; y < ey; y++) {
						for (DWORD x = bx; x < ex; x++) {
							RGBQUAD c = m_pImage->GetPixelColor(x, y);
							sumR += c.rgbRed;
							sumG += c.rgbGreen;
							sumB += c.rgbBlue;
							cnt++;
						}
					}

					// 2) 평균 (반올림해서 소수점 손실 최소화)
					RGBQUAD avg;
					avg.rgbRed = (BYTE)((sumR + cnt / 2) / cnt);
					avg.rgbGreen = (BYTE)((sumG + cnt / 2) / cnt);
					avg.rgbBlue = (BYTE)((sumB + cnt / 2) / cnt);
					avg.rgbReserved = 0;

					// 3) 블록 전체를 평균색으로 채우기
					for (DWORD y = by; y < ey; y++)
						for (DWORD x = bx; x < ex; x++)
							m_pImage->SetPixelColor(x, y, avg);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

// 0~255 범위를 벗어난 값을 잘라냄 (클램핑)
static BYTE ClampToByte(int value)
{
	if (value < 0)   return 0;
	if (value > 255) return 255;
	return (BYTE)value;
}

void CImageProcessingDoc::OnProcessComposite()
{
	// TODO: Add a composite code here // DONE
	if (m_pImage == NULL)
		return;

	DlgCompositeOption dlg;
	if (dlg.DoModal() != IDOK)
		return;

	int nOperatorID = dlg.GetCompositeOperatorID();		// 0: +, 1: -, 2: x, 3: /
	CxImage* pSecondImage = dlg.GetSecondImage();

	if (pSecondImage == NULL) {
		AfxMessageBox(_T("Please select the second image."));
		return;
	}
	if (nOperatorID != 0 && nOperatorID != 1) {
		AfxMessageBox(_T("Only + and - are supported."));
		return;
	}

	DWORD width   = m_pImage->GetWidth();			// 결과 크기 = 첫 번째 이미지 크기
	DWORD height  = m_pImage->GetHeight();
	DWORD width2  = pSecondImage->GetWidth();
	DWORD height2 = pSecondImage->GetHeight();

	// CxImage는 아래쪽 줄이 y = 0이므로, 두 이미지를 왼쪽 위 기준으로 맞추려면 높이 차이만큼 y를 보정
	long yOffset = (long)height - (long)height2;
	RGBQUAD black = { 0, 0, 0, 0 };

	for (DWORD y = 0; y < height; y++) {
		long y2 = (long)y - yOffset;
		for (DWORD x = 0; x < width; x++) {
			RGBQUAD first = m_pImage->GetPixelColor(x, y);

			// 두 번째 이미지 범위 밖이면 0(검정)으로 간주 → 첫 번째 이미지 값이 그대로 남음
			RGBQUAD second = black;
			if (x < width2 && y2 >= 0 && y2 < (long)height2)
				second = pSecondImage->GetPixelColor(x, y2);

			int r = first.rgbRed;
			int g = first.rgbGreen;
			int b = first.rgbBlue;

			switch (nOperatorID) {
			case 0:		// 더하기
				r += second.rgbRed;
				g += second.rgbGreen;
				b += second.rgbBlue;
				break;
			case 1:		// 빼기
				r -= second.rgbRed;
				g -= second.rgbGreen;
				b -= second.rgbBlue;
				break;
			}

			RGBQUAD result = first;			// 알파 등 나머지 값은 원본 유지
			result.rgbRed   = ClampToByte(r);
			result.rgbGreen = ClampToByte(g);
			result.rgbBlue  = ClampToByte(b);

			m_pImage->SetPixelColor(x, y, result);
		}
	}

	SetModifiedFlag(TRUE);		// 이미지가 바뀌었다고 표시
	CalculateHistogram();
	UpdateAllViews(NULL);		// 화면 다시 그리기
}

void CImageProcessingDoc::OnProcessIntensityTransform()
{
	// TODO: Add a Contrast conversion code here
	if (m_pImage) {
		DlgIntensityTransformOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nTransformType = dlg.m_nTransformType;
			double nLinearTransfromCoefficient_a = dlg.m_nLinearTransfromCoefficient_a;
			double nLinearTransfromCoefficient_b = dlg.m_nLinearTransfromCoefficient_b;

			int byThresholdValue = dlg.m_byThresholdValue;

			BYTE byContrastStretching_Start = dlg.m_byContrastStretching_Start;
			BYTE byContrastStretching_End = dlg.m_byContrastStretching_End;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessContrastStretch()
{
	// TODO: Add a Contrast stretching code here
	if (m_pImage) {
		DlgContrastStretchOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nStretchType = dlg.m_nStretchType;
			float fLow = dlg.m_fLow;
			float fHigh = dlg.m_fHigh;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;

			BYTE LUT[256];

			// initialize thresholds
			int nLowTherhs = 0;
			int nHighThresh = 255;


			// compute thresholds
			if (nStretchType == 0) { // Auto

			} else if (nStretchType == 1) { // Ends-In

			}


			// compute LUT


			// trnasfer image
			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);

					// using LUT

					m_pImage->SetPixelColor(x, y, newcolor);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessEqualization()
{
	// TODO: Add a Histogram equalization code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (Coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (Coding)  


		}

		// (4) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (Coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}



void CImageProcessingDoc::OnHistogramSpecification()
{
	// TODO: Add a Histogram specifications code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (coding)  


		}


		// Same as histogram equalization code from here.
		// However, After a cumulative histogram convert to float sum_hist[256] and
		// modifing to be entered in the real number on calculating sum_hist


		//(4) 
		DWORD desired_histogram[256];
		// Making desired_histogram
/*
		//1.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=i;
		}
*/
/*
		//2.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=255-i;
		}
*/
/*
		//3.		
		for (int i=0; i<=127; i++)
		{
			desired_histogram[i]=127-i;
		}
		for (int i=128; i<=255; i++)
		{
			desired_histogram[i]=i-127;
		}
*/

		// redefinition
		sum = 0;
		scale_factor = 255.0 / (width*height);

		//(5) Create the cumulative histogram of histogram you want
		for(int i=0; i<256; i++)
		{
			//(coding)



		}

		//(6) Is the inverse.
		float difference;	// calculate the difference
		int min;	// Calculate the minimum of the difference
		DWORD inv_hist[256];	// reverse-histogram

		//(Hint) will use dual-route.
		// float fabs() Use library functions.
		for(int i=0; i<256; i++)
		{
		//(coding)




		}

		//(7) Create look-up table of reverse-histogram
		for(int i=0; i<256 ; i++)
		{
		//(coding)

		}
		
		

		// (8) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	} // if(m_pImage)

	CalculateHistogram();
	UpdateAllViews(NULL);

	

}
