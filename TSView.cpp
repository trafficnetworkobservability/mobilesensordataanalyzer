// TSView.cpp : implementation of the CTimeSpaceView class
//

#include "stdafx.h"
#pragma warning(disable: 4786) // kill exceedingly long STL symbol warning
#include <set>
#include "TLite.h"
#include "TLiteDoc.h"
#include "Network.h"

#include "TSView.h"
#include "Dlg_Find_Vehicle.h"
#include <vector>
#include <sstream>
#include <algorithm>

#include "DlgCarFollowingModelSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern void g_SetCursor(_cursor_type cursor_type);
extern float g_Simulation_Time_Stamp;
extern CPen s_PenSimulationClock;
extern void g_SelectColorCode(CDC* pDC, int ColorCount);
extern void g_SelectSuperThickPenColor(CDC* pDC, int ColorCount);
CPen g_TempLinkPen2(PS_DASH,0,RGB(0,255,255));

CPen ThickPen(PS_SOLID,3,RGB(0,255,0));
CPen DoublePen(PS_SOLID,2,RGB(0,255,0));

CPen NormalPen(PS_SOLID,1,RGB(0,255,0));
CPen Normal2Pen(PS_SOLID,2,RGB(0,0,255));

CPen v_BluePen(PS_SOLID,1,RGB(0,0,255));
CPen v_BlackPen(PS_SOLID,1,RGB(0,0,0));
CPen v_RedPen(PS_SOLID,1,RGB(255,0,0));



CPen Pen(PS_SOLID,1,RGB(0,0,0));

CPen DashPen(PS_DASH,0,RGB(0,255,0));
CPen DoubleDashPen(PS_DASH,2,RGB(0,255,0));
CPen SelectPen(PS_DASH,2,RGB(255,0,0));

/////////////////////////////////////////////////////////////////////////////
// CTimeSpaceView

//-----------put num_front_vehs into scenario file, num_deduct_vehicles
int number_of_total_vehicles_output = 20;
int num_deduct_vehicles = 107;

IMPLEMENT_DYNCREATE(CTimeSpaceView, CView)

BEGIN_MESSAGE_MAP(CTimeSpaceView, CView)
	//{{AFX_MSG_MAP(CTimeSpaceView)
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TIMETABLE_RESOURCEPRICE, &CTimeSpaceView::OnTimetableResourceprice)
	ON_UPDATE_COMMAND_UI(ID_TIMETABLE_RESOURCEPRICE, &CTimeSpaceView::OnUpdateTimetableResourceprice)
	ON_COMMAND(ID_TIMETABLE_NODECOSTLABEL, &CTimeSpaceView::OnTimetableNodecostlabel)
	ON_UPDATE_COMMAND_UI(ID_TIMETABLE_NODECOSTLABEL, &CTimeSpaceView::OnUpdateTimetableNodecostlabel)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_TOOLS_LOADVEHICLETRAJECTORYFILE, &CTimeSpaceView::OnToolsLoadvehicletrajactoryfile)
	ON_COMMAND(ID_NGSIMDATA_LANE1, &CTimeSpaceView::OnNgsimdataLane1)
	ON_COMMAND(ID_NGSIMDATA_LANE2, &CTimeSpaceView::OnNgsimdataLane2)
	ON_COMMAND(ID_NGSIMDATA_LANE3, &CTimeSpaceView::OnNgsimdataLane3)
	ON_COMMAND(ID_NGSIMDATA_LANE4, &CTimeSpaceView::OnNgsimdataLane4)
	ON_COMMAND(ID_NGSIMDATA_LANE5, &CTimeSpaceView::OnNgsimdataLane5)
	ON_COMMAND(ID_NGSIMDATA_LANE6, &CTimeSpaceView::OnNgsimdataLane6)
	ON_COMMAND(ID_NGSIMDATA_LANE7, &CTimeSpaceView::OnNgsimdataLane7)
	ON_COMMAND(ID_NGSIMDATA_COLORSPEED, &CTimeSpaceView::OnNgsimdataColorspeed)
	ON_UPDATE_COMMAND_UI(ID_NGSIMDATA_COLORSPEED, &CTimeSpaceView::OnUpdateNgsimdataColorspeed)
	ON_COMMAND(ID_NGSIMDATA_CARFOLLOWINGSIMULATION, &CTimeSpaceView::OnNgsimdataCarfollowingsimulation)
	ON_COMMAND(ID_NGSIMDATA_SHOWSIMULATEDRESULTS, &CTimeSpaceView::OnNgsimdataShowsimulatedresults)
	ON_UPDATE_COMMAND_UI(ID_NGSIMDATA_SHOWSIMULATEDRESULTS, &CTimeSpaceView::OnUpdateNgsimdataShowsimulatedresults)
	ON_UPDATE_COMMAND_UI(ID_NGSIMDATA_SHOWBOTHOBSERVEDANDSIMULATIONRESULTS, &CTimeSpaceView::OnUpdateNgsimdataShowbothobservedandsimulationresults)
	ON_COMMAND(ID_NGSIMDATA_SHOWBOTHOBSERVEDANDSIMULATIONRESULTS, &CTimeSpaceView::OnNgsimdataShowbothobservedandsimulationresults)
	ON_COMMAND(ID_NGSIMDATA_CHANGESETTINGSOFCARFOLLOWINGMODEL, &CTimeSpaceView::OnNgsimdataChangesettingsofcarfollowingmodel)
	ON_COMMAND(ID_NGSIMDATA_CARFOLLOWINGDATAEXTRACTION, &CTimeSpaceView::OnNgsimdataCarfollowingdataextraction)
	ON_COMMAND(ID_NGSIMDATA_SHOWVEHICLEID, &CTimeSpaceView::OnNgsimdataShowvehicleid)
	ON_UPDATE_COMMAND_UI(ID_NGSIMDATA_SHOWVEHICLEID, &CTimeSpaceView::OnUpdateNgsimdataShowvehicleid)
	ON_COMMAND(ID_NGSIMDATA_SHOWPRECEEDINGVEHICLEPOSITION, &CTimeSpaceView::OnNgsimdataShowpreceedingvehicleposition)
	ON_UPDATE_COMMAND_UI(ID_NGSIMDATA_SHOWPRECEEDINGVEHICLEPOSITION, &CTimeSpaceView::OnUpdateNgsimdataShowpreceedingvehicleposition)
	ON_COMMAND(ID_NGSIMDATA_SEARCH_BY_VEHICLE_ID, &CTimeSpaceView::OnNgsimdataSearchByVehicleId)
	ON_COMMAND(ID_NGSIMMENU_EXPORTCUMULATIVEFLOWCOUNTDATA, &CTimeSpaceView::OnNgsimmenuExportcumulativeflowcountdata)
	ON_COMMAND(ID_NGSIMMENU_SHOWCALCULATEDCUMULATIVEFLOWCOUNTANDDENSITY, &CTimeSpaceView::OnNgsimmenuShowcalculatedcumulativeflowcountanddensity)
	ON_UPDATE_COMMAND_UI(ID_NGSIMMENU_SHOWCALCULATEDCUMULATIVEFLOWCOUNTANDDENSITY, &CTimeSpaceView::OnUpdateNgsimmenuShowcalculatedcumulativeflowcountanddensity)
	ON_COMMAND(ID_NGSIMMENU_SHOWSPACETIME_CONTOUR, &CTimeSpaceView::OnNgsimmenuShowspacetimeContour)
	ON_UPDATE_COMMAND_UI(ID_NGSIMMENU_SHOWSPACETIME_CONTOUR, &CTimeSpaceView::OnUpdateNgsimmenuShowspacetimeContour)

	ON_COMMAND(ID_NGSIMMENU_EXPORT_FUNDAMENTAL_VARIABLES, &CTimeSpaceView::OnNgsimmenuExportFDStreamVariables)
	ON_UPDATE_COMMAND_UI(ID_NGSIMMENU_EXPORT_FUNDAMENTAL_VARIABLES, &CTimeSpaceView::OnUpdateNgsimmenuExportFDStreamVariables)

	ON_COMMAND(ID_NGSIMMENU_SHOWCDFPLOT, &CTimeSpaceView::OnNgsimmenuShowcdfplot)
	ON_UPDATE_COMMAND_UI(ID_NGSIMMENU_SHOWCDFPLOT, &CTimeSpaceView::OnUpdateNgsimmenuShowcdfplot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSpaceView construction/destruction
CTimeSpaceView::CTimeSpaceView()
{

	m_MaxBinValue = 200;  // 20 seconds
	m_BinSize = 50;
	m_MaxFrequency = 50;

	m_bShowCumulativeFlowCount = false;
	m_bShowDensityContour = false;
	m_bExportFDStreamVariables = false;
	m_bShowPDFPlot = false;

	m_DataCollectionTimeInternval_in_sec = 30;
	m_SelectedVehicleCount = 0;
	m_SelectedStartTime = m_SelectedEndTime = 0;
	m_SelectedStartLocalY = m_SelectedEndLocalY = 0.0f;

	m_bMouseDownFlag = false;
	m_bLoadedDataFlag = false;
	m_NumberOfVehicles_lane = 0;
	m_KJam = 220; // 220 veh/mile
	m_WaveSpeed = 12; // in mph 
	m_FreeflowSpeed = 60.0f;

	m_bShowVehicleIDFlag = false;
	m_bShowLeaderPositionFlag = false;

	m_bShowSimulationDataFlag = false;
	m_bShowBothResultsFlag = false;
	m_bColorBySpeedFlag = false;
	m_bExcludeLaneChangingVehicles = false;

	m_SelectLaneID = 0;
	m_SelectedVehicleID = -1;
	m_NumberOfVehicles = 0;
	m_NumberOfTimeSteps = 0;

	m_bShowTimetable = true;

	m_bShowSlackTime = true;

	Cur_MOE_type1 = NGSIM_trajectory; 
	Cur_MOE_type2 = NGSIM_simulated_trajectory;

	m_NumberOfTimeSteps = 1440;
	m_TimeLeft = 0;
	m_TimeRight = m_NumberOfTimeSteps; 
	m_LocalYLowerBound = 0;

	m_bShowResourcePrice = false;
	m_bShowNodeCostLabel = false;

	bRangeInitialized = false;

	for(int LaneID = 0; LaneID < 7; LaneID++)
		VehicleSizeLane[LaneID] = 0;
}

CTimeSpaceView::~CTimeSpaceView()
{
	if(m_NumberOfVehicles>0)
	{
		m_VehicleDataList.clear ();
	}

	if(m_NumberOfVehicles_lane)
	{
		Deallocate3DDynamicArray<float>(m_VehicleSnapshotAry_Lane,_MAX_LANE_SIZE,m_NumberOfVehicles_lane);

		DeallocateDynamicArray<int>(m_VehicleStartTimeAry_Lane , _MAX_LANE_SIZE,m_NumberOfVehicles_lane);
		DeallocateDynamicArray<int>(m_VehicleEndTimeAry_Lane , _MAX_LANE_SIZE,m_NumberOfVehicles_lane);

	}


}

BOOL CTimeSpaceView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

CTLiteDoc* CTimeSpaceView::GetTLDocument() 
{ return reinterpret_cast<CTLiteDoc*>(m_pDocument); }

/////////////////////////////////////////////////////////////////////////////
// CTimeSpaceView drawing

void CTimeSpaceView::OnDraw(CDC* pDC)
{
	if (m_bLoadedDataFlag == false)
	{
		m_bLoadedDataFlag = true;
		if (((CTLiteApp*)AfxGetApp())->m_bLoadedDataFromVissim)
		{
			OnToolsLoadvehicletrajactoryfileFromVissim(((CTLiteApp*)AfxGetApp())->m_metric);
		}
		else
		{
			OnToolsLoadvehicletrajactoryfile();
		}
	}
	CRect rectClient(0,0,0,0);
	GetClientRect(&rectClient);

	CRect PlotRect;
	GetClientRect(PlotRect);

	CRect PlotRectOrg = PlotRect;

	PlotRect.top += 60;
	PlotRect.bottom -= 60;
	PlotRect.left += 60;
	PlotRect.right -= 60;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rectClient.Width(),
		rectClient.Height());
	memDC.SelectObject(&bmp);

	// Custom draw on top of memDC
	CBrush brush;
	if (!brush.CreateSolidBrush(RGB(255,255,255)))
		return;

	brush.UnrealizeObject();
	memDC.FillRect(rectClient, &brush);

	m_PlotRectNGSIM = PlotRect;

	if(!m_bShowBothResultsFlag) 	 // only one window	
		DrawNGSIMVehicleTrajectory(&memDC,Cur_MOE_type1,PlotRect);
	else
	{
		m_bShowSimulationDataFlag = false;
		PlotRect.bottom = PlotRectOrg.bottom /2;

		m_PlotRectNGSIM = PlotRect;

		DrawNGSIMVehicleTrajectory(&memDC,Cur_MOE_type1,PlotRect);  // observed data


		m_bShowSimulationDataFlag = true;
		PlotRect.top = PlotRectOrg.bottom /2+45;
		PlotRect.bottom = PlotRectOrg.bottom -20;

		m_PlotRectSimulated =  PlotRect;

		DrawNGSIMVehicleTrajectory(&memDC,Cur_MOE_type2,PlotRect);  // simulated data 
	}

	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0,
		0, SRCCOPY);

	ReleaseDC(pDC);

	// TODO: add draw code for native data here

}

/////////////////////////////////////////////////////////////////////////////
// CTimeSpaceView diagnostics

#ifdef _DEBUG
void CTimeSpaceView::AssertValid() const
{
	CView::AssertValid();
}

void CTimeSpaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTimeSpaceView message handlers

void CTimeSpaceView::DrawFramework(CDC* pDC, int MOEType, CRect PlotRect)
{
	if (!bRangeInitialized)
	{
		InitializeTimeRange();
		bRangeInitialized = true;
	}

	if (m_TimeLeft<0)
		m_TimeLeft = 0;

	if (m_TimeRight > m_NumberOfTimeSteps)
		m_TimeRight = m_NumberOfTimeSteps;

	CPen DataPen(PS_SOLID, 1, RGB(0, 0, 0));

	CPen TimePen(PS_DOT, 1, RGB(0, 255, 0));


	int i;
	// step 2: calculate m_UnitDistance;
	// data unit
	m_UnitDistance = 1;
	if ((m_LocalYUpperBound - m_LocalYLowerBound) > 0)
		m_UnitDistance = (float)(PlotRect.bottom - PlotRect.top) / (m_LocalYUpperBound - m_LocalYLowerBound);



	//PlotRect.right = PlotRect.left + 36000 * PlotRect.Height() / (10.0*5280.0);

	// step 3: time interval
	int TimeXPosition;

	int TimeInterval = FindClosestTimeResolution(m_TimeRight - m_TimeLeft);

	// time unit
	m_UnitTime = 1;
	if ((m_TimeRight - m_TimeLeft) > 0)
	{
		if (m_45_degree_velocity > 0.0f)
		{
			m_UnitTime = (float)(PlotRect.bottom - PlotRect.top) / ((m_LocalYUpperBound - m_LocalYLowerBound) / (m_45_degree_velocity * 5280.0 / 36000.0));
		}
		else
		{
			m_UnitTime = (float)(PlotRect.right - PlotRect.left) / (m_TimeRight - m_TimeLeft);
		}
	}

	// step 4: draw time axis

	pDC->SelectObject(&TimePen);

	char buff[20];
	for(i=m_TimeLeft;i<=m_TimeRight;i+=TimeInterval)
	{
		TimeXPosition=(int)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

		if(i>= m_TimeLeft)
		{
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);

			if(i/2 <10)
				TimeXPosition-=5;
			else
				TimeXPosition-=3;

			int min, sec;
			min = i/600;
			sec =  (i- min*600)/10;
			wsprintf(buff,"%2d:%02d",min, sec);
			pDC->TextOut(TimeXPosition,PlotRect.bottom+3,buff);
		}
	}

	// 	step 4: draw time axis

	pDC->SelectObject(&NormalPen);

	pDC->MoveTo(PlotRect.left, PlotRect.top);
	pDC->LineTo(PlotRect.left,PlotRect.bottom);

	pDC->MoveTo(PlotRect.right, PlotRect.top);
	pDC->LineTo(PlotRect.right,PlotRect.bottom);


}


//SITSLAB output NGSIM matrix
void CTimeSpaceView::Output_NGSIM_Matrix(string output_type)
{
	
	int time_unit = 1;		//0.1 sec
	string NGSIM_output_matrix_file_name = "C:\\NEXTA_DTALite_SoftwareRelease\\NGSIM_space_time_trajectory_" + output_type + "_Lane" + to_string(m_SelectLaneID) + ".csv";
	ofstream NGSIM_output_file;
	NGSIM_output_file.open(NGSIM_output_matrix_file_name, fstream::out);
	if (NGSIM_output_file.is_open())
	{
		NGSIM_output_file << "m_CorridorSensorData.LinkSensorDataVector.size()=" <<m_CorridorSensorData.LinkSensorDataVector.size() << endl;
		for (int iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
		{

			LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];
			
			int cumulative_count = 0;
			for (int t = 0; t < element.SpaceScanCountVector.size(); t++)
			{
				NGSIM_output_file << iLink << ",";
				NGSIM_output_file << t << ",";

				int timestamp = t* element.DataCollectionTimeInterval_in_sec * 10;
				int count = CountVehicles(timestamp, timestamp, element.DetectorLocalY_Downstream, element.DetectorLocalY_Upstream);
				cumulative_count = cumulative_count + count;
				float Density = count*1.0 / element.DetectorSpacing_in_miles;

				if (output_type == "cumulative_count")
				{
					NGSIM_output_file << cumulative_count << endl;
				}
				else if (output_type == "density")
				{
					NGSIM_output_file << Density << endl;
				}
				else if (output_type == "cumulative_Flow_count")
				{
					NGSIM_output_file << element.GetCumulativeFlowCount(timestamp / 10) << endl;
				}
				
				//m_CorridorSensorData.LinkSensorDataVector[iLink].SpaceScanCountVector[t] = count;

				//m_CorridorSensorData.LinkSensorDataVector[iLink].DensityVector[t] = count*1.0/element.DetectorSpacing_in_miles;

			}

		}
	}
	else
	{
		cout << "can not open " << NGSIM_output_matrix_file_name << endl;
	}
	NGSIM_output_file.close();

}

