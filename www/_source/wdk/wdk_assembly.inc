<?php

	function IsValidAssemblyID($strAssemblyID)
	{
		if ($strAssemblyID == "")	
		{
			return false;	
		}
		if (StringCheckCharSet($strAssemblyID,CHARSET_ASSEMBLYID) != true)
		{
			return false;	
		}
		if (StringCheckCharSet(StringSection($strAssemblyID,0,1),CHARSET_ALPHA_LOWERCASE) != true)
		{
			return false;
		}
		return true;
	}

	final class CAssembly extends CWebsiteSatellite
	{
		private $m_strRoorDir;
		private $m_bPluginsDir;
		private $m_bLayoutDir;
		private $m_bStyleSheetsDir;
		private $m_bJavaScriptsDir;
		private $m_bNavigationDir;
		private $m_bResDir;
		private $m_bModulesDir;
		private $m_bRoutingDir;
		
		function __construct(
			CWebSite &$website,
			$strAssemblyID,
			$strRootDir = '')
		{
			if (IsValidAssemblyID($strAssemblyID) == false)
			{
				FatalError("CAssembly has been initialized with an invalid assembly id.");
			}
			
			if ($strRootDir == "")
			{
				$strRootDir = $website->GetAssemblyDir($strAssemblyID);	
			}
			if (!IsDirectory($strRootDir))
			{
				FatalError("CAssembly has been initialized with an invalid root path: \"$strRootDir\"");
			}
			$this->SetRootDir($strRootDir);
			$this->m_bPluginsDir = IsDirectory($this->GetPluginsDir());			
			$this->m_bLayoutDir = IsDirectory($this->GetLayoutDir());			
			$this->m_bStyleSheetsDir = IsDirectory($this->GetStyleSheetsDir());
			$this->m_bJavaScriptsDir = IsDirectory($this->GetJavaScriptsDir());
			$this->m_bNavigationDir = IsDirectory($this->GetNavigationDir());			
			$this->m_bResDir = IsDirectory($this->GetResDir());			
			$this->m_bModulesDir = IsDirectory($this->GetModulesDir());			
			$this->m_bRoutingDir = IsDirectory($this->GetRoutingDir());			
			parent::__construct(
				$website,
				$strAssemblyID);
		}
		
		function GetSatelliteType()
		{
			return 'Assembly';	
		}


		final
		function SetRootDir($strRootDir)
		{
			$this->m_strRootDir = $strRootDir;
		}
		
		final
		function GetRootDir()
		{
			return $this->m_strRootDir;	
		}
		
		final
		function GetContentDir()
		{
			return $this->m_strRootDir . "content/";	
		}

		final
		function GetResDir()
		{
			return $this->m_strRootDir . "res/";	
		}

		final
		function GetRoutingDir()
		{
			return $this->m_strRootDir . "routing/";	
		}


		final
		function GetModulesDir()
		{
			return $this->m_strRootDir . "modules/";	
		}

		final
		function GetElementsDir()
		{
			return $this->m_strRootDir . "elements/";	
		}

		final
		function GetNavigationDir()
		{
			return $this->m_strRootDir . "navigation/";	
		}

		final
		function GetPluginsDir()
		{
			return $this->m_strRootDir . 'plugins/';	
		}		

		final
		function GetLayoutDir()
		{
			return $this->m_strRootDir . "layout/";	
		}		

		final
		function GetDataDir()
		{
			return $this->m_strRootDir . "data/";	
		}		

		final
		function GetDownloadDir()
		{
			return $this->m_strRootDir . "download/";	
		}	

		final
		function GetImagesDir()
		{
			return $this->m_strRootDir . "images/";	
		}	

		final
		function GetFontsDir()
		{
			return $this->m_strRootDir . "fonts/";	
		}	

		final
		function GetVideosDir()
		{
			return $this->m_strRootDir . "videos/";	
		}	


		final
		function GetStyleSheetsDir()
		{
			return $this->m_strRootDir . "css/";	
		}	

		final
		function GetJavaScriptsDir()
		{
			return $this->m_strRootDir . "js/";	
		}	

		function GetAssetFilePath(
			$strAssetType,
			$arrayParams)
		{
			$strFilePath = "";
			switch ($strAssetType)
			{
			case "content":
				$strFilePath = $this->MakeContentFilePath(
					ArrayGetValue($arrayParams,"content"),
					ArrayGetValue($arrayParams,"language"),
					ArrayGetValue($arrayParams,"ext")
					);
				break;
			case 'plugin':
				$strFilePath = $this->MakePluginSourceFilePath(
					ArrayGetValue($arrayParams,'plugin')
					);
				break;
			case 'element':
				$strFilePath = $this->MakeElementSourceFilePath(
					ArrayGetValue($arrayParams,"element")
					);
				break;					
			case "element-democode":
				$strFilePath = $this->MakeElementDemoCodeFilePath(
					ArrayGetValue($arrayParams,"element")
					);
				break;					
			case "layout-element":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeElementLayoutFilePath(
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"element"),
					ArrayGetValue($arrayParams,"style"),
					ArrayGetValue($arrayParams,"ext")
					);
				break;					
			case "layout-element-javascript":
				if ($this->m_bJavaScriptsDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeElementLayoutJavaScriptFilePath(
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"element"),
					ArrayGetValue($arrayParams,"style")
					);
				break;					
			case "layout-form":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeFormLayoutFilePath(
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"form"),
					ArrayGetValue($arrayParams,"style")
					);
				break;					
			case "layout-module":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeModuleLayoutFilePath(
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"moduleid"),
					ArrayGetValue($arrayParams,"state"),			
					ArrayGetValue($arrayParams,"style")
					);
				break;					
			case "layout-framework":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeFrameworkLayoutFilePath(
					ArrayGetValue($arrayParams,"framework"),     
					ArrayGetValue($arrayParams,"layout")
					);
				break;					
			case "layout-config":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeLayoutConfigFilePath(
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"config")
					);
				break;					
			case "layout-css":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeLayoutCssFilePath(
					ArrayGetValue($arrayParams,"layout")
					);
				break;		
			case "layout-inc":
				if ($this->m_bLayoutDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeLayoutIncFilePath(
					ArrayGetValue($arrayParams,"layout")
					);
				break;								
			case "stylesheet":
				if ($this->m_bStyleSheetsDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeStyleSheetFilePath(
					ArrayGetValue($arrayParams,"stylesheet")
					);
				break;					
			case "javascript":
				if ($this->m_bJavaScriptsDir == false)
				{
					return false;	
				}
				$strFilePath = $this->MakeJavaScriptFilePath(
					ArrayGetValue($arrayParams,"javascript")
					);
				break;					
			case "res":
				if ($this->m_bResDir == false)
				{
					return false;
				}
				$strFilePath = $this->MakeResourceFilePath(
					ArrayGetValue($arrayParams,"language"),
					ArrayGetValue($arrayParams,"id")
					);
				break;					
			case "routing":
				if ($this->m_bRoutingDir == false)
				{
					return false;
				}
				$strFilePath = $this->MakeRoutingMapFilePath(
					ArrayGetValue($arrayParams,"language"),
					ArrayGetValue($arrayParams,"id")
					);
				break;					
			case "navigation":
				if ($this->m_bNavigationDir == false)
				{
					return false;
				}
				$strFilePath = $this->MakeNavigationFilePath(
					ArrayGetValue($arrayParams,"navigation"),
					ArrayGetValue($arrayParams,"language")
					);
				break;					
			case "module":
				if ($this->m_bModulesDir == false)
				{
					return false;
				}
				$strFilePath = $this->MakeModuleSourceFilePath(
					ArrayGetValue($arrayParams,"modulepath")
					);
				break;					
			case "image-layout":
				$strFilePath = $this->MakeLayoutImageFilePath(
					ArrayGetValue($arrayParams,"image"),
					ArrayGetValue($arrayParams,"layout"),
					ArrayGetValue($arrayParams,"ext")
					);
				break;					
			case "image":
				$strFilePath = $this->MakeImageFilePath(
					ArrayGetValue($arrayParams,"imagefilename")
					);
				break;					
			case "font":
				$strFilePath = $this->MakeFontFilePath(
					ArrayGetValue($arrayParams,"fontfilename")
					);
				break;					
			case "video":
				$strFilePath = $this->MakeVideoFilePath(
					ArrayGetValue($arrayParams,"videofilename")
					);
				break;					
			case "data":
				$strFilePath = $this->MakeDataFilePath(
					ArrayGetValue($arrayParams,"filepath")
					);
				break;					
			case "download":
				$strFilePath = $this->MakeDownloadFilePath(
					ArrayGetValue($arrayParams,"filepath")
					);
				break;					
			default:
				$this->Error("GetAssetFilePath() has been called with an unsupported asset type: \"$strAssetType\"");
				break;				
			}
			
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}	
			return false;
		} 
		
		function MakeContentFilePath(
			$strContentID,
			$strLanguage,
			$strExt)
		{
			if ($strExt == "")
			{
				$strExt = "htm";
			}
			$strLanguagePath = "";
			if ($strLanguage != "" && $strExt != "inc")
			{
				$strLanguagePath = StringLowerCase($strLanguage) . "/";
			}
			return $this->GetContentDir().$strLanguagePath."content_" .$strContentID.".".$strExt;
		}

		function MakePluginSourceFilePath(
			$strPluginID)
		{
			return $this->GetPluginsDir().'plugin_'.$strPluginID.'.inc';
		}

		
		function MakeElementSourceFilePath(
			$strElementID)
		{
			return $this->GetElementsDir().'element_'.$strElementID.'.inc';
		}

		function MakeElementDemoCodeFilePath(
			$strElementID)
		{
			return $this->GetElementsDir()."element_".$strElementID."_democode.inc";
		}		
		
		function MakeElementLayoutFilePath(
			$strLayoutID,
			$strElementID,
			$strStyleID,
			$strExt)
		{
			if ($strLayoutID == "")
			{
				$strLayoutID = "default";
			}
			$strFilePath = $this->GetLayoutDir()."layout_" .$strLayoutID."_element_".$strElementID."_".$strStyleID.".".$strExt;
			//$this->Trace($strFilePath);
			return $strFilePath;
		}
		
		function MakeElementLayoutJavaScriptFilePath(
			$strLayoutID,
			$strElementID,
			$strStyleID)
		{
			if ($strLayoutID == "")
			{
				$strLayoutID = "default";
			}			
			return $this->GetJavaScriptsDir()."layout_element_".$strElementID."_".$strStyleID.".js";
		}
		
		function MakeFormLayoutFilePath(
			$strLayoutID,
			$strFormID,
			$strStyleID)
		{
			if ($strLayoutID == "")
			{
				$strLayoutID = "default";
			}
			$strFilePath = $this->GetLayoutDir(). "layout_" .$strLayoutID."_form_".$strFormID."_".$strStyleID.".htm";	
			//$this->Trace($strFilePath);
			return $strFilePath;
			
		}
		
		function MakeNavigationFilePath(
			$strNavigationID,
			$strLanguage)
		{
			if ($strLanguage == "")
			{
				$strLanguage = "int";	
			}
			return $this->GetNavigationDir() . "navigation_" .$strNavigationID."_". StringLowerCase($strLanguage) .".txt";
		}
		
		function MakeResourceFilePath($strLanguage,$strResID)
		{
			return $this->GetResDir()."res_".$strResID."_".StringLowerCase($strLanguage).".txt";
		}

		function MakeRoutingMapFilePath($strLanguage,$strID)
		{
			return $this->GetRoutingDir()."routing_".$strID."_".StringLowerCase($strLanguage).".txt";
		}

		
		function MakeModuleSourceFilePath(
			$strModulePath)
		{
			$arrayModulePathComponents = explode("/",$strModulePath);
			if (!is_array($arrayModulePathComponents) || ArrayCount($arrayModulePathComponents) != 2)
			{
				return false;
			}
			$strModuleGroup = $arrayModulePathComponents[0];
			$strModuleID = $arrayModulePathComponents[1];
			$strFile = $this->GetModulesDir().$strModuleGroup."/".$strModuleID."/module_".$strModuleID.".inc";
			return $strFile;
		}
		
		function MakeModuleLayoutFilePath(
			$strLayoutID,
			$strModuleID,
			$strState,
			$strStyle)
		{
			if ($strLayoutID == "")
			{
				$strLayoutID = "default";	
			}
			if ($strState == "")
			{
				$strState = "default";	
			}
			if ($strStyle == "default")
			{
				$strStyle = "";
			}
			if ($strStyle != "")
			{
				$strStyle = "_".$strStyle;	
			}
			return $this->GetLayoutDir().
				"layout_".$strLayoutID."_module_".$strModuleID."_".$strState.$strStyle.".htm";
		}
		
		function MakeDataFilePath($strFilePath)
		{
			return $this->GetDataDir().$strFilePath;
		}

		function MakeDownloadFilePath($strFilePath)
		{
			return $this->GetDownloadDir().$strFilePath;
		}


		function MakeStyleSheetFilePath($strID)
		{
			return $this->GetStyleSheetsDir().$strID.".css";
		}

		function MakeJavaScriptFilePath($strID)
		{
			return $this->GetJavaScriptsDir().$strID.".js";
		}

		
		function MakeLayoutImageFilePath($strImageID, $strLayout, $strExt)
		{
			return $this->GetImagesDir() . "layout_".$strLayout."_".$strImageID.".".$strExt;
		}

		function MakeImageFilePath($strImageFileName)
		{
			return $this->GetImagesDir().$strImageFileName;
		}	

		function MakeFontFilePath($strFontFileName)
		{
			return $this->GetFontsDir().$strFontFileName;
		}	

		function MakeVideoFilePath($strVideoFileName)
		{
			return $this->GetVideosDir().$strVideoFileName;
		}	

			
		function MakeFrameworkLayoutFilePath($strFramework,$strLayout)
		{
			if ($strLayout == "")
			{
				$strLayout = "default";
			}			
			return $this->GetLayoutDir() . "layout_".$strLayout."_framework_".$strFramework.".htm";
		}	
			
		function MakeLayoutConfigFilePath($strLayout,$strConfig='')
		{
			if ($strLayout == '')
			{
				$strLayout = 'default';
			}
			if ($strConfig != '')
			{
				$strConfig = '_config_'.$strConfig;	
			}
			return $this->GetLayoutDir() . 'layout_'.$strLayout.$strConfig.'.cfg';
		}		
		
		function MakeLayoutCssFilePath($strLayout)
		{
			if ($strLayout == "")
			{
				$strLayout = "default";
			}			
			return $this->GetLayoutDir() . "layout_".$strLayout.".css";
		}		
		
		function MakeLayoutIncFilePath($strLayout)
		{
			if ($strLayout == "")
			{
				$strLayout = "default";
			}			
			return $this->GetLayoutDir() . "layout_".$strLayout.".inc";
		}		
		
		
		
		
		function SetOutputItem($strItem, $strContents)
		{
			$this->GetWebsite()->SetOutputItem($strItem, $strContents);
		}
		function SetOutputItems($arrayItems)
		{
			$this->GetWebsite()->SetOutputItems($arrayItems);
		}

		
		
		function EnumerateElements()
		{
			$strElementsDir = $this->GetElementsDir();
			$arrayFileNames = ReadDirectory($strElementsDir);
			if (ArrayCount($arrayFileNames) == 0)
			{
				return array();
			}
			$arrayElements = array();
			foreach ($arrayFileNames as $strFileName)
			{
				if (StringSection($strFileName,0,8) == "element_")
				{
					if (FindString($strFileName,"_democode.inc") == -1)
					{
						if (GetExtentionFromPath($strFileName) == "inc")
						{
							$strElementID = StringSection($strFileName,8);
							$strElementID = StringSection($strElementID,0,-4);
							$arrayElements[] = $strElementID;
						}
					}
				}
			}
			return $arrayElements;
		}
		
		
		
		
	}



