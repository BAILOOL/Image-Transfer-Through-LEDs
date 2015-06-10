
// Mutimedia_projectView.cpp : implementation of the CMutimedia_projectView class
//

#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Mutimedia_project.h"
#endif

#include "Mutimedia_projectDoc.h"
#include "Mutimedia_projectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

/*/////////////////////////////////////////////////////////////////////////////////////////////*/
/*
#include <Windows.h>
#include <algorithm>
 
// Save the bitmap to a bmp file  
void SaveBitmapToFile( BYTE* pBitmapBits,  
                       LONG lWidth,  
                       LONG lHeight,  
                       WORD wBitsPerPixel, 
                       const unsigned long& padding_size,
                       LPCTSTR lpszFileName )  
{  
    // Some basic bitmap parameters  
    unsigned long headers_size = sizeof( BITMAPFILEHEADER ) +  
                                 sizeof( BITMAPINFOHEADER );  
 
    unsigned long pixel_data_size = lHeight * ( ( lWidth * ( wBitsPerPixel / 8 ) ) + padding_size );  
       
    BITMAPINFOHEADER bmpInfoHeader = {0};  
       
    // Set the size  
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  
       
    // Bit count  
    bmpInfoHeader.biBitCount = wBitsPerPixel;  
       
    // Use all colors  
    bmpInfoHeader.biClrImportant = 0;  
       
    // Use as many colors according to bits per pixel  
    bmpInfoHeader.biClrUsed = 0;  
       
    // Store as un Compressed  
    bmpInfoHeader.biCompression = BI_RGB;  
       
    // Set the height in pixels  
    bmpInfoHeader.biHeight = lHeight;  
       
    // Width of the Image in pixels  
    bmpInfoHeader.biWidth = lWidth;  
       
    // Default number of planes  
    bmpInfoHeader.biPlanes = 1;  
       
    // Calculate the image size in bytes  
    bmpInfoHeader.biSizeImage = pixel_data_size;  
       
    BITMAPFILEHEADER bfh = {0};  
       
    // This value should be values of BM letters i.e 0x4D42  
    // 0x4D = M 0×42 = B storing in reverse order to match with endian  
    bfh.bfType = 0x4D42;  
    //bfh.bfType = 'B'+('M' << 8); 
     
    // <<8 used to shift ‘M’ to end  
       
    // Offset to the RGBQUAD  
    bfh.bfOffBits = headers_size;  
       
    // Total size of image including size of headers  
    bfh.bfSize =  headers_size + pixel_data_size;  
       
    // Create the file in disk to write  
    HANDLE hFile = CreateFile( lpszFileName,  
                                GENERIC_WRITE,  
                                0,  
                                NULL,  
                                CREATE_ALWAYS,  
                                FILE_ATTRIBUTE_NORMAL,  
                                NULL );  
       
    // Return if error opening file  
    if( !hFile ) return;  
       
    DWORD dwWritten = 0;  
       
    // Write the File header  
    WriteFile( hFile,  
                &bfh,  
                sizeof(bfh),  
                &dwWritten ,  
                NULL );  
       
    // Write the bitmap info header  
    WriteFile( hFile,  
                &bmpInfoHeader,  
                sizeof(bmpInfoHeader),  
                &dwWritten,  
                NULL );  
       
    // Write the RGB Data  
    WriteFile( hFile,  
                pBitmapBits,  
                bmpInfoHeader.biSizeImage,  
                &dwWritten,  
                NULL );  
       
    // Close the file handle  
    CloseHandle( hFile );  
}  
 
BYTE* LoadBMP ( int* width, int* height, unsigned long* size, LPCTSTR bmpfile )
{
    BITMAPFILEHEADER bmpheader;
    BITMAPINFOHEADER bmpinfo;
    // value to be used in ReadFile funcs
    DWORD bytesread;
    // open file to read from
    HANDLE file = CreateFile ( bmpfile , 
                               GENERIC_READ, 
                               FILE_SHARE_READ,
                               NULL, 
                               OPEN_EXISTING, 
                               FILE_FLAG_SEQUENTIAL_SCAN, 
                               NULL );
    if ( NULL == file )
        return NULL;
     
    if ( ReadFile ( file, &bmpheader, sizeof ( BITMAPFILEHEADER ), &bytesread, NULL ) == false )
    {
        CloseHandle ( file );
        return NULL;
    }
 
    // Read bitmap info
    if ( ReadFile ( file, &bmpinfo, sizeof ( BITMAPINFOHEADER ), &bytesread, NULL ) == false )
    {
        CloseHandle ( file );
        return NULL;
    }
     
    // check if file is actually a bmp
    if ( bmpheader.bfType != 'MB' )
    {
        CloseHandle ( file );
        return NULL;
    }
 
    // get image measurements
    *width   = bmpinfo.biWidth;
    *height  = abs ( bmpinfo.biHeight );
 
    // Check if bmp iuncompressed
    if ( bmpinfo.biCompression != BI_RGB )
    {
        CloseHandle ( file );
        return NULL;
    }
 
    // Check if we have 24 bit bmp
    if ( bmpinfo.biBitCount != 24 )
    {
        CloseHandle ( file );
        return NULL;
    }
     
    // create buffer to hold the data
    *size = bmpheader.bfSize - bmpheader.bfOffBits;
    BYTE* Buffer = new BYTE[ *size ];
    // move file pointer to start of bitmap data
    SetFilePointer ( file, bmpheader.bfOffBits, NULL, FILE_BEGIN );
    // read bmp data
    if ( ReadFile ( file, Buffer, *size, &bytesread, NULL ) == false )
    {
        delete [] Buffer;
        CloseHandle ( file );
        return NULL;
    }
 
    // everything successful here: close file and return buffer
     
    CloseHandle ( file );
 
    return Buffer;
}
 
std::unique_ptr<BYTE[]> CreateNewBuffer( unsigned long& padding,
                                         BYTE* pmatrix, 
                                         const int& width,
                                         const int& height )
{
    padding = ( 4 - ( ( width * 3 ) % 4 ) ) % 4;  
    int scanlinebytes = width * 3;
    int total_scanlinebytes = scanlinebytes + padding;
    long newsize = height * total_scanlinebytes;
    std::unique_ptr<BYTE[]> newbuf( new BYTE[ newsize ] );
 
    // Fill new array with original buffer, pad remaining with zeros
    std::fill( &newbuf[ 0 ], &newbuf[ newsize ], 0 );
    long bufpos = 0;   
    long newpos = 0;
    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < 3 * width; x+=3 )
        {
            // Determine positions in original and padded buffers
            bufpos = y * 3 * width + ( 3 * width - x );     
            newpos = ( height - y - 1 ) * total_scanlinebytes + x; 
             
            // Swap R&B, G remains, swap B&R
            newbuf[ newpos ] = pmatrix[ bufpos + 2 ]; 
            newbuf[ newpos + 1 ] = pmatrix[ bufpos + 1 ];  
            newbuf[ newpos + 2 ] = pmatrix[ bufpos ];       
        }
    }
 
    return newbuf;
}

*/
//////////////////////////////////////////////////////////////////////////////////////////////*/