void CTimeSpaceView::Output_NGSIM_Matrix(string output_type, float SensorSpacing, map<float, map<int, float>> & Distance_Time_Value)
{


	//Time_Distance_Value.clear();

	//int time_unit = 1;		//0.1 sec

	string str = "";
	if (m_bExcludeLaneChangingVehicles)
	{
		str = "filtered";

	}
	string NGSIM_output_matrix_file_name = "C:\\NEXTA_DTALite_SoftwareRelease\\NGSIM_space_time_trajectory_" + str + "_" + output_type + "_Lane" + to_string(m_SelectLaneID) + ",x=" + to_string(SensorSpacing) + ",t=" + to_string(m_DataCollectionTimeInternval_in_sec) + ".csv";
	ofstream NGSIM_output_file;
	NGSIM_output_file.open(NGSIM_output_matrix_file_name, fstream::out);
	if (NGSIM_output_file.is_open())
	{	
		for (int iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
		{

			LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];

			int cumulative_count = 0;

			map<int,float> time_value;
			//time_value.reserve(element.SpaceScanCountVector.size());

			for (int t = 0; t < element.SpaceScanCountVector.size(); t++)
			{
				NGSIM_output_file << iLink * SensorSpacing << ",";

				int timestamp = t* element.DataCollectionTimeInterval_in_sec * 10;
				NGSIM_output_file << timestamp << ",";
				int count = CountVehicles(timestamp, timestamp, element.DetectorLocalY_Downstream, element.DetectorLocalY_Upstream);
				cumulative_count = cumulative_count + count;
				float Density = count*1.0 / element.DetectorSpacing_in_miles;
				
				//Time_Distance_Value;
				if (output_type == "cumulative_count")
				{
					NGSIM_output_file << cumulative_count << endl;
					time_value[timestamp/10] = cumulative_count;
				}
				else if (output_type == "density")
				{
					NGSIM_output_file << Density << endl;
					time_value[timestamp/10] = Density;
				}
				else if (output_type == "cumulative_Flow_count")
				{
					int cumulative_Flow_count = element.GetCumulativeFlowCount(timestamp / 10);
					NGSIM_output_file << cumulative_Flow_count << endl;
					time_value[timestamp/10] = cumulative_Flow_count;
				}

				//m_CorridorSensorData.LinkSensorDataVector[iLink].SpaceScanCountVector[t] = count;

				//m_CorridorSensorData.LinkSensorDataVector[iLink].DensityVector[t] = count*1.0/element.DetectorSpacing_in_miles;

			}
			Distance_Time_Value[iLink * SensorSpacing] = time_value;
		}
	}
	else
	{
		cout << "can not open " << NGSIM_output_matrix_file_name << endl;
	}
	NGSIM_output_file.close();

}

void CTimeSpaceView::Output_NGSIM_Individual_Vehicle_Trajectory(int display_resolution, CRect PlotRect, map<float, map<int, float>> & Distance_Time_Value)
{
	m_bExcludeLaneChangingVehicles = true;
	string str = "";
	if (m_bExcludeLaneChangingVehicles)
	{
		str = "filtered";

	}

	if (Distance_Time_Value.size() == 0)
		cout << "Distance_Time_Value is empty!" << endl;

	string NGSIM_output_file_name = "C:\\NEXTA_DTALite_SoftwareRelease\\NGSIM_Vehicle_trajectory_Lane" + to_string(m_SelectLaneID) + "_" + str + ",display_t=" + to_string(display_resolution) + ",t=" + to_string(m_DataCollectionTimeInternval_in_sec) + ".csv";
	ofstream NGSIM_output_file;
	NGSIM_output_file.open(NGSIM_output_file_name, fstream::out);
	if (NGSIM_output_file.is_open())
	{
		for (int v = 0; v < m_NumberOfVehicles; v += 1)
		{
			float StartTime = m_VehicleDataList[v].StartTime;

			if (StartTime < m_TimeLeft)
				StartTime = m_TimeLeft;

			float EndTime = m_VehicleDataList[v].EndTime;

			if (EndTime > m_TimeRight)
				EndTime = m_TimeRight;

			if (StartTime < EndTime)
			{
				for (int t = StartTime; t <= EndTime; t += display_resolution)
				{
					if (m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID)  // on the currently selected lane
					{
						

// 						int x = (int)m_VehicleSnapshotAry[v][t].LocalX;  // timestamp
 						int y = (int)m_VehicleSnapshotAry[v][t].LocalY;  // distance
// 						int x = (int)(PlotRect.left + (t - m_TimeLeft)*m_UnitTime);  // timestamp
// 						int y = PlotRect.bottom - (int)(((m_VehicleSnapshotAry[v][t].LocalY - m_LocalYLowerBound)*m_UnitDistance) + 0.50);  // distance
						

						map<float, map<int, float>>::iterator Distance_Time_Value_Iter_Low;
						Distance_Time_Value_Iter_Low = Distance_Time_Value.lower_bound(y);
						float output_value=0;
						if (Distance_Time_Value_Iter_Low != Distance_Time_Value.end())
						{
							map<int, float> time_value;
							time_value = Distance_Time_Value_Iter_Low->second;

							map<int, float>::iterator Time_Value_Iter_Low;
							Time_Value_Iter_Low = time_value.lower_bound(t/10);
							if (Time_Value_Iter_Low != time_value.end())
							{
								output_value = Time_Value_Iter_Low->second;
								NGSIM_output_file
									<< v << ","
									<< t << ","
									<< y << ","
									<< output_value << endl;
							}
							else
							{
								output_value = -2;
								cout << "Time_Value_Iter_Low == time_value.end() at t = " << t << " ,y = " << y << endl;
							}
								
						}
						else						
						{
							output_value = -1;
							cout << "Distance_Time_Value_Iter_Low == Distance_Time_Value.end() at y = " << y << endl;
						}



// 						NGSIM_output_file 
// 							<< v << ","
// 							<< t << ","
// 							<< y << ","
// 							<< output_value <<endl;
					}
				}
			}
		}
	}
	else
	{
		cout << "can not open " << NGSIM_output_file_name << endl;
	}
	NGSIM_output_file.close();
}

void CTimeSpaceView::DrawNGSIMVehicleTrajectory(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect)
{
	DrawFramework(pDC,MOEType,PlotRect);

	if(m_CorridorSensorData.LinkSensorDataVector .size()>0)  // data available
	{
		DrawSpaceTimeContour(pDC,MOEType,PlotRect);
	}

	pDC->SetBkMode(TRANSPARENT);

	CString str_MOE, str_selected_link;

	if(m_bShowSimulationDataFlag && MOEType != NGSIM_trajectory)
		str_MOE.Format ("Time-Space Graph: Lane %d (simulated), Kj = %5.1f vml, W = %5.1f, Vf= %5.1f", m_SelectLaneID,m_KJam,m_WaveSpeed,m_FreeflowSpeed);
	else
	{  // NGSIM_trajectory
		str_MOE.Format ("Time-Space Graph: Lane %d (observed)", m_SelectLaneID);

		float distance = fabs(m_SelectedEndLocalY - m_SelectedStartLocalY)/5280.0f;
		float time = abs(m_SelectedEndTime - m_SelectedStartTime)/10.0f;  // in sec
		float speed_in_mph = distance/max(0.000001,time/3600); // in mph

		int count_of_vehicles = m_SelectedVehicleCount;
		float density = count_of_vehicles/max(0.0001, distance);  // per mile
		float flow = count_of_vehicles/max(0.000001, time/3600); // per hour

		str_selected_link.Format("from time %d to %d, [%.1f sec], from distance %8.3f to %8.3f [%8.4f miles], speed = %5.2f mph, # of veh = %d, k = %5.1f, q = %5.1f", 
			m_SelectedStartTime, m_SelectedEndTime,time ,
			m_SelectedStartLocalY, m_SelectedEndLocalY,distance,
			speed_in_mph, count_of_vehicles, density, flow
			);

	}

	if(m_TempLinkStartPoint!= m_TempLinkEndPoint)
	{
		pDC->SelectObject(&Normal2Pen);
		pDC->MoveTo(m_TempLinkStartPoint);
		pDC->LineTo(m_TempLinkEndPoint);
	}

	pDC->SelectObject(&NormalPen);

	pDC->TextOut(PlotRect.right/2-100,PlotRect.top-30,str_MOE);

	if(str_selected_link.GetLength () > 0)
		pDC->TextOut(PlotRect.right/2-300,PlotRect.top-10,str_selected_link);

	// draw vehicles

	int display_resolution = 1;
	display_resolution = max(1,(m_TimeRight - m_TimeLeft)/2000.0f);

	if(m_bShowSimulationDataFlag == false)
	{ // show original data

		for(int v=0; v<m_NumberOfVehicles; v+=1)
		{
			float StartTime = m_VehicleDataList[v].StartTime;

			if(StartTime < m_TimeLeft)
				StartTime = m_TimeLeft;

			float EndTime = m_VehicleDataList[v].EndTime;

			if(EndTime > m_TimeRight)
				EndTime = m_TimeRight;

			bool bIniFlag  = false;
			if(StartTime< EndTime)
			{
				for(int t= StartTime; t<=EndTime; t+=display_resolution)  
				{

					if(m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID)  // on the currently selected lane
					{

						if(!m_bColorBySpeedFlag)
						{
							switch(m_VehicleDataList[v].VehicleType)
							{

							case 1: pDC->SelectObject(&v_BlackPen); break;  // passenger 
							case 2: pDC->SelectObject(&v_BlackPen); break;  // truck
							case 3: pDC->SelectObject(&v_BluePen); break;   // motocycle
							default: 
								pDC->SelectObject(&v_BlackPen);
							}

							if(m_SelectedVehicleID == m_VehicleDataList[v].VehicleID)
							{
								pDC->SelectObject(&v_RedPen);
							}

						}else
						{
							CPen penmoe;

							float power;

							float speed = max(1,m_VehicleSnapshotAry[v][t].Speed_mph);
							power= min(1.0f, 55/speed);

							float n= power*100;
							int R=(int)((255*n)/100);
							int G=(int)(255*(100-n)/100); 
							int B=0;

							penmoe.CreatePen (PS_SOLID, 1, RGB(R,G,B));
							pDC->SelectObject(&penmoe);

						}


						int x=(int)(PlotRect.left+(t - m_TimeLeft)*m_UnitTime);  // timestamp
						int y = PlotRect.bottom - (int)(((m_VehicleSnapshotAry[v][t].LocalY-m_LocalYLowerBound)*m_UnitDistance)+0.50);  // distance


						if(!bIniFlag)
						{
							pDC->MoveTo(x,y);

							if(m_bShowVehicleIDFlag)
							{
								CString str_vid;
								str_vid.Format ("%d",m_VehicleDataList[v].VehicleID);


								if(m_SelectedVehicleID == m_VehicleDataList[v].VehicleID)
								{
									pDC->SetTextColor(RGB(255,0,0));
								}else
								{
									pDC->SetTextColor(RGB(0,0,0));
								}


								pDC->TextOut(x,y,str_vid);
							}

							bIniFlag = true;

						}else
						{
							//select color according to vehicle type
							switch(m_VehicleDataList[v].VehicleType)
							{

							case 1: pDC->SelectObject(&v_BluePen); break;
							case 2: pDC->SelectObject(&v_BlackPen); break;
							case 3: pDC->SelectObject(&v_RedPen); break;
							default:
								pDC->SelectObject(&v_BlackPen);
							}

							int follower_x = x;
							int follower_y = y;
							pDC->LineTo(x,y);

							if(m_bShowLeaderPositionFlag && (t+v)%50==0) // every 5 seconds
							{
								int leader_vid = m_VehicleSnapshotAry[v][t].PrecedingVehicleID;
								if(leader_vid>0) // preceeding vehicle has been identified in the orignal NGSim data
								{

									int leader_no = m_VehicleIDtoNOMap[leader_vid];  // convert vid to no in our array

									if( leader_no > 0 && leader_no < m_NumberOfVehicles)
									{
										pDC->SelectObject(&DashPen);

										int leader_y = PlotRect.bottom - (int)(((m_VehicleSnapshotAry[leader_no][t].LocalY-m_LocalYLowerBound)*m_UnitDistance)+0.50);
										pDC->LineTo(x,leader_y);

										if(m_VehicleSnapshotAry[leader_no][t].LaneID != m_VehicleSnapshotAry[v][t].LaneID)  // not on the same lane!
										{
											CString str_lane;
											str_lane.Format ("L%d",m_VehicleSnapshotAry[leader_no][t].LaneID );
											pDC->TextOut(x,leader_y,str_lane);
										}
									}

								}

								pDC->MoveTo(follower_x,follower_y);


							}


						}

					}

				}
			}
		}

	}else
	{
		// show simulation data for m_SelectLaneID


		int SelectedLaneNo = m_SelectLaneID-1; //SelectedLaneNo starts from 0
		for(int v=0; v<VehicleSizeLane[SelectedLaneNo]; v+=1)
		{
			bool bIniFlag  = false;
			for(int t= m_VehicleStartTimeAry_Lane[SelectedLaneNo][v]; t<=m_VehicleEndTimeAry_Lane[SelectedLaneNo][v]; t+=display_resolution)  
			{
				pDC->SelectObject(&v_BlackPen);

				int x=(int)(PlotRect.left+(t - m_TimeLeft)*m_UnitTime);
				int y = PlotRect.bottom - (int)(((m_VehicleSnapshotAry_Lane[SelectedLaneNo][v][t]-m_LocalYLowerBound)*m_UnitDistance)+0.50);


				if(!bIniFlag)
				{
					pDC->MoveTo(x,y);
					bIniFlag = true;

				}else
				{
					pDC->LineTo(x,y);

				}

			}

		}
	}

	if(m_CorridorSensorData.LinkSensorDataVector .size()>0)  // data available
	{
		DrawPDFPlotsForAllLinks(pDC,MOEType,PlotRect);
	}


}

void CTimeSpaceView::DrawSpaceTimeContour(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect)
{

	CPen DataTimePen(PS_DASH,1,RGB(0,255,0));

	pDC->SelectObject(&DataTimePen);


	int i;
	for(i=m_TimeLeft;i<=m_TimeRight;i+=m_DataCollectionTimeInternval_in_sec*10)
	{
		int TimeXPosition=(int)(PlotRect.left+(i-m_TimeLeft)*m_UnitTime);

		if(i>= m_TimeLeft)
		{
			pDC->MoveTo(TimeXPosition,PlotRect.bottom+2);
			pDC->LineTo(TimeXPosition,PlotRect.top);

		}
	}

	pDC->SetTextColor(RGB(0,0,255));



	int bSpace = -1;
	int iLink = 0;


	for (iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{
		LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];
		int YFrom = PlotRect.bottom - (int)((element.DetectorLocalY_Upstream*m_UnitDistance)+0.50);
		int YTo= PlotRect.bottom - (int)((element.DetectorLocalY_Downstream *m_UnitDistance)+0.50);

		pDC->SetBkMode(TRANSPARENT);
		CString str;
		str.Format("sensor %d",iLink );
		pDC->TextOut(PlotRect.left-60,YFrom-5,str);

		///
		for(int t=m_TimeLeft; t<=m_TimeRight; t+= m_DataCollectionTimeInternval_in_sec*10)
		{
			int TimeXPosition=(int)(PlotRect.left+(t-m_TimeLeft)*m_UnitTime);

			float density;
			int cumulative_count  = element.GetCumulativeFlowCount (t/10);
			int flow_count = element.GetFlowCount (t/10);
			int vehicle_space_count  = element.GetSpaceCount  (t/10);

			int vehicle_space_scan_count = 0;

			float sensor_spacing = (element.DetectorLocalY_Downstream - element.DetectorLocalY_Upstream)/5280;
			if(MOEType == NGSIM_trajectory)
			{
				vehicle_space_scan_count  = element.GetSpaceScanCount  (t/10);
				density  = vehicle_space_scan_count / sensor_spacing;
				str.Format("%d,<%3.0f>",vehicle_space_scan_count, density);
			}
			else{
				vehicle_space_scan_count  = element.GetEstimatedSpaceScanCount  (t/10);
				density  = vehicle_space_scan_count / sensor_spacing;
				str.Format("%d,<%3.0f>",vehicle_space_scan_count, density);
			}

//			str.Format("%d,%d,%d,%d,<%3.0f>",cumulative_count,flow_count,vehicle_space_count,vehicle_space_scan_count, density);


			if(m_bShowCumulativeFlowCount)
				pDC->TextOut(TimeXPosition,YFrom,str);

			if(m_bShowDensityContour)
			{
				CRect cell;
				cell.left = TimeXPosition;
				cell.right = (int)(PlotRect.left+(t+ m_DataCollectionTimeInternval_in_sec*10 -m_TimeLeft)*m_UnitTime);
				cell.top = YTo;
				cell.bottom  = YFrom;

				float power = min(1,density / 250);
				float n= power*100;
				int R=(int)((255*n)/100);
				//			int R= 255;
				int G=(int)(255*(100-n)/100); 
				int B=0;
				pDC->FillSolidRect (cell,RGB(R, G, B));

			}
		}
	}

	if (m_bExportFDStreamVariables){
		ofstream NGSIM_output_file("Fundamental_Diagram_Flow_Variables.csv", fstream::out);
		NGSIM_output_file << m_CorridorSensorData.out_FD_variables.str();
		NGSIM_output_file.close();
	}
}
void CTimeSpaceView::RefreshWindow()
{
	OnUpdate(0, 0, 0);

}


BOOL CTimeSpaceView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int CurrentTime = int((pt.x - 60.0f)/m_UnitTime + m_TimeLeft);

	int LeftShiftSize = max(5,int(0.2*(CurrentTime-m_TimeLeft))/10*10);
	int RightShiftSize = max(5,int(0.2*(m_TimeRight-CurrentTime))/10*10);

	if(zDelta <	 0)
	{
		m_TimeLeft-=LeftShiftSize;
		m_TimeRight+=RightShiftSize;
	}
	else
	{
		m_TimeLeft+=LeftShiftSize;
		m_TimeRight-=RightShiftSize;
	}

	if(m_TimeLeft<0)  m_TimeLeft = 0;
	if(m_TimeRight>m_NumberOfTimeSteps) m_TimeRight = m_NumberOfTimeSteps;

	Invalidate();
	return true;

}

void CTimeSpaceView::InitializeTimeRange()
{
	int min_timestamp = m_NumberOfTimeSteps;
	int max_timestamp = 0;

	CTLiteDoc* pDoc = GetTLDocument();

	if(pDoc->m_TrainVector.size() ==0)
	{
		m_TimeLeft = 0;
		m_TimeRight = m_NumberOfTimeSteps;

		return;
	}
	// narrow the range for display

	for(unsigned int v = 0; v<pDoc->m_TrainVector.size(); v++)
	{

		DTA_Train* pTrain = pDoc->m_TrainVector[v];

		for(int n = 0; n< pTrain->m_NodeSize; n++)
		{

			if(min_timestamp > pTrain->m_aryTN[n].NodeTimestamp)
				min_timestamp = pTrain->m_aryTN[n].NodeTimestamp;

			if(max_timestamp < pTrain->m_aryTN[n].NodeTimestamp)
				max_timestamp = pTrain->m_aryTN[n].NodeTimestamp;

		}
	}

	m_TimeLeft = min_timestamp;
	m_TimeRight = max_timestamp;

}

void CTimeSpaceView::OnRButtonDown(UINT nFlags, CPoint point)
{

	InitializeTimeRange();
	m_TempLinkStartPoint  = m_TempLinkEndPoint ;


	Invalidate();
	CView::OnRButtonDown(nFlags, point);
}

void CTimeSpaceView::OnTimetableResourceprice()
{
	m_bShowResourcePrice = !m_bShowResourcePrice;
	Invalidate();

}

void CTimeSpaceView::OnUpdateTimetableResourceprice(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowResourcePrice);
}

void CTimeSpaceView::OnTimetableNodecostlabel()
{
	m_bShowNodeCostLabel = ! m_bShowNodeCostLabel;
	Invalidate();


}

void CTimeSpaceView::OnUpdateTimetableNodecostlabel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowNodeCostLabel);
}

void CTimeSpaceView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_last_cpoint = point;

	//	Invalidate();  // reflesh
		g_SetCursor(_cursor_movement_network);

	m_TempLinkStartPoint = point;
	m_TempLinkEndPoint = point;
	m_bMouseDownFlag = true;

	CView::OnLButtonDown(nFlags, point);
}

void CTimeSpaceView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bMouseDownFlag)
	{
		// if it is the first moving operation, erase the previous temporal link
		if(m_TempLinkStartPoint!=m_TempLinkEndPoint)
			DrawTemporalLink(m_TempLinkStartPoint,m_TempLinkEndPoint);
		CView::OnMouseMove(nFlags, point);
		// update m_TempLinkEndPoint from the current mouse point
		m_TempLinkEndPoint = point;

		// draw a new temporal link
		DrawTemporalLink(m_TempLinkStartPoint,m_TempLinkEndPoint);
	}

}

