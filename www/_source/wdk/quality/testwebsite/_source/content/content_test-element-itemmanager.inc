<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->IncludeElement("itemmanager");
			

			$itemmanagerdata = new CItemManagerData();
			
			$strFilterOptionTab = $this->GetWebsite()->GetVar("itemmanager_filteroptions_tab");
			if ($strFilterOptionTab == "")
			{
				$strFilterOptionTab = "all";	
			}
			
			$arrayTable = array();
			for ($nRowIndex = 0; $nRowIndex < 10; $nRowIndex++)
			{
				if (	
						($strFilterOptionTab == "all")
					|| (($strFilterOptionTab == "even") && (($nRowIndex % 2) == 0))
					|| (($strFilterOptionTab == "uneven") && (($nRowIndex % 2) != 0))
					)
				{
					$arrayRow = array();
					for ($nColIndex = 1; $nColIndex <= 10; $nColIndex++)
					{
						$arrayRow["COL$nColIndex"] = "Row$nRowIndex Col$nColIndex";
					}
					$arrayTable[$nRowIndex] = $arrayRow;
				}
			}
			$itemmanagerdata->SetItemList($arrayTable);
			
			$arrayFilterOptionTabs = array(
				"all" => "All",
				"even" => "Even",
				"uneven" => "Uneven"
				);
			$itemmanagerdata->SetFilterOptionsTabNavigation(
				$strFilterOptionTab,
				"itemmanager_filteroptions_tab",
				$arrayFilterOptionTabs);
			
			
			
			$itemmanagerdata->AddColumn("COL1","Column 1","col1",true);
			$itemmanagerdata->AddColumn("COL2","Column 2","col2");
			$itemmanagerdata->AddColumn("COL3","Column 3","");
			$itemmanagerdata->AddColumn("COL4","Column 4","",true,"primary");
			$itemmanagerdata->AddColumn("COL5","Column 5","",false,"primary");
			$itemmanagerdata->AddColumn("COL6","Column 6","",false,"secondary");
			$itemmanagerdata->AddColumn("COL7","Column 7","",false,"secondary");
			$itemmanagerdata->AddColumn("COL8","Column 8","",true,"tertiary");
			$itemmanagerdata->AddColumn("COL9","Column 9","",true,"tertiary");
			$itemmanagerdata->AddColumn("COL10","Column 10","col10",false,"tertiary");
			
			$arrayColumnTabs = array(
				"primary" => "Primary",
				"secondary" => "Secondary",
				"tertiary" => "Tertiary",
				"unused" => "Unused");
			 
			$itemmanagerdata->SetColumnsTabNavigation(
				$this->GetWebsite()->GetVar("itemmanager_columns_tab"),
				"itemmanager_columns_tab",
				$arrayColumnTabs);
			
			$itemmanagerdata->SetSortOptions(
				"itemmanager_sort_option",
				$this->GetVar("itemmanager_sort_option"),
				"itemmanager_sort_order",
				$this->GetVar("itemmanager_sort_order")
				);
			
				
			
			$itemmanagerdata->SetPageNavigation(
				$this->GetWebsite()->GetVar("itemmanager_offset"),
				10,
				"itemmanager_offset",
				20);
			
			
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			
			$arrayItems["ELEMENT_ITEMMANAGER"] = $itemmanager->RenderItemManager($itemmanagerdata);


		}
	}
	