// CMutimedia_projectView

IMPLEMENT_DYNCREATE(CMutimedia_projectView, CView)

BEGIN_MESSAGE_MAP(CMutimedia_projectView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMutimedia_projectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ANYFILE, &CMutimedia_projectView::OnAnyfile)
	ON_COMMAND(ID_Processing, &CMutimedia_projectView::OnProcessing)
	ON_COMMAND(ID_OPENRAWFILE_OPENGRAYSCALE, &CMutimedia_projectView::OnOpenrawfileOpengrayscale)
	ON_COMMAND(ID_smoothing, &CMutimedia_projectView::OnSmoothing)
	ON_COMMAND(ID_SAVEBMP_SAVE, &CMutimedia_projectView::OnSavebmpSave)
//	ON_COMMAND(ID_smooth1, &CMutimedia_projectView::OnSmooth1)
ON_COMMAND(ID_processinggray, &CMutimedia_projectView::OnProcessinggray)
END_MESSAGE_MAP()

// CMutimedia_projectView construction/destruction

CMutimedia_projectView::CMutimedia_projectView()
{
	// TODO: add construction code here

}

CMutimedia_projectView::~CMutimedia_projectView()
{
}

BOOL CMutimedia_projectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMutimedia_projectView drawing

void CMutimedia_projectView::OnDraw(CDC* pDC)
{
	CMutimedia_projectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	switch (viewMode){
	case 0:
		for(int y=0;y<file_size;y++){
			for(int x=0; x<file_size;x++){
				pDC->SetPixel(x,y,RGB(m_grayImg[y][x],m_grayImg[y][x],m_grayImg[y][x]));
			}
		}
		break;
	case 1://display the processed image color to be sent
		for(int y=0; y<file_size;y++){
			for(int x=0; x<file_size;x++){
				pDC->SetPixel(x,y,RGB(m_outlmg[y][x*3+0],m_outlmg[y][x*3+1],m_outlmg[y][x*3+2]));
				pDC->SetPixel(x+258,y,RGB(luminance[y][x],luminance[y][x],luminance[y][x]));
			}
		}
		for(int y=0; y<output_size;y++){
			for(int x=0; x<output_size;x++){
				pDC->SetPixel(x +516,y,RGB(substemp[y][x],substemp[y][x],substemp[y][x]));
				pDC->SetPixel(x+582,y,RGB(final[y][x],final[y][x],final[y][x]));
			}
		}
		break;
	case 2://mode for display the receiver side
		for(int y=0;y<file_size;y++){
			for(int x=0; x<file_size;x++){
				pDC->SetPixel(x,y,RGB(m_grayImg[y][x],m_grayImg[y][x],m_grayImg[y][x]));
			}
		}
		for(int y=0;y<output_size;y++){
			for(int x=0; x<output_size;x++){
				pDC->SetPixel(x + file_size + 3 ,y,RGB(finaldecode[y][x],finaldecode[y][x],finaldecode[y][x]));
			}
		}
		break;
    case 3://display the processed gray image to be sent
		for(int y=0;y<file_size;y++){
			for(int x=0; x<file_size;x++){
				pDC->SetPixel(x,y,RGB(m_grayImg[y][x],m_grayImg[y][x],m_grayImg[y][x]));
			}
		}
		for(int y=0; y<output_size;y++){
			for(int x=0; x<output_size;x++){
				pDC->SetPixel(x +258,y,RGB(substemp[y][x],substemp[y][x],substemp[y][x]));
				pDC->SetPixel(x+322,y,RGB(final[y][x],final[y][x],final[y][x]));
			}
		}
		break;


	}
	
}