void CTimeSpaceView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// obtain local y and timestamps for selected link
	m_SelectedStartTime = GetTimestampFromPointX(m_TempLinkStartPoint.x, m_PlotRectNGSIM);
	m_SelectedEndTime = GetTimestampFromPointX(m_TempLinkEndPoint.x, m_PlotRectNGSIM);

	m_SelectedStartLocalY = GetLocalYFromPointY(m_TempLinkStartPoint.y, m_PlotRectNGSIM);
	m_SelectedEndLocalY = GetLocalYFromPointY(m_TempLinkEndPoint.y, m_PlotRectNGSIM);

	CountVehicles(m_SelectedStartTime,m_SelectedEndTime, m_SelectedStartLocalY,m_SelectedEndLocalY);

	m_bMouseDownFlag = false;
	Invalidate();

	CView::OnLButtonUp(nFlags, point);
}

void CTimeSpaceView::OnClose()
{
	CView::OnClose();
}



void CTimeSpaceView::OnToolsLoadvehicletrajactoryfile()
{

	static char BASED_CODE szFilter[] = "NGSIM Vehicle Trajectory File (*.txt)|*.txt||";

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	std::vector<VehicleSnapshotData> OriginalDataVector;

	CTime ExeStartTime = CTime::GetCurrentTime();

	if(dlg.DoModal() == IDOK)
	{
		LoadVehicleTrajectoryFile(dlg.GetPathName());
	}

	CTime ExeEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = ExeEndTime  - ExeStartTime;

	CString str_running_time;

	str_running_time.Format ("Load %d vehicles from the vehicle trajectory file.\nLoading Time: %d hour %d min %d sec. \n",
		m_NumberOfVehicles, ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

	AfxMessageBox(str_running_time, MB_ICONINFORMATION);
}

void CTimeSpaceView::OnToolsLoadvehicletrajactoryfileFromVissim(bool unit)
{
	
	static char BASED_CODE szFilter[] = "VISSIM Vehicle Trajectory File (*.fzp)|*.fzp||";

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	std::vector<VehicleSnapshotData> OriginalDataVector;

	CTime ExeStartTime = CTime::GetCurrentTime();

	if (dlg.DoModal() == IDOK)
	{
		LoadVehicleTrajectoryFileFromVissim(dlg.GetPathName(), unit);
	}

	CTime ExeEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = ExeEndTime - ExeStartTime;

	CString str_running_time;

	str_running_time.Format("Load %d vehicles from the vehicle trajectory file.\nLoading Time: %d hour %d min %d sec. \n",
		m_NumberOfVehicles, ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

	AfxMessageBox(str_running_time, MB_ICONINFORMATION);
}

#include <direct.h>
#define GetCurrentDir _getcwd

bool CompareByEntryTime(const VehicleSnapshotData &a, const VehicleSnapshotData &b) {
	return a.Frame_ID < b.Frame_ID;
};

bool CompareByPairEntryTime(const pair<int, int> &a, const pair<int, int> &b) {
	return a.second < b.second;	// a.entry_time < b.entry_time;
};

void CTimeSpaceView::LoadVehicleTrajectoryFile(CString file_name)
{
	ReadNGSIMScenarioFile();

	int Vehicle_ID; //1 

	int Frame_ID;//2

	int Total_Frames; //3

	long long Global_Time ; //4

	float Local_X ; //5

	float Local_Y  ; //6

	float Global_X  ; //7

	float Global_Y  ; //8

	float Vehicle_Length ; //9

	float Vehicle_Width ; //10

	int	Vehicle_Class ; //11

	float	Vehicle_Velocity ; //12

	float	Vehicle_Acceleration  ; //13



	int 	Lane_Identification ; //14

	int	Preceding_Vehicle ; //15

	int	Following_Vehicle  ; //16



	float	Spacing ; //17

	float	Headway  ; //18


	float CalculatedSpeed;
	//SITSLAB
	int old_Lane_Identification;
	int num_passed_lanes;
	bool b_exclude_current_vehicle = false;
	//SITSLAB: exclude vehicles that have changed lanes
	m_bExcludeLaneChangingVehicles = true;
	vector<VehicleSnapshotData> current_vehicle_element_vector;
	int prev_Frame_ID;

	int prev_Total_Frames;



	std::vector<VehicleSnapshotData> OriginalDataVector;

	vector<VehicleSnapshotData> Copy_OriginalDataVector;


	CTime ExeStartTime = CTime::GetCurrentTime();

	int line = 0;



	CWaitCursor wc;

	char fname[_MAX_PATH];

	wsprintf(fname,"%s", file_name);

	FILE* st;



	m_LocalYUpperBound = 0;

	m_LocalYLowerBound = 9999999;

	//sdzhao
	map<int, float> out_laneno_max_acc;
	map<int, int> out_laneno_first_veh_no;
	map<int, map<int, ostringstream>> out_laneno_vehno_trajectory;
	map<int, map<int, ostringstream>> out_laneno_vehno_entry_time;
	map<int, map<int, ostringstream>> out_laneno_vehno_trajectory_ngsim;
	map<int, map<int, int>> out_laneno_velocity_frequency;

	map<int, double> min_frame_id;
	map<int, float> out_laneno_min_acc;
	map<int, float> out_laneno_max_velocity;
	map<int, float> out_laneno_avg_headway;
	map<int, float> out_laneno_avg_spacing;
	map<int, float> out_laneno_max_Local_Y;
	map<int,ostringstream> out_vehicle_entry_time;
	map<int, vector<pair<int, int>>> laneno_vehicleID_sorted_vector;
	//out_vehicle_entry_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m)" << endl;
	map<int, int> excluded_vehicleID_vector;
	//fopen_s(&st,fname,"r");
	ifstream input_file(fname);
	string str_line;
	float prev_local_y = -1;
	if (input_file.is_open())	//if(st!=NULL)
	{

		m_NumberOfVehicles = 0;
		m_NumberOfTimeSteps = 0;
		int Prev_Vehicle_ID = -1;
		int current_line = 0;
		vector<string> tokens;

		while (input_file.good()) //while (!feof(st))
		{
			getline(input_file, str_line);
			if (str_line != "")
			{
				current_line++;
				string_split(tokens, str_line, 18);

				VehicleCFData vcd;



				Vehicle_ID = stoi(tokens[0]); //g_read_integer(st);  //1



				if (Vehicle_ID < 0)

					break;



				Frame_ID = stoi(tokens[1]);// g_read_integer(st); //2



				if (Frame_ID > m_NumberOfTimeSteps)

					m_NumberOfTimeSteps = Frame_ID;



				Total_Frames = stoi(tokens[2]); // g_read_integer(st); //3
				//long long glt = 1118935680200;
				//Global_Time = glt;
				Global_Time = stoll(tokens[3]);	// g_read_integer(st); //4



				Local_X = stof(tokens[4]); // g_read_float(st); //5

				Local_Y = stof(tokens[5]); // g_read_float(st); //6



				if (Local_Y > m_LocalYUpperBound)

					m_LocalYUpperBound = Local_Y;



				if (Local_Y < m_LocalYLowerBound)

					m_LocalYLowerBound = Local_Y;


				CalculatedSpeed = 0;

				if (Vehicle_ID == Prev_Vehicle_ID)
				{
					CalculatedSpeed = (Local_Y - prev_local_y)*10.0f * 0.681818182;  // convert from feet per second to mph
				}

				prev_local_y = Local_Y;


				Global_X = stof(tokens[6]);		// g_read_float(st); //7

				Global_Y = stof(tokens[7]);		//g_read_float(st); //8

				Vehicle_Length = stof(tokens[8]); // g_read_float(st); //9

				Vehicle_Width = stof(tokens[9]); //g_read_float(st); //10



				Vehicle_Class = stoi(tokens[10]);  //g_read_integer(st); //11



				Vehicle_Velocity = stof(tokens[11]); //g_read_float(st); //12

				Vehicle_Acceleration = stof(tokens[12]); // g_read_float(st); //13



				Lane_Identification = stoi(tokens[13]); //g_read_integer(st); //14

				Preceding_Vehicle = stoi(tokens[14]); // g_read_integer(st); //15

				Following_Vehicle = stoi(tokens[15]); //g_read_integer(st); //16



				Spacing = stof(tokens[16]); //g_read_float(st); //17

				//Headway = stof(tokens[17]);  // g_read_float(st); //18

				if (current_line == 1)
				{
					Prev_Vehicle_ID = Vehicle_ID;
					old_Lane_Identification = Lane_Identification;
					prev_Frame_ID = Frame_ID;
					prev_Total_Frames = Total_Frames;
				}

				if (m_bExcludeLaneChangingVehicles)
				{
					if (Vehicle_ID == Prev_Vehicle_ID && Lane_Identification != old_Lane_Identification)
					{
						b_exclude_current_vehicle = true;
					}

				}

				old_Lane_Identification = Lane_Identification;

				if (Vehicle_ID != Prev_Vehicle_ID && b_exclude_current_vehicle == false){
					if (out_vehicle_entry_time.find(Lane_Identification) == out_vehicle_entry_time.end()){
						out_laneno_max_velocity[Lane_Identification] = 0.0;
						out_laneno_max_Local_Y[Lane_Identification] = 0.0;
						out_laneno_max_acc[Lane_Identification] = -DBL_MAX;
						out_laneno_min_acc[Lane_Identification] = DBL_MAX;
						out_laneno_first_veh_no[Lane_Identification] = -1;
						out_vehicle_entry_time[Lane_Identification] << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
						
						min_frame_id[Lane_Identification] = DBL_MAX;
						//velocity frequency
						out_laneno_velocity_frequency[Lane_Identification][0] = 0;
						out_laneno_velocity_frequency[Lane_Identification][1] = 0;
						out_laneno_velocity_frequency[Lane_Identification][2] = 0;
						out_laneno_velocity_frequency[Lane_Identification][3] = 0;
						out_laneno_velocity_frequency[Lane_Identification][4] = 0;
						out_laneno_velocity_frequency[Lane_Identification][5] = 0;
						out_laneno_velocity_frequency[Lane_Identification][6] = 0;
						out_laneno_velocity_frequency[Lane_Identification][7] = 0;
					}
					out_vehicle_entry_time[Lane_Identification] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
					laneno_vehicleID_sorted_vector[Lane_Identification].push_back(make_pair(Vehicle_ID, Frame_ID));
					out_laneno_first_veh_no[Lane_Identification] = (Frame_ID < min_frame_id[Lane_Identification]) ? Vehicle_ID : out_laneno_first_veh_no[Lane_Identification];
					min_frame_id[Lane_Identification] = (Frame_ID < min_frame_id[Lane_Identification]) ? Frame_ID : min_frame_id[Lane_Identification];
				}
				out_laneno_max_velocity[Lane_Identification] = max(out_laneno_max_velocity[Lane_Identification], Vehicle_Velocity*0.3048);
				out_laneno_max_Local_Y[Lane_Identification] = max(out_laneno_max_Local_Y[Lane_Identification], Local_Y*0.3048);
				out_laneno_min_acc[Lane_Identification] = min(out_laneno_min_acc[Lane_Identification], Vehicle_Acceleration*0.3048);
				out_laneno_max_acc[Lane_Identification] = max(out_laneno_max_acc[Lane_Identification], Vehicle_Acceleration*0.3048);
				
				int velocity_index = (int) ((Vehicle_Velocity * 0.681818182) / 10);
				out_laneno_velocity_frequency[Lane_Identification][velocity_index]++;

				//output vehicle trajectories
				out_laneno_vehno_trajectory[Lane_Identification][Vehicle_ID] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
				out_laneno_vehno_trajectory_ngsim[Lane_Identification][Vehicle_ID] << Vehicle_ID << "," << Frame_ID << ","<< Total_Frames << "," << Global_Time<<"," <<Local_X <<","<< Local_Y << ","
					<< Global_X << "," << Global_Y << "," << Vehicle_Length << "," << Vehicle_Width << "," << Vehicle_Class << "," << Vehicle_Velocity << "," << Vehicle_Acceleration << ","
					<< Lane_Identification << "," << Preceding_Vehicle << "," << Following_Vehicle << "," << Spacing << endl;
					//<<",0,0,0,0,0,0,0,0,0,0,0,0"<<endl;

				if (Vehicle_ID != Prev_Vehicle_ID){
					out_laneno_vehno_entry_time[Lane_Identification][Vehicle_ID] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;

				}
				//
				//
				//out_laneno_avg_headway[Lane_Identification] += Headway;
				//out_laneno_avg_spacing[Lane_Identification] += Spacing;
				if (Vehicle_ID != Prev_Vehicle_ID && b_exclude_current_vehicle == false)

				{

					//sdzhao out_vehicle_entry_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m)" << endl;
					// 				if (out_vehicle_entry_time.find(Lane_Identification) == out_vehicle_entry_time.end()){
					// 					out_laneno_max_velocity[Lane_Identification] = 0.0;
					// 					out_laneno_max_Local_Y[Lane_Identification] = 0.0;
					// 					out_laneno_max_acc[Lane_Identification] = -DBL_MAX;
					// 					out_laneno_min_acc[Lane_Identification] = DBL_MAX;
					// 					out_vehicle_entry_time[Lane_Identification] << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
					// 				}
					// 				out_vehicle_entry_time[Lane_Identification] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
					// 				out_laneno_max_velocity[Lane_Identification] = max(out_laneno_max_velocity[Lane_Identification], Vehicle_Velocity*0.3048);
					// 				out_laneno_max_Local_Y[Lane_Identification] = max(out_laneno_max_Local_Y[Lane_Identification], Local_Y*0.3048);
					// 				out_laneno_min_acc[Lane_Identification] = min(out_laneno_min_acc[Lane_Identification], Vehicle_Acceleration*0.3048);
					// 				out_laneno_max_acc[Lane_Identification] = max(out_laneno_max_acc[Lane_Identification], Vehicle_Acceleration*0.3048);

				
					m_NumberOfVehicles++;

					m_VehicleIDtoNOMap[Vehicle_ID] = m_NumberOfVehicles;  // ID to sequential No.



					//Prev_Vehicle_ID = Vehicle_ID;
					//vcd.VehicleID = Vehicle_ID;
					vcd.VehicleID = Prev_Vehicle_ID;
					Prev_Vehicle_ID = Vehicle_ID;

					// 				vcd.StartTime = Frame_ID;
					// 
					// 				vcd.EndTime = Frame_ID + Total_Frames;
					vcd.StartTime = prev_Frame_ID - prev_Total_Frames + 1;
					vcd.EndTime = prev_Frame_ID + 1;

					if (Frame_ID + 1 > m_NumberOfTimeSteps)//if(Frame_ID + Total_Frames > m_NumberOfTimeSteps)
					{
						m_NumberOfTimeSteps = Frame_ID + Total_Frames;
					}

					vcd.VehicleType = Vehicle_Class;



					m_VehicleDataList.push_back(vcd);


					for (int i = 0; i < current_vehicle_element_vector.size(); i++)
					{
						OriginalDataVector.push_back(current_vehicle_element_vector[i]);

					}
					current_vehicle_element_vector.clear();
				}


				if (Vehicle_ID != Prev_Vehicle_ID)
				{

					Prev_Vehicle_ID = Vehicle_ID;
					b_exclude_current_vehicle = false;

					current_vehicle_element_vector.clear();
				}

				//vector<string> tmp_element;
				//vector<vector<string>> element_vector;



				VehicleSnapshotData element;
				element.NGSIM_Vehicle_ID = Vehicle_ID;

				element.VehicleID = m_NumberOfVehicles;

				element.Frame_ID = Frame_ID;

				element.LocalY = Local_Y;  //m_NumberOfVehicles-1 make sure the index starting from 0

				element.LaneID = Lane_Identification;

				element.Speed_mph = Vehicle_Velocity * 0.681818182;  // convert from feet per second to mph
				element.CalculatedSpeed_mph = CalculatedSpeed;

				element.PrecedingVehicleID = Preceding_Vehicle;

				element.FollowingVehicleID = Following_Vehicle;
				element.Acceleration = Vehicle_Acceleration*0.3048; //mps
				element.Spacing_meters = Spacing * 0.3048;
				m_SelectLaneID = Lane_Identification;  // set Lane ID as a given number

				//OriginalDataVector.push_back(element);
				Copy_OriginalDataVector.push_back(element);

				current_vehicle_element_vector.push_back(element);
				prev_Frame_ID = Frame_ID;
				prev_Total_Frames = Total_Frames;


				// 			VehicleSnapshotData element;
				// 
				// 			element.VehicleID = m_NumberOfVehicles-1;
				// 
				// 			element.Frame_ID  = Frame_ID;
				// 
				// 			element.LocalY  =  Local_Y;  //m_NumberOfVehicles-1 make sure the index starting from 0
				// 
				// 			element.LaneID  =  Lane_Identification; 
				// 
				// 			element.Speed_mph  = Vehicle_Velocity * 0.681818182;  // convert from feet per second to mph
				// 			element.CalculatedSpeed_mph = CalculatedSpeed;
				// 
				// 			element.PrecedingVehicleID = Preceding_Vehicle;
				// 
				// 			element.FollowingVehicleID = Following_Vehicle;
				// 
				// 
				// 
				// 			m_SelectLaneID = Lane_Identification;  // set Lane ID as a given number
				// 
				// 
				// 
				// 			OriginalDataVector.push_back(element);



				/*

				//		ofstream  NGSIM_LogFile;

				//		NGSIM_LogFile.open ("C:\\temp\\NGSIM_Trajectry_Sample.txt", ios::out);

				//		NGSIM_LogFile.setf(ios::fixed);



				if (NGSIM_LogFile.is_open() && Lane_Identification == 4 && Frame_ID < 3000)

				{

				NGSIM_LogFile << Vehicle_ID << "," << Frame_ID << "," << Total_Frames << "," <<  Global_Time << ",";

				NGSIM_LogFile << Local_X <<  "," << Local_Y <<  "," << Global_X <<  "," << Global_Y <<  "," << Vehicle_Length <<  "," << Vehicle_Width << ",";

				NGSIM_LogFile	<< Vehicle_Class <<  "," << Vehicle_Velocity <<  "," << Vehicle_Acceleration <<  "," << Lane_Identification <<  "," <<	Preceding_Vehicle <<  "," <<  Following_Vehicle <<  "," << Spacing <<  "," << Headway << endl;



				}

				NGSIM_LogFile.close();



				*/

				line++;

			}
			//out_laneno_avg_headway[Lane_Identification] = out_laneno_avg_headway[Lane_Identification] / current_line;
			//out_laneno_avg_spacing[Lane_Identification] = out_laneno_avg_spacing[Lane_Identification] / current_line;

			//fclose(st);
		}
	}
	input_file.close();
	//sdzhao: export vehicle entry time to file
	//out_vehicle_entry_time
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		cout << "error current dir" << endl;
	string summary_folder(cCurrentPath);
	
	summary_folder += "/summary";
	if (CreateDirectory(summary_folder.c_str(), NULL))
		cout << "yes" << endl;
	else
	{
		cout << "fail" << endl;
	}

	for (auto &iter : out_vehicle_entry_time)
	{
//  		ofstream f_veh_entry_time("NGSIM_vehicle_entry_time_" + to_string(iter.first) + ".csv", fstream::out);
//  		f_veh_entry_time << iter.second.str();
// 		iter.second.str(""); //clear contents
//  		f_veh_entry_time.close();

		ofstream f_summary(summary_folder+"/NGSIM_summary_" + to_string(iter.first) + ".csv", fstream::out);
		f_summary << "max velocity(mps)," << out_laneno_max_velocity[iter.first] << endl;			//1
		f_summary << "max velocity(kph)," << out_laneno_max_velocity[iter.first] * 3.6 << endl;		//2
		f_summary << "max position(meters)," << out_laneno_max_Local_Y[iter.first] << endl;			//3
		f_summary << "max acceleration(mps2)," << out_laneno_max_acc[iter.first] << endl;			//4
		f_summary << "min acceleration(mps2)," << out_laneno_min_acc[iter.first] << endl;			//5
		f_summary << "first_vehicle_ID," << out_laneno_first_veh_no[iter.first] << endl;			//6
		f_summary << "desired interval/Tao (seconds),"<< endl;										//7
		f_summary << "desired min spacing/d (meters)," << endl;										//8
		f_summary << "Count of Velocity(0~10 mph)," << out_laneno_velocity_frequency[Lane_Identification][0] << endl;
		f_summary << "Count of Velocity(10~20 mph)," << out_laneno_velocity_frequency[Lane_Identification][1] << endl;
		f_summary << "Count of Velocity(20~30 mph)," << out_laneno_velocity_frequency[Lane_Identification][2] << endl;
		f_summary << "Count of Velocity(30~40 mph)," << out_laneno_velocity_frequency[Lane_Identification][3] << endl;
		f_summary << "Count of Velocity(40~50 mph)," << out_laneno_velocity_frequency[Lane_Identification][4] << endl;
		f_summary << "Count of Velocity(50~60 mph)," << out_laneno_velocity_frequency[Lane_Identification][5] << endl;
		f_summary << "Count of Velocity(60~70 mph)," << out_laneno_velocity_frequency[Lane_Identification][6] << endl;
		f_summary << "Count of Velocity(70+ mph)," << out_laneno_velocity_frequency[Lane_Identification][7] << endl;
		//f_summary << "average headway (seconds)," << out_laneno_avg_headway[iter.first] << endl;
		//f_summary << "average spacing (meters)," << out_laneno_avg_spacing[iter.first] * 0.3048 << endl;
		f_summary.close();
	}

	//ofstream ofile("log.txt", ios::out);
	m_VehicleSnapshotAry= AllocateDynamicArray<VehicleSnapshotData>(m_NumberOfVehicles,m_NumberOfTimeSteps+1);
	//ofile << m_NumberOfVehicles << "," << m_NumberOfTimeSteps + 1 << endl;

	map<int, vector<VehicleSnapshotData>> lane_no_first_25_vehs;

	for (unsigned i = 0; i < Copy_OriginalDataVector.size(); i++) //Copy_OriginalDataVector
	{
		VehicleSnapshotData element = Copy_OriginalDataVector[i];
		//
		int id = element.NGSIM_Vehicle_ID;
		auto pred = [id](const VehicleSnapshotData & item) {
			return item.NGSIM_Vehicle_ID == id;
		};
		if (std::find_if(std::begin(lane_no_first_25_vehs[element.LaneID]), std::end(lane_no_first_25_vehs[element.LaneID]), pred) == std::end(lane_no_first_25_vehs[element.LaneID])){
			lane_no_first_25_vehs[element.LaneID].push_back(element);
		}
		//
		////sdzhao export first vehicle trajectory
		if (out_laneno_first_veh_no[element.LaneID] == element.NGSIM_Vehicle_ID){
			out_vehicle_entry_time[element.LaneID]
				<< element.NGSIM_Vehicle_ID << ","
				<< element.Frame_ID << ","
				<< element.Speed_mph * 0.44704 << ","	//mps
				<< element.Acceleration << ","			//mps2
				<< element.LocalY *0.3048 << endl;      //meters
		}
	}
	if(lane_no_first_25_vehs.size()>=25)
	{
		for (auto &iter : lane_no_first_25_vehs){
			vector<VehicleSnapshotData>& vec = iter.second;
			sort(vec.begin(), vec.end(), CompareByEntryTime);
			vec.erase(vec.begin() + 25, vec.end());

			ofstream f_traj_time("NGSIM_1_25_vehicle_entrytime_" + to_string(iter.first) + ".csv", fstream::out);
			for (int i = 0; i < 24; i++){
				f_traj_time << vec[i].NGSIM_Vehicle_ID << ","
					<< vec[i].Frame_ID << ","
					<< vec[i].Speed_mph * 0.44704 << ","	//mps
					<< vec[i].Acceleration << ","			//mps2
					<< vec[i].LocalY *0.3048 << endl;      //meters
			}
			f_traj_time.close();
		}
	}
	map<int, vector<VehicleSnapshotData>> lane_no_first_25_vehs_traj;
	for (unsigned i = 0; i < Copy_OriginalDataVector.size(); i++) //Copy_OriginalDataVector
	{
		VehicleSnapshotData element = Copy_OriginalDataVector[i];
		vector<VehicleSnapshotData>& vec = lane_no_first_25_vehs[element.LaneID];
		//if (vec.find(element.))
		int id = element.NGSIM_Vehicle_ID;
		auto pred = [id](const VehicleSnapshotData & item) {
			return item.NGSIM_Vehicle_ID == id;
		};
		if (std::find_if(std::begin(vec), std::end(vec), pred) != std::end(vec)){

			lane_no_first_25_vehs_traj[element.LaneID].push_back(element);
		}
	}
	if(lane_no_first_25_vehs_traj.size()>=25)
	{ 
		for (auto &iter : lane_no_first_25_vehs_traj){
			ofstream f_traj_time("NGSIM_1_25_vehicle_trajectory_" + to_string(iter.first) + ".csv", fstream::out);
			vector<VehicleSnapshotData>& vec = iter.second;
			for (int i = 0; i < vec.size(); i++){
				f_traj_time << vec[i].NGSIM_Vehicle_ID << ","
					<< vec[i].Frame_ID << ","
					<< vec[i].Speed_mph * 0.44704 << ","	//mps
					<< vec[i].Acceleration << ","			//mps2
					<< vec[i].LocalY *0.3048 <<","
					<< vec[i].Spacing_meters << endl;      //meters
			}
			f_traj_time.close();
		}
	}


	//
// 	for (auto &iter : out_vehicle_entry_time)
// 	{
// 		ofstream f_veh_traj_time("NGSIM_first_vehicle_trajectory_" + to_string(iter.first) + ".csv", fstream::out);
// 		f_veh_traj_time << iter.second.str();
// 		iter.second.str(""); //clear contents
// 		f_veh_traj_time.close();
// 	}
	//

	for(unsigned i  = 0; i < OriginalDataVector.size(); i++)

	{

		VehicleSnapshotData element = OriginalDataVector[i];

		if (element.VehicleID == 1505 && element.Frame_ID == 5664)
			int a = 0;

		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].LaneID =  element.LaneID;

		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].LocalY =  element.LocalY;

		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].Speed_mph =  element.Speed_mph;
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].CalculatedSpeed_mph  =  element.CalculatedSpeed_mph;

		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].PrecedingVehicleID =  element.PrecedingVehicleID;

		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].FollowingVehicleID  =  element.FollowingVehicleID;
		
	}
	

	//vector sort
	for (auto &lane : laneno_vehicleID_sorted_vector)
	{
		vector<pair<int, int>> &vehicleID_EntryTime_Pair_Vector = lane.second;
		sort(vehicleID_EntryTime_Pair_Vector.begin(), vehicleID_EntryTime_Pair_Vector.end(), CompareByPairEntryTime);
	}
	for (auto &lane : laneno_vehicleID_sorted_vector)//for (auto &lane : out_laneno_vehno_entry_time)
	{
		int num_vehs_per_lane = lane.second.size();
		int veh_index = 0;
		ofstream f_veh_entry_time("NGSIM_vehicle_entry_time_" + to_string(lane.first) + ".csv", fstream::out);
		f_veh_entry_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
		ofstream f_veh_traj_time("NGSIM_shockwave_vehicle_trajectory_" + to_string(lane.first) + ".csv", fstream::out);
		f_veh_traj_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
		ofstream f_veh_traj_time_ngsim("NGSIM_shockwave_vehicle_trajectory_" + to_string(lane.first) + ".txt", fstream::out);

		//--------------
		int platoon_size = 0;
		int num_front_vehs = number_of_total_vehicles_output - 1 - platoon_size;

		for (auto &veh : lane.second)
		{
			//exclude 
			if (excluded_vehicleID_vector.find(veh.first) != excluded_vehicleID_vector.end())
				continue;

			// 			if (veh_index >= num_vehs_per_lane - 90 - num_front_vehs && veh_index <= num_vehs_per_lane - 90)//if (veh_index >= num_vehs_per_lane - 90 && veh_index <= num_vehs_per_lane - 90+5)
			// 			{
			// 				f_veh_traj_time << veh.second.str();
			// 			}
			//26
			if (veh_index >= num_vehs_per_lane - num_front_vehs - num_deduct_vehicles && veh_index <= num_vehs_per_lane + platoon_size - num_deduct_vehicles)
			{
				f_veh_traj_time << out_laneno_vehno_trajectory[lane.first][veh.first].str();
				f_veh_entry_time << out_laneno_vehno_entry_time[lane.first][veh.first].str();
				f_veh_traj_time_ngsim << out_laneno_vehno_trajectory_ngsim[lane.first][veh.first].str();
			}
			else if (veh_index > num_vehs_per_lane - 26 && veh_index <= num_vehs_per_lane - 26 + 5)
			{
				//f_veh_entry_time << out_laneno_vehno_entry_time[lane.first][veh.first].str();
			}
			veh_index++;
		}
		f_veh_entry_time.close();
		f_veh_traj_time.close();
		f_veh_traj_time_ngsim.close();
	}

	m_TimeLeft = 0;
	m_TimeRight = m_NumberOfTimeSteps;

	//read NGSIM scenario to get time interval and sensor spacing

	//ConstructCellBasedDensityProfile();
	float SensorSpacing = m_DataCollectionSensorSpacing_in_feet; // 200;
	float boundary_offset = 50;
	int data_collection_time_interval_in_sec = m_DataCollectionTimeInternval_in_sec;	//= 30;
	
	ConstructCellBasedDensityProfile(SensorSpacing, boundary_offset, data_collection_time_interval_in_sec);
	
}

