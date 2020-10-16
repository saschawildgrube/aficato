<?php

	require_once(GetWDKDir()."wdk_website.inc");
	
	class CTestWebSite extends CWebSite
	{

		function __construct(
			$arrayParams
			)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = "1.0";
			$arrayMeta["author"] = "Sascha Wildgrube";
			$arrayMeta["copyright"] = "Refer to WDK Documentation for copyright information.";
			
		
			$arrayConfig = array();
			
			$arrayConfig["languages"][] = "EN";
			$arrayConfig["languages"][] = "DE";
			$arrayConfig["fallbacklanguage"] = StringUpperCase(ArrayGetValue($_GET,"fallbacklanguage"));
			$arrayConfig["protocols"] = array("http","https");
			$arrayConfig["underconstruction"] = "1";
			$arrayConfig["admin_email"] = GetAdminEmail();
			
			$arrayConfig["seourl"] = "seourl";
			
			parent::__construct(
				GetRootURL(),
				GetSourceDir(),
				GetDocumentRootDir(),
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				"website_test");
					
		}
		
		function CallbackInitRoutingMap()
		{
			parent::CallbackInitRoutingMap();
			$this->LoadRoutingMap("default");
		}
		
		
		function CallbackInit()
		{
			return parent::CallbackInit();
		}
		
		function CallbackRenderSpecialContent($strContentID)
		{
			return "";
		}

		function CallbackRenderNavigation($strOutput)
		{
		}
		

		function CallbackCheckCondition($strCondition)
		{
			return true;	
		}
		
		function CallbackPreContent()
		{
		}
		
		function CallbackPostContent()
		{
		}	
		
		
		function CallbackInitAssemblyLine(&$assemblyLine)
		{
			$assembly = new CAssembly($this,"testimages");
			$assemblyLine->AddAssembly($assembly);

			$assembly = new CAssembly($this,"testdownload");
			$assemblyLine->AddAssembly($assembly);

			$assembly = new CAssembly($this,"testjavascript");
			$assemblyLine->AddAssembly($assembly);


			parent::CallbackInitAssemblyLine($assemblyLine);
		}		
		

	}

