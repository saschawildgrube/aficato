<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CStaticFileBrowserModule extends CModule
	{
		
		private $m_arrayErrors;
		private $m_strRootDir = "";
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/staticfilebrowser",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			if (!IsDirectory($strRootDir))
			{
				$this->SetErrorMessageCode("CONFIG_ROOTDIR_INVALID");
				return false;	
			}
			$arrayToolbarConfig = array(
				'browse' => array('refresh'),
				);
			$this->SetToolbarConfig($arrayToolbarConfig);
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case 'download':
					return $this->StateDownload();
				case 'refresh':
				case '':
					$this->SetState('browse');
				default:
					return $this->StateBrowse();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();

			$strCurrentDir = $this->GetModuleVar("currentdir");
			if ($strCurrentDir != "")
			{
				$strParentDir = GetParentDirectoryFromPath($strCurrentDir);
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "browse";
				$arrayParams[$this->GetModuleVarName("currentdir")] = $strParentDir;
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "level-up", "?TID_STATICFILEBROWSER_TOOL_PARENTDIR?");

				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "browse";
				$arrayParams[$this->GetModuleVarName("currentdir")] = "";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "level-up", "?TID_STATICFILEBROWSER_TOOL_ROOTDIR?");
			}
			
			
			if ($this->IsToolVisible('refresh'))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = 'browse';
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, 'refresh', '?TID_STATICFILEBROWSER_TOOL_REFRESH?');
			}

			if ($toolbarDef->GetToolCount() == 0)
			{
				$this->SetOutputItem("TOOLBAR",""); 
				return;
			}
			$toolbar = new CToolbarElement($this->GetWebsite(),'default');
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function CallbackIsToolVisible($strToolID)
		{
			if ($strToolID == 'refresh')
			{
				if ($this->GetModuleConfig('hiderefresh') == true)
				{
					return false;	
				}	
			}
			return parent::CallbackIsToolVisible($strToolID);
		}
		
		
		function GetRootDir()
		{
			$strRootDir = $this->GetModuleConfig("rootdir");
			return $strRootDir;
		}
		
		function GetCurrentDirRelative()
		{
			$strCurrentDirRel = $this->GetModuleVar("currentdir");
			if (FindString($strCurrentDirRel,"..") != -1)
			{
				$strCurrentDirRel = "";
			}
			return $strCurrentDirRel;
			
		}

		function GetCurrentDirAbsolute()
		{
			$strRootDir = $this->GetRootDir();
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDir = $strRootDir . $strCurrentDirRel;
			if (!IsDirectory($strCurrentDir))
			{
				$strCurrentDir = $strRootDir;
			}	
			return $strCurrentDir;
		}
	
		function StringTrimSlashes($strString)
		{
			$strString = StringRemoveTrailingCharacters($strString,"/");
			$strString = StringRemoveLeadingCharacters($strString,"/");
			return $strString;
		}
	
		function GetFileInfoText($strPath)
		{
			//$this->Trace("GetFileInfoText(\"$strPath\")");
			$strTID = MakeTID("FILEINFO_".$this->StringTrimSlashes($strPath))."";
			//$this->Trace("GetFileInfoText: $strTID");
			$strText = $this->LoadString($strTID);
			if ($strText != "")
			{
				return $strText;
			}

			$strFileName = GetFileNameFromPath($strPath);
			if ($strFileName != "")
			{
				$strTID = MakeTID("FILEINFO_".GetFileNameFromPath($strPath))."";
				//$this->Trace("GetFileInfoText: $strTID");
				$strText = $this->LoadString($strTID);
				if ($strText != "")
				{
					return $strText;
				}

				$strTID = MakeTID("FILEINFO_".GetFileTitleFromPath($strPath))."";
				//$this->Trace("GetFileInfoText: $strTID");
				$strText = $this->LoadString($strTID);
				if ($strText != "")
				{
					return $strText;
				}
			}
			return "";
		}


		function StateBrowsePortfolio()
		{
			$this->Trace("StateBrowsePortfolio");
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDirAbs = $this->GetCurrentDirAbsolute();

			$arrayDir = ReadDirectory($strCurrentDirAbs,false,false);

			$this->IncludeElement("portfolio");
			$portfolio = new CPortfolioElement($this->GetWebsite(),"downloads");

			$arrayPortfolio = array();
			foreach ($arrayDir as $strPathRel)
			{
				$arrayRow = array();
				
				$strLink = "";
				$strPathAbs = $strCurrentDirAbs . $strPathRel;
				
				if (IsFile($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "download";
					$arrayParams[$this->GetModuleVarName("file")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					$strFileInfo = $this->GetFileInfoText($strPathAbs);
									
					$arrayPortfolio[] = array(
   					"title" => "$strPathRel",  
   					"text" => $strFileInfo,
   					"linkurl" => $strLink,
   					"linktext" => "?TID_LINK_DOWNLOAD?");
					
				}
				
			}
			
			$strOutput = $portfolio->RenderPortfolio($arrayPortfolio);
			$this->SetOutputItem("PORTFOLIO",$strOutput);
 


			return "browse";
			
			
		}


		function StateBrowseItemManager()
		{
			$this->Trace("StateBrowseItemManager");
			
			$strRootDir = $this->GetModuleConfig("rootdir");
			
			$strCurrentDirRel = $this->GetCurrentDirRelative();
			$strCurrentDirAbs = $this->GetCurrentDirAbsolute();

			$arrayDir = ReadDirectory($strCurrentDirAbs,false,false);

			$bHideSize = $this->GetModuleConfig("hidesize");
			$bHideLastChange = $this->GetModuleConfig("hidelastchange");
			$bHideIcon = $this->GetModuleConfig("hideicon");
			$bHideInfo = $this->GetModuleConfig("hideinfo");
			$bHideTableHeader = $this->GetModuleConfig("hidetableheader");
			

			$this->IncludeElement("itemmanager");
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			$link = new CLinkElement($this->GetWebsite(),"default");
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			
			$itemmanagerdata = new CItemManagerData();
			
			if ($bHideTableHeader == true)
			{
				$itemmanagerdata->SetHideTableHeader(true);	
			}

			/*
			$strFilterOptionTab = $this->GetWebsite()->GetVar("itemmanager_filteroptions_tab");
			if ($strFilterOptionTab == "")
			{
			   $strFilterOptionTab = "all";   
			}
			*/
			

			$arrayTable = array();
			foreach ($arrayDir as $strPathRel)
			{
				$arrayRow = array();

				
				$arrayRow["NAME"] = "";
				$arrayRow["INFO"] = "";
				$arrayRow["TEXT"] = "";
				$arrayRow["SIZE"] = "";
				$arrayRow["LASTCHANGE"] = "";
				
				$strLink = "";
				$strPathAbs = $strCurrentDirAbs . $strPathRel;
				
				if ($bHideInfo == false)
				{
					$arrayRow["INFO"] = $this->GetFileInfoText($strPathAbs);
				}
				
				if (IsDirectory($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "browse";
					$arrayParams[$this->GetModuleVarName("currentdir")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					
					if ($bHideIcon == false)
					{
						$arrayRow["NAME"] = $linkicon->RenderLinkIcon($strLink,'folder',$strPathRel);
					}
				}
				else if (IsFile($strPathAbs))
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "download";
					$arrayParams[$this->GetModuleVarName("file")] = $strCurrentDirRel.$strPathRel;
					$strLink = $this->GetWebsite()->MakeURL($arrayParams);
					
					if ($bHideSize == false)
					{
						$arrayRow["SIZE"] = "".GetFileSize($strPathAbs);
					}
					if ($bHideLastChange == false)
					{
						$arrayRow["LASTCHANGE"] = $this->GetWebsite()->RenderDateTime(GetFileLastChangeTime($strPathAbs));
					}
					if ($bHideIcon == false)
					{
						$strFileCategory = GetFileCategory($strPathAbs);
						
						$arrayRow["NAME"] = $linkicon->RenderLinkIcon($strLink,GetIconIDByFileCategory($strFileCategory),$strPathRel);
					}
				}
				
				if ($strLink != "")
				{
					$arrayRow["NAME"] = $arrayRow["NAME"] .$link->RenderLink($strLink,$strPathRel);
				}
					
				$arrayTable[] = $arrayRow;
				
			}
			
			$itemmanagerdata->SetItemList($arrayTable);

			/*



$arrayFilterOptionTabs = array(
   "all" => "All",
   "even" => "Even",
   "uneven" => "Uneven"
   );
$itemmanagerdata->SetFilterOptionsTabNavigation(
   $strFilterOptionTab,
   "itemmanager_filteroptions_tab",
   $arrayFilterOptionTabs);

*/


			//$itemmanagerdata->AddColumn("ICON"," ","",false);
			$itemmanagerdata->AddColumn("NAME","?TID_STATICFILEBROWSER_TABLEHEADER_NAME?","",($bHideInfo)?(false):(true));
			if ($bHideInfo == false)
			{
				$itemmanagerdata->AddColumn("INFO"," ","",false);
			}
			if ($bHideSize == false)
			{
				$itemmanagerdata->AddColumn("SIZE","?TID_STATICFILEBROWSER_TABLEHEADER_SIZE?","",false,"","right");
			}
			if ($bHideLastChange == false)
			{
				$itemmanagerdata->AddColumn("LASTCHANGE","?TID_STATICFILEBROWSER_TABLEHEADER_LASTCHANGE?","",false);
			}

/*
$arrayColumnTabs = array(
   "primary" => "Primary",
   "secondary" => "Secondary",
   "tertiary" => "Tertiary",
   "unused" => "Unused");
 
$itemmanagerdata->SetColumnsTabNavigation(
   $this->GetWebsite()->GetVar("itemmanager_columns_tab"),
   "itemmanager_columns_tab",
   $arrayColumnTabs);
*/
/*
$itemmanagerdata->SetSortOptions(
   "itemmanager_sort_option",
   $this->GetVar("itemmanager_sort_option"),
   "itemmanager_sort_order",
   $this->GetVar("itemmanager_sort_order")
   );
*/
  		
			
			$strOutput = $itemmanager->RenderItemManager($itemmanagerdata);
			$this->SetOutputItem("LIST",$strOutput);
 
			$this->RenderToolBar();


			return "browse";
		}

		function StateBrowse()
		{
			switch ($this->GetStyle())
			{
			case "portfolio":
				return $this->StateBrowsePortfolio();
			default:
				return $this->StateBrowseItemManager();
			}
		}

		
		function StateDownload()
		{
			$strFilePathRel = $this->GetModuleVar("file");
			$strFilePathAbs = $this->GetRootDir().$strFilePathRel;
			$strFileName = GetFilenameFromPath($strFilePathAbs);
			
			if (IsFile($strFilePathAbs))
			{
				
				$this->GetWebsite()->DownloadFile(
					"application/unknown",
					$strFileName,
					FileReadBinary($strFilePathAbs));
			}

			$this->SetModuleVar("file","");
			return $this->StateBrowse();
		}
		


	}

