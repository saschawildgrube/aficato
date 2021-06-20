<?php

	require_once('wdk_assembly.inc');   
	
	class CAssemblyLine extends CWebsiteSatellite
	{
		private $m_arrayAssemblies;
		
		function __construct(
			CWebSite &$website)
		{
			$this->m_arrayAssemblies = array();
			parent::__construct($website,'assemblyline');
		}
		
		function GetSatelliteType()
		{
			return 'AssemblyLine';	
		}

		
		final
		function AddAssembly(CAssembly $assembly)
		{
			$this->m_arrayAssemblies[$assembly->GetID()] = $assembly;
			$assembly->Trace("CAssemblyLine::AddAssembly: ".$assembly->GetID()." ".$assembly->GetRootDir());
		}

		final
		function GetAssembly($strAssemblyID)
		{
			return ArrayGetValue($this->m_arrayAssemblies,$strAssemblyID);
		}
		
		final
		function GetAssemblyList()
		{
			return array_keys($this->m_arrayAssemblies);	
		}
		
		
		final
		function GetAssetFilePath(
			$strAssetType,
			$arrayParams = array(),
			$strFallbackParam = "")
		{
			foreach ($this->m_arrayAssemblies as $assembly)
			{
				$strFile = $assembly->GetAssetFilePath(
					$strAssetType,
					$arrayParams);
				if ($strFile != false)
				{
					return $strFile;
				}
			}
			if ($strFallbackParam == "")
			{
				return false;
			}
			$strFallbackParamValue = ArrayGetValue($arrayParams,$strFallbackParam);
			if ($strFallbackParamValue == "")
			{
				return false;
			}
			$arrayParams[$strFallbackParam] = "";
			return $this->GetAssetFilePath(
				$strAssetType,
				$arrayParams);
		}
		
		final
		function GetContentIncFilePath(
			$strContentID,
			$strLanguage)
		{
			return $this->GetAssetFilePath(
				"content",
				array(
					"content" => $strContentID,
					"language" => $strLanguage,
					"ext" => "inc")
				);
		}	

		final
		function GetContentHtmlFilePath(
			$strContentID,
			$strLanguage)
		{
			return $this->GetAssetFilePath(
				"content",
				array(
					"content" => $strContentID,
					"language" => $strLanguage,
					"ext" => "htm"),
				"language"
				);
		}	
		
		final
		function GetContentTxtFilePath(
			$strContentID,
			$strLanguage)
		{
			return $this->GetAssetFilePath(
				"content",
				array(
					"content" => $strContentID,
					"language" => $strLanguage,
					"ext" => "txt"),
				"language"
				);
		}				

		function GetPluginSourceFilePath(
			$strPluginID)
		{
			return $this->GetAssetFilePath(
				'plugin',
				array(
					'plugin' => $strPluginID)
				);		
		}


		function GetElementSourceFilePath(
			$strElementID)
		{
			return $this->GetAssetFilePath(
				'element',
				array(
					'element' => $strElementID)
				);		
		}
		
		function GetElementDemoCodeFilePath(
			$strElementID)
		{
			return $this->GetAssetFilePath(
				"element-democode",
				array(
					"element" => $strElementID)
				);		
		}		

		final
		function GetElementLayoutCfgFilePath(
			$strLayoutID,
			$strElementID,
			$strStyle)
		{
			return $this->GetAssetFilePath(
				"layout-element",
				array(
					"layout" => $strLayoutID,
					"element" => $strElementID,
					"style" => $strStyle,
					"ext" => "cfg"),
				"layout"
				);		
		}

		final
		function GetElementLayoutHtmlFilePath(
			$strLayoutID,
			$strElementID,
			$strStyle)
		{
			return $this->GetAssetFilePath(
				"layout-element",
				array(
					"layout" => $strLayoutID,
					"element" => $strElementID,
					"style" => $strStyle,
					"ext" => "htm"),
				"layout"
				);	
		}
		
		final
		function GetElementLayoutJavaScriptFilePath(
			$strLayoutID,
			$strElementID,
			$strStyle)
		{
			return $this->GetAssetFilePath(
				"layout-element-javascript",
				array(
					"layout" => $strLayoutID,
					"element" => $strElementID,
					"style" => $strStyle),
				"layout"
				);	
		}		
		
		final		
		function GetFormLayoutFilePath(
			$strLayoutID,
			$strFormID,
			$strStyle)
		{
			return $this->GetAssetFilePath(
				"layout-form",
				array(
					"layout" => $strLayoutID,
					"form" => $strFormID,
					"style" => $strStyle),
				"layout"
				);	
			return false;
		}		

		final
		function GetResourceFilePath($strLanguage,$strID)
		{
			return $this->GetAssetFilePath(
				"res",
				array(
					"language" => $strLanguage,
					"id" => $strID
					),
				"language"
				);	
		}

		final
		function GetRoutingMapFilePath($strLanguage,$strID)
		{
			return $this->GetAssetFilePath(
				"routing",
				array(
					"language" => $strLanguage,
					"id" => $strID
					),
				""
				);	
		}

		
		final		
		function GetNavigationFilePath(
			$strNavigationID,
			$strLanguage)
		{
			return $this->GetAssetFilePath(
				"navigation",
				array(
					"language" => $strLanguage,
					"navigation" => $strNavigationID
					),
				"language"
				);	
		}
		
		final		
		function GetModuleDir($strModulePath)
		{
			$strModuleSourceFilePath = $this->GetModuleSourceFilePath($strModulePath);
			if ($strModuleSourceFilePath != false)
			{
				return GetDirectoryFromPath($strModuleSourceFilePath);
			}
			return false;
		}
		
		final		
		function GetModuleSourceFilePath($strModulePath)
		{
			return $this->GetAssetFilePath(
				"module",
				array(
					"modulepath" => $strModulePath
					)
				);	
		}
		
		final		
		function GetModuleLayoutFilePath($strLayoutID,$strModuleID,$strState,$strStyle)
		{
			return $this->GetAssetFilePath(
				"layout-module",
				array(
					"layout" => $strLayoutID, 
					"moduleid" => $strModuleID,
					"state" => $strState,
					"style" => $strStyle
					),
				"layout"
				);				
		}

		final
		function GetStyleSheetFilePath($strID)
		{
			return $this->GetAssetFilePath(
				"stylesheet",
				array(
					"stylesheet" => $strID
					)
				);				
		}

		final
		function GetJavaScriptFilePath($strID)
		{
			return $this->GetAssetFilePath(
				"javascript",
				array(
					"javascript" => $strID
					)
				);				
		}


		final
		function GetDataFilePath($strFilePath)
		{
			return $this->GetAssetFilePath(
				"data",
				array(
					"filepath" => $strFilePath
					)
				);				
		}
		
		final
		function GetDownloadFilePath($strFilePath)
		{
			return $this->GetAssetFilePath(
				"download",
				array(
					"filepath" => $strFilePath
					)
				);				
		}
		
		
		
		final
		function GetImageFilePath($strImageFileName)
		{
			return $this->GetAssetFilePath(
				"image",
				array(
					"imagefilename" => $strImageFileName
					)
				);				
		}		
		
		final
		function GetLayoutImageFilePath($strImage,$strLayout,$strExt)
		{
			return $this->GetAssetFilePath(
				"image-layout",
				array(
					"image" => $strImage,
					"layout" => $strLayout,
					"ext" => $strExt
					),
				"layout"
				);				
		}

		final
		function GetFontFilePath($strFontFileName)
		{
			return $this->GetAssetFilePath(
				"font",
				array(
					"fontfilename" => $strFontFileName
					)
				);				
		}		

		final
		function GetVideoFilePath($strVideoFileName)
		{
			return $this->GetAssetFilePath(
				"video",
				array(
					"videofilename" => $strVideoFileName
					)
				);				
		}		


		final
		function GetFrameworkLayoutFilePath($strFramework,$strLayout)
		{
			return $this->GetAssetFilePath(
				"layout-framework",
				array(
					"layout" => $strLayout,
					"framework" => $strFramework
					),
				"layout"
				);				
		}

		final
		function GetLayoutConfigFilePath($strLayout,$strConfig = '')
		{
			return $this->GetAssetFilePath(
				"layout-config",
				array(
					'layout' => $strLayout,
					'config' => $strConfig
					),
				'layout'
				);				
		}


		final
		function GetLayoutCssFilePath($strLayout)
		{
			return $this->GetAssetFilePath(
				"layout-css",
				array(
					"layout" => $strLayout
					),
				"layout"
				);				
		}
		
		final
		function GetLayoutIncFilePath($strLayout)
		{
			return $this->GetAssetFilePath(
				"layout-inc",
				array(
					"layout" => $strLayout
					),
				"layout"
				);				
		}		

		function EnumerateElements()
		{
			$arrayElements = array();
			foreach ($this->m_arrayAssemblies as $assembly)
			{
				$arrayElements = ArrayAppend($arrayElements,$assembly->EnumerateElements());
			}
			return $arrayElements;
		}


		
	}



