/* AGEFrame_cpp/Units.cpp */

#include "../AGE_Frame.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
#include <cctype>
using std::tolower;

void AGE_Frame::OnUnitSubList(wxCommandEvent& Event)
{
	ListUnits(Units_Civs_List->GetSelection());	// List units by selected civ.
}

string AGE_Frame::GetUnitName(short UnitID, short UnitCivID, bool Filter)
{
	string Name = "";
	if(Filter == false)
	{
		if(LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName) != "")
		{
			Name = LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName);
		}
		else if(GenieFile->Civs[UnitCivID].Units[UnitID].Name != "")
		{
			Name = GenieFile->Civs[UnitCivID].Units[UnitID].Name;
		}
		else
		{
			Name = "New Unit";
		}
	}
	else
	{
		short Selection[2];
		for(short loop = 0;loop < 2;loop++)
		Selection[loop] = Units_Units_SearchFilters[loop]->GetSelection();
		
		if(Selection[0] == 0)	// Lang DLL Name
		{
			Name = lexical_cast<string>(GenieFile->Civs[UnitCivID].UnitPointers[UnitID])+" ";
			if(LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName) != "")	// Other than empty.
			{
				Name += LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName);
			}
			else if(GenieFile->Civs[UnitCivID].Units[UnitID].Name != "")
			{
				Name += GenieFile->Civs[UnitCivID].Units[UnitID].Name;	// If lang DLL is empty, use internal name.
			}
			else
			{
				Name += "New Unit";
			}
		}
		else if(Selection[0] == 1)	// Internal Name
		{
			Name = lexical_cast<string>(GenieFile->Civs[UnitCivID].UnitPointers[UnitID])+" ";
			if(GenieFile->Civs[UnitCivID].Units[UnitID].Name != "")
			{
				Name += GenieFile->Civs[UnitCivID].Units[UnitID].Name;
			}
			else
			{
				Name += "New Unit";
			}
		}
		else
		{
			for(short loop = 0;loop < 2;loop++)
			{
				if(Selection[loop] == 2)	// Type
				{
					Name += "T ";
					Name += lexical_cast<string>((short)GenieFile->Civs[UnitCivID].Units[UnitID].getType());
				}
				else if(Selection[loop] == 3)	// Class
				{
					Name += "C ";
					Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].Class);
				}
				else if(Selection[loop] == 4)	// Pointer
				{
					Name = lexical_cast<string>(GenieFile->Civs[UnitCivID].UnitPointers[UnitID]);
				}
				Name += ", ";
				if(Selection[loop+1] < 2) break;
			}
			
			if(LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName) != "")
			{
				Name += LanguageDllString(GenieFile->Civs[UnitCivID].Units[UnitID].LanguageDllName);
			}
			else if(GenieFile->Civs[UnitCivID].Units[UnitID].Name != "")
			{
				Name += GenieFile->Civs[UnitCivID].Units[UnitID].Name;
			}
			else
			{
				Name += "New Unit";
			}
		}
	}
	return Name;
}

void AGE_Frame::OnUnitsSearch(wxCommandEvent& Event)
{
	short Selection = Units_Civs_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		ListUnits(Selection);
	}
}

// Following void thing is a series of lists for user interface.
void AGE_Frame::ListUnits(short UnitCivID)
{

	string Name;
	SearchText = wxString(Units_Units_Search->GetValue()).Lower();
	ExcludeText = wxString(Units_Units_Search_R->GetValue()).Lower();
	string CompareText;
	for(short loop = 0;loop < 2;loop++)
	{
		if(Units_Units_UseAnd[loop]->GetValue() == true)
		UseAnd[loop] = true; else UseAnd[loop] = false;
	}
	
	if(!Units_Civs_List->IsEmpty())
	{
		Units_Civs_List->Clear();
	}
	if(UnitCivID == -1)
	{
		UnitCivID = 0;
	}
	for(short loop = 0;loop < GenieFile->Civs.size();loop++)
	{
		Name = lexical_cast<string>(loop);
		Name += " - ";
		Name += GetCivName(loop);
		Units_Civs_List->Append(Name);
	}
	Units_Civs_List->SetSelection(UnitCivID);
	
	short Selection = Units_Units_List->GetSelection();
	short UnitIDs[18];
	UnitIDs[0] = Units_ComboBox_DeadUnitID->GetSelection();
	UnitIDs[1] = Units_ComboBox_ProjectileUnitID->GetSelection();
	for(short loop = 0;loop < 2;loop++)
	UnitIDs[loop+2] = Units_ComboBox_DropSite[loop]->GetSelection();
	UnitIDs[4] = Units_ComboBox_AttackMissileDuplicationUnit->GetSelection();
	UnitIDs[5] = Units_ComboBox_TrackingUnit->GetSelection();
	UnitIDs[6] = Units_ComboBox_TrainLocationID->GetSelection();
	UnitIDs[7] = Units_ComboBox_StackUnitID->GetSelection();
	for(short loop = 0;loop < 4;loop++)
	UnitIDs[loop+8] = Units_ComboBox_AnnexUnit[loop]->GetSelection();
	UnitIDs[12] = Units_ComboBox_HeadUnit->GetSelection();
	UnitIDs[13] = Units_ComboBox_TransformUnit->GetSelection();
	UnitIDs[14] = Research_ComboBox_ResearchLocation->GetSelection();
	UnitIDs[15] = Effects_ComboBox_UnitsA->GetSelection();
	UnitIDs[16] = Effects_ComboBox_UnitsB->GetSelection();
	UnitIDs[17] = UnitCommands_ComboBox_UnitID->GetSelection();
	short UnitIDloop[30];
	for(short loop = 0;loop < 30;loop++)
	UnitIDloop[loop] = Terrains_ComboBox_TerrainUnitID[loop]->GetSelection();

	if(!Units_Units_List->IsEmpty())
	{
		Units_Units_List->Clear();
	}
	if(!Units_ComboBox_DeadUnitID->IsEmpty())
	{
		Units_ComboBox_DeadUnitID->Clear();
	}
	if(!Units_ComboBox_ProjectileUnitID->IsEmpty())
	{
		Units_ComboBox_ProjectileUnitID->Clear();
	}
	if(!Units_ComboBox_AttackMissileDuplicationUnit->IsEmpty())
	{
		Units_ComboBox_AttackMissileDuplicationUnit->Clear();
	}
	for(short loop = 0;loop < 2;loop++)
	if(!Units_ComboBox_DropSite[loop]->IsEmpty())
	{
		Units_ComboBox_DropSite[loop]->Clear();
	}
	if(!Units_ComboBox_TrackingUnit->IsEmpty())
	{
		Units_ComboBox_TrackingUnit->Clear();
	}
	if(!Units_ComboBox_TrainLocationID->IsEmpty())
	{
		Units_ComboBox_TrainLocationID->Clear();
	}
	if(!Units_ComboBox_StackUnitID->IsEmpty())
	{
		Units_ComboBox_StackUnitID->Clear();
	}
	for(short loop = 0;loop < 4;loop++)
	if(!Units_ComboBox_AnnexUnit[loop]->IsEmpty())
	{
		Units_ComboBox_AnnexUnit[loop]->Clear();
	}
	if(!Units_ComboBox_HeadUnit->IsEmpty())
	{
		Units_ComboBox_HeadUnit->Clear();
	}
	if(!Units_ComboBox_TransformUnit->IsEmpty())
	{
		Units_ComboBox_TransformUnit->Clear();
	}
	if(!Research_ComboBox_ResearchLocation->IsEmpty())
	{
		Research_ComboBox_ResearchLocation->Clear();
	}
	if(!Effects_ComboBox_UnitsA->IsEmpty())
	{
		Effects_ComboBox_UnitsA->Clear();
	}
	if(!Effects_ComboBox_UnitsB->IsEmpty())
	{
		Effects_ComboBox_UnitsB->Clear();
	}
	if(!UnitCommands_ComboBox_UnitID->IsEmpty())
	{
		UnitCommands_ComboBox_UnitID->Clear();
	}
	for(short loop = 0;loop < 30;loop++)
	if(!Terrains_ComboBox_TerrainUnitID[loop]->IsEmpty())
	{
		Terrains_ComboBox_TerrainUnitID[loop]->Clear();
	}
	
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	for(short loop = 0;loop < 18;loop++)
	{
		if(UnitIDs[loop] == wxNOT_FOUND)
		{
			UnitIDs[loop] = 0;
		}
	}
	for(short loop = 0;loop < 30;loop++)
	{
		if(UnitIDloop[loop] == wxNOT_FOUND)
		{
			UnitIDloop[loop] = 0;
		}
	}
	
	Units_ComboBox_DeadUnitID->Append("-1 - None");
	Units_ComboBox_ProjectileUnitID->Append("-1 - None");
	Units_ComboBox_AttackMissileDuplicationUnit->Append("-1 - None");
	for(short loop = 0;loop < 2;loop++)
	Units_ComboBox_DropSite[loop]->Append("-1 - None");
	Units_ComboBox_TrackingUnit->Append("-1 - None");
	Units_ComboBox_TrainLocationID->Append("-1 - None");
	Units_ComboBox_StackUnitID->Append("-1 - None");
	for(short loop = 0;loop < 4;loop++)
	Units_ComboBox_AnnexUnit[loop]->Append("-1 - None");
	Units_ComboBox_HeadUnit->Append("-1 - None");
	Units_ComboBox_TransformUnit->Append("-1 - None");
	Research_ComboBox_ResearchLocation->Append("-1 - None");
	Effects_ComboBox_UnitsA->Append("-1 - None");
	Effects_ComboBox_UnitsB->Append("-1 - None");
	UnitCommands_ComboBox_UnitID->Append("-1 - None");
	for(short loop = 0;loop < 30;loop++)
	Terrains_ComboBox_TerrainUnitID[loop]->Append("-1 - None");
	
	for(short loop = 0;loop < GenieFile->Civs[UnitCivID].Units.size();loop++)
	{
		CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitName(loop, UnitCivID, true)).Lower();
		if(SearchMatches(CompareText) == true)
		{
			Name = lexical_cast<string>(loop)+" - "+GetUnitName(loop, UnitCivID, true);
			Units_Units_List->Append(Name, (void*)&GenieFile->Civs[UnitCivID].Units[loop]);
		}
		Name = lexical_cast<string>(loop)+" - "+GetUnitName(loop, 0, false);
		Units_ComboBox_DeadUnitID->Append(Name);
		Units_ComboBox_ProjectileUnitID->Append(Name);
		Units_ComboBox_AttackMissileDuplicationUnit->Append(Name);
		for(short loop = 0;loop < 2;loop++)
		Units_ComboBox_DropSite[loop]->Append(Name);
		Units_ComboBox_TrackingUnit->Append(Name);
		Units_ComboBox_TrainLocationID->Append(Name);
		Units_ComboBox_StackUnitID->Append(Name);
		for(short loop = 0;loop < 4;loop++)
		Units_ComboBox_AnnexUnit[loop]->Append(Name);
		Units_ComboBox_HeadUnit->Append(Name);
		Units_ComboBox_TransformUnit->Append(Name);
		Research_ComboBox_ResearchLocation->Append(Name);
		Effects_ComboBox_UnitsA->Append(Name);
		Effects_ComboBox_UnitsB->Append(Name);
		UnitCommands_ComboBox_UnitID->Append(Name);
		for(short loop = 0;loop < 30;loop++)
		Terrains_ComboBox_TerrainUnitID[loop]->Append(Name);
	}
	
	Units_Units_List->SetSelection(0);
	Units_Units_List->SetFirstItem(Selection - 3);
	Units_Units_List->SetSelection(Selection);
	Units_ComboBox_DeadUnitID->SetSelection(UnitIDs[0]);
	Units_ComboBox_ProjectileUnitID->SetSelection(UnitIDs[1]);
	for(short loop = 0;loop < 2;loop++)
	Units_ComboBox_DropSite[loop]->SetSelection(UnitIDs[loop+2]);// 2 ja 3
	Units_ComboBox_AttackMissileDuplicationUnit->SetSelection(UnitIDs[4]);
	Units_ComboBox_TrackingUnit->SetSelection(UnitIDs[5]);
	Units_ComboBox_TrainLocationID->SetSelection(UnitIDs[6]);
	Units_ComboBox_StackUnitID->SetSelection(UnitIDs[7]);
	for(short loop = 0;loop < 4;loop++)
	Units_ComboBox_AnnexUnit[loop]->SetSelection(UnitIDs[loop+8]);// 8 - 11
	Units_ComboBox_HeadUnit->SetSelection(UnitIDs[12]);
	Units_ComboBox_TransformUnit->SetSelection(UnitIDs[13]);
	Research_ComboBox_ResearchLocation->SetSelection(UnitIDs[14]);
	Effects_ComboBox_UnitsA->SetSelection(UnitIDs[15]);
	Effects_ComboBox_UnitsB->SetSelection(UnitIDs[16]);
	UnitCommands_ComboBox_UnitID->SetSelection(UnitIDs[17]);
	for(short loop = 0;loop < 30;loop++)
	Terrains_ComboBox_TerrainUnitID[loop]->SetSelection(UnitIDloop[loop]);
	
	for(short loop = 0;loop < 2;loop++)
	UseAnd[loop] = false;
	
	wxCommandEvent E;
	OnUnitsSelect(E);
}

void AGE_Frame::ListUnitHeads()
{
	string Name;
	short CivSelection = Units_Civs_List->GetSelection();
	short Selection = Units_UnitHeads_List->GetSelection();

	if(CivSelection == -1)
	{
		CivSelection = 0;
	}
	if(!Units_UnitHeads_List->IsEmpty())
	{
		Units_UnitHeads_List->Clear();
	}
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	for(short loop = 0;loop < GenieFile->UnitHeaders.size();loop++)
	{
		Name = lexical_cast<string>(loop);
		Name += " - ";
		Name += GetUnitName(loop, CivSelection, false);
		Units_UnitHeads_List->Append(Name, (void*)&GenieFile->UnitHeaders[loop]);
	}
	Units_UnitHeads_List->SetSelection(Selection);
}

//	This links data into user interface
void AGE_Frame::OnUnitsSelect(wxCommandEvent& Event)
{
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)	// If a unit is selected.
	{
		if(Added)
		{
			Selection = Units_Units_List->GetCount() - 1;
			Units_Units_List->SetSelection(Selection);
		}
		gdat::Unit * UnitPointer = (gdat::Unit*)Units_Units_List->GetClientData(Selection);
		UnitCivID = Units_Civs_List->GetSelection();
		UnitID = UnitPointer - (&GenieFile->Civs[UnitCivID].Units[0]);
//		wxMessageBox("Unit Pointer: "+lexical_cast<string>(GenieFile->Civs[UnitCivID].UnitPointers[UnitID]));

//	Type 10+

		short UnitType = (short)UnitPointer->getType();
		Units_Type->ChangeValue(lexical_cast<string>(UnitType));
		if(UnitType == 10)
		{
			Units_ComboBox_Type->SetSelection(1);
		}
		else if(UnitType == 20)
		{
			Units_ComboBox_Type->SetSelection(2);
		}
		else if(UnitType == 30)
		{
			Units_ComboBox_Type->SetSelection(3);
		}
		else if(UnitType == 60)
		{
			Units_ComboBox_Type->SetSelection(4);
		}
		else if(UnitType == 70)
		{
			Units_ComboBox_Type->SetSelection(5);
		}
		else if(UnitType == 80)
		{
			Units_ComboBox_Type->SetSelection(6);
		}
		else
		{
			Units_ComboBox_Type->SetSelection(0);
		}
		Units_ID1->ChangeValue(lexical_cast<string>(UnitPointer->ID1));
		Units_ID1->Container = &UnitPointer->ID1;
		Units_LanguageDllName->ChangeValue(lexical_cast<string>(UnitPointer->LanguageDllName));
		Units_LanguageDllName->Container = &UnitPointer->LanguageDllName;
		Units_DLL_LanguageDllName->SetLabel(LanguageDllString(lexical_cast<short>(Units_LanguageDllName->GetValue())));
		Units_LanguageDllCreation->ChangeValue(lexical_cast<string>(UnitPointer->LanguageDllCreation));
		Units_LanguageDllCreation->Container = &UnitPointer->LanguageDllCreation;
		Units_DLL_LanguageDllCreation->SetLabel(LanguageDllString(lexical_cast<short>(Units_LanguageDllCreation->GetValue())));
		Units_Class->ChangeValue(lexical_cast<string>(UnitPointer->Class));
		Units_Class->Container = &UnitPointer->Class;
		Units_ComboBox_Class->SetSelection(UnitPointer->Class + 1);
		Units_StandingGraphic[0]->ChangeValue(lexical_cast<string>(UnitPointer->StandingGraphic.first));
		Units_StandingGraphic[0]->Container = &UnitPointer->StandingGraphic.first;
		Units_ComboBox_StandingGraphic[0]->SetSelection(UnitPointer->StandingGraphic.first + 1);
		if(GameVersion >= 2)
		{
			Units_StandingGraphic[1]->Show(true);
			Units_ComboBox_StandingGraphic[1]->Show(true);
		
			Units_StandingGraphic[1]->ChangeValue(lexical_cast<string>(UnitPointer->StandingGraphic.second));
			Units_StandingGraphic[1]->Container = &UnitPointer->StandingGraphic.second;
			Units_ComboBox_StandingGraphic[1]->SetSelection(UnitPointer->StandingGraphic.second + 1);
		}
		else
		{
			Units_StandingGraphic[1]->Show(false);
			Units_ComboBox_StandingGraphic[1]->Show(false);
		}
		Units_DyingGraphic[0]->ChangeValue(lexical_cast<string>(UnitPointer->DyingGraphic.first));
		Units_DyingGraphic[0]->Container = &UnitPointer->DyingGraphic.first;
		Units_ComboBox_DyingGraphic[0]->SetSelection(UnitPointer->DyingGraphic.first + 1);
		Units_DyingGraphic[1]->ChangeValue(lexical_cast<string>(UnitPointer->DyingGraphic.second));
		Units_DyingGraphic[1]->Container = &UnitPointer->DyingGraphic.second;
		Units_ComboBox_DyingGraphic[1]->SetSelection(UnitPointer->DyingGraphic.second + 1);
		Units_DeathMode->ChangeValue(lexical_cast<string>((short)UnitPointer->DeathMode));
		Units_DeathMode->Container = &UnitPointer->DeathMode;
		Units_CheckBox_DeathMode->SetValue((bool)UnitPointer->DeathMode);
		Units_HitPoints->ChangeValue(lexical_cast<string>(UnitPointer->HitPoints));
		Units_HitPoints->Container = &UnitPointer->HitPoints;
		Units_LineOfSight->ChangeValue(lexical_cast<string>(UnitPointer->LineOfSight));
		Units_LineOfSight->Container = &UnitPointer->LineOfSight;
		Units_GarrisonCapacity->ChangeValue(lexical_cast<string>((short)UnitPointer->GarrisonCapacity));
		Units_GarrisonCapacity->Container = &UnitPointer->GarrisonCapacity;
		Units_SizeRadius[0]->ChangeValue(lexical_cast<string>(UnitPointer->SizeRadius.first));
		Units_SizeRadius[0]->Container = &UnitPointer->SizeRadius.first;
		Units_SizeRadius[1]->ChangeValue(lexical_cast<string>(UnitPointer->SizeRadius.second));
		Units_SizeRadius[1]->Container = &UnitPointer->SizeRadius.second;
		Units_HPBarHeight1->ChangeValue(lexical_cast<string>(UnitPointer->HPBarHeight1));
		Units_HPBarHeight1->Container = &UnitPointer->HPBarHeight1;
		Units_TrainSound[0]->ChangeValue(lexical_cast<string>(UnitPointer->TrainSound.first));
		Units_TrainSound[0]->Container = &UnitPointer->TrainSound.first;
		Units_ComboBox_TrainSound[0]->SetSelection(UnitPointer->TrainSound.first + 1);
		if(GameVersion >= 2)
		{
			Units_TrainSound[1]->Show(true);
			Units_ComboBox_TrainSound[1]->Show(true);
			
			Units_TrainSound[1]->ChangeValue(lexical_cast<string>(UnitPointer->TrainSound.second));
			Units_TrainSound[1]->Container = &UnitPointer->TrainSound.second;
			Units_ComboBox_TrainSound[1]->SetSelection(UnitPointer->TrainSound.second + 1);
		}
		else
		{
			Units_TrainSound[1]->Show(false);
			Units_ComboBox_TrainSound[1]->Show(false);
		}
		Units_DeadUnitID->ChangeValue(lexical_cast<string>(UnitPointer->DeadUnitID));
		Units_DeadUnitID->Container = &UnitPointer->DeadUnitID;
		Units_ComboBox_DeadUnitID->SetSelection(UnitPointer->DeadUnitID + 1);
		Units_PlacementMode->ChangeValue(lexical_cast<string>((short)UnitPointer->PlacementMode));
		Units_PlacementMode->Container = &UnitPointer->PlacementMode;
		Units_AirMode->ChangeValue(lexical_cast<string>((short)UnitPointer->AirMode));
		Units_AirMode->Container = &UnitPointer->AirMode;
		Units_CheckBox_AirMode->SetValue((bool)UnitPointer->AirMode);
		Units_IconID->ChangeValue(lexical_cast<string>(UnitPointer->IconID));
		Units_IconID->Container = &UnitPointer->IconID;
		Units_HideInEditor->ChangeValue(lexical_cast<string>((short)UnitPointer->HideInEditor));
		Units_HideInEditor->Container = &UnitPointer->HideInEditor;
		Units_CheckBox_HideInEditor->SetValue((bool)UnitPointer->HideInEditor);
		Units_Unknown1->ChangeValue(lexical_cast<string>(UnitPointer->Unknown1));
		Units_Unknown1->Container = &UnitPointer->Unknown1;
		if(GameVersion >= 2)
		{
			Units_Enabled->SetBackgroundColour(wxColour(210, 230, 255));
			Units_Enabled->ChangeValue(lexical_cast<string>(UnitPointer->Enabled));
		}
		else
		{
			Units_Enabled->SetBackgroundColour(wxColour(255, 235, 215));
			Units_Enabled->ChangeValue(lexical_cast<string>((short)UnitPointer->Enabled));
		}
		Units_Enabled->Container = &UnitPointer->Enabled;
		Units_CheckBox_Enabled->SetValue((bool)UnitPointer->Enabled);
		Units_PlacementBypassTerrain[0]->ChangeValue(lexical_cast<string>(UnitPointer->PlacementBypassTerrain.first));
		Units_PlacementBypassTerrain[0]->Container = &UnitPointer->PlacementBypassTerrain.first;
		Units_ComboBox_PlacementBypassTerrain[0]->SetSelection(UnitPointer->PlacementBypassTerrain.first + 1);
		Units_PlacementBypassTerrain[1]->ChangeValue(lexical_cast<string>(UnitPointer->PlacementBypassTerrain.second));
		Units_PlacementBypassTerrain[1]->Container = &UnitPointer->PlacementBypassTerrain.second;
		Units_ComboBox_PlacementBypassTerrain[1]->SetSelection(UnitPointer->PlacementBypassTerrain.second + 1);
		Units_PlacementTerrain[0]->ChangeValue(lexical_cast<string>(UnitPointer->PlacementTerrain.first));
		Units_PlacementTerrain[0]->Container = &UnitPointer->PlacementTerrain.first;
		Units_ComboBox_PlacementTerrain[0]->SetSelection(UnitPointer->PlacementTerrain.first + 1);
		Units_PlacementTerrain[1]->ChangeValue(lexical_cast<string>(UnitPointer->PlacementTerrain.second));
		Units_PlacementTerrain[1]->Container = &UnitPointer->PlacementTerrain.second;
		Units_ComboBox_PlacementTerrain[1]->SetSelection(UnitPointer->PlacementTerrain.second + 1);
		Units_EditorRadius[0]->ChangeValue(lexical_cast<string>(UnitPointer->EditorRadius.first));
		Units_EditorRadius[0]->Container = &UnitPointer->EditorRadius.first;
		Units_EditorRadius[1]->ChangeValue(lexical_cast<string>(UnitPointer->EditorRadius.second));
		Units_EditorRadius[1]->Container = &UnitPointer->EditorRadius.second;
		Units_BuildingMode->ChangeValue(lexical_cast<string>((short)UnitPointer->BuildingMode));
		Units_BuildingMode->Container = &UnitPointer->BuildingMode;
		Units_VisibleInFog->ChangeValue(lexical_cast<string>((short)UnitPointer->VisibleInFog));
		Units_VisibleInFog->Container = &UnitPointer->VisibleInFog;
		Units_CheckBox_VisibleInFog->SetValue((bool)UnitPointer->VisibleInFog);
		Units_TerrainRestriction->ChangeValue(lexical_cast<string>(UnitPointer->TerrainRestriction));
		Units_TerrainRestriction->Container = &UnitPointer->TerrainRestriction;
		Units_ComboBox_TerrainRestriction->SetSelection(UnitPointer->TerrainRestriction + 1);
		Units_FlyMode->ChangeValue(lexical_cast<string>((short)UnitPointer->FlyMode));
		Units_FlyMode->Container = &UnitPointer->FlyMode;
		Units_CheckBox_FlyMode->SetValue((bool)UnitPointer->FlyMode);
		Units_ResourceCapacity->ChangeValue(lexical_cast<string>(UnitPointer->ResourceCapacity));
		Units_ResourceCapacity->Container = &UnitPointer->ResourceCapacity;
		Units_ResourceDecay->ChangeValue(lexical_cast<string>(UnitPointer->ResourceDecay));
		Units_ResourceDecay->Container = &UnitPointer->ResourceDecay;
		for(short loop = 0;loop < 2;loop++)
		{
			Units_Unknown2[loop]->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown2[loop]));
			Units_Unknown2[loop]->Container = &UnitPointer->Unknown2[loop];
		}
		Units_InteractionMode->ChangeValue(lexical_cast<string>((short)UnitPointer->InteractionMode));
		Units_InteractionMode->Container = &UnitPointer->InteractionMode;
		Units_MinimapMode->ChangeValue(lexical_cast<string>((short)UnitPointer->MinimapMode));
		Units_MinimapMode->Container = &UnitPointer->MinimapMode;
		if(GameVersion >= 2)
		{
			Units_CommandAttribute->SetBackgroundColour(wxColour(210, 230, 255));
			Units_CommandAttribute->ChangeValue(lexical_cast<string>(UnitPointer->CommandAttribute));
		}
		else
		{
			Units_CommandAttribute->SetBackgroundColour(wxColour(255, 235, 215));
			Units_CommandAttribute->ChangeValue(lexical_cast<string>((short)UnitPointer->CommandAttribute));
		}
		Units_CommandAttribute->Container = &UnitPointer->CommandAttribute;
		Units_Unknown3->ChangeValue(lexical_cast<string>(UnitPointer->Unknown3));
		Units_Unknown3->Container = &UnitPointer->Unknown3;
		if(GameVersion <= 1)
		{
			if(ShowUnknowns) Units_Holder_Unknown3a->Show(true);
		
			Units_Unknown3a->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown3a));
			Units_Unknown3a->Container = &UnitPointer->Unknown3a;
		}
		else
		{
			Units_Holder_Unknown3a->Show(false);
		}
		Units_LanguageDllHelp->ChangeValue(lexical_cast<string>(UnitPointer->LanguageDllHelp));
		Units_LanguageDllHelp->Container = &UnitPointer->LanguageDllHelp;
		for(short loop = 0;loop < 4;loop++)
		{
			Units_HotKey[loop]->ChangeValue(lexical_cast<string>(UnitPointer->HotKey[loop]));
			Units_HotKey[loop]->Container = &UnitPointer->HotKey[loop];
		}
		Units_DLL_HotKey4->SetLabel(LanguageDllString(lexical_cast<short>(Units_HotKey[3]->GetValue())));
		Units_Unknown4->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown4));
		Units_Unknown4->Container = &UnitPointer->Unknown4;
		Units_Unknown5->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown5));
		Units_Unknown5->Container = &UnitPointer->Unknown5;
		Units_Unselectable->ChangeValue(lexical_cast<string>((short)UnitPointer->Unselectable));
		Units_Unselectable->Container = &UnitPointer->Unselectable;
		Units_CheckBox_Unselectable->SetValue((bool)UnitPointer->Unselectable);
		Units_Unknown6->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown6));
		Units_Unknown6->Container = &UnitPointer->Unknown6;
		if(GameVersion >= 2)
		{
			if(ShowUnknowns) Units_Holder_Unknown7->Show(true);
			if(ShowUnknowns) Units_Holder_Unknown8->Show(true);
		
			Units_Unknown7->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown7));
			Units_Unknown7->Container = &UnitPointer->Unknown7;
			Units_Unknown8->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown8));
			Units_Unknown8->Container = &UnitPointer->Unknown8;
		}
		else
		{
			Units_Holder_Unknown7->Show(false);
			Units_Holder_Unknown8->Show(false);
		}
		Units_SelectionMask->ChangeValue(lexical_cast<string>((short)UnitPointer->SelectionMask));
		Units_SelectionMask->Container = &UnitPointer->SelectionMask;
		if(GameVersion >= 2)
		{
			Units_Holder_SelectionShapeType->Show(true);
			
			Units_SelectionShapeType->ChangeValue(lexical_cast<string>((short)UnitPointer->SelectionShapeType));
			Units_SelectionShapeType->Container = &UnitPointer->SelectionShapeType;
		}
		else
		{
			Units_Holder_SelectionShapeType->Show(false);
		}
		Units_SelectionShape->ChangeValue(lexical_cast<string>((short)UnitPointer->SelectionShape));
		Units_SelectionShape->Container = &UnitPointer->SelectionShape;
		short Unknown9;
		if(GameVersion >= 3)
		{
			Unknown9 = 4;
			for(short loop = 2;loop < 4;loop++)
			if(ShowUnknowns) Units_Unknown9[loop]->Show(true);
		}
		else if(GameVersion < 2)
		{
			Unknown9 = 2;
			for(short loop = 2;loop < 4;loop++)
			Units_Unknown9[loop]->Show(false);
		}
		else
		{
			Unknown9 = 0;
			Units_Holder_Unknown9->Show(false);
		}
		for(short loop = 0;loop < Unknown9;loop++)
		{
			Units_Unknown9[loop]->ChangeValue(lexical_cast<string>((short)UnitPointer->Unknown9[loop]));
			Units_Unknown9[loop]->Container = &UnitPointer->Unknown9[loop];
		}
		if(GameVersion >= 2)
		{
			Units_Holder_SelectionEffect->Show(true);
			Units_Holder_EditorSelectionColour->Show(true);
			
			Units_SelectionEffect->ChangeValue(lexical_cast<string>((short)UnitPointer->SelectionEffect));
			Units_SelectionEffect->Container = &UnitPointer->SelectionEffect;
			Units_EditorSelectionColour->ChangeValue(lexical_cast<string>((short)UnitPointer->EditorSelectionColour));
			Units_EditorSelectionColour->Container = &UnitPointer->EditorSelectionColour;
		}
		else
		{
			Units_Holder_SelectionEffect->Show(false);
			Units_Holder_EditorSelectionColour->Show(false);
		}
		Units_SelectionRadius[0]->ChangeValue(lexical_cast<string>(UnitPointer->SelectionRadius.first));
		Units_SelectionRadius[0]->Container = &UnitPointer->SelectionRadius.first;
		Units_SelectionRadius[1]->ChangeValue(lexical_cast<string>(UnitPointer->SelectionRadius.second));
		Units_SelectionRadius[1]->Container = &UnitPointer->SelectionRadius.second;
		Units_HPBarHeight2->ChangeValue(lexical_cast<string>(UnitPointer->HPBarHeight2));
		Units_HPBarHeight2->Container = &UnitPointer->HPBarHeight2;
		Units_SelectionSound->ChangeValue(lexical_cast<string>(UnitPointer->SelectionSound));
		Units_SelectionSound->Container = &UnitPointer->SelectionSound;
		Units_ComboBox_SelectionSound->SetSelection(UnitPointer->SelectionSound + 1);
		Units_DyingSound->ChangeValue(lexical_cast<string>(UnitPointer->DyingSound));
		Units_DyingSound->Container = &UnitPointer->DyingSound;
		Units_ComboBox_DyingSound->SetSelection(UnitPointer->DyingSound + 1);
		Units_AttackSound->ChangeValue(lexical_cast<string>(UnitPointer->AttackSound));
		Units_AttackSound->Container = &UnitPointer->AttackSound;
		Units_ComboBox_AttackSound->SetSelection(UnitPointer->AttackSound + 1);
	//	Units_NameLength->ChangeValue(lexical_cast<string>(UnitPointer->NameLength));
	//	Units_NameLength->Container = &UnitPointer->NameLength;
		Units_Name->ChangeValue(lexical_cast<string>(UnitPointer->Name));
		Units_Name->Container = &UnitPointer->Name;
		if(GameVersion >= 4)
		{
		//	Units_Holder_NameLength2->Show(true);
			Units_Holder_Name2->Show(true);
			Units_Holder_Unitline->Show(true);
			Units_Holder_MinTechLevel->Show(true);
			
		//	Units_NameLength2->ChangeValue(lexical_cast<string>(UnitPointer->NameLength2));
		//	Units_NameLength2->Container = &UnitPointer->NameLength2;
			Units_Name2->ChangeValue(lexical_cast<string>(UnitPointer->Name2));
			Units_Name2->Container = &UnitPointer->Name2;
			Units_Unitline->ChangeValue(lexical_cast<string>(UnitPointer->Unitline));
			Units_Unitline->Container = &UnitPointer->Unitline;
			Units_MinTechLevel->ChangeValue(lexical_cast<string>((short)UnitPointer->MinTechLevel));
			Units_MinTechLevel->Container = &UnitPointer->MinTechLevel;
		}
		else
		{
		//	Units_Holder_NameLength2->Show(false);
			Units_Holder_Name2->Show(false);
			Units_Holder_Unitline->Show(false);
			Units_Holder_MinTechLevel->Show(false);
		}
		Units_ID2->ChangeValue(lexical_cast<string>(UnitPointer->ID2));
		Units_ID2->Container = &UnitPointer->ID2;
		if(GameVersion >= 2)
		{
			Units_ID3->ChangeValue(lexical_cast<string>(UnitPointer->ID3));
			Units_ID3->Container = &UnitPointer->ID3;
		}

//	Type 20+
		
		if(UnitPointer->getType() >= 20)
		{
			Units_Speed->Enable(true);
			Units_Speed->ChangeValue(lexical_cast<string>(UnitPointer->Speed));
			Units_Speed->Container = &UnitPointer->Speed;
		}
		else
		{
			Units_Speed->Enable(false);
			Units_Speed->ChangeValue("0");
		}
	
//	Type 30+

		if(GenieFile->Civs[UnitCivID].Units[UnitID].DeadFish)
		{
			for(short loop = 0;loop < 2;loop++)
			{
				Units_WalkingGraphic[loop]->Enable(true);
				Units_ComboBox_WalkingGraphic[loop]->Enable(true);
			}
			Units_RotationSpeed->Enable(true);
			Units_Unknown11->Enable(true);
			Units_TrackingUnit->Enable(true);
			Units_ComboBox_TrackingUnit->Enable(true);
			Units_TrackingUnitUsed->Enable(true);
			Units_CheckBox_TrackingUnitUsed->Enable(true);
			Units_TrackingUnitDensity->Enable(true);
			Units_Unknown12->Enable(true);
			for(short loop = 0;loop < 17;loop++)
			Units_Unknown16[loop]->Enable(true);

			Units_WalkingGraphic[0]->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->WalkingGraphic.first));
			Units_WalkingGraphic[0]->Container = &UnitPointer->DeadFish->WalkingGraphic.first;
			Units_ComboBox_WalkingGraphic[0]->SetSelection(UnitPointer->DeadFish->WalkingGraphic.first + 1);
			Units_WalkingGraphic[1]->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->WalkingGraphic.second));
			Units_WalkingGraphic[1]->Container = &UnitPointer->DeadFish->WalkingGraphic.second;
			Units_ComboBox_WalkingGraphic[1]->SetSelection(UnitPointer->DeadFish->WalkingGraphic.second + 1);
			Units_RotationSpeed->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->RotationSpeed));
			Units_RotationSpeed->Container = &UnitPointer->DeadFish->RotationSpeed;
			Units_Unknown11->ChangeValue(lexical_cast<string>((short)UnitPointer->DeadFish->Unknown11));
			Units_Unknown11->Container = &UnitPointer->DeadFish->Unknown11;
			Units_TrackingUnit->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->TrackingUnit));
			Units_TrackingUnit->Container = &UnitPointer->DeadFish->TrackingUnit;
			Units_ComboBox_TrackingUnit->SetSelection(UnitPointer->DeadFish->TrackingUnit + 1);
			Units_TrackingUnitUsed->ChangeValue(lexical_cast<string>((short)UnitPointer->DeadFish->TrackingUnitUsed));
			Units_TrackingUnitUsed->Container = &UnitPointer->DeadFish->TrackingUnitUsed;
			Units_CheckBox_TrackingUnitUsed->SetValue((bool)UnitPointer->DeadFish->TrackingUnitUsed);
			Units_TrackingUnitDensity->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->TrackingUnitDensity));
			Units_TrackingUnitDensity->Container = &UnitPointer->DeadFish->TrackingUnitDensity;
			if(GameVersion >= 2)
			{
				if(ShowUnknowns) Units_Holder_Unknown12->Show(true);
				for(short loop = 1;loop < 17;loop++)
				if(ShowUnknowns) Units_Unknown16[loop]->Show(true);
				
				Units_Unknown12->ChangeValue(lexical_cast<string>(UnitPointer->DeadFish->Unknown12));
				Units_Unknown12->Container = &UnitPointer->DeadFish->Unknown12;
				for(short loop = 0;loop < 17;loop++)
				{
					Units_Unknown16[loop]->ChangeValue(lexical_cast<string>((short)UnitPointer->DeadFish->Unknown16[loop]));
					Units_Unknown16[loop]->Container = &UnitPointer->DeadFish->Unknown16[loop];
				}
			}
			else
			{
				Units_Holder_Unknown12->Show(false);
				for(short loop = 1;loop < 17;loop++)
				Units_Unknown16[loop]->Show(false);
			}
		}
		else
		{
			for(short loop = 0;loop < 2;loop++)
			{
				Units_WalkingGraphic[loop]->Enable(false);
				Units_WalkingGraphic[loop]->ChangeValue("0");
				Units_ComboBox_WalkingGraphic[loop]->Enable(false);
				Units_ComboBox_WalkingGraphic[loop]->SetSelection(0);
			}
			Units_RotationSpeed->Enable(false);
			Units_RotationSpeed->ChangeValue("0");
			Units_Unknown11->Enable(false);
			Units_Unknown11->ChangeValue("0");
			Units_TrackingUnit->Enable(false);
			Units_TrackingUnit->ChangeValue("0");
			Units_ComboBox_TrackingUnit->Enable(false);
			Units_ComboBox_TrackingUnit->SetSelection(0);
			Units_TrackingUnitUsed->Enable(false);
			Units_TrackingUnitUsed->ChangeValue("0");
			Units_CheckBox_TrackingUnitUsed->Enable(false);
			Units_CheckBox_TrackingUnitUsed->SetValue(false);
			Units_TrackingUnitDensity->Enable(false);
			Units_TrackingUnitDensity->ChangeValue("0");
			Units_Unknown12->Enable(false);
			Units_Unknown12->ChangeValue("0");
			for(short loop = 0;loop < 17;loop++)
			{
				Units_Unknown16[loop]->Enable(false);
				Units_Unknown16[loop]->ChangeValue("0");
			}
		}
