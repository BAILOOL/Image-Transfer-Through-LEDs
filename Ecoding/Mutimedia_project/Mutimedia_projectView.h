
// Mutimedia_projectView.h : interface of the CMutimedia_projectView class
//

#pragma once


class CMutimedia_projectView : public CView
{
protected: // create from serialization only
	CMutimedia_projectView();
	DECLARE_DYNCREATE(CMutimedia_projectView)

// Attributes
public:
	CMutimedia_projectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	static const int file_size=256;
	static const int output_size = 64;
	virtual ~CMutimedia_projectView();
	int viewMode;
	unsigned char m_grayImg[file_size][file_size];//store the read gray image 
	unsigned char m_orglmg[file_size][file_size*3];//store orignial image
	unsigned char m_outlmg[file_size][file_size*3];//store the output image 
	unsigned char luminance[file_size][file_size];//stores the luminance information
	unsigned char interpolate[output_size][output_size];//interpolated matrix (receiving side)
	unsigned char substemp[output_size][output_size];//stores the subsampled version
	unsigned char final[output_size][output_size];//stores the final image to be sent
	unsigned char finaldecode[output_size][output_size];// output matrix decoded 
	void RGB2Y(float r, float g, float b,float *y);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAnyfile();
	afx_msg void OnProcessing();
	afx_msg void OnOpenrawfileOpengrayscale();
	afx_msg void OnSmoothing();
	afx_msg void OnSavebmpSave();
//	afx_msg void OnSmooth1();
	afx_msg void OnProcessinggray();
};

#ifndef _DEBUG  // debug version in Mutimedia_projectView.cpp
inline CMutimedia_projectDoc* CMutimedia_projectView::GetDocument() const
   { return reinterpret_cast<CMutimedia_projectDoc*>(m_pDocument); }
#endif

