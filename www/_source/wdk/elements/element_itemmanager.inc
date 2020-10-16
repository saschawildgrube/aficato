<?php

	class CItemManagerData
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();	
			$this->m_arrayData['columns'] = array();
			
		}
		
		function GetDataArray()
		{
			return $this->m_arrayData;	
		}
		
		function GetData($strData1,$strData2='',$strData3='',$strData4='')
		{
			return ArrayGetValue($this->m_arrayData,$strData1,$strData2,$strData3,$strData4);	
		}
		
		
		
		function AddColumn(
			$strColumnID,
			$strColumnText,
			$strSortOption = '',
			$bCombined = false,
			$strTab = '',
			$strCellStyle = 'left')
		{
			$arrayColumn = array();
			array_push($this->m_arrayData['columns'],
				array(
					'id' => $strColumnID,
					'text' => $strColumnText,
					'sortoption' => $strSortOption,
					'combined' => $bCombined,
					'tab' => $strTab,
					'cellstyle' => $strCellStyle));
		}
		
		function SetFilterOptionsForm($formDef,$strFormStyle,$strFormID)
		{
			$this->m_arrayData['filteroptionsformdef'] = $formDef;
			$this->m_arrayData['filteroptionsformstyle'] = $strFormStyle;
			$this->m_arrayData['filteroptionsformid'] = $strFormID;
		}

		function SetFilterOptionsTabNavigation(
			$strCurrentTab,
			$strTabParam,
			$arrayTabs)
		{
			$this->m_arrayData['filteroptionscurrenttab'] = $strCurrentTab;
			$this->m_arrayData['filteroptionstabparam'] = $strTabParam;
			$this->m_arrayData['filteroptionstabs'] = $arrayTabs;
		}


		function SetColumnsTabNavigation(
			$strCurrentTab,
			$strTabParam,
			$arrayTabs)
		{
			$this->m_arrayData['columnscurrenttab'] = $strCurrentTab;
			$this->m_arrayData['columnstabparam'] = $strTabParam;
			$this->m_arrayData['columnstabs'] = $arrayTabs;
		}

		function SetPageNavigation(
			$nCurrentPage,
			$nPageCount,
			$strOffsetParam,
			$nPageSize = 0)
		{
			$this->m_arrayData['currentpage'] = $nCurrentPage;
			$this->m_arrayData['pagecount'] = $nPageCount;
			$this->m_arrayData['offsetparam'] = $strOffsetParam;
			$this->m_arrayData['pagesize'] = $nPageSize;	
		}
			
		function SetItemList($arrayItemList = array())
		{
			$this->m_arrayData['itemlist'] = $arrayItemList;
		}
		
		function SetSortOptions(
			$strSortOptionParam,
			$strSortOption,
			$strSortOrderParam,
			$strSortOrder)
		{
			$this->m_arrayData['sortoptionparam'] = $strSortOptionParam;
			$this->m_arrayData['sortoption'] = $strSortOption;
			$this->m_arrayData['sortorderparam'] = $strSortOrderParam;
			$this->m_arrayData['sortorder'] = $strSortOrder;
		}
		
		function SetHideTableHeader($bHide)
		{
			$this->m_arrayData['hidetableheader'] = $bHide;
		}
			

		
		function IsValid()
		{
			/*
			if (ArrayGetValue($this->m_arrayData,'url') == '')
			{
				return false;	
			}
			$arrayParams = ArrayGetValue($this->m_arrayData,'params');
			if (!is_array($arrayParams))
			{
				return false;	
			}
			$arrayControls = ArrayGetValue($this->m_arrayData,'controls');
			if (!is_array($arrayControls))
			{
				return false;	
			}
			*/
			return true;
		}
		
	}
	
	class CItemManagerElement extends CElement
	{
		private $m_sortoption;
		
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			$this->m_sortoption == null;
			parent::__construct(
				$website,
				'itemmanager',
				$strStyleID,
				false,
				true);
				
		}
		
		function RenderItemManager(&$itemManagerData)
		{
			$this->Trace('RenderItemManager');
			
			$this->IncludeElement('tabnavigation');
			$this->IncludeElement('sortoption');
			$this->IncludeElement('pagenavigation');
			
			
			if (!is_object($itemManagerData))
			{
				$this->Error('itemManagerData is not an object.');	
			}
			
			if ($itemManagerData->IsValid() != true)
			{
				$this->Error('itemManagerData->IsValid != true.');	
			}
			
			$strOutput = $this->GetLayoutHTML(); 



			// FILTER OPTIONS
			$formDefFilter = $itemManagerData->GetData('filteroptionsformdef');
			if (is_object($formDefFilter))
			{
				$strFilterFormStyle = $itemManagerData->GetData('filteroptionsformstyle');
				$strFilterFormID = $itemManagerData->GetData('filteroptionsformid');
				if ($strFilterFormID == '')
				{
					$formFilter = new CFormElement($this->GetWebsite(),$strFilterFormStyle);
					$arrayItems['FORM_FILTEROPTIONS'] = $formFilter->RenderForm(
						$formDefFilter,
						array());
				}
				else 
				{
					$formFilter = new CFormTemplateElement($this->GetWebsite(),$strFilterFormStyle);
					$arrayItems['FORM_FILTEROPTIONS'] = $formFilter->RenderFormTemplate(
						$formDefFilter,
						array(),
						array(),
						$strFilterFormID);
				}
			}
			else
			{
				$arrayItems['FORM_FILTEROPTIONS'] = '';
			}			







			// TABNAVIGATION FILTER OPTIONS
			$bUseTabNav = false;
			$strCurrentTab = '';
			$arrayTabs = $itemManagerData->GetData('filteroptionstabs');
			if (is_array($arrayTabs))
			{
				if (ArrayCount($arrayTabs) > 0)
				{
					$bUseTabNav = true;	 
				}
			}

						
			if ($bUseTabNav == true)
			{
				$tabnavigationDef = new CTabNavigationDefinition();
				$strCurrentTab = $itemManagerData->GetData('filteroptionscurrenttab');
				foreach ($arrayTabs as $strTabID => $strTabText)
				{
					$arrayParams = array();
					$arrayParams[$itemManagerData->GetData('filteroptionstabparam')] = $strTabID;
					$tabnavigationDef->AddTab(
						$strTabID,
						$strTabText,
						$this->GetWebsite()->MakeURL($arrayParams));
					if ($strCurrentTab == '')
					{
						$strCurrentTab = $strTabID;
					}
				}
				$tabnav = new CTabNavigationElement($this->GetWebsite(),'default');
				
				$arrayItems['TABNAVIGATION_FILTEROPTIONS'] = $tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strCurrentTab);
			}
			else
			{
				$arrayItems['TABNAVIGATION_FILTEROPTIONS'] = '';
			}








			// TABNAVIGATION COLUMNS
			$bUseTabNav = false;
			$strCurrentTab = '';
			$arrayTabs = $itemManagerData->GetData('columnstabs');
			if (is_array($arrayTabs))
			{
				if (ArrayCount($arrayTabs) > 0)
				{
					$bUseTabNav = true;	 
				}
			}
			$tabnavigationDef = new CTabNavigationDefinition();
			if ($bUseTabNav == true)
			{
				$bUseTabNav = false;
				$arrayColumns = $itemManagerData->GetData('columns');
				$arrayColumnIDs = array();
				foreach ($arrayColumns as $column)
				{
					$arrayColumnTabIDs[] = $column['tab'];
				}
				$strCurrentTab = $itemManagerData->GetData('columnscurrenttab');
				if (!ArrayValueExists($arrayColumnTabIDs,$strCurrentTab))
				{
					$strCurrentTab = '';
				}
				foreach ($arrayTabs as $strTabID => $strTabText)
				{
					if (ArrayValueExists($arrayColumnTabIDs,$strTabID))
					{					
						$arrayParams = array();
						$arrayParams[$itemManagerData->GetData('columnstabparam')] = $strTabID;
						$tabnavigationDef->AddTab(
							$strTabID,
							$strTabText,
							$this->GetWebsite()->MakeURL($arrayParams));
						if ($strCurrentTab == '')
						{
							$strCurrentTab = $strTabID;
						}
						$bUseTabNav = true;						
					}
				}
			}
			if ($bUseTabNav == true)
			{			
				$tabnav = new CTabNavigationElement($this->GetWebsite(),'default');
				
				$arrayItems['TABNAVIGATION_COLUMNS'] = $tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strCurrentTab);
			}
			else
			{
				$arrayItems['TABNAVIGATION_COLUMNS'] = '';
			}

			// DATA TABLE
			$arrayTableData = array();
			$arrayTableHeaders = array();
			$arrayTableCellStyle = array();
			
			$arrayColumns = $itemManagerData->GetData('columns');
			
			// Header Row
			$bHideTableHeader = $itemManagerData->GetData('hidetableheader');
			if ($bHideTableHeader == false)
			{
				$arrayHeaderRow = array();
				$arrayTableHeadersRow = array();
	
				$strCurrentSortOption = $itemManagerData->GetData('sortoption');
				$strSortOptionParam = $itemManagerData->GetData('sortoptionparam');
				$strSortOrderParam = $itemManagerData->GetData('sortorderparam');
					
				$strCurrentColumn = '';
				foreach ($arrayColumns as $column)
				{
					$strColumnID = $column['id'];
					$strColumnText = $column['text'];
					$bCombined = $column['combined'];
					$strSortOption = $column['sortoption'];
					$strColumnTab = $column['tab'];
					
					if ($strColumnTab == '' || $strColumnTab == $strCurrentTab)
					{
						if ($strSortOption == $strCurrentSortOption)
						{
							$strCurrentSortOrder = $itemManagerData->GetData('sortorder');
						}
						else
						{
							$strCurrentSortOrder = '';
						}
						$strCell = $this->CallbackRenderHeaderCell(
							$strColumnID,
							$strColumnText,
							$strSortOptionParam,
							$strSortOption,
							$strSortOrderParam,
							$strCurrentSortOrder);
						if ($strCell == '')
						{
							$strCell = '&nbsp;';
						}
	
						$strCurrentColumn .= $strCell;
										
						if ($bCombined == false)
						{
							array_push($arrayHeaderRow,$strCurrentColumn);
							array_push($arrayTableHeadersRow,true);
							$strCurrentColumn = '';
						}
						else
						{
							$strCurrentColumn .= '<br/>';
						}
					}
				}
				array_push($arrayTableData,$arrayHeaderRow);
				array_push($arrayTableHeaders,$arrayTableHeadersRow);
				array_push($arrayTableCellStyle,array());
			}
				
			// Data rows
			$strCurrentColumn = '';
			$arrayItemList = $itemManagerData->GetData('itemlist');
			foreach ($arrayItemList as $arrayItem)
			{
				$arrayRow = array();
				$arrayRowCellStyle = array();
				foreach ($arrayColumns as $column)
				{
					$strColumnID = $column['id'];
					$bCombined = $column['combined'];
					$strColumnTab = $column['tab'];
					$strCellStyle = $column['cellstyle'];
					
					if ($strColumnTab == '' || $strColumnTab == $strCurrentTab)
					{
						$strCell = $this->CallbackRenderDataCell(
							$strColumnID,
							$arrayItem);
						if ($strCell == '')
						{
							$strCell = '&nbsp;';
						}

						$strCurrentColumn .= $strCell;
									
						if ($bCombined == false)
						{
							array_push($arrayRow,$strCurrentColumn);
							array_push($arrayRowCellStyle,$strCellStyle);
							$strCurrentColumn = '';
						}
						else
						{
							$strCurrentColumn .= '<br/>';
						}
					}
				}
				array_push($arrayTableData,$arrayRow);
				array_push($arrayTableCellStyle,$arrayRowCellStyle);
			}
			
			// Render table
			$table = new CTableElement($this->GetWebsite(),'default');
			$arrayItems['TABLE'] = $table->RenderTableEx($arrayTableData,$arrayTableHeaders,$arrayTableCellStyle);
		



			
			// PAGENAVIGATION			
			$pagenav = new CPageNavigationElement($this->GetWebsite(),'default');
			
			$arrayItems['PAGENAVIGATION'] = $pagenav->RenderPageNavigation(
				$itemManagerData->GetData('currentpage'),
				$itemManagerData->GetData('pagecount'),
				$itemManagerData->GetData('offsetparam'),
				$itemManagerData->GetData('pagesize'));

			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
		
		
		function CallbackRenderHeaderCell(
			$strColumnID,
			$strColumnText,
			$strSortOptionParam,
			$strSortOption,
			$strSortOrderParam,
			$strSortOrder)
		{
			$strCell = '';
			if ($strSortOption != '')
			{
				if ($this->m_sortoption == null)
				{
					$this->m_sortoption = new CSortOptionElement($this->GetWebsite(),'default');
				}
				$strCell .= $this->m_sortoption->RenderSortOption(
					$strSortOptionParam,
					$strSortOption,
					$strSortOrderParam,
					$strSortOrder,
					'?TID_TOOLTIP_SORTASC?',
					'?TID_TOOLTIP_SORTDESC?');
			}
			
			if ($strColumnText == '')
			{
				$strColumnText = '?TID_TABLEHEADER_'.StringUpperCase($strColumnID).'?';
			}
			$strCell .= $strColumnText;
			return $strCell;
		}

		function CallbackRenderDataCell(
			$strColumnID,
			$arrayItem)
		{
			$strCell = ArrayGetValue($arrayItem,$strColumnID);
			return $strCell;
		}


	}