// CMutimedia_projectView printing


void CMutimedia_projectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMutimedia_projectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMutimedia_projectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMutimedia_projectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMutimedia_projectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMutimedia_projectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMutimedia_projectView diagnostics

#ifdef _DEBUG
void CMutimedia_projectView::AssertValid() const
{
	CView::AssertValid();
}

void CMutimedia_projectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMutimedia_projectDoc* CMutimedia_projectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMutimedia_projectDoc)));
	return (CMutimedia_projectDoc*)m_pDocument;
}
#endif //_DEBUG


// CMutimedia_projectView message handlers


void CMutimedia_projectView::OnAnyfile()
{
	//this function open a color image with size "file_sizeXfile_size"
	CFileDialog dlg(TRUE, _T("raw"), _T("*.raw"),OFN_OVERWRITEPROMPT,_T("Raw (*.raw)|(*.raw)}"));
	if(dlg.DoModal() == IDOK) {
		CString pathname;
		pathname = dlg.GetPathName();
		FILE *fp = _tfopen(pathname, _T("rb"));
		fread(m_orglmg,sizeof(unsigned char), file_size*file_size*3, fp);
		fclose (fp);
		memcpy(m_outlmg,m_orglmg,file_size*file_size*3*sizeof(unsigned char));
		viewMode = 1;
		Invalidate();
	}

}
void CMutimedia_projectView:: RGB2Y(float r, float g, float b,float *y_y)
{
	//this function is amde to get the Y (luminance componants) only for processing
*y_y = 0.299f * r + 0.587f * g + 0.114f * b;
}


