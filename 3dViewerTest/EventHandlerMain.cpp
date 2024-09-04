#include "stdafx.h"
#include "EventHandlerMain.h"

#include "UtilCommFuncFile.h"
#include "UtilStringManager.h"

#include "ImageView3D.h"
#include "HeightMap3D.h"

CEventHandlerMain::CEventHandlerMain()
{
}


CEventHandlerMain::~CEventHandlerMain()
{
}


void CEventHandlerMain::OnInspect(_In_ Algorithms::CRavidImage* pImgInfo, _In_ int nInspectionType, _In_opt_ CRavidImageView* pView, _In_opt_ int nChannel)
{
	do
	{
		if(nChannel == 2)
		{
			CRavidImageView* pView = dynamic_cast<CRavidImageView*>(CUIManager::FindView(2));
			if(!pView)
				break;
			pView->GetLayer(0)->Clear();
			FindBase();
					
		}
	}
	while(false);
}

bool CEventHandlerMain::LoadTestFile()
{
	bool bReturn = false;

	do 
	{
		CFileDialog dlg(true, _T("*.bmp"), _T(""), OFN_HIDEREADONLY, nullptr, nullptr, 0, true);

		if(dlg.DoModal() != IDOK)
			break;

		CFileFind ff;

		CString strImgPath = dlg.GetPathName();
		CString strCsvPath;
		strCsvPath = strImgPath;
		strCsvPath.Replace(_T(".bmp"), _T(".csv"));

		int nPos = strImgPath.ReverseFind(_T('\\'));

		if(nPos < 0)
			break;


		CRavidImageView* pView = dynamic_cast<CRavidImageView*>(CUIManager::FindView(2));
		if(!pView)
			break;
		
		m_ri2D.Load(strImgPath);
		pView->SetImageInfo(&m_ri2D);

		pView->Invalidate();

		LoadGocator3DData(strCsvPath);
		bReturn = true;
	} while (false);

	return bReturn;
}

bool CEventHandlerMain::LoadGocator3DData(const CString& strPath)
{
	bool bReturn = false;

	do 
	{

		FILE* fp = _tfopen(strPath, _T("rt"));
		CUtilStringManager usm;
		TCHAR lpszdata[2048] = { 0, };
		CString strData;
		CRavidPoint<int> rpSize;
		bool bCheck = false;

		std::vector<std::vector<float>> vctData;
		if(fp)
		{
			CString strParsing;

			CString strX;
			CString strY;
			bool bFlagX = false;
			bool bFlagY = false;
			int nYCount = 0;

			while(!feof(fp))
			{
				_fgetts(lpszdata, 2048, fp);
				strData = lpszdata;

				int nStep = 0;
				int nStart = 0;
				int nCheck = 0;

				if(strData.Find(_T("Y\\X,")) >= 0)
				{
					strData.Replace(_T("Y\\X,"), _T(""));
					bFlagX = true;
				}

				if(bFlagY)
				{
					if(strData.Find(_T("End")) >= 0)
					{
						bFlagY = false;
						bCheck = true;
					}
					else
					{
						strY.AppendFormat(strData);
						if(strData.Find(_T("\n")) >= 0)
						{
							usm.SetSplit(strY, _T(","));
							int nCount = usm.GetCount();
							std::vector<float> vctY;
							for(int i = 1; i < nCount; ++i)
							{
								strParsing = usm.GetSplit(i);
								if(strParsing.GetLength() == 0)
								{
									vctY.push_back(-10000);
								}
								else
									vctY.push_back(_ttof(strParsing));
							}
							vctData.push_back(vctY);
							strY.Format(_T(""));
						}
					}

				}
				if(bFlagX)
				{
					strX.AppendFormat(strData);
					if(strData.Find(_T("\n")) >=0)
					{
						bFlagX = false;
						bFlagY = true;
						usm.SetSplit(strX, _T(","));
						rpSize.x = usm.GetCount();
					}
				}


				
			}

			fclose(fp);

			if(bCheck)
			{
				int rps = 0;
				rpSize.y = vctData.size();

				if(rpSize.x > 0 && rpSize.y > 0)
				{
					m_ri3D.CreateImageBuffer(rpSize.x, rpSize.y, 0, CRavidImage::EValueFormat_1C_F32);

					BYTE** ppBuffer = m_ri3D.GetYOffsetTable();
					size_t * pSize = m_ri3D.GetXOffsetTable();

					for(int i = 0; i < rpSize.y; ++i)
					{
						for(int x = 0; x < rpSize.x; ++ x)
						{
							if(x < vctData.at(i).size())
								*(float*)(ppBuffer[i] + pSize[x]) = vctData[i][x];
							
						}
					}

					CImageView3D* pView = dynamic_cast<CImageView3D*>(CUIManager::FindView(1));
					if(pView)
					{
						pView->SetImageInfo(&m_ri3D);

						if(pView->GetMpa())
						{
							pView->GetMpa()->m_fInvalidValue = -10000.;
							pView->GetMpa()->m_fLow = -100.;
							pView->GetMpa()->m_fMid = -10.;
							pView->GetMpa()->m_fUp = 30;
							pView->GetMpa()->SetMap(&m_ri3D);
							pView->GetMpa()->Convert();
							pView->SetImageInfo(pView->GetMpa()->GetColorSudoMap());
						}

					}
				}
			}
		}
		bReturn = true;
	} while (false);
	
	return bReturn;
}