//	Type 40+

		if(GenieFile->Civs[UnitCivID].Units[UnitID].Bird)
		{
			Units_SheepConversion->Enable(true);
			Units_CheckBox_SheepConversion->Enable(true);
			Units_SearchRadius->Enable(true);
			Units_WorkRate->Enable(true);
			for(short loop = 0;loop < 2;loop++)
			{
				Units_DropSite[loop]->Enable(true);
				Units_ComboBox_DropSite[loop]->Enable(true);
			}
			Units_VillagerMode->Enable(true);
			Units_MoveSound->Enable(true);
			Units_ComboBox_MoveSound->Enable(true);
			Units_StopSound->Enable(true);
			Units_ComboBox_StopSound->Enable(true);
			Units_Unknown19->Enable(true);

			Units_SheepConversion->ChangeValue(lexical_cast<string>(UnitPointer->Bird->SheepConversion));
			Units_SheepConversion->Container = &UnitPointer->Bird->SheepConversion;
			switch(UnitPointer->Bird->SheepConversion)
			{
				case -1:
				{
					Units_CheckBox_SheepConversion->SetValue(false);
				}
				break;
				case 0:
				{
					Units_CheckBox_SheepConversion->SetValue(true);
				}
				break;
				default:
				{
					Units_CheckBox_SheepConversion->SetValue(false);
				}
			}
			Units_SearchRadius->ChangeValue(lexical_cast<string>(UnitPointer->Bird->SearchRadius));
			Units_SearchRadius->Container = &UnitPointer->Bird->SearchRadius;
			Units_WorkRate->ChangeValue(lexical_cast<string>(UnitPointer->Bird->WorkRate));
			Units_WorkRate->Container = &UnitPointer->Bird->WorkRate;
			Units_DropSite[0]->ChangeValue(lexical_cast<string>(UnitPointer->Bird->DropSite.first));
			Units_DropSite[0]->Container = &UnitPointer->Bird->DropSite.first;
			Units_ComboBox_DropSite[0]->SetSelection(UnitPointer->Bird->DropSite.first + 1);
			Units_DropSite[1]->ChangeValue(lexical_cast<string>(UnitPointer->Bird->DropSite.second));
			Units_DropSite[1]->Container = &UnitPointer->Bird->DropSite.second;
			Units_ComboBox_DropSite[1]->SetSelection(UnitPointer->Bird->DropSite.second + 1);
			Units_VillagerMode->ChangeValue(lexical_cast<string>((short)UnitPointer->Bird->VillagerMode));
			Units_VillagerMode->Container = &UnitPointer->Bird->VillagerMode;
			Units_MoveSound->ChangeValue(lexical_cast<string>(UnitPointer->Bird->MoveSound));
			Units_MoveSound->Container = &UnitPointer->Bird->MoveSound;
			Units_ComboBox_MoveSound->SetSelection(UnitPointer->Bird->MoveSound + 1);
			Units_StopSound->ChangeValue(lexical_cast<string>(UnitPointer->Bird->StopSound));
			Units_StopSound->Container = &UnitPointer->Bird->StopSound;
			Units_ComboBox_StopSound->SetSelection(UnitPointer->Bird->StopSound + 1);
			Units_Unknown19->ChangeValue(lexical_cast<string>((short)UnitPointer->Bird->Unknown19));
			Units_Unknown19->Container = &UnitPointer->Bird->Unknown19;
		}
		else
		{
			Units_SheepConversion->Enable(false);
			Units_SheepConversion->ChangeValue("0");
			Units_CheckBox_SheepConversion->Enable(false);
			Units_CheckBox_SheepConversion->SetValue(false);
			Units_SearchRadius->Enable(false);
			Units_SearchRadius->ChangeValue("0");
			Units_WorkRate->Enable(false);
			Units_WorkRate->ChangeValue("0");
			for(short loop = 0;loop < 2;loop++)
			{
				Units_DropSite[loop]->Enable(false);
				Units_DropSite[loop]->ChangeValue("0");
				Units_ComboBox_DropSite[loop]->Enable(false);
				Units_ComboBox_DropSite[loop]->SetSelection(0);
			}
			Units_VillagerMode->Enable(false);
			Units_VillagerMode->ChangeValue("0");
			Units_MoveSound->Enable(false);
			Units_MoveSound->ChangeValue("0");
			Units_ComboBox_MoveSound->Enable(false);
			Units_ComboBox_MoveSound->SetSelection(0);
			Units_StopSound->Enable(false);
			Units_StopSound->ChangeValue("0");
			Units_ComboBox_StopSound->Enable(false);
			Units_ComboBox_StopSound->SetSelection(0);
			Units_Unknown19->Enable(false);
			Units_Unknown19->ChangeValue("0");
		}

//	Type 60+

		if(GenieFile->Civs[UnitCivID].Units[UnitID].Projectile)
		{
			for(short loop = 0;loop < 2;loop++)
			Units_Unknown20[loop]->Enable(true);
			Units_Unknown21->Enable(true);
			Units_MaxRange->Enable(true);
			Units_BlastRadius->Enable(true);
			Units_ReloadTime1->Enable(true);
			Units_ProjectileUnitID->Enable(true);
			Units_ComboBox_ProjectileUnitID->Enable(true);
			Units_AccuracyPercent->Enable(true);
			Units_Unknown22->Enable(true);
			Units_Delay->Enable(true);
			for(short loop = 0;loop < 4;loop++)
			Units_GraphicDisplacement[loop]->Enable(true);
			Units_Unknown23->Enable(true);
			Units_MinRange->Enable(true);
			Units_GarrisonRecoveryRate->Enable(true);
			Units_AttackGraphic->Enable(true);
			Units_ComboBox_AttackGraphic->Enable(true);
			Units_DisplayedMeleeArmour->Enable(true);
			Units_DisplayedAttack->Enable(true);
			Units_DisplayedRange->Enable(true);
			Units_ReloadTime2->Enable(true);
			
			Attacks_Class->Enable(true);
			Attacks_ComboBox_Class->Enable(true);
			Attacks_Amount->Enable(true);
			Armors_Class->Enable(true);
			Armors_ComboBox_Class->Enable(true);
			Armors_Amount->Enable(true);

			Units_Unknown20[0]->ChangeValue(lexical_cast<string>((short)UnitPointer->Projectile->Unknown20));
			Units_Unknown20[0]->Container = &UnitPointer->Projectile->Unknown20;
			if(GameVersion >= 3)
			{
				if(ShowUnknowns) Units_Unknown20[1]->Show(true);
			
				Units_Unknown20[1]->ChangeValue(lexical_cast<string>((short)UnitPointer->Projectile->Unknown20_1));
				Units_Unknown20[1]->Container = &UnitPointer->Projectile->Unknown20_1;
			}
			else
			{
				Units_Unknown20[1]->Show(false);
			}
			Units_Unknown21->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->Unknown21));
			Units_Unknown21->Container = &UnitPointer->Projectile->Unknown21;
			Units_MaxRange->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->MaxRange));
			Units_MaxRange->Container = &UnitPointer->Projectile->MaxRange;
			Units_BlastRadius->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->BlastRadius));
			Units_BlastRadius->Container = &UnitPointer->Projectile->BlastRadius;
			Units_ReloadTime1->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->ReloadTime1));
			Units_ReloadTime1->Container = &UnitPointer->Projectile->ReloadTime1;
			Units_ProjectileUnitID->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->ProjectileUnitID));
			Units_ProjectileUnitID->Container = &UnitPointer->Projectile->ProjectileUnitID;
			Units_ComboBox_ProjectileUnitID->SetSelection(UnitPointer->Projectile->ProjectileUnitID + 1);
			Units_AccuracyPercent->ChangeValue(lexical_cast<string>((short)UnitPointer->Projectile->AccuracyPercent));
			Units_AccuracyPercent->Container = &UnitPointer->Projectile->AccuracyPercent;
			Units_Unknown22->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->Unknown22));
			Units_Unknown22->Container = &UnitPointer->Projectile->Unknown22;
			Units_Delay->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->Delay));
			Units_Delay->Container = &UnitPointer->Projectile->Delay;
			for(short loop = 0;loop < 3;loop++)
			{
				Units_GraphicDisplacement[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->GraphicDisplacement[loop]));
				Units_GraphicDisplacement[loop]->Container = &UnitPointer->Projectile->GraphicDisplacement[loop];
			}
			Units_Unknown23->ChangeValue(lexical_cast<string>((short)UnitPointer->Projectile->Unknown23));
			Units_Unknown23->Container = &UnitPointer->Projectile->Unknown23;
			Units_MinRange->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->MinRange));
			Units_MinRange->Container = &UnitPointer->Projectile->MinRange;
			if(GameVersion >= 2)
			{
				Units_Holder_GarrisonRecoveryRate->Show(true);
			
				Units_GarrisonRecoveryRate->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->GarrisonRecoveryRate));
				Units_GarrisonRecoveryRate->Container = &UnitPointer->Projectile->GarrisonRecoveryRate;
			}
			else
			{
				Units_Holder_GarrisonRecoveryRate->Show(false);
			}
			Units_AttackGraphic->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->AttackGraphic));
			Units_AttackGraphic->Container = &UnitPointer->Projectile->AttackGraphic;
			Units_ComboBox_AttackGraphic->SetSelection(UnitPointer->Projectile->AttackGraphic + 1);
			Units_DisplayedMeleeArmour->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->DisplayedMeleeArmour));
			Units_DisplayedMeleeArmour->Container = &UnitPointer->Projectile->DisplayedMeleeArmour;
			Units_DisplayedAttack->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->DisplayedAttack));
			Units_DisplayedAttack->Container = &UnitPointer->Projectile->DisplayedAttack;
			Units_DisplayedRange->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->DisplayedRange));
			Units_DisplayedRange->Container = &UnitPointer->Projectile->DisplayedRange;
			Units_ReloadTime2->ChangeValue(lexical_cast<string>(UnitPointer->Projectile->ReloadTime2));
			Units_ReloadTime2->Container = &UnitPointer->Projectile->ReloadTime2;
		}
		else
		{
			for(short loop = 0;loop < 2;loop++)
			{
				Units_Unknown20[loop]->Enable(false);
				Units_Unknown20[loop]->ChangeValue("0");
			}
			Units_Unknown21->Enable(false);
			Units_Unknown21->ChangeValue("0");
			Units_MaxRange->Enable(false);
			Units_MaxRange->ChangeValue("0");
			Units_BlastRadius->Enable(false);
			Units_BlastRadius->ChangeValue("0");
			Units_ReloadTime1->Enable(false);
			Units_ReloadTime1->ChangeValue("0");
			Units_ProjectileUnitID->Enable(false);
			Units_ProjectileUnitID->ChangeValue("0");
			Units_ComboBox_ProjectileUnitID->Enable(false);
			Units_ComboBox_ProjectileUnitID->SetSelection(0);
			Units_AccuracyPercent->Enable(false);
			Units_AccuracyPercent->ChangeValue("0");
			Units_Unknown22->Enable(false);
			Units_Unknown22->ChangeValue("0");
			Units_Delay->Enable(false);
			Units_Delay->ChangeValue("0");
			for(short loop = 0;loop < 3;loop++)
			{
				Units_GraphicDisplacement[loop]->Enable(false);
				Units_GraphicDisplacement[loop]->ChangeValue("0");
			}
			Units_Unknown23->Enable(false);
			Units_Unknown23->ChangeValue("0");
			Units_MinRange->Enable(false);
			Units_MinRange->ChangeValue("0");
			Units_GarrisonRecoveryRate->Enable(false);
			Units_GarrisonRecoveryRate->ChangeValue("0");
			Units_AttackGraphic->Enable(false);
			Units_AttackGraphic->ChangeValue("0");
			Units_ComboBox_AttackGraphic->Enable(false);
			Units_ComboBox_AttackGraphic->SetSelection(0);
			Units_DisplayedMeleeArmour->Enable(false);
			Units_DisplayedMeleeArmour->ChangeValue("0");
			Units_DisplayedAttack->Enable(false);
			Units_DisplayedAttack->ChangeValue("0");
			Units_DisplayedRange->Enable(false);
			Units_DisplayedRange->ChangeValue("0");
			Units_ReloadTime2->Enable(false);
			Units_ReloadTime2->ChangeValue("0");
			
			Attacks_Class->Enable(false);
			Attacks_ComboBox_Class->Enable(false);
			Attacks_Amount->Enable(false);
			Armors_Class->Enable(false);
			Armors_ComboBox_Class->Enable(false);
			Armors_Amount->Enable(false);
		}
		
//	Type 60 only

		if(GenieFile->Civs[UnitCivID].Units[UnitID].ProjectileOnly)
		{
			Units_StretchMode->Enable(true);
			Units_CompensationMode->Enable(true);
			Units_DropAnimationMode->Enable(true);
			Units_PenetrationMode->Enable(true);
			Units_Unknown24->Enable(true);
			Units_Unknown25->Enable(true);

			Units_StretchMode->ChangeValue(lexical_cast<string>((short)UnitPointer->ProjectileOnly->StretchMode));
			Units_StretchMode->Container = &UnitPointer->ProjectileOnly->StretchMode;
			Units_CompensationMode->ChangeValue(lexical_cast<string>((short)UnitPointer->ProjectileOnly->CompensationMode));
			Units_CompensationMode->Container = &UnitPointer->ProjectileOnly->CompensationMode;
			Units_DropAnimationMode->ChangeValue(lexical_cast<string>((short)UnitPointer->ProjectileOnly->DropAnimationMode));
			Units_DropAnimationMode->Container = &UnitPointer->ProjectileOnly->DropAnimationMode;
			Units_PenetrationMode->ChangeValue(lexical_cast<string>((short)UnitPointer->ProjectileOnly->PenetrationMode));
			Units_PenetrationMode->Container = &UnitPointer->ProjectileOnly->PenetrationMode;
			Units_Unknown24->ChangeValue(lexical_cast<string>((short)UnitPointer->ProjectileOnly->Unknown24));
			Units_Unknown24->Container = &UnitPointer->ProjectileOnly->Unknown24;
			Units_Unknown25->ChangeValue(lexical_cast<string>(UnitPointer->ProjectileOnly->ProjectileArc));
			Units_Unknown25->Container = &UnitPointer->ProjectileOnly->ProjectileArc;
		}
		else
		{
			Units_StretchMode->Enable(false);
			Units_StretchMode->ChangeValue("0");
			Units_CompensationMode->Enable(false);
			Units_CompensationMode->ChangeValue("0");
			Units_DropAnimationMode->Enable(false);
			Units_DropAnimationMode->ChangeValue("0");
			Units_PenetrationMode->Enable(false);
			Units_PenetrationMode->ChangeValue("0");
			Units_Unknown24->Enable(false);
			Units_Unknown24->ChangeValue("0");
			Units_Unknown25->Enable(false);
			Units_Unknown25->ChangeValue("0");
		}		
		
//	Type 70+

		if(GenieFile->Civs[UnitCivID].Units[UnitID].Creatable)
		{
			for(short loop = 0;loop < 2;loop++)
			{
				Units_GarrisonGraphic[loop]->Enable(true);
				Units_ComboBox_GarrisonGraphic[loop]->Enable(true);
			}
			for(short loop = 0;loop < 3;loop++)
			{
				Units_CostType[loop]->Enable(true);
				Units_ComboBox_CostType[loop]->Enable(true);
				Units_CostAmount[loop]->Enable(true);
				Units_CostUsed[loop]->Enable(true);
				Units_CheckBox_CostUsed[loop]->Enable(true);
				Units_Unknown27[loop]->Enable(true);
				Units_AttackMissileDuplicationUnknown[loop]->Enable(true);
			}
			Units_TrainTime->Enable(true);
			Units_TrainLocationID->Enable(true);
			Units_ComboBox_TrainLocationID->Enable(true);
			Units_ButtonID->Enable(true);
			Units_Unknown26->Enable(true);
			Units_Unknown28->Enable(true);
			Units_MissileGraphicDelay->Enable(true);
			Units_HeroMode->Enable(true);
			Units_CheckBox_HeroMode->Enable(true);
			Units_AttackMissileDuplicationAmount1->Enable(true);
			Units_AttackMissileDuplicationAmount2->Enable(true);
			Units_AttackMissileDuplicationUnit->Enable(true);
			Units_ComboBox_AttackMissileDuplicationUnit->Enable(true);
			Units_AttackMissileDuplicationGraphic->Enable(true);
			Units_ComboBox_AttackMissileDuplicationGraphic->Enable(true);
			Units_Unknown29->Enable(true);
			Units_DisplayedPierceArmour->Enable(true);

			for(short loop = 0;loop < 3;loop++)
			{
				Units_CostType[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->ResourceCosts[loop].Type));
				Units_CostType[loop]->Container = &UnitPointer->Creatable->ResourceCosts[loop].Type;
				Units_ComboBox_CostType[loop]->SetSelection(UnitPointer->Creatable->ResourceCosts[loop].Type + 1);
				Units_CostAmount[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->ResourceCosts[loop].Amount));
				Units_CostAmount[loop]->Container = &UnitPointer->Creatable->ResourceCosts[loop].Amount;
				Units_CostUsed[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->ResourceCosts[loop].Used));
				Units_CostUsed[loop]->Container = &UnitPointer->Creatable->ResourceCosts[loop].Used;
				Units_CheckBox_CostUsed[loop]->SetValue((bool)UnitPointer->Creatable->ResourceCosts[loop].Used);
			}
			Units_TrainTime->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->TrainTime));
			Units_TrainTime->Container = &UnitPointer->Creatable->TrainTime;
			Units_TrainLocationID->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->TrainLocationID));
			Units_TrainLocationID->Container = &UnitPointer->Creatable->TrainLocationID;
			Units_ComboBox_TrainLocationID->SetSelection(UnitPointer->Creatable->TrainLocationID + 1);
			Units_ButtonID->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->ButtonID));
			Units_ButtonID->Container = &UnitPointer->Creatable->ButtonID;
			if(GameVersion >= 2)
			{
				if(ShowUnknowns) Units_Holder_Unknown26->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown27->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown28->Show(true);
				Units_Holder_MissileGraphicDelay->Show(true);
				Units_Holder_HeroMode->Show(true);
				Units_Holder_GarrisonGraphic->Show(true);
				Units_Holder_AttackMissileDuplicationAmount1->Show(true);
				Units_Holder_AttackMissileDuplicationAmount2->Show(true);
				Units_Holder_AttackMissileDuplicationUnknown->Show(true);
				Units_Holder_AttackMissileDuplicationUnit->Show(true);
				Units_Holder_AttackMissileDuplicationGraphic->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown29->Show(true);
			
				Units_Unknown26->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->Unknown26));
				Units_Unknown26->Container = &UnitPointer->Creatable->Unknown26;
				for(short loop = 0;loop < 3;loop++)
				{
					Units_Unknown27[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->Unknown27[loop]));
					Units_Unknown27[loop]->Container = &UnitPointer->Creatable->Unknown27[loop];
				}
				Units_Unknown28->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->Unknown28));
				Units_Unknown28->Container = &UnitPointer->Creatable->Unknown28;
				Units_MissileGraphicDelay->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->MissileGraphicDelay));
				Units_MissileGraphicDelay->Container = &UnitPointer->Creatable->MissileGraphicDelay;
				Units_HeroMode->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->HeroMode));
				Units_HeroMode->Container = &UnitPointer->Creatable->HeroMode;
				Units_CheckBox_HeroMode->SetValue((bool)UnitPointer->Creatable->HeroMode);
				Units_GarrisonGraphic[0]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->GarrisonGraphic.first));
				Units_GarrisonGraphic[0]->Container = &UnitPointer->Creatable->GarrisonGraphic.first;
				Units_ComboBox_GarrisonGraphic[0]->SetSelection(UnitPointer->Creatable->GarrisonGraphic.first + 1);
				Units_GarrisonGraphic[1]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->GarrisonGraphic.second));
				Units_GarrisonGraphic[1]->Container = &UnitPointer->Creatable->GarrisonGraphic.second;
				Units_ComboBox_GarrisonGraphic[1]->SetSelection(UnitPointer->Creatable->GarrisonGraphic.second + 1);
				Units_AttackMissileDuplicationAmount1->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->AttackMissileDuplicationAmount1));
				Units_AttackMissileDuplicationAmount1->Container = &UnitPointer->Creatable->AttackMissileDuplicationAmount1;
				Units_AttackMissileDuplicationAmount2->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->AttackMissileDuplicationAmount2));
				Units_AttackMissileDuplicationAmount2->Container = &UnitPointer->Creatable->AttackMissileDuplicationAmount2;
				for(short loop = 0;loop < 3;loop++)
				{
					Units_AttackMissileDuplicationUnknown[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->AttackMissileDuplicationUnknown[loop]));
					Units_AttackMissileDuplicationUnknown[loop]->Container = &UnitPointer->Creatable->AttackMissileDuplicationUnknown[loop];
				}
				Units_AttackMissileDuplicationUnit->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->AttackMissileDuplicationUnit));
				Units_AttackMissileDuplicationUnit->Container = &UnitPointer->Creatable->AttackMissileDuplicationUnit;
				Units_ComboBox_AttackMissileDuplicationUnit->SetSelection(UnitPointer->Creatable->AttackMissileDuplicationUnit + 1);
				Units_AttackMissileDuplicationGraphic->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->AttackMissileDuplicationGraphic));
				Units_AttackMissileDuplicationGraphic->Container = &UnitPointer->Creatable->AttackMissileDuplicationGraphic;
				Units_ComboBox_AttackMissileDuplicationGraphic->SetSelection(UnitPointer->Creatable->AttackMissileDuplicationGraphic + 1);
				Units_Unknown29->ChangeValue(lexical_cast<string>((short)UnitPointer->Creatable->Unknown29));
				Units_Unknown29->Container = &UnitPointer->Creatable->Unknown29;
			}
			else
			{
				Units_Holder_Unknown26->Show(false);
				Units_Holder_Unknown27->Show(false);
				Units_Holder_Unknown28->Show(false);
				Units_Holder_MissileGraphicDelay->Show(false);
				Units_Holder_HeroMode->Show(false);
				Units_Holder_GarrisonGraphic->Show(false);
				Units_Holder_AttackMissileDuplicationAmount1->Show(false);
				Units_Holder_AttackMissileDuplicationAmount2->Show(false);
				Units_Holder_AttackMissileDuplicationUnknown->Show(false);
				Units_Holder_AttackMissileDuplicationUnit->Show(false);
				Units_Holder_AttackMissileDuplicationGraphic->Show(false);
				Units_Holder_Unknown29->Show(false);
			}
			Units_DisplayedPierceArmour->ChangeValue(lexical_cast<string>(UnitPointer->Creatable->DisplayedPierceArmour));
			Units_DisplayedPierceArmour->Container = &UnitPointer->Creatable->DisplayedPierceArmour;
		}
		else
		{
			for(short loop = 0;loop < 2;loop++)
			{
				Units_GarrisonGraphic[loop]->Enable(false);
				Units_GarrisonGraphic[loop]->ChangeValue("0");
				Units_ComboBox_GarrisonGraphic[loop]->Enable(false);
				Units_ComboBox_GarrisonGraphic[loop]->SetSelection(0);
			}
			for(short loop = 0;loop < 3;loop++)
			{
				Units_CostType[loop]->Enable(false);
				Units_CostType[loop]->ChangeValue("0");
				Units_ComboBox_CostType[loop]->Enable(false);
				Units_ComboBox_CostType[loop]->SetSelection(0);
				Units_CostAmount[loop]->Enable(false);
				Units_CostAmount[loop]->ChangeValue("0");
				Units_CostUsed[loop]->Enable(false);
				Units_CostUsed[loop]->ChangeValue("0");
				Units_CheckBox_CostUsed[loop]->Enable(false);
				Units_CheckBox_CostUsed[loop]->SetValue(false);
				Units_Unknown27[loop]->Enable(false);
				Units_Unknown27[loop]->ChangeValue("0");
				Units_AttackMissileDuplicationUnknown[loop]->Enable(false);
				Units_AttackMissileDuplicationUnknown[loop]->ChangeValue("0");
			}
			Units_TrainTime->Enable(false);
			Units_TrainTime->ChangeValue("0");
			Units_TrainLocationID->Enable(false);
			Units_TrainLocationID->ChangeValue("0");
			Units_ComboBox_TrainLocationID->Enable(false);
			Units_ComboBox_TrainLocationID->SetSelection(0);
			Units_ButtonID->Enable(false);
			Units_ButtonID->ChangeValue("0");
			Units_Unknown26->Enable(false);
			Units_Unknown26->ChangeValue("0");
			Units_Unknown28->Enable(false);
			Units_Unknown28->ChangeValue("0");
			Units_MissileGraphicDelay->Enable(false);
			Units_MissileGraphicDelay->ChangeValue("0");
			Units_HeroMode->Enable(false);
			Units_HeroMode->ChangeValue("0");
			Units_CheckBox_HeroMode->Enable(false);
			Units_CheckBox_HeroMode->SetValue(false);
			Units_AttackMissileDuplicationAmount1->Enable(false);
			Units_AttackMissileDuplicationAmount1->ChangeValue("0");
			Units_AttackMissileDuplicationAmount2->Enable(false);
			Units_AttackMissileDuplicationAmount2->ChangeValue("0");
			Units_AttackMissileDuplicationUnit->Enable(false);
			Units_AttackMissileDuplicationUnit->ChangeValue("0");
			Units_ComboBox_AttackMissileDuplicationUnit->Enable(false);
			Units_ComboBox_AttackMissileDuplicationUnit->SetSelection(0);
			Units_AttackMissileDuplicationGraphic->Enable(false);
			Units_AttackMissileDuplicationGraphic->ChangeValue("0");
			Units_ComboBox_AttackMissileDuplicationGraphic->Enable(false);
			Units_ComboBox_AttackMissileDuplicationGraphic->SetSelection(0);
			Units_Unknown29->Enable(false);
			Units_Unknown29->ChangeValue("0");
			Units_DisplayedPierceArmour->Enable(false);
			Units_DisplayedPierceArmour->ChangeValue("0");
		}
		
//	Type 80

		if(GenieFile->Civs[UnitCivID].Units[UnitID].Building)
		{
			Units_ConstructionGraphicID->Enable(true);
			Units_ComboBox_ConstructionGraphicID->Enable(true);
			Units_SnowGraphicID->Enable(true);
			Units_ComboBox_SnowGraphicID->Enable(true);
			Units_Unknown30->Enable(true);
			Units_Unknown31->Enable(true);
			Units_StackUnitID->Enable(true);
			Units_ComboBox_StackUnitID->Enable(true);
			Units_TerrainID->Enable(true);
			Units_ComboBox_TerrainID->Enable(true);
			Units_Unknown32->Enable(true);
			Units_ResearchID->Enable(true);
			Units_ComboBox_ResearchID->Enable(true);
			Units_Unknown33->Enable(true);
			for(short loop = 0;loop < 4;loop++)
			{
				Units_AnnexUnit[loop]->Enable(true);
				Units_ComboBox_AnnexUnit[loop]->Enable(true);
				for(short loop2 = 0;loop2 < 2;loop2++)
				Units_AnnexUnitMisplacement[loop][loop2]->Enable(true);
			}
			Units_HeadUnit->Enable(true);
			Units_ComboBox_HeadUnit->Enable(true);
			Units_TransformUnit->Enable(true);
			Units_ComboBox_TransformUnit->Enable(true);
			Units_Unknown34->Enable(true);
			Units_ConstructionSound->Enable(true);
			Units_ComboBox_ConstructionSound->Enable(true);
			Units_GarrisonType->Enable(true);
			Units_ComboBox_GarrisonType->Enable(true);
			Units_GarrisonHealRate->Enable(true);
			Units_Unknown35->Enable(true);
			Units_Unknown36->Enable(true);
			for(short loop = 0;loop < 6;loop++)
			Units_Unknown37[loop]->Enable(true);
			
			Units_ConstructionGraphicID->ChangeValue(lexical_cast<string>(UnitPointer->Building->ConstructionGraphicID));
			Units_ConstructionGraphicID->Container = &UnitPointer->Building->ConstructionGraphicID;
			Units_ComboBox_ConstructionGraphicID->SetSelection(UnitPointer->Building->ConstructionGraphicID + 1);
			if(GameVersion >= 3)
			{
				Units_Holder_SnowGraphicID->Show(true);
				
				Units_SnowGraphicID->ChangeValue(lexical_cast<string>(UnitPointer->Building->SnowGraphicID));
				Units_SnowGraphicID->Container = &UnitPointer->Building->SnowGraphicID;
				Units_ComboBox_SnowGraphicID->SetSelection(UnitPointer->Building->SnowGraphicID + 1);
			}
			else
			{
				Units_Holder_SnowGraphicID->Show(false);
			}
			Units_Unknown30->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown30));
			Units_Unknown30->Container = &UnitPointer->Building->Unknown30;
			Units_Unknown31->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown31));
			Units_Unknown31->Container = &UnitPointer->Building->Unknown31;
			Units_StackUnitID->ChangeValue(lexical_cast<string>(UnitPointer->Building->StackUnitID));
			Units_StackUnitID->Container = &UnitPointer->Building->StackUnitID;
			Units_ComboBox_StackUnitID->SetSelection(UnitPointer->Building->StackUnitID + 1);
			Units_TerrainID->ChangeValue(lexical_cast<string>(UnitPointer->Building->TerrainID));
			Units_TerrainID->Container = &UnitPointer->Building->TerrainID;
			Units_ComboBox_TerrainID->SetSelection(UnitPointer->Building->TerrainID + 1);
			Units_Unknown32->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown32));
			Units_Unknown32->Container = &UnitPointer->Building->Unknown32;
			Units_ResearchID->ChangeValue(lexical_cast<string>(UnitPointer->Building->ResearchID));
			Units_ResearchID->Container = &UnitPointer->Building->ResearchID;
			Units_ComboBox_ResearchID->SetSelection(UnitPointer->Building->ResearchID + 1);
			if(GameVersion >= 2)
			{
				if(ShowUnknowns) Units_Holder_Unknown33->Show(true);
				Units_Holder_AnnexUnit1->Show(true);
				Units_Holder_AnnexUnitMisplacement1->Show(true);
				Units_Holder_HeadUnit->Show(true);
				Units_Holder_HeadUnit->Show(true);
				Units_Holder_TransformUnit->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown34->Show(true);
				
				Units_Unknown33->ChangeValue(lexical_cast<string>((short)UnitPointer->Building->Unknown33));
				Units_Unknown33->Container = &UnitPointer->Building->Unknown33;
				for(short loop = 0;loop < 4;loop++)
				{
					Units_AnnexUnit[loop]->ChangeValue(lexical_cast<string>(UnitPointer->Building->Annexes[loop].UnitID));
					Units_AnnexUnit[loop]->Container = &UnitPointer->Building->Annexes[loop].UnitID;
					Units_ComboBox_AnnexUnit[loop]->SetSelection(UnitPointer->Building->Annexes[loop].UnitID + 1);
					Units_AnnexUnitMisplacement[loop][0]->ChangeValue(lexical_cast<string>(UnitPointer->Building->Annexes[loop].Misplacement.first));
					Units_AnnexUnitMisplacement[loop][0]->Container = &UnitPointer->Building->Annexes[loop].Misplacement.first;
					Units_AnnexUnitMisplacement[loop][1]->ChangeValue(lexical_cast<string>(UnitPointer->Building->Annexes[loop].Misplacement.second));
					Units_AnnexUnitMisplacement[loop][1]->Container = &UnitPointer->Building->Annexes[loop].Misplacement.second;
				}
				Units_HeadUnit->ChangeValue(lexical_cast<string>(UnitPointer->Building->HeadUnit));
				Units_HeadUnit->Container = &UnitPointer->Building->HeadUnit;
				Units_ComboBox_HeadUnit->SetSelection(UnitPointer->Building->HeadUnit + 1);
				Units_TransformUnit->ChangeValue(lexical_cast<string>(UnitPointer->Building->TransformUnit));
				Units_TransformUnit->Container = &UnitPointer->Building->TransformUnit;
				Units_ComboBox_TransformUnit->SetSelection(UnitPointer->Building->TransformUnit + 1);
				Units_Unknown34->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown34));
				Units_Unknown34->Container = &UnitPointer->Building->Unknown34;
			}
			else
			{
				Units_Holder_Unknown33->Show(false);
				Units_Holder_AnnexUnit1->Show(false);
				Units_Holder_AnnexUnitMisplacement1->Show(false);
				Units_Holder_HeadUnit->Show(false);
				Units_Holder_TransformUnit->Show(false);
				Units_Holder_Unknown34->Show(false);
			}
			Units_ConstructionSound->ChangeValue(lexical_cast<string>(UnitPointer->Building->ConstructionSound));
			Units_ConstructionSound->Container = &UnitPointer->Building->ConstructionSound;
			Units_ComboBox_ConstructionSound->SetSelection(UnitPointer->Building->ConstructionSound + 1);
			if(GameVersion >= 2)
			{
				Units_Holder_GarrisonType->Show(true);
				Units_Holder_GarrisonHealRate->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown35->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown36->Show(true);
				if(ShowUnknowns) Units_Holder_Unknown37->Show(true);
			
				Units_GarrisonType->ChangeValue(lexical_cast<string>((short)UnitPointer->Building->GarrisonType));
				Units_GarrisonType->Container = &UnitPointer->Building->GarrisonType;
				Units_ComboBox_GarrisonType->SetSelection(UnitPointer->Building->GarrisonType + 1);
				Units_GarrisonHealRate->ChangeValue(lexical_cast<string>(UnitPointer->Building->GarrisonHealRate));
				Units_GarrisonHealRate->Container = &UnitPointer->Building->GarrisonHealRate;
				Units_Unknown35->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown35));
				Units_Unknown35->Container = &UnitPointer->Building->Unknown35;
				Units_Unknown36->ChangeValue(lexical_cast<string>(UnitPointer->Building->Unknown36));
				Units_Unknown36->Container = &UnitPointer->Building->Unknown36;
				for(short loop = 0;loop < 6;loop++)
				{
					Units_Unknown37[loop]->ChangeValue(lexical_cast<string>((short)UnitPointer->Building->Unknown37[loop]));
					Units_Unknown37[loop]->Container = &UnitPointer->Building->Unknown37[loop];
				}
			}
			else
			{
				Units_Holder_GarrisonType->Show(false);
				Units_Holder_GarrisonHealRate->Show(false);
				Units_Holder_Unknown35->Show(false);
				Units_Holder_Unknown36->Show(false);
				Units_Holder_Unknown37->Show(false);
			}
		}
		else
		{
			Units_ConstructionGraphicID->Enable(false);
			Units_ConstructionGraphicID->ChangeValue("0");
			Units_ComboBox_ConstructionGraphicID->Enable(false);
			Units_ComboBox_ConstructionGraphicID->SetSelection(0);
			Units_SnowGraphicID->Enable(false);
			Units_SnowGraphicID->ChangeValue("0");
			Units_ComboBox_SnowGraphicID->Enable(false);
			Units_ComboBox_SnowGraphicID->SetSelection(0);
			Units_Unknown30->Enable(false);
			Units_Unknown30->ChangeValue("0");
			Units_Unknown31->Enable(false);
			Units_Unknown31->ChangeValue("0");
			Units_StackUnitID->Enable(false);
			Units_StackUnitID->ChangeValue("0");
			Units_ComboBox_StackUnitID->Enable(false);
			Units_ComboBox_StackUnitID->SetSelection(0);
			Units_TerrainID->Enable(false);
			Units_TerrainID->ChangeValue("0");
			Units_ComboBox_TerrainID->Enable(false);
			Units_ComboBox_TerrainID->SetSelection(0);
			Units_Unknown32->Enable(false);
			Units_Unknown32->ChangeValue("0");
			Units_ResearchID->Enable(false);
			Units_ResearchID->ChangeValue("0");
			Units_ComboBox_ResearchID->Enable(false);
			Units_ComboBox_ResearchID->SetSelection(0);
			Units_Unknown33->Enable(false);
			Units_Unknown33->ChangeValue("0");
			for(short loop = 0;loop < 4;loop++)
			{
				Units_AnnexUnit[loop]->Enable(false);
				Units_AnnexUnit[loop]->ChangeValue("0");
				Units_ComboBox_AnnexUnit[loop]->Enable(false);
				Units_ComboBox_AnnexUnit[loop]->SetSelection(0);
				for(short loop2 = 0;loop2 < 2;loop2++)
				{
					Units_AnnexUnitMisplacement[loop][loop2]->Enable(false);
					Units_AnnexUnitMisplacement[loop][loop2]->ChangeValue("0");
				}
			}
			Units_HeadUnit->Enable(false);
			Units_HeadUnit->ChangeValue("0");
			Units_ComboBox_HeadUnit->Enable(false);
			Units_ComboBox_HeadUnit->SetSelection(0);
			Units_TransformUnit->Enable(false);
			Units_TransformUnit->ChangeValue("0");
			Units_ComboBox_TransformUnit->Enable(false);
			Units_ComboBox_TransformUnit->SetSelection(0);
			Units_Unknown34->Enable(false);
			Units_Unknown34->ChangeValue("0");
			Units_ConstructionSound->Enable(false);
			Units_ConstructionSound->ChangeValue("0");
			Units_ComboBox_ConstructionSound->Enable(false);
			Units_ComboBox_ConstructionSound->SetSelection(0);
			Units_GarrisonType->Enable(false);
			Units_GarrisonType->ChangeValue("0");
			Units_ComboBox_GarrisonType->Enable(false);
			Units_ComboBox_GarrisonType->SetSelection(0);
			Units_GarrisonHealRate->Enable(false);
			Units_GarrisonHealRate->ChangeValue("0");
			Units_Unknown35->Enable(false);
			Units_Unknown35->ChangeValue("0");
			Units_Unknown36->Enable(false);
			Units_Unknown36->ChangeValue("0");
			for(short loop = 0;loop < 6;loop++)
			{
				Units_Unknown37[loop]->Enable(false);
				Units_Unknown37[loop]->ChangeValue("0");
			}
		}
		
		for(short loop = 0;loop < 3;loop++)
		{
			ResourceStorage_Type[loop]->ChangeValue(lexical_cast<string>(UnitPointer->ResourceStorage[loop].Type));
			ResourceStorage_Type[loop]->Container = &UnitPointer->ResourceStorage[loop].Type;
			ResourceStorage_Amount[loop]->ChangeValue(lexical_cast<string>(UnitPointer->ResourceStorage[loop].Amount));
			ResourceStorage_Amount[loop]->Container = &UnitPointer->ResourceStorage[loop].Amount;
			ResourceStorage_Enabled[loop]->ChangeValue(lexical_cast<string>((short)UnitPointer->ResourceStorage[loop].Enabled));
			ResourceStorage_Enabled[loop]->Container = &UnitPointer->ResourceStorage[loop].Enabled;
			ResourceStorage_ComboBox_Type[loop]->SetSelection(UnitPointer->ResourceStorage[loop].Type + 1);
		}
		
		Units_DLL_LanguageDllName->Wrap(Units_DLL_LanguageDllName->GetSize().GetWidth());
		Units_DLL_LanguageDllCreation->Wrap(Units_DLL_LanguageDllCreation->GetSize().GetWidth());
		Units_DLL_LanguageDllHelp->Wrap(Units_DLL_LanguageDllHelp->GetSize().GetWidth());
		Units_DLL_HotKey4->Wrap(Units_DLL_HotKey4->GetSize().GetWidth());
		
		Added = false;
		ListUnitDamageGraphics(UnitID, UnitCivID);
		ListUnitAttacks(UnitID, UnitCivID);
		ListUnitArmors(UnitID, UnitCivID);
		if(GameVersion > 1)	// AoK, TC, SWGB or CC
		{	// Disabling this (unit headers) doesn't increase search speed.
			Units_UnitHeads_List->SetSelection(UnitID); // Correct selection even when units are search filtered.
			wxCommandEvent E;
			OnUnitHeadsSelect(E);
		}
		else	// AoE or RoR
		{
			Units_UnitHeads_Name->SetLabel("");
			ListUnitCommands(UnitID, UnitCivID);
		}
	}
}