void CMutimedia_projectView::OnProcessing()
{
	//this function is for processing the color image into either dithered or 
	//monochrome image depending on the readability of the image  
	//declaration for Y extraction
	float r,g,b,y_y;
	//declaration for subsampling
	int src_x,src_y;
	//declarations for ordered dithering 
	int matDither[4][4]={0,128,32,160,192,64,224,96,48,176,16,144,240,112,208,80};//matrix took from lab
	//extracting the Y value (luminance)
	for (int y=0; y<file_size;y++){
		for (int x=0; x<file_size;x++){
			r= (float)m_orglmg[y][x*3+0]/255.0f;
			g= (float)m_orglmg[y][x*3+1]/255.0f;
			b= (float)m_orglmg[y][x*3+2]/255.0f;
			RGB2Y(r,g,b,&y_y);
			luminance[y][x] = (unsigned char)(y_y*255.0);
		}
	}
	// the scale varies from case to case there fore a general one (file_size/output_size)
	double scale = file_size*1.0/output_size; 
	for (int y=0; y<64;y++){
		for (int x=0; x<64;x++){
			src_y=(int)(y*scale);
			src_x=(int)(x*scale);
			if (src_x> file_size-1) src_x =file_size-1;
			if (src_y> file_size-1) src_y =file_size-1;
			//take pixel values
			substemp[y][x]=luminance[src_y][src_x];
		}
	}
	//ordered dithering 
	for (int y=0; y<output_size;y++){
		for (int x=0; x<output_size;x++){
			int i=x%4;
			int j=y%4;
			if (substemp[y][x]>matDither[i][j])
				final[y][x]= 255;
			else 
				final[y][x]=0;
		}
	}
	//monochrome image (if the dithered image is bad)
	/*for (int y=0; y<64;y++){
		for (int x=0; x<64;x++){
			if(substemp[y][x]>80)
				final[y][x]=255;
			else
				final[y][x]=0;
		}
	}*/
	//save the final image as raw to be sent 
	CFileDialog dlg(TRUE, _T("raw"), _T("*.raw"),OFN_OVERWRITEPROMPT,_T("Raw (*.raw)|(*.raw)}"));
	if(dlg.DoModal() == IDOK) {
		CString pathname;
		pathname = dlg.GetPathName();
		FILE *pFile = _tfopen(pathname, _T("wb"));
		int finalSize = sizeof(final);
        fwrite (final , sizeof(char), sizeof(final), pFile);
        fclose (pFile);
	}
	
	Invalidate();
}


void CMutimedia_projectView::OnOpenrawfileOpengrayscale()
{
	// as we seperated the color case from the gray image, we will also seperate the processing
	CFileDialog dlg(TRUE, _T("raw"), _T("*.raw"),OFN_OVERWRITEPROMPT,_T("Raw (*.raw)|(*.raw)}"));
	if(dlg.DoModal() == IDOK) {
		CString pathname;
		pathname = dlg.GetPathName();
		FILE *fp = fopen(pathname, "rb");
		fread(m_grayImg,sizeof( char), file_size*file_size, fp);
		fclose (fp);
		viewMode=0;
		Invalidate();
	}
	
}	
void CMutimedia_projectView::OnSmoothing()
{
	// take an image (file_sizeXfile_size) the one received and process it to reconstruct the data back (without color)
	//to be able to use it, file_size and output_size have to be changed manually to the desired values
	//smoothing part
	int maskvalues[3][3];
	int src_x,src_y;
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			maskvalues[i][j]=1;
	int sumvalue;
	unsigned char tempbuf[file_size][file_size];
	for (int y=1; y < file_size-1;y++){
		for (int x=1; x < file_size-1; x++){
			sumvalue=0;
			//apply mask for each pixel
			for(int ir = -1; ir<=1; ir++){
				for(int ic = -1; ic<=1; ic++){
					sumvalue+= m_grayImg[y+ir][x+ic] * maskvalues[ir+1][ic+1];
				}
			}
			tempbuf[y][x]= sumvalue/9;
		}
	}
	for (int y=1; y < file_size-1; y++)
		for (int x=1; x < file_size-1; x++)
			m_grayImg[y][x]=tempbuf[y][x];//gets copied just for the display 
	//interpolation after : to get the size back
	double scale = output_size*1.0/file_size;
	//scale  interpolation 
	//normal interpolation (nearest neighbor)
	for (int y=0; y<output_size;y++){
		for (int x=0; x<output_size;x++){
			src_y=(int)(y/scale);
			src_x=(int)(x/scale);
			if (src_x> file_size-1) src_x =file_size-1;
			if (src_y> file_size-1) src_y =file_size-1;
			//take pixel values
			interpolate[y][x]=tempbuf[src_y][src_x];
		}
	}
	// bilinear interpolation (better approach)
	/*for (int y=1;y<output_size -1;y++){
		for (int x=1;x<output_size-1;x++){
			//getting the rounded value to reference in the current m_orglmg matrix
			src_x= (int)(x/scale +0.5);
			src_y= (int)(y/scale +0.5);
			//check the for the edge contrains,but here in this case with scale 2 this excess never happens 
			if (src_x > file_size-1) src_x =file_size-1;
			if (src_y > file_size-1) src_y =file_size-1;
			//the bilinear algorithm
			interpolate[y][x]= (unsigned char) 0.25* (tempbuf[src_y][src_x] + tempbuf[src_y +1][src_x] + tempbuf[src_y][src_x+1] + tempbuf[src_y+1][src_x+1]);
			//the new one 
	     	}
	}*/
	//finally the shapening mask to get the edges back           
	//initialize the 3X3 filtering matrix 
	int maskValues[3][3]={-1,-1,-1,-1,9,-1,-1,-1,-1};
	//initializethe temporary variables that will be used
	int sumValue;
	int temp;
	for (int y=1;y<output_size-1;y++){
		for (int x=1;x<output_size-1;x++){
			sumValue = 0;
			temp = 0;
			for (int ir =-1; ir<=1; ir++ ){
				for (int ic=-1;ic<=1;ic++){
					sumValue += interpolate[y+ir][x+ic] * maskValues[ir+1][ic+1];
				}
			}
			//limit the range to [0,255] only
			temp = sumValue;
			if (sumValue < 0)
				temp = 0;
			else if (sumValue > 255)
				temp = 255;
			substemp[y][x]=temp; //substemp (from color image) used as a temp matrix to store the final result as it has the same size with finaldecode matrix
		}
	}
	for (int y=1;y<output_size;y++)
		for(int x=1;x<output_size;x++)
			finaldecode[y][x] = substemp[y][x];  	
			
	viewMode = 2;
	Invalidate();

}




