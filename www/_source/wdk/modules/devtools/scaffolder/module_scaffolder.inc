<?php

	require_once(GetWDKDir()."wdk_scaffolding.inc");

	class CScaffolderModule extends CModule
	{
		private $m_arrayErrors;
		
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			parent::__construct(
				$website,
				"devtools/scaffolder",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			if ($this->GetPreselectedScaffoldID() != "")
			{
				$this->SetModuleVar("scaffoldcategory",$this->GetPreselectedScaffoldCategory());
				$this->SetModuleVar("scaffoldid",$this->GetPreselectedScaffoldID());
			}
			if ($this->GetState() == "")
			{				
				$this->SetState("select");	
			}

			$arrayToolbarConfig = array();
			$arrayToolbarConfig["help"] = array("selectcategory");
			$arrayToolbarConfig["selectcategory"] = array("help");
			$arrayToolbarConfig["selectscaffold"] = array("help","selectcategory");
			$arrayToolbarConfig["configure"] = array("help","selectcategory","selectscaffold");
			$arrayToolbarConfig["prepare"] = array("help","selectcategory","selectscaffold","configure");
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return true;
		}

		function GetPreselectedScaffoldCategory()
		{
			$arrayPreselectedScaffold = StringExplode($this->GetModuleConfig("preselectedscaffold"),"/",true);
			return ArrayGetValue($arrayPreselectedScaffold,0);
		}
	
		function GetPreselectedScaffoldID()
		{
			$arrayPreselectedScaffold = StringExplode($this->GetModuleConfig("preselectedscaffold"),"/",true);
			return ArrayGetValue($arrayPreselectedScaffold,1);
		}
		

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "help":
				{
					return $this->StateHelp();
				}				
				case "selectscaffold":
				{
					return $this->StateSelectScaffold();
				}				
				case "configure":
				{
					return $this->StateConfigure();
				}				
				case "prepare":
				{
					return $this->StatePrepare();
				}				
				case "download":
				{
					return $this->StateDownload();
				}				
				case "selectcategory":
				default:
				{
					return $this->StateSelectCategory();
				}
			}
		}
		
		
		function EnumerateScaffolds($strCategory)
		{
			$arrayScaffoldRepositoryDirs = array();
			
			$arrayAssemblies = $this->GetWebsite()->GetAssemblyLine()->GetAssemblyList();
			if (is_array($arrayAssemblies))
			{
				foreach ($arrayAssemblies as $strAssembly)
				{
					$assembly = $this->GetWebsite()->GetAssemblyLine()->GetAssembly($strAssembly);
					if (is_object($assembly))
					{
						$strAssemblyRootDir = $assembly->GetRootDir();
						if (IsDirectory($strAssemblyRootDir))
						{
							$arrayScaffoldRepositoryDirs[] = $strAssemblyRootDir."scaffolds/".$strCategory."/";
						}
					}
				}
			}
			
			//$arrayScaffoldRepositoryDirs[] = GetWDKDir()."scaffolds/".$strCategory."/";
			
			$arrayScaffolds = array();
			foreach ($arrayScaffoldRepositoryDirs as $strScaffoldRepositoryDir)
			{
				$arrayScaffoldDirs = ReadDirectory($strScaffoldRepositoryDir,false);
				if ($arrayScaffoldDirs != false)
				{
					foreach ($arrayScaffoldDirs as $strScaffoldDir)
					{
						$strScaffoldDirFull = $strScaffoldRepositoryDir . $strScaffoldDir;
						if (IsFile($strScaffoldDirFull."scaffold.cfg"))
						{
							$strScaffoldID = GetFilenameFromPath(StringSection($strScaffoldDirFull,0,-1));
							$this->GetWebsite()->LoadStringResources("scaffold-".$strCategory."-".$strScaffoldID);	
							$arrayScaffolds[$strScaffoldID] = $strScaffoldDirFull;
						}
					}
				}
			}
			return $arrayScaffolds;
		}

		function GetScaffoldDir($strCategory,$strID)
		{
			$arrayScaffolds = $this->EnumerateScaffolds($strCategory);
			$strDir = ArrayGetValue($arrayScaffolds,$strID);
			if (!IsDirectory($strDir))
			{
				return false;
			}
			return $strDir;
		}

		function MakeScaffoldCategoryList()
		{
			$arrayScaffoldCategories = array();
			$arrayScaffoldCategories["websites"] = "?TID_SCAFFOLDER_CATEGORY_WEBSITES?";
			$arrayScaffoldCategories["components"] = "?TID_SCAFFOLDER_CATEGORY_COMPONENTS?";
			$arrayScaffoldCategories["other"] = "?TID_SCAFFOLDER_CATEGORY_OTHER?";
			return $arrayScaffoldCategories;
		}

		
		function MakeScaffoldList($arrayScaffolds,$strScaffoldCategory)
		{
			$arrayScaffoldList = array();
			foreach ($arrayScaffolds as $strScaffoldID => $strScaffoldDir)
			{
				$strScaffoldName = $strScaffoldID;

				$strScaffoldName = "?TID_SCAFFOLD_".StringUpperCase($strScaffoldCategory)."_".StringUpperCase($strScaffoldID)."_NAME?";
				$strScaffoldName .= "<br/><br/>";
				$strScaffoldName .= "?TID_SCAFFOLD_".StringUpperCase($strScaffoldCategory)."_".StringUpperCase($strScaffoldID)."_DESCRIPTION?";
				$strScaffoldName .= "<br/>";				
				
				$arrayScaffoldList[$strScaffoldID] = $strScaffoldName;
			}
			return $arrayScaffoldList;
		}
		
		 
		function GetScaffolderClass($strScaffoldCategory,$strScaffoldID)
		{
			$strScaffoldDir = $this->GetScaffoldDir($strScaffoldCategory,$strScaffoldID);
			if ($strScaffoldDir == "")
			{
				return null;	
			}
			
			$strScaffoldConfigFile = $strScaffoldDir . "scaffold.cfg";
			$strScaffoldClassFile = $strScaffoldDir . "scaffold.inc";
			if (IsFile($strScaffoldClassFile))
			{
				include_once($strScaffoldClassFile);
				$scaffolder = new CMyScaffolder($this,$strScaffoldDir,$strScaffoldCategory,$strScaffoldID);
			}
			else
			{
				$scaffolder = new CScaffolder($this,$strScaffoldDir,$strScaffoldCategory,$strScaffoldID);
			}
			return $scaffolder;
		}
		
	
		function RenderToolbar()
		{
			if ($this->GetPreselectedScaffoldID() != "")
			{
				$this->SetOutputItem("TOOLBAR","");
				return;	
			}
			
		
			$toolbarDef = new CToolbarDefinition();

			if ($this->IsToolVisible("help"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "help";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "help", "?TID_SCAFFOLDER_TOOL_HELP?");
			}

			if ($this->IsToolVisible("selectcategory"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "selectcategory";
				$arrayParams[$this->GetModuleVarName("scaffoldcategory")] = "";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_SCAFFOLDER_TOOL_SELECTCATEGORY?");
			}

			if ($this->IsToolVisible("selectscaffold"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "selectscaffold";
				$arrayParams[$this->GetModuleVarName("scaffoldid")] = "";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_SCAFFOLDER_TOOL_SELECTSCAFFOLD?");
			}

			if ($this->IsToolVisible("configure"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "configure";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_SCAFFOLDER_TOOL_CONFIGURE?");
			}

			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}

		function StateHelp()
		{
			$this->SetState("help");
			
			if ($this->GetPreselectedScaffoldID() != "")
			{
				return $this->StatePrepare();	
			}			
			
			
			$this->RenderToolbar();
			return "help";
		}
		
		
		function StateSelectCategory()
		{
			$this->SetState("selectcategory");
			
			if ($this->GetPreselectedScaffoldID() != "")
			{
				return $this->StateConfigure();	
			}			
			
			$this->SetModuleVar("scaffoldcategory","");
			$this->RenderToolbar();

			$arrayScaffoldCategories = $this->MakeScaffoldCategoryList();
			if (ArrayCount($arrayScaffoldCategories) == 0)
			{
				$this->SetErrorMessageCode("NO_SCAFFOLDS_AVAILABLE");	
				return "error";
			}

			$formDef = new CFormDefinition();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "selectscaffold";   
			$arrayParams[$this->GetModuleVarName("scaffoldcategory")] = $this->GetModuleVar("scaffoldcategory");    
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlRadio(
   			"SCAFFOLDCATEGORY",
   			$this->GetModuleVarName("scaffoldcategory"),
   			"?TID_SCAFFOLDER_CONTROL_SELECTCATEGORY?",
   			$arrayScaffoldCategories,
   			key($arrayScaffoldCategories));

			$formDef->AddControlStatic(
 				"INFO",
 				"?TID_SCAFFOLDER_TEXT_SELECTCATEGORY?");

			
			$formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");
			
			$form = new CFormElement(
				$this->GetWebsite(),
   			"default");
			
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
      		$formDef,
      		$this->GetAllModuleVars())
   			);
			
			return "selectcategory";
		}

		
		
		function StateSelectScaffold()
		{
			$this->SetState("selectscaffold");
			
			if ($this->GetPreselectedScaffoldID() != "")
			{
				return $this->StateConfigure();	
			}
			
			
			$this->SetModuleVar("scaffoldid","");
			$strScaffoldCategory = $this->GetModuleVar("scaffoldcategory");
			if ($strScaffoldCategory == "")
			{
				return $this->StateSelectCategory();	
			}
			$this->RenderToolbar();
	
			$arrayScaffolds = $this->EnumerateScaffolds($strScaffoldCategory);
			if (ArrayCount($arrayScaffolds) == 0)
			{
				$this->SetErrorMessageCode("NO_SCAFFOLDS_AVAILABLE");	
				return "error";
			}
				
			$arrayScaffoldOptions = $this->MakeScaffoldList($arrayScaffolds,$strScaffoldCategory);


			$formDef = new CFormDefinition();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "configure";   
			//$arrayParams[$this->GetModuleVarName("scaffoldid")] = $this->GetModuleVar("scaffoldid");
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic(
 				"INFO",
 				"?TID_SCAFFOLDER_TEXT_SELECTSCAFFOLD?");

			$formDef->AddControlRadio(
   			"SCAFFOLD",
   			$this->GetModuleVarName("scaffoldid"),
   			"?TID_SCAFFOLDER_CONTROL_SELECTSCAFFOLD?",
   			$arrayScaffoldOptions,
   			key($arrayScaffoldOptions));
			
			$formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");
			
			$form = new CFormElement(
				$this->GetWebsite(),
   			"default");
			
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
      		$formDef,
      		$this->GetAllModuleVars())
   			);
			
			return "selectscaffold";
		}

		
		function StateConfigure()
		{
			$this->SetState("configure");

			$strScaffoldCategory = $this->GetModuleVar("scaffoldcategory");
			if ($strScaffoldCategory == "")
			{
				return $this->StateSelectCategory();	
			}
			
			$strScaffoldID = $this->GetModuleVar("scaffoldid");
			if ($strScaffoldID == "")
			{
				return $this->StateSelectScaffold();	
			}

			$scaffolder = $this->GetScaffolderClass($strScaffoldCategory, $strScaffoldID);
			if ($scaffolder == null)
			{
				$this->RenderToolbar();
				return "error";	
			}
			
			if ($scaffolder->IsConfigRequired() == false)
			{
				return $this->StatePrepare();	
			}
			$this->RenderToolbar();
			
			$formDef = new CFormDefinition();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "prepare";   

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic(
 				"DESCRIPTION",
 				$scaffolder->GetScaffoldConfig("DESCRIPTION"));

			if ($scaffolder->IsConfigRequired() == true)
			{
				$formDef->AddControlStatic(
	 				"INFO",
	 				"?TID_SCAFFOLDER_TEXT_CONFIGURE?");
				
				$scaffolder->PrepareForm($formDef,$this);
			}
	
			$formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");
		
			$form = new CFormElement(
				$this->GetWebsite(),
				"default");
		
			$this->SetOutputItem(
				"FORM",
			  $form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors)
				);
			
			return "configure";
		}
		
		function CallbackIsToolVisible($strToolID)
		{
			return parent::CallbackIsToolVisible($strToolID);
			
		}
		
		function StatePrepare()
		{
			$this->SetState("prepare");
			$strScaffoldCategory = $this->GetModuleVar("scaffoldcategory");
			if ($strScaffoldCategory == "")
			{
				return $this->StateSelectCategory();	
			}
			
			$strScaffoldID = $this->GetModuleVar("scaffoldid");
			if ($strScaffoldID == "")
			{
				return $this->StateSelectScaffold();	
			}
			
			$scaffolder = $this->GetScaffolderClass($strScaffoldCategory, $strScaffoldID);
			if ($scaffolder == null)
			{
				$this->RenderToolbar();
				return "error";	
			}

			$scaffolder->CheckParams($this->m_arrayErrors); 
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateConfigure();	
			}


			if ($scaffolder->IsConfigRequired() == false)
			{
				$arrayToolbarConfig = $this->GetToolbarConfig();
				$arrayToolbarConfig["prepare"] = ArrayRemoveValue($arrayToolbarConfig["prepare"],"configure");
				$this->Trace($arrayToolbarConfig);
				$this->SetToolbarConfig($arrayToolbarConfig);
			}
			
			$this->RenderToolbar();			
			
			$formDef = new CFormDefinition();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "download";   
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			$formDef->AddControlStatic(
 				"INFO",
 				"?TID_SCAFFOLDER_TEXT_PREPARE?");
			
			$formDef->AddControlButtonSubmitDownload("?TID_SCAFFOLDER_BUTTON_DOWNLOAD?");
		
			$form = new CFormElement(
				$this->GetWebsite(),
				"default");
		
			$this->SetOutputItem(
				"FORM",
			  $form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					null)
				);

			return "prepare";
		}

		function StateDownload()
		{
			$this->SetState("download");
				$strScaffoldCategory = $this->GetModuleVar("scaffoldcategory");
			if ($strScaffoldCategory == "")
			{
				return $this->StateSelectCategory();	
			}
			
			$strScaffoldID = $this->GetModuleVar("scaffoldid");
			if ($strScaffoldID == "")
			{
				return $this->StateSelectScaffold();	
			}

			$scaffolder = $this->GetScaffolderClass($strScaffoldCategory, $strScaffoldID);
			if ($scaffolder == null)
			{
				$this->RenderToolbar();				
				return "error";	
			}

/*
			if ($scaffolder->IsConfigRequired() == false)
			{
				$arrayToolbarConfig = $this->GetToolbarConfig();
				ArrayRemoveValue($arrayToolbarConfig["download"],"configure");
				$this->SetToolbarConfig($arrayToolbarConfig);
			}
*/

			$scaffolder->CheckParams($this->m_arrayErrors);
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateConfigure();	
			}
			
			$strScaffoldArchiveFile = $scaffolder->Create();
					
			if ($strScaffoldArchiveFile === false)
			{
				$this->SetErrorMessageCode("SCAFFOLD_ARCHIVE_FILE_CREATION_FAILED");
				return $this->StatePrepare();	
			}
			
			
			
			$this->GetWebsite()->DownloadFile(
				"application/zip",
				$scaffolder->GetZipFileName(),
				$strScaffoldArchiveFile);
						
			return $this->StatePrepare();
		}
	}
	
	