void CTimeSpaceView::LoadVehicleTrajectoryFileFromVissim(CString file_name, bool metric)
{
	
	int Vehicle_ID; //1 p
	int Frame_ID;//2
	int Total_Frames; //3
	long long Global_Time; //4
	float Local_X; //5
	float Local_Y; //6
	float Global_X; //7
	float Global_Y; //8
	float Vehicle_Length; //9
	float Vehicle_Width; //10
	int	Vehicle_Class; //11
	float	Vehicle_Velocity; //12
	float	Vehicle_Acceleration; //13
	int 	Lane_Identification; //14
	int	Preceding_Vehicle; //15
	int	Following_Vehicle; //16
	float	Spacing; //17
	float	Headway; //18
	float CalculatedSpeed;
	//SITSLAB
	int old_Lane_Identification;
	int num_passed_lanes;
	bool b_exclude_current_vehicle = false;
	//SITSLAB: exclude vehicles that have changed lanes
	m_bExcludeLaneChangingVehicles = true;
	vector<VehicleSnapshotData> current_vehicle_element_vector;
	int prev_Frame_ID;
	int prev_Total_Frames;
	std::vector<VehicleSnapshotData> OriginalDataVector;
	vector<VehicleSnapshotData> Copy_OriginalDataVector;
	CTime ExeStartTime = CTime::GetCurrentTime();
	int line = 0;
	CWaitCursor wc;
	char fname[_MAX_PATH];
	wsprintf(fname, "%s", file_name);
	FILE* st;
	m_LocalYUpperBound = 0;
	m_LocalYLowerBound = 9999999;
	//sdzhao
	map<int, float> out_laneno_max_acc;
	map<int, int> out_laneno_first_veh_no;
	map<int, map<int, int>> out_laneno_velocity_frequency;
	map<int, double> min_frame_id;
	map<int, float> out_laneno_min_acc;
	map<int, float> out_laneno_max_velocity;
	map<int, float> out_laneno_avg_headway;
	map<int, float> out_laneno_avg_spacing;
	map<int, float> out_laneno_max_Local_Y;
	map<int, ostringstream> out_vehicle_entry_time;
	map<int, int> excluded_vehicleID_vector;
	map<int, VehicleSnapshotData> vehicle_no_latest_information;
	map<int, VehicleCFData> vehicle_cf_data_map;
	map<int, vector<VehicleSnapshotData>> current_vehicle_element_map;
	map<int, int> ngsim_to_index_map;

	ifstream input_file(fname);
	string str_line;
	float prev_local_y = -1;
	if (input_file.is_open())	//if(st!=NULL)
	{
		m_NumberOfVehicles = 0;
		m_NumberOfTimeSteps = 0;
		int Prev_Vehicle_ID = -1;
		int current_line = 0;
		vector<string> tokens;
		int start_line = 19;
		while (input_file.good()) //while (!feof(st))
		{
			getline(input_file, str_line);
			if (str_line != "")
			{
				current_line++;
				if (current_line > start_line-1)
				{
					string_split(tokens, str_line, ";");
					VehicleCFData vcd;
					
					Vehicle_ID = stoi(tokens[1]); 
					if (Vehicle_ID < 0)
						break;
					Frame_ID = round(stof(tokens[0]) / 0.1);
					if (Frame_ID > m_NumberOfTimeSteps)
					{ 
						m_NumberOfTimeSteps = Frame_ID;
					}
					Local_X = stof(tokens[5]); 
					

					if (!metric)
					{
						Local_Y = stof(tokens[4]);
						if (current_vehicle_element_map.find(Vehicle_ID) != current_vehicle_element_map.end())
						{
							if (Local_Y - current_vehicle_element_map[Vehicle_ID].back().LocalY < 0)
								Local_Y = Local_Y + current_vehicle_element_map[Vehicle_ID].back().LocalY;
						}
					}
					else
					{
						Local_Y = stof(tokens[4])*3.28084;
						if (current_vehicle_element_map.find(Vehicle_ID) != current_vehicle_element_map.end())
						{
							if (Local_Y - current_vehicle_element_map[Vehicle_ID].back().LocalY < 0)
								Local_Y = Local_Y + current_vehicle_element_map[Vehicle_ID].back().LocalY;
						}
					}
					

					if (Local_Y > m_LocalYUpperBound)
					{ 
						m_LocalYUpperBound = Local_Y ;
					}
					if (Local_Y < m_LocalYLowerBound)
					{
						m_LocalYLowerBound = Local_Y ;
					}
					CalculatedSpeed = 0;

					if (vehicle_no_latest_information.find(Vehicle_ID) == vehicle_no_latest_information.end())
					{
						Vehicle_Velocity = 0.0;
						Vehicle_Acceleration = 0.0;
					}
					else
					{
						VehicleSnapshotData vehicle_data = vehicle_no_latest_information[Vehicle_ID];
						Vehicle_Velocity = (Local_Y - vehicle_data.LocalY) / (Frame_ID - vehicle_data.Frame_ID) * 10.0 * 0.681818182;
						Vehicle_Acceleration = (Vehicle_Velocity - vehicle_data.Speed_mph) / (Frame_ID - vehicle_data.Frame_ID) * 10.0;
					}
					//if (Vehicle_ID == Prev_Vehicle_ID)
					//{
					//	CalculatedSpeed = (Local_Y - prev_local_y)*10.0f * 0.681818182;  // convert from feet per second to mph
					//}
					prev_local_y = Local_Y;
					Global_X = stof(tokens[5]);	
					if (!metric)
						Global_Y = stof(tokens[4]);
					else
						Global_Y = stof(tokens[4])*3280.84;
					//Global_Y = stof(tokens[4]);		
					Lane_Identification = stoi(tokens[3]); //g_read_integer(st); //14
				
					Global_Time = 0;	// g_read_integer(st); //4
					Vehicle_Length = 0; // g_read_float(st); //9
					Vehicle_Width = 0; //g_read_float(st); //10
					Vehicle_Class = 0;  //g_read_integer(st); //11
					Vehicle_Velocity = 0; //g_read_float(st); //12
					Preceding_Vehicle = 0; // g_read_integer(st); //15
					Following_Vehicle = 0; //g_read_integer(st); //16
					Spacing = 0; //g_read_float(st); //17

					if (current_line == start_line)
					{
						Vehicle_Velocity = 0;
						Prev_Vehicle_ID = Vehicle_ID;
						old_Lane_Identification = Lane_Identification;
						prev_Frame_ID = Frame_ID;
						Total_Frames = Frame_ID;
						prev_Total_Frames = Total_Frames;
					}
					else
					{
						Total_Frames = Total_Frames > Frame_ID ? Total_Frames : Frame_ID;
					}

					if (m_bExcludeLaneChangingVehicles)
					{
						if (Vehicle_ID == Prev_Vehicle_ID && Lane_Identification != old_Lane_Identification)
						{
							b_exclude_current_vehicle = true;
							excluded_vehicleID_vector[Vehicle_ID]++;
						}
					}
					old_Lane_Identification = Lane_Identification;
					
					//if (Vehicle_ID != Prev_Vehicle_ID)
					//{
						if (out_vehicle_entry_time.find(Lane_Identification) == out_vehicle_entry_time.end())
						{
							out_laneno_max_velocity[Lane_Identification] = 0.0;
							out_laneno_max_Local_Y[Lane_Identification] = 0.0;
							out_laneno_max_acc[Lane_Identification] = -DBL_MAX;
							out_laneno_min_acc[Lane_Identification] = DBL_MAX;
							out_laneno_first_veh_no[Lane_Identification] = -1;
							out_vehicle_entry_time[Lane_Identification] << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
							min_frame_id[Lane_Identification] = DBL_MAX;
							//velocity frequency
							out_laneno_velocity_frequency[Lane_Identification][0] = 0;
							out_laneno_velocity_frequency[Lane_Identification][1] = 0;
							out_laneno_velocity_frequency[Lane_Identification][2] = 0;
							out_laneno_velocity_frequency[Lane_Identification][3] = 0;
							out_laneno_velocity_frequency[Lane_Identification][4] = 0;
							out_laneno_velocity_frequency[Lane_Identification][5] = 0;
							out_laneno_velocity_frequency[Lane_Identification][6] = 0;
							out_laneno_velocity_frequency[Lane_Identification][7] = 0;
						}
						out_vehicle_entry_time[Lane_Identification] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
						out_laneno_first_veh_no[Lane_Identification] = (Frame_ID < min_frame_id[Lane_Identification]) ? Vehicle_ID : out_laneno_first_veh_no[Lane_Identification];
						min_frame_id[Lane_Identification] = (Frame_ID < min_frame_id[Lane_Identification]) ? Frame_ID : min_frame_id[Lane_Identification];
					//}
					
					out_laneno_max_velocity[Lane_Identification] = max(out_laneno_max_velocity[Lane_Identification], Vehicle_Velocity*0.3048);
					out_laneno_max_Local_Y[Lane_Identification] = max(out_laneno_max_Local_Y[Lane_Identification], Local_Y*0.3048);
					out_laneno_min_acc[Lane_Identification] = min(out_laneno_min_acc[Lane_Identification], Vehicle_Acceleration*0.3048);
					out_laneno_max_acc[Lane_Identification] = max(out_laneno_max_acc[Lane_Identification], Vehicle_Acceleration*0.3048);

					int velocity_index = (int)((Vehicle_Velocity * 0.681818182) / 10);
					out_laneno_velocity_frequency[Lane_Identification][velocity_index]++;

					//output vehicle trajectories
					/*out_laneno_vehno_trajectory[Lane_Identification][Vehicle_ID] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
					if (Vehicle_ID != Prev_Vehicle_ID)
					{
						out_laneno_vehno_entry_time[Lane_Identification][Vehicle_ID] << Vehicle_ID << "," << Frame_ID << "," << Vehicle_Velocity*0.3048 << "," << Local_Y*0.3048 << "," << prev_Frame_ID << endl;
					}
*/
					vehicle_cf_data_map[Vehicle_ID].StartTime = prev_Frame_ID - prev_Total_Frames + 1;
					vehicle_cf_data_map[Vehicle_ID].EndTime = prev_Frame_ID + 1;

					//
					//
					//out_laneno_avg_headway[Lane_Identification] += Headway;
					//out_laneno_avg_spacing[Lane_Identification] += Spacing;
					/*if (Vehicle_ID != Prev_Vehicle_ID && b_exclude_current_vehicle == false)
					{
						m_NumberOfVehicles++;
						m_VehicleIDtoNOMap[Vehicle_ID] = m_NumberOfVehicles;  // ID to sequential No.
						vcd.VehicleID = Prev_Vehicle_ID;
						Prev_Vehicle_ID = Vehicle_ID;
						vcd.StartTime = prev_Frame_ID - prev_Total_Frames + 1;
						vcd.EndTime = prev_Frame_ID + 1;
						if (Frame_ID + 1 > m_NumberOfTimeSteps)//if(Frame_ID + Total_Frames > m_NumberOfTimeSteps)
						{
							m_NumberOfTimeSteps = Frame_ID + Total_Frames;
						}
						vcd.VehicleType = Vehicle_Class;
						m_VehicleDataList.push_back(vcd);
						for (int i = 0; i < current_vehicle_element_vector.size(); i++)
						{
							OriginalDataVector.push_back(current_vehicle_element_vector[i]);
						}
						current_vehicle_element_vector.clear();
					}
					if (Vehicle_ID != Prev_Vehicle_ID)
					{

						Prev_Vehicle_ID = Vehicle_ID;
						b_exclude_current_vehicle = false;
						current_vehicle_element_vector.clear();
					}*/
					
					VehicleSnapshotData element;
					element.NGSIM_Vehicle_ID = Vehicle_ID;
					//element.VehicleID = m_NumberOfVehicles;
					if (ngsim_to_index_map.find(Vehicle_ID) == ngsim_to_index_map.end())
					{
						element.VehicleID = ngsim_to_index_map.size();
						ngsim_to_index_map.insert(make_pair(Vehicle_ID, element.VehicleID));
					}
					else
					{
						element.VehicleID = ngsim_to_index_map[Vehicle_ID];
					}
					element.Frame_ID = Frame_ID;
					element.LocalY = Local_Y;  //m_NumberOfVehicles-1 make sure the index starting from 0
					element.LaneID = Lane_Identification;
					element.Speed_mph = Vehicle_Velocity * 0.681818182;  // convert from feet per second to mph
					element.CalculatedSpeed_mph = CalculatedSpeed;
					element.PrecedingVehicleID = Preceding_Vehicle;
					element.FollowingVehicleID = Following_Vehicle;
					element.Acceleration = Vehicle_Acceleration*0.3048; //mps
					element.Spacing_meters = Spacing * 0.3048;
					m_SelectLaneID = Lane_Identification;  // set Lane ID as a given number

					//OriginalDataVector.push_back(element);
					Copy_OriginalDataVector.push_back(element);
					current_vehicle_element_map[Vehicle_ID].push_back(element);
					//current_vehicle_element_vector.push_back(element);
					prev_Frame_ID = Frame_ID;
					prev_Total_Frames = Total_Frames;
					line++;

					vehicle_no_latest_information.insert(make_pair(Vehicle_ID, element));
				}
			}
		}
	}

	m_NumberOfVehicles = vehicle_cf_data_map.size();
	map<int, VehicleCFData>::iterator iter = vehicle_cf_data_map.begin();
	for (; iter != vehicle_cf_data_map.end(); iter++)
	{
		int Vehicle_ID = iter->first; 
		
		m_VehicleDataList.push_back(iter->second);
		current_vehicle_element_vector = current_vehicle_element_map[Vehicle_ID];
		for (int i = 0; i < current_vehicle_element_vector.size(); i++)
		{
			OriginalDataVector.push_back(current_vehicle_element_vector[i]);
		}
	}

	input_file.close();
	//sdzhao: export vehicle entry time to file
	//out_vehicle_entry_time
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		cout << "error current dir" << endl;
	string summary_folder(cCurrentPath);

	summary_folder += "/summary";
	if (CreateDirectory(summary_folder.c_str(), NULL))
		cout << "yes" << endl;
	else
	{
		cout << "fail" << endl;
	}

	for (auto &iter : out_vehicle_entry_time)
	{
		//  		ofstream f_veh_entry_time("NGSIM_vehicle_entry_time_" + to_string(iter.first) + ".csv", fstream::out);
		//  		f_veh_entry_time << iter.second.str();
		// 		iter.second.str(""); //clear contents
		//  		f_veh_entry_time.close();

		ofstream f_summary(summary_folder + "/NGSIM_summary_" + to_string(iter.first) + ".csv", fstream::out);
		f_summary << "max velocity(mps)," << out_laneno_max_velocity[iter.first] << endl;			//1
		f_summary << "max velocity(kph)," << out_laneno_max_velocity[iter.first] * 3.6 << endl;		//2
		f_summary << "max position(meters)," << out_laneno_max_Local_Y[iter.first] << endl;			//3
		f_summary << "max acceleration(mps2)," << out_laneno_max_acc[iter.first] << endl;			//4
		f_summary << "min acceleration(mps2)," << out_laneno_min_acc[iter.first] << endl;			//5
		f_summary << "first_vehicle_ID," << out_laneno_first_veh_no[iter.first] << endl;			//6
		f_summary << "desired interval/Tao (seconds)," << endl;										//7
		f_summary << "desired min spacing/d (meters)," << endl;										//8
		f_summary << "Count of Velocity(0~10 mph)," << out_laneno_velocity_frequency[Lane_Identification][0] << endl;
		f_summary << "Count of Velocity(10~20 mph)," << out_laneno_velocity_frequency[Lane_Identification][1] << endl;
		f_summary << "Count of Velocity(20~30 mph)," << out_laneno_velocity_frequency[Lane_Identification][2] << endl;
		f_summary << "Count of Velocity(30~40 mph)," << out_laneno_velocity_frequency[Lane_Identification][3] << endl;
		f_summary << "Count of Velocity(40~50 mph)," << out_laneno_velocity_frequency[Lane_Identification][4] << endl;
		f_summary << "Count of Velocity(50~60 mph)," << out_laneno_velocity_frequency[Lane_Identification][5] << endl;
		f_summary << "Count of Velocity(60~70 mph)," << out_laneno_velocity_frequency[Lane_Identification][6] << endl;
		f_summary << "Count of Velocity(70+ mph)," << out_laneno_velocity_frequency[Lane_Identification][7] << endl;
		//f_summary << "average headway (seconds)," << out_laneno_avg_headway[iter.first] << endl;
		//f_summary << "average spacing (meters)," << out_laneno_avg_spacing[iter.first] * 0.3048 << endl;
		f_summary.close();
	}

	m_VehicleSnapshotAry = AllocateDynamicArray<VehicleSnapshotData>(m_NumberOfVehicles, m_NumberOfTimeSteps + 1);

	map<int, vector<VehicleSnapshotData>> lane_no_first_25_vehs;

	for (unsigned i = 0; i < Copy_OriginalDataVector.size(); i++) //Copy_OriginalDataVector
	{
		VehicleSnapshotData element = Copy_OriginalDataVector[i];
		//
		int id = element.NGSIM_Vehicle_ID;
		auto pred = [id](const VehicleSnapshotData & item) {
			return item.NGSIM_Vehicle_ID == id;
		};
		if (std::find_if(std::begin(lane_no_first_25_vehs[element.LaneID]), std::end(lane_no_first_25_vehs[element.LaneID]), pred) == std::end(lane_no_first_25_vehs[element.LaneID])){
			lane_no_first_25_vehs[element.LaneID].push_back(element);
		}
		//
		////sdzhao export first vehicle trajectory
		if (out_laneno_first_veh_no[element.LaneID] == element.NGSIM_Vehicle_ID){
			out_vehicle_entry_time[element.LaneID]
				<< element.NGSIM_Vehicle_ID << ","
				<< element.Frame_ID << ","
				<< element.Speed_mph * 0.44704 << ","	//mps
				<< element.Acceleration << ","			//mps2
				<< element.LocalY *0.3048 << endl;      //meters
		}
	}
	//if (lane_no_first_25_vehs.size()>25)
	//{ 
	//	for (auto &iter : lane_no_first_25_vehs){
	//		vector<VehicleSnapshotData>& vec = iter.second;
	//		sort(vec.begin(), vec.end(), CompareByEntryTime);
	//		vec.erase(vec.begin() + 25, vec.end());

	//		ofstream f_traj_time("NGSIM_1_25_vehicle_entrytime_" + to_string(iter.first) + ".csv", fstream::out);
	//		for (int i = 0; i < 24; i++){
	//			f_traj_time << vec[i].NGSIM_Vehicle_ID << ","
	//				<< vec[i].Frame_ID << ","
	//				<< vec[i].Speed_mph * 0.44704 << ","	//mps
	//				<< vec[i].Acceleration << ","			//mps2
	//				<< vec[i].LocalY *0.3048 << endl;      //meters
	//		}
	//		f_traj_time.close();
	//	}
	//}
	//map<int, vector<VehicleSnapshotData>> lane_no_first_25_vehs_traj;
	//for (unsigned i = 0; i < Copy_OriginalDataVector.size(); i++) //Copy_OriginalDataVector
	//{
	//	VehicleSnapshotData element = Copy_OriginalDataVector[i];
	//	vector<VehicleSnapshotData>& vec = lane_no_first_25_vehs[element.LaneID];
	//	//if (vec.find(element.))
	//	int id = element.NGSIM_Vehicle_ID;
	//	auto pred = [id](const VehicleSnapshotData & item) {
	//		return item.NGSIM_Vehicle_ID == id;
	//	};
	//	if (std::find_if(std::begin(vec), std::end(vec), pred) != std::end(vec)){

	//		lane_no_first_25_vehs_traj[element.LaneID].push_back(element);
	//	}
	//}

	//for (auto &iter : lane_no_first_25_vehs_traj){
	//	ofstream f_traj_time("NGSIM_1_25_vehicle_trajectory_" + to_string(iter.first) + ".csv", fstream::out);
	//	vector<VehicleSnapshotData>& vec = iter.second;
	//	for (int i = 0; i < vec.size(); i++){
	//		f_traj_time << vec[i].NGSIM_Vehicle_ID << ","
	//			<< vec[i].Frame_ID << ","
	//			<< vec[i].Speed_mph * 0.44704 << ","	//mps
	//			<< vec[i].Acceleration << ","			//mps2
	//			<< vec[i].LocalY *0.3048 << ","
	//			<< vec[i].Spacing_meters << endl;      //meters
	//	}
	//	f_traj_time.close();
	//}

	//for (auto &lane : out_laneno_vehno_entry_time){
	//	int num_vehs_per_lane = lane.second.size();
	//	int veh_index = 0;
	//	ofstream f_veh_entry_time("NGSIM_vehicle_entry_time_" + to_string(lane.first) + ".csv", fstream::out);
	//	f_veh_entry_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
	//	ofstream f_veh_traj_time("NGSIM_shockwave_vehicle_trajectory_" + to_string(lane.first) + ".csv", fstream::out);
	//	f_veh_traj_time << "Vehicle_ID,Frame_ID,Vehicle_Velocity(mps),Local_Y(m),prev_veh_exit_time" << endl;
	//	int num_front_vehs = 14;
	//	for (auto &veh : lane.second){
	//		// 			if (veh_index >= num_vehs_per_lane - 90 - num_front_vehs && veh_index <= num_vehs_per_lane - 90)//if (veh_index >= num_vehs_per_lane - 90 && veh_index <= num_vehs_per_lane - 90+5)
	//		// 			{
	//		// 				f_veh_traj_time << veh.second.str();
	//		// 			}
	//		if (veh_index >= num_vehs_per_lane - 90 - num_front_vehs && veh_index <= num_vehs_per_lane - 90 + 5)
	//		{
	//			f_veh_traj_time << out_laneno_vehno_trajectory[lane.first][veh.first].str();
	//			f_veh_entry_time << out_laneno_vehno_entry_time[lane.first][veh.first].str();
	//		}
	//		else if (veh_index > num_vehs_per_lane - 90 && veh_index <= num_vehs_per_lane - 90 + 5)
	//			f_veh_entry_time << out_laneno_vehno_entry_time[lane.first][veh.first].str();
	//		veh_index++;
	//	}
	//	f_veh_entry_time.close();
	//	f_veh_traj_time.close();
	//}

	for (unsigned i = 0; i < OriginalDataVector.size(); i++)
	{
		VehicleSnapshotData element = OriginalDataVector[i];

		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].LaneID = element.LaneID;
		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].LocalY = element.LocalY;
		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].Speed_mph = element.Speed_mph;
		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].CalculatedSpeed_mph = element.CalculatedSpeed_mph;
		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].PrecedingVehicleID = element.PrecedingVehicleID;
		m_VehicleSnapshotAry[element.VehicleID][element.Frame_ID].FollowingVehicleID = element.FollowingVehicleID;
		if (StartTimeLane[element.LaneID] < 0)
			StartTimeLane[element.LaneID] = element.Frame_ID;
		StartTimeLane[element.LaneID] = element.Frame_ID < StartTimeLane[element.LaneID] ? element.Frame_ID : StartTimeLane[element.LaneID];
		EndTimeLane[element.LaneID] = element.Frame_ID > EndTimeLane[element.LaneID] ? element.Frame_ID : EndTimeLane[element.LaneID];
	}
	
	VehicleSnapshotData element = m_VehicleSnapshotAry[0][0]; 

	m_TimeLeft = 0;
	m_TimeRight = m_NumberOfTimeSteps;
	//read NGSIM scenario to get time interval and sensor spacing
	//ConstructCellBasedDensityProfile();
	ReadNGSIMScenarioFile();
	float SensorSpacing = m_DataCollectionSensorSpacing_in_feet; // 200;
	//float SensorSpacing = 200;
	float boundary_offset = 50;
	int data_collection_time_interval_in_sec = m_DataCollectionTimeInternval_in_sec;	//= 30;
	//ConstructCellBasedDensityProfile(int(boundary_offset), boundary_offset, data_collection_time_interval_in_sec);
	//ConstructCellBasedDensityProfile(int(boundary_offset / 2), int(boundary_offset / 2), data_collection_time_interval_in_sec);
	ConstructCellBasedDensityProfile(SensorSpacing, boundary_offset, data_collection_time_interval_in_sec);

	if (m_bOutputVissimToNGSIM)
	{
		ofstream ofile("vissim_to_ngsim_output.csv", ios::out);
		map<int, vector<VehicleSnapshotData>>::iterator ite = current_vehicle_element_map.begin();
		string delimeter = ",";
		for (; ite != current_vehicle_element_map.end(); ite++)
		{
			for (VehicleSnapshotData v_data : ite->second)
			{
				ofile << v_data.NGSIM_Vehicle_ID << delimeter;
				ofile << v_data.Frame_ID << delimeter;
				ofile << "" << delimeter;
				ofile << v_data.LocalX << delimeter;
				ofile << v_data.LocalY << delimeter;
				ofile << 0 << delimeter;
				ofile << 0 << delimeter;
				ofile << 0 << delimeter;
				ofile << 0 << delimeter;
				ofile << v_data.VehicleClass << delimeter;
				ofile << v_data.Speed_mph << delimeter;
				ofile << v_data.Acceleration << delimeter;
				ofile << v_data.LaneID << delimeter;
				ofile << -1 << delimeter;
				ofile << -1 << delimeter;
				ofile << 0 << endl;
			}
		}
	}
}