void CMutimedia_projectView::OnSavebmpSave()
{
	//save image in another format using openCV
	// TODO: Add your command handler code here
	/*
	BYTE* buf = new BYTE[ file_size * 3 * file_size ];
    int c = 0;
  
    for ( int i = 0; i < file_size; i++ )
    {
        for ( int j = 0; j < file_size; j++ )
        {
            buf[ c + 0 ] = (BYTE) 255;
            buf[ c + 1 ] = (BYTE) 0;
            buf[ c + 2 ] = (BYTE) 0;
  
            c += 3;
        }
    }
  
    SaveBitmapToFile( (BYTE*) buf,
                    128,
                    128,
                    24,
                    0,
                    "bluesquare.bmp" );
  */
}


//void CMutimedia_projectView::OnSmooth1()
//{
//
//	// TODO: Add your command handler code here
//	//smoothing
//	int maskvalues[3][3];
//	int src_x,src_y;
//	for (int i=0;i<3;i++)
//		for (int j=0;j<3;j++)
//			maskvalues[i][j]=1;
//	int sumvalue;
//	unsigned char tempbuf[output_size][output_size];
//	for (int y=1; y < output_size-1;y++){
//		for (int x=1; x < output_size-1; x++){
//			sumvalue=0;
//			//apply mask for each pixel
//			for(int ir = -1; ir<=1; ir++){
//				for(int ic = -1; ic<=1; ic++){
//					sumvalue+= m_grayImg[y+ir][x+ic] * maskvalues[ir+1][ic+1];
//				}
//			}
//			tempbuf[y][x]= sumvalue/9;
//		}
//	}
//	for (int y=1; y < output_size-1; y++)
//		for (int x=1; x < output_size-1; x++)
//			m_grayImg[y][x]=tempbuf[y][x];//gets copied just for the display 
//
//	double scale = file_size*1.0/output_size;
//	//scale  interpolation 
//	for (int y=0; y<file_size;y++){
//		for (int x=0; x<file_size;x++){
//			src_y=(int)(y/scale);
//			src_x=(int)(x/scale);
//			if (src_x> file_size-1) src_x =file_size-1;
//			if (src_y> file_size-1) src_y =file_size-1;
//			//take pixel values
//			interpolate[y][x]=tempbuf[src_y][src_x];
//		}
//	}
//	// bilinear interpolation
//	for (int y=0;y<file_size;y++){
//		for (int x=0;x<file_size;x++){
//			//getting the rounded value to reference in the current m_orglmg matrix
//			src_x= (int)(x/scale +0.5);
//			src_y= (int)(y/scale +0.5);
//			//check the for the edge contrains,but here in this case with scale 2 this excess never happens 
//			if (src_x > file_size-1) src_x =file_size-1;
//			if (src_y > file_size-1) src_y =file_size-1;
//			//the bilinear algorithm
//			interpolate[y][x]= 0.25 * (tempbuf[src_y][src_x] + tempbuf[src_y+1][src_x] + tempbuf[src_y][src_x+1] + tempbuf[src_y+1][src_x+1]);
//			//the new one 
//	     	}
//	}
//	         
//	//initialize the 3X3 filtering matrix 
//	int maskValues[3][3]={-1,-1,-1,-1,9,-1,-1,-1,-1};
//	//initializethe temporary variables that will be used
//	int sumValue;
//	int temp;
//	for (int y=1;y<file_size-1;y++){
//		for (int x=1;x<file_size-1;x++){
//			sumValue = 0;
//			temp = 0;
//			for (int ir =-1; ir<=1; ir++ ){
//				for (int ic=-1;ic<=1;ic++){
//					sumValue += interpolate[y+ir][x+ic] * maskValues[ir+1][ic+1];
//				}
//			}
//			//limit the range to [0,255] only
//			temp = sumValue;
//			if (sumValue < 0)
//				temp = 0;
//			else if (sumValue > 255)
//				temp = 255;
//			luminance[y][x]=temp;
//		}
//	}
//	for (int y=1;y<file_size;y++)
//		for(int x=1;x<file_size;x++)
//			interpolate[y][x] = luminance[y][x];  	
//		
//	viewMode = 3;
//	Invalidate();
//
//}