void AGE_Frame::OnUnitHeadsSelect(wxCommandEvent& Event)
{
	short Selection = Units_UnitHeads_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		gdat::UnitHeader * UnitHeadPointer = (gdat::UnitHeader*)Units_UnitHeads_List->GetClientData(Selection);
		Units_UnitHeads_Name->SetLabel(" "+lexical_cast<string>(UnitID)+" - "+GetUnitName(UnitID, 0, false));
		Units_Exists->ChangeValue(lexical_cast<string>((short)UnitHeadPointer->Exists));
		Units_Exists->Container = &UnitHeadPointer->Exists;
		ListUnitCommands(UnitID, 0);
	}
}

void AGE_Frame::OnUnitsAdd(wxCommandEvent& Event)
{
	if(GameVersion > 1)	// AoK, TC, SWGB or CC
	{
		gdat::UnitHeader Temp;
		GenieFile->UnitHeaders.push_back(Temp);
	}
	wxCommandEvent E;
	OnUnitHeadsAdd(E);
}

void AGE_Frame::OnUnitHeadsAdd(wxCommandEvent& Event)
{
	gdat::Unit Temp;
	for(short loop = 0;loop < GenieFile->Civs.size();loop++)
	{
		GenieFile->Civs[loop].Units.push_back(Temp);
		GenieFile->Civs[loop].UnitPointers.push_back(1);
		GenieFile->Civs[loop].Units[GenieFile->Civs[0].Units.size() - 1].ID1 = lexical_cast<short>(GenieFile->Civs[0].Units.size() - 1);	//	ID Fix
		GenieFile->Civs[loop].Units[GenieFile->Civs[0].Units.size() - 1].ID2 = lexical_cast<short>(GenieFile->Civs[0].Units.size() - 1);
		if(GameVersion >= 2)
		GenieFile->Civs[loop].Units[GenieFile->Civs[0].Units.size() - 1].ID3 = lexical_cast<short>(GenieFile->Civs[0].Units.size() - 1);
	}
	if(GameVersion > 1)
	{
		ListUnitHeads();
	}
	Added = true;
	ListUnits(UnitCivID);
}

void AGE_Frame::OnUnitsDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	if(GameVersion > 1)
	{
		short Selection = Units_UnitHeads_List->GetSelection();
		if(Selection != wxNOT_FOUND)	// If unit is selected.
		{
			GenieFile->UnitHeaders.erase(GenieFile->UnitHeaders.begin() + UnitID);
		}
	}
	wxCommandEvent E;
	OnUnitHeadsDelete(E);
}

void AGE_Frame::OnUnitHeadsDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units.erase(GenieFile->Civs[loop].Units.begin() + UnitID);
			GenieFile->Civs[loop].UnitPointers.erase(GenieFile->Civs[loop].UnitPointers.begin() + UnitID);
			for(short loop2 = UnitID;loop2 < GenieFile->Civs[0].Units.size();loop2++)	//	ID Fix
			{
				GenieFile->Civs[loop].Units[loop2].ID1 = lexical_cast<short>(loop2);
				GenieFile->Civs[loop].Units[loop2].ID2 = lexical_cast<short>(loop2);
				if(GameVersion >= 2)
				GenieFile->Civs[loop].Units[loop2].ID3 = lexical_cast<short>(loop2);
			}
		}
		if(GameVersion > 1)
		{
			ListUnitHeads();
		}
		if(Selection == Units_Units_List->GetCount() - 1)
		Units_Units_List->SetSelection(Selection - 1);
		ListUnits(UnitCivID);
	}
}

void AGE_Frame::OnUnitsCopy(wxCommandEvent& Event)
{
	if(GameVersion > 1)
	{
		short Selection = Units_UnitHeads_List->GetSelection();
		if(Selection != wxNOT_FOUND)
		{
			UnitHeaderCopy = *(gdat::UnitHeader*)Units_UnitHeads_List->GetClientData(Selection);
		}
	}
	wxCommandEvent E;
	OnUnitHeadsCopy(E);
}

void AGE_Frame::OnUnitHeadsCopy(wxCommandEvent& Event)
{
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		RefreshLists = UnitID;
		UnitCopy = GenieFile->Civs[UnitCivID].Units[UnitID];
		if(AutoCopy == MenuOption_Exclude)
		{
			for(short loop = 0;loop < GenieFile->Civs.size();loop++)
			{	// Collects only graphic data, not all data again.
				UnitGraphics[loop].IconID = GenieFile->Civs[loop].Units[UnitID].IconID;// This probably shouldn't be here.
				UnitGraphics[loop].StandingGraphic = GenieFile->Civs[loop].Units[UnitID].StandingGraphic;
				UnitGraphics[loop].DyingGraphic = GenieFile->Civs[loop].Units[UnitID].DyingGraphic;
				UnitGraphics[loop].DamageGraphicCount = GenieFile->Civs[loop].Units[UnitID].DamageGraphicCount;
				UnitGraphics[loop].DamageGraphics = GenieFile->Civs[loop].Units[UnitID].DamageGraphics;
				if(GenieFile->Civs[loop].Units[UnitID].DeadFish)
				UnitGraphics30[loop].WalkingGraphic = GenieFile->Civs[loop].Units[UnitID].DeadFish->WalkingGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Projectile)
				UnitGraphics60[loop].AttackGraphic = GenieFile->Civs[loop].Units[UnitID].Projectile->AttackGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Creatable)
				UnitGraphics70[loop].GarrisonGraphic = GenieFile->Civs[loop].Units[UnitID].Creatable->GarrisonGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Building){
				UnitGraphics80[loop].ConstructionGraphicID = GenieFile->Civs[loop].Units[UnitID].Building->ConstructionGraphicID;
				UnitGraphics80[loop].SnowGraphicID = GenieFile->Civs[loop].Units[UnitID].Building->SnowGraphicID;}
			}
		}
	}
}

void AGE_Frame::OnUnitsPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	if(GameVersion > 1)
	{
		short Selection = Units_UnitHeads_List->GetSelection();
		if(Selection != wxNOT_FOUND)
		{
			*(gdat::UnitHeader*)Units_UnitHeads_List->GetClientData(Selection) = UnitHeaderCopy;
		}
	}
	wxCommandEvent E;
	OnUnitHeadsPaste(E);
}

void AGE_Frame::OnUnitHeadsPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(AutoCopy == MenuOption_NoAuto)
		{
			GenieFile->Civs[UnitCivID].Units[UnitID] = UnitCopy;
		}
		else for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID] = UnitCopy;
			if(AutoCopy == MenuOption_Exclude)
			{
				GenieFile->Civs[loop].Units[UnitID].IconID = UnitGraphics[loop].IconID;
				GenieFile->Civs[loop].Units[UnitID].StandingGraphic = UnitGraphics[loop].StandingGraphic;
				GenieFile->Civs[loop].Units[UnitID].DyingGraphic = UnitGraphics[loop].DyingGraphic;
				GenieFile->Civs[loop].Units[UnitID].DamageGraphicCount = UnitGraphics[loop].DamageGraphicCount;
				GenieFile->Civs[loop].Units[UnitID].DamageGraphics = UnitGraphics[loop].DamageGraphics;
				if(GenieFile->Civs[loop].Units[UnitID].DeadFish)
				GenieFile->Civs[loop].Units[UnitID].DeadFish->WalkingGraphic = UnitGraphics30[loop].WalkingGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Projectile)
				GenieFile->Civs[loop].Units[UnitID].Projectile->AttackGraphic = UnitGraphics60[loop].AttackGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Creatable)
				GenieFile->Civs[loop].Units[UnitID].Creatable->GarrisonGraphic = UnitGraphics70[loop].GarrisonGraphic;
				if(GenieFile->Civs[loop].Units[UnitID].Building){
				GenieFile->Civs[loop].Units[UnitID].Building->ConstructionGraphicID = UnitGraphics80[loop].ConstructionGraphicID;
				GenieFile->Civs[loop].Units[UnitID].Building->SnowGraphicID = UnitGraphics80[loop].SnowGraphicID;}
			}
		}
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].ID1 = lexical_cast<short>(UnitID);	//	ID Fix
			GenieFile->Civs[loop].Units[UnitID].ID2 = lexical_cast<short>(UnitID);
			if(GameVersion >= 2)
			GenieFile->Civs[loop].Units[UnitID].ID3 = lexical_cast<short>(UnitID);
		}
		if(RefreshLists != UnitID)
		{
			if(GameVersion > 1)
			{
				ListUnitHeads();
			}
			ListUnits(UnitCivID);
		}
	}
}

//	SubVectors

string AGE_Frame::GetUnitDamageGraphicName(short Index, short UnitCivID, short UnitID)
{
	string Name = "";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].DamageGraphics[Index].DamagePercent);
	Name += " % - ID: ";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].DamageGraphics[Index].GraphicID);
	return Name;
}

void AGE_Frame::ListUnitDamageGraphics(short Index, short UnitCivID)
{
	string Name;
	SearchText = wxString(Units_DamageGraphics_Search->GetValue()).Lower();
	ExcludeText = wxString(Units_DamageGraphics_Search_R->GetValue()).Lower();
	string CompareText;
	short Selection = Units_DamageGraphics_List->GetSelection();

	if(!Units_DamageGraphics_List->IsEmpty())
	{
		Units_DamageGraphics_List->Clear();
	}
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	for(short loop = 0;loop < GenieFile->Civs[UnitCivID].Units[Index].DamageGraphics.size();loop++)
	{
		CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitDamageGraphicName(loop, UnitCivID, Index)).Lower();
		if(SearchMatches(CompareText) == true)
		{
			Name = lexical_cast<string>(loop);
			Name += " - ";
			Name += GetUnitDamageGraphicName(loop, UnitCivID, Index);
			Units_DamageGraphics_List->Append(Name, (void*)&GenieFile->Civs[UnitCivID].Units[Index].DamageGraphics[loop]);
		}
	}
	Units_DamageGraphics_List->SetSelection(0);
	Units_DamageGraphics_List->SetFirstItem(Selection - 3);
	Units_DamageGraphics_List->SetSelection(Selection);

	wxCommandEvent E;
	OnUnitDamageGraphicsSelect(E);
}

void AGE_Frame::OnUnitDamageGraphicsSearch(wxCommandEvent& Event)
{
	ListUnitDamageGraphics(UnitID, UnitCivID);
}

void AGE_Frame::OnUnitDamageGraphicsSelect(wxCommandEvent& Event)
{
	short Selection = Units_DamageGraphics_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(Added)
		{
			Selection = Units_DamageGraphics_List->GetCount() - 1;
			Units_DamageGraphics_List->SetSelection(Selection);
		}
		gdat::unit::DamageGraphic * DamageGraphicPointer = (gdat::unit::DamageGraphic*)Units_DamageGraphics_List->GetClientData(Selection);
		DamageGraphicID = DamageGraphicPointer - (&GenieFile->Civs[UnitCivID].Units[UnitID].DamageGraphics[0]);
		DamageGraphics_GraphicID->ChangeValue(lexical_cast<string>(DamageGraphicPointer->GraphicID));
		DamageGraphics_GraphicID->Container = &DamageGraphicPointer->GraphicID;
		DamageGraphics_ComboBox_GraphicID->SetSelection(DamageGraphicPointer->GraphicID + 1);
		DamageGraphics_DamagePercent->ChangeValue(lexical_cast<string>(DamageGraphicPointer->DamagePercent));
		DamageGraphics_DamagePercent->Container = &DamageGraphicPointer->DamagePercent;
		DamageGraphics_Unknown1->ChangeValue(lexical_cast<string>((short)DamageGraphicPointer->Unknown1));
		DamageGraphics_Unknown1->Container = &DamageGraphicPointer->Unknown1;
		Added = false;
	}
	else
	{
		DamageGraphics_GraphicID->ChangeValue("0");
		DamageGraphics_ComboBox_GraphicID->SetSelection(0);
		DamageGraphics_DamagePercent->ChangeValue("0");
		DamageGraphics_Unknown1->ChangeValue("0");
	}
}

void AGE_Frame::OnUnitDamageGraphicsAdd(wxCommandEvent& Event)
{
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		gdat::unit::DamageGraphic Temp;
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].DamageGraphics.push_back(Temp);
		}
		Added = true;
		ListUnitDamageGraphics(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitDamageGraphicsDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_DamageGraphics_List->GetSelection(); // Gives the current list selection.
	if(Selection != wxNOT_FOUND)
	{
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].DamageGraphics.erase(GenieFile->Civs[loop].Units[UnitID].DamageGraphics.begin() + DamageGraphicID);
		}
		if(Selection == Units_DamageGraphics_List->GetCount() - 1)
		Units_DamageGraphics_List->SetSelection(Selection - 1);
		ListUnitDamageGraphics(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitDamageGraphicsCopy(wxCommandEvent& Event)
{
	short Selection = Units_DamageGraphics_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		DamageGraphicCopy = *(gdat::unit::DamageGraphic*)Units_DamageGraphics_List->GetClientData(Selection);
	}
}

void AGE_Frame::OnUnitDamageGraphicsPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_DamageGraphics_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(AutoCopy == MenuOption_NoAuto)
			GenieFile->Civs[UnitCivID].Units[UnitID].DamageGraphics[DamageGraphicID] = DamageGraphicCopy;
		else for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].DamageGraphics[DamageGraphicID] = DamageGraphicCopy;
		}
		ListUnitDamageGraphics(UnitID, UnitCivID);
	}
}

string AGE_Frame::GetUnitAttackName(short Index, short UnitCivID, short UnitID)
{
	string Name = "";
	Name += "Amount: ";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Attacks[Index].Amount);
	Name += " - Class ";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Attacks[Index].Class);
	return Name;
}

void AGE_Frame::ListUnitAttacks(short Index, short UnitCivID)
{
	string Name;
	SearchText = wxString(Units_Attacks_Search->GetValue()).Lower();
	ExcludeText = wxString(Units_Attacks_Search_R->GetValue()).Lower();
	string CompareText;
	short Selection = Units_Attacks_List->GetSelection();

	if(!Units_Attacks_List->IsEmpty())
	{
		Units_Attacks_List->Clear();
	}
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	if(GenieFile->Civs[UnitCivID].Units[Index].Projectile)
	{
		Units_Attacks_Add->Enable(true);
		for(short loop = 0;loop < GenieFile->Civs[UnitCivID].Units[Index].Projectile->Attacks.size();loop++)
		{
			CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitAttackName(loop, UnitCivID, Index)).Lower();
			if(SearchMatches(CompareText) == true)
			{
				Name = lexical_cast<string>(loop);
				Name += " - ";
				Name += GetUnitAttackName(loop, UnitCivID, Index);
				Units_Attacks_List->Append(Name, (void*)&GenieFile->Civs[UnitCivID].Units[Index].Projectile->Attacks[loop]);
			}
		}
	}
	else
	{
		Units_Attacks_Add->Enable(false);
	}
	Units_Attacks_List->SetSelection(0);
	Units_Attacks_List->SetFirstItem(Selection - 3);
	Units_Attacks_List->SetSelection(Selection);

	wxCommandEvent E;
	OnUnitAttacksSelect(E);
}

void AGE_Frame::OnUnitAttacksSearch(wxCommandEvent& Event)
{
	ListUnitAttacks(UnitID, UnitCivID);
}

void AGE_Frame::OnUnitAttacksSelect(wxCommandEvent& Event)
{
	short Selection = Units_Attacks_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(Added)
		{
			Selection = Units_Attacks_List->GetCount() - 1;
			Units_Attacks_List->SetSelection(Selection);
		}
		gdat::unit::AttackOrArmor * AttackPointer = (gdat::unit::AttackOrArmor*)Units_Attacks_List->GetClientData(Selection);
		AttackID = AttackPointer - (&GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Attacks[0]);
		Attacks_Class->ChangeValue(lexical_cast<string>(AttackPointer->Class));
		Attacks_Class->Container = &AttackPointer->Class;
		Attacks_ComboBox_Class->SetSelection(AttackPointer->Class);
		Attacks_Amount->ChangeValue(lexical_cast<string>(AttackPointer->Amount));
		Attacks_Amount->Container = &AttackPointer->Amount;
		Added = false;
	}
	else
	{
		Attacks_Class->ChangeValue("0");
		Attacks_ComboBox_Class->SetSelection(0);
		Attacks_Amount->ChangeValue("0");
	}
}

void AGE_Frame::OnUnitAttacksAdd(wxCommandEvent& Event)
{
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		gdat::unit::AttackOrArmor Temp;
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Attacks.push_back(Temp);
		}
		Added = true;
		ListUnitAttacks(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitAttacksDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Attacks_List->GetSelection(); // Gives the current list selection.
	if(Selection != wxNOT_FOUND)
	{
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Attacks.erase(GenieFile->Civs[loop].Units[UnitID].Projectile->Attacks.begin() + AttackID);
		}
		if(Selection == Units_Attacks_List->GetCount() - 1)
		Units_Attacks_List->SetSelection(Selection - 1);
		ListUnitAttacks(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitAttacksCopy(wxCommandEvent& Event)
{
	short Selection = Units_Attacks_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		AttackCopy = *(gdat::unit::AttackOrArmor*)Units_Attacks_List->GetClientData(Selection);
	}
}

void AGE_Frame::OnUnitAttacksPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Attacks_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(AutoCopy == MenuOption_NoAuto)
			GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Attacks[AttackID] = AttackCopy;
		else for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Attacks[AttackID] = AttackCopy;
		}
		ListUnitAttacks(UnitID, UnitCivID);
	}
}

string AGE_Frame::GetUnitArmorName(short Index, short UnitCivID, short UnitID)
{
	string Name = "";
	Name += "Amount: ";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Armours[Index].Amount);
	Name += " - Class ";
	Name += lexical_cast<string>(GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Armours[Index].Class);
	return Name;
}

void AGE_Frame::ListUnitArmors(short Index, short UnitCivID)
{
	string Name;
	SearchText = wxString(Units_Armors_Search->GetValue()).Lower();
	ExcludeText = wxString(Units_Armors_Search_R->GetValue()).Lower();
	string CompareText;
	short Selection = Units_Armors_List->GetSelection();

	if(!Units_Armors_List->IsEmpty())
	{
		Units_Armors_List->Clear();
	}
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	if(GenieFile->Civs[UnitCivID].Units[Index].Projectile)
	{
		Units_Armors_Add->Enable(true);
		for(short loop = 0;loop < GenieFile->Civs[UnitCivID].Units[Index].Projectile->Armours.size();loop++)
		{
			CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitArmorName(loop, UnitCivID, Index)).Lower();
			if(SearchMatches(CompareText) == true)
			{
				Name = lexical_cast<string>(loop);
				Name += " - ";
				Name += GetUnitArmorName(loop, UnitCivID, Index);
				Units_Armors_List->Append(Name, (void*)&GenieFile->Civs[UnitCivID].Units[Index].Projectile->Armours[loop]);
			}
		}
	}
	else
	{
		Units_Armors_Add->Enable(false);
	}
	Units_Armors_List->SetSelection(0);
	Units_Armors_List->SetFirstItem(Selection - 3);
	Units_Armors_List->SetSelection(Selection);

	wxCommandEvent E;
	OnUnitArmorsSelect(E);
}

void AGE_Frame::OnUnitArmorsSearch(wxCommandEvent& Event)
{
	ListUnitArmors(UnitID, UnitCivID);
}

void AGE_Frame::OnUnitArmorsSelect(wxCommandEvent& Event)
{
	short Selection = Units_Armors_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(Added)
		{
			Selection = Units_Armors_List->GetCount() - 1;
			Units_Armors_List->SetSelection(Selection);
		}
		gdat::unit::AttackOrArmor * ArmorPointer = (gdat::unit::AttackOrArmor*)Units_Armors_List->GetClientData(Selection);
		ArmorID = ArmorPointer - (&GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Armours[0]);
		Armors_Class->ChangeValue(lexical_cast<string>(ArmorPointer->Class));
		Armors_Class->Container = &ArmorPointer->Class;
		Armors_ComboBox_Class->SetSelection(ArmorPointer->Class);
		Armors_Amount->ChangeValue(lexical_cast<string>(ArmorPointer->Amount));
		Armors_Amount->Container = &ArmorPointer->Amount;
		Added = false;
	}
	else
	{
		Armors_Class->ChangeValue("0");
		Armors_ComboBox_Class->SetSelection(0);
		Armors_Amount->ChangeValue("0");
	}
}

void AGE_Frame::OnUnitArmorsAdd(wxCommandEvent& Event)
{
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		gdat::unit::AttackOrArmor Temp;
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Armours.push_back(Temp);
		}
		Added = true;
		ListUnitArmors(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitArmorsDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Armors_List->GetSelection(); // Gives the current list selection.
	if(Selection != wxNOT_FOUND)
	{
		for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Armours.erase(GenieFile->Civs[loop].Units[UnitID].Projectile->Armours.begin() + ArmorID);
		}
		if(Selection == Units_Armors_List->GetCount() - 1)
		Units_Armors_List->SetSelection(Selection - 1);
		ListUnitArmors(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitArmorsCopy(wxCommandEvent& Event)
{
	short Selection = Units_Armors_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		ArmorCopy = *(gdat::unit::AttackOrArmor*)Units_Armors_List->GetClientData(Selection);
	}
}

void AGE_Frame::OnUnitArmorsPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_Armors_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(AutoCopy == MenuOption_NoAuto)
			GenieFile->Civs[UnitCivID].Units[UnitID].Projectile->Armours[ArmorID] = ArmorCopy;
		else for(short loop = 0;loop < GenieFile->Civs.size();loop++)
		{
			GenieFile->Civs[loop].Units[UnitID].Projectile->Armours[ArmorID] = ArmorCopy;
		}
		ListUnitArmors(UnitID, UnitCivID);
	}
}

//	AoE/TC/SWGB/CC Unit Commands

string AGE_Frame::GetUnitCommandName(short Index, short UnitCivID, short UnitID)
{
	string Name = "";
	short CommandType = -1;
	short CommandSubType = -1;
	if(GameVersion > 1)	// AoK, TC, SWGB or CC
	{
		CommandType = GenieFile->UnitHeaders[UnitID].Commands[Index].Type;
		CommandSubType = GenieFile->UnitHeaders[UnitID].Commands[Index].SubType;
	}
	else	// AoE or RoR
	{
		CommandType = GenieFile->Civs[UnitCivID].Units[UnitID].Bird->Commands[Index].Type;
		CommandSubType = GenieFile->Civs[UnitCivID].Units[UnitID].Bird->Commands[Index].SubType;
	}
	if(CommandType == 3 && CommandSubType == -1)
	{
		Name = "Ability to Garrison";
	}
	else if(CommandType == 5 && CommandSubType == 47)
	{
		Name = "Ability to Mine Gold";
	}
	else if(CommandType == 5 && CommandSubType == 79)
	{
		Name = "Ability to Mine Stone";
	}
	else if(CommandType == 5 && CommandSubType == 190)
	{
		Name = "Ability to Fish, Forage, or Farm";
	}
	else if(CommandType == 5 && CommandSubType == -1)
	{
		Name = "Ability to Rebuild";
	}
	else if(CommandType == 6 && CommandSubType == -1)
	{
		Name = "Type 6, Sub -1";
	}
	else if(CommandType == 7 && CommandSubType == -1)
	{
		Name = "Ability to Attack";
	}
	else if(CommandType == 10 && CommandSubType == -1)
	{
		Name = "Ability to Fly";
	}
	else if(CommandType == 11 && CommandSubType == -1)
	{
		Name = "Type 11, Sub -1";
	}
	else if(CommandType == 12 && CommandSubType == -1)
	{
		Name = "Ability to Unload (Boat-Like)";
	}
	else if(CommandType == 13 && CommandSubType == -1)
	{
		Name = "Ability to Auto-Attack";
	}
	else if(CommandType == 21 && CommandSubType == -1)
	{
		Name = "Type 21, Sub -1";
	}
	else if(CommandType == 101 && CommandSubType == -1)
	{
		Name = "Ability to Build";
	}
	else if(CommandType == 104 && CommandSubType == -1)
	{
		Name = "Ability to Convert";
	}
	else if(CommandType == 105 && CommandSubType == -1)
	{
		Name = "Ability to Heal";
	}
	else if(CommandType == 106 && CommandSubType == -1)
	{
		Name = "Ability to Repair";
	}
	else if(CommandType == 107 && CommandSubType == -1)
	{
		Name = "Type 107, Sub -1";
	}
	else if(CommandType == 109 && CommandSubType == -1)
	{
		Name = "Type 109, Sub -1";
	}
	else if(CommandType == 110 && CommandSubType == 189)
	{
		Name = "Ability to Chop Wood";
	}
	else if(CommandType == 110 && CommandSubType == 190)
	{
		Name = "Ability to Hunt Prey Animals";
	}
	else if(CommandType == 110 && CommandSubType == -1)
	{
		Name = "Ability to Hunt Predator Animals";
	}
	else if(CommandType == 111 && CommandSubType == -1)
	{
		Name = "Ability to Trade";
	}
	else if(CommandType == 120 && CommandSubType == -1)
	{
		Name = "Ability to Generate Wonder Victory*";
	}
	else if(CommandType == 121 && CommandSubType == -1)
	{
		Name = "Type 121, Sub -1";
	}
	else if(CommandType == 122 && CommandSubType == -1)
	{
		Name = "Ability to Mine Porex (Ore)";
	}
	else if(CommandType == 125 && CommandSubType == -1)
	{
		Name = "Ability to Unpack & Attack";
	}
	else if(CommandType == 131 && CommandSubType == -1)
	{
		Name = "Type 131, Sub -1";
	}
	else if(CommandType == 132 && CommandSubType == -1)
	{
		Name = "Ability to Pickup Unit";
	}
	else if(CommandType == 135 && CommandSubType == -1)
	{
		Name = "Type 135, Sub -1";
	}
	else if(CommandType == 136 && CommandSubType == -1)
	{
		Name = "Ability to Deposit Unit";
	}
	else
	{
		Name = "Unknown";
	}
	return Name;
}

void AGE_Frame::ListUnitCommands(short Index, short UnitCivID)
{
	string Name;
	SearchText = wxString(Units_UnitCommands_Search->GetValue()).Lower();
	ExcludeText = wxString(Units_UnitCommands_Search_R->GetValue()).Lower();
	string CompareText;
	short Selection = Units_UnitCommands_List->GetSelection();

	if(!Units_UnitCommands_List->IsEmpty())
	{
		Units_UnitCommands_List->Clear();
	}
	if(Selection == wxNOT_FOUND)
	{
		Selection = 0;
	}
	if(GameVersion >= 2)	// AoK, TC, SWGB or CC
	{
		for(short loop = 0;loop < GenieFile->UnitHeaders[Index].Commands.size();loop++)
		{
			CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitCommandName(loop, 0, Index)).Lower();
			if(SearchMatches(CompareText) == true)
			{
				Name = lexical_cast<string>(loop);
				Name += " - ";
				Name += GetUnitCommandName(loop, 0, Index);
				Units_UnitCommands_List->Append(Name, (void*)&GenieFile->UnitHeaders[Index].Commands[loop]);
			}
		}
	}
	else	// AoE or RoR
	{
		if(GenieFile->Civs[UnitCivID].Units[Index].Bird)
		{
			Units_UnitCommands_Add->Enable(true);
			for(short loop = 0;loop < GenieFile->Civs[UnitCivID].Units[Index].Bird->Commands.size();loop++)
			{
				CompareText = wxString(lexical_cast<string>(loop)+ " - "+GetUnitCommandName(loop, UnitCivID, Index)).Lower();
				if(SearchMatches(CompareText) == true)
				{
					Name = lexical_cast<string>(loop);
					Name += " - ";
					Name += GetUnitCommandName(loop, UnitCivID, Index);
					Units_UnitCommands_List->Append(Name, (void*)&GenieFile->Civs[UnitCivID].Units[Index].Bird->Commands[loop]);
				}
			}
		}
		else
		{
			Units_UnitCommands_Add->Enable(false);
		}
	}
	Units_UnitCommands_List->SetSelection(0);
	Units_UnitCommands_List->SetFirstItem(Selection - 3);
	Units_UnitCommands_List->SetSelection(Selection);
	
	wxCommandEvent E;
	OnUnitCommandsSelect(E);
}

void AGE_Frame::OnUnitCommandsSearch(wxCommandEvent& Event)
{
	ListUnitCommands(UnitID, UnitCivID);
}