void CTimeSpaceView::OnToolsLoadvehicletrajactoryfile_backup()
{
	int Vehicle_ID; //1 
	int Frame_ID;//2
	int Total_Frames; //3
	long long Global_Time ; //4
	double Local_X ; //5
	double Local_Y  ; //6
	double Global_X  ; //7
	double Global_Y  ; //8
	float Vehicle_Length ; //9
	float Vehicle_Width ; //10
	int	Vehicle_Class ; //11

	float Vehicle_Velocity ; //12
	float Vehicle_Acceleration  ; //13

	int Lane_Identification ; //14
	int	Preceding_Vehicle ; //15
	int	Following_Vehicle  ; //16

	float Spacing ; //17
	float Headway  ; //18


	static char BASED_CODE szFilter[] = "NGSIM Vehicle Trajectory File (*.txt)|*.txt||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);

	std::vector<VehicleSnapshotData> OriginalDataVector;

	CTime ExeStartTime = CTime::GetCurrentTime();
	int line = 0;

	if(dlg.DoModal() == IDOK)
	{
		CWaitCursor wc;
		char fname[_MAX_PATH];
		wsprintf(fname,"%s", dlg.GetPathName());
		//FILE* st;

		m_LocalYUpperBound = 0;
		m_LocalYLowerBound = 9999999;

		int Prev_Vehicle_ID = -9999;
		int Prev_Vehicle_ID_In_TargetLanes = -9999;
		int Last_Observed_LaneID = -1;
		bool b_OutputAVI = false;
		int first_frame_id = -1;

		//fopen_s(&st,fname,"r");
		ifstream in(dlg.GetPathName());
		if(in.is_open())
		{
			m_NumberOfVehicles = 0;
			m_NumberOfTimeSteps = 0;

			string s;

			ofstream  NGSIM_LogFile;
			NGSIM_LogFile.open ("C:\\NGSIM_Trajectry_Sample.csv", ios::out);
			NGSIM_LogFile.setf(ios::fixed);
			NGSIM_LogFile << "Veh_ID,Veh_Class,Lane_ID,Sensor_Type,Frame_ID,X,Y,Speed,Acceleration,Error" << endl;

			while(/*!feof(st)*/getline(in,s))
			{	
				VehicleCFData vcd;

				istringstream is(s);

				is >> Vehicle_ID >> Frame_ID >> Total_Frames >> Global_Time
					>> Local_X >> Local_Y >> Global_X >> Global_Y 
					>> Vehicle_Length >> Vehicle_Width >> Vehicle_Class
					>> Vehicle_Velocity >> Vehicle_Acceleration >> Lane_Identification
					>> Preceding_Vehicle >> Following_Vehicle >> Spacing >> Headway;

				//Vehicle_ID = g_read_integer(st);  //1
				if(Vehicle_ID < 0)
					break;

				//Frame_ID = g_read_integer(st); //2

				if(Frame_ID > m_NumberOfTimeSteps)
					m_NumberOfTimeSteps = Frame_ID;

				//Total_Frames = g_read_integer(st); //3
				//Global_Time = g_read_integer(st); //4

				//Local_X = g_read_float(st); //5
				//Local_Y = g_read_float(st); //6

				if(Local_Y > m_LocalYUpperBound)
					m_LocalYUpperBound = Local_Y;

				if(Local_Y < m_LocalYLowerBound)
					m_LocalYLowerBound = Local_Y;

				//Global_X = g_read_float(st); //7
				//Global_Y = g_read_float(st); //8
				//Vehicle_Length = g_read_float(st); //9
				//Vehicle_Width = g_read_float(st); //10
				//Vehicle_Class = g_read_integer(st); //11
				//Vehicle_Velocity = g_read_float(st); //12
				//Vehicle_Acceleration  = g_read_float(st); //13
				//Lane_Identification = g_read_integer(st); //14
				//Preceding_Vehicle = g_read_integer(st); //15
				//Following_Vehicle = g_read_integer(st); //16
				//Spacing =  g_read_float(st); //17
				//Headway =  g_read_float(st); //18

				if(Prev_Vehicle_ID != Vehicle_ID)
				{

					b_OutputAVI = true;
					m_NumberOfVehicles++;

					m_VehicleIDtoNOMap[Vehicle_ID] = m_NumberOfVehicles;  // ID to sequential No.

					//Prev_Vehicle_ID = Vehicle_ID;


					first_frame_id = Frame_ID;

					vcd.VehicleID = Vehicle_ID;
					vcd.StartTime = Frame_ID;
					vcd.EndTime = Frame_ID + Total_Frames;
					vcd.VehicleType = Vehicle_Class;

					m_VehicleDataList.push_back (vcd);

				}

				VehicleSnapshotData element;
				element.VehicleID = m_NumberOfVehicles-1;
				element.Frame_ID  = Frame_ID;
				element.LocalX = Local_X;
				element.LocalY  =  Local_Y;  //m_NumberOfVehicles-1 make sure the index starting from 0
				element.LaneID  =  Lane_Identification; 
				element.Speed_mph  = Vehicle_Velocity * 0.681818182;  // convert from feet per second to mph
				element.Acceleration = Vehicle_Acceleration * (3600*3600 / 5280);
				element.VehicleClass = Vehicle_Class;
				element.PrecedingVehicleID = Preceding_Vehicle;
				element.FollowingVehicleID = Following_Vehicle;

				m_SelectLaneID = Lane_Identification;  // set Lane ID as a given number

				OriginalDataVector.push_back(element);


				if (NGSIM_LogFile.is_open() )
				{
					if ( (Lane_Identification == 4 || Lane_Identification == 6))
					{
						//Normal output 
						NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
							<< Lane_Identification << "," 
							<< 0 << ","
							<< Frame_ID << "," 
							<< Local_X <<  "," << Local_Y <<  "," 
							<< Vehicle_Velocity <<  "," << Vehicle_Acceleration << ","
							<< 0 // for normal data, the error is 0
							<< endl;

						//Loop detector, at fixed location (in this case: local_y = 860
						//10% error is added into observed speed
						if (Local_Y >= 860 && b_OutputAVI) 
						{
							NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
								<< Lane_Identification << "," 
								<< 1 << ","
								<< Frame_ID << "," 
								<< Local_X <<  "," << 860 <<  ",";
							// for loop detector, based on the random number generated, >= 0.5 --> 0.1 otherwise -0.1
							float random_error = g_GetRandomRatio()>= 0.5?0.1:-0.1;
							NGSIM_LogFile << Vehicle_Velocity + Vehicle_Velocity * random_error  <<  "," 
								<< Vehicle_Acceleration << ","
								<< random_error
								<< endl;

							b_OutputAVI = false;
						}


						//AVI Data -- 20% error in observation time in term of time (FrameID)
						//First observation of one specific vehicle
						if(Prev_Vehicle_ID_In_TargetLanes != Vehicle_ID)
						{
							float random_error = g_GetRandomRatio()>= 0.5?0.2:-0.2;

							NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
								<< Lane_Identification << "," 
								<< 2 << ","
								<< Frame_ID + (int)( Frame_ID * random_error) << "," 
								<< Local_X <<  "," << Local_Y <<  "," 
								<< Vehicle_Velocity <<  "," << Vehicle_Acceleration << ","
								<< random_error // for normal data, the error is 0
								<< endl;

							Prev_Vehicle_ID_In_TargetLanes = Vehicle_ID;
							Last_Observed_LaneID = Lane_Identification;
						}


						//Last observation of one specific vehicle
						if ( Frame_ID  == Total_Frames + first_frame_id -1)
						{
							float random_error = g_GetRandomRatio()>= 0.5?0.2:-0.2;

							NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
								<< Lane_Identification << "," 
								<< 2 << ","
								<< Frame_ID + (int)( Frame_ID * random_error) << "," 
								<< Local_X <<  "," << Local_Y <<  "," 
								<< Vehicle_Velocity <<  "," << Vehicle_Acceleration << ","
								<< random_error // for normal data, the error is 0
								<< endl;
						}

						//GPS trace
						//10% error is added in to time, local_y and speed
						if ((Frame_ID - first_frame_id) % 100 == 0)
						{
							float random_error = g_GetRandomRatio()>= 0.5?0.1:-0.1;
							NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
								<< Lane_Identification << "," 
								<< 3 << ","
								<< Frame_ID + (int) (Frame_ID * random_error) << "," 
								<< Local_X <<  "," << Local_Y + (int) (Frame_ID * random_error) <<  "," 
								<< Vehicle_Velocity + Vehicle_Velocity * random_error <<  "," 
								<< Vehicle_Acceleration << ","
								<< random_error // for normal data, the error is 0
								<< endl;
						}

					}

					//for those vehilce change from lane 4 or 6 to another lane
					if (Prev_Vehicle_ID_In_TargetLanes == Vehicle_ID && Last_Observed_LaneID != Lane_Identification)
					{
						if ( !(Last_Observed_LaneID == 4 || Last_Observed_LaneID == 6) && !(Lane_Identification == 4 || Lane_Identification == 6))
						{
						}
						else
						{
							float random_error = g_GetRandomRatio()>= 0.5?0.2:-0.2;

							int lane_id;
							if ( Last_Observed_LaneID == 4 || Last_Observed_LaneID == 6)
							{
								lane_id = Last_Observed_LaneID;
							}
							else
							{
								lane_id = Lane_Identification;
							}
							NGSIM_LogFile << Vehicle_ID << "," << Vehicle_Class << "," 
								<< lane_id << "," 
								<< 2 << ","
								<< Frame_ID + (int)( Frame_ID * random_error) << "," 
								<< Local_X <<  "," << Local_Y <<  "," 
								<< Vehicle_Velocity <<  "," << Vehicle_Acceleration << ","
								<< random_error // for normal data, the error is 0
								<< endl;
						}

					}

				}

				if(Prev_Vehicle_ID != Vehicle_ID)
				{
					Prev_Vehicle_ID = Vehicle_ID;
				}

				Last_Observed_LaneID = Lane_Identification;

				line++;
			}

			NGSIM_LogFile.close();
			//fclose(st);
		}
	}

	m_VehicleSnapshotAry	= AllocateDynamicArray<VehicleSnapshotData>(m_NumberOfVehicles,m_NumberOfTimeSteps+1);

	for(unsigned i  = 0; i < OriginalDataVector.size(); i++)
	{
		VehicleSnapshotData element = OriginalDataVector[i];
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].LaneID =  element.LaneID;
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].LocalY =  element.LocalY;
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].Speed_mph =  element.Speed_mph;
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].PrecedingVehicleID =  element.PrecedingVehicleID;
		m_VehicleSnapshotAry [element.VehicleID ] [element.Frame_ID].FollowingVehicleID  =  element.FollowingVehicleID;
	}

	m_TimeLeft = 0;
	m_TimeRight = m_NumberOfTimeSteps;
	/// loading statistics
	CTime ExeEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = ExeEndTime  - ExeStartTime;
	CString str_running_time;

	str_running_time.Format ("Load %d vehicles and %d lines from the vehicle trajectory file.\nLoading Time: %d hour %d min %d sec. \n",
		m_NumberOfVehicles, line, ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

	AfxMessageBox(str_running_time, MB_ICONINFORMATION);
}
void CTimeSpaceView::OnNgsimdataLane1()
{
	m_SelectLaneID  = 1;
	Invalidate();

	map<float, map<int, float>> Distance_Time_Value;
	float sensor_spacing = 100;
	//Output_NGSIM_Matrix("cumulative_count", sensor_spacing, Distance_Time_Value);
	int display_resolution = 1; //30
	//Output_NGSIM_Individual_Vehicle_Trajectory(display_resolution, m_PlotRectNGSIM, Distance_Time_Value);
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataLane2()
{
	m_SelectLaneID  = 2;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
	//Output_NGSIM_Individual_Vehicle_Trajectory(1);
}

void CTimeSpaceView::OnNgsimdataLane3()
{
	m_SelectLaneID  = 3;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataLane4()
{
	m_SelectLaneID  = 4;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataLane5()
{
	m_SelectLaneID  = 5;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataLane6()
{
	m_SelectLaneID  = 6;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataLane7()
{
	m_SelectLaneID  = 7;
	Invalidate();
	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::OnNgsimdataColorspeed()
{
	m_bColorBySpeedFlag = !m_bColorBySpeedFlag;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimdataColorspeed(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bColorBySpeedFlag);
}

bool compare_timestamp (int first, int second)
{
	if(first < second) return true;
	else 
		return false;
}

void CTimeSpaceView::OnNgsimdataCarfollowingsimulation()
{
	// for each lane
	// find out the first vehicle on this lane
	// record the trajectory as the lead vehicle as the boundary condition
	// record the upstream and downstream sensor location: +- 50 feet 
	// count vehicles passing through sensors, build the arrival and departure timestamps
	// create new vehicle car following data array
	// simulation
	// display

	m_NumberOfVehicles_lane  = m_NumberOfTimeSteps / 20;  // 2 second per vehicle at most


	int t;
	m_VehicleSnapshotAry_Lane = Allocate3DDynamicArray<float>(_MAX_LANE_SIZE,m_NumberOfVehicles_lane,m_NumberOfTimeSteps+1);

	m_VehicleStartTimeAry_Lane = AllocateDynamicArray<int>(_MAX_LANE_SIZE,m_NumberOfVehicles_lane);
	m_VehicleEndTimeAry_Lane = AllocateDynamicArray<int>(_MAX_LANE_SIZE,m_NumberOfVehicles_lane);


	for(int LaneID=0; LaneID<_MAX_LANE_SIZE; LaneID++)
	{
		// find out the first vehicle on this lane

		int v;
		float FirstTimeStamp  = 999999;
		int FirstVehicleID = 0;
		for( v=0; v<m_NumberOfVehicles; v+=1)
		{
			for(t= m_VehicleDataList[v].StartTime; t<=m_VehicleDataList[v].EndTime; t+=1)  
			{
				if(m_VehicleSnapshotAry[v][t].LaneID ==LaneID+1)  //LaneID starts from 0
				{
					if(t < FirstTimeStamp)
					{
						FirstTimeStamp = t;
						FirstVehicleID = v;

					}
				}

			}
		}

		// record vehicle trajectory

		// for leader vehicle, initialize
		StartTimeLane[LaneID] = -1; 
		EndTimeLane[LaneID] = -1;


		for(t= m_VehicleDataList[FirstVehicleID].StartTime; t<=m_VehicleDataList[FirstVehicleID].EndTime; t+=1)  
		{

			if(StartTimeLane[LaneID] != -1 && m_VehicleSnapshotAry[FirstVehicleID][t].LaneID !=LaneID+1)
			{
				break; // switch to another link!
			}

			if(m_VehicleSnapshotAry[FirstVehicleID][t].LaneID ==LaneID+1)  //LaneID starts from 0
			{

				if(StartTimeLane[LaneID] == -1) // has not been initialized
				{
					StartTimeLane[LaneID] = t;
					StartLocalYLane[LaneID] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY;
				}

				EndTimeLane[LaneID] = t;  // keep assinging time stamp value
				EndLocalYLane[LaneID] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY;

				// record trajactory for the first vehicle
				m_VehicleSnapshotAry_Lane[LaneID][0][t] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY ; 
				// simulated data array (lane by lane)     original data array

			}
		}

		// set up the position for upstream and downstream (single) loop detectors, can be extended to double loops
		StartLocalYLane[LaneID] += 50.0f; // move up by 50 feet
		EndLocalYLane[LaneID] -= 50.0f; // move down by 50 feet


		// find passing time stamps of vehicles

		std::vector<int> StartPassingTimeStamp; 
		std::vector<int> EndPassingTimeStamp;

		for( v=0; v<m_NumberOfVehicles; v+=1)
		{
			for(t= m_VehicleDataList[v].StartTime; t<m_VehicleDataList[v].EndTime; t+=1)  
			{
				if(m_VehicleSnapshotAry[v][t].LaneID ==LaneID+1)  //LaneID starts from 0
				{

					if(m_VehicleSnapshotAry[v][t].LocalY <= StartLocalYLane[LaneID] && StartLocalYLane [LaneID]< m_VehicleSnapshotAry[v][t+1].LocalY )
					{
						StartPassingTimeStamp.push_back(t);
					}

					if(m_VehicleSnapshotAry[v][t].LocalY <= EndLocalYLane [LaneID] && EndLocalYLane [LaneID] < m_VehicleSnapshotAry[v][t+1].LocalY )
					{
						EndPassingTimeStamp.push_back(t);
					}

				}

			}
		}

		std::sort(StartPassingTimeStamp.begin(), StartPassingTimeStamp.end(), compare_timestamp);
		std::sort(EndPassingTimeStamp.begin(), EndPassingTimeStamp.end(), compare_timestamp);

		// here we assume the first leader vehicle is 0

		VehicleSizeLane[LaneID] = min(StartPassingTimeStamp.size (),EndPassingTimeStamp.size());
		for (v = 0; v < VehicleSizeLane[LaneID]; v++)  // 1 starts from the first follower.
		{
			m_VehicleStartTimeAry_Lane[LaneID][v] = StartPassingTimeStamp[v];
			m_VehicleEndTimeAry_Lane[LaneID][v] = EndPassingTimeStamp[v];
			TRACE("no.%d, %d, %d\n", v, StartPassingTimeStamp[v], EndPassingTimeStamp[v]); 
		}	

		// construct cumuluative count,
		// apply stochatic three detector theorem. 
		// reconstruct arrival and departure time sequences
		// run traffic simulation


		float m_FreeflowMoveDistance = m_FreeflowSpeed *5280 / 36000;  // 60 mph as free-flow speed, 5280 feet per mile, 3600 second with 0.1 seconds as time step

		for (v = 1; v < VehicleSizeLane[LaneID]; v++)  // start from vehicle 1 as the first follower
		{
			for(t= StartPassingTimeStamp[v]; t<= EndPassingTimeStamp[v]; t++) 
			{

				//calculate free-flow position
				//xiF(t) = xi(t-τ) + vf(τ)

				if(t==StartPassingTimeStamp[v] )
				{
					m_VehicleSnapshotAry_Lane[LaneID][v][t] =  StartLocalYLane[LaneID];  // assign starting position
					continue;
				}
				else
				{
					m_VehicleSnapshotAry_Lane[LaneID][v][t] = m_VehicleSnapshotAry_Lane[LaneID][v][t-1] +  m_FreeflowMoveDistance;
				}

				// if this vehicle exceeds the downstream sensor location, needs to stop (wait!)
				if( m_VehicleSnapshotAry_Lane[LaneID][v][t] > EndLocalYLane [LaneID] )
					m_VehicleSnapshotAry_Lane[LaneID][v][t] = EndLocalYLane [LaneID] ;

				// calculate congested position

				//τ = 1/(wkj)
				// δ = 1/kj
				float time_tau_in_time_step  = 1 /(m_WaveSpeed*m_KJam)*36000; // to 0.1 second  
				float spacing = 1.0f/m_KJam*5280;   // to feet
				//xiC(t) = xi-1(t-τ) - δ
				int time_t_minus_tau = t - int(time_tau_in_time_step); // need to convert time in second to time in simulation time interval

				if(time_t_minus_tau >=StartPassingTimeStamp[v-1])  // the leader has not reached destination yet
				{
					float CongestedDistance = max(StartLocalYLane[LaneID], m_VehicleSnapshotAry_Lane[LaneID][v-1][time_t_minus_tau]  - spacing); 
					// max (upsteram sensor location,) makes sure the congested distance is at least upsteram sensor location.
					// v-1 is the leader of the current vehicle, time_t_minus_tau is the lagged timestamp
					// xi(t) = min(xAF(t), xAC(t))
					if (m_VehicleSnapshotAry_Lane[LaneID][v][t]  > CongestedDistance && CongestedDistance >= m_VehicleSnapshotAry_Lane[LaneID][v][t-1])
						m_VehicleSnapshotAry_Lane[LaneID][v][t] = CongestedDistance;


				}

			}
		}
	}
	m_bShowSimulationDataFlag = true;

}

void  CTimeSpaceView::UpdateVehicleStartANDEndTimes(std::vector<int> &StartPassingTimeStamp, std::vector<int> &EndPassingTimeStamp)
{
	// construct cumuluative count, A, D. 
	// apply stochatic three detector theorem  to get A' and D'
	// apply deterministic detector theorem again to get the perfect A and D
	// reconstruct arrival and departure time sequences

}

void CTimeSpaceView::OnNgsimdataShowsimulatedresults()
{
	m_bShowSimulationDataFlag = ! m_bShowSimulationDataFlag;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimdataShowsimulatedresults(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSimulationDataFlag);
}

void CTimeSpaceView::OnUpdateNgsimdataShowbothobservedandsimulationresults(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowBothResultsFlag);
}

void CTimeSpaceView::OnNgsimdataShowbothobservedandsimulationresults()
{
	m_bShowBothResultsFlag = !m_bShowBothResultsFlag;
	Invalidate();
}

void CTimeSpaceView::OnNgsimdataChangesettingsofcarfollowingmodel()
{
	CDlgCarFollowingModelSettings dlg;
	dlg.m_Kjam = m_KJam ;
	dlg.m_WaveSpeed = m_WaveSpeed;
	dlg.m_FreeflowSpeed = m_FreeflowSpeed;
	if(dlg.DoModal() == IDOK)
	{
		m_KJam = dlg.m_Kjam;
		m_WaveSpeed = dlg.m_WaveSpeed;
		m_FreeflowSpeed = dlg.m_FreeflowSpeed;

		OnNgsimdataCarfollowingsimulation();
	}

}

void CTimeSpaceView::OnNgsimdataCarfollowingdataextraction()
{
	CString str;
	CFileDialog dlg (FALSE, "*.csv", "*.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
		"(*.csv)|*.csv||", NULL);
	if(dlg.DoModal() == IDOK)
	{
		char fname[_MAX_PATH];
		wsprintf(fname,"%s", dlg.GetPathName());
		CWaitCursor wait;

		if(!ExportDataToFile(fname,1))
		{
			str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", fname);
			AfxMessageBox(str);
		}

	}

}

bool CTimeSpaceView::ExportDataToFile(char csv_file[_MAX_PATH], int DataFlag)
{
	FILE* st;
	fopen_s(&st,csv_file,"w");

	m_VehicleNoVector_Lane.clear ();
	int v;
	for( v=0; v<m_NumberOfVehicles; v+=1)
	{
		float StartTime = m_VehicleDataList[v].StartTime;

		if(StartTime < m_TimeLeft)
			StartTime = m_TimeLeft;

		float EndTime = m_VehicleDataList[v].EndTime;

		if(EndTime > m_TimeRight)
			EndTime = m_TimeRight;

		if(StartTime< EndTime)
		{
			for(int t= StartTime; t<=EndTime; t+=1)  
			{

				if(m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID)
				{

					m_VehicleNoVector_Lane.push_back (v);   // add vehicle no v into the vector to be searched
					break;

				}
			}
		}

	}


	if(st!=NULL)
	{
		CWaitCursor wc;
		fprintf(st, "vehicle_id, vehicle_type,start_lane,time stamp (1/10 sec),local_y (feet),calculated speed (mph), speed (mph), acceleration, preceeding_vehicle_id,local_y,calculated speed (mph),speed (mph),acceleration,following_vehicle_id,local_y,calculated speed (mph),speed,acceleration");

		int prev_leader_no = -1;
		int prev_follower_no = -1;

		for(v=0; v<m_NumberOfVehicles; v+=1)
		{
			bool bIniFlag  = false;
			float StartTime = m_VehicleDataList[v].StartTime;

			if(StartTime < m_TimeLeft)
				StartTime = m_TimeLeft;

			float EndTime = m_VehicleDataList[v].EndTime;

			if(EndTime > m_TimeRight)
				EndTime = m_TimeRight;

			if(StartTime< EndTime)
			{
				for(int t= StartTime; t<=EndTime; t+=1)  
				{

					if(m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID)
					{

						if( bIniFlag == false)
						{
							fprintf(st,"\n%d,%d,%d,",m_VehicleDataList[v].VehicleID,m_VehicleDataList[v].VehicleType,m_SelectLaneID );
							bIniFlag = true;

						}else
						{
							fprintf(st,",,,");

						}

						//time,local_y,calculated speed, speed
						fprintf(st,"%d,%9.3f,%9.3f,%9.3f,",t,m_VehicleSnapshotAry[v][t].LocalY, m_VehicleSnapshotAry[v][t].CalculatedSpeed_mph , m_VehicleSnapshotAry[v][t].Speed_mph); 

						//acceleration
						if(t>StartTime)
							fprintf(st,"%9.3f,",m_VehicleSnapshotAry[v][t].Speed_mph - m_VehicleSnapshotAry[v][t-1].Speed_mph); 
						else // t== StartTime
							fprintf(st,"%9.3f,",0.0f); 


						int leader_no = FindPreceedingVehicleNo(m_SelectLaneID,m_VehicleSnapshotAry[v][t].LocalY,t);
						//time,local_y,calculated speed, speed
						if(leader_no>=0)
						{
							fprintf(st,"%d,%9.3f,%9.3f,%9.3f,",m_VehicleDataList[leader_no].VehicleID ,
								m_VehicleSnapshotAry[leader_no][t].LocalY,
								m_VehicleSnapshotAry[leader_no][t].CalculatedSpeed_mph ,
								m_VehicleSnapshotAry[leader_no][t].Speed_mph); 
							if(prev_leader_no == leader_no)
								fprintf(st,"%9.3f,",m_VehicleSnapshotAry[leader_no][t].Speed_mph - m_VehicleSnapshotAry[leader_no][t-1].Speed_mph); 
							else
								fprintf(st,"0.0,");
						}else
						{
							fprintf(st,"-1,0,0,0,",0.0f); 

						}

						prev_leader_no = leader_no;

						int follower_no = FindFollowingVehicleNo(m_SelectLaneID,m_VehicleSnapshotAry[v][t].LocalY,t);
						//time,local_y,speed
						if(follower_no>=0)
						{
							fprintf(st,"%d,%9.3f,%9.3f,%9.3f,",m_VehicleDataList[follower_no].VehicleID ,
								m_VehicleSnapshotAry[follower_no][t].LocalY,
								m_VehicleSnapshotAry[follower_no][t].CalculatedSpeed_mph , 
								m_VehicleSnapshotAry[follower_no][t].Speed_mph); 
							if(prev_follower_no ==follower_no ) 
								fprintf(st,"%9.3f\n",m_VehicleSnapshotAry[follower_no][t].Speed_mph - m_VehicleSnapshotAry[follower_no][t-1].Speed_mph); 
							else
								fprintf(st,"0\n"); 

						}else
						{
							fprintf(st,"-1,0,0,0\n",0.0f); 

						}

						prev_follower_no = follower_no;



					}


				}

			}

		}

		fclose(st);
		return true;
	}
	return false;
}

void CTimeSpaceView::OnNgsimdataShowvehicleid()
{
	m_bShowVehicleIDFlag = !m_bShowVehicleIDFlag;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimdataShowvehicleid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowVehicleIDFlag);

}

void CTimeSpaceView::OnNgsimdataShowpreceedingvehicleposition()
{
	m_bShowLeaderPositionFlag = !m_bShowLeaderPositionFlag;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimdataShowpreceedingvehicleposition(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowLeaderPositionFlag);
}


int CTimeSpaceView::FindPreceedingVehicleNo(int LaneNo,float LocalY, int t)
{
	unsigned int vi;
	int leader_no = -1;
	float local_y_leader = 9999999;
	for( vi=0; vi<m_VehicleNoVector_Lane.size(); vi+=1)
	{
		int v = m_VehicleNoVector_Lane[vi];
		float StartTime = m_VehicleDataList[v].StartTime;

		if(StartTime < m_TimeLeft)
			StartTime = m_TimeLeft;

		float EndTime = m_VehicleDataList[v].EndTime;

		if(EndTime > m_TimeRight)
			EndTime = m_TimeRight;

		if(StartTime<= t && t <= EndTime && m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID && m_VehicleSnapshotAry[v][t].LocalY > LocalY + 0.1)
		{
			if(m_VehicleSnapshotAry[v][t].LocalY < local_y_leader )  // taking minimum
			{
				local_y_leader = m_VehicleSnapshotAry[v][t].LocalY ;
				leader_no = v;

			}

		}
	}

	return leader_no;
}

int CTimeSpaceView::FindFollowingVehicleNo(int LaneNo,float LocalY, int t)
{
	int vi;
	int follower_no = -1;
	float local_y_follower = 0;
	for( vi=0; vi<m_VehicleNoVector_Lane.size(); vi+=1)
	{
		int v = m_VehicleNoVector_Lane[vi];
		float StartTime = m_VehicleDataList[v].StartTime;

		if(StartTime < m_TimeLeft)
			StartTime = m_TimeLeft;

		float EndTime = m_VehicleDataList[v].EndTime;

		if(EndTime > m_TimeRight)
			EndTime = m_TimeRight;

		if(StartTime<= t && t <= EndTime && m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID && m_VehicleSnapshotAry[v][t].LocalY < LocalY - 0.1)
		{
			if(m_VehicleSnapshotAry[v][t].LocalY > local_y_follower )  // taking maximum
			{
				local_y_follower = m_VehicleSnapshotAry[v][t].LocalY ;
				follower_no = v;

			}

		}
	}

	return follower_no;
}

void CTimeSpaceView::OnNgsimdataSearchByVehicleId()
{
	CDlg_Find_Vehicle dlg;
	if(dlg.DoModal () == IDOK)
	{
		m_SelectedVehicleID = dlg.m_VehicleID;
		Invalidate();	
	}

}


void  CTimeSpaceView::DrawTemporalLink(CPoint start_point, CPoint end_point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);

	dc.SelectObject(&g_TempLinkPen2);
	// Select drawing mode

	int oldROP2 = dc.SetROP2(R2_XORPEN);

	// Draw the line
	dc.MoveTo(start_point);
	dc.LineTo(end_point);

	// Reset drawing mode
	dc.SetROP2(oldROP2);

}


int CTimeSpaceView::CountVehicles(int SelectedStartTime, int SelectedEndTime, float SelectedStartLocalY,float SelectedEndLocalY)
{
	m_SelectedVehicleCount = 0;

	TRACE("----");

	if(SelectedStartTime > SelectedEndTime)  //swap timestamps and locations
	{
		int temp = 	SelectedStartTime;
		SelectedStartTime = SelectedEndTime;
		SelectedEndTime = temp;

		float temp_y = SelectedStartLocalY;
		SelectedStartLocalY = SelectedEndLocalY;
		SelectedEndLocalY = temp_y;
	}


	for(int v=0; v<m_NumberOfVehicles; v+=1)
	{ 
		bool intersection_flag = false;  // default value: false

		float current_local_y ;
		float previous_local_y = -1;
		for(int t = SelectedStartTime; t<= SelectedEndTime; t++)  // first loop
		{
			current_local_y  = SelectedStartLocalY + (float)(t-SelectedStartTime)/max(1,	SelectedEndTime-SelectedStartTime)*(SelectedEndLocalY - SelectedStartLocalY); 

			if(previous_local_y < 0 )  // initialization
			{

				if(SelectedStartTime == SelectedEndTime)  // at the same time, special condition applied here
				{
					previous_local_y =  SelectedEndLocalY;
				}else
				{
				previous_local_y =  SelectedStartLocalY;
				}
			}

			float i_x = 0;
			float i_y = 0;

			if( m_VehicleDataList[v].StartTime < t && m_VehicleDataList[v].EndTime >t 
				&& m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID )
			{
				int intersection_test = g_get_line_intersection(t-1,previous_local_y,t , current_local_y,
					t-1, m_VehicleSnapshotAry[v][t-1].LocalY, t, m_VehicleSnapshotAry[v][t].LocalY, &i_x, &i_y) ;

				if(intersection_test >0)
				{
					intersection_flag = true;
					break;

				}  //
			}
		}
		if(intersection_flag)
		{
			m_SelectedVehicleCount ++;
		}

	}  // for each vehicle

	return m_SelectedVehicleCount;
}

double CTimeSpaceView::CalculateSpaceMeanSpeedHarmonic(int StartTime, int EndTime, LinkSensorData &element){
	int totoal_num_vehicles = 0;
	double sum_spd_inverse = 0.0;
	for (int t = StartTime; t < EndTime; t++){
		int num_vehs = element.observed_spd_within_sensor_time_rectangle[t].size();
		totoal_num_vehicles += num_vehs;
		for (int v = 0; v < num_vehs; v++){
			sum_spd_inverse += (1/ element.observed_spd_within_sensor_time_rectangle[t][v]);
		}
	}
	return  ((double)totoal_num_vehicles)  / sum_spd_inverse;		//mph
};

double CTimeSpaceView::CalculateSpaceMeanSpeedElapsedTime(int iLink, int t, float DataCollectionSensorSpacing_in_feet, 
	map<int, vector<map<int, int>>> & StartPassingTimeStampAtSensorIndex,
	map<int, vector<map<int, int>>> & EndPassingTimeStampAtSensorIndex,
	float &edie_density){
	double avg_ElapsedTime = 0.0;
	int ctr = 0;
	for (auto &iter : StartPassingTimeStampAtSensorIndex[iLink][t]){
		int v_index = iter.first;
		if (EndPassingTimeStampAtSensorIndex[iLink][t].find(v_index) != EndPassingTimeStampAtSensorIndex[iLink][t].end())
		{
			double ElapsedTime = EndPassingTimeStampAtSensorIndex[iLink][t][v_index] - StartPassingTimeStampAtSensorIndex[iLink][t][v_index];
			avg_ElapsedTime += ElapsedTime;
			ctr++;
		}

	}

	
	if (ctr == 0){
		return -1;
	}
	else
	{
		edie_density = (avg_ElapsedTime / 10) / (m_DataCollectionTimeInternval_in_sec * (DataCollectionSensorSpacing_in_feet / 5280));
		avg_ElapsedTime /= ctr;
		return (DataCollectionSensorSpacing_in_feet / 5280) / (avg_ElapsedTime / 36000);
	}
	
};

double CTimeSpaceView::CalculateTimeMeanSpeed(int StartTime, int EndTime, LinkSensorData &element){
	int totoal_num_vehicles = 0;
	double sum_spd = 0.0;
	for (int t = StartTime; t < EndTime; t++){
		int num_vehs = element.observed_spd_within_sensor_time_rectangle[t].size();
		totoal_num_vehicles += num_vehs;
		for (int v = 0; v < num_vehs; v++){
			sum_spd += element.observed_spd_within_sensor_time_rectangle[t][v];
		}
	}
	return  sum_spd / (double)totoal_num_vehicles;		//mph
};

void CTimeSpaceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CountVehicles(m_SelectedStartTime,m_SelectedEndTime, m_SelectedStartLocalY,m_SelectedEndLocalY);
}
void CTimeSpaceView::ConstructCellBasedDensityProfile()
{
	float SensorSpacing = 300.0f;  // 0.1 mile
	// step 1: point sensor data

	float boundary_offset = 50;
	float DetectorLocalY;
	for(DetectorLocalY = m_LocalYLowerBound + boundary_offset; DetectorLocalY <= m_LocalYUpperBound - boundary_offset; DetectorLocalY+= SensorSpacing) // - 50 and +50 at boundary.
	{
		// find passing time stamps of vehicles

		std::vector<int> StartPassingTimeStamp; 
		std::vector<int> EndPassingTimeStamp;

		for(int v=0; v< m_NumberOfVehicles; v+=1)  // for all vehicles
		{
			for(int t= m_VehicleDataList[v].StartTime; t<m_VehicleDataList[v].EndTime; t+=1)  // for all valid timestamp
			{
				if(m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID)   // for passing lane
				{
					if(m_VehicleSnapshotAry[v][t].LocalY <= DetectorLocalY && DetectorLocalY < m_VehicleSnapshotAry[v][t+1].LocalY )
					{
						StartPassingTimeStamp.push_back(t);
					}
				}
			}
		}
		std::sort(StartPassingTimeStamp.begin(), StartPassingTimeStamp.end(), compare_timestamp);

		PointSensorData point_sensor;

		point_sensor.DetectorLocalY = DetectorLocalY;
		point_sensor.PassingTimeStampVector = StartPassingTimeStamp;

		int data_collection_time_interval_in_sec = 30;
		int number_of_time_intervals = m_NumberOfTimeSteps / data_collection_time_interval_in_sec/10;
		point_sensor.ConstructCumulativeFlowVector(data_collection_time_interval_in_sec,number_of_time_intervals);

		m_CorridorSensorData.DataCollectionTimeInterval_in_sec = data_collection_time_interval_in_sec;
		m_CorridorSensorData.PointSensorDataVector.push_back(point_sensor);

	}

	// step 2: construct link sensor data

	 m_CorridorSensorData.CreateLinkSensorDataFromPointSensorVector();

	//step 3: scan image to count vehicles
	for (int iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{

		LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];
		for(int t = 0; t < element.SpaceScanCountVector .size(); t++)
		{
			int timestamp  = t* element.DataCollectionTimeInterval_in_sec*10;
			int count = CountVehicles(timestamp,timestamp, element.DetectorLocalY_Downstream,element.DetectorLocalY_Upstream);
			m_CorridorSensorData.LinkSensorDataVector[iLink].SpaceScanCountVector[t] = count;
			if(count)
				TRACE("");
			
			m_CorridorSensorData.LinkSensorDataVector[iLink].DensityVector[t] = count*1.0/element.DetectorSpacing_in_miles;
		}

	}

	// construct downstream cumuative flow counts
	m_CorridorSensorData.LocalY_Upstream = m_LocalYLowerBound + boundary_offset;
	m_CorridorSensorData.LocalY_Downstream = m_LocalYUpperBound - boundary_offset;

	std::vector<int> PassingTimeStamp;

	DetectorLocalY = m_CorridorSensorData.LocalY_Downstream;
		for(int v=0; v< m_NumberOfVehicles; v+=1)  // for all vehicles
		{
			for(int t= m_VehicleDataList[v].StartTime; t<m_VehicleDataList[v].EndTime; t+=1)  // for all valid timestamp
			{
				if(m_VehicleSnapshotAry[v][t].LaneID ==m_SelectLaneID)   // for passing lane
				{
					if(m_VehicleSnapshotAry[v][t].LocalY <= DetectorLocalY && DetectorLocalY < m_VehicleSnapshotAry[v][t+1].LocalY )
					{
						PassingTimeStamp.push_back(t);
					}
				}
			}
		}
		std::sort(PassingTimeStamp.begin(), PassingTimeStamp.end(), compare_timestamp);
 
		m_CorridorSensorData.ConstructCumulativeFlowCount(PassingTimeStamp,m_NumberOfTimeSteps/10);
  
		m_CorridorSensorData.EstimateSpaceScanCountVectorForAllLinks();



		//Output_NGSIM_Matrix("cumulative_count");
}
void CTimeSpaceView::ConstructCellBasedDensityProfile(float SensorSpacing, float boundary_offset, int data_collection_time_interval_in_sec)
{
	//sdzhao test StartLocalYLane and EndLocalYLane
	//ofstream ofile("log.ext", ios::out);
	
	if (m_NumberOfTimeSteps == 0)
		m_NumberOfTimeSteps = 1440;
	m_NumberOfVehicles_lane = m_NumberOfTimeSteps / 20;

	m_VehicleSnapshotAry_Lane = Allocate3DDynamicArray<float>(_MAX_LANE_SIZE, m_NumberOfVehicles_lane, m_NumberOfTimeSteps + 1);

	m_VehicleStartTimeAry_Lane = AllocateDynamicArray<int>(_MAX_LANE_SIZE, m_NumberOfVehicles_lane);
	m_VehicleEndTimeAry_Lane = AllocateDynamicArray<int>(_MAX_LANE_SIZE, m_NumberOfVehicles_lane);

	float FirstTimeStamp = 999999;
	int FirstVehicleID = 0;
	for (int v = 0; v < m_NumberOfVehicles; v += 1)
	{
		for (int t = m_VehicleDataList[v].StartTime; t <= m_VehicleDataList[v].EndTime; t += 1)
		{
			//ofile << v << "," << t << "," << m_VehicleSnapshotAry[v][t].LaneID << endl;
			if (m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID)  //LaneID starts from 0
			{
				if (t < FirstTimeStamp)
				{
					FirstTimeStamp = t;
					FirstVehicleID = v;
				}
			}
		}
	}
	StartTimeLane[m_SelectLaneID] = -1;
	EndTimeLane[m_SelectLaneID] = -1;

	for (int t = m_VehicleDataList[FirstVehicleID].StartTime; t <= m_VehicleDataList[FirstVehicleID].EndTime; t += 1)
	{

		if (StartTimeLane[m_SelectLaneID] != -1 && m_VehicleSnapshotAry[FirstVehicleID][t].LaneID != m_SelectLaneID)
		{
			break; // switch to another link!
		}

		if (m_VehicleSnapshotAry[FirstVehicleID][t].LaneID == m_SelectLaneID)  //LaneID starts from 0
		{

			if (StartTimeLane[m_SelectLaneID] == -1) // has not been initialized
			{
				StartTimeLane[m_SelectLaneID] = t;
				StartLocalYLane[m_SelectLaneID] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY;
			}

			EndTimeLane[m_SelectLaneID] = t;  // keep assinging time stamp value
			EndLocalYLane[m_SelectLaneID] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY;

			// record trajactory for the first vehicle
			m_VehicleSnapshotAry_Lane[m_SelectLaneID][0][t] = m_VehicleSnapshotAry[FirstVehicleID][t].LocalY;
			// simulated data array (lane by lane)     original data array

		}
	}

	// set up the position for upstream and downstream (single) loop detectors, can be extended to double loops
	StartLocalYLane[m_SelectLaneID] += 50.0f; // move up by 50 feet
	EndLocalYLane[m_SelectLaneID] -= 50.0f; // move down by 50 feet

	// find passing time stamps of vehicles

	std::vector<int> StartPassingTimeStamp;
	std::vector<int> EndPassingTimeStamp;

	for (int v = 0; v < m_NumberOfVehicles; v += 1)
	{
		for (int t = m_VehicleDataList[v].StartTime; t < m_VehicleDataList[v].EndTime; t += 1)
		{
			if (m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID)  //LaneID starts from 0
			{

				if (m_VehicleSnapshotAry[v][t].LocalY <= StartLocalYLane[m_SelectLaneID] && StartLocalYLane[m_SelectLaneID] < m_VehicleSnapshotAry[v][t + 1].LocalY)
				{
					StartPassingTimeStamp.push_back(t);
				}

				if (m_VehicleSnapshotAry[v][t].LocalY <= EndLocalYLane[m_SelectLaneID] && EndLocalYLane[m_SelectLaneID] < m_VehicleSnapshotAry[v][t + 1].LocalY)
				{
					EndPassingTimeStamp.push_back(t);
				}

			}

		}
	}

	std::sort(StartPassingTimeStamp.begin(), StartPassingTimeStamp.end(), compare_timestamp);
	std::sort(EndPassingTimeStamp.begin(), EndPassingTimeStamp.end(), compare_timestamp);

	// here we assume the first leader vehicle is 0

	VehicleSizeLane[m_SelectLaneID] = min(StartPassingTimeStamp.size(), EndPassingTimeStamp.size());
	for (int v = 0; v < VehicleSizeLane[m_SelectLaneID]; v++)  // 1 starts from the first follower.
	{
		m_VehicleStartTimeAry_Lane[m_SelectLaneID][v] = StartPassingTimeStamp[v];
		m_VehicleEndTimeAry_Lane[m_SelectLaneID][v] = EndPassingTimeStamp[v];
		TRACE("no.%d, %d, %d\n", v, StartPassingTimeStamp[v], EndPassingTimeStamp[v]);
	}

	//


	//float SensorSpacing = 300.0f;  // 0.1 mile
	// step 1: point sensor data

	//float boundary_offset = 50;	

	float DetectorLocalY;
	for (DetectorLocalY = m_LocalYLowerBound + boundary_offset; DetectorLocalY <= m_LocalYUpperBound - boundary_offset; DetectorLocalY += SensorSpacing) // - 50 and +50 at boundary.
	{
		// find passing time stamps of vehicles

		std::vector<int> StartPassingTimeStamp;
		std::vector<int> EndPassingTimeStamp;
		std::map<int, std::vector<double>> observed_spd_within_sensor_time_rectangle;

		for (int v = 0; v < m_NumberOfVehicles; v += 1)  // for all vehicles
		{
			for (int t = m_VehicleDataList[v].StartTime; t < m_VehicleDataList[v].EndTime; t += 1)  // for all valid timestamp
			{
				if (m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID)   // for passing lane
				{
					if (m_VehicleSnapshotAry[v][t].LocalY <= DetectorLocalY && DetectorLocalY < m_VehicleSnapshotAry[v][t + 1].LocalY)
					{
						StartPassingTimeStamp.push_back(t);
						//sdzhao get observed speed from file
						observed_spd_within_sensor_time_rectangle[t].push_back(m_VehicleSnapshotAry[v][t].Speed_mph);
						//sdzhao: get elapsed time of vehicles 
						
						//double elapsed_time = m_VehicleEndTimeAry_Lane[m_SelectLaneID][v] - m_VehicleStartTimeAry_Lane[m_SelectLaneID][v];
					}
				}
				//StartLocalYLane;
			}
		}
		if (!StartPassingTimeStamp.size())
			continue;

		std::sort(StartPassingTimeStamp.begin(), StartPassingTimeStamp.end(), compare_timestamp);


		PointSensorData point_sensor;

		point_sensor.DetectorLocalY = DetectorLocalY;
		point_sensor.PassingTimeStampVector = StartPassingTimeStamp;

		//int data_collection_time_interval_in_sec = 30;
		int number_of_time_intervals = m_NumberOfTimeSteps / data_collection_time_interval_in_sec / 10;
		point_sensor.ConstructCumulativeFlowVector(data_collection_time_interval_in_sec, number_of_time_intervals);
		point_sensor.observed_spd_within_sensor_time_rectangle = observed_spd_within_sensor_time_rectangle;

		m_CorridorSensorData.DataCollectionTimeInterval_in_sec = data_collection_time_interval_in_sec;
		m_CorridorSensorData.PointSensorDataVector.push_back(point_sensor);

		
	}

	// step 2: construct link sensor data

	m_CorridorSensorData.CreateLinkSensorDataFromPointSensorVector();
	ostringstream &ostr = m_CorridorSensorData.out_FD_variables;
	ostr << "Sensor,time,Instantaneous_density(veh/mile),Edie_definition_density(veh/mile),Point_harmonic_speed(mph),Time_mean_speed(mph),total_distance_time_harmonic_speed(mph),obserbed_flow(veh/hour),Point_harmonic_flow(veh/hour),Time_mean_flow(veh/hour),total_distance_time_harmonic__flow(veh/hour)" << endl;
	//step 3: scan image to count vehicles
	//
	map<int, vector<map<int,int>>> StartPassingTimeStampAtSensorIndex_VehIndex;
	
	map<int, vector<map<int, int>>> EndPassingTimeStampAtSensorIndex_VehIndex;
	
	//
	for (int iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{
		

		double ilink_sensor_local_y_endpoint = m_CorridorSensorData.LinkSensorDataVector[iLink].DetectorLocalY_Downstream;
		double ilink_sensor_local_y_startpoint = m_CorridorSensorData.LinkSensorDataVector[iLink].DetectorLocalY_Upstream;

		LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];
		StartPassingTimeStampAtSensorIndex_VehIndex[iLink].resize(element.SpaceScanCountVector.size());
		EndPassingTimeStampAtSensorIndex_VehIndex[iLink].resize(element.SpaceScanCountVector.size());
		//
		for (int v = 0; v < m_NumberOfVehicles; v += 1)
		{
			//
			
			for (int t = m_VehicleDataList[v].StartTime; t<m_VehicleDataList[v].EndTime; t += 1)
			{			
				int t_id = (t / 10) / element.DataCollectionTimeInterval_in_sec;
				if (t_id > element.SpaceScanCountVector.size() - 1)
					continue; 
				//bool start_valid = false, end_valid = false;

				if (m_VehicleSnapshotAry[v][t].LocalY <= ilink_sensor_local_y_startpoint&& ilink_sensor_local_y_startpoint < m_VehicleSnapshotAry[v][t + 1].LocalY)
				{
					StartPassingTimeStampAtSensorIndex_VehIndex[iLink][t_id][v] = t; //.push_back(t);
	
				}

				if (m_VehicleSnapshotAry[v][t].LocalY <= ilink_sensor_local_y_endpoint  &&  ilink_sensor_local_y_endpoint< m_VehicleSnapshotAry[v][t + 1].LocalY)
				{//
					EndPassingTimeStampAtSensorIndex_VehIndex[iLink][t_id][v] = t;// .push_back(t);				
				}

			}
		}
		//

		for (int t = 0; t < element.SpaceScanCountVector.size(); t++)
		{
			int timestamp = t* element.DataCollectionTimeInterval_in_sec * 10;
			int count = CountVehicles(timestamp, timestamp, element.DetectorLocalY_Downstream, element.DetectorLocalY_Upstream);
			//sdzhao
			double space_mean_speed = CalculateSpaceMeanSpeedHarmonic(timestamp, timestamp, element);
			int end_timestamp = (t+1)* element.DataCollectionTimeInterval_in_sec * 10;
			double avg_rect_vehicle_count = 0.0;// CountVehicles(timestamp, end_timestamp, element.DetectorLocalY_Downstream, element.DetectorLocalY_Upstream);
			for (int tt = timestamp; tt < end_timestamp; tt++){
				avg_rect_vehicle_count += CountVehicles(tt, tt+1, element.DetectorLocalY_Downstream, element.DetectorLocalY_Upstream);
			}
			avg_rect_vehicle_count = avg_rect_vehicle_count / (end_timestamp - timestamp);

			double point_harmonic_speed = CalculateSpaceMeanSpeedHarmonic(timestamp, end_timestamp, element);
			
			//
			double time_mean_spd = CalculateTimeMeanSpeed(timestamp, end_timestamp, element);
			float sensor_spacing = m_DataCollectionSensorSpacing_in_feet;
			float edie_density = -1.0;
			double total_dist_time_harmonic_speed = CalculateSpaceMeanSpeedElapsedTime(iLink, t, sensor_spacing, StartPassingTimeStampAtSensorIndex_VehIndex, EndPassingTimeStampAtSensorIndex_VehIndex, edie_density);
// 			double avg_ElapsedTime = 0.0;
// 			for (int vi = 0; vi < StartPassingTimeStampAtSensorIndex[iLink][t].size(); vi++){
// 			
// 				double ElapsedTime = EndPassingTimeStampAtSensorIndex[iLink][t][vi] - StartPassingTimeStampAtSensorIndex[iLink][t][vi];
// 				avg_ElapsedTime += ElapsedTime;
// 			}
// 			avg_ElapsedTime /= StartPassingTimeStampAtSensorIndex[iLink][t].size();
// 			double space_mean_speed_elapsed_time = (m_DataCollectionSensorSpacing_in_feet / 5280 )/ (avg_ElapsedTime / 36000);
			//


			m_CorridorSensorData.LinkSensorDataVector[iLink].SpaceScanCountVector[t] = count;
			if (count)
				TRACE("");

			m_CorridorSensorData.LinkSensorDataVector[iLink].DensityVector[t] = count*1.0 / element.DetectorSpacing_in_miles;

			float obserbed_flow = element.GetFlowCount(timestamp/10) * (3600 / m_DataCollectionTimeInternval_in_sec);
			double rect_density = m_CorridorSensorData.LinkSensorDataVector[iLink].DensityVector[t];
			double instantaneous_avg_rect_density = avg_rect_vehicle_count*1.0 / element.DetectorSpacing_in_miles;
			double rect_flow = point_harmonic_speed * rect_density;
			double point_harmonic_flow = instantaneous_avg_rect_density * point_harmonic_speed;
			if (point_harmonic_flow > 0.0){
				ostr << iLink << ","
					<< t << ","
					//<< avg_rect_vehicle_count << ","
					<< instantaneous_avg_rect_density << ","
					<< ((edie_density < 0.0 || edie_density > 280.0) ? " " : to_string(edie_density)) << ","
					<< point_harmonic_speed << ","
					<< time_mean_spd << ","
					<< ((total_dist_time_harmonic_speed < 0.0 || total_dist_time_harmonic_speed > 65.0) ? " " : to_string(total_dist_time_harmonic_speed)) << ","
					//<< space_mean_speed << "," 
					//<< rect_density << ","			
					//<< rect_flow  << ","
					//<< count * (3600 / m_DataCollectionTimeInternval_in_sec) << ","
					<< obserbed_flow << ","
					<< point_harmonic_flow << ","
					<< time_mean_spd*instantaneous_avg_rect_density << ","
					<< ((total_dist_time_harmonic_speed < 0.0 || total_dist_time_harmonic_speed > 65.0) ? " " : to_string(total_dist_time_harmonic_speed*instantaneous_avg_rect_density)) << endl;
			}
		}

	}

	// construct downstream cumuative flow counts
	m_CorridorSensorData.LocalY_Upstream = m_LocalYLowerBound + boundary_offset;
	m_CorridorSensorData.LocalY_Downstream = m_LocalYUpperBound - boundary_offset;

	std::vector<int> PassingTimeStamp;

	DetectorLocalY = m_CorridorSensorData.LocalY_Downstream;
	for (int v = 0; v < m_NumberOfVehicles; v += 1)  // for all vehicles
	{
		for (int t = m_VehicleDataList[v].StartTime; t < m_VehicleDataList[v].EndTime; t += 1)  // for all valid timestamp
		{
			if (m_VehicleSnapshotAry[v][t].LaneID == m_SelectLaneID)   // for passing lane
			{
				if (m_VehicleSnapshotAry[v][t].LocalY <= DetectorLocalY && DetectorLocalY < m_VehicleSnapshotAry[v][t + 1].LocalY)
				{
					PassingTimeStamp.push_back(t);
				}


			}
		}
	}
	std::sort(PassingTimeStamp.begin(), PassingTimeStamp.end(), compare_timestamp);

	m_CorridorSensorData.ConstructCumulativeFlowCount(PassingTimeStamp, m_NumberOfTimeSteps / 10);

	m_CorridorSensorData.EstimateSpaceScanCountVectorForAllLinks();

	

	//Output_NGSIM_Matrix("cumulative_count");
}

void CTimeSpaceView::ReadNGSIMScenarioFile(){
	ifstream NGSIM_scenario("NGSIM_Scenario.csv", fstream::in);
	string line;
	if (NGSIM_scenario.is_open()){
		int iline = 0;
		vector<string> tokens;
		while (NGSIM_scenario.good())
		{
			getline(NGSIM_scenario, line);
			iline = iline + 1;
			if (line != ""){
				string_split(tokens, line, ",");
				if (iline == 1)
				{
					m_DataCollectionTimeInternval_in_sec = stoi(tokens[1]);
				}
				else if (iline == 2)
				{
					m_DataCollectionSensorSpacing_in_feet = stoi(tokens[1]);
				}
				else if (iline == 3)
				{
					m_bOutputVissimToNGSIM = stoi(tokens[1])?true:false;
				}
				else if (iline == 4)
				{
					m_45_degree_velocity = stof(tokens[1]);
				}
				else if (iline == 5)
				{
					number_of_total_vehicles_output = stoi(tokens[1]);
				}
				else if (iline == 6)
				{
					num_deduct_vehicles = stoi(tokens[1]);
				}
			}
		}
		NGSIM_scenario.close();
	}
};

void CTimeSpaceView::OnNgsimmenuExportcumulativeflowcountdata()
{
	ConstructCellBasedDensityProfile();
}

void CTimeSpaceView::OnNgsimmenuShowcalculatedcumulativeflowcountanddensity()
{
	m_bShowCumulativeFlowCount = !m_bShowCumulativeFlowCount;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimmenuShowcalculatedcumulativeflowcountanddensity(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowCumulativeFlowCount);
}

void CTimeSpaceView::OnNgsimmenuShowspacetimeContour()
{
	m_bShowDensityContour = !m_bShowDensityContour;
	Invalidate();
}

void CTimeSpaceView::OnNgsimmenuExportFDStreamVariables()
{
	m_bExportFDStreamVariables = !m_bExportFDStreamVariables;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimmenuShowspacetimeContour(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowDensityContour);
}

void CTimeSpaceView::OnUpdateNgsimmenuExportFDStreamVariables(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bExportFDStreamVariables);
}
void CTimeSpaceView::DrawPDFPlot(CDC* pDC,CRect PlotRect, std::vector<int> ValueVector1, std::vector<int> ValueVector2, int MaxBinValue, int BinSize, int MaxFrequency)
{

	std::vector<int> FrequencyCount1;
	std::vector<int> FrequencyCount2;

	
	unsigned int i;
	// initialization
	for(i = 0; i<= BinSize; i++)
	{
		FrequencyCount1.push_back (0);
		FrequencyCount2.push_back (0);
	
	}

	unsigned int j;
	int bin_interval  = MaxBinValue / BinSize +1;

	for(j = 0; j < ValueVector1.size(); j++)
	{
	 int bin_no = ValueVector1[j] / bin_interval;
	 if(bin_no < FrequencyCount1.size())
	 {
	 FrequencyCount1[bin_no]++;  // count frequency
	 }
	}

	for(j = 0; j < ValueVector2.size(); j++)
	{
	 int bin_no = ValueVector2[j] / bin_interval;

	 if(bin_no < FrequencyCount2.size())
	 {
		 FrequencyCount2[bin_no]++;  // count frequency
	 }
	}


	// draw figures;

	pDC->FillSolidRect (PlotRect,RGB(192,192,192));  //grey brush
	

	float x_axis_unit =  PlotRect.Width ()*1.0f/ MaxBinValue;
	float y_axis_unit =  PlotRect.Height() *1.0f/ MaxFrequency;

	int k;

	CPen Ground_truth_Pen(PS_SOLID,1,RGB(255,0,0));
	CPen Estimated_Pen(PS_SOLID,1,RGB(0,0,255));

	pDC->SelectObject (&Ground_truth_Pen);
	for(k = 0; k <=BinSize; k++)
	{
	 int x =  PlotRect.left + k*1.0f*x_axis_unit;
	 int y =  PlotRect.bottom - FrequencyCount1[k]*y_axis_unit;

	 if(k==0)
	 {
		 pDC->MoveTo (x,y);
	 }else
	 {
		 pDC->LineTo (x,y);
	 }

	}

	pDC->SelectObject (&Estimated_Pen);

	for(k = 0; k <=BinSize; k++)
	{
	 int x =  PlotRect.left + k*1.0f*x_axis_unit;
	 int y =  PlotRect.bottom - FrequencyCount2[k]*y_axis_unit;

	 if(k==0)
	 {
		 pDC->MoveTo (x,y);
	 }else
	 {
		 pDC->LineTo (x,y);
	 }

	}


}

void CTimeSpaceView::SetupMaxFrequencyAndBinValue()
{
	// setup m_MaxBinValue
	m_MaxFrequency  = 0;
	m_MaxBinValue = 100;  // manual setup

	unsigned int iLink;
/*	
	for (iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{
	std::vector<int> ValueVector1;
	
	ValueVector1 = m_CorridorSensorData.PointSensorDataVector [iLink].PassingHeadwayVector ;  // upstream sensor

	std::sort(ValueVector1.begin(), ValueVector1.end());
	m_MaxBinValue = max(m_MaxBinValue,ValueVector1[ValueVector1.size()-1]);

	}
*/
	// setup m_MaxFrequency
	for (iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{
	unsigned int i;
	std::vector<int> FrequencyCount1;

	// initialization
	for(i = 0; i<= m_BinSize; i++)
	{
		FrequencyCount1.push_back (0);
	}

	int bin_interval  = m_MaxBinValue / m_BinSize+1;

	std::vector<int> ValueVector1;
	
	ValueVector1 = m_CorridorSensorData.PointSensorDataVector [iLink].PassingHeadwayVector ;  // upstream sensor

	unsigned int j;
	for(j = 0; j < ValueVector1.size(); j++)
	{

	 int bin_no = ValueVector1[j] / bin_interval;

		 if(bin_no < FrequencyCount1.size())
		 {
		 FrequencyCount1[bin_no]++;  // count frequency
		 }else
		 {
//		 FrequencyCount1[FrequencyCount1.size()-1]++;  // count frequency for out of bound value
		 }
	}


	std::sort(FrequencyCount1.begin(), FrequencyCount1.end());
	
	m_MaxFrequency = max(m_MaxFrequency, FrequencyCount1[FrequencyCount1.size()-1]);

	}
	
}
void CTimeSpaceView::OnNgsimmenuShowcdfplot()
{
	m_bShowPDFPlot = !m_bShowPDFPlot;
	Invalidate();
}

void CTimeSpaceView::OnUpdateNgsimmenuShowcdfplot(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowPDFPlot);
	
}



void CTimeSpaceView::DrawPDFPlotsForAllLinks(CDC* pDC,eNGSIMDisplayMode MOEType,CRect PlotRect)
{

	int iLink = 0;

	if(m_bShowPDFPlot)
	{
	SetupMaxFrequencyAndBinValue();
	}

	for (iLink = 0; iLink < m_CorridorSensorData.LinkSensorDataVector.size(); iLink++)
	{
		LinkSensorData element = m_CorridorSensorData.LinkSensorDataVector[iLink];
		int YFrom = PlotRect.bottom - (int)((element.DetectorLocalY_Upstream*m_UnitDistance)+0.50);
		int YTo= PlotRect.bottom - (int)((element.DetectorLocalY_Downstream *m_UnitDistance)+0.50);

		if(m_bShowPDFPlot)
		{
			CRect CDFPlotRect;
			
			CDFPlotRect.top  =  YTo;
			CDFPlotRect.bottom = YFrom;
			CDFPlotRect.right = PlotRect.right ;
			CDFPlotRect.left = PlotRect.right - 200;

			std::vector<int> ValueVector1;
			std::vector<int> ValueVector2;

			ValueVector1 = m_CorridorSensorData.PointSensorDataVector [iLink].PassingHeadwayVector ;  // upstream sensor
			DrawPDFPlot(pDC,CDFPlotRect,ValueVector1,ValueVector2,m_MaxBinValue,m_BinSize,m_MaxFrequency);
		
		}


		}

}