void CMutimedia_projectView::OnProcessinggray()
{
	// Same as colro processing except that we don't have to extract the Luminance channel as it is already there
	//declaration for Y extrac
	//unsigned char luminance[256][256];
	//declaration for subsampling
	//unsigned char substemp[64][64];
	int src_x,src_y;
	//declarations for ordered dithering 
	int matDither[4][4]={0,128,32,160,192,64,224,96,48,176,16,144,240,112,208,80};
	//unsigned char final[64][64];
	//extracting the Y value (luminance)
	for (int y=0; y<file_size;y++){
		for (int x=0; x<file_size;x++){
			luminance[y][x] = m_grayImg[y][x];
		}
	}

	double scale = file_size*1.0/output_size;
	//scale = 4 subsampling 
	for (int y=0; y<64;y++){
		for (int x=0; x<64;x++){
			src_y=(int)(y*scale);
			src_x=(int)(x*scale);
			if (src_x> file_size-1) src_x =file_size-1;
			if (src_y> file_size-1) src_y =file_size-1;
			//take pixel values
			substemp[y][x]=luminance[src_y][src_x];
		}
	}
	//ordered dithering 
	for (int y=0; y<output_size;y++){
		for (int x=0; x<output_size;x++){
			int i=x%4;
			int j=y%4;
			if (substemp[y][x]>matDither[i][j])
				final[y][x]= 255;
			else 
				final[y][x]=0;
		}
	}
	//incase monochrome image 
	/*for (int y=0; y<64;y++){
		for (int x=0; x<64;x++){
			if(substemp[y][x]>80) // 80 as best threshold based on tests
				final[y][x]=255;
			else
				final[y][x]=0;
		}
	}*/
	//save the final image as raw
	CFileDialog dlg(TRUE, _T("raw"), _T("*.raw"),OFN_OVERWRITEPROMPT,_T("Raw (*.raw)|(*.raw)}"));
	if(dlg.DoModal() == IDOK) {
		CString pathname;
		pathname = dlg.GetPathName();
		FILE *pFile = _tfopen(pathname, _T("wb"));
		int finalSize = sizeof(final);
        fwrite (final , sizeof(char), sizeof(final), pFile);
        fclose (pFile);
	}
	viewMode = 3;
	Invalidate();
}