bool CEventHandlerMain::FindBase()
{
	bool bReturn = false;

	do 
	{
		CString strMsg;

		CRavidImageView* pView = dynamic_cast<CRavidImageView*>(CUIManager::FindView(2));
		if(!pView)
			break;

		CRavidImage* pImgInfo = pView->GetImageInfo();
		if(!pImgInfo)
			break;

		CRavidImage riFilter(pImgInfo);

		riFilter.SingleThreshold(5);
		CConnectedComponent cc;
		CMultipleVariable rmvThreshold;
		rmvThreshold.AddValue(30);

		cc.SetSingleThreshold(rmvThreshold);
		cc.SetLogicalCondition(ELogicalCondition_Greater);

		double dblAccuracy = 0.02;
		cc.Encode(pImgInfo);

		double dblFilterValue0 = 2000.000;
		cc.Filter(CConnectedComponent::EFilterItem_Area, dblFilterValue0, ELogicalCondition_LessEqual);

		double dblFilterValue1 = 1000.000;
		cc.Filter(CConnectedComponent::EFilterItem_BoundingBoxWidth, dblFilterValue1, ELogicalCondition_LessEqual);

		double dblFilterValue2 = 200.000;
		cc.Filter(CConnectedComponent::EFilterItem_BoundingBoxHeight, dblFilterValue2, ELogicalCondition_LessEqual);


		cc.Sort(CConnectedComponent::ESortItem_Area, CConnectedComponent::ESortMethod_DescendingOrder);
		 
		std::vector<CRavidRect<int>> vctBlob;
		cc.GetBoundary(vctBlob);

		double dblBaseDist = 12.;
		double dblBaseDistPixel = dblBaseDist / dblAccuracy;

		bool bFlag[3] = { false, false, false };
		CRavidLine<double> rlDist[3];
		CRavidPoint<double> rpNormal[3];

		if(vctBlob.size() >= 4) 
		{
			pView->GetLayer(0)->DrawShape(vctBlob[0], CYAN, 1);

			pView->GetLayer(0)->DrawShape(vctBlob[1], YELLOW, 1);
			pView->GetLayer(0)->DrawShape(vctBlob[2], YELLOW, 1);
			pView->GetLayer(0)->DrawShape(vctBlob[3], YELLOW, 1);


			CLineGauge::ETransitionType eType = CLineGauge::ETransitionType_BW;
			CLineGauge::ETransitionChoice eChoice = CLineGauge::ETransitionChoice_LargestAmplitude;
			long nThreshold = 10;
			long nMinAmplitude = 5;
			long nThickness = 11;
			double dblSampling = 1.;
			double dblFilterThreshold = 2.;
			double dblFilterPass = 2.;

			// Line Gague
			CLineGauge lg;
			lg.SetTransitionType(eType);
			lg.SetTransitionChoice(eChoice);
			lg.SetThreshold(nThreshold);
			lg.SetMinimumAmplitude(nMinAmplitude);
			lg.SetThickness(nThickness);
			lg.SetSamplingStep(dblSampling);
			lg.SetOutLiersThreshold(dblFilterThreshold);
			lg.SetNumOfPass(dblFilterPass);
			
			CRavidLine<double> rlMeasuredObjectTop[3];
			CRavidLine<double> rlMeasuredObjectLeft[3];
			CRavidLine<double> rlMeasuredObjectRight[3];
			CRavidLine<double> rlBaseResult[3];

			CRavidLine<double> rlResult[3];
			for(int i = 0; i < 3; ++i)
			{
				lg.SetTransitionChoice(eChoice);
				CRavidLine<double> rl;
				rl.SetLine(vctBlob[i+1].left, vctBlob[i+1].top, vctBlob[i+1].right, vctBlob[i+1].top);
				lg.SetMeasurementRegion(rl, 30);

				if(lg.Measure(&riFilter) == EAlgorithmResult_OK)
				{
					std::vector<CRavidPoint<double> > vctMeasuredValidPoints;
					std::vector<CRavidPoint<double> > vctMeasuredInvalidPoints;

					lg.GetMeasuredObject(rlMeasuredObjectTop[i]);
					lg.GetMeasuredValidPoints(vctMeasuredValidPoints);
					lg.GetMeasuredInvalidPoints(vctMeasuredInvalidPoints);
// 
// 					for(auto& iter : vctMeasuredValidPoints)
// 						pView->GetLayer(0)->DrawShape(iter, YELLOW, 1);

				}
				else
				{
					continue;
				}
				rl.SetLine(vctBlob[i + 1].left, vctBlob[i + 1].bottom, vctBlob[i + 1].left, vctBlob[i + 1].top);
				lg.SetMeasurementRegion(rl, 30);

				if(lg.Measure(&riFilter) == EAlgorithmResult_OK)
				{
					std::vector<CRavidPoint<double> > vctMeasuredValidPoints;
					std::vector<CRavidPoint<double> > vctMeasuredInvalidPoints;

					lg.GetMeasuredObject(rlMeasuredObjectLeft[i]);
					lg.GetMeasuredValidPoints(vctMeasuredValidPoints);
					lg.GetMeasuredInvalidPoints(vctMeasuredInvalidPoints);

// 					for(auto& iter : vctMeasuredValidPoints)
// 						pView->GetLayer(0)->DrawShape(iter, YELLOW, 1);

				}
				else
				{
					continue;
				}

				rl.SetLine(vctBlob[i + 1].right, vctBlob[i + 1].top, vctBlob[i + 1].right, vctBlob[i + 1].bottom);
				lg.SetMeasurementRegion(rl, 30);

				if(lg.Measure(&riFilter) == EAlgorithmResult_OK)
				{
					std::vector<CRavidPoint<double> > vctMeasuredValidPoints;
					std::vector<CRavidPoint<double> > vctMeasuredInvalidPoints;

					lg.GetMeasuredObject(rlMeasuredObjectRight[i]);
					lg.GetMeasuredValidPoints(vctMeasuredValidPoints);
					lg.GetMeasuredInvalidPoints(vctMeasuredInvalidPoints);

// 					for(auto& iter : vctMeasuredValidPoints)
// 						pView->GetLayer(0)->DrawShape(iter, YELLOW, 1);

				}
				else
				{
					continue;
				}

				rlMeasuredObjectTop[i] = rlMeasuredObjectTop[i].GetInfiniteLine();
				rlMeasuredObjectLeft[i] = rlMeasuredObjectLeft[i].GetInfiniteLine();
				rlMeasuredObjectRight[i] = rlMeasuredObjectRight[i].GetInfiniteLine();

				CRavidPoint<double> rp[2];
				if(rlMeasuredObjectTop[i].DoesIntersect(rlMeasuredObjectLeft[i]))
					rp[0] = rlMeasuredObjectTop[i].GetIntersection(rlMeasuredObjectLeft[i]);
				else
					continue;
				if(rlMeasuredObjectTop[i].DoesIntersect(rlMeasuredObjectRight[i]))
					rp[1] = rlMeasuredObjectTop[i].GetIntersection(rlMeasuredObjectRight[i]);
				else
					continue;
				rlResult[i].SetLine(rp[0], rp[1]);
				pView->GetLayer(0)->DrawShape(rlResult[i], LIME, 3);

				rpNormal[i] = rlResult[i].GetNormalVector();

				CRavidLine<double> rlBase;

				rlBase.rpPoints[0] = rlResult[i].rpPoints[0] - (rpNormal[i] * dblBaseDistPixel);
				rlBase.rpPoints[1] = rlResult[i].rpPoints[1] - (rpNormal[i] * dblBaseDistPixel);
				
				pView->GetLayer(0)->DrawShape(rlBase, GRAY, 1);

				lg.SetMeasurementRegion(rlBase, 30);
				lg.SetTransitionChoice(CLineGauge::ETransitionChoice_End);

				if(lg.Measure(&riFilter) == EAlgorithmResult_OK)
				{
			
					lg.GetMeasuredObject(rlBaseResult[i]);
					pView->GetLayer(0)->DrawShape(rlBaseResult[i], BLUE, 1);

// 					for(auto& iter : vctMeasuredValidPoints)
// 						pView->GetLayer(0)->DrawShape(iter, YELLOW, 1);
					CRavidPoint<double> rpP;
					rpP = rlBaseResult[i].GetOrthogonalPoint(rlResult[i].GetCenter());
					pView->GetLayer(0)->DrawShape(rpP.MakeCrossHair(150), GRAY, 1);

					double dblDist = rlResult[i].GetOrthogonalDistance(rlBaseResult[i].GetCenter()) * dblAccuracy;

					rlDist[i].SetLine(rlResult[i].GetCenter(), rpP);
					pView->GetLayer(0)->DrawShape(rlDist[i], LIME, 2);
					strMsg.Format(_T("%0.3f mm"), dblDist);
					pView->GetLayer(0)->DrawText(rpP, strMsg, LIME);
				}
				else
				{ 
					continue;
				}

				bFlag[i]= true;
			}

		}

		if(!bFlag[0] || !bFlag[1] || !bFlag[2])
			break;
		

		CRavidRect<double> rrHeightBase[3];
		CRavidRect<double> rrHeightConnector[3];

		for(int i = 0; i < 3; ++i)
		{
			rrHeightConnector[i].SetRect(rlDist[i].rpPoints[0].x - 120, rlDist[i].rpPoints[0].y + 80, rlDist[i].rpPoints[0].x + 120, rlDist[i].rpPoints[0].y + 100);
			rrHeightBase[i].SetRect(rlDist[i].rpPoints[1].x - 120, rlDist[i].rpPoints[1].y + 120, rlDist[i].rpPoints[1].x + 120, rlDist[i].rpPoints[1].y + 180);

			pView->GetLayer(0)->DrawShape(rrHeightConnector[i], ORANGE, 2);
			pView->GetLayer(0)->DrawShape(rrHeightBase[i], ORANGE, 2);

		}
		

		CRavidPoint<double> rpAccuracy3D;
		rpAccuracy3D.x = 0.178;
		rpAccuracy3D.y = 0.02;

		CRavidPoint<double> rpAccurayRatio;
		CImageView3D* pView3 = dynamic_cast<CImageView3D*>(CUIManager::FindView(1));
		
		if(pView3)
		{
			pView3->GetLayer(1)->Clear();

			rpAccurayRatio.x = rpAccuracy3D.x / dblAccuracy;
			rpAccurayRatio.y = rpAccuracy3D.y / dblAccuracy;

			for(int i = 0; i < 3; ++i)
			{
				rrHeightConnector[i].left /= rpAccurayRatio.x;
				rrHeightConnector[i].right /= rpAccurayRatio.x;
				rrHeightBase[i].left /= rpAccurayRatio.x;
				rrHeightBase[i].right /= rpAccurayRatio.x;

				auto pLayerOne = pView3->GetLayer(1);
				pLayerOne->DrawShape(rrHeightConnector[i], ORANGE);
				pLayerOne->DrawShape(rrHeightBase[i],ORANGE);
				
				
				
				

				pView3->GetLayer(1)->DrawShape(rrHeightConnector[i], ORANGE);
				pView3->GetLayer(1)->DrawShape(rrHeightBase[i], ORANGE);

				CMultipleVariable mvResult[2];
				double dblZ[2] = {0, };
				if(m_ri3D.GetMin(&mvResult[0], &rrHeightBase[i]) == EAlgorithmResult_OK)
				{
					dblZ[0] = mvResult[0].GetAt(0);
					strMsg.Format(_T("Z = %0.3f mm"), dblZ[0]);
					pView3->GetLayer(1)->DrawText(rrHeightBase[i].GetCenter(), strMsg, ORANGE); 
				}
				else
					continue;
				if(m_ri3D.GetMin(&mvResult[1], &rrHeightConnector[i]) == EAlgorithmResult_OK)
				{
					dblZ[1] = mvResult[1].GetAt(0);
					strMsg.Format(_T("Z = %0.3f mm"), dblZ[1]);
					pView3->GetLayer(1)->DrawText(rrHeightConnector[i].GetCenter(), strMsg, ORANGE);

				}
				else
					continue;

				double dblResultZ = dblZ[0] - dblZ[1];
				strMsg.Format(_T("Z Gap = %0.3f mm"), dblResultZ);

				pView3->GetLayer(1)->DrawText((rrHeightConnector[i].GetCenter() + rrHeightBase[i].GetCenter())/2, strMsg, ORANGE);
				pView3->Invalidate();
			}

		}
		bReturn = true;
	} while (false);
	return bReturn;
}