void AGE_Frame::OnUnitCommandsSelect(wxCommandEvent& Event)
{
	short Selection = Units_UnitCommands_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(Added)
		{
			Selection = Units_UnitCommands_List->GetCount() - 1;
			Units_UnitCommands_List->SetSelection(Selection);
		}
		gdat::UnitCommand * UnitCommandPointer = (gdat::UnitCommand*)Units_UnitCommands_List->GetClientData(Selection);
		if(GameVersion >= 2)
		{
			CommandID = UnitCommandPointer - (&GenieFile->UnitHeaders[UnitID].Commands[0]);
		}
		else
		{
			CommandID = UnitCommandPointer - (&GenieFile->Civs[UnitCivID].Units[UnitID].Bird->Commands[0]);
		}
		UnitCommands_One->ChangeValue(lexical_cast<string>(UnitCommandPointer->One));
		UnitCommands_One->Container = &UnitCommandPointer->One;
		UnitCommands_ID->ChangeValue(lexical_cast<string>(UnitCommandPointer->ID));
		UnitCommands_ID->Container = &UnitCommandPointer->ID;
		UnitCommands_Unknown1->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown1));
		UnitCommands_Unknown1->Container = &UnitCommandPointer->Unknown1;
		UnitCommands_Type->ChangeValue(lexical_cast<string>(UnitCommandPointer->Type));
		UnitCommands_Type->Container = &UnitCommandPointer->Type;
		UnitCommands_SubType->ChangeValue(lexical_cast<string>(UnitCommandPointer->SubType));
		UnitCommands_SubType->Container = &UnitCommandPointer->SubType;
		if(UnitCommandPointer->Type == 3 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(1);
		}
		else if(UnitCommandPointer->Type == 5 && UnitCommandPointer->SubType == 47)
		{
			UnitCommands_ComboBox_Types->SetSelection(2);
		}
		else if(UnitCommandPointer->Type == 5 && UnitCommandPointer->SubType == 79)
		{
			UnitCommands_ComboBox_Types->SetSelection(3);
		}
		else if(UnitCommandPointer->Type == 5 && UnitCommandPointer->SubType == 190)
		{
			UnitCommands_ComboBox_Types->SetSelection(4);
		}
		else if(UnitCommandPointer->Type == 5 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(5);
		}
		else if(UnitCommandPointer->Type == 6 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(6);
		}
		else if(UnitCommandPointer->Type == 7 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(7);
		}
		else if(UnitCommandPointer->Type == 10 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(8);
		}
		else if(UnitCommandPointer->Type == 11 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(9);
		}
		else if(UnitCommandPointer->Type == 12 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(10);
		}
		else if(UnitCommandPointer->Type == 13 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(11);
		}
		else if(UnitCommandPointer->Type == 21 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(12);
		}
		else if(UnitCommandPointer->Type == 101 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(13);
		}
		else if(UnitCommandPointer->Type == 104 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(14);
		}
		else if(UnitCommandPointer->Type == 105 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(15);
		}
		else if(UnitCommandPointer->Type == 106 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(16);
		}
		else if(UnitCommandPointer->Type == 107 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(17);
		}
		else if(UnitCommandPointer->Type == 109 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(18);
		}
		else if(UnitCommandPointer->Type == 110 && UnitCommandPointer->SubType == 189)
		{
			UnitCommands_ComboBox_Types->SetSelection(19);
		}
		else if(UnitCommandPointer->Type == 110 && UnitCommandPointer->SubType == 190)
		{
			UnitCommands_ComboBox_Types->SetSelection(20);
		}
		else if(UnitCommandPointer->Type == 110 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(21);
		}
		else if(UnitCommandPointer->Type == 111 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(22);
		}
		else if(UnitCommandPointer->Type == 120 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(23);
		}
		else if(UnitCommandPointer->Type == 121 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(24);
		}
		else if(UnitCommandPointer->Type == 122 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(25);
		}
		else if(UnitCommandPointer->Type == 125 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(26);
		}
		else if(UnitCommandPointer->Type == 131 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(27);
		}
		else if(UnitCommandPointer->Type == 132 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(28);
		}
		else if(UnitCommandPointer->Type == 135 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(29);
		}
		else if(UnitCommandPointer->Type == 136 && UnitCommandPointer->SubType == -1)
		{
			UnitCommands_ComboBox_Types->SetSelection(30);
		}
		else
		{
			UnitCommands_ComboBox_Types->SetSelection(0);
		}
		UnitCommands_ClassID->ChangeValue(lexical_cast<string>(UnitCommandPointer->ClassID));
		UnitCommands_ClassID->Container = &UnitCommandPointer->ClassID;
		UnitCommands_ComboBox_ClassID->SetSelection(UnitCommandPointer->ClassID + 1);
		UnitCommands_UnitID->ChangeValue(lexical_cast<string>(UnitCommandPointer->UnitID));
		UnitCommands_UnitID->Container = &UnitCommandPointer->UnitID;
		UnitCommands_ComboBox_UnitID->SetSelection(UnitCommandPointer->UnitID + 1);
		UnitCommands_Unknown2->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown2));
		UnitCommands_Unknown2->Container = &UnitCommandPointer->Unknown2;
		UnitCommands_ResourceIn->ChangeValue(lexical_cast<string>(UnitCommandPointer->ResourceIn));
		UnitCommands_ResourceIn->Container = &UnitCommandPointer->ResourceIn;
		UnitCommands_ComboBox_ResourceIn->SetSelection(UnitCommandPointer->ResourceIn + 1);
		UnitCommands_ResourceOut->ChangeValue(lexical_cast<string>(UnitCommandPointer->ResourceOut));
		UnitCommands_ResourceOut->Container = &UnitCommandPointer->ResourceOut;
		UnitCommands_ComboBox_ResourceOut->SetSelection(UnitCommandPointer->ResourceOut + 1);
		UnitCommands_Unknown3->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown3));
		UnitCommands_Unknown3->Container = &UnitCommandPointer->Unknown3;
		UnitCommands_Unknown4->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown4));
		UnitCommands_Unknown4->Container = &UnitCommandPointer->Unknown4;
		UnitCommands_ExecutionRadius->ChangeValue(lexical_cast<string>(UnitCommandPointer->ExecutionRadius));
		UnitCommands_ExecutionRadius->Container = &UnitCommandPointer->ExecutionRadius;
		UnitCommands_Unknown5->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown5));
		UnitCommands_Unknown5->Container = &UnitCommandPointer->Unknown5;
		UnitCommands_Unknown6->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown6));
		UnitCommands_Unknown6->Container = &UnitCommandPointer->Unknown6;
		UnitCommands_Unknown7->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown7));
		UnitCommands_Unknown7->Container = &UnitCommandPointer->Unknown7;
		UnitCommands_Unknown8->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown8));
		UnitCommands_Unknown8->Container = &UnitCommandPointer->Unknown8;
		UnitCommands_Unknown9->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown9));
		UnitCommands_Unknown9->Container = &UnitCommandPointer->Unknown9;
		UnitCommands_Unknown10->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown10));
		UnitCommands_Unknown10->Container = &UnitCommandPointer->Unknown10;
		UnitCommands_Unknown11->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown11));
		UnitCommands_Unknown11->Container = &UnitCommandPointer->Unknown11;
		UnitCommands_Unknown12->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown12));
		UnitCommands_Unknown12->Container = &UnitCommandPointer->Unknown12;
		UnitCommands_Unknown13->ChangeValue(lexical_cast<string>(UnitCommandPointer->Unknown13));
		UnitCommands_Unknown13->Container = &UnitCommandPointer->Unknown13;
		UnitCommands_Unknown14->ChangeValue(lexical_cast<string>((short)UnitCommandPointer->Unknown14));
		UnitCommands_Unknown14->Container = &UnitCommandPointer->Unknown14;
		for(short loop = 0;loop < 6;loop++)
		{
			UnitCommands_Graphics[loop]->ChangeValue(lexical_cast<string>(UnitCommandPointer->Graphics[loop]));
			UnitCommands_Graphics[loop]->Container = &UnitCommandPointer->Graphics[loop];
			UnitCommands_ComboBox_Graphics[loop]->SetSelection(UnitCommandPointer->Graphics[loop] + 1);
		}
		Added = false;
	}
	else
	{
		UnitCommands_One->ChangeValue("0");
		UnitCommands_ID->ChangeValue("0");
		UnitCommands_Unknown1->ChangeValue("0");
		UnitCommands_Type->ChangeValue("0");
		UnitCommands_ComboBox_Types->SetSelection(0);
		UnitCommands_ClassID->ChangeValue("0");
		UnitCommands_ComboBox_ClassID->SetSelection(0);
		UnitCommands_UnitID->ChangeValue("0");
		UnitCommands_ComboBox_UnitID->SetSelection(0);
		UnitCommands_Unknown2->ChangeValue("0");
		UnitCommands_ResourceIn->ChangeValue("0");
		UnitCommands_ComboBox_ResourceIn->SetSelection(0);
		UnitCommands_SubType->ChangeValue("0");
		UnitCommands_ResourceOut->ChangeValue("0");
		UnitCommands_ComboBox_ResourceOut->SetSelection(0);
		UnitCommands_Unknown3->ChangeValue("0");
		UnitCommands_Unknown4->ChangeValue("0");
		UnitCommands_ExecutionRadius->ChangeValue("0");
		UnitCommands_Unknown5->ChangeValue("0");
		UnitCommands_Unknown6->ChangeValue("0");
		UnitCommands_Unknown7->ChangeValue("0");
		UnitCommands_Unknown8->ChangeValue("0");
		UnitCommands_Unknown9->ChangeValue("0");
		UnitCommands_Unknown10->ChangeValue("0");
		UnitCommands_Unknown11->ChangeValue("0");
		UnitCommands_Unknown12->ChangeValue("0");
		UnitCommands_Unknown13->ChangeValue("0");
		UnitCommands_Unknown14->ChangeValue("0");
		for(short loop = 0;loop < 6;loop++)
		{
			UnitCommands_Graphics[loop]->ChangeValue("0");
			UnitCommands_ComboBox_Graphics[loop]->SetSelection(0);
		}
	}
}

