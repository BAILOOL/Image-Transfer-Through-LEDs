
// Mutimedia_projectDoc.h : interface of the CMutimedia_projectDoc class
//


#pragma once


class CMutimedia_projectDoc : public CDocument
{
protected: // create from serialization only
	CMutimedia_projectDoc();
	DECLARE_DYNCREATE(CMutimedia_projectDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMutimedia_projectDoc();
	unsigned char m_orglmg[256][256*3];//store orignial image
	unsigned char m_outlmg[256][256*3];//store the output image 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
