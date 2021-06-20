<?php

	require_once(GetWDKDir()."wdk_zip.inc");

	class CScaffolder extends CWebsiteSatellite
	{
		private $m_module;
		private $m_strScaffoldDir;
		private $m_strCategory;
		private $m_strScaffoldID;
		private $m_arrayCfg;
		private $m_strZipFileName;
	
		function __construct(
			CModule $module,
			$strScaffoldDir,
			$strCategory,
			$strScaffoldID)
		{
			$this->m_module = $module;
			$this->m_strScaffoldDir = $strScaffoldDir;
			$this->m_strScaffoldID = $strScaffoldID;
			$this->m_strCategory = $strCategory;
			$website = $this->m_module->GetWebSite();
			parent::__construct($website,$strCategory."/".$strScaffoldID);
			$this->GetWebsite()->LoadStringResources("scaffold-".$this->GetCategory()."-".$this->GetScaffoldID());	
			$this->m_arrayCfg = LoadNamedArrayFromTextFile($this->m_strScaffoldDir."scaffold.cfg");
			$this->m_strZipFileName = "wdk-scaffold.zip";
		}

		function GetSatelliteType()
		{
			return 'Scaffolder';	
		}


		final
		function GetScaffoldDir()
		{
			return $this->m_strScaffoldDir;
		}

		
		final
		function GetCategory()
		{
			return $this->m_strCategory;
		}
		
		final
		function GetScaffoldID()
		{
			return $this->m_strScaffoldID;
		}
		
		final
		function GetTID($strTIDPart)
		{
			return 	"?TID_SCAFFOLD_"
							.StringUpperCase($this->GetCategory())
							."_"
							.StringUpperCase($this->GetScaffoldID())
							."_".$strTIDPart."?";
		}
		
		
		final
		function GetScaffoldConfig($strTag)
		{
			return ArrayGetValue($this->m_arrayCfg,$strTag);
		}
		
		function IsConfigRequired()
		{
			return true;	
		}
		
		final
		function PrepareForm(CFormDefinition &$formDef)
		{
			$this->OnPrepareForm($formDef);
		}
		
		final
		function CheckParams(&$arrayErrors)
		{
			$arrayParams = array();
			$this->OnGetParams($arrayParams);
			$this->OnCheckParams($arrayParams,$arrayErrors);
			if (ArrayCount($arrayErrors) > 0)
			{
				return false;	
			}
			return true;
		}
		
		final		
		function Create()
		{
			$arrayScaffoldParams = array();
			$arrayScaffoldParams["wdk"] = $this->GetWebsite()->GetOutputItem("WDK");
			$arrayScaffoldParams["wdk-docs-url"] = $this->GetWebsite()->GetOutputItem("WDK_DOCS_URL");
			$arrayScaffoldParams["wdk-scaffolder-url"] = $this->GetWebsite()->GetOutputItem("WDK_SCAFFOLDER_URL");
			$this->OnGetParams($arrayScaffoldParams);
			
			$strZipFileTitle = ArrayGetValue($this->m_arrayCfg,"ZIPFILETITLE");
			if ($strZipFileTitle != "")
			{
				$strZipFileTitle = ReplaceTags($strZipFileTitle, $arrayScaffoldParams, "$$$", "$$$");	
				$this->m_strZipFileName = $strZipFileTitle.".zip";
			}
			
			$arrayArchiveFiles = array();
			$this->OnAddAdditionalArchiveFiles($arrayScaffoldParams,$arrayArchiveFiles);
			return CreateScaffoldArchive($this->GetScaffoldDir(),$arrayScaffoldParams,$arrayArchiveFiles);
		}
		
		final
		function GetZipFileName()
		{
			return $this->m_strZipFileName;	
		}

		final
		function GetModule()
		{
			return $this->m_module;	
		}

		final
		function GetVarName($strVar) 
		{
			return $this->GetModule()->GetModuleVarName($this->GetScaffoldID()."_".$strVar);    
		}
		
		final
		function GetVar($strVar)
		{
			return $this->GetModule()->GetModuleVar($this->GetScaffoldID()."_".$strVar);
		}		

		final
		function GetVarInsecure($strVar)
		{
			return $this->GetModule()->GetModuleVarInsecure($this->GetScaffoldID()."_".$strVar);
		}		

		
		function OnPrepareForm(CFormDefinition &$formDef)
		{
			
		}
		
		function OnGetParams(&$arrayParams)
		{
			
		}
		
		function OnCheckParams($arrayParams,&$arrayErrors)
		{
			
		}
		
		function OnAddAdditionalArchiveFiles($arrayParams,&$arrayArchiveFiles)
		{
			
		}

	}


	function CreateScaffoldArchive($strScaffoldDir,$arrayParams,$arrayArchiveFiles = array())
	{
		if (IsDirectory($strScaffoldDir) != true)
		{
			return false;	
		}
		
		$strTemplatesDir = $strScaffoldDir."templates/";
		$strSnippetsDir = $strScaffoldDir."snippets/";
		
		$arrayTemplateFiles = ReadDirectory($strTemplatesDir,true,false);
		$arraySnippetsFiles = ReadDirectory($strSnippetsDir,true,false);
		
		if (!is_array($arrayArchiveFiles))
		{
			$arrayArchiveFiles = array();
		}
		if (!is_array($arrayTemplateFiles))
		{
			$arrayTemplateFiles = array();
		}

		
		foreach ($arrayTemplateFiles as $strFile)
		{
			$strFileContents = FileReadBinary($strTemplatesDir.$strFile);
			
			$strFileExt = GetExtentionFromPath($strFile);
			if (	$strFileExt == "htm"
				|| 	$strFileExt == "txt"
				|| 	$strFileExt == "inc"
				|| 	$strFileExt == "php"
				|| 	$strFileExt == "cfg"
				|| 	$strFileExt == "htaccess"
				)
			{
				$strFileContents = ReplaceTags($strFileContents, $arrayParams, "$$$", "$$$");
			}
			$strFile = ReplaceTags($strFile, $arrayParams, "$$$", "$$$");
			$strFile = ReplaceString($strFile,"//","/"); 

			$arrayArchiveFiles[] = array(
				"archivefilepath" => $strFile,
				"contents" => $strFileContents
				);
		}
		
		$strArchiveContent = CompressFilesInMemory($arrayArchiveFiles);
		if ($strArchiveContent == false)
		{
			return false;	
		}
		
		return $strArchiveContent;
	}