void AGE_Frame::OnUnitCommandsAdd(wxCommandEvent& Event)
{
	gdat::UnitCommand Temp;
	short Selection = Units_Units_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(GameVersion > 1)
		{
			GenieFile->UnitHeaders[UnitID].Commands.push_back(Temp);
			GenieFile->UnitHeaders[UnitID].Commands[GenieFile->UnitHeaders[UnitID].Commands.size() - 1].ID = lexical_cast<short>(GenieFile->UnitHeaders[UnitID].Commands.size() - 1);	//	ID Fix
		}
		else
		{
			for(short loop = 0;loop < GenieFile->Civs.size();loop++)
			{
				GenieFile->Civs[loop].Units[UnitID].Bird->Commands.push_back(Temp);
				GenieFile->Civs[loop].Units[UnitID].Bird->Commands[GenieFile->Civs[0].Units[UnitID].Bird->Commands.size() - 1].ID = lexical_cast<short>(GenieFile->Civs[0].Units[UnitID].Bird->Commands.size() - 1);	//	ID Fix
			}
		}
		Added = true;
		ListUnitCommands(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitCommandsDelete(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_UnitCommands_List->GetSelection(); // Gives the current list selection.
	if(Selection != wxNOT_FOUND)
	{
		if(GameVersion > 1)
		{
			GenieFile->UnitHeaders[UnitID].Commands.erase(GenieFile->UnitHeaders[UnitID].Commands.begin() + CommandID);
			for(short loop2 = CommandID;loop2 < GenieFile->UnitHeaders[UnitID].Commands.size();loop2++)	//	ID Fix
			{
				GenieFile->UnitHeaders[UnitID].Commands[loop2].ID = lexical_cast<short>(loop2);
			}
		}
		else
		{
			for(short loop = 0;loop < GenieFile->Civs.size();loop++)
			{
				GenieFile->Civs[loop].Units[UnitID].Bird->Commands.erase(GenieFile->Civs[loop].Units[UnitID].Bird->Commands.begin() + CommandID);
				for(short loop2 = CommandID;loop2 < GenieFile->Civs[0].Units[UnitID].Bird->Commands.size();loop2++)	//	ID Fix
				{
					GenieFile->Civs[loop].Units[UnitID].Bird->Commands[loop2].ID = lexical_cast<short>(loop2);
				}
			}
		}
		if(Selection == Units_UnitCommands_List->GetCount() - 1)
		Units_UnitCommands_List->SetSelection(Selection - 1);
		ListUnitCommands(UnitID, UnitCivID);
	}
}

void AGE_Frame::OnUnitCommandsCopy(wxCommandEvent& Event)
{
	short Selection = Units_UnitCommands_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		UnitCommandCopy = *(gdat::UnitCommand*)Units_UnitCommands_List->GetClientData(Selection);
	}
}

void AGE_Frame::OnUnitCommandsPaste(wxCommandEvent& Event)
{
	wxBusyCursor WaitCursor;
	short Selection = Units_UnitCommands_List->GetSelection();
	if(Selection != wxNOT_FOUND)
	{
		if(GameVersion > 1)
		{
			*(gdat::UnitCommand*)Units_UnitCommands_List->GetClientData(Selection) = UnitCommandCopy;
			GenieFile->UnitHeaders[UnitID].Commands[CommandID].ID = lexical_cast<short>(CommandID);	//	ID Fix
		}
		else
		{
			if(AutoCopy == MenuOption_NoAuto)
				GenieFile->Civs[UnitCivID].Units[UnitID].Bird->Commands[CommandID] = UnitCommandCopy;
			for(short loop = 0;loop < GenieFile->Civs.size();loop++)
			{
				if(AutoCopy != MenuOption_NoAuto)
				GenieFile->Civs[loop].Units[UnitID].Bird->Commands[CommandID] = UnitCommandCopy;
				GenieFile->Civs[loop].Units[UnitID].Bird->Commands[CommandID].ID = lexical_cast<short>(CommandID);	//	ID Fix
			}
		}
		ListUnitCommands(UnitID, UnitCivID);
	}
}

void AGE_Frame::CreateUnitControls()
{
//	UnitControls new things

	Tab_Units = new wxPanel(TabBar_Main, wxID_ANY, wxDefaultPosition, wxSize(-1, 350));
	
	Units_Main = new wxBoxSizer(wxHORIZONTAL);
	Units_ListArea = new wxBoxSizer(wxVERTICAL);
	Units_Units = new wxStaticBoxSizer(wxVERTICAL, Tab_Units, "Unit slot");
	Units_Civs_List = new wxComboBox(Tab_Units, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	Units_Units_Search = new wxTextCtrl(Tab_Units, wxID_ANY);
	Units_Units_Search_R = new wxTextCtrl(Tab_Units, wxID_ANY);
	for(short loop = 0;loop < 2;loop++)
	{
		Units_Units_Searches[loop] = new wxBoxSizer(wxHORIZONTAL);
		Units_Units_SearchFilters[loop] = new wxOwnerDrawnComboBox(Tab_Units, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
		Units_Units_UseAnd[loop] = new wxCheckBox(Tab_Units, wxID_ANY, "And", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
	}
	Units_Units_List = new wxListBox(Tab_Units, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	Units_Units_Buttons = new wxGridSizer(2, 0, 0);
	Units_Add = new wxButton(Tab_Units, wxID_ANY, "Add", wxDefaultPosition, wxSize(-1, 20));
	Units_Delete = new wxButton(Tab_Units, wxID_ANY, "Delete", wxDefaultPosition, wxSize(-1, 20));
	Units_Copy = new wxButton(Tab_Units, wxID_ANY, "Copy", wxDefaultPosition, wxSize(-1, 20));
	Units_Paste = new wxButton(Tab_Units, wxID_ANY, "Paste", wxDefaultPosition, wxSize(-1, 20));

	Units_DataArea = new wxBoxSizer(wxVERTICAL);
	Units_MainRow1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TopRow = new wxStaticBoxSizer(wxHORIZONTAL, Tab_Units, "Other");
	Units_Holder_Type = new wxBoxSizer(wxHORIZONTAL);
	Units_AutoCopyState = new wxStaticText(Tab_Units, wxID_ANY, "Undefined", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Scroller = new AGE_ScrolledWindow(Tab_Units, wxVSCROLL | wxTAB_TRAVERSAL);
	Units_ScrollerWindows = new wxBoxSizer(wxHORIZONTAL);
	Units_ScrollerWindowsSpace = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TypeArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type");
	Units_Holder_TypeArea1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TypeArea2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_LangDLLArea = new wxStaticBoxSizer(wxHORIZONTAL, Units_Scroller, "Language DLLs");
	Units_Holder_GraphicsArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Graphics");
	Units_Holder_GraphicsArea1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_GraphicsArea2 = new wxGridSizer(4, 0, 5);
	Units_Holder_GraphicsArea3 = new wxGridSizer(4, 0, 5);
	Units_Holder_GraphicsArea4 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_GraphicsArea5 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_GraphicsArea6 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GraphicsArea7 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_StatsArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Statistics");
	Units_Holder_StatsArea1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_StatsAreaRanges = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_StatsAreaGarrison = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_StatsArea2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Projectiles");
	Units_Holder_ProjectilesArea1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea3 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea4 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea5 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea6 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ProjectilesArea7 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attributes = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Attributes");
	Units_Holder_AttributesBoxes1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesBoxes2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attributes1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesTerrain = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesModes1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesDropSite = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttributesSizes = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesSelection1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesSelection2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attributes2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesHotKey = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttributesTracking = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesTrain1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesTrain2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attributes3 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttributesAnnexUnit = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttributesAnnexUnit2 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Attributes4 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_SoundsArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Sounds");
	Units_Holder_SoundsArea1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_SoundsArea2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_MiscArea = new wxStaticBoxSizer(wxHORIZONTAL, Units_Scroller, "Miscellaneous");
	Units_Holder_UnknownArea = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Type10plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 10+ Unknowns");
	Units_Holder_Type10plusUnknowns1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Grid_Type10plusUnknowns = new wxGridSizer(4, 5, 5);
	Units_Holder_Type30plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 30+ Unknowns");
	Units_Grid_Type30plusUnknownArea = new wxGridSizer(4, 5, 5);
	Units_Holder_Type40plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 40+ Unknowns");
	Units_Holder_Type60plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 60+ Unknowns");
	Units_Grid_Type60plusUnknownArea = new wxGridSizer(4, 5, 5);
	Units_Holder_Type70plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 70+ Unknowns");
	Units_Grid_Type70plusUnknownArea = new wxGridSizer(4, 5, 5);
	Units_Holder_Type80plusUnknownArea = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Type 80+ Unknowns");
	Units_Grid_Type80plusUnknownArea = new wxGridSizer(4, 5, 5);
	Units_Holder_CommandsArea = new wxStaticBoxSizer(wxHORIZONTAL, Units_Scroller, "Commands");
	
//	Invisible Holder Windows
//	Type 10+

	Units_Holder_NameLength = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ID1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_LanguageDllName = new wxBoxSizer(wxVERTICAL);
	Units_Holder_LanguageDllCreation = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Class = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_StandingGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DyingGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DeathMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_HitPoints = new wxBoxSizer(wxVERTICAL);
	Units_Holder_LineOfSight = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GarrisonCapacity = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SizeRadius = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_HPBarHeight1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TrainSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrainSound1 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrainSound2 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrainSound3 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_DeadUnitID = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_PlacementMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AirMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_IconID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_HideInEditor = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown1 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Enabled = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_PlacementBypassTerrain = new wxBoxSizer(wxVERTICAL);
	Units_Holder_PlacementBypassTerrainGrid = new wxGridSizer(2, 0, 5);
	Units_Holder_PlacementTerrain = new wxBoxSizer(wxVERTICAL);
	Units_Holder_PlacementTerrainGrid = new wxGridSizer(2, 0, 5);
	Units_Holder_EditorRadius = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_BuildingMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_VisibleInFog = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TerrainRestriction = new wxBoxSizer(wxVERTICAL);
	Units_Holder_FlyMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ResourceCapacity = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ResourceDecay = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown2 = new wxBoxSizer(wxVERTICAL);
	Units_Grid_Unknown2 = new wxGridSizer(2, 0, 0);
	Units_Holder_InteractionMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_MinimapMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_CommandAttribute = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown3 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown3a = new wxBoxSizer(wxVERTICAL);
	Units_Holder_LanguageDllHelp = new wxBoxSizer(wxVERTICAL);
	Units_Holder_HotKey = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown4 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown5 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unselectable = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown6 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown7 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown8 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SelectionMask = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SelectionShapeType = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SelectionShape = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown9 = new wxBoxSizer(wxVERTICAL);
	Units_Grid_Unknown9 = new wxGridSizer(4, 0, 0);
	Units_Holder_SelectionEffect = new wxBoxSizer(wxVERTICAL);
	Units_Holder_EditorSelectionColour = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SelectionRadius = new wxBoxSizer(wxVERTICAL);
	Units_Holder_HPBarHeight2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ResourceStorageHeader = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Resource Storage slot");
	Units_Grid_ResourceStorage = new wxGridSizer(3, 0, 5);
	for(short loop = 0;loop < 3;loop++)
	Units_Holder_ResourceStorage[loop] = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_SelectionSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DyingSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Name = new wxBoxSizer(wxVERTICAL);
	Units_Holder_NameLength2 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Name2 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unitline = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_MinTechLevel = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ID2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ID3 = new wxBoxSizer(wxHORIZONTAL);

//	Type 20+

	Units_Holder_Speed = new wxBoxSizer(wxVERTICAL);

//	Type 30+

	Units_Holder_WalkingGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_RotationSpeed = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown11 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrackingUnit = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrackingUnitBox = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TrackingUnitUsed = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrackingUnitUsedBox = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TrackingUnitDensity = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown12 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown16 = new wxBoxSizer(wxHORIZONTAL);

//	Type 40+

	Units_Holder_SheepConversionBox = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_SheepConversion = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SearchRadius = new wxBoxSizer(wxVERTICAL);
	Units_Holder_WorkRate = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DropSite = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_VillagerMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_MoveSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_StopSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown19 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Exists = new wxBoxSizer(wxVERTICAL);

//	Type 60+

	Units_Holder_Unknown20 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown21 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_MaxRange = new wxBoxSizer(wxVERTICAL);
	Units_Holder_BlastRadius = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ReloadTime1 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ProjectileUnitID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AccuracyPercent = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown22 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Delay = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GraphicDisplacement = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown23 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_MinRange = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GarrisonRecoveryRate = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DisplayedMeleeArmour = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DisplayedAttack = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DisplayedRange = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ReloadTime2 = new wxBoxSizer(wxVERTICAL);

//	Type 60 only

	Units_Holder_StretchMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_CompensationMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DropAnimationMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_PenetrationMode = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown24 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown25 = new wxBoxSizer(wxVERTICAL);

//	Type 70+

	Units_Holder_CostHeader = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Cost slot");
	Units_Holder_CostType = new wxBoxSizer(wxHORIZONTAL);
	Units_Grid_CostType = new wxGridSizer(3, 0, 5);
	Units_Holder_CostAmount = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_CostUsed = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TrainTime = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrainLocationID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TrainLocationID1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_ButtonID = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown26 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown27 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown28 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_MissileGraphicDelay = new wxBoxSizer(wxVERTICAL);
	Units_Holder_HeroMode = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_GarrisonGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackMissileDuplicationAmount1 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackMissileDuplicationAmount2 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackMissileDuplicationUnknown = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_AttackMissileDuplicationUnit = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AttackMissileDuplicationGraphic = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown29 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DisplayedPierceArmour = new wxBoxSizer(wxVERTICAL);

//	Type 80

	Units_Holder_ConstructionGraphicID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_SnowGraphicID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown30 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown31 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_StackUnitID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_TerrainID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown32 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ResearchID = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown33 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_AnnexUnit1 = new wxBoxSizer(wxVERTICAL);
	Units_Grid_AnnexUnit = new wxGridSizer(4, 0, 5);
	Units_Holder_AnnexUnitMisplacement1 = new wxBoxSizer(wxVERTICAL);
	Units_Grid_AnnexUnitMisplacement = new wxGridSizer(4, 0, 5);
	Units_Holder_HeadUnit = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_TransformUnit = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Unknown34 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_ConstructionSound = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GarrisonType = new wxBoxSizer(wxVERTICAL);
	Units_Holder_GarrisonType1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_GarrisonHealRate = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown35 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown36 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Unknown37 = new wxBoxSizer(wxHORIZONTAL);

//	Data Container Names
//	Type 10+

	Units_Text_Type = new wxStaticText(Tab_Units, wxID_ANY, "Type ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_NameLength = new wxStaticText(Units_Scroller, wxID_ANY, " Name Length ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ID1 = new wxStaticText(Units_Scroller, wxID_ANY, "ID 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_LanguageDllName = new wxStaticText(Units_Scroller, wxID_ANY, " Language Dll Name ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_LanguageDllCreation = new wxStaticText(Units_Scroller, wxID_ANY, " Language Dll Creation ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Class = new wxStaticText(Units_Scroller, wxID_ANY, "Class ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_StandingGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Standing Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DyingGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Dying Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_HitPoints = new wxStaticText(Units_Scroller, wxID_ANY, " Hit Points ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_LineOfSight = new wxStaticText(Units_Scroller, wxID_ANY, " Line Of Sight ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GarrisonCapacity = new wxStaticText(Units_Scroller, wxID_ANY, " Garrison Capacity ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SizeRadius = new wxStaticText(Units_Scroller, wxID_ANY, "Size Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_HPBarHeight1 = new wxStaticText(Units_Scroller, wxID_ANY, "HP Bar Height 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TrainSound = new wxStaticText(Units_Scroller, wxID_ANY, " Train Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DeadUnitID = new wxStaticText(Units_Scroller, wxID_ANY, "Dead Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_PlacementMode = new wxStaticText(Units_Scroller, wxID_ANY, "Placement Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_IconID = new wxStaticText(Units_Scroller, wxID_ANY, " Icon ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown1 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_PlacementBypassTerrain = new wxStaticText(Units_Scroller, wxID_ANY, " Placement Bypass Terrain ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_PlacementTerrain = new wxStaticText(Units_Scroller, wxID_ANY, " Placement Terrain ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_EditorRadius = new wxStaticText(Units_Scroller, wxID_ANY, "Editor Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_BuildingMode = new wxStaticText(Units_Scroller, wxID_ANY, "Building Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TerrainRestriction = new wxStaticText(Units_Scroller, wxID_ANY, " Terrain Restriction ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ResourceCapacity = new wxStaticText(Units_Scroller, wxID_ANY, " Resource Capacity ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ResourceDecay = new wxStaticText(Units_Scroller, wxID_ANY, " Resource Decay ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown2 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 2", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_InteractionMode = new wxStaticText(Units_Scroller, wxID_ANY, " Interaction Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MinimapMode = new wxStaticText(Units_Scroller, wxID_ANY, " Minimap Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_CommandAttribute = new wxStaticText(Units_Scroller, wxID_ANY, " Command Attribute ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown3 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 3 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown3a = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 3a ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_LanguageDllHelp = new wxStaticText(Units_Scroller, wxID_ANY, " Language Dll Help ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_HotKey = new wxStaticText(Units_Scroller, wxID_ANY, "Hot Key ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown4 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 4 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown5 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 5 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown6 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 6 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown7 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 7 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown8 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 8 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionMask = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Mask ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionShapeType = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Shape Type ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionShape = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Shape ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown9 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 9", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionEffect = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Effect ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_EditorSelectionColour = new wxStaticText(Units_Scroller, wxID_ANY, " Editor Selection Colour ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionRadius = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_HPBarHeight2 = new wxStaticText(Units_Scroller, wxID_ANY, "HP Bar Height 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	ResourceStorage_Text_Type = new wxStaticText(Units_Scroller, wxID_ANY, "Type ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	ResourceStorage_Text_Amount = new wxStaticText(Units_Scroller, wxID_ANY, "Amount ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	ResourceStorage_Text_Enabled = new wxStaticText(Units_Scroller, wxID_ANY, "Enable Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	Units_Text_SelectionSound = new wxStaticText(Units_Scroller, wxID_ANY, " Selection Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DyingSound = new wxStaticText(Units_Scroller, wxID_ANY, " Dying Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackSound = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Name = new wxStaticText(Units_Scroller, wxID_ANY, " Name ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_NameLength2 = new wxStaticText(Units_Scroller, wxID_ANY, " Name Length 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Name2 = new wxStaticText(Units_Scroller, wxID_ANY, " Name 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unitline = new wxStaticText(Units_Scroller, wxID_ANY, "Unitline ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MinTechLevel = new wxStaticText(Units_Scroller, wxID_ANY, "Min Tech Level ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ID2 = new wxStaticText(Units_Scroller, wxID_ANY, "ID 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ID3 = new wxStaticText(Units_Scroller, wxID_ANY, "ID 3 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 20+

	Units_Text_Speed = new wxStaticText(Units_Scroller, wxID_ANY, " Speed ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 30+

	Units_Text_WalkingGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Walking Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_RotationSpeed = new wxStaticText(Units_Scroller, wxID_ANY, " Rotation Speed ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown11 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 11 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TrackingUnit = new wxStaticText(Units_Scroller, wxID_ANY, " Tracking Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TrackingUnitUsed = new wxStaticText(Units_Scroller, wxID_ANY, " Tracking Unit Used ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TrackingUnitDensity = new wxStaticText(Units_Scroller, wxID_ANY, " Tracking Unit Density ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown12 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 12 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown16 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 16 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 40+

	Units_Text_SheepConversion = new wxStaticText(Units_Scroller, wxID_ANY, " Sheep Conversion ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SearchRadius = new wxStaticText(Units_Scroller, wxID_ANY, " Search Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_WorkRate = new wxStaticText(Units_Scroller, wxID_ANY, " Work Rate ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DropSite = new wxStaticText(Units_Scroller, wxID_ANY, "Drop Site ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_VillagerMode = new wxStaticText(Units_Scroller, wxID_ANY, " Villager Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MoveSound = new wxStaticText(Units_Scroller, wxID_ANY, " Move Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_StopSound = new wxStaticText(Units_Scroller, wxID_ANY, " Stop Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown19 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 19 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Exists = new wxStaticText(Units_Scroller, wxID_ANY, " Exists", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 60+

	Units_Text_Unknown20 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 20 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown21 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 21 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MaxRange = new wxStaticText(Units_Scroller, wxID_ANY, " Max Range ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_BlastRadius = new wxStaticText(Units_Scroller, wxID_ANY, " Blast Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ReloadTime1 = new wxStaticText(Units_Scroller, wxID_ANY, " Reload Time 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ProjectileUnitID = new wxStaticText(Units_Scroller, wxID_ANY, " Projectile Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AccuracyPercent = new wxStaticText(Units_Scroller, wxID_ANY, " Accuracy Percent ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown22 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 22 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Delay = new wxStaticText(Units_Scroller, wxID_ANY, " Delay ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GraphicDisplacement = new wxStaticText(Units_Scroller, wxID_ANY, "Graphic Displacement XYZ ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown23 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 23 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MinRange = new wxStaticText(Units_Scroller, wxID_ANY, " Min Range ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GarrisonRecoveryRate = new wxStaticText(Units_Scroller, wxID_ANY, " Garrison Recovery Rate ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DisplayedMeleeArmour = new wxStaticText(Units_Scroller, wxID_ANY, " Displayed Melee Armour ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DisplayedAttack = new wxStaticText(Units_Scroller, wxID_ANY, " Displayed Attack ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DisplayedRange = new wxStaticText(Units_Scroller, wxID_ANY, " Displayed Range ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ReloadTime2 = new wxStaticText(Units_Scroller, wxID_ANY, " Reload Time 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 60 only

	Units_Text_StretchMode = new wxStaticText(Units_Scroller, wxID_ANY, " Stretch Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_CompensationMode = new wxStaticText(Units_Scroller, wxID_ANY, " Compensation Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DropAnimationMode = new wxStaticText(Units_Scroller, wxID_ANY, " DropAnimation Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_PenetrationMode = new wxStaticText(Units_Scroller, wxID_ANY, " Penetration Mode ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown24 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 24 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown25 = new wxStaticText(Units_Scroller, wxID_ANY, " Projectile Arc ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 70+

	Units_Text_CostType = new wxStaticText(Units_Scroller, wxID_ANY, "Cost Type ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	Units_Text_CostAmount = new wxStaticText(Units_Scroller, wxID_ANY, "Cost Amount ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	Units_Text_CostUsed = new wxStaticText(Units_Scroller, wxID_ANY, "Cost Used ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	Units_Text_TrainTime = new wxStaticText(Units_Scroller, wxID_ANY, " Train Time ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TrainLocationID = new wxStaticText(Units_Scroller, wxID_ANY, " Train Location ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ButtonID = new wxStaticText(Units_Scroller, wxID_ANY, "Button ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown26 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 26 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown27 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 27 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown28 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 28 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_MissileGraphicDelay = new wxStaticText(Units_Scroller, wxID_ANY, " Missile Graphic Delay ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GarrisonGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Garrison Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackMissileDuplicationAmount1 = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Missile Duplication Amount 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackMissileDuplicationAmount2 = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Missile Duplication Amount 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackMissileDuplicationUnknown = new wxStaticText(Units_Scroller, wxID_ANY, "Attack Missile Duplication Unknown ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackMissileDuplicationUnit = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Missile Duplication Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AttackMissileDuplicationGraphic = new wxStaticText(Units_Scroller, wxID_ANY, " Attack Missile Duplication Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown29 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 29 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_DisplayedPierceArmour = new wxStaticText(Units_Scroller, wxID_ANY, " Displayed Pierce Armour ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Type 80

	Units_Text_ConstructionGraphicID = new wxStaticText(Units_Scroller, wxID_ANY, " Construction Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_SnowGraphicID = new wxStaticText(Units_Scroller, wxID_ANY, " Snow Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown30 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 30 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown31 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 31 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_StackUnitID = new wxStaticText(Units_Scroller, wxID_ANY, " Stack Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TerrainID = new wxStaticText(Units_Scroller, wxID_ANY, " Terrain ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown32 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 32 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ResearchID = new wxStaticText(Units_Scroller, wxID_ANY, " Research ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown33 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 33 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AnnexUnit = new wxStaticText(Units_Scroller, wxID_ANY, " Annex Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_AnnexUnitMisplacement = new wxStaticText(Units_Scroller, wxID_ANY, " Annex Unit Misplacement ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_HeadUnit = new wxStaticText(Units_Scroller, wxID_ANY, "Head Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_TransformUnit = new wxStaticText(Units_Scroller, wxID_ANY, "Transform Unit ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown34 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 34 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_ConstructionSound = new wxStaticText(Units_Scroller, wxID_ANY, " Construction Sound ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GarrisonType = new wxStaticText(Units_Scroller, wxID_ANY, " Garrison Type ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_GarrisonHealRate = new wxStaticText(Units_Scroller, wxID_ANY, " Garrison Heal Rate ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown35 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 35 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown36 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 36 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_Text_Unknown37 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 37 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

//	Data Containers
//	Type 10+

	for(short loop = 0;loop < 2;loop++)
	{
		Units_StandingGraphic[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_StandingGraphic[loop] = new ComboBox_Short(Units_Scroller, Units_StandingGraphic[loop]);
		Units_DyingGraphic[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_DyingGraphic[loop] = new ComboBox_Short(Units_Scroller, Units_DyingGraphic[loop]);
		Units_SizeRadius[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
		Units_TrainSound[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_TrainSound[loop] = new ComboBox_Short(Units_Scroller, Units_TrainSound[loop]);
		Units_PlacementBypassTerrain[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_PlacementBypassTerrain[loop] = new ComboBox_Short(Units_Scroller, Units_PlacementBypassTerrain[loop]);
		Units_PlacementTerrain[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_PlacementTerrain[loop] = new ComboBox_Short(Units_Scroller, Units_PlacementTerrain[loop]);
		Units_EditorRadius[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
		Units_Unknown2[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);
		Units_SelectionRadius[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
	}
	for(short loop = 0;loop < 3;loop++)
	{
		ResourceStorage_Type[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		ResourceStorage_ComboBox_Type[loop] = new ComboBox_Short(Units_Scroller, ResourceStorage_Type[loop]);
		ResourceStorage_Amount[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
		ResourceStorage_Enabled[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	}
	for(short loop = 0;loop < 4;loop++)
	{
		Units_HotKey[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_Unknown9[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	}
	Units_Type = new wxTextCtrl(Tab_Units, wxID_ANY);
	Units_ComboBox_Type = new wxOwnerDrawnComboBox(Tab_Units, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	Units_NameLength = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ID1 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_LanguageDllName = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_DLL_LanguageDllName = new wxStaticText(Units_Scroller, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE | wxALIGN_CENTRE_HORIZONTAL);
	Units_LanguageDllCreation = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_DLL_LanguageDllCreation = new wxStaticText(Units_Scroller, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE | wxALIGN_CENTRE_HORIZONTAL);
	Units_Class = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_Class = new ComboBox_Short(Units_Scroller, Units_Class);
	Units_DeathMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_DeathMode = new CheckBox_Byte(Units_Scroller, "Death: Revives", Units_DeathMode);
	Units_HitPoints = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_LineOfSight = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_GarrisonCapacity = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_HPBarHeight1 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_DeadUnitID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_DeadUnitID = new ComboBox_Short(Units_Scroller, Units_DeadUnitID);
	Units_PlacementMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_AirMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_AirMode = new CheckBox_Byte(Units_Scroller, "Air Mode", Units_AirMode);
	Units_IconID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_HideInEditor = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_HideInEditor = new CheckBox_Byte(Units_Scroller, "Hide In Editor", Units_HideInEditor);
	Units_Unknown1 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Enabled = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_CheckBox_Enabled = new CheckBox_Short(Units_Scroller, "Enabled", Units_Enabled);
	Units_BuildingMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_VisibleInFog = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_VisibleInFog = new CheckBox_Byte(Units_Scroller, "Visible In Fog", Units_VisibleInFog);
	Units_TerrainRestriction = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_TerrainRestriction = new ComboBox_Short(Units_Scroller, Units_TerrainRestriction);
	Units_FlyMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_FlyMode = new CheckBox_Byte(Units_Scroller, "Fly Mode", Units_FlyMode);
	Units_ResourceCapacity = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ResourceDecay = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_InteractionMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_MinimapMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CommandAttribute = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Unknown3 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_Unknown3a = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_LanguageDllHelp = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_DLL_LanguageDllHelp = new wxStaticText(Units_Scroller, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE | wxALIGN_CENTRE_HORIZONTAL);
	Units_DLL_HotKey4 = new wxStaticText(Units_Scroller, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE | wxST_NO_AUTORESIZE | wxALIGN_CENTRE_HORIZONTAL);
	Units_Unknown4 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown5 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unselectable = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_Unselectable = new CheckBox_Byte(Units_Scroller, "Unselectable", Units_Unselectable);
	Units_Unknown6 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown7 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown8 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_SelectionMask = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_SelectionShapeType = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_SelectionShape = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_SelectionEffect = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_EditorSelectionColour = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_SelectionRadiusBox = new wxBoxSizer(wxHORIZONTAL);
	Units_HPBarHeight2 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_SelectionSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_SelectionSound = new ComboBox_Short(Units_Scroller, Units_SelectionSound);
	Units_DyingSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_DyingSound = new ComboBox_Short(Units_Scroller, Units_DyingSound);
	Units_AttackSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_AttackSound = new ComboBox_Short(Units_Scroller, Units_AttackSound);
	Units_Name = new TextCtrl_String(Units_Scroller, "0", NULL);
	Units_NameLength2 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Name2 = new TextCtrl_String(Units_Scroller, "0", NULL);
	Units_Unitline = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_MinTechLevel = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_ID2 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ID3 = new TextCtrl_Short(Units_Scroller, "0", NULL);

//	Type 20+

	Units_Speed = new TextCtrl_Float(Units_Scroller, "0", NULL);

//	Type 30+

	for(short loop = 0;loop < 2;loop++)
	{
		Units_WalkingGraphic[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_WalkingGraphic[loop] = new ComboBox_Short(Units_Scroller, Units_WalkingGraphic[loop]);
	}
	Units_RotationSpeed = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_Unknown11 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_TrackingUnit = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_TrackingUnit = new ComboBox_Short(Units_Scroller, Units_TrackingUnit);
	Units_TrackingUnitUsed = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_TrackingUnitUsed = new CheckBox_Byte(Units_Scroller, "Used", Units_TrackingUnitUsed);
	Units_TrackingUnitDensity = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_Unknown12 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 17;loop++)
	Units_Unknown16[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);

//	Type 40+

	Units_SheepConversion = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_CheckBox_SheepConversion = new CheckBox_Short_ZeroIsYes(Units_Scroller, "Yes", Units_SheepConversion);
	Units_SearchRadius = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_WorkRate = new TextCtrl_Float(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 2;loop++)
	{
		Units_DropSite[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_DropSite[loop] = new ComboBox_Short(Units_Scroller, Units_DropSite[loop]);
	}
	Units_VillagerMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_MoveSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_MoveSound = new ComboBox_Short(Units_Scroller, Units_MoveSound);
	Units_StopSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_StopSound = new ComboBox_Short(Units_Scroller, Units_StopSound);
	Units_Unknown19 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Exists = new TextCtrl_Byte(Units_Scroller, "0", NULL);

//	Type 60+

	for(short loop = 0;loop < 2;loop++)
	Units_Unknown20[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown21 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_MaxRange = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_BlastRadius = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_ReloadTime1 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_ProjectileUnitID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_ProjectileUnitID = new ComboBox_Short(Units_Scroller, Units_ProjectileUnitID);
	Units_AccuracyPercent = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown22 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Delay = new TextCtrl_Short(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 3;loop++)
	Units_GraphicDisplacement[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_Unknown23 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_MinRange = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_GarrisonRecoveryRate = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_AttackGraphic = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_AttackGraphic = new ComboBox_Short(Units_Scroller, Units_AttackGraphic);
	Units_DisplayedMeleeArmour = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_DisplayedAttack = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_DisplayedRange = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_ReloadTime2 = new TextCtrl_Float(Units_Scroller, "0", NULL);

//	Type 60 only

	Units_StretchMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CompensationMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_DropAnimationMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_PenetrationMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown24 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown25 = new TextCtrl_Float(Units_Scroller, "0", NULL);

//	Type 70+

	for(short loop = 0;loop < 2;loop++)
	{
		Units_GarrisonGraphic[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_GarrisonGraphic[loop] = new ComboBox_Short(Units_Scroller, Units_GarrisonGraphic[loop]);
	}
	for(short loop = 0;loop < 3;loop++)
	{
		Units_CostType[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_CostType[loop] = new ComboBox_Short(Units_Scroller, Units_CostType[loop]);
		Units_CostAmount[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_CostUsed[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_CheckBox_CostUsed[loop] = new CheckBox_Short(Units_Scroller, "Used", Units_CostUsed[loop]);
		Units_Unknown27[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_AttackMissileDuplicationUnknown[loop] = new TextCtrl_Float(Units_Scroller, "0", NULL);
	}
	Units_TrainTime = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_TrainLocationID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_TrainLocationID = new ComboBox_Short(Units_Scroller, Units_TrainLocationID);
	Units_ButtonID = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown26 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_Unknown28 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_MissileGraphicDelay = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_HeroMode = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_CheckBox_HeroMode = new CheckBox_Byte(Units_Scroller, "Hero Mode", Units_HeroMode);
	Units_AttackMissileDuplicationAmount1 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_AttackMissileDuplicationAmount2 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_AttackMissileDuplicationUnit = new TextCtrl_Long(Units_Scroller, "0", NULL);
	Units_ComboBox_AttackMissileDuplicationUnit = new ComboBox_Long(Units_Scroller, Units_AttackMissileDuplicationUnit);
	Units_AttackMissileDuplicationGraphic = new TextCtrl_Long(Units_Scroller, "0", NULL);
	Units_ComboBox_AttackMissileDuplicationGraphic = new ComboBox_Long(Units_Scroller, Units_AttackMissileDuplicationGraphic);
	Units_Unknown29 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_DisplayedPierceArmour = new TextCtrl_Short(Units_Scroller, "0", NULL);

//	Type 80

	Units_ConstructionGraphicID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_ConstructionGraphicID = new ComboBox_Short(Units_Scroller, Units_ConstructionGraphicID);
	Units_SnowGraphicID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_SnowGraphicID = new ComboBox_Short(Units_Scroller, Units_SnowGraphicID);
	Units_Unknown30 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Unknown31 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_StackUnitID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_StackUnitID = new ComboBox_Short(Units_Scroller, Units_StackUnitID);
	Units_TerrainID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_TerrainID = new ComboBox_Short(Units_Scroller, Units_TerrainID);
	Units_Unknown32 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ResearchID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_ResearchID = new ComboBox_Short(Units_Scroller, Units_ResearchID);
	Units_Unknown33 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 4;loop++)
	{
		Units_AnnexUnit[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		Units_ComboBox_AnnexUnit[loop] = new ComboBox_Short(Units_Scroller, Units_AnnexUnit[loop]);
	}
	for(short loop = 0;loop < 4;loop++)
	{
		for(short loop2 = 0;loop2 < 2;loop2++)
		Units_AnnexUnitMisplacement[loop][loop2] = new TextCtrl_Float(Units_Scroller, "0", NULL);
	}
	Units_HeadUnit = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_HeadUnit = new ComboBox_Short(Units_Scroller, Units_HeadUnit);
	Units_TransformUnit = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_TransformUnit = new ComboBox_Short(Units_Scroller, Units_TransformUnit);
	Units_Unknown34 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ConstructionSound = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_ComboBox_ConstructionSound = new ComboBox_Short(Units_Scroller, Units_ConstructionSound);
	Units_GarrisonType = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_ComboBox_GarrisonType = new ComboBox_Byte(Units_Scroller, Units_GarrisonType);
	Units_GarrisonHealRate = new TextCtrl_Float(Units_Scroller, "0", NULL);
	Units_Unknown35 = new TextCtrl_Long(Units_Scroller, "0", NULL);
	Units_Unknown36 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 6;loop++)
	Units_Unknown37[loop] = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	
	Units_Holder_DamageGraphics = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_DamageGraphics_Data = new wxBoxSizer(wxVERTICAL);
	Units_Holder_DamageGraphics_Space = new wxBoxSizer(wxVERTICAL);
	DamageGraphics_Holder_GraphicID = new wxBoxSizer(wxVERTICAL);
	DamageGraphics_Holder_DamagePercent = new wxBoxSizer(wxVERTICAL);
	DamageGraphics_Holder_Unknown1 = new wxBoxSizer(wxVERTICAL);
	DamageGraphics_Text_GraphicID = new wxStaticText(Units_Scroller, wxID_ANY, " Graphic ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	DamageGraphics_Text_DamagePercent = new wxStaticText(Units_Scroller, wxID_ANY, " Damage Percent ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	DamageGraphics_Text_Unknown1 = new wxStaticText(Units_Scroller, wxID_ANY, " Unknown 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	DamageGraphics_GraphicID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	DamageGraphics_ComboBox_GraphicID = new ComboBox_Short(Units_Scroller, DamageGraphics_GraphicID);
	DamageGraphics_DamagePercent = new TextCtrl_Short(Units_Scroller, "0", NULL);
	DamageGraphics_Unknown1 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	Units_DamageGraphics = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Unit Damage Graphics slot");
	Units_DamageGraphics_Search = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_DamageGraphics_Search_R = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_DamageGraphics_List = new wxListBox(Units_Scroller, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	Units_DamageGraphics_Buttons = new wxGridSizer(2, 0, 0);
	Units_DamageGraphics_Add = new wxButton(Units_Scroller, wxID_ANY, "Add", wxDefaultPosition, wxSize(-1, 20));
	Units_DamageGraphics_Delete = new wxButton(Units_Scroller, wxID_ANY, "Delete", wxDefaultPosition, wxSize(-1, 20));
	Units_DamageGraphics_Copy = new wxButton(Units_Scroller, wxID_ANY, "Copy", wxDefaultPosition, wxSize(-1, 20));
	Units_DamageGraphics_Paste = new wxButton(Units_Scroller, wxID_ANY, "Paste", wxDefaultPosition, wxSize(-1, 20));
	
	Units_Holder_Attacks = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attacks_Data = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Attacks_Space = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Attacks_Data1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attacks_Data2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Attacks_Data3 = new wxBoxSizer(wxHORIZONTAL);
	Attacks_Holder_Class = new wxBoxSizer(wxVERTICAL);
	Attacks_Holder_Class1 = new wxBoxSizer(wxHORIZONTAL);
	Attacks_Holder_Amount = new wxBoxSizer(wxVERTICAL);
	Attacks_Text_Class = new wxStaticText(Units_Scroller, wxID_ANY, " Class ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Attacks_Text_Amount = new wxStaticText(Units_Scroller, wxID_ANY, " Amount ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Attacks_Class = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Attacks_ComboBox_Class = new ComboBox_Short_AttackType(Units_Scroller, Attacks_Class);
	Attacks_Amount = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Attacks = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Unit Attacks slot");
	Units_Attacks_Search = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_Attacks_Search_R = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_Attacks_List = new wxListBox(Units_Scroller, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	Units_Attacks_Buttons = new wxGridSizer(2, 0, 0);
	Units_Attacks_Add = new wxButton(Units_Scroller, wxID_ANY, "Add", wxDefaultPosition, wxSize(-1, 20));
	Units_Attacks_Delete = new wxButton(Units_Scroller, wxID_ANY, "Delete", wxDefaultPosition, wxSize(-1, 20));
	Units_Attacks_Copy = new wxButton(Units_Scroller, wxID_ANY, "Copy", wxDefaultPosition, wxSize(-1, 20));
	Units_Attacks_Paste = new wxButton(Units_Scroller, wxID_ANY, "Paste", wxDefaultPosition, wxSize(-1, 20));	
	
	Units_Holder_Armors = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Armors_Data = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Armors_Data1 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Armors_Data2 = new wxBoxSizer(wxHORIZONTAL);
	Units_Holder_Armors_Data3 = new wxBoxSizer(wxVERTICAL);
	Units_Holder_Armors_Space = new wxBoxSizer(wxVERTICAL);
	Armors_Holder_Class = new wxBoxSizer(wxVERTICAL);
	Armors_Holder_Class1 = new wxBoxSizer(wxHORIZONTAL);
	Armors_Holder_Amount = new wxBoxSizer(wxVERTICAL);
	Armors_Text_Class = new wxStaticText(Units_Scroller, wxID_ANY, " Class ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Armors_Text_Amount = new wxStaticText(Units_Scroller, wxID_ANY, " Amount ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Armors_Class = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Armors_ComboBox_Class = new ComboBox_Short_AttackType(Units_Scroller, Armors_Class);
	Armors_Amount = new TextCtrl_Short(Units_Scroller, "0", NULL);
	Units_Armors = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Unit Armors slot");
	Units_Armors_Search = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_Armors_Search_R = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_Armors_List = new wxListBox(Units_Scroller, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	Units_Armors_Buttons = new wxGridSizer(2, 0, 0);
	Units_Armors_Add = new wxButton(Units_Scroller, wxID_ANY, "Add", wxDefaultPosition, wxSize(-1, 20));
	Units_Armors_Delete = new wxButton(Units_Scroller, wxID_ANY, "Delete", wxDefaultPosition, wxSize(-1, 20));
	Units_Armors_Copy = new wxButton(Units_Scroller, wxID_ANY, "Copy", wxDefaultPosition, wxSize(-1, 20));
	Units_Armors_Paste = new wxButton(Units_Scroller, wxID_ANY, "Paste", wxDefaultPosition, wxSize(-1, 20));

	Units_CommandHolder_Lists = new wxBoxSizer(wxVERTICAL);
	Units_UnitHeads = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Unit Header slot");
	Units_UnitHeads_Name = new wxStaticText(Units_Scroller, wxID_ANY, "Unit Header", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	Units_UnitHeads_List = new wxListBox(Units_Scroller, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	Units_UnitCommands = new wxStaticBoxSizer(wxVERTICAL, Units_Scroller, "Unit Commands slot");
	Units_UnitCommands_Search = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_UnitCommands_Search_R = new wxTextCtrl(Units_Scroller, wxID_ANY);
	Units_UnitCommands_List = new wxListBox(Units_Scroller, wxID_ANY, wxDefaultPosition, wxSize(-1, 190));
	Units_UnitCommands_Buttons = new wxGridSizer(2, 0, 0);
	Units_UnitCommands_Add = new wxButton(Units_Scroller, wxID_ANY, "Add", wxDefaultPosition, wxSize(-1, 20));
	Units_UnitCommands_Delete = new wxButton(Units_Scroller, wxID_ANY, "Delete", wxDefaultPosition, wxSize(-1, 20));
	Units_UnitCommands_Copy = new wxButton(Units_Scroller, wxID_ANY, "Copy", wxDefaultPosition, wxSize(-1, 20));
	Units_UnitCommands_Paste = new wxButton(Units_Scroller, wxID_ANY, "Paste", wxDefaultPosition, wxSize(-1, 20));
	Units_CommandHolder_Data = new wxBoxSizer(wxHORIZONTAL);
	Units_CommandHolder_Data1 = new wxBoxSizer(wxVERTICAL);
	Units_CommandHolder_Data2 = new wxBoxSizer(wxVERTICAL);

	UnitCommands_Holder_One = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_ID = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_1 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown1 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Type = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_SubType = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_Types = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_2 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Resources = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_ClassID = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_UnitID = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_Unknown2 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_ResourceIn = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_ResourceOut = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_Unknown3 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown4 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_ExecutionRadius = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown5 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown6 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown7 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown8 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown9 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown10 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown11 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown12 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown13 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Unknown14 = new wxBoxSizer(wxHORIZONTAL);
	UnitCommands_Holder_Graphics = new wxBoxSizer(wxVERTICAL);
	UnitCommands_Holder_GraphicsGrid = new wxGridSizer(2, 2, 2);
	
	UnitCommands_Text_One = new wxStaticText(Units_Scroller, wxID_ANY, "One ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_ID = new wxStaticText(Units_Scroller, wxID_ANY, "ID ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown1 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 1 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Type = new wxStaticText(Units_Scroller, wxID_ANY, " Type", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_ClassID = new wxStaticText(Units_Scroller, wxID_ANY, " Class", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_UnitID = new wxStaticText(Units_Scroller, wxID_ANY, " Unit", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown2 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 2 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_ResourceIn = new wxStaticText(Units_Scroller, wxID_ANY, " Resource In", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_SubType = new wxStaticText(Units_Scroller, wxID_ANY, " Sub Type", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_ResourceOut = new wxStaticText(Units_Scroller, wxID_ANY, " Resource Out", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown3 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 3 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown4 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 4 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_ExecutionRadius = new wxStaticText(Units_Scroller, wxID_ANY, "Execution Radius ", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown5 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 5 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown6 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 6 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown7 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 7 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown8 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 8 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown9 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 9 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown10 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 10 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown11 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 11 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown12 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 12 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown13 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 13 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Unknown14 = new wxStaticText(Units_Scroller, wxID_ANY, "Unknown 14 ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	UnitCommands_Text_Graphics = new wxStaticText(Units_Scroller, wxID_ANY, " Graphics", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);

	UnitCommands_One = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_Unknown1 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Type = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ComboBox_Types = new wxOwnerDrawnComboBox(Units_Scroller, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	UnitCommands_ClassID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ComboBox_ClassID = new ComboBox_Short(Units_Scroller, UnitCommands_ClassID);
	UnitCommands_UnitID = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ComboBox_UnitID = new ComboBox_Short(Units_Scroller, UnitCommands_UnitID);
	UnitCommands_Unknown2 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ResourceIn = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ComboBox_ResourceIn = new ComboBox_Short(Units_Scroller, UnitCommands_ResourceIn);
	UnitCommands_SubType = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ResourceOut = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_ComboBox_ResourceOut = new ComboBox_Short(Units_Scroller, UnitCommands_ResourceOut);
	UnitCommands_Unknown3 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_Unknown4 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	UnitCommands_ExecutionRadius = new TextCtrl_Float(Units_Scroller, "0", NULL);
	UnitCommands_Unknown5 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	UnitCommands_Unknown6 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Unknown7 = new TextCtrl_Float(Units_Scroller, "0", NULL);
	UnitCommands_Unknown8 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Unknown9 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Unknown10 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Unknown11 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	UnitCommands_Unknown12 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_Unknown13 = new TextCtrl_Short(Units_Scroller, "0", NULL);
	UnitCommands_Unknown14 = new TextCtrl_Byte(Units_Scroller, "0", NULL);
	for(short loop = 0;loop < 6;loop++)
	{
		UnitCommands_Graphics[loop] = new TextCtrl_Short(Units_Scroller, "0", NULL);
		UnitCommands_ComboBox_Graphics[loop] = new ComboBox_Short(Units_Scroller, UnitCommands_Graphics[loop]);
	}

//	UnitControls actual interface

	Units_ComboBox_Type->Append("No Type/Invalid Type");	// Selection 0
	Units_ComboBox_Type->Append("10 - Eye Candy");	// Selection 1
	Units_ComboBox_Type->Append("20 - Flag");
	Units_ComboBox_Type->Append("30 - Dead Unit/Fish Unit");
	Units_ComboBox_Type->Append("60 - Projectile");
	Units_ComboBox_Type->Append("70 - Living Unit");
	Units_ComboBox_Type->Append("80 - Building");	// Selection 6
	Units_ComboBox_Type->SetSelection(0);
	
	Units_ComboBox_Class->Append("No Class/Invalid Class");	// Selection 0
	Units_ComboBox_Class->Append("0 - Archer");	// Selection 1
	Units_ComboBox_Class->Append("1 - Artifact");
	Units_ComboBox_Class->Append("2 - Trade Boat");
	Units_ComboBox_Class->Append("3 - Building");
	Units_ComboBox_Class->Append("4 - Civilian");
	Units_ComboBox_Class->Append("5 - Sea Fish");
	Units_ComboBox_Class->Append("6 - Soldier");
	Units_ComboBox_Class->Append("7 - Berry Bush");
	Units_ComboBox_Class->Append("8 - Stone Mine");
	Units_ComboBox_Class->Append("9 - Prey Animal");
	Units_ComboBox_Class->Append("10 - Predator Animal");
	Units_ComboBox_Class->Append("11 - Other");
	Units_ComboBox_Class->Append("12 - Cavalry");
	Units_ComboBox_Class->Append("13 - Siege Weapon");
	Units_ComboBox_Class->Append("14 - Terrain");
	Units_ComboBox_Class->Append("15 - Trees");
	Units_ComboBox_Class->Append("16 - Unused");
	Units_ComboBox_Class->Append("17 - Unused");
	Units_ComboBox_Class->Append("18 - Priest");
	Units_ComboBox_Class->Append("19 - Trade Cart");
	Units_ComboBox_Class->Append("20 - Transport Boat");
	Units_ComboBox_Class->Append("21 - Fishing Boat");
	Units_ComboBox_Class->Append("22 - War Boat");
	Units_ComboBox_Class->Append("23 - Conquistador");
	Units_ComboBox_Class->Append("24 - Unused");
	Units_ComboBox_Class->Append("25 - Unused");
	Units_ComboBox_Class->Append("26 - Unused");
	Units_ComboBox_Class->Append("27 - Walls");
	Units_ComboBox_Class->Append("28 - Phalanx");
	Units_ComboBox_Class->Append("29 - Unused");
	Units_ComboBox_Class->Append("30 - Flags");
	Units_ComboBox_Class->Append("31 - Unused");
	Units_ComboBox_Class->Append("32 - Gold Mine");
	Units_ComboBox_Class->Append("33 - Shore Fish");
	Units_ComboBox_Class->Append("34 - Cliff");
	Units_ComboBox_Class->Append("35 - Petard");
	Units_ComboBox_Class->Append("36 - Cavalry Archer");
	Units_ComboBox_Class->Append("37 - Dolphin");
	Units_ComboBox_Class->Append("38 - Birds");
	Units_ComboBox_Class->Append("39 - Gates");
	Units_ComboBox_Class->Append("40 - Piles");
	Units_ComboBox_Class->Append("41 - Piles of Resource");
	Units_ComboBox_Class->Append("42 - Relic");
	Units_ComboBox_Class->Append("43 - Monk with Relic");
	Units_ComboBox_Class->Append("44 - Hand Cannoneer");
	Units_ComboBox_Class->Append("45 - Two Handed Swordsman");
	Units_ComboBox_Class->Append("46 - Pikeman");
	Units_ComboBox_Class->Append("47 - Scout Cavalry");
	Units_ComboBox_Class->Append("48 - Ore Mine");
	Units_ComboBox_Class->Append("49 - Farm");
	Units_ComboBox_Class->Append("50 - Spearman");
	Units_ComboBox_Class->Append("51 - Packed Siege Units");
	Units_ComboBox_Class->Append("52 - Tower");
	Units_ComboBox_Class->Append("53 - Boarding Boat");
	Units_ComboBox_Class->Append("54 - Unpacked Siege Units");
	Units_ComboBox_Class->Append("55 - Scorpion");
	Units_ComboBox_Class->Append("56 - Raider");
	Units_ComboBox_Class->Append("57 - Cavalry Raider");
	Units_ComboBox_Class->Append("58 - Sheep");
	Units_ComboBox_Class->Append("59 - King");
	Units_ComboBox_Class->Append("60 - Unused");
	Units_ComboBox_Class->Append("61 - Horse");
	Units_ComboBox_Class->SetSelection(0);
	
	UnitCommands_ComboBox_Types->Append("Unused Ability/Invalid Ability");	// Selection 0
	UnitCommands_ComboBox_Types->Append("Ability to Garrison");	// Selection 1
	UnitCommands_ComboBox_Types->Append("Ability to Mine Gold");
	UnitCommands_ComboBox_Types->Append("Ability to Mine Stone");
	UnitCommands_ComboBox_Types->Append("Ability to Fish, Forage, or Farm");
	UnitCommands_ComboBox_Types->Append("Ability to Rebuild");
	UnitCommands_ComboBox_Types->Append("Type 6, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Attack");
	UnitCommands_ComboBox_Types->Append("Ability to Fly");
	UnitCommands_ComboBox_Types->Append("Type 11, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Unload (Boat-Like)");
	UnitCommands_ComboBox_Types->Append("Ability to Auto-Attack");
	UnitCommands_ComboBox_Types->Append("Type 21, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Build");
	UnitCommands_ComboBox_Types->Append("Ability to Convert");
	UnitCommands_ComboBox_Types->Append("Ability to Heal");
	UnitCommands_ComboBox_Types->Append("Ability to Repair");
	UnitCommands_ComboBox_Types->Append("Type 107, Sub -1");
	UnitCommands_ComboBox_Types->Append("Type 109, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Chop Wood");
	UnitCommands_ComboBox_Types->Append("Ability to Hunt Prey Animals");
	UnitCommands_ComboBox_Types->Append("Ability to Hunt Predator Animals");
	UnitCommands_ComboBox_Types->Append("Ability to Trade");
	UnitCommands_ComboBox_Types->Append("Ability to Generate Wonder Victory*");
	UnitCommands_ComboBox_Types->Append("Type 121, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Mine Porex (Ore)");
	UnitCommands_ComboBox_Types->Append("Ability to Unpack & Attack");
	UnitCommands_ComboBox_Types->Append("Type 131, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Pickup Unit");
	UnitCommands_ComboBox_Types->Append("Type 135, Sub -1");
	UnitCommands_ComboBox_Types->Append("Ability to Deposit Unit");	// Selection 30
	UnitCommands_ComboBox_Types->SetSelection(0);
	
	UnitCommands_ComboBox_ClassID->Append("No Class/Invalid Class");	// Selection 0
	UnitCommands_ComboBox_ClassID->Append("0 - Archer");	// Selection 1
	UnitCommands_ComboBox_ClassID->Append("1 - Artifact");
	UnitCommands_ComboBox_ClassID->Append("2 - Trade Boat");
	UnitCommands_ComboBox_ClassID->Append("3 - Building");
	UnitCommands_ComboBox_ClassID->Append("4 - Civilian");
	UnitCommands_ComboBox_ClassID->Append("5 - Sea Fish");
	UnitCommands_ComboBox_ClassID->Append("6 - Soldier");
	UnitCommands_ComboBox_ClassID->Append("7 - Berry Bush");
	UnitCommands_ComboBox_ClassID->Append("8 - Stone Mine");
	UnitCommands_ComboBox_ClassID->Append("9 - Prey Animal");
	UnitCommands_ComboBox_ClassID->Append("10 - Predator Animal");
	UnitCommands_ComboBox_ClassID->Append("11 - Other");
	UnitCommands_ComboBox_ClassID->Append("12 - Cavalry");
	UnitCommands_ComboBox_ClassID->Append("13 - Siege Weapon");
	UnitCommands_ComboBox_ClassID->Append("14 - Terrain");
	UnitCommands_ComboBox_ClassID->Append("15 - Trees");
	UnitCommands_ComboBox_ClassID->Append("16 - Unused");
	UnitCommands_ComboBox_ClassID->Append("17 - Unused");
	UnitCommands_ComboBox_ClassID->Append("18 - Priest");
	UnitCommands_ComboBox_ClassID->Append("19 - Trade Cart");
	UnitCommands_ComboBox_ClassID->Append("20 - Transport Boat");
	UnitCommands_ComboBox_ClassID->Append("21 - Fishing Boat");
	UnitCommands_ComboBox_ClassID->Append("22 - War Boat");
	UnitCommands_ComboBox_ClassID->Append("23 - Conquistador");
	UnitCommands_ComboBox_ClassID->Append("24 - Unused");
	UnitCommands_ComboBox_ClassID->Append("25 - Unused");
	UnitCommands_ComboBox_ClassID->Append("26 - Unused");
	UnitCommands_ComboBox_ClassID->Append("27 - Walls");
	UnitCommands_ComboBox_ClassID->Append("28 - Phalanx");
	UnitCommands_ComboBox_ClassID->Append("29 - Unused");
	UnitCommands_ComboBox_ClassID->Append("30 - Flags");
	UnitCommands_ComboBox_ClassID->Append("31 - Unused");
	UnitCommands_ComboBox_ClassID->Append("32 - Gold Mine");
	UnitCommands_ComboBox_ClassID->Append("33 - Shore Fish");
	UnitCommands_ComboBox_ClassID->Append("34 - Cliff");
	UnitCommands_ComboBox_ClassID->Append("35 - Petard");
	UnitCommands_ComboBox_ClassID->Append("36 - Cavalry Archer");
	UnitCommands_ComboBox_ClassID->Append("37 - Dolphin");
	UnitCommands_ComboBox_ClassID->Append("38 - Birds");
	UnitCommands_ComboBox_ClassID->Append("39 - Gates");
	UnitCommands_ComboBox_ClassID->Append("40 - Piles");
	UnitCommands_ComboBox_ClassID->Append("41 - Piles of Resource");
	UnitCommands_ComboBox_ClassID->Append("42 - Relic");
	UnitCommands_ComboBox_ClassID->Append("43 - Monk with Relic");
	UnitCommands_ComboBox_ClassID->Append("44 - Hand Cannoneer");
	UnitCommands_ComboBox_ClassID->Append("45 - Two Handed Swordsman");
	UnitCommands_ComboBox_ClassID->Append("46 - Pikeman");
	UnitCommands_ComboBox_ClassID->Append("47 - Scout Cavalry");
	UnitCommands_ComboBox_ClassID->Append("48 - Ore Mine");
	UnitCommands_ComboBox_ClassID->Append("49 - Farm");
	UnitCommands_ComboBox_ClassID->Append("50 - Spearman");
	UnitCommands_ComboBox_ClassID->Append("51 - Packed Siege Units");
	UnitCommands_ComboBox_ClassID->Append("52 - Tower");
	UnitCommands_ComboBox_ClassID->Append("53 - Boarding Boat");
	UnitCommands_ComboBox_ClassID->Append("54 - Unpacked Siege Units");
	UnitCommands_ComboBox_ClassID->Append("55 - Scorpion");
	UnitCommands_ComboBox_ClassID->Append("56 - Raider");
	UnitCommands_ComboBox_ClassID->Append("57 - Cavalry Raider");
	UnitCommands_ComboBox_ClassID->Append("58 - Sheep");
	UnitCommands_ComboBox_ClassID->Append("59 - King");
	UnitCommands_ComboBox_ClassID->Append("60 - Unused");
	UnitCommands_ComboBox_ClassID->Append("61 - Horse");
	UnitCommands_ComboBox_ClassID->SetSelection(0);
	
	Attacks_ComboBox_Class->Append("Unused Class/No Class");	// Selection 0
	Attacks_ComboBox_Class->Append("1 - Infantry");	// Selection 1
	Attacks_ComboBox_Class->Append("2 - Turtle Ships");
	Attacks_ComboBox_Class->Append("3 - Base Pierce");
	Attacks_ComboBox_Class->Append("4 - Base Melee");
	Attacks_ComboBox_Class->Append("5 - War Elephants");
	Attacks_ComboBox_Class->Append("6 - None");
	Attacks_ComboBox_Class->Append("7 - None");
	Attacks_ComboBox_Class->Append("8 - Cavalry");
	Attacks_ComboBox_Class->Append("9 - None");
	Attacks_ComboBox_Class->Append("10 - None");
	Attacks_ComboBox_Class->Append("11 - All Buildings (except Port)");
	Attacks_ComboBox_Class->Append("12 - None");
	Attacks_ComboBox_Class->Append("13 - Stone Defense");
	Attacks_ComboBox_Class->Append("14 - None");
	Attacks_ComboBox_Class->Append("15 - Archers");
	Attacks_ComboBox_Class->Append("16 - Ships & Camels & Saboteurs");
	Attacks_ComboBox_Class->Append("17 - Rams");
	Attacks_ComboBox_Class->Append("18 - Trees");
	Attacks_ComboBox_Class->Append("19 - Unique Units (except Turtle Ship)");
	Attacks_ComboBox_Class->Append("20 - Siege Weapons");
	Attacks_ComboBox_Class->Append("21 - Standard Buildings");
	Attacks_ComboBox_Class->Append("22 - Walls & Gates");
	Attacks_ComboBox_Class->Append("23 - None");
	Attacks_ComboBox_Class->Append("24 - Boars");
	Attacks_ComboBox_Class->Append("25 - Monks");
	Attacks_ComboBox_Class->Append("26 - Castle");
	Attacks_ComboBox_Class->Append("27 - Spearmen");
	Attacks_ComboBox_Class->Append("28 - Cavalry Archers");
	Attacks_ComboBox_Class->Append("29 - Eagle Warriors");
	Attacks_ComboBox_Class->Append("30 - None");	// Selection 30
	Attacks_ComboBox_Class->Append("31 - Extra Class 1");
	Attacks_ComboBox_Class->Append("32 - Extra Class 2");
	Attacks_ComboBox_Class->Append("33 - Extra Class 3");
	Attacks_ComboBox_Class->Append("34 - Extra Class 4");
	Attacks_ComboBox_Class->Append("35 - Extra Class 5");
	Attacks_ComboBox_Class->SetSelection(0);
	
	Armors_ComboBox_Class->Append("Unused Class/No Class");	// Selection 0
	Armors_ComboBox_Class->Append("1 - Infantry");	// Selection 1
	Armors_ComboBox_Class->Append("2 - Turtle Ships");
	Armors_ComboBox_Class->Append("3 - Base Pierce");
	Armors_ComboBox_Class->Append("4 - Base Melee");
	Armors_ComboBox_Class->Append("5 - War Elephants");
	Armors_ComboBox_Class->Append("6 - None");
	Armors_ComboBox_Class->Append("7 - None");
	Armors_ComboBox_Class->Append("8 - Cavalry");
	Armors_ComboBox_Class->Append("9 - None");
	Armors_ComboBox_Class->Append("10 - None");
	Armors_ComboBox_Class->Append("11 - All Buildings (except Port)");
	Armors_ComboBox_Class->Append("12 - None");
	Armors_ComboBox_Class->Append("13 - Stone Defense");
	Armors_ComboBox_Class->Append("14 - None");
	Armors_ComboBox_Class->Append("15 - Archers");
	Armors_ComboBox_Class->Append("16 - Ships & Camels & Saboteurs");
	Armors_ComboBox_Class->Append("17 - Rams");
	Armors_ComboBox_Class->Append("18 - Trees");
	Armors_ComboBox_Class->Append("19 - Unique Units (except Turtle Ship)");
	Armors_ComboBox_Class->Append("20 - Siege Weapons");
	Armors_ComboBox_Class->Append("21 - Standard Buildings");
	Armors_ComboBox_Class->Append("22 - Walls & Gates");
	Armors_ComboBox_Class->Append("23 - None");
	Armors_ComboBox_Class->Append("24 - Boars");
	Armors_ComboBox_Class->Append("25 - Monks");
	Armors_ComboBox_Class->Append("26 - Castle");
	Armors_ComboBox_Class->Append("27 - Spearmen");
	Armors_ComboBox_Class->Append("28 - Cavalry Archers");
	Armors_ComboBox_Class->Append("29 - Eagle Warriors");
	Armors_ComboBox_Class->Append("30 - None");	// Selection 30
	Armors_ComboBox_Class->Append("31 - Extra Class 1");
	Armors_ComboBox_Class->Append("32 - Extra Class 2");
	Armors_ComboBox_Class->Append("33 - Extra Class 3");
	Armors_ComboBox_Class->Append("34 - Extra Class 4");
	Armors_ComboBox_Class->Append("35 - Extra Class 5");
	Armors_ComboBox_Class->SetSelection(0);
	
	Units_ComboBox_GarrisonType->Append("No Type/Invalid Type");	// Selection 0
	Units_ComboBox_GarrisonType->Append("0 - None");	// Selection 1
	Units_ComboBox_GarrisonType->Append("1 - Villager");
	Units_ComboBox_GarrisonType->Append("2 - Infantry");
	Units_ComboBox_GarrisonType->Append("3 - Villager + Infantry");
	Units_ComboBox_GarrisonType->Append("4 - Cavalry");
	Units_ComboBox_GarrisonType->Append("5 - Cavalry + Villager");
	Units_ComboBox_GarrisonType->Append("6 - Cavalry + Infantry");
	Units_ComboBox_GarrisonType->Append("7 - Cavalry + Infantry + Villager");
	Units_ComboBox_GarrisonType->Append("8 - Monk");
	Units_ComboBox_GarrisonType->Append("9 - Monk + Villager");
	Units_ComboBox_GarrisonType->Append("10 - Monk + Infantry");
	Units_ComboBox_GarrisonType->Append("11 - Monk + Infantry + Villager");
	Units_ComboBox_GarrisonType->Append("12 - Monk + Cavalry");
	Units_ComboBox_GarrisonType->Append("13 - Monk + Villager + Calavry");
	Units_ComboBox_GarrisonType->Append("14 - Monk + Calavry + Infantry");
	Units_ComboBox_GarrisonType->Append("15 - Monk + Villager + Infantry + Calavry");
	Units_ComboBox_GarrisonType->SetSelection(0);
	
	for(short loop = 0;loop < 2;loop++)
	{
		Units_Units_SearchFilters[loop]->Append("Lang DLL Name");	// 0
		Units_Units_SearchFilters[loop]->Append("Internal Name");
		Units_Units_SearchFilters[loop]->Append("Type");
		Units_Units_SearchFilters[loop]->Append("Class");
		Units_Units_SearchFilters[loop]->Append("Pointer");
	/*	Units_Units_SearchFilters[loop]->Append("Garrison Type");
		Units_Units_SearchFilters[loop]->Append("Projectile Unit");
		Units_Units_SearchFilters[loop]->Append("Enabled");
		Units_Units_SearchFilters[loop]->Append("Hidden In Editor");
		Units_Units_SearchFilters[loop]->Append("Visible In Fog");
		Units_Units_SearchFilters[loop]->Append("Death Mode");
		Units_Units_SearchFilters[loop]->Append("Hero Mode");
		Units_Units_SearchFilters[loop]->Append("Air Mode");
		Units_Units_SearchFilters[loop]->Append("Fly Mode");
		Units_Units_SearchFilters[loop]->Append("Building Mode");
		Units_Units_SearchFilters[loop]->Append("Placement Mode");
		Units_Units_SearchFilters[loop]->Append("Terrain Restriction");
		Units_Units_SearchFilters[loop]->Append("Interaction Mode");
		Units_Units_SearchFilters[loop]->Append("Minimap Mode");
		Units_Units_SearchFilters[loop]->Append("Sheep Conversion");
		Units_Units_SearchFilters[loop]->Append("Villager Mode");
		Units_Units_SearchFilters[loop]->Append("Unseletable");
		Units_Units_SearchFilters[loop]->Append("Selection Mask");
		Units_Units_SearchFilters[loop]->Append("Selection Shape Type");
		Units_Units_SearchFilters[loop]->Append("Selection Shape");
		Units_Units_SearchFilters[loop]->Append("Selection Effect");
		Units_Units_SearchFilters[loop]->Append("Editor Selection Color");
		Units_Units_SearchFilters[loop]->Append("Unitline");
		Units_Units_SearchFilters[loop]->Append("Tracking Unit Used");
		Units_Units_SearchFilters[loop]->Append("Train Location");
		Units_Units_SearchFilters[loop]->Append("Command Attribute");
		Units_Units_SearchFilters[loop]->Append("Stack Unit");
		Units_Units_SearchFilters[loop]->Append("Terrain");
		Units_Units_SearchFilters[loop]->Append("Research");
	*/	Units_Units_SearchFilters[loop]->SetSelection(0);
	}
	
	Units_Units_Buttons->Add(Units_Add, 1, wxEXPAND);
	Units_Units_Buttons->Add(Units_Delete, 1, wxEXPAND);
	Units_Units_Buttons->Add(Units_Copy, 1, wxEXPAND);
	Units_Units_Buttons->Add(Units_Paste, 1, wxEXPAND);
	
	Units_Units_Searches[0]->Add(Units_Units_Search, 1, wxEXPAND);
	Units_Units_Searches[0]->Add(Units_Units_UseAnd[0], 0, wxEXPAND);
	Units_Units_Searches[1]->Add(Units_Units_Search_R, 1, wxEXPAND);
	Units_Units_Searches[1]->Add(Units_Units_UseAnd[1], 0, wxEXPAND);
	
	Units_Units->Add(Units_Civs_List, 0, wxEXPAND);
	Units_Units->Add(-1, 2);
	for(short loop = 0;loop < 2;loop++)
	Units_Units->Add(Units_Units_Searches[loop], 0, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Units->Add(Units_Units_SearchFilters[loop], 0, wxEXPAND);
	Units_Units->Add(-1, 2);
	Units_Units->Add(Units_Units_List, 1, wxEXPAND);
	Units_Units->Add(-1, 2);
	Units_Units->Add(Units_Units_Buttons, 0, wxEXPAND);
	
	Units_ListArea->Add(-1, 10);
	Units_ListArea->Add(Units_Units, 1, wxEXPAND);
	Units_ListArea->Add(-1, 10);
	
	Units_Holder_Type->Add(Units_Text_Type, 0, wxEXPAND);
	Units_Holder_Type->Add(Units_Type, 1, wxEXPAND);
	Units_Holder_Type->Add(Units_ComboBox_Type, 2, wxEXPAND);

//	Type 10+
	
	Units_Holder_NameLength->Add(Units_Text_NameLength, 0, wxEXPAND);
	Units_Holder_ID1->Add(Units_Text_ID1, 0, wxEXPAND);
	Units_Holder_LanguageDllName->Add(Units_Text_LanguageDllName, 0, wxEXPAND);
	Units_Holder_LanguageDllCreation->Add(Units_Text_LanguageDllCreation, 0, wxEXPAND);
	Units_Holder_Class->Add(Units_Text_Class, 0, wxEXPAND);
	Units_Holder_HitPoints->Add(Units_Text_HitPoints, 0, wxEXPAND);
	Units_Holder_LineOfSight->Add(Units_Text_LineOfSight, 0, wxEXPAND);
	Units_Holder_GarrisonCapacity->Add(Units_Text_GarrisonCapacity, 0, wxEXPAND);
	Units_Holder_SizeRadius->Add(Units_Text_SizeRadius, 0, wxEXPAND);
	Units_Holder_HPBarHeight1->Add(Units_Text_HPBarHeight1, 0, wxEXPAND);
	Units_Holder_DeadUnitID->Add(Units_Text_DeadUnitID, 0, wxEXPAND);
	Units_Holder_PlacementMode->Add(Units_Text_PlacementMode, 0, wxEXPAND);
	Units_Holder_IconID->Add(Units_Text_IconID, 0, wxEXPAND);
	Units_Holder_Unknown1->Add(Units_Text_Unknown1, 0, wxEXPAND);
	Units_Holder_PlacementBypassTerrain->Add(Units_Text_PlacementBypassTerrain, 0, wxEXPAND);
	Units_Holder_PlacementTerrain->Add(Units_Text_PlacementTerrain, 0, wxEXPAND);
	Units_Holder_EditorRadius->Add(Units_Text_EditorRadius, 0, wxEXPAND);
	Units_Holder_BuildingMode->Add(Units_Text_BuildingMode, 0, wxEXPAND);
	Units_Holder_TerrainRestriction->Add(Units_Text_TerrainRestriction, 0, wxEXPAND);
	Units_Holder_ResourceCapacity->Add(Units_Text_ResourceCapacity, 0, wxEXPAND);
	Units_Holder_ResourceDecay->Add(Units_Text_ResourceDecay, 0, wxEXPAND);
	Units_Holder_Unknown2->Add(Units_Text_Unknown2, 0, wxEXPAND);
	Units_Holder_Unknown2->Add(-1, 2);
	Units_Holder_InteractionMode->Add(Units_Text_InteractionMode, 0, wxEXPAND);
	Units_Holder_MinimapMode->Add(Units_Text_MinimapMode, 0, wxEXPAND);
	Units_Holder_CommandAttribute->Add(Units_Text_CommandAttribute, 0, wxEXPAND);
	Units_Holder_Unknown3->Add(Units_Text_Unknown3, 0, wxEXPAND);
	Units_Holder_Unknown3a->Add(Units_Text_Unknown3a, 0, wxEXPAND);
	Units_Holder_LanguageDllHelp->Add(Units_Text_LanguageDllHelp, 0, wxEXPAND);
	Units_Holder_HotKey->Add(Units_Text_HotKey, 0, wxEXPAND);
	Units_Holder_Unknown4->Add(Units_Text_Unknown4, 0, wxEXPAND);
	Units_Holder_Unknown5->Add(Units_Text_Unknown5, 0, wxEXPAND);
	Units_Holder_Unknown6->Add(Units_Text_Unknown6, 0, wxEXPAND);
	Units_Holder_Unknown7->Add(Units_Text_Unknown7, 0, wxEXPAND);
	Units_Holder_Unknown8->Add(Units_Text_Unknown8, 0, wxEXPAND);
	Units_Holder_SelectionMask->Add(Units_Text_SelectionMask, 0, wxEXPAND);
	Units_Holder_SelectionShapeType->Add(Units_Text_SelectionShapeType, 0, wxEXPAND);
	Units_Holder_SelectionShape->Add(Units_Text_SelectionShape, 0, wxEXPAND);
	Units_Holder_Unknown9->Add(Units_Text_Unknown9, 0, wxEXPAND);
	Units_Holder_Unknown9->Add(-1, 2);
	Units_Holder_SelectionEffect->Add(Units_Text_SelectionEffect, 0, wxEXPAND);
	Units_Holder_EditorSelectionColour->Add(Units_Text_EditorSelectionColour, 0, wxEXPAND);
	Units_Holder_SelectionRadius->Add(Units_Text_SelectionRadius, 0, wxEXPAND);
	Units_Holder_HPBarHeight2->Add(Units_Text_HPBarHeight2, 0, wxEXPAND);
	Units_Holder_SelectionSound->Add(Units_Text_SelectionSound, 0, wxEXPAND);
	Units_Holder_DyingSound->Add(Units_Text_DyingSound, 0, wxEXPAND);
	Units_Holder_AttackSound->Add(Units_Text_AttackSound, 0, wxEXPAND);
	Units_Holder_Name->Add(Units_Text_Name, 0, wxEXPAND);
	Units_Holder_NameLength2->Add(Units_Text_NameLength2, 0, wxEXPAND);
	Units_Holder_Name2->Add(Units_Text_Name2, 0, wxEXPAND);
	Units_Holder_Unitline->Add(Units_Text_Unitline, 0, wxEXPAND);
	Units_Holder_MinTechLevel->Add(Units_Text_MinTechLevel, 0, wxEXPAND);
	Units_Holder_ID2->Add(Units_Text_ID2, 0, wxEXPAND);
	Units_Holder_ID3->Add(Units_Text_ID3, 0, wxEXPAND);

//	Type 20+

	Units_Holder_Speed->Add(Units_Text_Speed, 0, wxEXPAND);

//	Type 30+

	Units_Holder_RotationSpeed->Add(Units_Text_RotationSpeed, 0, wxEXPAND);
	Units_Holder_Unknown11->Add(Units_Text_Unknown11, 0, wxEXPAND);
	Units_Holder_TrackingUnit->Add(Units_Text_TrackingUnit, 0, wxEXPAND);
	Units_Holder_TrackingUnitUsed->Add(Units_Text_TrackingUnitUsed, 0, wxEXPAND);
	Units_Holder_TrackingUnitDensity->Add(Units_Text_TrackingUnitDensity, 0, wxEXPAND);
	Units_Holder_Unknown12->Add(Units_Text_Unknown12, 0, wxEXPAND);
	Units_Holder_Unknown16->Add(Units_Text_Unknown16, 0, wxEXPAND);

//	Type 40+

	Units_Holder_SheepConversion->Add(Units_Text_SheepConversion, 0, wxEXPAND);
	Units_Holder_SearchRadius->Add(Units_Text_SearchRadius, 0, wxEXPAND);
	Units_Holder_WorkRate->Add(Units_Text_WorkRate, 0, wxEXPAND);
	Units_Holder_DropSite->Add(Units_Text_DropSite, 0, wxEXPAND);
	Units_Holder_VillagerMode->Add(Units_Text_VillagerMode, 0, wxEXPAND);
	Units_Holder_MoveSound->Add(Units_Text_MoveSound, 0, wxEXPAND);
	Units_Holder_StopSound->Add(Units_Text_StopSound, 0, wxEXPAND);
	Units_Holder_Unknown19->Add(Units_Text_Unknown19, 0, wxEXPAND);
	Units_Holder_Exists->Add(Units_Text_Exists, 0, wxEXPAND);

//	Type 60+

	Units_Holder_Unknown20->Add(Units_Text_Unknown20, 0, wxEXPAND);
	Units_Holder_Unknown21->Add(Units_Text_Unknown21, 0, wxEXPAND);
	Units_Holder_MaxRange->Add(Units_Text_MaxRange, 0, wxEXPAND);
	Units_Holder_BlastRadius->Add(Units_Text_BlastRadius, 0, wxEXPAND);
	Units_Holder_ReloadTime1->Add(Units_Text_ReloadTime1, 0, wxEXPAND);
	Units_Holder_AccuracyPercent->Add(Units_Text_AccuracyPercent, 0, wxEXPAND);
	Units_Holder_Unknown22->Add(Units_Text_Unknown22, 0, wxEXPAND);
	Units_Holder_Delay->Add(Units_Text_Delay, 0, wxEXPAND);
	Units_Holder_GraphicDisplacement->Add(Units_Text_GraphicDisplacement, 0, wxEXPAND);
	Units_Holder_Unknown23->Add(Units_Text_Unknown23, 0, wxEXPAND);
	Units_Holder_MinRange->Add(Units_Text_MinRange, 0, wxEXPAND);
	Units_Holder_GarrisonRecoveryRate->Add(Units_Text_GarrisonRecoveryRate, 0, wxEXPAND);
	Units_Holder_AttackGraphic->Add(Units_Text_AttackGraphic, 0, wxEXPAND);
	Units_Holder_DisplayedMeleeArmour->Add(Units_Text_DisplayedMeleeArmour, 0, wxEXPAND);
	Units_Holder_DisplayedAttack->Add(Units_Text_DisplayedAttack, 0, wxEXPAND);
	Units_Holder_DisplayedRange->Add(Units_Text_DisplayedRange, 0, wxEXPAND);
	Units_Holder_ReloadTime2->Add(Units_Text_ReloadTime2, 0, wxEXPAND);

//	Type 60 only

	Units_Holder_StretchMode->Add(Units_Text_StretchMode, 0, wxEXPAND);
	Units_Holder_CompensationMode->Add(Units_Text_CompensationMode, 0, wxEXPAND);
	Units_Holder_DropAnimationMode->Add(Units_Text_DropAnimationMode, 0, wxEXPAND);
	Units_Holder_PenetrationMode->Add(Units_Text_PenetrationMode, 0, wxEXPAND);
	Units_Holder_Unknown24->Add(Units_Text_Unknown24, 0, wxEXPAND);
	Units_Holder_Unknown25->Add(Units_Text_Unknown25, 0, wxEXPAND);

//	Type 70+

	Units_Holder_CostType->Add(Units_Text_CostType, 2, wxEXPAND);
	Units_Holder_CostAmount->Add(Units_Text_CostAmount, 2, wxEXPAND);
	Units_Holder_CostUsed->Add(Units_Text_CostUsed, 2, wxEXPAND);
	Units_Holder_TrainTime->Add(Units_Text_TrainTime, 0, wxEXPAND);
	Units_Holder_TrainLocationID->Add(Units_Text_TrainLocationID, 0, wxEXPAND);
	Units_Holder_ButtonID->Add(Units_Text_ButtonID, 0, wxEXPAND);
	Units_Holder_Unknown26->Add(Units_Text_Unknown26, 0, wxEXPAND);
	Units_Holder_Unknown27->Add(Units_Text_Unknown27, 0, wxEXPAND);
	Units_Holder_Unknown28->Add(Units_Text_Unknown28, 0, wxEXPAND);
	Units_Holder_MissileGraphicDelay->Add(Units_Text_MissileGraphicDelay, 0, wxEXPAND);
	Units_Holder_AttackMissileDuplicationAmount1->Add(Units_Text_AttackMissileDuplicationAmount1, 0, wxEXPAND);
	Units_Holder_AttackMissileDuplicationAmount2->Add(Units_Text_AttackMissileDuplicationAmount2, 0, wxEXPAND);
	Units_Holder_AttackMissileDuplicationUnknown->Add(Units_Text_AttackMissileDuplicationUnknown, 0, wxEXPAND);
	Units_Holder_Unknown29->Add(Units_Text_Unknown29, 0, wxEXPAND);
	Units_Holder_DisplayedPierceArmour->Add(Units_Text_DisplayedPierceArmour, 0, wxEXPAND);

//	Type 80

	Units_Holder_ConstructionGraphicID->Add(Units_Text_ConstructionGraphicID, 0, wxEXPAND);
	Units_Holder_SnowGraphicID->Add(Units_Text_SnowGraphicID, 0, wxEXPAND);
	Units_Holder_Unknown30->Add(Units_Text_Unknown30, 0, wxEXPAND);
	Units_Holder_Unknown31->Add(Units_Text_Unknown31, 0, wxEXPAND);
	Units_Holder_StackUnitID->Add(Units_Text_StackUnitID, 0, wxEXPAND);
	Units_Holder_TerrainID->Add(Units_Text_TerrainID, 0, wxEXPAND);
	Units_Holder_Unknown32->Add(Units_Text_Unknown32, 0, wxEXPAND);
	Units_Holder_ResearchID->Add(Units_Text_ResearchID, 0, wxEXPAND);
	Units_Holder_Unknown33->Add(Units_Text_Unknown33, 0, wxEXPAND);
	Units_Holder_AnnexUnit1->Add(Units_Text_AnnexUnit, 0, wxEXPAND);
	Units_Holder_AnnexUnitMisplacement1->Add(Units_Text_AnnexUnitMisplacement, 0, wxEXPAND);
	Units_Holder_HeadUnit->Add(Units_Text_HeadUnit, 0, wxEXPAND);
	Units_Holder_TransformUnit->Add(Units_Text_TransformUnit, 0, wxEXPAND);
	Units_Holder_Unknown34->Add(Units_Text_Unknown34, 0, wxEXPAND);
	Units_Holder_ConstructionSound->Add(Units_Text_ConstructionSound, 0, wxEXPAND);
	Units_Holder_GarrisonHealRate->Add(Units_Text_GarrisonHealRate, 0, wxEXPAND);
	Units_Holder_Unknown35->Add(Units_Text_Unknown35, 0, wxEXPAND);
	Units_Holder_Unknown36->Add(Units_Text_Unknown36, 0, wxEXPAND);
	Units_Holder_Unknown37->Add(Units_Text_Unknown37, 0, wxEXPAND);
	
//	Type 10+
	
	Units_Holder_NameLength->Add(2, 2);
	Units_Holder_NameLength->Add(Units_NameLength, 1, wxEXPAND);
	Units_Holder_ID1->Add(Units_ID1, 1, wxEXPAND);
	Units_Holder_LanguageDllName->Add(2, 2);
	Units_Holder_LanguageDllName->Add(Units_LanguageDllName, 1, wxEXPAND);
	Units_Holder_LanguageDllName->Add(Units_DLL_LanguageDllName, 1, wxEXPAND);
	Units_Holder_LanguageDllCreation->Add(2, 2);
	Units_Holder_LanguageDllCreation->Add(Units_LanguageDllCreation, 1, wxEXPAND);
	Units_Holder_LanguageDllCreation->Add(Units_DLL_LanguageDllCreation, 1, wxEXPAND);
	Units_Holder_Class->Add(Units_Class, 1, wxEXPAND);
	Units_Holder_Class->Add(Units_ComboBox_Class, 2, wxEXPAND);
	Units_Holder_DeathMode->Add(Units_DeathMode, 1, wxEXPAND);
	Units_Holder_DeathMode->Add(2, 2);
	Units_Holder_DeathMode->Add(Units_CheckBox_DeathMode, 2, wxEXPAND);
	Units_Holder_HitPoints->Add(2, 2);
	Units_Holder_HitPoints->Add(Units_HitPoints, 1, wxEXPAND);
	Units_Holder_LineOfSight->Add(2, 2);
	Units_Holder_LineOfSight->Add(Units_LineOfSight, 1, wxEXPAND);
	Units_Holder_GarrisonCapacity->Add(2, 2);
	Units_Holder_GarrisonCapacity->Add(Units_GarrisonCapacity, 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_SizeRadius->Add(Units_SizeRadius[loop], 1, wxEXPAND);
	Units_Holder_HPBarHeight1->Add(Units_HPBarHeight1, 1, wxEXPAND);
	Units_Holder_DeadUnitID->Add(Units_DeadUnitID, 1, wxEXPAND);
	Units_Holder_DeadUnitID->Add(Units_ComboBox_DeadUnitID, 2, wxEXPAND);
	Units_Holder_PlacementMode->Add(Units_PlacementMode, 1, wxEXPAND);
	Units_Holder_AirMode->Add(Units_AirMode, 1, wxEXPAND);
	Units_Holder_AirMode->Add(2, 2);
	Units_Holder_AirMode->Add(Units_CheckBox_AirMode, 2, wxEXPAND);
	Units_Holder_IconID->Add(2, 2);
	Units_Holder_IconID->Add(Units_IconID, 1, wxEXPAND);
	Units_Holder_IconID->AddStretchSpacer(1);
	Units_Holder_HideInEditor->Add(Units_HideInEditor, 1, wxEXPAND);
	Units_Holder_HideInEditor->Add(2, 2);
	Units_Holder_HideInEditor->Add(Units_CheckBox_HideInEditor, 2, wxEXPAND);
	Units_Holder_Unknown1->Add(Units_Unknown1, 1, wxEXPAND);
	Units_Holder_Enabled->Add(Units_Enabled, 1, wxEXPAND);
	Units_Holder_Enabled->Add(2, 2);
	Units_Holder_Enabled->Add(Units_CheckBox_Enabled, 2, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_PlacementBypassTerrainGrid->Add(Units_PlacementBypassTerrain[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_PlacementBypassTerrainGrid->Add(Units_ComboBox_PlacementBypassTerrain[loop], 1, wxEXPAND);
	Units_Holder_PlacementBypassTerrain->Add(2, 2);
	Units_Holder_PlacementBypassTerrain->Add(Units_Holder_PlacementBypassTerrainGrid, 0, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_PlacementTerrainGrid->Add(Units_PlacementTerrain[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_PlacementTerrainGrid->Add(Units_ComboBox_PlacementTerrain[loop], 1, wxEXPAND);
	Units_Holder_PlacementTerrain->Add(2, 2);
	Units_Holder_PlacementTerrain->Add(Units_Holder_PlacementTerrainGrid, 0, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_EditorRadius->Add(Units_EditorRadius[loop], 1, wxEXPAND);
	Units_Holder_BuildingMode->Add(Units_BuildingMode, 1, wxEXPAND);
	Units_Holder_VisibleInFog->Add(Units_VisibleInFog, 1, wxEXPAND);
	Units_Holder_VisibleInFog->Add(2, 2);
	Units_Holder_VisibleInFog->Add(Units_CheckBox_VisibleInFog, 2, wxEXPAND);
	Units_Holder_TerrainRestriction->Add(2, 2);
	Units_Holder_TerrainRestriction->Add(Units_TerrainRestriction, 1, wxEXPAND);
	Units_Holder_TerrainRestriction->Add(Units_ComboBox_TerrainRestriction, 1, wxEXPAND);
	Units_Holder_FlyMode->Add(Units_FlyMode, 1, wxEXPAND);
	Units_Holder_FlyMode->Add(2, 2);
	Units_Holder_FlyMode->Add(Units_CheckBox_FlyMode, 2, wxEXPAND);
	Units_Holder_ResourceCapacity->Add(2, 2);
	Units_Holder_ResourceCapacity->Add(Units_ResourceCapacity, 1, wxEXPAND);
	Units_Holder_ResourceDecay->Add(2, 2);
	Units_Holder_ResourceDecay->Add(Units_ResourceDecay, 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Grid_Unknown2->Add(Units_Unknown2[loop], 1, wxEXPAND);
	Units_Holder_Unknown2->Add(Units_Grid_Unknown2, 0, wxEXPAND);
	Units_Holder_InteractionMode->Add(2, 2);
	Units_Holder_InteractionMode->Add(Units_InteractionMode, 1, wxEXPAND);
	Units_Holder_MinimapMode->Add(2, 2);
	Units_Holder_MinimapMode->Add(Units_MinimapMode, 1, wxEXPAND);
	Units_Holder_CommandAttribute->Add(Units_CommandAttribute, 1, wxEXPAND);
	Units_Holder_Unknown3->Add(Units_Unknown3, 1, wxEXPAND);
	Units_Holder_Unknown3a->Add(Units_Unknown3a, 1, wxEXPAND);
	Units_Holder_LanguageDllHelp->Add(2, 2);
	Units_Holder_LanguageDllHelp->Add(Units_LanguageDllHelp, 1, wxEXPAND);
	Units_Holder_LanguageDllHelp->Add(Units_DLL_LanguageDllHelp, 1, wxEXPAND);
	for(short loop = 0;loop < 4;loop++)
	Units_Holder_HotKey->Add(Units_HotKey[loop], 1, wxEXPAND);
	Units_Holder_HotKey->Add(Units_DLL_HotKey4, 1, wxEXPAND);
	Units_Holder_Unknown4->Add(Units_Unknown4, 1, wxEXPAND);
	Units_Holder_Unknown5->Add(Units_Unknown5, 1, wxEXPAND);
	Units_Holder_Unselectable->Add(Units_CheckBox_Unselectable, 0, wxEXPAND);
	Units_Holder_Unselectable->Add(2, 2);
	Units_Holder_Unselectable->Add(Units_Unselectable, 1, wxEXPAND);
	Units_Holder_Unknown6->Add(Units_Unknown6, 1, wxEXPAND);
	Units_Holder_Unknown7->Add(Units_Unknown7, 1, wxEXPAND);
	Units_Holder_Unknown8->Add(Units_Unknown8, 1, wxEXPAND);
	Units_Holder_SelectionMask->Add(2, 2);
	Units_Holder_SelectionMask->Add(Units_SelectionMask, 0, wxEXPAND);
	Units_Holder_SelectionShapeType->Add(2, 2);
	Units_Holder_SelectionShapeType->Add(Units_SelectionShapeType, 0, wxEXPAND);
	Units_Holder_SelectionShape->Add(2, 2);
	Units_Holder_SelectionShape->Add(Units_SelectionShape, 0, wxEXPAND);
	for(short loop = 0;loop < 4;loop++)
	Units_Grid_Unknown9->Add(Units_Unknown9[loop], 1, wxEXPAND);
	Units_Holder_Unknown9->Add(Units_Grid_Unknown9, 0, wxEXPAND);
	Units_Holder_SelectionEffect->Add(2, 2);
	Units_Holder_SelectionEffect->Add(Units_SelectionEffect, 1, wxEXPAND);
	Units_Holder_EditorSelectionColour->Add(2, 2);
	Units_Holder_EditorSelectionColour->Add(Units_EditorSelectionColour, 1, wxEXPAND);
	Units_SelectionRadiusBox->Add(Units_SelectionRadius[0], 1, wxEXPAND);
	Units_SelectionRadiusBox->Add(5, 5);
	Units_SelectionRadiusBox->Add(Units_SelectionRadius[1], 1, wxEXPAND);
	Units_Holder_SelectionRadius->Add(2, 2);
	Units_Holder_SelectionRadius->Add(Units_SelectionRadiusBox, 1, wxEXPAND);
	Units_Holder_HPBarHeight2->Add(Units_HPBarHeight2, 1, wxEXPAND);
	Units_Holder_SelectionSound->Add(2, 2);
	Units_Holder_SelectionSound->Add(Units_SelectionSound, 1, wxEXPAND);
	Units_Holder_SelectionSound->Add(Units_ComboBox_SelectionSound, 1, wxEXPAND);
	Units_Holder_DyingSound->Add(2, 2);
	Units_Holder_DyingSound->Add(Units_DyingSound, 1, wxEXPAND);
	Units_Holder_DyingSound->Add(Units_ComboBox_DyingSound, 1, wxEXPAND);
	Units_Holder_AttackSound->Add(2, 2);
	Units_Holder_AttackSound->Add(Units_AttackSound, 1, wxEXPAND);
	Units_Holder_AttackSound->Add(Units_ComboBox_AttackSound, 1, wxEXPAND);
	Units_Holder_Name->Add(2, 2);
	Units_Holder_Name->Add(Units_Name, 1, wxEXPAND);
	Units_Holder_NameLength2->Add(2, 2);
	Units_Holder_NameLength2->Add(Units_NameLength2, 1, wxEXPAND);
	Units_Holder_Name2->Add(2, 2);
	Units_Holder_Name2->Add(Units_Name2, 1, wxEXPAND);
	Units_Holder_Unitline->Add(Units_Unitline, 1, wxEXPAND);
	Units_Holder_MinTechLevel->Add(Units_MinTechLevel, 1, wxEXPAND);
	Units_Holder_ID2->Add(Units_ID2, 1, wxEXPAND);
	Units_Holder_ID3->Add(Units_ID3, 1, wxEXPAND);

//	Type 20+

	Units_Holder_Speed->Add(2, 2);
	Units_Holder_Speed->Add(Units_Speed, 1, wxEXPAND);

//	Type 30+

	Units_Holder_RotationSpeed->Add(2, 2);
	Units_Holder_RotationSpeed->Add(Units_RotationSpeed, 1, wxEXPAND);
	Units_Holder_Unknown11->Add(Units_Unknown11, 1, wxEXPAND);
	Units_Holder_TrackingUnitBox->Add(Units_TrackingUnit, 1, wxEXPAND);
	Units_Holder_TrackingUnitBox->Add(Units_ComboBox_TrackingUnit, 2, wxEXPAND);
	Units_Holder_TrackingUnit->Add(2, 2);
	Units_Holder_TrackingUnit->Add(Units_Holder_TrackingUnitBox, 0, wxEXPAND);
	Units_Holder_TrackingUnitUsedBox->Add(Units_TrackingUnitUsed, 2, wxEXPAND);
	Units_Holder_TrackingUnitUsedBox->Add(2, 2);
	Units_Holder_TrackingUnitUsedBox->Add(Units_CheckBox_TrackingUnitUsed, 1, wxEXPAND);
	Units_Holder_TrackingUnitUsed->Add(2, 2);
	Units_Holder_TrackingUnitUsed->Add(Units_Holder_TrackingUnitUsedBox, 0, wxEXPAND);
	Units_Holder_TrackingUnitDensity->Add(2, 2);
	Units_Holder_TrackingUnitDensity->Add(Units_TrackingUnitDensity, 1, wxEXPAND);
	Units_Holder_Unknown12->Add(Units_Unknown12, 1, wxEXPAND);
	for(short loop = 0;loop < 17;loop++)
	Units_Holder_Unknown16->Add(Units_Unknown16[loop], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);

//	Type 40+

	Units_Holder_SheepConversionBox->Add(Units_SheepConversion, 2, wxEXPAND);
	Units_Holder_SheepConversionBox->Add(2, 2);
	Units_Holder_SheepConversionBox->Add(Units_CheckBox_SheepConversion, 1, wxEXPAND);
	Units_Holder_SheepConversion->Add(Units_Holder_SheepConversionBox, 1, wxEXPAND);
	Units_Holder_SearchRadius->Add(2, 2);
	Units_Holder_SearchRadius->Add(Units_SearchRadius, 1, wxEXPAND);
	Units_Holder_WorkRate->Add(2, 2);
	Units_Holder_WorkRate->Add(Units_WorkRate, 1, wxEXPAND);
	Units_Holder_DropSite->Add(Units_DropSite[0], 1, wxEXPAND);
	Units_Holder_DropSite->Add(Units_ComboBox_DropSite[0], 2, wxEXPAND);
	Units_Holder_DropSite->Add(5, 5);
	Units_Holder_DropSite->Add(Units_DropSite[1], 1, wxEXPAND);
	Units_Holder_DropSite->Add(Units_ComboBox_DropSite[1], 2, wxEXPAND);
	Units_Holder_VillagerMode->Add(2, 2);
	Units_Holder_VillagerMode->Add(Units_VillagerMode, 1, wxEXPAND);
	Units_Holder_MoveSound->Add(2, 2);
	Units_Holder_MoveSound->Add(Units_MoveSound, 1, wxEXPAND);
	Units_Holder_MoveSound->Add(Units_ComboBox_MoveSound, 1, wxEXPAND);
	Units_Holder_StopSound->Add(2, 2);
	Units_Holder_StopSound->Add(Units_StopSound, 1, wxEXPAND);
	Units_Holder_StopSound->Add(Units_ComboBox_StopSound, 1, wxEXPAND);
	Units_Holder_Unknown19->Add(Units_Unknown19, 1, wxEXPAND);
	Units_Holder_Exists->Add(-1, 2);
	Units_Holder_Exists->Add(Units_Exists, 1, wxEXPAND);

//	Type 60+

	for(short loop = 0;loop < 2;loop++)
	Units_Holder_Unknown20->Add(Units_Unknown20[loop], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Unknown21->Add(Units_Unknown21, 1, wxEXPAND);
	Units_Holder_MaxRange->Add(2, 2);
	Units_Holder_MaxRange->Add(Units_MaxRange, 1, wxEXPAND);
	Units_Holder_BlastRadius->Add(2, 2);
	Units_Holder_BlastRadius->Add(Units_BlastRadius, 1, wxEXPAND);
	Units_Holder_ReloadTime1->Add(2, 2);
	Units_Holder_ReloadTime1->Add(Units_ReloadTime1, 1, wxEXPAND);
	Units_Holder_AccuracyPercent->Add(2, 2);
	Units_Holder_AccuracyPercent->Add(Units_AccuracyPercent, 1, wxEXPAND);
	Units_Holder_Unknown22->Add(Units_Unknown22, 1, wxEXPAND);
	Units_Holder_Delay->Add(2, 2);
	Units_Holder_Delay->Add(Units_Delay, 1, wxEXPAND);
	for(short loop = 0;loop < 3;loop++)
	Units_Holder_GraphicDisplacement->Add(Units_GraphicDisplacement[loop], 1, wxEXPAND);
	Units_Holder_Unknown23->Add(Units_Unknown23, 1, wxEXPAND);
	Units_Holder_MinRange->Add(2, 2);
	Units_Holder_MinRange->Add(Units_MinRange, 1, wxEXPAND);
	Units_Holder_GarrisonRecoveryRate->Add(2, 2);
	Units_Holder_GarrisonRecoveryRate->Add(Units_GarrisonRecoveryRate, 1, wxEXPAND);
	Units_Holder_AttackGraphic->Add(2, 2);
	Units_Holder_AttackGraphic->Add(Units_AttackGraphic, 1, wxEXPAND);
	Units_Holder_AttackGraphic->Add(Units_ComboBox_AttackGraphic, 1, wxEXPAND);
	Units_Holder_DisplayedMeleeArmour->Add(2, 2);
	Units_Holder_DisplayedMeleeArmour->Add(Units_DisplayedMeleeArmour, 1, wxEXPAND);
	Units_Holder_DisplayedAttack->Add(2, 2);
	Units_Holder_DisplayedAttack->Add(Units_DisplayedAttack, 1, wxEXPAND);
	Units_Holder_DisplayedRange->Add(2, 2);
	Units_Holder_DisplayedRange->Add(Units_DisplayedRange, 1, wxEXPAND);
	Units_Holder_ReloadTime2->Add(2, 2);
	Units_Holder_ReloadTime2->Add(Units_ReloadTime2, 1, wxEXPAND);

//	Type 60 only

	Units_Holder_StretchMode->Add(2, 2);
	Units_Holder_StretchMode->Add(Units_StretchMode, 1, wxEXPAND);
	Units_Holder_CompensationMode->Add(2, 2);
	Units_Holder_CompensationMode->Add(Units_CompensationMode, 1, wxEXPAND);
	Units_Holder_DropAnimationMode->Add(2, 2);
	Units_Holder_DropAnimationMode->Add(Units_DropAnimationMode, 1, wxEXPAND);
	Units_Holder_PenetrationMode->Add(2, 2);
	Units_Holder_PenetrationMode->Add(Units_PenetrationMode, 1, wxEXPAND);
	Units_Holder_Unknown24->Add(2, 2);
	Units_Holder_Unknown24->Add(Units_Unknown24, 1, wxEXPAND);
	Units_Holder_Unknown25->Add(2, 2);
	Units_Holder_Unknown25->Add(Units_Unknown25, 1, wxEXPAND);

//	Type 70+

	for(short loop = 0;loop < 3;loop++)
	Units_Grid_CostType->Add(Units_CostType[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 3;loop++)
	Units_Grid_CostType->Add(Units_ComboBox_CostType[loop], 1, wxEXPAND);
	Units_Holder_CostType->Add(Units_Grid_CostType, 12, wxEXPAND);
	Units_Holder_CostAmount->Add(Units_CostAmount[0], 4, wxEXPAND);
	Units_Holder_CostAmount->Add(5, 5);
	Units_Holder_CostAmount->Add(Units_CostAmount[1], 4, wxEXPAND);
	Units_Holder_CostAmount->Add(5, 5);
	Units_Holder_CostAmount->Add(Units_CostAmount[2], 4, wxEXPAND);
	Units_Holder_CostUsed->Add(Units_CostUsed[0], 3, wxEXPAND);
	Units_Holder_CostUsed->Add(2, 2);
	Units_Holder_CostUsed->Add(Units_CheckBox_CostUsed[0], 1, wxEXPAND);
	Units_Holder_CostUsed->Add(5, 5);
	Units_Holder_CostUsed->Add(Units_CostUsed[1], 3, wxEXPAND);
	Units_Holder_CostUsed->Add(2, 2);
	Units_Holder_CostUsed->Add(Units_CheckBox_CostUsed[1], 1, wxEXPAND);
	Units_Holder_CostUsed->Add(5, 5);
	Units_Holder_CostUsed->Add(Units_CostUsed[2], 3, wxEXPAND);
	Units_Holder_CostUsed->Add(2, 2);
	Units_Holder_CostUsed->Add(Units_CheckBox_CostUsed[2], 1, wxEXPAND);
	Units_Holder_TrainTime->Add(2, 2);
	Units_Holder_TrainTime->Add(Units_TrainTime, 1, wxEXPAND);
	Units_Holder_TrainLocationID1->Add(Units_TrainLocationID, 1, wxEXPAND);
	Units_Holder_TrainLocationID1->Add(Units_ComboBox_TrainLocationID, 2, wxEXPAND);
	Units_Holder_TrainLocationID->Add(2, 2);
	Units_Holder_TrainLocationID->Add(Units_Holder_TrainLocationID1, 0, wxEXPAND);
	Units_Holder_ButtonID->Add(2, 2);
	Units_Holder_ButtonID->Add(Units_ButtonID, 1, wxEXPAND);
	Units_Holder_Unknown26->Add(Units_Unknown26, 1, wxEXPAND);
	for(short loop = 0;loop < 3;loop++)
	Units_Holder_Unknown27->Add(Units_Unknown27[loop], 1, wxEXPAND);
	Units_Holder_Unknown28->Add(Units_Unknown28, 1, wxEXPAND);
	Units_Holder_MissileGraphicDelay->Add(2, 2);
	Units_Holder_MissileGraphicDelay->Add(Units_MissileGraphicDelay, 1, wxEXPAND);
	Units_Holder_HeroMode->Add(Units_HeroMode, 1, wxEXPAND);
	Units_Holder_HeroMode->Add(2, 2);
	Units_Holder_HeroMode->Add(Units_CheckBox_HeroMode, 2, wxEXPAND);
	Units_Holder_AttackMissileDuplicationAmount1->Add(2, 2);
	Units_Holder_AttackMissileDuplicationAmount1->Add(Units_AttackMissileDuplicationAmount1, 1, wxEXPAND);
	Units_Holder_AttackMissileDuplicationAmount2->Add(2, 2);
	Units_Holder_AttackMissileDuplicationAmount2->Add(Units_AttackMissileDuplicationAmount2, 1, wxEXPAND);
	for(short loop = 0;loop < 3;loop++)
	Units_Holder_AttackMissileDuplicationUnknown->Add(Units_AttackMissileDuplicationUnknown[loop], 1, wxEXPAND);
	Units_Holder_Unknown29->Add(Units_Unknown29, 1, wxEXPAND);
	Units_Holder_DisplayedPierceArmour->Add(2, 2);
	Units_Holder_DisplayedPierceArmour->Add(Units_DisplayedPierceArmour, 1, wxEXPAND);

//	Type 80

	Units_Holder_ConstructionGraphicID->Add(2, 2);
	Units_Holder_ConstructionGraphicID->Add(Units_ConstructionGraphicID, 1, wxEXPAND);
	Units_Holder_ConstructionGraphicID->Add(Units_ComboBox_ConstructionGraphicID, 1, wxEXPAND);
	Units_Holder_SnowGraphicID->Add(2, 2);
	Units_Holder_SnowGraphicID->Add(Units_SnowGraphicID, 1, wxEXPAND);
	Units_Holder_SnowGraphicID->Add(Units_ComboBox_SnowGraphicID, 1, wxEXPAND);
	Units_Holder_Unknown30->Add(Units_Unknown30, 1, wxEXPAND);
	Units_Holder_Unknown31->Add(Units_Unknown31, 1, wxEXPAND);
	Units_Holder_StackUnitID->Add(Units_StackUnitID, 1, wxEXPAND);
	Units_Holder_StackUnitID->Add(Units_ComboBox_StackUnitID, 1, wxEXPAND);
	Units_Holder_TerrainID->Add(Units_TerrainID, 1, wxEXPAND);
	Units_Holder_TerrainID->Add(Units_ComboBox_TerrainID, 1, wxEXPAND);
	Units_Holder_Unknown32->Add(Units_Unknown32, 1, wxEXPAND);
	Units_Holder_ResearchID->Add(Units_ResearchID, 1, wxEXPAND);
	Units_Holder_ResearchID->Add(Units_ComboBox_ResearchID, 1, wxEXPAND);
	Units_Holder_Unknown33->Add(Units_Unknown33, 1, wxEXPAND);
	for(short loop = 0;loop < 4;loop++)
	Units_Grid_AnnexUnit->Add(Units_AnnexUnit[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 4;loop++)
	Units_Grid_AnnexUnit->Add(Units_ComboBox_AnnexUnit[loop], 1, wxEXPAND);
	Units_Holder_AnnexUnit1->Add(2, 2);
	Units_Holder_AnnexUnit1->Add(Units_Grid_AnnexUnit, 0, wxEXPAND);
	for(short loop = 0;loop < 4;loop++)
	{
		for(short loop2 = 0;loop2 < 2;loop2++)
		Units_Grid_AnnexUnitMisplacement->Add(Units_AnnexUnitMisplacement[loop][loop2], 1, wxEXPAND);
	}
	Units_Holder_AnnexUnitMisplacement1->Add(2, 2);
	Units_Holder_AnnexUnitMisplacement1->Add(Units_Grid_AnnexUnitMisplacement, 0, wxEXPAND);
	Units_Holder_HeadUnit->Add(Units_HeadUnit, 1, wxEXPAND);
	Units_Holder_HeadUnit->Add(Units_ComboBox_HeadUnit, 2, wxEXPAND);
	Units_Holder_TransformUnit->Add(Units_TransformUnit, 1, wxEXPAND);
	Units_Holder_TransformUnit->Add(Units_ComboBox_TransformUnit, 2, wxEXPAND);
	Units_Holder_Unknown34->Add(Units_Unknown34, 1, wxEXPAND);
	Units_Holder_ConstructionSound->Add(2, 2);
	Units_Holder_ConstructionSound->Add(Units_ConstructionSound, 1, wxEXPAND);
	Units_Holder_ConstructionSound->Add(Units_ComboBox_ConstructionSound, 1, wxEXPAND);
	Units_Holder_GarrisonHealRate->Add(2, 2);
	Units_Holder_GarrisonHealRate->Add(Units_GarrisonHealRate, 1, wxEXPAND);
	Units_Holder_Unknown35->Add(Units_Unknown35, 1, wxEXPAND);
	Units_Holder_Unknown36->Add(Units_Unknown36, 1, wxEXPAND);
	for(short loop = 0;loop < 6;loop++)
	Units_Holder_Unknown37->Add(Units_Unknown37[loop], 1, wxEXPAND);
	
	for(short loop = 0;loop < 3;loop++)
	Units_Grid_ResourceStorage->Add(ResourceStorage_Type[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 3;loop++)
	Units_Grid_ResourceStorage->Add(ResourceStorage_ComboBox_Type[loop], 1, wxEXPAND);
	Units_Holder_ResourceStorage[0]->Add(ResourceStorage_Text_Type, 2, wxEXPAND);
	Units_Holder_ResourceStorage[0]->Add(Units_Grid_ResourceStorage, 12, wxEXPAND);
	Units_Holder_ResourceStorage[1]->Add(ResourceStorage_Text_Amount, 2, wxEXPAND);
	Units_Holder_ResourceStorage[1]->Add(ResourceStorage_Amount[0], 4, wxEXPAND);
	Units_Holder_ResourceStorage[1]->Add(5, 5);
	Units_Holder_ResourceStorage[1]->Add(ResourceStorage_Amount[1], 4, wxEXPAND);
	Units_Holder_ResourceStorage[1]->Add(5, 5);
	Units_Holder_ResourceStorage[1]->Add(ResourceStorage_Amount[2], 4, wxEXPAND);
	Units_Holder_ResourceStorage[2]->Add(ResourceStorage_Text_Enabled, 2, wxEXPAND);
	Units_Holder_ResourceStorage[2]->Add(ResourceStorage_Enabled[0], 4, wxEXPAND);
	Units_Holder_ResourceStorage[2]->Add(5, 5);
	Units_Holder_ResourceStorage[2]->Add(ResourceStorage_Enabled[1], 4, wxEXPAND);
	Units_Holder_ResourceStorage[2]->Add(5, 5);
	Units_Holder_ResourceStorage[2]->Add(ResourceStorage_Enabled[2], 4, wxEXPAND);
	
	Attacks_Holder_Class1->Add(Attacks_Class, 1, wxEXPAND);
	Attacks_Holder_Class1->Add(Attacks_ComboBox_Class, 2, wxEXPAND);
	Attacks_Holder_Class->Add(Attacks_Text_Class, 0, wxEXPAND);
	Attacks_Holder_Class->Add(2, 2);
	Attacks_Holder_Class->Add(Attacks_Holder_Class1, 1, wxEXPAND);
	Attacks_Holder_Amount->Add(Attacks_Text_Amount, 0, wxEXPAND);
	Attacks_Holder_Amount->Add(2, 2);
	Attacks_Holder_Amount->Add(Attacks_Amount, 1, wxEXPAND);
	
	Units_Attacks_Buttons->Add(Units_Attacks_Add, 1, wxEXPAND);
	Units_Attacks_Buttons->Add(Units_Attacks_Delete, 1, wxEXPAND);
	Units_Attacks_Buttons->Add(Units_Attacks_Copy, 1, wxEXPAND);
	Units_Attacks_Buttons->Add(Units_Attacks_Paste, 1, wxEXPAND);
	
	Units_Attacks->Add(Units_Attacks_Search, 0, wxEXPAND);
	Units_Attacks->Add(Units_Attacks_Search_R, 0, wxEXPAND);
	Units_Attacks->Add(-1, 2);
	Units_Attacks->Add(Units_Attacks_List, 1, wxEXPAND);
	Units_Attacks->Add(-1, 2);
	Units_Attacks->Add(Units_Attacks_Buttons, 0, wxEXPAND);
	
	Armors_Holder_Class1->Add(Armors_Class, 1, wxEXPAND);
	Armors_Holder_Class1->Add(Armors_ComboBox_Class, 2, wxEXPAND);
	Armors_Holder_Class->Add(Armors_Text_Class, 0, wxEXPAND);
	Armors_Holder_Class->Add(2, 2);
	Armors_Holder_Class->Add(Armors_Holder_Class1, 1, wxEXPAND);
	Armors_Holder_Amount->Add(Armors_Text_Amount, 0, wxEXPAND);
	Armors_Holder_Amount->Add(2, 2);
	Armors_Holder_Amount->Add(Armors_Amount, 1, wxEXPAND);
	
	Units_Armors_Buttons->Add(Units_Armors_Add, 1, wxEXPAND);
	Units_Armors_Buttons->Add(Units_Armors_Delete, 1, wxEXPAND);
	Units_Armors_Buttons->Add(Units_Armors_Copy, 1, wxEXPAND);
	Units_Armors_Buttons->Add(Units_Armors_Paste, 1, wxEXPAND);
	
	Units_Armors->Add(Units_Armors_Search, 0, wxEXPAND);
	Units_Armors->Add(Units_Armors_Search_R, 0, wxEXPAND);
	Units_Armors->Add(-1, 2);
	Units_Armors->Add(Units_Armors_List, 1, wxEXPAND);
	Units_Armors->Add(-1, 2);
	Units_Armors->Add(Units_Armors_Buttons, 0, wxEXPAND);
	
	UnitCommands_Holder_One->Add(UnitCommands_Text_One, 0, wxEXPAND);
	UnitCommands_Holder_One->Add(UnitCommands_One, 1, wxEXPAND);
	UnitCommands_Holder_ID->Add(UnitCommands_Text_ID, 0, wxEXPAND);
	UnitCommands_Holder_ID->Add(UnitCommands_ID, 1, wxEXPAND);
	UnitCommands_Holder_Unknown1->Add(UnitCommands_Text_Unknown1, 0, wxEXPAND);
	UnitCommands_Holder_Unknown1->Add(UnitCommands_Unknown1, 1, wxEXPAND);
	UnitCommands_Holder_Type->Add(UnitCommands_Text_Type, 0, wxEXPAND);
	UnitCommands_Holder_Type->Add(2, 2);
	UnitCommands_Holder_Type->Add(UnitCommands_Type, 1, wxEXPAND);
	UnitCommands_Holder_SubType->Add(UnitCommands_Text_SubType, 0, wxEXPAND);
	UnitCommands_Holder_SubType->Add(2, 2);
	UnitCommands_Holder_SubType->Add(UnitCommands_SubType, 1, wxEXPAND);
	UnitCommands_Holder_ClassID->Add(UnitCommands_Text_ClassID, 0, wxEXPAND);
	UnitCommands_Holder_ClassID->Add(-1, 2);
	UnitCommands_Holder_ClassID->Add(UnitCommands_ClassID, 1, wxEXPAND);
	UnitCommands_Holder_ClassID->Add(UnitCommands_ComboBox_ClassID, 1, wxEXPAND);
	UnitCommands_Holder_UnitID->Add(UnitCommands_Text_UnitID, 0, wxEXPAND);
	UnitCommands_Holder_UnitID->Add(-1, 2);
	UnitCommands_Holder_UnitID->Add(UnitCommands_UnitID, 1, wxEXPAND);
	UnitCommands_Holder_UnitID->Add(UnitCommands_ComboBox_UnitID, 1, wxEXPAND);
	UnitCommands_Holder_Unknown2->Add(UnitCommands_Text_Unknown2, 0, wxEXPAND);
	UnitCommands_Holder_Unknown2->Add(UnitCommands_Unknown2, 1, wxEXPAND);
	UnitCommands_Holder_ResourceIn->Add(UnitCommands_Text_ResourceIn, 0, wxEXPAND);
	UnitCommands_Holder_ResourceIn->Add(-1, 2);
	UnitCommands_Holder_ResourceIn->Add(UnitCommands_ResourceIn, 1, wxEXPAND);
	UnitCommands_Holder_ResourceIn->Add(UnitCommands_ComboBox_ResourceIn, 1, wxEXPAND);
	UnitCommands_Holder_ResourceOut->Add(UnitCommands_Text_ResourceOut, 0, wxEXPAND);
	UnitCommands_Holder_ResourceOut->Add(-1, 2);
	UnitCommands_Holder_ResourceOut->Add(UnitCommands_ResourceOut, 1, wxEXPAND);
	UnitCommands_Holder_ResourceOut->Add(UnitCommands_ComboBox_ResourceOut, 1, wxEXPAND);
	UnitCommands_Holder_Unknown3->Add(UnitCommands_Text_Unknown3, 0, wxEXPAND);
	UnitCommands_Holder_Unknown3->Add(UnitCommands_Unknown3, 1, wxEXPAND);
	UnitCommands_Holder_Unknown4->Add(UnitCommands_Text_Unknown4, 0, wxEXPAND);
	UnitCommands_Holder_Unknown4->Add(UnitCommands_Unknown4, 1, wxEXPAND);
	UnitCommands_Holder_ExecutionRadius->Add(UnitCommands_Text_ExecutionRadius, 0, wxEXPAND);
	UnitCommands_Holder_ExecutionRadius->Add(UnitCommands_ExecutionRadius, 1, wxEXPAND);
	UnitCommands_Holder_Unknown5->Add(UnitCommands_Text_Unknown5, 0, wxEXPAND);
	UnitCommands_Holder_Unknown5->Add(UnitCommands_Unknown5, 1, wxEXPAND);
	UnitCommands_Holder_Unknown6->Add(UnitCommands_Text_Unknown6, 0, wxEXPAND);
	UnitCommands_Holder_Unknown6->Add(UnitCommands_Unknown6, 1, wxEXPAND);
	UnitCommands_Holder_Unknown7->Add(UnitCommands_Text_Unknown7, 0, wxEXPAND);
	UnitCommands_Holder_Unknown7->Add(UnitCommands_Unknown7, 1, wxEXPAND);
	UnitCommands_Holder_Unknown8->Add(UnitCommands_Text_Unknown8, 0, wxEXPAND);
	UnitCommands_Holder_Unknown8->Add(UnitCommands_Unknown8, 1, wxEXPAND);
	UnitCommands_Holder_Unknown9->Add(UnitCommands_Text_Unknown9, 0, wxEXPAND);
	UnitCommands_Holder_Unknown9->Add(UnitCommands_Unknown9, 1, wxEXPAND);
	UnitCommands_Holder_Unknown10->Add(UnitCommands_Text_Unknown10, 0, wxEXPAND);
	UnitCommands_Holder_Unknown10->Add(UnitCommands_Unknown10, 1, wxEXPAND);
	UnitCommands_Holder_Unknown11->Add(UnitCommands_Text_Unknown11, 0, wxEXPAND);
	UnitCommands_Holder_Unknown11->Add(UnitCommands_Unknown11, 1, wxEXPAND);
	UnitCommands_Holder_Unknown12->Add(UnitCommands_Text_Unknown12, 0, wxEXPAND);
	UnitCommands_Holder_Unknown12->Add(UnitCommands_Unknown12, 1, wxEXPAND);
	UnitCommands_Holder_Unknown13->Add(UnitCommands_Text_Unknown13, 0, wxEXPAND);
	UnitCommands_Holder_Unknown13->Add(UnitCommands_Unknown13, 1, wxEXPAND);
	UnitCommands_Holder_Unknown14->Add(UnitCommands_Text_Unknown14, 0, wxEXPAND);
	UnitCommands_Holder_Unknown14->Add(UnitCommands_Unknown14, 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_Graphics[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_ComboBox_Graphics[loop], 1, wxEXPAND);
	for(short loop = 2;loop < 4;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_Graphics[loop], 1, wxEXPAND);
	for(short loop = 2;loop < 4;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_ComboBox_Graphics[loop], 1, wxEXPAND);
	for(short loop = 4;loop < 6;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_Graphics[loop], 1, wxEXPAND);
	for(short loop = 4;loop < 6;loop++)
	UnitCommands_Holder_GraphicsGrid->Add(UnitCommands_ComboBox_Graphics[loop], 1, wxEXPAND);
	UnitCommands_Holder_Graphics->Add(UnitCommands_Text_Graphics, 1, wxEXPAND);
	UnitCommands_Holder_Graphics->Add(2, 2);
	UnitCommands_Holder_Graphics->Add(UnitCommands_Holder_GraphicsGrid, 0, wxEXPAND);
	
	Units_Holder_TypeArea1->Add(Units_Holder_Class, 3, wxEXPAND);
	Units_Holder_TypeArea2->Add(Units_Holder_NameLength, 1, wxEXPAND);
	Units_Holder_TypeArea2->Add(Units_Holder_Name, 2, wxEXPAND);
	Units_Holder_TypeArea2->Add(5, 5);
	Units_Holder_TypeArea2->Add(Units_Holder_NameLength2, 1, wxEXPAND);
	Units_Holder_TypeArea2->Add(Units_Holder_Name2, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_TypeArea1->Add(5, 5);
	Units_Holder_TypeArea1->Add(Units_Holder_ID1, 1, wxEXPAND);
	Units_Holder_TypeArea1->Add(5, 5);
	Units_Holder_TypeArea1->Add(Units_Holder_ID2, 1, wxEXPAND);
	Units_Holder_TypeArea1->Add(5, 5);
	Units_Holder_TypeArea1->Add(Units_Holder_ID3, 1, wxEXPAND);
	
	Units_Holder_TypeArea->Add(Units_Holder_TypeArea1, 0, wxEXPAND);
	Units_Holder_TypeArea->Add(5, 5);
	Units_Holder_TypeArea->Add(Units_Holder_TypeArea2, 0, wxEXPAND);
	
	Units_Holder_LangDLLArea->Add(Units_Holder_LanguageDllName, 1, wxEXPAND);
	Units_Holder_LangDLLArea->Add(5, 5);
	Units_Holder_LangDLLArea->Add(Units_Holder_LanguageDllCreation, 1, wxEXPAND);
	Units_Holder_LangDLLArea->Add(5, 5);
	Units_Holder_LangDLLArea->Add(Units_Holder_LanguageDllHelp, 1, wxEXPAND);
	
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea2->Add(Units_StandingGraphic[loop], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea2->Add(Units_GarrisonGraphic[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea2->Add(Units_ComboBox_StandingGraphic[loop], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea2->Add(Units_ComboBox_GarrisonGraphic[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea3->Add(Units_WalkingGraphic[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea3->Add(Units_DyingGraphic[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea3->Add(Units_ComboBox_WalkingGraphic[loop], 1, wxEXPAND);
	for(short loop = 0;loop < 2;loop++)
	Units_Holder_GraphicsArea3->Add(Units_ComboBox_DyingGraphic[loop], 1, wxEXPAND);
	Units_Holder_StandingGraphic->Add(Units_Text_StandingGraphic, 0, wxEXPAND);
	Units_Holder_GarrisonGraphic->Add(Units_Text_GarrisonGraphic, 0, wxEXPAND);
	Units_Holder_WalkingGraphic->Add(Units_Text_WalkingGraphic, 0, wxEXPAND);
	Units_Holder_DyingGraphic->Add(Units_Text_DyingGraphic, 0, wxEXPAND);
	
	Units_Holder_GraphicsArea1->Add(Units_Holder_IconID, 1, wxEXPAND);
	Units_Holder_GraphicsArea1->Add(5, 5);
	Units_Holder_GraphicsArea1->Add(Units_Holder_ConstructionGraphicID, 1, wxEXPAND);
	Units_Holder_GraphicsArea1->Add(5, 5);
	Units_Holder_GraphicsArea1->Add(Units_Holder_SnowGraphicID, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_GraphicsArea1->Add(5, 5);
	Units_Holder_GraphicsArea1->Add(Units_Holder_AttackGraphic, 1, wxEXPAND);
	Units_Holder_GraphicsArea4->Add(Units_Holder_StandingGraphic, 1, wxEXPAND);
	Units_Holder_GraphicsArea4->Add(5, 5);
	Units_Holder_GraphicsArea4->Add(Units_Holder_GarrisonGraphic, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_GraphicsArea5->Add(Units_Holder_WalkingGraphic, 1, wxEXPAND);
	Units_Holder_GraphicsArea5->Add(5, 5);
	Units_Holder_GraphicsArea5->Add(Units_Holder_DyingGraphic, 1, wxEXPAND);
	
	Units_Holder_GraphicsArea6->Add(Units_Holder_GraphicsArea4, 0, wxEXPAND);
	Units_Holder_GraphicsArea6->Add(5, 5);
	Units_Holder_GraphicsArea6->Add(Units_Holder_GraphicsArea2, 0, wxEXPAND);
	Units_Holder_GraphicsArea7->Add(Units_Holder_GraphicsArea5, 0, wxEXPAND);
	Units_Holder_GraphicsArea7->Add(5, 5);
	Units_Holder_GraphicsArea7->Add(Units_Holder_GraphicsArea3, 0, wxEXPAND);
	
	DamageGraphics_Holder_GraphicID->Add(DamageGraphics_Text_GraphicID, 0, wxEXPAND);
	DamageGraphics_Holder_GraphicID->Add(2, 2);
	DamageGraphics_Holder_GraphicID->Add(DamageGraphics_GraphicID, 1, wxEXPAND);
	DamageGraphics_Holder_GraphicID->Add(DamageGraphics_ComboBox_GraphicID, 1, wxEXPAND);
	DamageGraphics_Holder_DamagePercent->Add(DamageGraphics_Text_DamagePercent, 0, wxEXPAND);
	DamageGraphics_Holder_DamagePercent->Add(2, 2);
	DamageGraphics_Holder_DamagePercent->Add(DamageGraphics_DamagePercent, 1, wxEXPAND);
	DamageGraphics_Holder_Unknown1->Add(DamageGraphics_Text_Unknown1, 0, wxEXPAND);
	DamageGraphics_Holder_Unknown1->Add(2, 2);
	DamageGraphics_Holder_Unknown1->Add(DamageGraphics_Unknown1, 1, wxEXPAND);
	Units_Holder_DamageGraphics_Data->Add(5, 5);
	Units_Holder_DamageGraphics_Data->Add(DamageGraphics_Holder_GraphicID, 0, wxEXPAND);
	Units_Holder_DamageGraphics_Data->Add(5, 5);
	Units_Holder_DamageGraphics_Data->Add(DamageGraphics_Holder_DamagePercent, 0, wxEXPAND);
	Units_Holder_DamageGraphics_Data->Add(5, 5);
	Units_Holder_DamageGraphics_Data->Add(DamageGraphics_Holder_Unknown1, 0, wxEXPAND);
	
	Units_DamageGraphics_Buttons->Add(Units_DamageGraphics_Add, 1, wxEXPAND);
	Units_DamageGraphics_Buttons->Add(Units_DamageGraphics_Delete, 1, wxEXPAND);
	Units_DamageGraphics_Buttons->Add(Units_DamageGraphics_Copy, 1, wxEXPAND);
	Units_DamageGraphics_Buttons->Add(Units_DamageGraphics_Paste, 1, wxEXPAND);
	
	Units_DamageGraphics->Add(Units_DamageGraphics_Search, 0, wxEXPAND);
	Units_DamageGraphics->Add(Units_DamageGraphics_Search_R, 0, wxEXPAND);
	Units_DamageGraphics->Add(-1, 2);
	Units_DamageGraphics->Add(Units_DamageGraphics_List, 1, wxEXPAND);
	Units_DamageGraphics->Add(-1, 2);
	Units_DamageGraphics->Add(Units_DamageGraphics_Buttons, 0, wxEXPAND);
	
	Units_Holder_DamageGraphics_Space->Add(-1, 200);

	Units_Holder_DamageGraphics->Add(Units_DamageGraphics, 1, wxEXPAND);
	Units_Holder_DamageGraphics->Add(5, -1);
	Units_Holder_DamageGraphics->Add(Units_Holder_DamageGraphics_Data, 1, wxEXPAND);
	Units_Holder_DamageGraphics->Add(Units_Holder_DamageGraphics_Space, 2, wxEXPAND);
	
	Units_Holder_GraphicsArea->Add(Units_Holder_GraphicsArea1, 0, wxEXPAND);
	Units_Holder_GraphicsArea->Add(5, 5);
	Units_Holder_GraphicsArea->Add(Units_Holder_GraphicsArea6, 0, wxEXPAND);
	Units_Holder_GraphicsArea->Add(5, 5);
	Units_Holder_GraphicsArea->Add(Units_Holder_GraphicsArea7, 0, wxEXPAND);
	Units_Holder_GraphicsArea->Add(5, 5);
	Units_Holder_GraphicsArea->Add(Units_Holder_DamageGraphics, 0, wxEXPAND);
	
	Units_Holder_StatsArea1->Add(Units_Holder_HitPoints, 1, wxEXPAND);
	Units_Holder_StatsArea1->Add(5, 5);
	Units_Holder_StatsArea1->Add(Units_Holder_Speed, 1, wxEXPAND);
	Units_Holder_StatsArea1->Add(5, 5);
	Units_Holder_StatsArea1->Add(Units_Holder_RotationSpeed, 1, wxEXPAND);
	Units_Holder_StatsAreaRanges->Add(Units_Holder_LineOfSight, 1, wxEXPAND);
	Units_Holder_StatsAreaRanges->Add(5, 5);
	Units_Holder_StatsAreaRanges->Add(Units_Holder_SearchRadius, 1, wxEXPAND);
	Units_Holder_StatsAreaRanges->Add(5, 5);
	Units_Holder_StatsAreaRanges->Add(Units_Holder_MaxRange, 1, wxEXPAND);
	Units_Holder_StatsAreaRanges->Add(5, 5);
	Units_Holder_StatsAreaRanges->Add(Units_Holder_MinRange, 1, wxEXPAND);
	Units_Holder_StatsAreaRanges->Add(5, 5);
	Units_Holder_StatsAreaRanges->Add(Units_Holder_DisplayedRange, 1, wxEXPAND);
	
	Units_Holder_Attacks_Data1->Add(Attacks_Holder_Amount, 1, wxEXPAND);
	Units_Holder_Attacks_Data1->Add(5, 5);
	Units_Holder_Attacks_Data1->Add(Attacks_Holder_Class, 3, wxEXPAND);
	Units_Holder_Attacks_Data1->Add(5, 5);
	Units_Holder_Attacks_Data2->Add(Units_Holder_AccuracyPercent, 1, wxEXPAND);
	Units_Holder_Attacks_Data2->Add(5, 5);
	Units_Holder_Attacks_Data2->Add(Units_Holder_DisplayedAttack, 1, wxEXPAND);
	Units_Holder_Attacks_Data2->Add(5, 5);
	Units_Holder_Attacks_Data2->Add(Units_Holder_Delay, 1, wxEXPAND);
	Units_Holder_Attacks_Data3->Add(Units_Holder_BlastRadius, 1, wxEXPAND);
	Units_Holder_Attacks_Data3->Add(5, 5);
	Units_Holder_Attacks_Data3->Add(Units_Holder_ReloadTime1, 1, wxEXPAND);
	Units_Holder_Attacks_Data3->Add(5, 5);
	Units_Holder_Attacks_Data3->Add(Units_Holder_ReloadTime2, 1, wxEXPAND);
	
	Units_Holder_Attacks_Data->Add(Units_Holder_Attacks_Data1, 0, wxEXPAND);
	Units_Holder_Attacks_Data->Add(5, 5);
	Units_Holder_Attacks_Data->Add(Units_Holder_Attacks_Data2, 0, wxEXPAND);
	Units_Holder_Attacks_Data->Add(5, 5);
	Units_Holder_Attacks_Data->Add(Units_Holder_Attacks_Data3, 0, wxEXPAND);
	
	Units_Holder_Armors_Data1->Add(Armors_Holder_Amount, 1, wxEXPAND);
	Units_Holder_Armors_Data1->Add(5, 5);
	Units_Holder_Armors_Data1->Add(Armors_Holder_Class, 3, wxEXPAND);
	Units_Holder_Armors_Data1->Add(5, 5);
	Units_Holder_Armors_Data3->Add(Units_Holder_DisplayedMeleeArmour, 0, wxEXPAND);
	Units_Holder_Armors_Data3->Add(5, 5);
	Units_Holder_Armors_Data3->Add(Units_Holder_DisplayedPierceArmour, 0, wxEXPAND);
	
	Units_Holder_Armors_Data2->Add(Units_Holder_Armors_Data3, 1, wxEXPAND);
	Units_Holder_Armors_Data2->AddStretchSpacer(2);
	
	Units_Holder_Armors_Data->Add(Units_Holder_Armors_Data1, 0, wxEXPAND);
	Units_Holder_Armors_Data->Add(5, 5);
	Units_Holder_Armors_Data->Add(Units_Holder_Armors_Data2, 0, wxEXPAND);
	
	Units_Holder_Attacks_Space->Add(-1, 200);
	Units_Holder_Armors_Space->Add(-1, 200);
	
	Units_Holder_GarrisonType1->Add(Units_GarrisonType, 1, wxEXPAND);
	Units_Holder_GarrisonType1->Add(Units_ComboBox_GarrisonType, 2, wxEXPAND);
	Units_Holder_GarrisonType->Add(Units_Text_GarrisonType, 0, wxEXPAND);
	Units_Holder_GarrisonType->Add(2, 2);
	Units_Holder_GarrisonType->Add(Units_Holder_GarrisonType1, 0, wxEXPAND);

	Units_Holder_Attacks->Add(Units_Attacks, 1, wxEXPAND);
	Units_Holder_Attacks->Add(5, 5);
	Units_Holder_Attacks->Add(Units_Holder_Attacks_Data, 3, wxEXPAND);
	Units_Holder_Attacks->Add(Units_Holder_Attacks_Space, 0, wxEXPAND);
	Units_Holder_Armors->Add(Units_Armors, 1, wxEXPAND);
	Units_Holder_Armors->Add(5, 5);
	Units_Holder_Armors->Add(Units_Holder_Armors_Data, 3, wxEXPAND);
	Units_Holder_Armors->Add(Units_Holder_Armors_Space, 0, wxEXPAND);
	Units_Holder_StatsAreaGarrison->Add(Units_Holder_GarrisonType, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_StatsAreaGarrison->Add(5, 5);
	Units_Holder_StatsAreaGarrison->Add(Units_Holder_GarrisonHealRate, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_StatsAreaGarrison->Add(5, 5);
	Units_Holder_StatsAreaGarrison->Add(Units_Holder_GarrisonRecoveryRate, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_StatsAreaGarrison->Add(5, 5);
	Units_Holder_StatsAreaGarrison->Add(Units_Holder_GarrisonCapacity, 1, wxEXPAND);
	Units_Holder_StatsArea2->Add(Units_Holder_ResourceCapacity, 1, wxEXPAND);
	Units_Holder_StatsArea2->Add(5, 5);
	Units_Holder_StatsArea2->Add(Units_Holder_ResourceDecay, 1, wxEXPAND);
	Units_Holder_StatsArea2->Add(5, 5);
	Units_Holder_StatsArea2->Add(Units_Holder_WorkRate, 1, wxEXPAND);
	
	Units_Holder_StatsArea->Add(Units_Holder_StatsArea1, 0, wxEXPAND);
	Units_Holder_StatsArea->Add(5, 5);
	Units_Holder_StatsArea->Add(Units_Holder_StatsAreaRanges, 0, wxEXPAND);
	Units_Holder_StatsArea->Add(5, 5);
	Units_Holder_StatsArea->Add(Units_Holder_Attacks, 0, wxEXPAND);
	Units_Holder_StatsArea->Add(5, 5);
	Units_Holder_StatsArea->Add(Units_Holder_Armors, 0, wxEXPAND);
	Units_Holder_StatsArea->Add(5, 5);
	Units_Holder_StatsArea->Add(Units_Holder_StatsAreaGarrison, 0, wxEXPAND);
	Units_Holder_StatsArea->Add(5, 5);
	Units_Holder_StatsArea->Add(Units_Holder_StatsArea2, 0, wxEXPAND);
	
	Units_Holder_ProjectilesArea5->Add(Units_AttackMissileDuplicationGraphic, 1, wxEXPAND);
	Units_Holder_ProjectilesArea5->Add(Units_ComboBox_AttackMissileDuplicationGraphic, 2, wxEXPAND);
	Units_Holder_ProjectilesArea6->Add(Units_AttackMissileDuplicationUnit, 1, wxEXPAND);
	Units_Holder_ProjectilesArea6->Add(Units_ComboBox_AttackMissileDuplicationUnit, 2, wxEXPAND);
	Units_Holder_ProjectilesArea7->Add(Units_ProjectileUnitID, 1, wxEXPAND);
	Units_Holder_ProjectilesArea7->Add(Units_ComboBox_ProjectileUnitID, 2, wxEXPAND);
	
	Units_Holder_ProjectileUnitID->Add(Units_Text_ProjectileUnitID, 0, wxEXPAND);
	Units_Holder_ProjectileUnitID->Add(2, 2);
	Units_Holder_ProjectileUnitID->Add(Units_Holder_ProjectilesArea7, 1, wxEXPAND);
	Units_Holder_AttackMissileDuplicationUnit->Add(Units_Text_AttackMissileDuplicationUnit, 0, wxEXPAND);
	Units_Holder_AttackMissileDuplicationUnit->Add(2, 2);
	Units_Holder_AttackMissileDuplicationUnit->Add(Units_Holder_ProjectilesArea6, 1, wxEXPAND);
	Units_Holder_AttackMissileDuplicationGraphic->Add(Units_Text_AttackMissileDuplicationGraphic, 0, wxEXPAND);
	Units_Holder_AttackMissileDuplicationGraphic->Add(2, 2);
	Units_Holder_AttackMissileDuplicationGraphic->Add(Units_Holder_ProjectilesArea5, 1, wxEXPAND);

	Units_Holder_ProjectilesArea1->Add(Units_Holder_StretchMode, 1, wxEXPAND);
	Units_Holder_ProjectilesArea1->Add(5, 5);
	Units_Holder_ProjectilesArea1->Add(Units_Holder_CompensationMode, 1, wxEXPAND);
	Units_Holder_ProjectilesArea1->Add(5, 5);
	Units_Holder_ProjectilesArea1->Add(Units_Holder_DropAnimationMode, 1, wxEXPAND);
	Units_Holder_ProjectilesArea1->Add(5, 5);
	Units_Holder_ProjectilesArea1->Add(Units_Holder_PenetrationMode, 1, wxEXPAND);
	Units_Holder_ProjectilesArea2->Add(Units_Holder_Unknown24, 1, wxEXPAND);
	Units_Holder_ProjectilesArea2->Add(5, 5);
	Units_Holder_ProjectilesArea2->Add(Units_Holder_Unknown25, 1, wxEXPAND);
	Units_Holder_ProjectilesArea2->Add(5, 5);
	Units_Holder_ProjectilesArea2->Add(Units_Holder_ProjectileUnitID, 2, wxEXPAND);
	Units_Holder_ProjectilesArea3->Add(Units_Holder_MissileGraphicDelay, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_ProjectilesArea3->Add(5, 5);
	Units_Holder_ProjectilesArea3->Add(Units_Holder_AttackMissileDuplicationAmount1, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_ProjectilesArea3->Add(5, 5);
	Units_Holder_ProjectilesArea3->Add(Units_Holder_AttackMissileDuplicationAmount2, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_ProjectilesArea4->Add(Units_Holder_AttackMissileDuplicationUnit, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_ProjectilesArea4->Add(5, 5);
	Units_Holder_ProjectilesArea4->Add(Units_Holder_AttackMissileDuplicationGraphic, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	
	Units_Holder_ProjectilesArea->Add(Units_Holder_ProjectilesArea1, 0, wxEXPAND);
	Units_Holder_ProjectilesArea->Add(5, 5);
	Units_Holder_ProjectilesArea->Add(Units_Holder_ProjectilesArea2, 0, wxEXPAND);
	Units_Holder_ProjectilesArea->Add(5, 5);
	Units_Holder_ProjectilesArea->Add(Units_Holder_GraphicDisplacement, 0, wxEXPAND);
	Units_Holder_ProjectilesArea->Add(5, 5);
	Units_Holder_ProjectilesArea->Add(Units_Holder_ProjectilesArea3, 0, wxEXPAND);
	Units_Holder_ProjectilesArea->Add(5, 5);
	Units_Holder_ProjectilesArea->Add(Units_Holder_AttackMissileDuplicationUnknown, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_ProjectilesArea->Add(5, 5);
	Units_Holder_ProjectilesArea->Add(Units_Holder_ProjectilesArea4, 0, wxEXPAND);
	
	Units_Holder_AttributesBoxes1->Add(Units_Holder_Enabled, 1, wxEXPAND);
	Units_Holder_AttributesBoxes1->Add(5, 5);
	Units_Holder_AttributesBoxes1->Add(Units_Holder_HideInEditor, 1, wxEXPAND);
	Units_Holder_AttributesBoxes1->Add(5, 5);
	Units_Holder_AttributesBoxes1->Add(Units_Holder_VisibleInFog, 1, wxEXPAND);
	Units_Holder_AttributesBoxes1->Add(5, 5);
	Units_Holder_AttributesBoxes1->AddStretchSpacer(1);
	Units_Holder_AttributesBoxes2->Add(Units_Holder_DeathMode, 1, wxEXPAND);
	Units_Holder_AttributesBoxes2->Add(5, 5);
	Units_Holder_AttributesBoxes2->Add(Units_Holder_HeroMode, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesBoxes2->Add(5, 5);
	Units_Holder_AttributesBoxes2->Add(Units_Holder_AirMode, 1, wxEXPAND);
	Units_Holder_AttributesBoxes2->Add(5, 5);
	Units_Holder_AttributesBoxes2->Add(Units_Holder_FlyMode, 1, wxEXPAND);
	Units_Holder_Attributes1->Add(Units_Holder_DeadUnitID, 2, wxEXPAND);
	Units_Holder_Attributes1->Add(5, 5);
	Units_Holder_Attributes1->Add(Units_Holder_BuildingMode, 1, wxEXPAND);
	Units_Holder_Attributes1->Add(5, 5);
	Units_Holder_Attributes1->Add(Units_Holder_PlacementMode, 1, wxEXPAND);
	Units_Holder_AttributesTerrain->Add(Units_Holder_PlacementBypassTerrain, 2, wxEXPAND);
	Units_Holder_AttributesTerrain->Add(5, 5);
	Units_Holder_AttributesTerrain->Add(Units_Holder_PlacementTerrain, 2, wxEXPAND);
	Units_Holder_AttributesTerrain->Add(5, 5);
	Units_Holder_AttributesTerrain->Add(Units_Holder_TerrainRestriction, 1, wxEXPAND);
	Units_Holder_AttributesModes1->Add(Units_Holder_InteractionMode, 1, wxEXPAND);
	Units_Holder_AttributesModes1->Add(5, 5);
	Units_Holder_AttributesModes1->Add(Units_Holder_MinimapMode, 1, wxEXPAND);
	Units_Holder_AttributesModes1->Add(5, 5);
	Units_Holder_AttributesModes1->Add(Units_Holder_SheepConversion, 1, wxEXPAND);
	Units_Holder_AttributesModes1->Add(5, 5);
	Units_Holder_AttributesModes1->Add(Units_Holder_VillagerMode, 1, wxEXPAND);
	Units_Holder_AttributesDropSite->Add(Units_Holder_DropSite, 0, wxEXPAND);
	Units_Holder_AttributesSizes->Add(Units_Holder_SizeRadius, 1, wxEXPAND);
	Units_Holder_AttributesSizes->Add(5, 5);
	Units_Holder_AttributesSizes->Add(Units_Holder_EditorRadius, 1, wxEXPAND);
	Units_Holder_AttributesSelection1->Add(Units_Holder_Unselectable, 1, wxEXPAND);
	Units_Holder_AttributesSelection1->Add(5, 5);
	Units_Holder_AttributesSelection1->Add(Units_Holder_SelectionMask, 1, wxEXPAND);
	Units_Holder_AttributesSelection1->Add(5, 5);
	Units_Holder_AttributesSelection1->Add(Units_Holder_SelectionShapeType, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesSelection1->Add(5, 5);
	Units_Holder_AttributesSelection1->Add(Units_Holder_SelectionShape, 1, wxEXPAND);
	Units_Holder_AttributesSelection2->Add(Units_Holder_SelectionEffect, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesSelection2->Add(5, 5);
	Units_Holder_AttributesSelection2->Add(Units_Holder_EditorSelectionColour, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesSelection2->Add(5, 5);
	Units_Holder_AttributesSelection2->Add(Units_Holder_SelectionRadius, 2, wxEXPAND);
	Units_Holder_ResourceStorageHeader->Add(Units_Holder_ResourceStorage[0], 0, wxEXPAND);
	Units_Holder_ResourceStorageHeader->Add(5, 5);
	Units_Holder_ResourceStorageHeader->Add(Units_Holder_ResourceStorage[1], 0, wxEXPAND);
	Units_Holder_ResourceStorageHeader->Add(5, 5);
	Units_Holder_ResourceStorageHeader->Add(Units_Holder_ResourceStorage[2], 0, wxEXPAND);
	Units_Holder_Attributes2->Add(Units_Holder_Unknown7, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Attributes2->Add(5, 5);
	Units_Holder_Attributes2->Add(Units_Holder_Unitline, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesHotKey->Add(Units_Holder_HotKey, 0, wxEXPAND);
	Units_Holder_AttributesTracking->Add(Units_Holder_TrackingUnit, 1, wxEXPAND);
	Units_Holder_AttributesTracking->Add(5, 5);
	Units_Holder_AttributesTracking->Add(Units_Holder_TrackingUnitUsed, 1, wxEXPAND);
	Units_Holder_AttributesTracking->Add(5, 5);
	Units_Holder_AttributesTracking->Add(Units_Holder_TrackingUnitDensity, 1, wxEXPAND);
	Units_Holder_CostHeader->Add(Units_Holder_CostType, 0, wxEXPAND);
	Units_Holder_CostHeader->Add(5, 5);
	Units_Holder_CostHeader->Add(Units_Holder_CostAmount, 0, wxEXPAND);
	Units_Holder_CostHeader->Add(5, 5);
	Units_Holder_CostHeader->Add(Units_Holder_CostUsed, 0, wxEXPAND);
	Units_Holder_AttributesTrain1->Add(Units_Holder_TrainTime, 2, wxEXPAND);
	Units_Holder_AttributesTrain1->Add(5, 5);
	Units_Holder_AttributesTrain1->Add(Units_Holder_TrainLocationID, 3, wxEXPAND);
	Units_Holder_AttributesTrain1->Add(5, 5);
	Units_Holder_AttributesTrain1->Add(Units_Holder_CommandAttribute, 2, wxEXPAND);
	Units_Holder_AttributesTrain2->Add(Units_Holder_MinTechLevel, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesTrain2->Add(5, 5);
	Units_Holder_AttributesTrain2->Add(Units_Holder_ButtonID, 1, wxEXPAND);
	Units_Holder_Attributes3->Add(Units_Holder_StackUnitID, 1, wxEXPAND);
	Units_Holder_Attributes3->Add(5, 5);
	Units_Holder_Attributes3->Add(Units_Holder_TerrainID, 1, wxEXPAND);
	Units_Holder_Attributes3->Add(5, 5);
	Units_Holder_Attributes3->Add(Units_Holder_ResearchID, 1, wxEXPAND);
	Units_Holder_AttributesAnnexUnit->Add(Units_Holder_AnnexUnit1, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_AttributesAnnexUnit2->Add(Units_Holder_AnnexUnitMisplacement1, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Attributes4->Add(Units_Holder_HeadUnit, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Attributes4->Add(5, 5);
	Units_Holder_Attributes4->Add(Units_Holder_TransformUnit, 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	
	Units_Holder_Attributes->Add(Units_Holder_AttributesBoxes1, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesBoxes2, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_Attributes1, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesTerrain, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesModes1, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesDropSite, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesSizes, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesSelection1, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesSelection2, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_ResourceStorageHeader, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_Attributes2, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesHotKey, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesTracking, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_CostHeader, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesTrain1, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesTrain2, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_Attributes3, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesAnnexUnit, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_AttributesAnnexUnit2, 0, wxEXPAND);
	Units_Holder_Attributes->Add(5, 5);
	Units_Holder_Attributes->Add(Units_Holder_Attributes4, 0, wxEXPAND);
	
	Units_Holder_TrainSound1->Add(Units_TrainSound[0], 1, wxEXPAND);
	Units_Holder_TrainSound1->Add(Units_ComboBox_TrainSound[0], 1, wxEXPAND);
	Units_Holder_TrainSound2->Add(Units_TrainSound[1], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_TrainSound2->Add(Units_ComboBox_TrainSound[1], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	
	Units_Holder_TrainSound3->Add(Units_Holder_TrainSound1, 1, wxEXPAND);
	Units_Holder_TrainSound3->Add(5, 5);
	Units_Holder_TrainSound3->Add(Units_Holder_TrainSound2, 1, wxEXPAND);
	
	Units_Holder_TrainSound->Add(Units_Text_TrainSound, 0, wxEXPAND);
	Units_Holder_TrainSound->Add(2, 2);
	Units_Holder_TrainSound->Add(Units_Holder_TrainSound3, 1, wxEXPAND);
	
	Units_Holder_SoundsArea1->Add(Units_Holder_ConstructionSound, 1, wxEXPAND);
	Units_Holder_SoundsArea1->Add(5, 5);
	Units_Holder_SoundsArea1->Add(Units_Holder_TrainSound, 2, wxEXPAND);
	Units_Holder_SoundsArea1->Add(5, 5);
	Units_Holder_SoundsArea1->Add(Units_Holder_SelectionSound, 1, wxEXPAND);
	Units_Holder_SoundsArea2->Add(Units_Holder_AttackSound, 1, wxEXPAND);
	Units_Holder_SoundsArea2->Add(5, 5);
	Units_Holder_SoundsArea2->Add(Units_Holder_MoveSound, 1, wxEXPAND);
	Units_Holder_SoundsArea2->Add(5, 5);
	Units_Holder_SoundsArea2->Add(Units_Holder_StopSound, 1, wxEXPAND);
	Units_Holder_SoundsArea2->Add(5, 5);
	Units_Holder_SoundsArea2->Add(Units_Holder_DyingSound, 1, wxEXPAND);
	
	Units_Holder_SoundsArea->Add(Units_Holder_SoundsArea1, 0, wxEXPAND);
	Units_Holder_SoundsArea->Add(5, 5);
	Units_Holder_SoundsArea->Add(Units_Holder_SoundsArea2, 0, wxEXPAND);
	
	Units_Holder_MiscArea->Add(Units_Holder_HPBarHeight1, 1, wxEXPAND);
	Units_Holder_MiscArea->Add(5, 5);
	Units_Holder_MiscArea->Add(Units_Holder_HPBarHeight2, 1, wxEXPAND);
	
	Units_Holder_Type10plusUnknowns1->Add(Units_Holder_Unknown2, 0, wxEXPAND);
	Units_Holder_Type10plusUnknowns1->Add(5, -1);
	Units_Holder_Type10plusUnknowns1->Add(Units_Holder_Unknown9, 0, wxEXPAND);

	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown1, 0, wxEXPAND);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown3, 0, wxEXPAND);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown3a, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown4, 0, wxEXPAND);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown5, 0, wxEXPAND);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown6, 0, wxEXPAND);
	Units_Grid_Type10plusUnknowns->Add(Units_Holder_Unknown8, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type10plusUnknownArea->Add(Units_Holder_Type10plusUnknowns1, 0, wxEXPAND);
	Units_Holder_Type10plusUnknownArea->Add(-1, 5);
	Units_Holder_Type10plusUnknownArea->Add(Units_Grid_Type10plusUnknowns, 0, wxEXPAND);

	Units_Grid_Type30plusUnknownArea->Add(Units_Holder_Unknown11, 0, wxEXPAND);
	Units_Grid_Type30plusUnknownArea->Add(Units_Holder_Unknown12, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type30plusUnknownArea->Add(Units_Holder_Unknown16, 0, wxEXPAND);
	Units_Holder_Type30plusUnknownArea->Add(-1, 5);
	Units_Holder_Type30plusUnknownArea->Add(Units_Grid_Type30plusUnknownArea, 0, wxEXPAND);

	Units_Holder_Type40plusUnknownArea->Add(Units_Holder_Unknown19, 0, wxEXPAND);

	Units_Grid_Type60plusUnknownArea->Add(Units_Holder_Unknown21, 0, wxEXPAND);
	Units_Grid_Type60plusUnknownArea->Add(Units_Holder_Unknown22, 0, wxEXPAND);
	Units_Grid_Type60plusUnknownArea->Add(Units_Holder_Unknown23, 0, wxEXPAND);
	Units_Holder_Type60plusUnknownArea->Add(Units_Holder_Unknown20, 0, wxEXPAND);
	Units_Holder_Type60plusUnknownArea->Add(-1, 5);
	Units_Holder_Type60plusUnknownArea->Add(Units_Grid_Type60plusUnknownArea, 0, wxEXPAND);

	Units_Grid_Type70plusUnknownArea->Add(Units_Holder_Unknown26, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type70plusUnknownArea->Add(Units_Holder_Unknown28, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type70plusUnknownArea->Add(Units_Holder_Unknown29, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type70plusUnknownArea->Add(Units_Holder_Unknown27, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type70plusUnknownArea->Add(-1, 5);
	Units_Holder_Type70plusUnknownArea->Add(Units_Grid_Type70plusUnknownArea, 0, wxEXPAND);

	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown30, 0, wxEXPAND);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown31, 0, wxEXPAND);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown32, 0, wxEXPAND);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown33, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown34, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown35, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Grid_Type80plusUnknownArea->Add(Units_Holder_Unknown36, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type80plusUnknownArea->Add(Units_Holder_Unknown37, 0, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	Units_Holder_Type80plusUnknownArea->Add(-1, 5);
	Units_Holder_Type80plusUnknownArea->Add(Units_Grid_Type80plusUnknownArea, 0, wxEXPAND);
	
//	Units_UnitHeads_Buttons->Add(Units_UnitHeads_Copy, 1, wxEXPAND);
//	Units_UnitHeads_Buttons->Add(Units_UnitHeads_Paste, 1, wxEXPAND);
	
//	Units_UnitHeads->Add(Units_UnitHeads_Search, 0, wxEXPAND);
//	Units_UnitHeads->Add(-1, 2);
	Units_UnitHeads->Add(Units_UnitHeads_List, 1, wxEXPAND);
//	Units_UnitHeads->Add(-1, 2);
//	Units_UnitHeads->Add(Units_UnitHeads_Buttons, 0, wxEXPAND);
	
	Units_UnitCommands_Buttons->Add(Units_UnitCommands_Add, 1, wxEXPAND);
	Units_UnitCommands_Buttons->Add(Units_UnitCommands_Delete, 1, wxEXPAND);
	Units_UnitCommands_Buttons->Add(Units_UnitCommands_Copy, 1, wxEXPAND);
	Units_UnitCommands_Buttons->Add(Units_UnitCommands_Paste, 1, wxEXPAND);
	
	Units_UnitCommands->Add(Units_UnitCommands_Search, 0, wxEXPAND);
	Units_UnitCommands->Add(Units_UnitCommands_Search_R, 0, wxEXPAND);
	Units_UnitCommands->Add(-1, 2);
	Units_UnitCommands->Add(Units_UnitCommands_List, 1, wxEXPAND);
	Units_UnitCommands->Add(-1, 2);
	Units_UnitCommands->Add(Units_UnitCommands_Buttons, 0, wxEXPAND);
	
	Units_CommandHolder_Lists->Add(Units_UnitHeads_Name, 0, wxEXPAND);
	Units_CommandHolder_Lists->Add(Units_UnitHeads, 0, wxEXPAND);
	Units_CommandHolder_Lists->Add(5, 5);
	Units_CommandHolder_Lists->Add(Units_UnitCommands, 0, wxEXPAND);
	Units_CommandHolder_Lists->Add(5, 5);
	Units_CommandHolder_Lists->Add(Units_Holder_Exists, 0, wxEXPAND);
	
	UnitCommands_Holder_1->Add(UnitCommands_Holder_ID, 1, wxEXPAND);
	UnitCommands_Holder_1->Add(5, 5);
	UnitCommands_Holder_1->Add(UnitCommands_Holder_One, 1, wxEXPAND);
	
	UnitCommands_Holder_Types->Add(UnitCommands_Holder_Type, 1, wxEXPAND);
	UnitCommands_Holder_Types->Add(5, -1);
	UnitCommands_Holder_Types->Add(UnitCommands_Holder_SubType, 1, wxEXPAND);
	UnitCommands_Holder_2->Add(UnitCommands_Holder_ClassID, 1, wxEXPAND);
	UnitCommands_Holder_2->Add(5, -1);
	UnitCommands_Holder_2->Add(UnitCommands_Holder_UnitID, 1, wxEXPAND);
	UnitCommands_Holder_Resources->Add(UnitCommands_Holder_ResourceIn, 1, wxEXPAND);
	UnitCommands_Holder_Resources->Add(5, -1);
	UnitCommands_Holder_Resources->Add(UnitCommands_Holder_ResourceOut, 1, wxEXPAND);
	
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_1, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(-1, 5);
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_Types, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(UnitCommands_ComboBox_Types, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(-1, 5);
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_2, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(-1, 5);
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_Resources, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(-1, 5);
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_ExecutionRadius, 0, wxEXPAND);
	Units_CommandHolder_Data1->Add(-1, 5);
	Units_CommandHolder_Data1->Add(UnitCommands_Holder_Graphics, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown1, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown2, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown3, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown4, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown5, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown6, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown7, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown8, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown9, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown10, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown11, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown12, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown13, 0, wxEXPAND);
	Units_CommandHolder_Data2->Add(-1, 5);
	Units_CommandHolder_Data2->Add(UnitCommands_Holder_Unknown14, 0, wxEXPAND);
	
	Units_CommandHolder_Data->Add(Units_CommandHolder_Data1, 3, wxEXPAND);
	Units_CommandHolder_Data->Add(5, -1);
	Units_CommandHolder_Data->Add(Units_CommandHolder_Data2, 2, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	
	Units_Holder_CommandsArea->Add(Units_CommandHolder_Lists, 1, wxEXPAND);
	Units_Holder_CommandsArea->Add(5, -1);
	Units_Holder_CommandsArea->Add(Units_CommandHolder_Data, 3, wxEXPAND);
	
	Units_Holder_UnknownArea->Add(Units_Holder_Type10plusUnknownArea, 0, wxEXPAND);
	Units_Holder_UnknownArea->Add(5, 5);
	Units_Holder_UnknownArea->Add(Units_Holder_Type30plusUnknownArea, 0, wxEXPAND);
	Units_Holder_UnknownArea->Add(5, 5);
	Units_Holder_UnknownArea->Add(Units_Holder_Type40plusUnknownArea, 0, wxEXPAND);
	Units_Holder_UnknownArea->Add(5, 5);
	Units_Holder_UnknownArea->Add(Units_Holder_Type60plusUnknownArea, 0, wxEXPAND);
	Units_Holder_UnknownArea->Add(5, 5);
	Units_Holder_UnknownArea->Add(Units_Holder_Type70plusUnknownArea, 0, wxEXPAND);
	Units_Holder_UnknownArea->Add(5, 5);
	Units_Holder_UnknownArea->Add(Units_Holder_Type80plusUnknownArea, 0, wxEXPAND);
	
	Units_ScrollerWindowsSpace->Add(Units_Holder_TypeArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_LangDLLArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_GraphicsArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_StatsArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_ProjectilesArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_Attributes, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_SoundsArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_MiscArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_UnknownArea, 0, wxEXPAND);
	Units_ScrollerWindowsSpace->Add(5, 5);
	Units_ScrollerWindowsSpace->Add(Units_Holder_CommandsArea, 0, wxEXPAND);
	
	Units_ScrollerWindows->Add(Units_ScrollerWindowsSpace, 1, wxEXPAND);
	Units_ScrollerWindows->Add(5, -1);
	
	Units_Scroller->SetSizer(Units_ScrollerWindows);
	Units_Scroller->SetScrollRate(0, 20);
	
	Units_Holder_TopRow->Add(Units_Holder_Type, 2, wxEXPAND);
	Units_Holder_TopRow->Add(15, 15);
	Units_Holder_TopRow->Add(Units_AutoCopyState, 2, wxEXPAND);

	Units_MainRow1->Add(Units_Holder_TopRow, 1);
	
	Units_DataArea->Add(-1, 10);
	Units_DataArea->Add(Units_MainRow1, 0, wxEXPAND);
	Units_DataArea->Add(-1, 5);
	Units_DataArea->Add(Units_Scroller, 1, wxEXPAND);
	Units_DataArea->Add(-1, 10);
	
	Units_Main->Add(10, -1);
	Units_Main->Add(Units_ListArea, 1, wxEXPAND);
	Units_Main->Add(10, -1);
	Units_Main->Add(Units_DataArea, 3, wxEXPAND);
	Units_Main->Add(10, -1);
	
//	Units_UnitHeads_Search->Enable(false);
	Units_UnitHeads_List->Enable(false);
//	Units_UnitHeads_Copy->Enable(false);
//	Units_UnitHeads_Paste->Enable(false);
	Units_UnitHeads->Show(false);
	if(AutoCopy == MenuOption_NoAuto)
	{
		Units_AutoCopyState->SetLabel("Auto-copy: Disabled");
	}
	else if(AutoCopy == MenuOption_Include)
	{
		Units_AutoCopyState->SetLabel("Auto-copy: Include graphics");
	}
	else if(AutoCopy == MenuOption_Exclude)
	{
		Units_AutoCopyState->SetLabel("Auto-copy: Exclude graphics");
	}
	if(SearchFilters == MenuOption_NoExtra)
	{
		for(short loop = 0;loop < 2;loop++)
		Units_Units_SearchFilters[loop]->Show(false);
		Units_Units_SearchFilters[0]->SetSelection(0);
	}
	else if(SearchFilters == MenuOption_1stFilters)
	{
		Units_Units_SearchFilters[0]->Show(true);
		Units_Units_SearchFilters[1]->Show(false);
		Units_Units_SearchFilters[1]->SetSelection(0);
	}
	else if(SearchFilters == MenuOption_2ndFilters)
	{
		for(short loop = 0;loop < 2;loop++)
		Units_Units_SearchFilters[loop]->Show(true);
	}
	Units_ID1->Enable(false);
	Units_ID2->Enable(false);
	Units_ID3->Enable(false);
	Units_Holder_NameLength->Show(false);
	Units_Holder_NameLength2->Show(false);
	UnitCommands_ID->Enable(false);
//	Units_Add->Enable(false);
//	Units_Delete->Enable(false);
	
	Tab_Units->SetSizer(Units_Main);
	
	Connect(Units_Units_Search->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitsSearch));
	Connect(Units_Units_Search_R->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitsSearch));
	for(short loop = 0;loop < 2;loop++)
	{
		Connect(Units_Units_UseAnd[loop]->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnSelection_CheckBoxes));
		Connect(Units_Units_SearchFilters[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnSelection_ComboBoxes));
	}
	Connect(Units_Civs_List->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitSubList));
	Connect(Units_Units_List->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitsSelect));
	Connect(Units_Add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitsAdd));
	Connect(Units_Delete->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitsDelete));
	Connect(Units_Copy->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitsCopy));
	Connect(Units_Paste->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitsPaste));
	Connect(Units_UnitCommands_List->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsSelect));
	Connect(Units_UnitCommands_Search->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsSearch));
	Connect(Units_UnitCommands_Search_R->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsSearch));
	Connect(Units_UnitCommands_Add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsAdd));
	Connect(Units_UnitCommands_Delete->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsDelete));
	Connect(Units_UnitCommands_Copy->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsCopy));
	Connect(Units_UnitCommands_Paste->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitCommandsPaste));
	Connect(Units_DamageGraphics_Search->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsSearch));
	Connect(Units_DamageGraphics_Search_R->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsSearch));
	Connect(Units_DamageGraphics_List->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsSelect));
	Connect(Units_DamageGraphics_Add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsAdd));
	Connect(Units_DamageGraphics_Delete->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsDelete));
	Connect(Units_DamageGraphics_Copy->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsCopy));
	Connect(Units_DamageGraphics_Paste->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitDamageGraphicsPaste));
	Connect(Units_Attacks_Search->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksSearch));
	Connect(Units_Attacks_Search_R->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksSearch));
	Connect(Units_Attacks_List->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksSelect));
	Connect(Units_Attacks_Add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksAdd));
	Connect(Units_Attacks_Delete->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksDelete));
	Connect(Units_Attacks_Copy->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksCopy));
	Connect(Units_Attacks_Paste->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitAttacksPaste));
	Connect(Units_Armors_Search->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsSearch));
	Connect(Units_Armors_Search_R->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsSearch));
	Connect(Units_Armors_List->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsSelect));
	Connect(Units_Armors_Add->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsAdd));
	Connect(Units_Armors_Delete->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsDelete));
	Connect(Units_Armors_Copy->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsCopy));
	Connect(Units_Armors_Paste->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AGE_Frame::OnUnitArmorsPaste));
	Connect(UnitCommands_ComboBox_Types->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnSelection_ComboBoxes));
	
//	Listing and Auto Copy

	Units_Type->Connect(Units_Type->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_TextControls), NULL, this);
	Connect(Units_ComboBox_Type->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnSelection_ComboBoxes));

	for(short loop = 0;loop < 2;loop++)
	{
		Units_Unknown2[loop]->Connect(Units_Unknown2[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
		Units_Unknown20[loop]->Connect(Units_Unknown20[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	}
	Units_DeathMode->Connect(Units_DeathMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_GarrisonCapacity->Connect(Units_GarrisonCapacity->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_PlacementMode->Connect(Units_PlacementMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_AirMode->Connect(Units_AirMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_HideInEditor->Connect(Units_HideInEditor->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_BuildingMode->Connect(Units_BuildingMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_VisibleInFog->Connect(Units_VisibleInFog->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_FlyMode->Connect(Units_FlyMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_InteractionMode->Connect(Units_InteractionMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_MinimapMode->Connect(Units_MinimapMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown3a->Connect(Units_Unknown3a->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown4->Connect(Units_Unknown4->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown5->Connect(Units_Unknown5->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown6->Connect(Units_Unknown6->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown7->Connect(Units_Unknown7->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown8->Connect(Units_Unknown8->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_SelectionMask->Connect(Units_SelectionMask->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_SelectionShapeType->Connect(Units_SelectionShapeType->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_SelectionShape->Connect(Units_SelectionShape->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	for(short loop = 0;loop < 4;loop++)
	Units_Unknown9[loop]->Connect(Units_Unknown9[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_SelectionEffect->Connect(Units_SelectionEffect->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_EditorSelectionColour->Connect(Units_EditorSelectionColour->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_MinTechLevel->Connect(Units_MinTechLevel->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown11->Connect(Units_Unknown11->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_TrackingUnitUsed->Connect(Units_TrackingUnitUsed->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	for(short loop = 0;loop < 17;loop++)
	Units_Unknown16[loop]->Connect(Units_Unknown16[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_VillagerMode->Connect(Units_VillagerMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown19->Connect(Units_Unknown19->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_AccuracyPercent->Connect(Units_AccuracyPercent->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown23->Connect(Units_Unknown23->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_StretchMode->Connect(Units_StretchMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_CompensationMode->Connect(Units_CompensationMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_DropAnimationMode->Connect(Units_DropAnimationMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_PenetrationMode->Connect(Units_PenetrationMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown24->Connect(Units_Unknown24->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_ButtonID->Connect(Units_ButtonID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown26->Connect(Units_Unknown26->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown28->Connect(Units_Unknown28->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_MissileGraphicDelay->Connect(Units_MissileGraphicDelay->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_HeroMode->Connect(Units_HeroMode->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	Units_AttackMissileDuplicationAmount2->Connect(Units_AttackMissileDuplicationAmount2->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown29->Connect(Units_Unknown29->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_Unknown33->Connect(Units_Unknown33->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	Units_GarrisonType->Connect(Units_GarrisonType->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxByte), NULL, this);
	for(short loop = 0;loop < 6;loop++)
	Units_Unknown37[loop]->Connect(Units_Unknown37[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);

	for(short loop = 0;loop < 2;loop++)
	{
		Units_StandingGraphic[loop]->Connect(Units_StandingGraphic[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_DyingGraphic[loop]->Connect(Units_DyingGraphic[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_TrainSound[loop]->Connect(Units_TrainSound[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_PlacementBypassTerrain[loop]->Connect(Units_PlacementBypassTerrain[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_PlacementTerrain[loop]->Connect(Units_PlacementTerrain[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_WalkingGraphic[loop]->Connect(Units_WalkingGraphic[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_DropSite[loop]->Connect(Units_DropSite[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_GarrisonGraphic[loop]->Connect(Units_GarrisonGraphic[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	}
	for(short loop = 0;loop < 3;loop++)
	{
		Units_CostType[loop]->Connect(Units_CostType[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		Units_CostAmount[loop]->Connect(Units_CostAmount[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
		Units_CostUsed[loop]->Connect(Units_CostUsed[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxShort), NULL, this);
		Units_Unknown27[loop]->Connect(Units_Unknown27[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	}
	for(short loop = 0;loop < 4;loop++)
	{
		Units_HotKey[loop]->Connect(Units_HotKey[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
		Units_AnnexUnit[loop]->Connect(Units_AnnexUnit[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	}
	Units_LanguageDllName->Connect(Units_LanguageDllName->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_LanguageDllCreation->Connect(Units_LanguageDllCreation->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_Class->Connect(Units_Class->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_HitPoints->Connect(Units_HitPoints->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_DeadUnitID->Connect(Units_DeadUnitID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_IconID->Connect(Units_IconID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_Unknown1->Connect(Units_Unknown1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_Enabled->Connect(Units_Enabled->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxShort), NULL, this);
	Units_TerrainRestriction->Connect(Units_TerrainRestriction->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_ResourceCapacity->Connect(Units_ResourceCapacity->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_CommandAttribute->Connect(Units_CommandAttribute->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_LanguageDllHelp->Connect(Units_LanguageDllHelp->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_SelectionSound->Connect(Units_SelectionSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_DyingSound->Connect(Units_DyingSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_AttackSound->Connect(Units_AttackSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unitline->Connect(Units_Unitline->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_TrackingUnit->Connect(Units_TrackingUnit->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_SheepConversion->Connect(Units_SheepConversion->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxShortUnitSheepConversion), NULL, this);
	Units_MoveSound->Connect(Units_MoveSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_StopSound->Connect(Units_StopSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown21->Connect(Units_Unknown21->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_ProjectileUnitID->Connect(Units_ProjectileUnitID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown22->Connect(Units_Unknown22->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_Delay->Connect(Units_Delay->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_AttackGraphic->Connect(Units_AttackGraphic->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_DisplayedMeleeArmour->Connect(Units_DisplayedMeleeArmour->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_DisplayedAttack->Connect(Units_DisplayedAttack->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_TrainTime->Connect(Units_TrainTime->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_TrainLocationID->Connect(Units_TrainLocationID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_DisplayedPierceArmour->Connect(Units_DisplayedPierceArmour->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_ConstructionGraphicID->Connect(Units_ConstructionGraphicID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_SnowGraphicID->Connect(Units_SnowGraphicID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown30->Connect(Units_Unknown30->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_Unknown31->Connect(Units_Unknown31->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_StackUnitID->Connect(Units_StackUnitID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_TerrainID->Connect(Units_TerrainID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown32->Connect(Units_Unknown32->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_ResearchID->Connect(Units_ResearchID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_HeadUnit->Connect(Units_HeadUnit->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_TransformUnit->Connect(Units_TransformUnit->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown34->Connect(Units_Unknown34->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Units_ConstructionSound->Connect(Units_ConstructionSound->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	Units_Unknown36->Connect(Units_Unknown36->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);

	for(short loop = 0;loop < 2;loop++)
	{
		Units_SizeRadius[loop]->Connect(Units_SizeRadius[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
		Units_EditorRadius[loop]->Connect(Units_EditorRadius[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
		Units_SelectionRadius[loop]->Connect(Units_SelectionRadius[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	}
	for(short loop = 0;loop < 3;loop++)
	{
		Units_GraphicDisplacement[loop]->Connect(Units_GraphicDisplacement[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
		Units_AttackMissileDuplicationUnknown[loop]->Connect(Units_AttackMissileDuplicationUnknown[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	}
	for(short loop = 0;loop < 4;loop++)
	{
		for(short loop2 = 0;loop2 < 2;loop2++)
		Units_AnnexUnitMisplacement[loop][loop2]->Connect(Units_AnnexUnitMisplacement[loop][loop2]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	}
	Units_LineOfSight->Connect(Units_LineOfSight->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_HPBarHeight1->Connect(Units_HPBarHeight1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_ResourceDecay->Connect(Units_ResourceDecay->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_Unknown3->Connect(Units_Unknown3->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_HPBarHeight2->Connect(Units_HPBarHeight2->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_Speed->Connect(Units_Speed->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_RotationSpeed->Connect(Units_RotationSpeed->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_TrackingUnitDensity->Connect(Units_TrackingUnitDensity->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_Unknown12->Connect(Units_Unknown12->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_SearchRadius->Connect(Units_SearchRadius->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_WorkRate->Connect(Units_WorkRate->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_MaxRange->Connect(Units_MaxRange->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_BlastRadius->Connect(Units_BlastRadius->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_ReloadTime1->Connect(Units_ReloadTime1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_MinRange->Connect(Units_MinRange->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_GarrisonRecoveryRate->Connect(Units_GarrisonRecoveryRate->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_DisplayedRange->Connect(Units_DisplayedRange->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_ReloadTime2->Connect(Units_ReloadTime2->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_Unknown25->Connect(Units_Unknown25->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_AttackMissileDuplicationAmount1->Connect(Units_AttackMissileDuplicationAmount1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	Units_GarrisonHealRate->Connect(Units_GarrisonHealRate->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);

	Units_Unselectable->Connect(Units_Unselectable->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_CheckBoxByte), NULL, this);
	
	Units_AttackMissileDuplicationUnit->Connect(Units_AttackMissileDuplicationUnit->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxLong), NULL, this);
	Units_AttackMissileDuplicationGraphic->Connect(Units_AttackMissileDuplicationGraphic->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxLong), NULL, this);
	Units_Unknown35->Connect(Units_Unknown35->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Long), NULL, this);

	Units_Name->Connect(Units_Name->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_String), NULL, this);
	Units_Name2->Connect(Units_Name2->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_String), NULL, this);

	for(short loop = 0;loop < 3;loop++)
	{
		ResourceStorage_Type[loop]->Connect(ResourceStorage_Type[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
		ResourceStorage_Amount[loop]->Connect(ResourceStorage_Amount[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
		ResourceStorage_Enabled[loop]->Connect(ResourceStorage_Enabled[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	}

	DamageGraphics_GraphicID->Connect(DamageGraphics_GraphicID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	DamageGraphics_DamagePercent->Connect(DamageGraphics_DamagePercent->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	DamageGraphics_Unknown1->Connect(DamageGraphics_Unknown1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	
	Attacks_Class->Connect(Attacks_Class->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShortAttackType), NULL, this);
	Attacks_Amount->Connect(Attacks_Amount->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	Armors_Class->Connect(Armors_Class->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShortAttackType), NULL, this);
	Armors_Amount->Connect(Armors_Amount->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);

	UnitCommands_One->Connect(UnitCommands_One->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_Type->Connect(UnitCommands_Type->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_SubType->Connect(UnitCommands_SubType->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_ClassID->Connect(UnitCommands_ClassID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_UnitID->Connect(UnitCommands_UnitID->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_Unknown2->Connect(UnitCommands_Unknown2->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_ResourceIn->Connect(UnitCommands_ResourceIn->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_ResourceOut->Connect(UnitCommands_ResourceOut->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_Unknown3->Connect(UnitCommands_Unknown3->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_Unknown12->Connect(UnitCommands_Unknown12->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	UnitCommands_Unknown13->Connect(UnitCommands_Unknown13->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Short), NULL, this);
	for(short loop = 0;loop < 6;loop++)
	UnitCommands_Graphics[loop]->Connect(UnitCommands_Graphics[loop]->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_ComboBoxShort), NULL, this);

	UnitCommands_Unknown4->Connect(UnitCommands_Unknown4->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	UnitCommands_ExecutionRadius->Connect(UnitCommands_ExecutionRadius->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	UnitCommands_Unknown5->Connect(UnitCommands_Unknown5->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	UnitCommands_Unknown7->Connect(UnitCommands_Unknown7->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Float), NULL, this);
	
	UnitCommands_Unknown1->Connect(UnitCommands_Unknown1->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown6->Connect(UnitCommands_Unknown6->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown8->Connect(UnitCommands_Unknown8->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown9->Connect(UnitCommands_Unknown9->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown10->Connect(UnitCommands_Unknown10->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown11->Connect(UnitCommands_Unknown11->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);
	UnitCommands_Unknown14->Connect(UnitCommands_Unknown14->GetId(), wxEVT_KILL_FOCUS, wxFocusEventHandler(AGE_Frame::OnKillFocus_AutoCopy_Byte), NULL, this);

//	Combo/Check Boxes updating AutoCopy linkings

	for(short loop = 0;loop < 2;loop++)
	{
		Units_ComboBox_StandingGraphic[loop]->Connect(Units_ComboBox_StandingGraphic[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_DyingGraphic[loop]->Connect(Units_ComboBox_DyingGraphic[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_TrainSound[loop]->Connect(Units_ComboBox_TrainSound[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_PlacementBypassTerrain[loop]->Connect(Units_ComboBox_PlacementBypassTerrain[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_PlacementTerrain[loop]->Connect(Units_ComboBox_PlacementTerrain[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_WalkingGraphic[loop]->Connect(Units_ComboBox_WalkingGraphic[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_DropSite[loop]->Connect(Units_ComboBox_DropSite[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_ComboBox_GarrisonGraphic[loop]->Connect(Units_ComboBox_GarrisonGraphic[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	}
	for(short loop = 0;loop < 3;loop++)
	{
		Units_ComboBox_CostType[loop]->Connect(Units_ComboBox_CostType[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		ResourceStorage_ComboBox_Type[loop]->Connect(ResourceStorage_ComboBox_Type[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
		Units_CheckBox_CostUsed[loop]->Connect(Units_CheckBox_CostUsed[loop]->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxShort), NULL, this);
	}
	for(short loop = 0;loop < 4;loop++)
	{
		Units_ComboBox_AnnexUnit[loop]->Connect(Units_ComboBox_AnnexUnit[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	}
	for(short loop = 0;loop < 6;loop++)
	{
		UnitCommands_ComboBox_Graphics[loop]->Connect(UnitCommands_ComboBox_Graphics[loop]->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	}
	Units_ComboBox_GarrisonType->Connect(Units_ComboBox_GarrisonType->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxByte), NULL, this);
	Units_ComboBox_Class->Connect(Units_ComboBox_Class->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_DeadUnitID->Connect(Units_ComboBox_DeadUnitID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_TerrainRestriction->Connect(Units_ComboBox_TerrainRestriction->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_SelectionSound->Connect(Units_ComboBox_SelectionSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_DyingSound->Connect(Units_ComboBox_DyingSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_AttackSound->Connect(Units_ComboBox_AttackSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_TrackingUnit->Connect(Units_ComboBox_TrackingUnit->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_MoveSound->Connect(Units_ComboBox_MoveSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_StopSound->Connect(Units_ComboBox_StopSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_ProjectileUnitID->Connect(Units_ComboBox_ProjectileUnitID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_AttackGraphic->Connect(Units_ComboBox_AttackGraphic->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_TrainLocationID->Connect(Units_ComboBox_TrainLocationID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_ConstructionGraphicID->Connect(Units_ComboBox_ConstructionGraphicID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_SnowGraphicID->Connect(Units_ComboBox_SnowGraphicID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_StackUnitID->Connect(Units_ComboBox_StackUnitID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_TerrainID->Connect(Units_ComboBox_TerrainID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_ResearchID->Connect(Units_ComboBox_ResearchID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_HeadUnit->Connect(Units_ComboBox_HeadUnit->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_TransformUnit->Connect(Units_ComboBox_TransformUnit->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_ConstructionSound->Connect(Units_ComboBox_ConstructionSound->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Units_ComboBox_AttackMissileDuplicationUnit->Connect(Units_ComboBox_AttackMissileDuplicationUnit->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxLong), NULL, this);
	Units_ComboBox_AttackMissileDuplicationGraphic->Connect(Units_ComboBox_AttackMissileDuplicationGraphic->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxLong), NULL, this);
	DamageGraphics_ComboBox_GraphicID->Connect(DamageGraphics_ComboBox_GraphicID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	Attacks_ComboBox_Class->Connect(Attacks_ComboBox_Class->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShortAttackType), NULL, this);
	Armors_ComboBox_Class->Connect(Armors_ComboBox_Class->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShortAttackType), NULL, this);
	UnitCommands_ComboBox_ClassID->Connect(UnitCommands_ComboBox_ClassID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_ComboBox_UnitID->Connect(UnitCommands_ComboBox_UnitID->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_ComboBox_ResourceIn->Connect(UnitCommands_ComboBox_ResourceIn->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);
	UnitCommands_ComboBox_ResourceOut->Connect(UnitCommands_ComboBox_ResourceOut->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_ComboBoxShort), NULL, this);

	Units_CheckBox_DeathMode->Connect(Units_CheckBox_DeathMode->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_AirMode->Connect(Units_CheckBox_AirMode->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_HideInEditor->Connect(Units_CheckBox_HideInEditor->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_VisibleInFog->Connect(Units_CheckBox_VisibleInFog->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_FlyMode->Connect(Units_CheckBox_FlyMode->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_TrackingUnitUsed->Connect(Units_CheckBox_TrackingUnitUsed->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_HeroMode->Connect(Units_CheckBox_HeroMode->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);
	Units_CheckBox_Enabled->Connect(Units_CheckBox_Enabled->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxShort), NULL, this);
	Units_CheckBox_SheepConversion->Connect(Units_CheckBox_SheepConversion->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxShortUnitSheepConversion), NULL, this);
	Units_CheckBox_Unselectable->Connect(Units_CheckBox_Unselectable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AGE_Frame::OnUpdate_AutoCopy_CheckBoxByte), NULL, this);

}